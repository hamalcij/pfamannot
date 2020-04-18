[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/install/installation.md)
[//]: # (Jan Hamalcik)
[//]: # ()
[//]: # (Guide to install pfamannot)
[//]: # ()

# Installation

If the requirements specified [here](requirements.md) are being met, you
may continue with the installation of `pfamannot`.
First of all, download the [archive](https://github.com/hamalcij/pfamannot/archive/master.zip)
with `pfamannot`'s source from GitHub, navigate to your Downloads folder,
extract the .zip file and change current directory to the extracted
folder by typing into the command line:
```
unzip pfamannot-master.zip
cd pfamannot-master
```

Next you will run the `configure` executable, which will prepare the
`Makefile`.
Following configuration is possible (you can invoke this info by typing
`./configure --help`):
```
--prefix=<path>: Installation prefix (DEFAULT = "/usr/local")
--enable-debug: Include debug symbols
--disable-debug: Do not include debug symbols (DEFAULT)
```

The `configure` executable has now generated the `Makefile`.
You can now build `pfamannot` in the current directory by typing
`sudo make` (you don't need `sudo` if `zlib` and `libcurl` are installed
on your machine) and/or install it locally (usually in
`/usr/local/bin/pfamannot`) by typing `sudo make install`.
The installation process can be summed up by the following three commands:
```
./configure [options]
sudo make             // you will need sudo if curl or zlib are not installed on your machine
sudo make install
```

If you do not intend to experiment with the source code and rebuild
`pfamannot` after the installation, you can remove some redundant files
with
```
make clean
make cleandep
```

To uninstall `pfamannot`, navigate to the `pfamannot-master` directory
and simply type
```
sudo make uninstall
```

---

[Previous](requirements.md)
