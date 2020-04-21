/*
MIT License

Copyright (c) 2020 Jan Hamalčík

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// pfamannot
// Protein Family Annotator
//
// source/parameters.h
// Copyright (c) 2020 Jan Hamalčík
//
// Data structure containing all information about program's parameters.
//

#include<string>
#include<set>
#include<fstream>

using setOfPfamIDs = std::set<std::string>;

class Parameters {
public:
  Parameters() { checkIfResetNeeded(); }

  bool allParametersSpecified();

  void addPfamIDToInclude(const std::string& pfamID);
  void addPfamIDToExclude(const std::string& pfamID);
  void setPfamAFullLocation(const std::string& pfamAFullLocation);
  void setUniprotReferenceProteomesLocation(const std::string& uniprotReferenceProteomesLocation);
  void setOutputFileLocation(const std::string& outputFileLocation);
  void setUserWantsAllProteins();
  void setUserWantsAllDomainContainingProteins();
  void setReset();

  void unsetSaveMapOfProteins() { saveMapOfProteins_ = false; }

  const setOfPfamIDs& getPfamIDsToInclude() const { return pfamIDsToInclude_; }
  const setOfPfamIDs& getPfamIDsToExclude() const { return pfamIDsToExclude_; }
  const std::string& getPfamAFullLocation() const { return pfamAFullLocation_; }
  const std::string& getUniprotReferenceProteomesLocation() const { return uniprotReferenceProteomesLocation_; }
  const std::string& getOutputFileLocation() const { return outputFileLocation_; }
  const bool getReset() const { return reset_; }
  const bool getSaveMapOfProteins() const { return saveMapOfProteins_; }
  const bool getUserWantsAllProteins() const { return userWantsAllProteins_; }
  const bool getUserWantsAllDomainContainingProteins() const { return userWantsAllDomainContainingProteins_; }
  const std::string& getSavedMapOfProteinsLocation() const { return savedMapOfProteinsLocation_; }

  const bool pfamIDSpecified() const { return crucialInformation_[0]; }
  const bool pfamFileSpecified() const { return crucialInformation_[1]; }
  const bool uniprotFileSpecified() const { return crucialInformation_[2]; }
  const bool outputFileSpecified() const { return crucialInformation_[3]; }

private:
  void checkIfResetNeeded();

  bool crucialInformation_[4]{
    false, // [0]: At least one Pfam ID has been specified OR Pfam ID must not be specified
    false, // [1]: Location of Pfam-A.full file is known or not needed
    false, // [2]: Location of uniprot_reference_proteomes.dat file is known
    false  // [3]: Location of output file is known
  };
  setOfPfamIDs pfamIDsToInclude_;
  setOfPfamIDs pfamIDsToExclude_;
  std::string pfamAFullLocation_{""};
  std::string uniprotReferenceProteomesLocation_{""};
  std::string outputFileLocation_{""};
  bool reset_{false};
  bool saveMapOfProteins_{true};
  bool userWantsAllProteins_{false};
  bool userWantsAllDomainContainingProteins_{false};

  const std::string savedMapOfProteinsLocation_{"/usr/local/share/.pfamannot_parsed_pfam"};

};
