[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/development/downloader.md)
[//]: # (Copyright © 2020 Jan Hamalčík)
[//]: # ()
[//]: # (Programmer's documentation of Downloader class)
[//]: # ()

# `class Downloader`

`class Downloader` uses [curl library](https://curl.haxx.se) to download
a `.gz` file from given URL, specifically for `pfamannot` it downloads
the `uniprot_reference_proteomes.dat.gz` and `Pfam-A.full.gz` files from
The European Bioinformatics Institutes's
[FTP server](ftp://ftp.ebi.ac.uk/pub/databases/Pfam/current_release) (see
[theory](../user/theory.md)) and then uses
[zlib library](https://www.zlib.net) to decompress the downloaded `.gz`
file.
The class is defined in [source/downloader.hpp](../../source/downloader.hpp)
and its methods are implemented in
[source/downloader.cpp](../../source/downloader.cpp).

## Private variables

```cpp
bool fileIsBeingDownloaded_;
```

Variable `fileIsBeingDownloaded_` is preset to `false` and informs
whether a `curl` process is ongoing.
If `SIGINT` interrupt signal is called, `fileIsBeingDownloaded_` is
checked to decide if the downloaded `.gz` file is incomplete and shall
therefore be deleted to prevent unwanted behavior next time `pfamannot`
is run.

---

```cpp
bool fileIsBeingUnzipped_;
```

Variable `fileIsBeingUnzipped_` is preset to `false` and informs whether
a `zlib` process is ongoing.
If `SIGINT` interrupt signal is called, `fileIsBeingUnzipped_` is checked
to decide if the unzipped file is incomplete and shall therefore be
deleted to prevent unwanted behavior next time `pfamannot` is run.

---

```cpp
const char* url_;
```

Constant `url_` points to the URL of the `.gz` file that shall be
downloaded by an instance of `Downloader`.

---

```cpp
const char* zipLocation_;
```

Constant `zipLocation_` points to the path where the downloaded `.gz`
file shall be written.

---

```cpp
const std::string& unzipLocation_;
```

Constant `unzipLocation_` references the path where the downloaded `.gz`
file shall be inflated.

---

```cpp
const std::string& name_;
```

Constant `name_` references human readable name of the file that shall be
downloaded.
It should be either `uniprot_reference_proteomes.dat` or `Pfam-A.full`.

---

```cpp
static const int bufferSize_;
```

Constant `bufferSize_` is preset to `8192` and stores the size of the
buffer that will be used by `zlib`.

---

## Private functions

```cpp
void Downloader::run();
```

Procedure `run()` first initiates the singleton wrapper
`DownloaderWrapper` and sets its private variable
`Downloader* downloader_` to this particular instance.
This is needed because of `SIGINT` interrupt signal handling, so that
the static method `static void Downloader::signalHandler(int signum);`
can access private variables owned by this instance.

The `SIGINT` signal handler is then initiated, and methods
`void Downloader::downloadFile()` and `void Downloader::unzipFile()` are
called to download and inflate the `.gz` file respectively.

---

```cpp
void Downloader::downloadFile();
```

Procedure `downloadFile()` performs a `curl` process which yields the
downloaded `.gz` file.
During the process, `fileIsBeingDownloaded_` should be set to `true`, so
that possible `SIGINT` interrupt signal can be handled properly.
The `.gz` file will be downloaded from `url_` and written to
`zipLocation_`.
See[`curl_easy_setopt`](https://curl.haxx.se/libcurl/c/curl_easy_setopt.html)
to gain in depth knowledge of the curl API.

In case of an curl error during download, the incompletely downloaded
file will be discarded to prevent unwanted behavior and `pfamannot` will
terminate.

---

```cpp
void Downloader::unzipFile();
```

Procedure `unzipFile()` performs a `zlib` process which inflates the
downloaded `.gz` file.
During the process, `fileIsBeingUnzipped_` should be set to `true`, so
that possible `SIGINT` interrupt signal can be handled properly.
The `.gz` file will be inflated from `zipLocation_` to `unzipLocation_`.
See [`libz` interface](https://refspecs.linuxbase.org/LSB_3.0.0/LSB-Core-generic/LSB-Core-generic/libzman.html)
to gain in depth knowledge of the `zlib` library functions `gzopen()`,
`gzread()` and `gzclose()`.

In case of an error while opening `zipLocation_` or `unzipLocation_`,
`pfamannot` will terminate to prevent further undefined behavior.

---

```cpp
static void Downloader::deleteFile(const char* location);
```

Deletes a file from local machine, especially useful with destroying
incompletely processed files to prevent further undefined behavior.
Following parameters have to be specified:

* `const char* location` is the full or relative path to a file that
shall be deleted.

---

```cpp
static void Downloader::signalHandler(int signum);
```

Informs the user about incoming signal, mainly `SIGINT` interrupt signal,
and checks `fileIsBeingDownloaded_` and `fileIsBeingUnzipped_` to delete
any incompletely processed file.
Access to private variables of a specific `Downloader` instance is
granted by the singleton wrapper `DownloaderWrapper`.
`signalHandler()` is passed as an argument to `std::signal`.
Following parameters have to be specified:

* `int signum` is the integer code of a command line signal, for example
the `CTRL`+`C` `SIGINT` interrupt signal.

---

```cpp
static std::size_t Downloader::writeFunction(void *ptr, size_t size, size_t nmemb, FILE *stream);
```

Returns the number of bytes taken care of by `curl`.
See [`CURLOPT_WRITEFUNCTION`](https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html)
to gain in depth knowledge about how `curl` uses write functions.

---

## Constructor

```cpp
Downloader::Downloader(const char* url, const char* zipLocation, const std::string& unzipLocation, const std::string& name);
```

Assigns passed references to all unset private variables:

* `const char* url` is the URL of the `.gz` file that shall be
downloaded.
* `const char* zipLocation` is the path where the downloaded `.gz` file
shall be written.
* `const std::string& unzipLocation` is the path where the downloaded
`.gz` file shall be inflated.
* `const std::string& name` is the human readable name of the file that
shall be downloaded.
It should be either `uniprot_reference_proteomes.dat` or `Pfam-A.full`.

---

[Next](downloaderWrapper.md)

[Previous](architecture.md)
