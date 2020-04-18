// pfamannot
// Protein Family Annotator
//
// source/downloader.cpp
// Jan Hamalcik
//
// Downloads a text file from FTP or HTTP
//

#include "downloader.hpp"

// Informs user about planned procedures and calls them
void Downloader::run() {
  static DownloaderWrapper& wrapper = DownloaderWrapper::getInstance();
  wrapper.set(this);

  std::cout << std::endl;
  std::cout << "Downloading " << name_ << " file from " << url_ << std::endl;

  std::signal(SIGINT, signalHandler);

  downloadFile();
  unzipFile();

  std::cout << std::endl;
  std::cout << name_ << " file downloaded, unzipped and saved here: " << unzipLocation_ << std::endl;
}

// Uses curl API to download a .gz file from FTP or HTTP
void Downloader::downloadFile() {

  CURL *curl;
  FILE *file;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    file = std::fopen(zipLocation_, "w");
    fileIsBeingDownloaded_ = true;

    char errorBuffer[CURL_ERROR_SIZE];


    curl_easy_setopt(curl, CURLOPT_URL, url_);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      std::cerr << "Download was not completed correctly!" << std::endl;
      std::cerr << "Curl error: (" << res << ") " << errorBuffer << std::endl;

      std::fclose(file);
      deleteFile(zipLocation_);

      exit(0);
    }

    curl_easy_cleanup(curl);
    std::fclose(file);
    fileIsBeingDownloaded_ = false;
  }
}

// Inflates downloaded .gz file, saves it in unzipped file and deletes the .gz
// https://stackoverflow.com/a/17062022/12481671
void Downloader::unzipFile() {
  gzFile gz = gzopen(zipLocation_, "r");

  // Downloaded .gz file is not available -> terminate
  if (gz == NULL) {
    std::cerr << "Downloaded " << zipLocation_ << " file is corrupted." << std::endl;
    exit(0);
  }
  /*unsigned */char unzipBuffer[bufferSize_];
  /*unsigned */int unzipBytes;
  std::ofstream unzipFile(unzipLocation_);
  if (unzipFile.good()) {
    fileIsBeingUnzipped_ = true;
    std::cout << "Unzipping " << zipLocation_ << " to " << unzipLocation_ << std::endl;
    for (;;) {
      unzipBytes = gzread(gz, unzipBuffer, bufferSize_);
      if (unzipBytes > 0) {
        char *outputBuffer = unzipBuffer;
        unzipFile.write(outputBuffer, unzipBytes);
      }
      else {
        unzipFile.close();
        gzclose(gz);
        fileIsBeingUnzipped_ = false;
        break;
      }
    }
    int gzErrorNumber;
    std::string gzErrorText = gzerror(gz, &gzErrorNumber);
    if (gzErrorNumber || unzipFile.fail()) {
      std::cerr << "Unzipping was not completed correctly!" << std::endl;
      if (gzErrorNumber) {
        std::cerr << "Zlib error: (" << gzErrorNumber << ") " << gzErrorText << std::endl;
      }
      deleteFile(zipLocation_);
      deleteFile(unzipLocation_.c_str());

      exit(0);
    }
  }

  // Could not create file, where inflated .gz file should have been saved -> terminate
  else {
    std::cerr << "Could not create " << name_ << " file at location " << unzipLocation_ << std::endl;
    exit(0);
  }

  deleteFile(zipLocation_);
}

// Deletes file located at location
void Downloader::deleteFile(const char* location) {
  int status = std::remove(location);
  if (status != 0) {
    std::cerr << "Unable to delete file " << location << std::endl;
  }
}

// If pfamannot is terminated by a termination signal, e.g. CTRL+C, this will delete any incomplete file
void Downloader::signalHandler(int signum) {

  // Enables access to a specific instance of Downloader
  static DownloaderWrapper& wrapper = DownloaderWrapper::getInstance();

  // Incomplete download, incomplete file at zipLocation_ shall be deleted
  if (wrapper.get()->fileIsBeingDownloaded_) {
    std::cerr << std::endl;
    std::cerr << "Interrupt signal " << signum << " recieved." << std::endl;
    std::cerr << "Deleting incompletely downloaded file " << wrapper.get()->zipLocation_ << std::endl;
    deleteFile(wrapper.get()->zipLocation_);
  }

  // Incomplete unzipping, incomplete file at unzipLocation_ shall be deleted
  else if (wrapper.get()->fileIsBeingUnzipped_) {
    std::cerr << std::endl;
    std::cerr << "Interrupt signal " << signum << " recieved." << std::endl;
    std::cerr << "Deleting incompletely unzipped file " << wrapper.get()->unzipLocation_ << std::endl;
    deleteFile(wrapper.get()->unzipLocation_.c_str());
  }

  exit(signum);
}

// Custom function to write downloaded data to file, called by curl
std::size_t Downloader::writeFunction(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  std::size_t written = std::fwrite(ptr, size, nmemb, stream);
  return written;
}
