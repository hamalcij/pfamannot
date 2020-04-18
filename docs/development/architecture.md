[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/development/architecture.md)
[//]: # (Copyright (c) 2020 Jan Hamalčík)
[//]: # ()
[//]: # (Programmer's documentation of Architecture class)
[//]: # ()

# `class Architecture`

`class Architecture` represents a protein's architecture (see
[theory](../user/theory.md)) by storing ordered domains belonging to
given protein in a container.
The class is defined in [source/architecture.hpp](../../source/architecture.hpp)
and its methods are implemented in
[source/architecture.cpp](../../source/architecture.cpp).

## Using-declarations

```cpp
using setOfDomains = std::set<Domains, DomainComparator>;
```

`setOfDomains` denotes a set consisting of `Domain` instances.
These are sorted upon insertion by the `DomainComparator` functor, which
guarantees that inserted domains will be kept in correct order as in
given protein.

---

## Private variables

```cpp
setOfDomains architecture_;
```

Variable `architecture_` stores the architecture of the protein
represented by an instance of `class Architecture`.
It should only contain domains that are present in the protein represented
by the instance.

---

## Constructor

```cpp
Architecture::Architecture(const std::string& pfamID, const int& from, const int& to);
```

Emplaces a new instance of `class Domain` to newly created
`architecture_` by the construction of `Architecture`.
An architecture cannot exist without at least one domain.
Following parameters have to be specified:

* `const std::string domain` is the domain's Pfam ID.
Its format should always be `PFxxxxx`, where `x` is a digit.
* `const int from` is the position of domain's first amino acid in given
protein.
Its value should always be less than `to`.
* `const int to` is the position of domain's last amino acid in given
protein.
Its value should always be greater than `from`.

---

## Public functions

```cpp
void Architecture::emplace(const std::string& pfamID, const int& from, const int& to);
```

Same as [constructor](#Constructor).
In this case, however, at least one domain from represented protein
already exists and this adds another domain.

---

```cpp
const setOfDomains& Architecture::getArchitecture() const
```

Returns constant reference to private variable
`setOfDomains architecture_`.

---

[Next](downloader.md)

[Previous](domainComparator.md)
