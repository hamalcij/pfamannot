[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/development/flowController.md)
[//]: # (Copyright (c) 2020 Jan Hamalčík)
[//]: # ()
[//]: # (Programmer's documentation of FlowController class)
[//]: # ()

# `class FlowController`

`class FlowController` controls the flow of `pfamannot`.
The `main()` function implemented in [source/main.cpp](../../source/main.cpp)
calls the constructor of `FlowController` to initiate `pfamannot`.
This class parses the command line, communicates with the user, if
additional information is needed to start parsing, which is also
initiated.
The class is defined in
[source/flowController.hpp](../../source/flowController.hpp) and its methods
are implemented in
[source/flowController.cpp](../../source/flowController.cpp).

## Private variables

```cpp
char** argv_;
```

Variable `argv_` stores the command line arguments.
These have to be checked for options.

---

```cpp
Parameters parameters_;
```

Variable `parameters_` stores an instance of class `Parameters`.
The user's input will decide, which values will the private variables of
`Parameters` store.

---

```cpp
const std::string pf_;
```

Constant `pf_` is preset to `"PF"` and stores the invariant beginning
of every Pfam ID (see [theory](../user/theory.md)).

---

```cpp
const std::string outputFileLongOption_;
```

Constant `outputFileLongOption_` is preset to `"--output-file"` and
defines what the long option for specifying path to the output file
should look like.

---

```cpp
const std::string pfamFileLongOption_;
```

Constant `pfamFileLongOption_` is preset to `"--pfam-file"` and defines
what the long option for specifying path to `Pfam-A.full` file should
look like.

---

```cpp
const std::string uniprotFileLongOption_;
```

Constant `uniprotFileLongOption_` is preset to `"--uniprot-file"` and
defines what the long option for specifying path to
`uniprot_reference_proteomes.dat` file should look like.

---

```cpp
const char* pfamAFullFTP_;
```

Constant `pfamAFullFTP_` is preset to
`"ftp://ftp.ebi.ac.uk/pub/databases/Pfam/current_release/Pfam-A.full.gz"`
and defines the URL to the `Pfam-A.full.gz` file at
The European Bioinformatics Institutes's
[FTP server](ftp://ftp.ebi.ac.uk/pub/databases/Pfam/current_release).
It is used by `curl` in class `Downloader`.

---

```cpp
const char* uniprotReferenceProteomesFTP_;
```

Constant `uniprotReferenceProteomesFTP_` is preset to
`"ftp://ftp.ebi.ac.uk/pub/databases/Pfam/current_release/uniprot_reference_proteomes.dat.gz"`
and defines the URL to the `uniprot_reference_proteomes.dat` file at
The European Bioinformatics Institutes's
[FTP server](ftp://ftp.ebi.ac.uk/pub/databases/Pfam/current_release).
It is used by `curl` in class `Downloader`.

---

```cpp
static const int standardPfamIDLength_;
```

Constant `standardPfamIDLength_` is preset to `7` and states how long
a Pfam ID must be.
As the correct Pfam ID format is `PFxxxxx`, where `x` is a digit, any
Pfam ID is clearly 7 characters long.

---

## Private functions

```cpp
void FlowController::run();
```

Procedure `run()` runs the whole `pfamannot` process.
First, it parses the command line by calling
`FlowController::parseCommandLine()`.
If there is no error, the user is greeted and `pfamannot`'s current
version is printed out.
It uses macros defined in `source/pfamannotConfig.hpp`, a file created
by the [configure](../../configure) script from
[source/pfamannotConfig.hpp.in](../../source/pfamannotConfig.hpp.in).

If the command line contained sufficient amount of options to start
parsing, parsing is started.
Else, the user is asked to provide some additional information first.

---

```cpp
void FlowController::parseCommandLine();
```

Procedure `parseCommandLine()` iterates over all arguments from the
command line to extract options and other their arguments defined by the
user.
See [docs/user/options.md](../user/options.md) to get familiar with
`pfamannot`'s options, their relationships and arguments that can be
passed with them.

Following flags are used by this procedure to steer the parsing in a
meaningful way:

* `bool outputFileNotSpecified` is preset to `true` and informs, whether
path to the output file has not been specified yet to prevent multiple
`-o` or `--output-file` options.
* `bool pfamFileNotSpecified` is preset to `true` and informs, whether
path to `Pfam-A.full` file has not been specified yet to prevent multiple
`-p` or `--pfam-file` options.
* `bool uniprotFileNotSpecified` is preset to `true` and inform, whether
path to `uniprot_reference_proteomes.dat` file has not been specified yet
to prevent multiple `-u` or `--uniprot-file` options.
* `bool pfamIDNotSpecified` is preset to `true` and informs, whether no
Pfam ID has been specified yet to assure incompatibility of options `-a`
with `-d` and with `-i` or `-e`.
* `bool userWantsReset` is preset to `false` and informs, whether the
user has already specified the option `-r` to prevent multiple
occurrences of it.
This is needed, because `Parameters::checkIfResetNeeded()` checks
automatically, if a reset is needed, in such case setting
`Parameters::reset_` to `true`, which would without the flag
`userWantsReset` either allow the user to specify multiple options `-r`,
or none at all, if system detects automatically, that a reset is needed.
* `bool loopMayContinue` is preset to `true` at each new possible option
and informs, whether multiple options can follow after one *hyphen*.
The only possible combinations, where this can occur, are `-ar`, `-ra`,
`-dr` and `-rd`.
* `bool anOptionFollows` is preset to `true` at each new possible option
and informs, whether an option follows after a single *hyphen*.
* `char previousChar` is preset to `-` at each new possible option and
helps decide, if an option comes right after a single *hyphen* or after
another option.
For options, that must stand alone, the only allowed value of
`previousChar` upon their encounter is `-`.

---

```cpp
void FlowController::completeParameters();
```

Procedure `completeParameters()` is called, when the user did not
provide sufficient information on the command line.
This procedure goes through `Parameters::crucialInformation_[i]` and if
it is `false` for any `i`, it will ask the user to provide this crucial
information.

At least one Pfam ID has to be provided, either to
`Parameters::pfamIDsToInclude_` or `Parameters::pfamIDsToExclude_`.
The Pfam IDs have to meet the correct Pfam ID format `PFxxxxx`, where `x`
is a digit, and they have to be separated by a space.

If any of the inputs files are not specified, the user is asked, if
`pfamannot` shall download them.
Else, the user must provide a correct relative or full path to them.
In case path to the output file is missing as well, the user is also
asked to provide it.

---

```cpp
void FlowController::summarize();
```

Procedure `summarize()` informs the user about the specified parameters
and asks the user to either confirm them to start parsing or to reject
them.
It is important to ask the user to inspect their input closely,
`pfamannot` may namely run for many hours depending on the parameters
and it is much better to detect any typo early on than getting an
unwanted result after couple of hours waiting time.

If a reset of stored data structure `Parser::proteins_` created by
previous parsing of the `Pfam-A.full` file is needed, the user gets also
the possibility to decide, if the de novo parser `Pfam-A.full` file
shall be saved.
This will cause `pfamannot` to run slower this time, but faster in the
following runs.

---

```cpp
void FlowController::checkForHomeDirectory(std::string& path);
```

Procedure `checkForHomeDirectory()` substitutes the tidle `~` for the
`$HOME` environment variable, if it makes sense.
This procedure is called, if `bash` does not replace it itself, e.g. when
using a long option on the command line without double quotation marks
or in `pfamannot`'s process.
Following parameters have to be specified:

* `std::string& path` is a reference to the path, which may need the
substitution of `~` to be able to be successfully opened by
`std::fstream`.

---

```cpp
void FlowController::invalidCommandLine(const std::string& errorMessage);
```

Procedure `invalidCommandLine()` prints an error message to the error
output and terminates `pfamannot`.
Following parameters have to be specified:

* `const std::string& errorMessage` is a constant reference to the string
describing the error that has occurred.

---

```cpp
void FlowController::testOutputFile(const std::string& location);
```

Procedure `testOutputFile` tries to open file at `location` with
`std::ofstream`.
If the file cannot be opened, `FlowController::invalidCommandLine()` must
be called to terminate `pfamannot`.
Following parameters have to be specified:

* `const std::string& location` is a constant reference to the path of
the tested file.

---

```cpp
void FlowController::testInputFile(const std::string& location);
```

Procedure `testInputFile` tries to open file at `location` with
`std::ifstream`.
If the file cannot be opened, `FlowController::invalidCommandLine()` must
be called to terminate `pfamannot`.
Following parameters have to be specified:

* `const std::string& location` is a constant reference to the path of
the tested file.

---

```cpp
void FlowController::parsePfamIDs(const bool isOptionI);
```

Procedure `parsePfamIDs()` is called after the option `-i` or `-e` was
specified to retrieve one or more consecutive Pfam IDs that must follow.
If a Pfam ID is of the correct format `PFxxxxx`, where `x` is a digit,
it is inserted into the correct `setOfPfamIDs`.
Following parameters have to be specified:

* `const bool isOptionI` is a constant boolean value informing, whether
the Pfam IDs, that shall be parsed, come after the option `-i`.
In such case, the Pfam IDs will be inserted into
`Parameters::pfamIDsToInclude_`, else into
`Parameters::pfamIDsToExclude_`, meaning that they come after the option
`-e`.

---

```cpp
void FlowController::checkIfPfamIDsMayBeSpecified(const bool isOptionI, bool& pfamIDNotSpecified);
```

Procedure `checkIfPfamIDsMayBeSpecified` checks, if option `-a` or option
`-d` have not been chosen yet.
It that is true, Pfam IDs will be parsed.
Following parameters have to be specified:

* `const bool isOptionI` is a constant boolean value informing, whether
the Pfam IDs, that shall be parsed, if Pfam IDs may be specified, come
after the option `-i`.
In such case, the Pfam IDs will be inserted into
`Parameters::pfamIDsToInclude_`, else into
`Parameters::pfamIDsToExclude_`, meaning that they come after the option
`-e`.
* `bool& pfamIDNotSpecified` is a reference to flag `pfamIDNotSpecified`
of `FlowController::parseCommandLine()`, that shall be set to `false`,
if Pfam ID may be specified.

---

```cpp
void FlowController::askIfFileShouldBeDownloaded(const char* ftp, std::string& location, const std::string& fileName, const std::string& freeStorage);
```

Procedure `askIfFileShouldBeDownloaded()` asks the user, whether
`pfamannot` shall download either `Pfam-A.full` or
`uniprot_reference_proteomes.dat` files, if a corresponding `i` for
`Parameters::crucialInformation_[i]` is `false`.
In such case, the file will be downloaded by class `Downloader`, else
the user must provide a path to an existing file.
Following parameters have to be specified:

* `const char* ftp` is a pointer to the URL address, where given file
is stored at The European Bioinformatics Institutes's
[FTP server](ftp://ftp.ebi.ac.uk/pub/databases/Pfam/current_release).
`ftp` is used by `curl`.
* `std::string& location` is a reference to the location of given file.
Its changed value is then used by `FlowController::completeParameters()`
to test the input file.
* `const std::string& fileName` is a constant reference to human readable
file name.
Its value should be either `"Pfam-A.full"` or
`"uniprot_reference_proteomes.dat"`.
* `const std::string& freeStorage` is a constant reference to human
readable free local storage size, which is required to be able to
download given file.
Its value should be either `"90 GB"` for `Pfam-A.full` file or `"180 GB"`
for `uniprot_reference_proteomes.dat` file.

---

```cpp
void FlowController::printHashLine();
```

Procedure `printHashLine()` prints a line with 60 `#` characters to
separate the summary from other parts of the standard output.

---

```cpp
bool FlowController::pfamIDIsCorrect(const std::string& pfamID);
```

Return `true` iff `pfamID` is in format `PFxxxxx`, where `x` is a digit.
Following parameters have to be specified:

* `const std::string& pfamID` is a constant reference to a Pfam ID that
is going to be checked for the correct format.

---

```cpp
bool FlowController::checkAgainstPfamIDsToExclude(const std::string& pfamID);
```

Return `true` iff `pfamID`, that shall be inserted into
`Parameters::pfamIDsToInclude_` is not already present in
`Parameters::pfamIDsToExclude_`.
It wouldn't make sense to search for proteins that contain and at the
same time not contain a domain with the same Pfam ID.
Following parameters have to be specified:

* `const std::string& pfamID` is a constant reference to a Pfam ID that
shall be inserted into `Parameters::pfamIDsToInclude_`, if it is not
already present in `Parameters::pfamIDsToExclude_`.

---

```cpp
bool FlowController::checkAgainstPfamIDsToInclude(const std::string& pfamID);
```

Return `true` iff `pfamID`, that shall be inserted into
`Parameters::pfamIDsToExclude_` is not already present in
`Parameters::pfamIDsToInclude_`.
It wouldn't make sense to search for proteins that contain and at the
same time not contain a domain with the same Pfam ID.
Following parameters have to be specified:

* `const std::string& pfamID` is a constant reference to a Pfam ID that
shall be inserted into `Parameters::pfamIDsToExclude_`, if it is not
already present in `Parameters::pfamIDsToInclude_`.

---

```cpp
bool FlowController::optionStandsAlone(const char& option, const char& previousChar);
```

Returns `true` iff an arbitraty option `x` is alone, i.e. if ` -x ` is its
definition on the command line.
Must return true for options, that must stand alone: `-i`, `-e`, `-p`,
`-u` and `-o`.
Following parameters have to be specified:

* `const char& option` is a constant reference to the option, which must
stand alone.
It must be present for the possible error message.
* `const char& previousChar` is a constant reference to the character
standing right in front of `option` on the command line.

---

```cpp
bool FlowController::userAnswersNo();
```

Returns `true` iff the user answers `no`.
Returns `false` iff the user answers `yes`.
If the user's answer does not match `yes` or `no`, he is asked to provide
a new answer until the answer matches.

---

```cpp
std::string FlowController::readAWord();
```

Returns a string containing a word pointed to by `argv_[0]`.

---

```cpp
std::string FlowController::userWritesFilePath(const std::string& fileName);
```

Returns a string containing a path that the user has written.
Following parameters have to be specified:

* `const std::string& fileName` is the human readable name of the file,
to which the user will be asked to provide the path.
Its value should be either `"Pfam-A.full"`,
`"uniprot_reference_proteomes.dat"` or `"output"`.

---

[Previous](parserWrapper.md)
