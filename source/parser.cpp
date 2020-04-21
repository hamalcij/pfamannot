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
// source/parser.cpp
// Copyright (c) 2020 Jan Hamalčík
//
// Parses files Pfam-A.full and uniprot_reference_proteomes.dat and creates output consisting of annotated proteins containing domains from user input
//

#include "parser.hpp"

// Runs parsing
void Parser::run() {
  static ParserWrapper& wrapper = ParserWrapper::getInstance();
  wrapper.set(this);

  std::signal(SIGINT, signalHandler);

  // User wants to parse Pfam-A.full again
  if (parameters_.getReset()) {
    parsePfamAFull();
    if (parameters_.getSaveMapOfProteins()) {
      saveMapOfProteins();
    }
  }

  // User wants to use saved data from past paring
  else {
    loadSavedMapOfProteins();
  }

  // User has specified domains to be included in / excluded from the search
  if (!parameters_.getUserWantsAllProteins() && !parameters_.getUserWantsAllDomainContainingProteins()) {
    deleteProteinsNotOfInterest();
  }

  parseUniprotReferenceProteomes();
}

// Parses Pfam-A.full file based on user preferences
void Parser::parsePfamAFull() {
  std::string currentPfamID = "";
  std::ifstream pfamFile(parameters_.getPfamAFullLocation());
  char buffer[maxBufferSize_];

  // Pfam-A.full file is being read line by line
  // Careful!
  // Due to maxBufferSize_, one long line in text may theoretically be split into more than one lines while reading
  // But it shouldn't be the case, at this moment (19.3.2020) the longest line in Pfam-A.full is 8119 characters long
  if (pfamFile.good()) {
    std::cout << std::endl;
    std::cout << currentTime() << "   Parsing Pfam-A.full file located at " << parameters_.getPfamAFullLocation() << std::endl;
    std::cout << currentTime() << "   This will take a couple of minutes." << std::endl;
    std::cout << currentTime() << "   Have a break, read some scientific paper, help out a colleague..." << std::endl;

    // Each line of Pfam-A.full is read
    for (;;) {
      pfamFile.getline(buffer, maxBufferSize_);
      if (pfamFile.fail()) break;

      // We're at the end of one pfam entry
      if (buffer[0] == '/') {
        currentPfamID = "";
      }

      // We're within a pfam entry
      else if (buffer[0] == '#') {

        // Security check
        if (buffer[1] == '=') {

          // Lines beginning with "#=GF" contain various information, including PfamID
          if (buffer[3] == 'F') {

            // Lines beginning with "#=GF AC" contain PfamID on position pfamIDOffset_
            if (buffer[5] == 'A' && buffer[6] == 'C') {
              int i = 0;
              while (buffer[i + pfamIDOffset_] && i < standardPfamIDLength_) {
                currentPfamID += buffer[i + pfamIDOffset_];
                ++i;
              }
            }
          }

          // Lines beginning with "#=GS" contain information about proteins, which contain currentPfamID at which position within their sequence
          else if (buffer[3] == 'S') {
            addProteinToDomainMapping(currentPfamID, buffer);
          }
        }
      }

      // Lines can only begin with '#', '/', uppercase letter or a digit
      else if (!isupper(buffer[0]) && !isdigit(buffer[0])) {
        std::cerr << buffer << std::endl;
        failure("Pfam-A.full file located at " + parameters_.getPfamAFullLocation() + " is corrupted!");
      }
    }
    std::cout << std::endl;
    std::cout << currentTime() << "   Done parsing Pfam-A.full fule located at " << parameters_.getPfamAFullLocation() << std::endl;
  }

  // Pfam-A.full file opening failed
  else {
    failure("Could not open Pfam-A.full file located at " + parameters_.getPfamAFullLocation());
  }

  pfamFile.close();
}

// proteins_ will be stored to a system file so that pfamannot runs faster next time. This system file has an exact structure and compared to Pfam-A.full file does not contain any unnecessary data
void Parser::saveMapOfProteins() {
  std::ofstream savedMapOfProteins(parameters_.getSavedMapOfProteinsLocation());
  if (savedMapOfProteins.good()) {
    std::cout << std::endl;
    std::cout << currentTime() << "   Saving parsed data from Pfam-A.full file to run faster next time." << std::endl;

    savedMapOfProteinsIsBeingSaved_ = true;
    for (auto protein = proteins_.cbegin(); protein != proteins_.cend(); ++protein) {

      // First line of file entry begins with '#' after which an Uniprot ID follows
      savedMapOfProteins << '#' << protein->first << ' ' << std::endl;

      bool isFirstDomain = true;
      for (auto domain = protein->second.getArchitecture().cbegin(); domain != protein->second.getArchitecture().cend(); ++domain) {

        // Following lines begin with '@' ('*' for first domain) after which a Pfam ID follows, followed by amino acid range in the current sequence, separated by a space
        if (isFirstDomain) {
          savedMapOfProteins << '*';
          isFirstDomain = false;
        }
        else {
          savedMapOfProteins << '@';
        }
        savedMapOfProteins << domain->getDomain();
        savedMapOfProteins << ' ' << domain->getFrom();
        savedMapOfProteins << ' ' << domain->getTo() << ' ' << std::endl;
      }

      // Each entry is terminated by '/'
      savedMapOfProteins << '/' << std::endl;
    }
    savedMapOfProteinsIsBeingSaved_ = false;
  }
  else {
    std::cout << std::endl;
    std::cout << currentTime() << "   Could not save parsed data..." << std::endl;
  }
}

// proteins_ will be restored from a system file created from past parsing. It will be much faster than restoring proteins_ from Pfam-A.full file
void Parser::loadSavedMapOfProteins() {
  char buffer[maxBufferSize_];
  std::ifstream savedMapOfProteins(parameters_.getSavedMapOfProteinsLocation());

  if (savedMapOfProteins.good()) {
    std::cout << std::endl;
    std::cout << currentTime() << "   Retrieving saved data from past parsing." << std::endl;

    std::pair<mapOfProteins::iterator, bool> currentProtein;
    std::string currentUniprotID = "";
    for (;;) {
      savedMapOfProteins.getline(buffer, maxBufferSize_);
      if (savedMapOfProteins.fail()) break;

      // We're at the end of one protein entry
      if (buffer[0] == '/') {
        currentUniprotID = "";
      }

      // Lines beginning with '#' contain uniprotID from position 1 until the first space
      else if (buffer[0] == '#') {
        int i = 1;
        while (buffer[i] != ' ') {
          currentUniprotID += buffer[i];
          ++i;
        }
      }

      // Other lines contain info about domains
      else {
        std::string currentPfamID = "";
        std::string from = "";
        std::string to = "";

        int i = 1;
        while (buffer[i] != ' ') {
          currentPfamID += buffer[i];
          ++i;
        }
        ++i;
        while (buffer[i] != ' ') {
          from += buffer[i];
          ++i;
        }
        int currentFrom = std::stoi(from);
        ++i;
        while (buffer[i] != ' ') {
          to += buffer[i];
          ++i;
        }
        int currentTo = std::stoi(to);

        // Lines beginning with '*' contain the first domain
        if (buffer[0] == '*') {
          currentProtein = proteins_.emplace(currentUniprotID, Architecture(currentPfamID, currentFrom, currentTo));
        }

        // Lines beginning with '@' contain other domains
        else {
          currentProtein.first->second.emplace(currentPfamID, currentFrom, currentTo);
        }
      }
    }
  }
  else {
    failure("File containing saved parsed data is corrupted.");
  }
}

// Parses uniprot_reference_proteomes.dat
void Parser::parseUniprotReferenceProteomes() {
  std::ifstream uniprotFile(parameters_.getUniprotReferenceProteomesLocation());
  std::ofstream outputFile(parameters_.getOutputFileLocation());

  // uniprot_reference_proteomes.dat file is being read line by line
  // Careful!
  // Due to maxBufferSize_, one long line in text may theoretically be split into more than one lines while reading
  // But due to the file's structure with restricted line length, this shouldn't be the case
  if (uniprotFile.good()) {
    if (outputFile.good()) {
      std::cout << std::endl;
      std::cout << currentTime() << "   Parsing uniprot_reference_proteomes.dat file located at " << parameters_.getUniprotReferenceProteomesLocation() << std::endl;
      std::cout << currentTime() << "   This will take a couple of minutes." << std::endl;
      std::cout << currentTime() << "   Go for lunch, review your code, grade your students' tests..." << std::endl;

      // uniprot_reference_proteomes.dat file contains also proteins without any domains, choosing -a option, the user wants to get these proteins too
      if (parameters_.getUserWantsAllProteins()) {
        chooseAlsoProteinsWithoutDomains(uniprotFile, outputFile);
      }

      // Only proteins from proteins_ will be considered
      // Happens either when userWantsAllDomainContainingProteins_ or when user lists any pfamID
      else {
        chooseOnlyProteinsWithDomains(uniprotFile, outputFile);
      }

      std::cout << std::endl;
      std::cout << currentTime() << "   Done parsing uniprot_reference_proteomes.dat file located at " << parameters_.getUniprotReferenceProteomesLocation() << std::endl;
      std::cout << currentTime() << "   Go ahead and find your results here: " << parameters_.getOutputFileLocation() << std::endl;
    }

    // Output file failed
    else {
      failure("Could not open output file located at " + parameters_.getOutputFileLocation());
    }

  }

  // uniprot_reference_proteomes.dat file failed
  else {
    failure("Could not open uniprot_reference_proteomes.dat file located at " + parameters_.getUniprotReferenceProteomesLocation());
  }

  uniprotFile.close();
  outputFile.close();
}

// Parses from uniprot_reference_proteomes.dat only those proteins that are contained in proteins_
void Parser::chooseOnlyProteinsWithDomains(std::ifstream& uniprotFile, std::ofstream& outputFile) {
  bool isUniprotEntryOfInterest = false;
  bool isAboutToProccessSequence = false;
  std::string currentUniprotID = "";
  auto protein = proteins_.cend();
  char buffer[maxBufferSize_];

  for (;;) {
    uniprotFile.getline(buffer, maxBufferSize_);
    if (uniprotFile.fail()) break;

    // We're at the end of one uniprot entry
    if (buffer[0] == '/') {
      if (isUniprotEntryOfInterest) {
        outputFile << std::endl;
      }
      isUniprotEntryOfInterest = false;
      isAboutToProccessSequence = false;
      currentUniprotID = "";
    }

    // We're at the beginning a uniprot entry and we need to determine, whether this entry is of our interest
    else if (buffer[0] == 'I' && buffer[1] == 'D') {
      int i = 0;
      while (!isspace(buffer[i + uniprotOffset_])) {
        currentUniprotID += buffer[i + uniprotOffset_];
        ++i;
      }
      protein = proteins_.find(currentUniprotID);
      if (protein != proteins_.cend()) {
        isUniprotEntryOfInterest = true;
        outputFile << currentUniprotID << "\t";
      }
    }

    // We're within a uniprot entry of our interest
    else if (isUniprotEntryOfInterest) {

      // Lines beginning with "OC" contain information about organism and can span multiple consecutive lines
      if (buffer[0] == 'O' && buffer[1] == 'C') {
        readUniprotLineToEndAndOutputWithoutSpace(outputFile, buffer);
      }

      // Lines beginning with spaces contain protein sequence and can span multiple consecutive lines
      else if (buffer[0] == ' ') {

        // Prior to sequence, architecture should be written to outputFile
        if (!isAboutToProccessSequence) {
          outputFile << "\t";
          for (auto domain = protein->second.getArchitecture().cbegin(); domain != protein->second.getArchitecture().cend(); ++domain) {
            outputFile << domain->getDomain() << "\t" << domain->getFrom() << "\t" << domain->getTo() << "\t";
          }
          isAboutToProccessSequence = true;
         }
        readUniprotLineToEndAndOutputWithoutSpace(outputFile, buffer);
      }

      // Lines can only begin with '/', ' ' or an uppercase letter
      else if (!isupper(buffer[0])) {
        failure("uniprot_reference_proteomes.dat file located at " + parameters_.getUniprotReferenceProteomesLocation() + " is corrupted!");
      }
    }

    // Lines can only begin with '/', ' ' or an uppercase letter
    else if (!isupper(buffer[0]) && buffer[0] != ' ') {
      failure("uniprot_reference_proteomes.dat file located at " + parameters_.getUniprotReferenceProteomesLocation() + " is corrupted!");
    }
  }
}

// Parses from uniprot_reference_proteomes.dat not only those proteins that are contained in proteins_ but also other proteins without domains
void Parser::chooseAlsoProteinsWithoutDomains(std::ifstream& uniprotFile, std::ofstream& outputFile) {
  bool isAboutToProccessSequence = false;
  std::string currentUniprotID = "";
  auto protein = proteins_.cend();
  char buffer[maxBufferSize_];

  for (;;) {
    uniprotFile.getline(buffer, maxBufferSize_);
    if (uniprotFile.fail()) break;

    // We're at the end of one uniprot entry
    if (buffer[0] == '/') {
      outputFile << std::endl;
      isAboutToProccessSequence = false;
      currentUniprotID = "";
    }

    // We're at the beginning a uniprot entry
    else if (buffer[0] == 'I' && buffer[1] == 'D') {
      int i = 0;
      while (!isspace(buffer[i + uniprotOffset_])) {
        currentUniprotID += buffer[i + uniprotOffset_];
        ++i;
      }
      protein = proteins_.find(currentUniprotID);
      outputFile << currentUniprotID << "\t";
    }

    // Lines beginning with "OC" contain information about organism and can span multiple consecutive lines
    else if (buffer[0] == 'O' && buffer[1] == 'C') {
      readUniprotLineToEndAndOutputWithoutSpace(outputFile, buffer);
    }

    // Lines beginning with spaces contain protein sequence and can span multiple consecutive lines
    else if (buffer[0] == ' ') {

      // Prior to sequence, architecture should be written to outputFile, if exists
      if (!isAboutToProccessSequence) {
        outputFile << "\t";
        if (protein != proteins_.cend()) {
          for (auto domain = protein->second.getArchitecture().cbegin(); domain != protein->second.getArchitecture().cend(); ++domain) {
            outputFile << domain->getDomain() << "\t" << domain->getFrom() << "\t" << domain->getTo() << "\t";
          }
        }
        isAboutToProccessSequence = true;
      }
      readUniprotLineToEndAndOutputWithoutSpace(outputFile, buffer);
    }

    // Lines can only begin with '/', ' ' or an uppercase letter
    else if (!isupper(buffer[0])) {
      failure("uniprot_reference_proteomes.dat file located at " + parameters_.getUniprotReferenceProteomesLocation() + " is corrupted!");
    }
  }
}

// Goes through proteins_ and deletes those that the user does not want.
void Parser::deleteProteinsNotOfInterest() {
  unsigned long numberOfDeleted = 0;
  deleteProteinsWithoutDomainsToBeIncluded(numberOfDeleted);

  // All proteins were deleted in previous procedure
  if (proteins_.empty()) {
    failure("No protein containing domains " + getPfamIDsInString(parameters_.getPfamIDsToInclude()) + "exists.");
  }
  else {
    deleteProteinsWithDomainsToBeExcluded(numberOfDeleted);

    // All proteins were deleted in previous procedure
    if (proteins_.empty()) {
      std::cout << std::endl;

      // User has only specified Pfam IDs to be excluded from the search
      if (parameters_.getPfamIDsToInclude().empty()) {
        failure("No protein not containing any of domains " + getPfamIDsInString(parameters_.getPfamIDsToExclude()) + "exists.");
      }

      // User has specified both Pfam IDs to be included in the search and Pfam IDs to be excluded from the search
      else {
        failure("No proteins containing domains " + getPfamIDsInString(parameters_.getPfamIDsToInclude()) + "and at the same time not containing any of domain " + getPfamIDsInString(parameters_.getPfamIDsToExclude()) + "exists.");
      }
    }
  }

  std::cout << currentTime() << "   " << numberOfDeleted << " proteins removed." << std::endl;
}

// Keeps only those proteins, that contain all domains from pfamIDsToInclude_
void Parser::deleteProteinsWithoutDomainsToBeIncluded(unsigned long& numberOfDeleted) {
  if (!parameters_.getPfamIDsToInclude().empty()) {
    std::cout << std::endl;
    std::cout << currentTime() << "   Removing proteins not containing domains";
    printPfamIDs(parameters_.getPfamIDsToInclude());
    std::cout << std::endl;

    int numberOfDomainsToBeIncluded = parameters_.getPfamIDsToInclude().size();

    for (auto protein = proteins_.cbegin(); protein != proteins_.cend();) {
      bool proteinShallBeDeleted = false;

      // If protein contains at least the same number of domains as those that should be included in the search, it makes sense to check, if these domains do match
      if (protein->second.getArchitecture().size() >= numberOfDomainsToBeIncluded) {
        for (auto pfamID = parameters_.getPfamIDsToInclude().cbegin(); pfamID != parameters_.getPfamIDsToInclude().cend(); ++pfamID) {
          bool pfamIDHasBeenFound = false;
          for (auto domain = protein->second.getArchitecture().cbegin(); domain != protein->second.getArchitecture().cend(); ++domain) {
            if (domain->getDomain() == *pfamID) {
              pfamIDHasBeenFound = true;
              break;
            }
          }

          // Protein does not include at least one of the Pfam IDs to be included in the search, therefore shall be deleted
          if (!pfamIDHasBeenFound) {
            proteinShallBeDeleted = true;
            break;
          }
        }
      }

      // Protein has less domains than those required to be contained in the protein, therefore shall be deleted
      else {
        proteinShallBeDeleted = true;
      }
      if (proteinShallBeDeleted) {
        proteins_.erase(protein++);
        ++numberOfDeleted;
      }
      else ++protein;
    }
  }
}

// Keeps only those proteins that do not contain domains from pfamIDsToExclude_
void Parser::deleteProteinsWithDomainsToBeExcluded(unsigned long& numberOfDeleted) {
  if (!parameters_.getPfamIDsToExclude().empty()) {
    std::cout << std::endl;
    std::cout << currentTime() << "   Removing proteins containing domains";
    printPfamIDs(parameters_.getPfamIDsToExclude());
    std::cout << std::endl;

    for (auto protein = proteins_.cbegin(); protein != proteins_.cend();) {
      bool proteinShallBeDeleted = false;
      for (auto domain = protein->second.getArchitecture().cbegin(); domain != protein->second.getArchitecture().cend(); ++domain) {
        for (auto pfamID = parameters_.getPfamIDsToExclude().cbegin(); pfamID != parameters_.getPfamIDsToExclude().cend(); ++pfamID) {
          if (domain->getDomain() == *pfamID) {
            proteinShallBeDeleted = true;
            break;
          }
        }
        if (proteinShallBeDeleted == true) break;
      }
      if (proteinShallBeDeleted) {
        proteins_.erase(protein++);
        ++numberOfDeleted;
      }
      else ++protein;
    }
  }
}

// Lines beginning with "#=GS" contain information about proteins, which contain currentPfamID at which position within their sequence
// Adds a protein from such a line together with information about sequence span of given domain to proteins_
void Parser::addProteinToDomainMapping(const std::string& pfamID, const char* line) {
  int i = 0;

  // Reads Uniprot ID
  std::string uniprotID = "";
  while (line[i + pfamProteinOffset_] != '/') {
    uniprotID += line[i + pfamProteinOffset_];
    ++i;
  }
  ++i;

  // Reads sequence position, where current domain begins within the protein designed by current Uniprot ID
  std::string fromAsString = "";
  while (line[i + pfamProteinOffset_] != '-') {
    fromAsString += line[i + pfamProteinOffset_];
    ++i;
  }
  int from = std::stoi(fromAsString);
  ++i;

  // Reads sequence position, where current domain ends within the protein designed by current Uniprot ID
  std::string toAsString = "";
  while (!isspace(line[i + pfamProteinOffset_])) {
    toAsString += line[i + pfamProteinOffset_];
    ++i;
  }
  int to = std::stoi(toAsString);

  auto protein = proteins_.find(uniprotID);

  // If proteins_ already contain current Uniprot ID, we only add a new domain to it
  if (protein != proteins_.end()) {
    protein->second.emplace(pfamID, from, to);
  }

  // Protein with current Uniprot ID has not been inserted into proteins_ yet, so we insert it with its domain
  else {
    proteins_.emplace(uniprotID, Architecture(pfamID, from, to));
  }
}

// Sequence in uniprot_reference_proteomes.dat file is split into multiple consecutive lines
// On one line, there are at most 6 10-mers separated by a space
void Parser::readUniprotLineToEndAndOutputWithoutSpace(std::ofstream& outputFile, const char* line) {
  int i = 0;
  while (line[i + uniprotOffset_]) {
    if (!isspace(line[i + uniprotOffset_])) {
      outputFile << line[i + uniprotOffset_];
    }
    ++i;
  }
}

// Prints all Pfam IDs from a set of strings
void Parser::printPfamIDs(const setOfPfamIDs& pfamIDs) {
  for (auto&& pfamID : pfamIDs) {
    std::cout << " " << pfamID;
  }
}

// Prints custom error message and terminates pfamannot
void Parser::failure(const std::string& errorMessage) {
  std::cerr << errorMessage << std::endl;
  std::cerr << std::endl;
  std::cerr << "pfamannot will terminate..." << std::endl;
  std::exit(0);
}

// Returns string containing all Pfam IDs from setOfPfamIDs separated by a space
const std::string Parser::getPfamIDsInString(const setOfPfamIDs& pfamIDs) const {
  std::ostringstream stream;
  std::copy(pfamIDs.cbegin(), pfamIDs.cend(), std::ostream_iterator<std::string>(stream, " "));
  return stream.str();
}

// Returns string with current Time in hh:mm:ss format
std::string Parser::currentTime() {
  std::time_t time = std::time(0);
  std::tm* now = std::localtime(&time);

  std::string hour = std::to_string(now->tm_hour);
  std::string minute = std::to_string(now->tm_min);
  std::string second = std::to_string(now->tm_sec);

  oneDigitTimeToTwoDigitTime(hour);
  oneDigitTimeToTwoDigitTime(minute);
  oneDigitTimeToTwoDigitTime(second);

  return hour + ":" + minute + ":" + second;
}

// Converts one-digit time unit to dwo-tigit time unit by adding a zero
void Parser::oneDigitTimeToTwoDigitTime(std::string& timeUnit) {
  if (timeUnit.length() == 1) {
    timeUnit = "0" + timeUnit;
  }
}

// If pfamannot is terminated by a termination signal, e.g. CTRL+C, this will delete possible incomplete system file of stored proteins_
void Parser::signalHandler(int signum) {

  // Enables access to a specific instance of Parser
  static ParserWrapper& wrapper = ParserWrapper::getInstance();

  if (wrapper.get()->savedMapOfProteinsIsBeingSaved_) {
    std::cerr << "Interrupt signal " << signum << " recieved." << std::endl;
    std::cerr << "Deleting incompletely created parsed data from Pfam-A.full file." << std::endl;

    int status = std::remove(wrapper.get()->parameters_.getSavedMapOfProteinsLocation().c_str());
    if (status != 0) {
      std::cerr << "FATAL FAILURE! Run pfamannot again with -r option!" << std::endl;
    }
  }

  exit(signum);
}
