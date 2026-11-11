#ifndef SRothman_DataFormats_EECConversion_h
#define SRothman_DataFormats_EECConversion_h

#include "SRothman/DataFormats/interface/EECJet.h"

#include <fastjet/PseudoJet.hh>

#include <vector>

namespace srothman {

  // One PseudoJet per constituent, each carrying an eec::EECParticleInfo(charge, pdgId).
  std::vector<fastjet::PseudoJet> toPseudoJets(const EECJet& jet);

  // The jet four-momentum from (pt, eta, phi, mass).
  fastjet::PseudoJet jetPseudoJet(const EECJet& jet);

}  // namespace srothman

#endif
