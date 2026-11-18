#ifndef SRothman_DataFormats_EECJet_h
#define SRothman_DataFormats_EECJet_h

#include <cstdint>
#include <vector>

namespace srothman {

  struct EECParticle {
    double px = 0, py = 0, pz = 0, E = 0;
    std::int8_t charge = 0;
    std::int32_t pdgId = 0;
    float puppiWeight = 1;
    // Index in the source candidate collection (packedPFCandidates or packedGenParticles).
    std::uint32_t srcIdx = 0;
  };

  // An alternative jet axis, as a four-vector.
  struct EECAxis {
    double px = 0, py = 0, pz = 0, E = 0;
    // Meaning set by the producing axis type; 0 is the regular case.
    std::uint8_t status = 0;
  };

  struct EECJet {
    // Corrected (after any jet-level variation) kinematics.
    double pt = 0, eta = 0, phi = 0, mass = 0;
    double rawPt = 0;
    // Index in the source jet collection.
    std::uint32_t srcIdx = 0;
    std::uint8_t jetIdBits = 0;
    // Product of the JER smearing factors applied to the four-momentum (1 without smearing).
    double jerFactor = 1;
    std::vector<EECParticle> constituents;
    // In the order of the axis list of the EECJetAxisProducer that filled them; empty if none ran.
    std::vector<EECAxis> axes;
  };

  using EECJetCollection = std::vector<EECJet>;

}  // namespace srothman

#endif
