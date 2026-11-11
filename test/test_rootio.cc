#include "catch.hpp"

#include "SRothman/DataFormats/interface/EECJet.h"
#include "SRothman/EECs/interface/Outputs.h"

#include "TClass.h"
#include "TFile.h"
#include "TSystem.h"

#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

namespace {

  // Writes obj to a ROOT file through its dictionary and reads it back.
  template <class T>
  std::unique_ptr<T> roundTrip(const T& obj, const std::string& name) {
    TClass* cl = TClass::GetClass(typeid(T));
    REQUIRE(cl != nullptr);
    REQUIRE(cl->IsLoaded());
    const std::string path = std::string(gSystem->TempDirectory()) + "/srothman_df_" + name + "_" +
                             std::to_string(gSystem->GetPid()) + ".root";
    {
      TFile f(path.c_str(), "RECREATE");
      REQUIRE(f.WriteObjectAny(&obj, cl, "obj") > 0);
    }
    TFile f(path.c_str(), "READ");
    std::unique_ptr<T> back(static_cast<T*>(f.GetObjectChecked("obj", cl)));
    f.Close();
    gSystem->Unlink(path.c_str());
    REQUIRE(back != nullptr);
    return back;
  }

}  // namespace

TEST_CASE("EECJetCollection round-trips through ROOT I/O", "[rootio]") {
  srothman::EECJetCollection jets(2);
  jets[0].pt = 512.25;
  jets[0].eta = -0.75;
  jets[0].phi = 1.5;
  jets[0].mass = 80.5;
  jets[0].rawPt = 480.0;
  jets[0].srcIdx = 3;
  jets[0].jetIdBits = 0b101;
  jets[0].constituents.push_back({1.0, -2.0, 3.5, 4.25, -1, -211, 0.75f, 42});
  jets[0].constituents.push_back({0.5, 0.5, 0.0, 0.75, 0, 22, 1.0f, 7});
  jets[1].pt = 300;
  jets[1].srcIdx = 0xFFFFFFFFu;

  const auto back = roundTrip(jets, "jets");
  REQUIRE(back->size() == 2);
  const auto& j = (*back)[0];
  CHECK(j.pt == 512.25);
  CHECK(j.eta == -0.75);
  CHECK(j.phi == 1.5);
  CHECK(j.mass == 80.5);
  CHECK(j.rawPt == 480.0);
  CHECK(j.srcIdx == 3);
  CHECK(j.jetIdBits == 0b101);
  REQUIRE(j.constituents.size() == 2);
  const auto& c = j.constituents[0];
  CHECK(c.px == 1.0);
  CHECK(c.py == -2.0);
  CHECK(c.pz == 3.5);
  CHECK(c.E == 4.25);
  CHECK(c.charge == -1);
  CHECK(c.pdgId == -211);
  CHECK(c.puppiWeight == 0.75f);
  CHECK(c.srcIdx == 42);
  CHECK(j.constituents[1].pdgId == 22);
  CHECK((*back)[1].srcIdx == 0xFFFFFFFFu);
  CHECK((*back)[1].constituents.empty());
}

TEST_CASE("Binned EEC results round-trip through ROOT I/O", "[rootio]") {
  const eec::JetMeta meta{123.5, 1.5, 3};

  std::vector<eec::DenseResult> dense{{meta, {0.0, 0.25, 1.5}}};
  auto d = roundTrip(dense, "dense");
  REQUIRE(d->size() == 1);
  CHECK((*d)[0].meta.E_jet == 123.5);
  CHECK((*d)[0].meta.n == 1.5);
  CHECK((*d)[0].meta.N == 3);
  CHECK((*d)[0].values == std::vector<double>{0.0, 0.25, 1.5});

  std::vector<eec::SparseResult> sparse{{meta, {{4u, 0.5}, {9u, -0.125}}}};
  auto s = roundTrip(sparse, "sparse");
  REQUIRE((*s)[0].entries.size() == 2);
  CHECK((*s)[0].meta.N == 3);
  CHECK((*s)[0].entries[1].bin == 9u);
  CHECK((*s)[0].entries[1].weight == -0.125);

  std::vector<eec::SparseUnmergedResult> unm{{meta, {{4u, 0.5, 6u}, {4u, 0.25, 3u}}}};
  auto u = roundTrip(unm, "unmerged");
  REQUIRE((*u)[0].entries.size() == 2);
  CHECK((*u)[0].entries[0].symfactor == 6u);
  CHECK((*u)[0].entries[1].weight == 0.25);
  CHECK((*u)[0].entries[1].symfactor == 3u);
}

TEST_CASE("Unbinned EEC results round-trip through ROOT I/O", "[rootio]") {
  const eec::JetMeta meta{50.0, 1.0, 1};
  std::vector<eec::UnbinnedResult<1>> u1{{meta, {{{{0.125}}, 0.5, 2u}}}};
  auto b1 = roundTrip(u1, "unbinned1");
  REQUIRE((*b1)[0].entries.size() == 1);
  CHECK((*b1)[0].entries[0].coord[0] == 0.125);
  CHECK((*b1)[0].entries[0].symfactor == 2u);

  std::vector<eec::UnbinnedResult<2>> u2{{meta, {{{{1.25, -2.5}}, 0.75, 1u}, {{{0.5, 3.0}}, 0.25, 1u}}}};
  auto b2 = roundTrip(u2, "unbinned2");
  REQUIRE((*b2)[0].entries.size() == 2);
  CHECK((*b2)[0].entries[0].coord[0] == 1.25);
  CHECK((*b2)[0].entries[0].coord[1] == -2.5);
  CHECK((*b2)[0].entries[1].coord[1] == 3.0);
  CHECK((*b2)[0].entries[1].weight == 0.25);
  CHECK((*b2)[0].meta.E_jet == 50.0);
}
