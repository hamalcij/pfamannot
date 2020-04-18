// pfamannot
// Protein Family Annotator
//
// source/domain.h
// Copyright (c) 2020 Jan Hamalčík
//
// Object containing information about protein's domain: PfamID and interval in protein's sequence.
//

#include<string>

class Domain {
public:
  Domain(const std::string domain, const int from, const int to) : domain_(domain), from_(from), to_(to) {}

  const std::string& getDomain() const { return domain_; }
  const int& getFrom() const { return from_; }
  const int& getTo() const { return to_; }

private:

  const std::string domain_;
  const int from_;
  const int to_;
};
