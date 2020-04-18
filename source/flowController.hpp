// pfamannot
// Protein Family Annotator
//
// source/flowController.h
// Jan Hamalcik
//
// Communicates with the user and based on user's answers controlls the flow of pfamannot
//

#include "parser.hpp"
#include "downloader.hpp"
#include "pfamannotConfig.hpp"

class FlowController {
public:
  FlowController(char** argv) : argv_(argv) { run(); }

private:
  void run();
  void parseCommandLine();
  void completeParameters();
  void summarize();
  void checkForHomeDirectory(std::string& path);
  void invalidCommandLine(const std::string& errorMessage);
  void testOutputFile(const std::string& location);
  void testInputFile(const std::string& location);
  void parsePfamIDs(const bool isOptionI);
  void checkIfPfamIDsMayBeSpecified(const bool isOptionI, bool& pfamIDNotSpecified);
  void askIfFileShouldBeDownloaded(const char* ftp, std::string& location, const std::string& fileName, const std::string& freeStorage);
  void printHashLine();
  bool pfamIDIsCorrect(const std::string& pfamID);
  bool checkAgainstPfamIDsToExclude(const std::string& pfamID);
  bool checkAgainstPfamIDsToInclude(const std::string& pfamID);
  bool optionStandsAlone(const char& option, const char& previousChar);
  bool userAnswersNo();
  std::string readAWord();
  std::string userWritesFilePath(const std::string& fileName);

  char** argv_;
  Parameters parameters_;

  const std::string pf_{"PF"};
  const std::string outputFileLongOption_{"--output-file"};
  const std::string pfamFileLongOption_{"--pfam-file"};
  const std::string uniprotFileLongOption_{"--uniprot-file"};
  const char* pfamAFullFTP_{"ftp://ftp.ebi.ac.uk/pub/databases/Pfam/current_release/Pfam-A.full.gz"};
  const char* uniprotReferenceProteomesFTP_{"ftp://ftp.ebi.ac.uk/pub/databases/Pfam/current_release/uniprot_reference_proteomes.dat.gz"};
  
  static const int standardPfamIDLength_{7};
};
