#include "SRothman/DataFormats/interface/EECConversion.h"
#include "SRothman/EECs/interface/ParticleInfo.h"

#include <cmath>

namespace srothman {

  std::vector<fastjet::PseudoJet> toPseudoJets(const EECJet& jet) {
    std::vector<fastjet::PseudoJet> out;
    out.reserve(jet.constituents.size());
    for (const auto& c : jet.constituents) {
      out.emplace_back(c.px, c.py, c.pz, c.E);
      out.back().set_user_info(new eec::EECParticleInfo(c.charge, c.pdgId));
    }
    return out;
  }

  fastjet::PseudoJet jetPseudoJet(const EECJet& jet) {
    const double px = jet.pt * std::cos(jet.phi);
    const double py = jet.pt * std::sin(jet.phi);
    const double pz = jet.pt * std::sinh(jet.eta);
    const double E = std::sqrt(px * px + py * py + pz * pz + jet.mass * jet.mass);
    return fastjet::PseudoJet(px, py, pz, E);
  }

}  // namespace srothman
