// pfamannot
// Protein Family Annotator
//
// source/downloader.h
// Copyright (c) 2020 Jan Hamalčík
//
// Downloads a text file from FTP or HTTP
//

#include<curl/curl.h>
#include<zlib.h>
#include<cstdio>
#include<string>
#include<iostream>
#include<fstream>
#include<csignal>

class Downloader;

// Singleton wrapper allowing to handle the right instance for signal handling
// https://stackoverflow.com/a/37861144/12481671
class DownloaderWrapper {
public:
  DownloaderWrapper() = default;

  static DownloaderWrapper& getInstance() {
    static DownloaderWrapper* w = new DownloaderWrapper();
    return *w;
  }

  void set(Downloader* downloader) {
    downloader_ = downloader;
  }

  Downloader* get() {
    return downloader_;
  }

private:
  Downloader* downloader_;
};

//
class Downloader {
public:
  Downloader(const char* url, const char* zipLocation, const std::string& unzipLocation, const std::string& name) : url_(url), zipLocation_(zipLocation), unzipLocation_(unzipLocation), name_(name) { run(); }

private:
  void run();
  void downloadFile();
  void unzipFile();

  static void deleteFile(const char* location);
  static void signalHandler(int signum);
  static std::size_t writeFunction(void *ptr, size_t size, size_t nmemb, FILE *stream);

  bool fileIsBeingDownloaded_{false};
  bool fileIsBeingUnzipped_{false};

  const char* url_;
  const char* zipLocation_;
  const std::string& unzipLocation_;
  const std::string& name_;

  static const int bufferSize_{8192};
};
