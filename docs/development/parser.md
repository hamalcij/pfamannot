[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/development/parser.md)
[//]: # (Copyright © 2020 Jan Hamalčík)
[//]: # ()
[//]: # (Programmer's documentation of Parser class)
[//]: # ()

# `class Parser`

`class Parser` performs the parsing of `Pfam-A.full` and
`uniprot_reference_proteomes.dat` files, alternatively
`Parameters::savedMapOfProteinsLocation_` and
`uniprot_reference_proteomes.dat` files.
Output is written to `Parameters::outputFileLocation_`.
The user is informed about the progress.
The class is defined in [source/parser.hpp](../../source/parser.hpp) and its
methods are implemented in [source/parser.cpp](../../source/parser.cpp).

## Using-declarations

```cpp
using mapOfProteins = std::unordered_map<std::string, Architecture>;
```

`mapOfProteins` denotes an unordered map of pairs consisting of
`std::string` and `Architecture` instances.
String shall represent UniProt ID of a protein with at least one domain
(see [theory](../user/theory.md)) and `Architecture` is the protein's
architecture.

`std::unordered_map` is used instead of `std::map` to reduce time
complexity of search, insertion and removal of elements from logarithmic
to constant-time.
The key shall always be an UniProt ID, which is a short string unique for
each protein, therefore we can assume, that `c++`'s build-in string hash
function will assign each UniProt ID an unique hash.

---

## Private variables

```cpp
mapOfProteins proteins_;
```

Variable `proteins_` stores the mapping from UniProt IDs to their
architecture and shall contain only proteins, that will be annotated
and written to the output file (exception: option `-a` has been invoked).
Proteins appearing in `proteins_` are to be found in the `Pfam-A.full`
file, thereby guaranteeing that each protein in `proteins_` will contain
at least one domain, thus having an architecture defined.

To create a valid mapping `proteins_`, whole `Pfam-A.full` file has to be
parsed before removing any unwanted proteins, because the `Pfam-A.full`
file is a mapping from domains to proteins, not vice versa.
Therefore, each protein in `proteins_` has a complete architecture, when
all domains from `Pfam-A.full` file are processed.
First then it is possible to delete any unwanted proteins to have the
wanted once fully annotated, without any domain missing.

`proteins_` data structure can be saved to a file at
`Parameters::savedMapOfProteinsLocation_` and recreated next time
`pfamannot` is run.
This reconstruction of `proteins_` from
`Parameters::savedMapOfProteinsLocation_` will be much faster then
creating it de novo from `Pfam-A.full` file, because the `Pfam-A.full`
file contains a lot of useless data.

If option `-a` has not been invoked, `pfamannot` only pays attention to
those proteins in `uniprot_reference_proteomes.dat` file, which are
present in the `proteins_` mapping after deleting unwanted proteins.
In case option `-a` has been used, all proteins from the
`uniprot_reference_proteomes.dat` files are annotated, even those without
any domain, and the `proteins_` mapping is used to retrieve the
architecture of those, that have at least one domain.

---

```cpp
bool savedMapOfProteinsIsBeingSaved_;
```

Variable `savedMapOfProteinsIsBeingSaved_` is preset to `false` and
informs, whether the process of saving the `proteins_` data structure
of parsed `Pfam-A.full` file is currently ongoing.
If `SIGINT` interrupt signal is called,
`savedMapOfProteinsIsBeingSaved_` is checked to decide if the
`Parameters::savedMapOfProteinsLocation_` file is incomplete and shall
therefore be deleted to prevent unwanted behavior next time `pfamannot`
is run.

---

```cpp
const Parameters& parameters_;
```

Constant `parameters_` stores a constant reference to an instance of
class `Parameters`.
Private variables of `Parameters` will be used to steer the parsing
process depending on the user's input.

---

```cpp
static const int standardPfamIDLength_;
```

Constant `standardPfamIDLength_` is preset to `7` and states how long
a Pfam ID must be.
As the correct Pfam ID format is `PFxxxxx`, where `x` is a digit, any
Pfam ID is clearly 7 characters long.

---

```cpp
static const int pfamIDOffset_;
```

Constant `pfamIDOffset_` is preset to `10` and states that a Pfam ID in
`Pfam-A.full` file always starts at position `pfamIDOffset_` on a line
beginning with `#=GF AC`.

---

```cpp
static const int pfamProteinOffset_;
```

Constant `pfamProteinOffset_` is preset to `5` and states that an
UniProt ID containing current Pfam ID in `Pfam-A.full` file always
starts at position `pfamProteinOffset_` on a line beginning with `#=GS`.

---

```cpp
static const int uniprotOffset_;
```

Constant `uniprotOffset_` is preset to `5` and states that any
information in `uniprot_reference_proteomes.dat` starts at position
`uniprotOffset_` on each line.

---

```cpp
static constexpr std::size_t maxBufferSize_;
```

Constant expression `maxBufferSize_` is preset to `1024*1024` and denotes
the maximum size of the buffer, into which a line of either `Pfam-A.full`
or `uniprot_reference_proteomes.dat` file is read by `std::ifstream`.
This expression has been set to yield such value that would prevent
even the longest line of any of these files to be split into two, when
reading the file.

---

## Private functions

```cpp
void Parser::run();
```

Procedure `run()` first initiates the singleton wrapper `ParserWrapper`
and sets its private variable `Parser* parser_` to this particular
instance.
This is needed because of `SIGINT` interrupt signal handling, so that
the static method `static void Parser::signalHandler(int signum);`
can access private variables owned by this instance.

The `SIGINT` signal handler is then initiated and the parsing begins.
First, if the saved data structure of previously parsed `Pfam-A.full`
file cannot be used, either because no such file exists or because the
user has used option `-r`, the `Pfam-A.full` file is parsed and the
`proteins_` data structure is optionally saved.
Else, the saved data structure of previously parsed `Pfam-A.full` file
is loaded.
Unwanted proteins from `proteins_` are then deleted and the parsing
of `uniprot_reference_proteomes.dat` and the annotation of proteins and
their output to the output file is then initiated.

---

```cpp
void Parser::parsePfamAFull();
```

Procedure `parsePfamAFull()` creates the data structure `proteins_`
from the `Pfam-A.full` file by reading it line by line and analyzing
each line stored in `char buffer[maxBufferSize_]`.
Using a char array is much faster than construing `std::string` for each
of `Pfam-A.full`'s roughly 100,000,000 lines.

While parsing, `parsePfamAFull()` procedure looks for following
patterns at the beginning of each line:

* `#=GF AC   PFxxxxx` is a line defining a new entry about Pfam ID
`PFxxxxx`, where `x` is a digit.
* `#=GS ABCDEF_GHIJ/from-to` is a line stating that a protein with
UniProt ID `ABCDEF_GHIJ` contains a domain represented by the current
entry `PFxxxxx`.
This domain is to be found in the protein's sequence spanning from
position `from` to position `to`.
* `//` is a line, where the currect entry `PFxxxxx` ends.
Next entry, which defines a domain with Pfam ID `PFyyyyy` and all
proteins containing this domain, follows.
Alternatively, we have reached the end of `Pfam-A.full` file.
* Lines beginning with an uppercase letter or a digit contain the
Multiple Sequence Alignment of the current entry's domain.
These lines are not of our interest.
* If a line is found in the `Pfam-A.full` file, that does not begin with
`#`, `/`, an uppercase letter or a digit, the `Pfam-A.full` file is
corrupted and `pfamannot` must terminate to prevent any incorrect
output.

See an [example entry](pfamExample.txt) of the
`Pfam-A.full` file.

---

```cpp
void Parser::saveMapOfProteins();
```

Procedure `saveMapOfProteins()` saves the data structure `proteins_`
created with the `Parser::parsePfamAFull()` procedure to
`Parameters::savedMapOfProteinsLocation_`.
This is done by default, so that `pfamannot` can run faster next time.
During the process, `savedMapOfProteinsIsBeingSaved_` should be set to
`true`, so that possible `SIGINT` interrupt signal can be handled
properly.

The syntax used by `saveMapOfProteins()` to save the data structure
`proteins_` to a file should be reflected by
`Parser::loadSavedMapOfProteins()`, which should be able to effectively
restore the data structure, when `pfamannot` is run next time.
The authors of `pfamannot` use the following syntax:

* `#ABCDEF_GHIJ`: On any line starting with `#`, a UniProt ID follows
immediately afterwords.
Here, a new protein is defined and its architecture will follow.
* `*PFxxxxx from to`: On any line starting with `*`, a Pfam ID of a
domain followed by a space, domain's starting position within the
protein's sequence, another space and domain's ending position within the protein's sequence, follows immediately afterwords.
The previous line must start with `#` and contain the protein's UniProt
ID and when restoring `proteins_` in `Parser::loadSavedMapOfProteins()`,
a new protein is emplaced to the `proteins_` mapping with constructors of
`std::string` and `Architecture` to create them in place.
A arbitrary number of lines starting with `@` and containing further domains belonging the the protein follows.
* `@PFxxxxx from to`: Any line starting with `@` contains further domains
with syntax analogical to `*`-lines.
When restoring `proteins_` in `Parser::loadSavedMapOfProteins()`, this
domain is emplaced to current protein's architecture using the
`Architecture::emplace()` method.
* `/`: A line with a single `/` denotes the end of current protein.

See an [example entry](savedMapOfProteinsExample.txt)
of the saved data structure.

---

```cpp
void Parser::loadSavedMapOfProteins();
```

Procedure `loadSavedMapOfProteins()` restores the data structure
`proteins_` from `Parameters::savedMapOfProteinsLocation_`. Calling this
procedure makes sense only if the data structure `proteins_` has been
successfully saved in some of the previous runs of `pfamannot` and if
option `-r` has not been invoked.

As the procedure `Parser::saveMapOfProteins()` decides the syntax of
the file with the saved data structure `proteins_` stored at
`Parameters::savedMapOfProteinsLocation_`, `loadSavedMapOfProteins()`
must reflect this syntax.
See `Parser::saveMapOfProteins()` to gain in depth knowledge of the
syntax or see an
[example entry](savedMapOfProteinsExample.txt)
of the saved data structure.

---

```cpp
void Parser::parseUniprotReferenceProteomes();
```

Procedure `parseUniprotReferenceProteomes()` goes through the
`uniprot_reference_proteomes.dat` and outputs them annotated to the
output file located at `Parameters::outputFileLocation_`.

The choice of which proteins from `uniprot_reference_proteomes.dat` file
shall be annotated and written to the output file, depends on the option
`-a`.
If option `-a` has been invoked, all proteins from the
`uniprot_reference_proteomes.dat` file shall be annotated, even those
without any domain. See `Parser::chooseAlsoProteinsWithoutDomains()`.
Else only proteins contained in the `proteins_` data structure shall be
annotated. See `Parser::chooseOnlyProteinsWithDomains()`.

In both cases, the procedures look for the following patterns at the
beginning of each line in the `uniprot_reference_proteomes.dat` file:

* `ID   ABCDEF_GHIJ` is a line defining a new entry of a protein denoted
by the UniProt ID `ABCDEF_GHIJ`.
* `OC   Organism; Full; Name` is a line stating in which organism is
the protein denoted by the current UniProt ID to be found.
The full name of the organism can be long and can therefore span
multiple consecutive lines.
* `     AMINOACIDS` is a line that contains the protein's amino acid
sequence.
The sequence usually spans multiple consecutive lines, and each line
contains 6 blocks of 10 amino acids, each block separated by a space.
One exception is the last line, where less then six blocks can be found
with the last block possibly containing less than 10 amino acids.
* `//` is the line, where the current entry `ABCDEF_GHIJ` ends.
Next entry, which defines a protein with UniProt ID `KLMNOP_QRST`,
organism where it can be found, and its sequence, follows.
Alternatively, we have reached the end of
`uniprot_reference_proteomes.dat` file.
*  If a line is found in the `uniprot_reference_proteomes.dat` file,
that does not begin with `/`, an uppercase letter or a space, the
`uniprot_reference_proteomes.dat` file is corrupted and `pfamannot` must
terminate.

See an [example entry](uniprotExample.txt) of the
`uniprot_reference_proteomes.dat` file.

---

```cpp
void Parser::chooseOnlyProteinsWithDomains(std::ifstream& uniprotFile, std::ofstream& outputFile);
```

Procedure `chooseOnlyProteinsWithDomains()` reads and analyzes
`uniprot_reference_proteomes.dat` file, each line is stored in
`char buffer[maxBufferSize_]`.
Using a char array is much faster than construing `std::string` for each
of `uniprot_reference_proteomes.dat`'s roughly 280,000,000 lines.
If an entry from the file is found in `proteins_` data structure, it
will be annotated and written to the output file.

See `parseUniprotReferenceProteomes()` or
[example entry](uniprotExample.txt) to get in depth
knowledge of the structure of `uniprot_reference_proteomes.dat` file.

Following parameters have to be specified:

* `std::ifstream& uniprotFile` is the reference to the input file stream
representing the `uniprot_reference_proteomes.dat` file.
* `std::ofstream& outputFile` is the reference to the output file stream
representing the output file.

---

```cpp
void Parser::chooseAlsoProteinsWithoutDomains(std::ifstream& uniprotFile, std::ofstream& outputFile);
```

Procedure `chooseAlsoProteinsWithoutDomains()` reads and analyzes
`uniprot_reference_proteomes.dat` file, each line is stored in
`char buffer[maxBufferSize_]`.
Using a char array is much faster than construing `std::string` for each
of `uniprot_reference_proteomes.dat`'s roughly 280,000,000 lines.
Every protein from this file is annotated and written to the output file,
if a protein is also found in the `proteins_` data structure, the
annotation will contain its architecture.

See `parseUniprotReferenceProteomes()` or
[example entry](uniprotExample.txt) to get in depth
knowledge of the structure of `uniprot_reference_proteomes.dat` file.

Following parameters have to be specified:

* `std::ifstream& uniprotFile` is the reference to the input file stream
representing the `uniprot_reference_proteomes.dat` file.
* `std::ofstream& outputFile` is the reference to the output file stream
representing the output file.

---

```cpp
void Parser::deleteProteinsNotOfInterest();
```

Procedure `deleteProteinsNotOfInterest()` goes through the `proteins_`
data structure, first deletes any protein not containing all domains
from `Parameters::pfamIDsToInclude_` by calling
`Parser::deleteProteinsWithoutDomainsToBeIncluded()`, and then deletes
any protein containing any of domains from
`Parameters::pfamIDsToExclude_` by calling
`Parser::deleteProteinsWithDomainsToBeExcluded()`.
Then writes the total number of deleted proteins to the standard output.
This is relevant only if options `-a` and `-d` are not invoked.

---

```cpp
void Parser::deleteProteinsWithoutDomainsToBeIncluded(unsigned long& numberOfDeleted);
```

Procedure `deleteProteinsWithoutDomainsToBeIncluded()` goes through the
`proteins_` data structure.
If a protein's architecture does not contain all domains from
`Parameters::pfamIDsToInclude_`, it is not relevant for the user and
therefore will be deleted from `proteins_`.
At the end, only proteins with architectures, where every wanted domain
is present, will be kept.
Following parameters have to be specified:

* `unsigned long& numberOfDeleted` is a reference to a long integer. This
number shall be incremented every time a protein is deleted.

---

```cpp
void Parser::deleteProteinsWithDomainsToBeExcluded(unsigned long& numberOfDeleted);
```

Procedure `deleteProteinsWithDomainsToBeExcluded()` goes through the
`proteins_` data structure.
If a protein's architecture does contain at least one of the domains from
`Parameters::pfamIDsToExclude_`, it is not relevant for the user and
therefore will be deleted from `proteins_`.
At the end, only proteins with architectures, where none of the unwanted
domains is present, will be kept.
Following parameters have to be specified:

* `unsigned long& numberOfDeleted` is a reference to a long integer. This
number shall be incremented every time a protein is deleted.

---

```cpp
void Parser::addProteinToDomainMapping(const std::string& pfamID, const char* line);
```

Procedure `addProteinToDomainMapping()` parses a line from the
`Pfam-A.full` file starting with `#=GS ABCDEF_GHIJ/from-to`.
From this line, it extracts the UniProt ID of a protein that contains
a domain denoted by the current Pfam ID, the domain's starting position
within the protein's sequence and the domain's ending position within
the protein's sequence.
Then it searches in `proteins_` for the UniProt ID `ABCDEF_GHIJ`.

If such protein is already found in `proteins_`, a `Domain` is emplaced
to the protein's `Architecture`.
Else, the protein is emplaced into `proteins_` together with its first
found `Domain`, which is in turn emplaces into `Architecture`'s
constructor.

Following parameters have to be specified:

* `const std::string& pfamID` is a constant reference to the Pfam ID of
the current entry in `Pfam-A.full` file.
This domain is contained in the protein described on the `line`.
* `const char* line` is a constant pointer to the current line in the
`Pfam-A.full` file, where information about the protein and interval
within its position, where the domain `pfamID` is to be found, is stored.
This line must begin with `#=GS`.

---

```cpp
void Parser::readUniprotLineToEndAndOutputWithoutSpace(std::ofstream& outputFile, const char* line);
```

Procedure `readUniprotLineToEndAndOutputWithoutSpace()` reads a line
from the `uniprot_reference_proteomes.dat` file and outputs its content
without spaces.
The content begins at position `uniprotOffset_` and contains either
the organism, where current protein can be found, or the protein's
sequence.
Following parameters have to be specified:

* `std::ofstream& outputFile` is the reference to the output file stream
representing the output file.
* `const char* line` is a constant pointer to the current line in the
`uniprot_reference_proteomes.dat` file, where information about the
organism, where the current protein can be found, or its sequence, is
stored.
This line must begin either with `OC` or a space.

---

```cpp
void Parser::printPfamIDs(const setOfPfamIDs& pfamIDs);
```

Procedure `printPfamIDs()` writes out every Pfam ID from `pfamIDs` to
the standard output with a single space in front of every of them.
Following parameters have to be specified:

* `const setOfPfamIDs& pfamIDs` is a constant reference to a set of
`std::string` instances representing Pfam IDs.
It is either `Parameters::pfamIDsToInclude_` or
`Parameters::pfamIDsToExclude_`.

---

```cpp
void Parser::failure(const std::string& errorMessage);
```

Procedure `failure()` prints an error message to the error output and
terminates `pfamannot`.
Following parameters have to be specified:

* `const std::string& errorMessage` is a constant reference to the string
describing the error that has occurred.

---

```cpp
static std::string Parser::currentTime();
```

Returns a string containing current time in hh:mm:ss format.
Used to inform the user about `pfamannot`'s progress.

---

```cpp
static void Parser::oneDigitTimeToTwoDigitTime(std::string& timeUnit);
```

Procedure `oneDigitTimeToTwoDigitTime()` checks if a time unit is
represented only by one digit and in such case converts it to a time unit
with two digits by adding a `"0"` at the beginning.
E.g. `"7"` becomes `"07"`, thus getting a neat time format.
Following parameters have to be specified:

* `std::string& timeUnit` is a reference to a string representing a time
unit, either hours, minutes or seconds.

---

```cpp
static void Parser::signalHandler(int signum);
```

Informs the user about incoming signal, mainly `SIGINT` interrupt signal,
and checks `savedMapOfProteinsIsBeingSaved_` to delete an incompletely processed file at `Parameters::savedMapOfProteinsLocation_`.
Access to private variables of a specific `Parser` instance is
granted by the singleton wrapper `ParserWrapper`.
`signalHandler()` is passed as an argument to `std::signal`.
Following parameters have to be specified:

* `int signum` is the integer code of a command line signal, for example
the `CTRL`+`C` `SIGINT` interrupt signal.

---

## Constructor

```cpp
Parser::Parser(const Parameters& parameters);
```

Calls `Parser::run()` and assigns the passed reference to the private variable `parameters_`:

* `parameters` is a constant reference to an instance of `Parameters`.

---

[Next](parserWrapper.md)

[Previous](parameters.md)
