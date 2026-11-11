# SRothman/DataFormats

EDM data formats for the boostedV EEC workflow (CMSSW package `SRothman/DataFormats`).

- `interface/EECJet.h`: `srothman::EECParticle`, `srothman::EECJet`, `srothman::EECJetCollection` — the
  jets (with their constituents) passed between the JetSetup and EECs plugins.
- `interface/EECConversion.h`: `toPseudoJets(jet)` (constituents as `fastjet::PseudoJet` carrying
  `eec::EECParticleInfo(charge, pdgId)`, as the EEC calculators expect) and `jetPseudoJet(jet)`.
- `src/classes_def.xml`: dictionaries for the above and for the `SRothman/EECs` result types
  (`std::vector<eec::{Dense,Sparse,SparseUnmerged}Result>`, `std::vector<eec::UnbinnedResult<1|2>>`), with
  `edm::Wrapper`s. EEC result vectors are index-aligned with the `EECJetCollection` they were computed from.

Build and test from `$CMSSW_BASE/src`: `scram b -j8 && scram b runtests`.
