[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/user/usage.md)
[//]: # (Jan Hamalcik)
[//]: # ()
[//]: # (Informs about standard usage)
[//]: # ()

# Usage

To run Protein Family Annotator, open terminal and simply type:
```
pfamannot
```

A greeting message will appear and you will be asked to write out all
Pfam IDs you want to include in your proteins.
Separate these by a space and then press *enter*, e.g.:
```
PF00069 PF00780 PF00002
```

Then you will be asked to write out all Pfam IDs you want to exclude in
your proteins, for example `PF03291`.
Again, separate them by a space and press *enter*.
If you don't want to exclude any Pfam IDs from the search, leave the line
blank and press *enter*.

If this is the first time you run `pfamannot` on your local machine
(or if you invoked the option `-r`), you will be asked, if you want both
`Pfam-A.full` and `uniprot_reference_proteomes.dat` files to be
automatically downloaded by `pfamannot`.
Answer by typing `yes` or `no`.
In case `pfamannot` shall download a file, you have to provide full path
to the folder where you want the file to be downloaded and extracted.
Expect that you will need about 90 GB free local storage space to get
the `Pfam-A.full` file and about 180 GB free local storage space to get
the `uniprot_reference_proteomes.dat` file.

If you happen to already have these files on your machine or if you
decide to download them yourself from
The European Bioinformatics Institutes's
[FTP server](ftp://ftp.ebi.ac.uk/pub/databases/Pfam/current_release),
answer `no` and provide full or relative path to these files.

Finally, provide the full or relative path to the output file upon
being asked, where your proteins will be annotated.
CAREFUL! If you provide a path to an existing file, its content will be
lost!

After pressing *enter*, you should see a summary.
In case you are not seeing any summary and `pfammanot` terminated, check
the error message, which should give you a hint on what has gone wrong.
For example:
```
Could not open file /path/to/nonexistent/location

pfamannot will terminate...
```

Go through the summary very carefully to prevent some unwanted behavior
due to some typo or other human error, the consequences of such mistake
can first show up after hours of `pfamannot`'s run.
If everything looks fine, type `yes` to initiate the annotation.

If this is the first time you run `pfamannot` on your local machine
(or if you invoked the option `-r`), you will also be asked if you want
`pfamannot` to save parsed data on your local machine.
As the parsing of `Pfam-A.full` file always returns the same data
structure used to traverse the `uniprot_reference_proteomes.dat` file,
it may be practical to save this data structure on your local machine.
This is especially useful if you aim to run `pfamannot` more than once,
since parsing the `Pfam-A.full` file is much slower than restoring
the already saved data structure.
Answer `yes` or `no`.

`pfamannot` will now annotate proteins containing all wanted domains
and at the same time not containing any unwanted domain.
You will be continuously informed about `pfamannot`'s progress.
When the parsing is completed, you can find your annotated proteins
in the output file.

---

[Next](options.md)

[Previous](theory.md)
