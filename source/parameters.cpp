// pfamannot
// Protein Family Annotator
//
// source/parameters.cpp
// Copyright (c) 2020 Jan Hamalčík
//
// Data structure containing all information about program's parameters.
//

#include "parameters.hpp"

// If file at savedMapOfProteinsLocation_ cannot be opened, pfamannot has to perform a reset - parse Pfam-A.full file again
void Parameters::checkIfResetNeeded() {
  std::ifstream savedMapOfProteins(savedMapOfProteinsLocation_);
  if (savedMapOfProteins.good()) {

    // File exists but is for some reason empty
    if (savedMapOfProteins.peek() == std::ifstream::traits_type::eof()) {
      reset_ = true;
    }
    else {
      crucialInformation_[1] = true;
    }
  }
  else {
    reset_ = true;
  }
  savedMapOfProteins.close();
}

// Checks if all information that is crucial for pfamannot has been specified by the user
bool Parameters::allParametersSpecified() {
  for (auto& info : crucialInformation_) {
    if (!info) {
      return false;
    }
  }
  return true;
}

// Emplaces a Pfam ID into pfamIDsToInclude_ and updates crucialInformation_
void Parameters::addPfamIDToInclude(const std::string& pfamID) {
  pfamIDsToInclude_.emplace(pfamID);
  crucialInformation_[0] = true;
}

// Emplaces a Pfam ID into pfamIDsToExclude_ and updates crucialInformation_
void Parameters::addPfamIDToExclude(const std::string& pfamID) {
  pfamIDsToExclude_.emplace(pfamID);
  crucialInformation_[0] = true;
}

// Sets location of Pfam-A.full file and updates crucialInformation_
void Parameters::setPfamAFullLocation(const std::string& pfamAFullLocation) {
  pfamAFullLocation_ = pfamAFullLocation;
  crucialInformation_[1] = true;
}

// Sets location of uniprot_reference_proteomes.dat file and updates crucialInformation_
void Parameters::setUniprotReferenceProteomesLocation(const std::string& uniprotReferenceProteomesLocation) {
  uniprotReferenceProteomesLocation_ = uniprotReferenceProteomesLocation;
  crucialInformation_[2] = true;
}

// Sets location of output file and updates crucialInformation_
void Parameters::setOutputFileLocation(const std::string& outputFileLocation) {
  outputFileLocation_ = outputFileLocation;
  crucialInformation_[3] = true;
}

// Updates crucialInformation_ as user wants to annotate all proteins from uniprot_reference_proteomes.dat
void Parameters::setUserWantsAllProteins() {
  userWantsAllProteins_ = true;
  crucialInformation_[0] = true;
}

// Updates crucialInformation_ as user wants to annotate all proteins containing at least one domain
void Parameters::setUserWantsAllDomainContainingProteins() {
  userWantsAllDomainContainingProteins_ = true;
  crucialInformation_[0] = true;
}

// When user wants to parse from Pfam-A.full file, e.g. reset saved parsed data, pfamAFullLocation_ must be specified
void Parameters::setReset() {
  reset_ = true;
  if (pfamAFullLocation_.empty()) {
    crucialInformation_[1] = false;
  }
}
