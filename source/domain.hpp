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
