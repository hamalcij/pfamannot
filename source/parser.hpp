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
// source/parser.h
// Copyright (c) 2020 Jan Hamalčík
//
// Parses files Pfam-A.full and uniprot_reference_proteomes.dat and creates output consisting of annotated proteins containing domains from user input
//

#include "architecture.hpp"
#include "parameters.hpp"

#include<iostream>
#include<unordered_map>
#include<cstdlib>
#include<cctype>
#include<algorithm>
#include<iterator>
#include<sstream>
#include<csignal>
#include<ctime>

using mapOfProteins = std::unordered_map<std::string, Architecture>;

class Parser;

// Singleton wrapper allowing to handle the right instance for signal handling
// https://stackoverflow.com/a/37861144/12481671
class ParserWrapper {
public:
  ParserWrapper() = default;

  static ParserWrapper& getInstance() {
    static ParserWrapper* w = new ParserWrapper();
    return *w;
  }

  void set(Parser* parser) {
    parser_ = parser;
  }

  Parser* get() {
    return parser_;
  }

private:
  Parser* parser_;
};

//
class Parser {
public:
  Parser(const Parameters& parameters) : parameters_(parameters) { run(); }

private:
  void run();
  void parsePfamAFull();
  void saveMapOfProteins();
  void loadSavedMapOfProteins();
  void parseUniprotReferenceProteomes();
  void chooseOnlyProteinsWithDomains(std::ifstream& uniprotFile, std::ofstream& outputFile);
  void chooseAlsoProteinsWithoutDomains(std::ifstream& uniprotFile, std::ofstream& outputFile);
  void deleteProteinsNotOfInterest();
  void deleteProteinsWithoutDomainsToBeIncluded(unsigned long& numberOfDeleted);
  void deleteProteinsWithDomainsToBeExcluded(unsigned long& numberOfDeleted);
  void addProteinToDomainMapping(const std::string& pfamID, const char* line);
  void readUniprotLineToEndAndOutputWithoutSpace(std::ofstream& outputFile, const char* line);
  void printPfamIDs(const setOfPfamIDs& pfamIDs);
  void failure(const std::string& errorMessage);
  const std::string getPfamIDsInString(const setOfPfamIDs& pfamIDs) const;

  static std::string currentTime();
  static void oneDigitTimeToTwoDigitTime(std::string& timeUnit);
  static void signalHandler(int signum);

  mapOfProteins proteins_;

  bool savedMapOfProteinsIsBeingSaved_{false};

  const Parameters& parameters_;

  static const int standardPfamIDLength_{7};
  static const int pfamIDOffset_{10};
  static const int pfamProteinOffset_{5};
  static const int uniprotOffset_{5};
  static constexpr std::size_t maxBufferSize_{1024*1024};
};
