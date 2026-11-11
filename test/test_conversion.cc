#include "catch.hpp"

#include "SRothman/DataFormats/interface/EECConversion.h"
#include "SRothman/EECs/interface/ParticleInfo.h"

#include <cmath>

using srothman::EECJet;
using srothman::EECParticle;

TEST_CASE("toPseudoJets copies momenta and attaches charge and pdgId", "[conversion]") {
  EECJet j;
  j.constituents.push_back({1.0, 2.0, 3.0, 4.0, -1, -211, 0.5f, 7});
  j.constituents.push_back({-0.5, 0.25, -2.0, 2.5, 0, 22, 1.0f, 3});
  const auto ps = srothman::toPseudoJets(j);
  REQUIRE(ps.size() == 2);
  CHECK(ps[0].px() == 1.0);
  CHECK(ps[0].py() == 2.0);
  CHECK(ps[0].pz() == 3.0);
  CHECK(ps[0].E() == 4.0);
  CHECK(ps[1].px() == -0.5);
  CHECK(ps[1].E() == 2.5);
  CHECK(eec::particleCharge(ps[0]) == -1);
  CHECK(eec::particleCharge(ps[1]) == 0);
  CHECK(ps[0].user_info<eec::EECParticleInfo>().pdgId() == -211);
  CHECK(ps[1].user_info<eec::EECParticleInfo>().pdgId() == 22);
}

TEST_CASE("toPseudoJets of an empty jet is empty", "[conversion]") {
  CHECK(srothman::toPseudoJets(EECJet{}).empty());
}

TEST_CASE("jetPseudoJet reproduces pt, eta, phi, mass", "[conversion]") {
  EECJet j;
  j.pt = 437.5;
  j.eta = -1.3;
  j.phi = 2.9;
  j.mass = 91.2;
  const auto p = srothman::jetPseudoJet(j);
  CHECK(p.pt() == Approx(j.pt).epsilon(1e-12));
  CHECK(p.pseudorapidity() == Approx(j.eta).epsilon(1e-12));
  CHECK(p.phi_std() == Approx(j.phi).epsilon(1e-12));
  CHECK(p.m() == Approx(j.mass).epsilon(1e-9));
  // Rapidity differs from pseudorapidity for a massive jet, so this distinguishes an eta/y mix-up.
  CHECK(std::abs(p.rap() - j.eta) > 1e-3);
}
