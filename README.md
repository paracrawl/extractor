# Extractor

Extractor is a tool for fast monolingual data collection from Common Crawl data. It processes Common Crawl WET files, removes invalid UTF-8, identifies languages and outputs file-separated monolingual data. Text is classified by language using Google's CLD2 language detector (https://github.com/CLD2Owners/cld2).


Following is an example of french output file:
```
...
f6fa1abb58549287111ba8d776733e9 uri:http://blogalolotte.canalblog.com/tag/pomme%20de%20terre language:fr bytes:59
pomme de terre : Tous les messages sur pomme de terre - Le
df6fa1abb58549287111ba8d776733e9 uri:http://blogalolotte.canalblog.com/tag/pomme%20de%20terre language:fr bytes:81
...
```

Each header line starts with the same hash code followed by metadata. Then, the extracted language-speific text (here french) starts on a new line and is ended by the next header line.

Apart from this, Extractor can also produce statistics on language distribution per domain.


## Requirements

- GCC, C++11 compiler
- Boost 1.58.0 or later (Boost 1.65.1 or later for LZMA compression)
- Cmake 3.5 or later
- curl, libcurl4-openssl-dev


## Installation

```bash
mkdir -p build && cd build
cmake .. -DWITH_LZMA=off -DBUILD_TEST=on -DCMAKE_BUILD_TYPE=Release
make -j 4
```

`-DWITH_LZMA=on` enables LZMA compression but requires Boost version 1.65.1 or higher.



## Usage

Extractor can be used to process monolingual data with the following command:

```bash
cat <input_file> | ./mono --icompression <compression> --ocompression <compression> --workers <number_of_workers> --output <output_folder>
```

Extractor reads from standard input and expects a line-separated list of path names. Such list is then processed in parallel with the number of workers specified as an argument.

`icompression` and `ocompression` arguments set the input and ouput compresion formats respectively.
The following compression formats are supported: `gzip|bzip2|zlib|lzma|none`.

One folder per worker is created in the output folder and each such folder contains file-separated monolingual data. Processed files are logged in `done.log` file located in each folder.


### Optional arguments:

- --curl: uses cURL to download remote files
- --print_stats: prints language statistics

If `--curl` option is used, the input is expected to be a list of URL addresses instead of a list of path names. Extractor also supports multiple URL sources; If one URL source fails, it attempts to download the content from another one. To define additional sources, just add more URLs on the same line separated by a single space. Any cURL errors are logged in `errors.log` file in each worker folder.

If `--print_stats` option is enabled, it creates additional `stats` folder in each worker folder, and outputs language statistics on the processed files. Each file contains statistics on language distribution per domain with the following format:
```
<domain>	<language code>	<byte length>
```

Each file contains no duplicates and is alphabetically sorted.




## Known Issues

CLD2 detects almost 300 different languages when processing Common Crawl. Since each worker opens a new file for every detected language, the total number of opened files might exceed the user's hard or soft resource limits on open file descriptors. You can type ```ulimit -a``` to get a list of all current resource limits.

For the Common Crawl purposes, executing the following command should increase the limit of open file descriptors sufficiently:
```bash
ulimit -n 32768
```