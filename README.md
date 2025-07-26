# Indexer On Demand (IOD)
A simple command-line utility that builds index files that can later be used to query for individual tokens. Works on both individual files and files in a folder.

**NOTE** 
<br>
Curently supports only text-based files with `.txt` extension. WIP to add extensibility in parsing and other areas.

## Build steps
This is a CMake project. <br>
The general process to build it are as follows:

1. 
```bash
cd IOD/
```
2. 
```bash
mkdir build/
cd build/
```
3. 
```bash
cmake ..
```
4. 
```bash
make
```
5. Now you have your `iod` binary in the `build/` folder.

## Usage
Run the binary with `help` argument to get a simple usage guide.

The following are the commands currently supported:

### `index` command
This is used to index a given file/folder and build an index file. This binary index file has a `.iix` extension. The index file has the following structure:

```cpp
SIGNATURE: ".IIX"
DOC_COUNT: size_t
[
    DOC_ID: long
    PATH_SIZE: size_t
    PATH_STR: string[PATH_SIZE]     // absolute path
    DOC_TOK_CNT: size_t
]
INDEX_SIZE: size_t
[
    TOKEN_SIZE: size_t
    TOKEN_STR: string[TOKEN_SIZE]
    REF_LIST_SIZE: size_t
    [
        DOC_ID: long
        POS_CNT: size_t
        [
            POS: size_t
        ]
    ]
]
```

This file is fed as input to the binary to query for strings.

### `search` command (one-off)
This command is used to perform a search in a given index file (generated with the `index` command) for a specified query. The structure of this usage is:

```bash
iod search myIndex.iix <things_to_search>
```

This will return any hits for the individual tokens in the input.

### `search` command (interactive)
This is similar to its one-off variant, with the only difference being a simple interactive shell that allows to run multiple queries on the same index file, until it is explicitly quit (by entering `qq` as query input).

It can be used by not providing the search string in the one-off search command.

```bash
iod search myIndex.iix
```
