#!/bin/bash
source exo-utils.sh

function magic_msg_def {
    name="$1"
    magic="$2"
    echo "-D"$(basename $name | sed "s|\.|_|" | awk '{print toupper($0)}')=$magic
}

function help
{
    echo 'Takes a file path, and returns the contents'
    exit 0
}

function usage
{
    echo 'get [file-path]'
    exit 0
}

basepath="$1"
file_regex="$2"

invoke help $basepath
invoke usage $basepath

# if a grep style regex wasn't passed to match files just look for headers
if [ -z $file_regex ]; then
    file_regex='\.h'
fi

chksum_files=$(exo rls $basepath | egrep $file_regex)

# generate checksums
# for msg in $chksum_files; do
#     ext="${msg##*.}"
#     echo $(exo magic $file) > $(echo $msg | sed "s|\.$ext|\.magic|")
# done

# print all the symbol flags
SYMBOLS=""
for file in $chksum_files; do
    ext="${file##*.}"
    #symbol=$(magic_msg_def $(echo $msg | sed "s|\.$ext|\.magic|"))
    magic_name=$(echo $file | sed "s|\.$ext|\.magic|")
    symbol=$(magic_msg_def $magic_name $(exo magic $file))
    # SYMBOLS="$SYMBOLS $symbol"
    printf "%s " $symbol
done

# printf  $SYMBOLS
