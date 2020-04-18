[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (README.md)
[//]: # (Copyright (c) 2020 Jan Hamalčík)
[//]: # ()
[//]: # (Contains basic information about pfamannot's usage and installation)
[//]: # ()

# Protein Family Annotator (`pfamannot`)

A command line tool for bioinformatics.

## Introduction

Most information about proteins is stored in the
[UniProt](https://www.uniprot.org) database.
However, a complete architecture of a protein based on protein families
(domains) from the [Pfam](http://pfam.xfam.org) database is usually not
present and to retrieve all annotated proteins (not) containing given
domains is not that straightforward.

Protein Family Annotator completes exactly this task.
It downloads `Pfam-A.full` and `uniprot_reference_proteomes.dat` files
(if not already downloaded by the user), scans for proteins (not)
containing user-specified [Pfam](http://pfam.xfam.org) domains and
outputs them annotated in following format:

UniProt ID | Organism | First domain ID | First domain starting position | First domain ending position | ... | Last domain ID | Last domain starting position | Last domain ending position | Sequence
---------- | -------- | --------------- | ------------------------------ | ---------------------------- | --- | -------------- | ----------------------------- | --------------------------- | --------
`ABCDEF_GHIJ` | `Eukaryota;Metazoa;...` | `PFxxxxx` | `24` | `156` | `...` | `PFxxxxx` | `486` | `633` | `MFHLVA...DECYWL`
`KLMNOP_QRST` | `Archaea;Asgardgroup;...` | `PFxxxxx` | `11` | `209` | `...` | `PFxxxxx` | `789` | `941` | `MTGIIT...QPSCAY`

## Requirements

Read [requirements](docs/install/requirements.md) to get informed about
what is required to run `pfamannot`.

## Installation

Read [installation](docs/install/installation.md) to get informed about
how to install `pfamannot`.

## Usage

Read [user documetnation](docs/user) to get informed about everyday usage
of `pfamannot`.

## Development

Read [programmer's documentation](docs/development) to obtain in depth
knowledge of `pfamannot`'s source code.

## License

Read [LICENSE](LICENSE) to get informed about licensing, distribution and allowed usage
of `pfamannot`.

## Authors

### Jan Hamalcik

Bioinformatics student at Charles University.

jan.hamalcik@icloud.com

## Project status

Ready to submit...
