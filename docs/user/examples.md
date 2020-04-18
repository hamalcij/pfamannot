[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/user/examples.md)
[//]: # (Copyright © 2020 Jan Hamalčík)
[//]: # ()
[//]: # (Gives some correct and incorrect example of pfamannot's call)
[//]: # ()

# Examples

In this section, some correct and incorrect examples of calling
`pfamannot` from the command line are showed, incorrectness will be
explained.

## Correct examples

```
pfamannot
pfamannot -a
pfamannot -d
pfamannot -r
pfamannot -ar
pfamannot -ra
pfamannot -dr
pfamannot -rd
pfamannot -i PF00069
pfamannot -e PF03291 -i PF00069 PF00780
pfamannot -i PF00780 -e PF00032 -i PF00002
pfamannot -i PF00069 -o /path/to/output_file
pfamannot --uniprot-file=relative/path/to/uniprot_file -d -r
pfamannot -p relative/path/to/pfam_file -e PF03291 -i PF00002 -u /path/to/uniprot_file
pfamannot -o /path/to/output/file -r -i PF00002 --uniprot_file=/path/to/uniprot_file -e PF00032 -i PF00780
pfamannot -i PF00069 PF00780 --output-file=/path/to/output_file -e PF03291 -u relative/path/to/uniprot_file --pfam-file=relative/path/to/pfam_file
```

## Incorrect examples

```
pfamannot -c
>>  Option -c is invalid.
```
You cannot use any undefined options.

---
```
pfamannot hello
>>  Argument hello is invalid.
```
You cannot use any undefined arguments.

---
```
pfamannot -a-r
>>  New option must be separated by a space!
```
Either write `-a -r` or `-ar`.

---
```
pfamannot -ri PF00069
>>  Option -i must stand alone.
```
Even though `-r` may be together with other options after one *hyphen*,
`-i` cannot.

---
```
pfamannot -ePF03291
>>  Option -e must stand alone.
```
Pfam ID has to be separated from its option by a space.

---
```
pfamannot -i PF1234
>>  Pfam ID PF1234 is invalid!
```
Correct Pfam ID format is `PFxxxxx`, where `x` is a digit.

---
```
pfamannot -output-file=/path/to/file
>>  Option -o must stand alone.
```
Long options must begin with two consecutive *hyphens*: `--`

---
```
pfamannot --pfam-file /path/to/file
>>  Argument /path/to/file is invalid.
```
Long options must have an equal sign `=` between them and their argument.

---
```
pfamannot -u relative/path/to/file --uniprot-file=relative/path/to/file
>>  uniprot_reference_proteomes.dat file specified more than once.
```
Any file may be specified only once.

---
```
pfamannot -i PF00069 -e PF03291 -r -p relative/path/to/file --uniprot-file=/path/to/file -i PF00780 -o
>>  No output file specified.
```
If you write a file option, you also have to specify the path to the
file.

---
```
pfamannot -o /path/to/file -d --uniprot-file=relative/path/to/file -r -e PF00032 -p relative/path/to/file
>>  You must not specify any Pfam ID, if you want to choose the option -d
```
If option `-a` or `-d` is chosen, you cannot specify any Pfam ID.

---

[Previous](options.md)
