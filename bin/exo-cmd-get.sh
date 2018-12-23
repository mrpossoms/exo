#!/bin/bash
source exo-utils.sh
cwd=$(pwd)

function help
{
    echo 'Takes a file path, and returns the contents'
}

function usage
{
    echo 'get [file-path]'
}

# the key-file whose value we are looking for
key="$1"

invoke help $key
invoke usage $key

while [ $cwd != '/' ]; do
    cwd=$(dirname $cwd)

    if [ -f $cwd/$key ]; then
        cat $cwd/$key
        exit 0
    fi
done
