[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/development/domainComparator.md)
[//]: # (Copyright (c) 2020 Jan Hamalčík)
[//]: # ()
[//]: # (Programmer's documentation of DomainComparator struct)
[//]: # ()

# `struct DomainComparator`

`struct DomainComparator` is a functor comparing two `Domains` based on
their position within proteins architecture (see
[theory](../user/theory.md)).
Only domains from the same protein should be compared by
`DomainComparator` to yield biologically relevant results.
The struct is defined in
[source/domainComparator.hpp](../../source/domainComparator.hpp)
and its methods are implemented in
[source/domainComparator.cpp](../../source/domainComparator.cpp).

# Public functions

```cpp
bool DomainComparator::operator()(const Domain& x, const Domain& y) const;
```

Function `bool operator()()` has following parameters:

* `const Domain& x` is a reference to an instance of class `Domain`.
* `const Domain& y` is a reference to an instance of class `Domain`.

It returns true iff position of the first amino acid of domain `x` within
a given protein is less than position of the first amino acid of domain
`y` within the same protein.
This is done by comparing integer references returned by
`const int& Domain::getFrom() const` method.

---

[Next](architecture.md)

[Previous](domain.md)
