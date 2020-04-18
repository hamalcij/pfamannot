[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/user/options.md)
[//]: # (Copyright © 2020 Jan Hamalčík)
[//]: # ()
[//]: # (Informs about options that can be invoked from the command line)
[//]: # ()

# Options

Using options when starting `pfamannot` from the command line lets you
specify all information mentioned above, and allows for invoking special
features that else cannot be activated.

## `-i [Pfam IDs]`

Include set of Pfam IDs separated by a space.

Only proteins containing all these domains will be considered for the
search.

This option must stand alone, but you can use it more than once.
This option cannot be combined with options `-a` and/or `-d`.
Following code shows some correct examples:
```
pfamannot -i PF00069
pfamannot -i PF00069 PF00780 PF00002
pfamannot -i PF00069 -i PF00780 PF00002
```

## `-e [Pfam IDs]`

Exclude set of Pfam IDs separated by a space.

Only proteins not containing any of these domains will be considered  for
the search.

This option must stand alone, but you can use it more than once.
This option cannot be combined with options `-a` and/or `-d`.
Following code shows some correct examples:
```
pfamannot -e PF03291
pfamannot -e PF03291 PF00009 PF00032
pfamannot -e PF03291 PF00009 -e PF00032
```

## `-a`

Annotate all proteins.

All proteins from `uniprot_reference_proteomes.dat` file will be
annotated, even those without any domain.

This option can stick together after a single *hyphen* with other
options with this property, but you may use it only once.
This option cannot be combined with options `-d`, `-i` and/or `-e`.
Following code shows some correct examples:
```
pfamannot -a
pfamannot -ra
```

## `-d`

Annotate all proteins with at least one domain.

All proteins appearing at least once in `Pfam-A.full` file will be
annotated.

This option can stick together after a single *hyphen* with other
options with this property, but you may use it only once.
This option cannot be combined with options `-d`, `-i` and/or `-e`.
Following code shows some correct examples:
```
pfamannot -d
pfamannot -dr
```

## `-p /path/to/file`, `--pfam-file=/path/to/file`

Provide path to `Pfam-A.full` file.

Specify relative or full path to the `Pfam-A.full` file on your local
machine.

This option must stand alone and you can only use it once.
Do not use this option if you want `pfamannot` to download the
`Pfam-A.full` file.
Following code shows some correct examples:
```
pfamannot -p /full/path/to/Pfam-A.full
pfamannot -p relative/path/to/Pfam-A.full
pfamannot --pfam-file=/full/path/to/Pfam-A.full
pfamannot --pfam-file=relative/path/to/Pfam-A.full
```

## `-u /path/to/file`, `--uniprot-file=/path/to/file`

Provide path to `uniprot_reference_proteomes.dat` file.

Specify relative or full path to the `uniprot_reference_proteomes.dat`
file on your local machine.

This option must stand alone and you can only use it once.
Do not use this option if you want `pfamannot` to download the
`uniprot_reference_proteomes.dat` file.
Following code shows some correct examples:
```
pfamannot -u /full/path/to/uniprot_reference_proteomes.dat
pfamannot -u relative/path/to/uniprot_reference_proteomes.dat
pfamannot --uniprot-file=/full/path/to/uniprot_reference_proteomes.dat
pfamannot --uniprot-file=relative/path/to/uniprot_reference_proteomes.dat
```

## `-o /path/to/file`, `--output-file=/path/to/file`

Provide path to the output file.

Specify relative or full path to the file, where the proteins will be
annotated.
CAREFUL!
If you provide a path to an existing file, its content will be
lost!
Do not introduce any whitespace in the output file's name!

This option must stand alone and you can only use it once.
Following code shows some correct examples:
```
pfamannot -o /full/path/to/output_file
pfamannot -o relative/path/to/output_file
pfamannot --output-file=/full/path/to/output_file
pfamannot --output-file=relative/path/to/output_file
```

## `-r`

Reset `pfamannot`.

Forces `pfamannot` to destroy the locally saved data structure from
previous `Pfam-A.full` parsing, thus implying that `Pfam-A.full` file
will be parsed de novo.
Useful when The European Bioinformatics Institutes issues a new release
of the `Pfam-A.full` file or if you suspect that the saved data structure
is for some reason corrupted.

This option can stick together after a single *hyphen* with other
options with this property, but you may use it only once.
Following code shows some correct examples:
```
pfamannot -r
pfamannot -ar
```

---

[Next](examples.md)

[Previous](usage.md)
