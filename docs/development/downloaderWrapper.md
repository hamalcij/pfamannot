[//]: # (pfamannot)
[//]: # (Protein Family Annotator)
[//]: # ()
[//]: # (docs/development/downloaderWrapper.md)
[//]: # (Copyright (c) 2020 Jan Hamalčík)
[//]: # ()
[//]: # (Programmer's documentation of DownloaderWrapper class)
[//]: # ()

# `class DownloaderWrapper`

`class DownloaderWrapper` is a singleton wrapper wrapping
`class Downloader`.
It allows the static method
`static void Downloader::signalHandler(int signum)` to access private
variables of a specific instance of `Downloader` needed to handle a
`SIGINT` interrupt signal raised by the user during download or
inflating.
The class is defined in [`source/downloader.hpp`](../../source/downloader.hpp).

## Private variables

```cpp
Downloader* downloader_;
```

Variable `downloader_` points to current instance of `Downloader`, thus
allowing only one instance at one time.
Through this pointer, static methods defined in `Downloader` can access
private data of the current instance.

---

## Public functions

```cpp
static DownloaderWrapper& DownloaderWrapper::getInstance();
```

Returns static reference to an instance of `DownloaderWrapper`, wrapping
around an instance of `Downloader`.

---

```cpp
void DownloaderWrapper::set(Downloader* downloader);
```

Procedure `set()` sets the pointer `downloader_` to point to an instance
of `Downloader`. Following parameters have to be specified:

* `Downloader* downloader` is the pointer to a specific `Downloader`
instance.
This shall be set from the instance itself using the keyword `this`.

---

```cpp
void Downloaded* DownloaderWrapper::get();
```

Returns a pointer to private variable `downloader_`.

---

[Next](parameters.md)

[Previous](downloader.md)
