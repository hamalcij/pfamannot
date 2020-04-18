// pfamannot
// Protein Family Annotator
//
// source/flowController.cpp
// Jan Hamalcik
//
// Communicates with the user and based on user's answers controlls the flow of pfamannot
//

#include "flowController.hpp"

// Calls pfamannot's essential methods
void FlowController::run() {
  parseCommandLine();
  std::cout << std::endl;
  std::cout << "Welcome to pfamannot - Protein Family Annotator! Version " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;
  if (!parameters_.allParametersSpecified()) {
    std::cout << "Before I start parsing, you have to provide some additional information." << std::endl;
    completeParameters();
  }
  summarize();
  Parser parser(parameters_);
}

// Options and arguments are retrieved from argv_
void FlowController::parseCommandLine() {
  bool outputFileNotSpecified = true;
  bool pfamFileNotSpecified = true;
  bool uniprotFileNotSpecified = true;
  bool pfamIDNotSpecified = true;
  bool userWantsReset = false; // Additional info about reset, because system checks automatically, if reset is needed, and if user afterwords decides himself that a reset is needed, this would yield an error, that -r option has been specified more than once

  // Iterate over all arguments
  while (*++argv_) {

    // Options
    if (**argv_ == '-') {
      bool loopMayContinue = true;
      bool anOptionFollows = false;
      char previousChar = '-';

      // More options can follow after one hyphen, i.e. -qa, but some options have to be separated by a space, i.e. -q -i, which yields loopMayContinue = false
      while (loopMayContinue && *++argv_[0]) {
        anOptionFollows = true;
        char thisChar = argv_[0][0];
        switch (thisChar) {

          // Output file as a long option followed by '='
          case '-':
            loopMayContinue = false;
            if (previousChar == '-') {
              std::string actualLongOption = "--";

              while (*++argv_[0] && argv_[0][0] != '=') {
                actualLongOption += argv_[0][0];
              }

              // User is going to specify output file location
              if (actualLongOption == outputFileLongOption_) {
                if (outputFileNotSpecified) {
                  outputFileNotSpecified = false;
                  std::string outputFileLocation = readAWord();
                  checkForHomeDirectory(outputFileLocation);
                  parameters_.setOutputFileLocation(outputFileLocation);
                  testOutputFile(outputFileLocation);
                }

                // Output file already specified -> terminate
                else {
                  invalidCommandLine("Output file specified more than once.");
                }
              }

              // User is going to specify Pfam-A.full file location
              else if (actualLongOption == pfamFileLongOption_) {
                if (pfamFileNotSpecified) {
                  pfamFileNotSpecified = false;
                  std::string pfamFileLocation = readAWord();
                  checkForHomeDirectory(pfamFileLocation);
                  parameters_.setPfamAFullLocation(pfamFileLocation);
                  testInputFile(pfamFileLocation);
                }

                // Pfam-A.full file already specified -> terminate
                else {
                  invalidCommandLine("Pfam-A.full file specified more than once.");
                }
              }

              // User is going to specify uniprot_reference_proteomes.dat file location
              else if (actualLongOption == uniprotFileLongOption_) {
                if (uniprotFileNotSpecified) {
                  uniprotFileNotSpecified = false;
                  std::string uniprotFileLocation = readAWord();
                  checkForHomeDirectory(uniprotFileLocation);
                  parameters_.setUniprotReferenceProteomesLocation(uniprotFileLocation);
                  testInputFile(uniprotFileLocation);
                }

                // Output file already specified -> terminate
                else {
                  invalidCommandLine("uniprot_reference_proteomes.dat file specified more than once.");
                }
              }

              // User's long option doesn't match -> terminate
              else {
                invalidCommandLine("Long option " + actualLongOption + " is invalid.");
              }
            }

            // We have two options not separated by a space, e.g. "-a-q" -> terminate
            else {
              invalidCommandLine("New option must be separated by a space!");
            }
            break;

          // Output file - This option MUST be present and path to the output file MUST be specified right afterwords with arbitrary number of spaces separating them two
          case 'o':
            loopMayContinue = false;
            if (optionStandsAlone('o', previousChar)) {

              // Output file may be specified only once
              if (outputFileNotSpecified) {
                outputFileNotSpecified = false;

                // Check if there is something after the option
                if (*++argv_) {
                  --argv_[0];
                  std::string outputFileLocation = readAWord();
                  parameters_.setOutputFileLocation(outputFileLocation);
                  testOutputFile(outputFileLocation);
                }

                // There is nothing after the option -> terminate
                else {
                  invalidCommandLine("No output file specified.");
                }
              }

              // Output file already specified -> terminate
              else {
                invalidCommandLine("Output file specified more than once.");
              }
            }
            break;

          // Pfam-A.full file
          case 'p':
            loopMayContinue = false;
            if (optionStandsAlone('p', previousChar)) {

              // Pfam-A.full file may be specified only once
              if (pfamFileNotSpecified) {
                pfamFileNotSpecified = false;

                // Check if there is something after the option
                if (*++argv_) {
                  --argv_[0];
                  std::string pfamFileLocation = readAWord();
                  parameters_.setPfamAFullLocation(pfamFileLocation);
                  testInputFile(pfamFileLocation);
                }

                // There is nothing after the option -> terminate
                else {
                  invalidCommandLine("No Pfam-A.full file specified");
                }
              }

              // Pfam-A.full file already specified -> terminate
              else {
                invalidCommandLine("Pfam-A.full file specified more than once.");
              }
            }
            break;

          // uniprot_reference_proteomes.dat file
          case 'u':
            loopMayContinue = false;
            if (optionStandsAlone('u', previousChar)) {

              // uniprot_reference_proteomes.dat file may be specified only once
              if (uniprotFileNotSpecified) {
                uniprotFileNotSpecified = false;

                // Check if there is something after the option
                if (*++argv_) {
                  --argv_[0];
                  std::string uniprotFileLocation = readAWord();
                  parameters_.setUniprotReferenceProteomesLocation(uniprotFileLocation);
                  testInputFile(uniprotFileLocation);
                }

                // There is nothing after the option -> terminate
                else {
                  invalidCommandLine("No uniprot_reference_proteomes.dat file specified");
                }
              }

              // uniprot_reference_proteomes.dat file already specified -> terminate
              else {
                invalidCommandLine("uniprot_reference_proteomes.dat file specified more than once.");
              }
            }
            break;

          // Include - After this option, wanted pfamIDs separated by space shall be specified
          case 'i':
            loopMayContinue = false;
            if (optionStandsAlone('i', previousChar)) {
              checkIfPfamIDsMayBeSpecified(true, pfamIDNotSpecified);
            }
            break;

          // Exclude - After this option, unwanted pfamIDs separated by space shall be specified
          case 'e':
            loopMayContinue = false;
            if (optionStandsAlone('e', previousChar)) {
              checkIfPfamIDsMayBeSpecified(false, pfamIDNotSpecified);
            }
            break;

          // Reset - After the first use, pfamannot will automatically save the result of parsing Pfam-A.full file, because every time it does the same thing and could slow things down. This option enables the user to force pfamannot to start over with parsing Pfam-A.full.
          case 'r':
            if (userWantsReset) {
              invalidCommandLine("Option -r specified more than once.");
            }
            else {
              parameters_.setReset();
              userWantsReset = true;
            }
            break;

          // All - User wants all proteins annotated
          case 'a':
            if (parameters_.getUserWantsAllDomainContainingProteins()) {
              invalidCommandLine("Options -a and -d cannot be chosen together.");
            }
            else if (!pfamIDNotSpecified) {
              invalidCommandLine("You must not specify any Pfam ID, if you want to choose the option -a");
            }
            else if (parameters_.getUserWantsAllProteins()) {
              invalidCommandLine("Option -a specified more than once.");
            }
            else {
              parameters_.setUserWantsAllProteins();
            }
            break;

          // Domain - User wants all proteins containing at least one domain annotated
          case 'd':
            if (parameters_.getUserWantsAllProteins()) {
              invalidCommandLine("Options -a and -d cannot be chosen together.");
            }
            else if (!pfamIDNotSpecified) {
              invalidCommandLine("You must not specify any Pfam ID, if you want to choose the option -d");
            }
            else if (parameters_.getUserWantsAllDomainContainingProteins()) {
              invalidCommandLine("Option -d specified more than once.");
            }
            else {
              parameters_.setUserWantsAllDomainContainingProteins();
            }
            break;

          // Option isn't defined -> terminate
          default:
            invalidCommandLine("Option -" + std::string(1, argv_[0][0]) + " is invalid.");
        }
        previousChar = thisChar;
      }

      // User writes only a hyphen followed by a space -> terminate
      if (!anOptionFollows) {
        invalidCommandLine("No option specified after the hyphen ('-')");
      }
    }

    // Something else than an option -> terminate
    else {
      --argv_[0];
      std::string invalidArgument = readAWord();
      invalidCommandLine("Argument " + invalidArgument + " is invalid.");
    }
  }
}

// If user forgets to specify some crucial information in argv_, this procedure completes them
void FlowController::completeParameters() {

  // Info about which Pfam IDs shall be included in / excluded from the search is missing
  while (!parameters_.pfamIDSpecified()) {
    std::string pfamID;
    std::string line;

    // First we ask the user to specify Pfam IDs to be included in the search
    std::cout << std::endl;
    std::cout << "Write all Pfam IDs you want to include in your proteins, separate these by a space (if none, just press enter):" << std::endl;
    std::getline(std::cin, line);
    std::stringstream pfamIDsToInclude(line);
    while (pfamIDsToInclude >> pfamID) {
      if (pfamIDIsCorrect(pfamID)) {
        if (checkAgainstPfamIDsToExclude(pfamID)) {
          break;
        }
      }
      else {
        invalidCommandLine("Pfam ID " + pfamID + " is invalid.");
      }
    }

    // Then we ask the user to specify Pfam IDs to be excluded from the search
    std::cout << std::endl;
    std::cout << "Write all Pfam IDs you want to exclude in your proteins, separate these by a space (if none, just press enter):" << std::endl;
    std::getline(std::cin, line);
    std::stringstream pfamIDsToExclude(line);
    while (pfamIDsToExclude >> pfamID) {
      if (pfamIDIsCorrect(pfamID)) {
        if (checkAgainstPfamIDsToInclude(pfamID)) {
          break;
        }
      }
      else {
        invalidCommandLine("Pfam ID " + pfamID + " is invalid.");
      }
    }

    // Loop will reiterate, because the user hasn't specified any Pfam ID
    if (!parameters_.pfamIDSpecified()) {
      std::cout << "You have to specify at least one Pfam ID!" << std::endl;
    }
    else {
      break;
    }
  }

  // Location of Pfam-A.full file is missing (not needed if parsing from previous runs is saved)
  if (!parameters_.pfamFileSpecified()) {
    std::string pfamFileLocation;
    askIfFileShouldBeDownloaded(pfamAFullFTP_, pfamFileLocation, "Pfam-A.full", "90 GB");
    parameters_.setPfamAFullLocation(pfamFileLocation);
    testInputFile(pfamFileLocation);
  }

  // Location of uniprot_reference_proteomes.dat file is missing
  if (!parameters_.uniprotFileSpecified()) {
    std::string uniprotFileLocation;
    askIfFileShouldBeDownloaded(uniprotReferenceProteomesFTP_, uniprotFileLocation, "uniprot_reference_proteomes.dat", "180 GB");
    parameters_.setUniprotReferenceProteomesLocation(uniprotFileLocation);
    testInputFile(uniprotFileLocation);
  }

  // Location of output file is missing
  if (!parameters_.outputFileSpecified()) {
    std::string outputFileLocation = userWritesFilePath("output");
    parameters_.setOutputFileLocation(outputFileLocation);
    testOutputFile(outputFileLocation);
  }
}

// Summarizes all parameters and asks user to confirm these to start parsing
void FlowController::summarize() {
  printHashLine();
  std::cout << "SUMMARY:" << std::endl;
  std::cout << std::endl;
  std::cout << "pfamannot will output and annotate ";
  if (parameters_.getUserWantsAllProteins()) {
    std::cout << "all proteins from uniprot_reference_proteomes.dat" << std::endl;
  }
  else if (parameters_.getUserWantsAllDomainContainingProteins()) {
    std::cout << "all proteins with at least one domain" << std::endl;
  }
  else {
    if (!parameters_.getPfamIDsToInclude().empty()) {
      std::cout << "proteins containing following domains:" << std::endl;
      for (auto&& pfamID : parameters_.getPfamIDsToInclude()) {
        std::cout << "- " << pfamID << std::endl;
      }
      if (!parameters_.getPfamIDsToExclude().empty()) {
        std::cout << "and ";
      }
    }
    if (!parameters_.getPfamIDsToExclude().empty()) {
      std::cout << "proteins not containing following domains:" << std::endl;
      for (auto&& pfamID : parameters_.getPfamIDsToExclude()) {
        std::cout << "- " << pfamID << std::endl;
      }
    }
  }
  if (!parameters_.getReset()) {
    std::cout << "Path to Pfam-A.full file doesn't need to be specified, data from previous parsing will be used." << std::endl;
  }
  else {
    std::cout << "Pfam-A.full file is located here: " << parameters_.getPfamAFullLocation() << std::endl;
  }
  std::cout << "uniprot_reference_proteomes.dat file is located here: " << parameters_.getUniprotReferenceProteomesLocation() << std::endl;
  std::cout << "pfamannot's output will be written here: " << parameters_.getOutputFileLocation() << std::endl;

  std::cout << std::endl;
  std::cout << "pfamannot will now run with this configuration. Do you agree? (yes/no): ";
  if (userAnswersNo()) {
    invalidCommandLine("Please run pfamannot again and provide a configuration you will agree with.");
  }

  if (parameters_.getReset()) {
    std::cout << std::endl;
    std::cout << "Protein architecture will be parsed from Pfam-A.full file." << std::endl;
    std::cout << "pfamannot can save parsed data, due to which will run slower now, but faster when used again. Do you agree? (yes/no): ";
    if (userAnswersNo()) {
      parameters_.unsetSaveMapOfProteins();
    }
  }

  printHashLine();
}

// Substitutes '~' for $HOME environment variable
void FlowController::checkForHomeDirectory(std::string& path) {
  if (path[0] == '~' && (path.length() == 1 || (path.length() > 1 && path[1] == '/'))) {
    path.replace(0, 1, HOME_DIRECTORY);
  }
}

// Prints custom error message and terminates pfamannot
void FlowController::invalidCommandLine(const std::string& errorMessage) {
  std::cerr << errorMessage << std::endl;
  std::cerr << std::endl;
  std::cerr << "pfamannot will terminate..." << std::endl;
  std::exit(0);
}

// If we cannot open (create) the output file specified by the user, we have to terminate
void FlowController::testOutputFile(const std::string& location) {
  std::ofstream file(location);
  if (!file.good()) {
    invalidCommandLine("Could not open file " + location);
  }
  file.close();
}

// If we cannot open (read) the input file specified by the user or the file is empty, we have to terminate
void FlowController::testInputFile(const std::string& location) {
  std::ifstream file(location);
  if (!file.good()) {
    invalidCommandLine("Could not open file " + location);
  }
  else if (file.peek() == std::ifstream::traits_type::eof()) {
    invalidCommandLine("File " + location + " is empty.");
  }
  file.close();
}

// After option -i or -e, series of pfamIDs has to be specified. This function parses these pfamIDs following given option.
void FlowController::parsePfamIDs(const bool isOptionI) {
  bool noPfamID = true;

  // PfamIDs PFxxxxx, where x is a digit
  while (*++argv_ && **argv_ != '-') {
    noPfamID = false;
    --argv_[0];
    std::string pfamID = readAWord();

    if (pfamIDIsCorrect(pfamID)) {

      // pfamID shall be included in the search, we have to check agains pfamIDsToExclude_
      if (isOptionI) {
        if (checkAgainstPfamIDsToExclude(pfamID)) {
          break;
        }
      }

      // pfamID shall be excluded from the search, we have to check agains pfamIDsToInclude_
      else {
        if (checkAgainstPfamIDsToInclude(pfamID)) {
          break;
        }
      }
    }
    else {
      invalidCommandLine("Pfam ID " + pfamID + " is invalid!");
    }
  }

  // We moved forward when checking the condition of the while-loop above, therefore we have to move backward, so that the outer while-loop in the parseCommandLine() procedure is consistent.
  --argv_;

  // There was no text after the option
  if (noPfamID) {
    if (isOptionI) {
      invalidCommandLine("You have to specify at least one PfamID after the option -i");
    }
    else {
      invalidCommandLine("You have to specify at least one PfamID after the option -e");
    }
  }
}

// If -a or -d options were already chosen, pfam IDs cannot be specified
void FlowController::checkIfPfamIDsMayBeSpecified(const bool isOptionI, bool& pfamIDNotSpecified) {
  if (parameters_.getUserWantsAllProteins()) {
    invalidCommandLine("You must not specify any Pfam ID, if you want to choose the option -a");
  }
  else if (parameters_.getUserWantsAllDomainContainingProteins()) {
    invalidCommandLine("You must not specify any Pfam ID, if you want to choose the option -d");
  }
  else {
    pfamIDNotSpecified = false;
    parsePfamIDs(isOptionI);
  }
}

// When Pfam-A.full or uniprot_reference_proteomes.dat file is missing, we ask the user if he wants to provide the file himself, or if he wants pfamannot to download it
void FlowController::askIfFileShouldBeDownloaded(const char* ftp, std::string& location, const std::string& fileName, const std::string& freeStorage) {
  std::cout << std::endl;
  std::cout << fileName << " file is missing. Do you want pfamannot to download it? You will need at least " << freeStorage << " free storage on your disk. (yes/no): ";

  // User has file already on his machine
  if (userAnswersNo()) {
    location = userWritesFilePath(fileName);
  }

  // File will be downloaded and extracted from gz
  else {
    location = userWritesFilePath("the folder where you want to save the downloaded " + fileName);
    if (location[location.length() - 1] != '/') {
      location += '/';
    }
    location += fileName;
    Downloader downloader(ftp, (location + ".gz").c_str(), location, fileName);
  }
}

// Prints a separation line made out of hash marks
void FlowController::printHashLine() {
  std::cout << std::endl;
  std::cout << "############################################################" << std::endl;
}

// Returns true iff Pfam ID is in format PFxxxxx, where x is a digit
bool FlowController::pfamIDIsCorrect(const std::string& pfamID) {
  if (pfamID.length() != standardPfamIDLength_) {
    return false;
  }
  if (pfamID[0] != 'P') {
    return false;
  }
  if (pfamID[1] != 'F') {
    return false;
  }
  for (int i = 2; i < standardPfamIDLength_; ++i) {
    if (!isdigit(pfamID[i])) {
      return false;
    }
  }
  return true;
}

// Returns true iff Pfam ID to be inserted into pfamIDsToInclude_ is found in pfamIDsToExclude_
bool FlowController::checkAgainstPfamIDsToExclude(const std::string& pfamID) {
  auto pfamIDToCheckAgainst = parameters_.getPfamIDsToExclude().find(pfamID);

  // A valid Pfam ID cannot be in both pfamIDsToInclude_ and pfamIDsToExclude_
  if (pfamIDToCheckAgainst != parameters_.getPfamIDsToExclude().cend()) {
    invalidCommandLine("PfamID " + pfamID + " was already specified as excluded from the search.");
    return true;
  }
  else {
    parameters_.addPfamIDToInclude(pfamID);
    return false;
  }
}

// Returns true iff Pfam ID to be inserted into pfamIDsToExclude_ is found in pfamIDsToInclude_
bool FlowController::checkAgainstPfamIDsToInclude(const std::string& pfamID) {
  auto pfamIDToCheckAgainst = parameters_.getPfamIDsToInclude().find(pfamID);

  // A valid Pfam ID cannot be in both pfamIDsToInclude_ and pfamIDsToExclude_
  if (pfamIDToCheckAgainst != parameters_.getPfamIDsToInclude().cend()) {
    invalidCommandLine("PfamID " + pfamID + " was already specified as included in the search.");
    return true;
  }
  else {
    parameters_.addPfamIDToExclude(pfamID);
    return false;
  }
}

// Some options must stand alone.
// Returns false iff there is something prior or after given option.
bool FlowController::optionStandsAlone(const char& option, const char& previousChar) {
  if (*++argv_[0] || previousChar != '-') {
    invalidCommandLine("Option -" + std::string(1, option) + " must stand alone.");
    return false;
  }
  else {
    return true;
  }
}

// Returns true iff user answers no, returns false iff user answers yes
bool FlowController::userAnswersNo() {
  std::string answer;
  for (;;) {
    std::getline(std::cin, answer);
    if (answer == "no") {
      return true;
    }
    else if (answer == "yes") {
      return false;
    }
    else {
      std::cout << "Write yes or no: ";
    }
  }
}

// Returns a string continuing from current pointer to argv_[0] to the end of argv_[0]
std::string FlowController::readAWord() {
  std::string word = "";
  while (*++argv_[0]) {
    word += argv_[0][0];
  }
  return word;
}

// Returns user specified file location
std::string FlowController::userWritesFilePath(const std::string& fileName) {
  std::string fileLocation;

  std::cout << std::endl;
  std::cout << "Write full or relative path to " << fileName << " file: ";
  std::getline(std::cin, fileLocation);
  checkForHomeDirectory(fileLocation);

  return fileLocation;
}
