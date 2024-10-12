# restore-mtime
The tool allows you to save modification time information of files and restore them later.

### usage
 - subcommand: **save** *[--add-directory VAR]... [--add-file VAR]... --output VAR*
```
Generates a database that stores the mtime of files.

Optional arguments:
  -d, --add-directory  Add a directory. [nargs=0..1] [default: {"."}] [may be repeated]
  -f, --add-file       Add a file. [may be repeated]
  -o, --output         Where to save the database file? [required]
```
 - subcommand: **restore** *[--base-dir VAR] --input VAR*
```
Restore the file's mtime from the database.

Optional arguments:
  -b, --base-dir  Set the base path for restore. [nargs=0..1] [default: "."]
  -i, --input     Where is the database that stores file mtime? [required]
```

### use with `actions/cache`

> To be honest, this tool is designed to be used with *actions/cache*.

When building a C++ project, the build system uses mtime (modification time of the file) to decide whether to do an incremental build. When we try to use *actions/cache* to save the build cache and restore it the next time, the build system often does a fresh build because *actions/cache* only simply downloads the file and puts it in the specified location - in other words, the mtime of the file is changed.

The problem can be solved by simply changing the `build.yml`: 
```diff
# For reference only, some configurations have been omitted
# Assume:
#   Build directory is: build
#   Source code directory is: src

name: Build

on:
...

+ env:
+   RESTORE_MTIME_VERSION: 0.1.0

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4

      - uses: xmake-io/github-action-setup-xmake@v1
            
      - uses: actions/cache@v4
        with:
          path: |
            build
          key: build-${{ github.run_id }}
          restore-keys: |
            build-

+     - name: Setup restore-mtime
+       run: |
+         wget -O restore-mtime.zip https://github.com/Redbeanw44602/restore-mtime/releases/download/v$RESTORE_MTIME_VERSION/restore-mtime-debug-linux-x64.zip
+         unzip restore-mtime.zip
+         chmod +x restore-mtime
+
+         # test
+         ./restore-mtime -v
+
+     - name: Restore cache's mtime
+       if: ${{ hashFiles('build/saved_mtime_db') != '' }} # is exists
+       run: |
+         ./restore-mtime restore -b src -i build/saved_mtime_db

      - run: |
          xmake f

      - run: |
          xmake build

+     - name: Save cache's mtime
+       run: |
+         ./restore-mtime save -d src -o build/saved_mtime_db

      - uses: actions/upload-artifact@v4
...
```

Hmm, this might be a bit bloated, I will study how to package it into a custom step later.

### technical details
 - Files with the same hash value are considered to be the same file.
 - The mtime of all files is saved in text format. You can view the details in the output file.
 - Only process *regular files* (skip symlinks/block devices/...)

### license 
Open source under the *MIT license*.

