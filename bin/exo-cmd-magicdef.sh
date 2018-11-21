#!/bin/bash
source exo-utils.sh

function magic_msg_def {
    name=$1
    magic=$(exo get $name)
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

basepath=$1

invoke help $basepath
invoke usage $basepath


msgs=$(exo rls $basepath/msg | grep .h)

# generate checksums
for msg in $msgs; do
    ext="${msg##*.}"
    echo $(exo magic $msg) > $(echo $msg | sed "s|\.$ext|\.magic|")
done

# print all the symbol flags
SYMBOLS=""
for msg in $msgs; do
    ext="${msg##*.}"
    symbol=$(magic_msg_def $(echo $msg | sed "s|\.$ext|\.magic|"))
    # SYMBOLS="$SYMBOLS $symbol"
    printf "%s " $symbol
done

# printf  $SYMBOLS
