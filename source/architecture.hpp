// pfamannot
// Protein Family Annotator
//
// source/architecture.h
// Jan Hamalcik
//
// Set of domains
//

#include "domainComparator.hpp"

#include<set>

using setOfDomains = std::set<Domain, DomainComparator>;

class Architecture {
public:
  Architecture(const std::string& pfamID, const int& from, const int& to) {
    architecture_.emplace(pfamID, from, to);
  }

  void emplace(const std::string& pfamID, const int& from, const int& to) { architecture_.emplace(pfamID, from, to); }
  const setOfDomains& getArchitecture() const { return architecture_; }

private:

  setOfDomains architecture_;
};
