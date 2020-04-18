[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/user/theory.md)
[//]: # (Jan Hamalcik)
[//]: # ()
[//]: # (Gives theoretical info about proteins, domains and databases)
[//]: # ()

# Theory

This text will give you some theoretical information to understand the
essence of what `pfamannot` is all about.

## Proteins

Proteins are linear biomacromolecules consisting of sequence of amino
acids.
There are 20 different amino acids found in proteins, each of them has its
name, 3-letter code, 1-letter code and specific physicochemical features.

Name | 3-letter code | 1-letter code | Physicochemical feature
---- | ------------- | ------------- | -----------------------
Alanine | Ala | A | Small, hydrophobic
Valine | Val | V | Small, hydrophobic
Glycine | Gly | G | Small, hydrophobic
Proline | Pro | P | Small, hydrophobic, rigid
Cysteine | Cys | C | Small, sulphuric
Serine | Ser | S | Small, polar
Threonine | Thr | T | Small, polar
Leucine | Leu | L | Hydrophobic
Isoleucine | Iso | I | Hydrophobic
Methionine | Met | M | Hydrophobic, sulphuric
Asparagine | Asn | N | Polar
Glutamine | Gln | Q | Polar
Aspartic acid | Asp | D | Negatively charged
Glutamic acid | Glu | E | Negatively charged
Phenylalanine | Phe | F | Large, aromatic
Tyrosine | Tyr | Y | Large, aromatic
Tryptophan | Trp | W | Large, aromatic
Arginine | Arg | R | Large, positively charged
Lysine | Lys | K | Large, positively charged
Histidine | His | H | Large, positively charged, aromatic

In standard physiological environment, we can postulate Afinsen's dogma,
which states that the native 3D structure (conformation) is determined
only by the protein's amino acid sequence.
This native conformation then determines protein's function.

Proteins can function as enzymes, biocatalysts that accelerate chemical
reaction in living cells, they can serve as signal molecules and
regulators of plenty of biological processes including the cell cycle,
or can build scaffolds and larger molecular complexes, for example
keratin in hairs and nails.

### Domains

A conserved part of protein's amino acid sequence is called a domain,
if it's tertiary structure is stable and folds independently of the rest
of the protein.
Such parts of a protein usually have a specific function conserved among
evolutionary clade, which is however more or less influenced by the rest
of the protein.

One protein can consist of an arbitrary number of domains.
Amino acid sequence, which is not part of any domain in a protein with at
least one domain is called a linker.

Ordered list of domains within a protein make up the protein's
architecture, which is an important measure of protein's similarity.
If two proteins have the same architecture, chances are that they also
share the same function.
This is especially important in determining protein's role in an
organism or in modeling protein's 3D structure.
Regarding this importance, `pfamannot` can be viewed as a very important
tool for bioinformatics.

## Databases

There are plenty of databases storing various data about proteins.
Possibly the most reliable source is
[The European Bioinformatics Institute](https://www.ebi.ac.uk)
which integrates many curated databases.
[UniProt](https://www.uniprot.org) and [Pfam](http://pfam.xfam.org)
are the key reference of `pfamannot` as they contain all essential
information about proteins and domains.

### UniProt

[UniProt](https://www.uniprot.org) database provides a high-quality
comprehensive resource of protein sequence and functional information.
Each entry is encoded by a UniProt ID and contains data collected from
many other reliable sources.

The UniProt Knowledgebase (UniProtKB) aims to act as a central hub of
protein knowledge.
A constitutive cross-section of the taxonomic diversity found within
UniProtKB is called the Reference proteomes and they include the
proteomes of well-studied organisms and other proteomes relevant for
biomedical and biochemical research.
These can be downloaded from The European Bioinformatics Institutes's
[FTP server](ftp://ftp.ebi.ac.uk/pub/databases/Pfam/current_release) as
`uniprot_reference_proteomes.dat`.

### Pfam

[Pfam](http://pfam.xfam.org) is a large collection of protein families
represented by hidden Markov models, each family represents one protein
domain.
Each entry is encoded by a Pfam ID and contains multiple sequence
alignment of all proteins containing given domain.
These can also be downloaded from The European Bioinformatics
Institutes's
[FTP server](ftp://ftp.ebi.ac.uk/pub/databases/Pfam/current_release)
under `Pfam-A.full`.

## `pfamannot`

`pfamannot` retrieves information both from
[UniProt](https://www.uniprot.org) and [Pfam](http://pfam.xfam.org)
and combines them in a useful manner.
Given a set of Pfam IDs that a protein must contain and another set of
Pfam IDs that the same protein must not contain, `pfamannot` will return
all such proteins found in UniProtKB.
Each protein will be described by an organism, in which it has been
found, complete architecture with domain span within the sequence, and
the sequence itself.

Said list of annotated proteins can serve as a starting point for many
evolutionary studies.
For example, it has been used in a study of domain's function
dependency on linker composition.
To perform such research, a frequent domain was chosen as a model, in
this case the protein kinase domain.
This set was then annotated, linker and domain sequences were extracted
and bioinformatical analysis was carried out on a restricted set based
on protein's origin.

---

[Next](usage.md)
