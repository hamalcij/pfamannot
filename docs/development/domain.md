[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/development/domain.md)
[//]: # (Jan Hamalcik)
[//]: # ()
[//]: # (Programmer's documentation of Domain class)
[//]: # ()

# `class Domain`

`class Domain` represents a protein's domain (see
[theory](../user/theory.md)), by storing its Pfam ID and span within
protein's sequence.
The class is defined in [source/domain.hpp](../../source/domain.hpp) and its
methods are implemented in [source/domain.cpp](../../source/domain.cpp).

## Private variables

```cpp
const std::string domain_;
```

Constant `domain_` stores name of `Domain`'s instance.
It should be formatted as a Pfam ID `PFxxxxx`, where `x` is a digit.

---

```cpp
const int from_;
```

Constant `from_` stores the position of `Domain`'s first amino acid in
given protein.

---

```cpp
const int to_;
```

Constant `to_` stores the position of `Domain`'s last amino acid in given
protein.

---

## Constructor

```cpp
Domain::Domain(const std::string domain, const int from, const int to);
```

Assigns passed values to all private variables:

* `const std::string domain` is the domain's Pfam ID.
Its format should always be `PFxxxxx`, where `x` is a digit.
* `const int from` is the position of domain's first amino acid in given
protein.
Its value should always be less than `to`.
* `const int to` is the position of domain's last amino acid in given
protein.
Its value should always be greater than `from`.

---

# Public functions

```cpp
const std::string& Domain::getDomain() const;
```

Returns constant reference to private variable
`const std::string domain_`.

---

```cpp
const int& Domain::getFrom() const;
```

Returns constant reference to private variable `const int from_`.

---

```cpp
const int& Domain::getTo() const;
```

Returns constant reference to private variable `const int to_`.

---

[Next](domainComparator.md)
