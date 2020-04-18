// pfamannot
// Protein Family Annotator
//
// source/domainComparator.h
// Jan Hamalcik
//
// Functor comparing two domains based on their position within protein's sequence
//

#include "domain.hpp"

struct DomainComparator {
public:
  bool operator()(const Domain& x, const Domain& y) const { return x.getFrom() < y.getFrom(); }

};
