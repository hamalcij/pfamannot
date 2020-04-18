[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/development/parameters.md)
[//]: # (Jan Hamalcik)
[//]: # ()
[//]: # (Programmer's documentation of Parameters class)
[//]: # ()

# `class Parameters`

`class Parameters` encapsulates all parameters, which steer the parsing.
They are set or edited through `pfamannot`'s options or through the
initial phase of `pfamannot` prior to parsing.
The class is defined in [source/parameters.hpp](../../source/parameters.hpp)
and its methods are implemented in
[source/parameters.cpp](../../source/parameters.cpp).

## Using-declarations

```cpp
using setOfPfamIDs = std::set<std::string>;
```

`setOfPfamIDs` denotes a set consisting of `std::string` instances.
These strings shall represent Pfam IDs (see
[theory](../user/theory.md)).
Their format must be `PFxxxxx`, where `x` is a digit.

---

## Private variables

```cpp
bool crucialInformation_[4];
```

Array `crucialInformation_` of size 4 is preset to `false` for each `i`.
`pfamannot` may start parsing iff all elements of `crucialInformation_`
are se to `true`.

* `crucialInformation_[0]` states, if at least one Pfam ID has been
specified, or if Pfam ID must not be specified.
In other words, it is set to `true` iff either option `-i`, `-e`, `-a` or
`d` has been used, or if user has provided at least one Pfam ID when
being asked.
* `crucialInformation_[1]` states, if location of `Pfam-A.full` file is
known or not needed.
Such location is known when the user uses option `-p` or `--pfam-file`,
when the user provides the path when being asked or when `pfamannot`
downloads `Pfam-A.full` file.
Location is not needed, if `pfamannot` saved parsed data in any of the
previous runs.
* `crucialInformation_[2]` states, if location of
`uniprot_reference_proteomes.dat` file is known.
Such location is known when the user uses option `-u` or
`--uniprot-file`, when the user provides the path when being asked or
when `pfamannot` downloads `uniprot_reference_proteomes.dat` file.
* `crucialInformation_[3]` states, if location of output file is known.
Such location is known when the user uses option `-o` or `--output-file`,
or when the user provides the path when being asked.

---

```cpp
setOfPfamIDs pfamIDsToInclude_;
```

Variable `pfamIDsToInclude_` stores a set of Pfam IDs.
All of these domains must be found in a protein's architecture to be
annotated and written to the output file.
The user specifies such proteins using option `-i` or by providing them
when being asked.

---

```cpp
setOfPfamIDs pfamIDsToExclude_;
```

Variable `pfamIDsToExclude_` stores a set of Pfam IDs.
None of these domains can be found in a protein's architecture to be
annotated and written to the output file.
The user specifies such proteins using option `-e` or by providing them
when being asked.

---

```cpp
std::string pfamAFullLocation_;
```

Variable `pfamAFullLocation_` is preset to `""` and contains the path
to the `Pfam-A.full` file.
If `Pfam-A.full` file doesn't need to be specified, `pfamAFullLocation_`
must remain empty.
This happens if `pfamannot` saved parsed data in any of the previous
runs.

---

```cpp
std::string uniprotReferenceProteomesLocation_;
```

Variable `uniprotReferenceProteomesLocation_` is preset to `""` and
contains the path to the `uniprot_reference_proteomes.dat` file.

---

```cpp
std::string outputFileLocation_;
```

Variable `outputFileLocation_` is preset to `""` and contains the path
to the output file.

---

```cpp
bool reset_;
```

Variable `reset_` is preset to `false` and informs, whether `pfamannot`
must use the `Pfam-A.full` file.
This can be invoked by using option `-r` or is detected automatically
if file at `savedMapOfProteinsLocation_` does not exist.

---

```cpp
bool saveMapOfProteins_;
```

Variable `saveMapOfProteins_` is preset to `true` and informs, whether
`pfamannot` shall save the data structure of parsed `Pfam-A.full` file
to run faster in further runs.
User can disable this option, when being asked during `pfamannot`'s first
run or after invoking option `-r`.

---

```cpp
bool userWantsAllProteins_;
```

Variable `userWantsAllProteins_` is preset to `false` and informs,
whether all proteins from `uniprot_reference_proteomes.dat` file shall
be annotated and written to the output file, even those not containing
any domain.
This can be invoked only by using the option `-a`.
In such case, both `pfamIDsToInclude_` and `pfamIDsToExclude_` must be
empty and `userWantsAllDomainContainingProteins_` must be set to `false`.

---

```cpp
bool userWantsAllDomainContainingProteins_;
```

Variable `userWantsAllDomainContainingProteins_` is preset to `false`
and informs, whether all proteins with at least one domain shall be
annotated and written to the output file.
This can be invoked only by using the option `-d`.
In such case, both `pfamIDsToInclude_` and `pfamIDsToExclude_` must be
empty and `userWantsAllProteins_` must be set to `false`.

---

```cpp
const std::string savedMapOfProteinsLocation_;
```

Constant `savedMapOfProteinsLocation_` is preset to
`"/usr/local/share/.pfamannot_parsed_pfam"` and contains the path where
the data structure of parsed `Pfam-A.full` file shall be saved to make
`pfamannot` run faster in further runs.
Better performance by parsing the data structure saved in this file is
achieved, because `Pfam-A.full` file contains a lot of useless data,
whereas file at `savedMapOfProteinsLocation_` contains only data
relevant for `pfamannot`.

---

## Private functions

```cpp
void Parameters::checkIfResetNeeded();
```

Procedure `checkIfResetNeeded()` checks if value of `reset_` shall be
set to `true`.
This is done by checking if file at `savedMapOfProteinsLocation_` exists
and is not empty.

---

## Constructor

```cpp
Parameters::Parameters();
```

Calls `Parameters::checkIfResetNeeded()`.

---

## Public functions

```cpp
bool Parameters::allParametersSpecified();
```

Returns true iff value of `crucialInformation_[i]` is set to `true` for
each `i`.
If `false` is returned, `pfamannot` does not have sufficient amount of
information to start parsing.

---

```cpp
void Parameters::addPfamIDToInclude(const std::string& pfamID);
```

Emplaces `pfamID` into `pfamIDsToInclude_` and sets
`crucialInformation_[0]` to `true`, as at least one Pfam ID has been
provided.
Following parameters have to be specified:

* `const std::string& pfamID` is the domain's Pfam ID, such domain
must be present in all proteins that are annotated and written to the
output file.
Its format should always be `PFxxxxx`, where `x` is a digit.

---

```cpp
void Parameters::addPfamIDToExclude(const std::string& pfamID);
```

Emplaces `pfamID` into `pfamIDsToExclude_` and sets
`crucialInformation_[0]` to `true`, as at least one Pfam ID has been
provided.
Following parameters have to be specified:

* `const std::string& pfamID` is the domain's Pfam ID, such domain
must not be present in any protein that is annotated and written to the
output file.
Its format should always be `PFxxxxx`, where `x` is a digit.

---

```cpp
void Parameters::setPfamAFullLocation(const std::string& pfamAFullLocation);
```

Sets `pfamAFullLocation_` to `pfamAFullLocation` and sets
`crucialInformation_[1]` to `true`, as location of `Pfam-A.full` file
is hereby known.
Following parameters have to be specified:

* `const std::string& pfamAFullLocation` is the path to the `Pfam-A.full`
file.

---

```cpp
void Parameters::setUniprotReferenceProteomesLocation(const std::string& uniprotReferenceProteomesLocation);
```

Sets `uniprotReferenceProteomesLocation_` to
`uniprotReferenceProteomesLocation` and sets `crucialInformation_[2]` to
`true`, as location of `uniprot_reference_proteomes.dat` file is hereby
known.
Following parameters have to be specified:

* `const std::string& uniprotReferenceProteomesLocation` is the path to
the `uniprot_reference_proteomes.dat` file.

---

```cpp
void Parameters::setOutputFileLocation(const std::string& outputFileLocation);
```

Sets `outputFileLocation_` to `outputFileLocation` and sets
`crucialInformation_[3]` to `true`, as location of the output file is
hereby known.
Following parameters have to be specified:

* `const std::string& outputFileLocation` is the path to the output file.

---

```cpp
void Parameters::setUserWantsAllProteins();
```

Sets `userWantsAllProteins_` to `true` and sets `crucialInformation_[0]`
to `true`, as no Pfam ID may be specified.

---

```cpp
void Parameters::setUserWantsAllDomainContainingProteins();
```

Sets `userWantsAllDomainContainingProteins_` to `true` and sets
`crucialInformation_[0]` to `true`, as no Pfam ID may be specified.

---

```cpp
void Parameters::setReset();
```

Sets `reset` to `true`.
If no path to `Pfam-A.full` file is known, `crucialInformation_[1]` must
be set to `false`, because `Pfam-A.full` file is needed, as the saved
data structure of parsed `Pfam-A.full` may no longer be used - it shall
be reset.

---

```cpp
void Parameters::unsetSaveMapOfProteins();
```

Sets `saveMapOfProteins_` to `false`.
This is useful, if the user does not want to save the data structure of
parsed `Pfam-A.full` file, which the user can decide when being asked
upon using option `-r` or when `pfamannot` is run for the first time.

---

```cpp
const setOfPfamIDs& Parameters::getPfamIDsToInclude() const;
```

Returns constant reference to private variable `pfamIDsToInclude_`.

---

```cpp
const setOfPfamIDs& Parameters::getPfamIDsToExclude() const;
```

Returns constant reference to private variable `pfamIDsToExclude_`.

---

```cpp
const std::string& Parameters::getPfamAFullLocation() const;
```

Returns constant reference to private variable `pfamAFullLocation_`.

---

```cpp
const std::string& Parameters::getUniprotReferenceProteomesLocation() const;
```

Returns constant reference to private variable
`uniprotReferenceProteomesLocation_`.

---

```cpp
const std::string& Parameters::getOutputFileLocation() const;
```

Returns constant reference to private variable `outputFileLocation_`.

---

```cpp
const bool Parameters::getReset() const;
```

Returns constant value of private variable `reset_`.

---

```cpp
const bool Parameters::getSaveMapOfProteins() const;
```

Returns constant value of private variable `saveMapOfProteins_`.

---

```cpp
const bool Parameters::getUserWantsAllProteins() const;
```

Returns constant value of private variable `userWantsAllProteins_`.

---

```cpp
const bool Parameters::getUserWantsAllDomainContainingProteins() const;
```

Returns constant value of private variable
`userWantsAllDomainContainingProteins_`.

---

```cpp
const std::string& Parameters::getSavedMapOfProteinsLocation() const;
```

Returns constant reference to private constant
`savedMapOfProteinsLocation_`.

---

```cpp
const bool Parameters::pfamIDSpecified() const;
```

Returns constant value of private variable `crucialInformation_[0]`.

---

```cpp
const bool Parameters::pfamFileSpecified() const;
```

Returns constant value of private variable `crucialInformation_[1]`.

---

```cpp
const bool Parameters::uniprotFileSpecified() const;
```

Returns constant value of private variable `crucialInformation_[2]`.

---

```cpp
const bool Parameters::outputFileSpecified() const;
```

Returns constant value of private variable `crucialInformation_[3]`.

---

[Next](parser.md)

[Previous](downloaderWrapper.md)
