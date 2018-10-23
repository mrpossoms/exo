#!/bin/sh
cwd=$(pwd)

# the key-file whose value we are looking for
key=$1

while [ $cwd != '/' ]; do
    cwd=$(dirname $cwd)

    if [ -f $cwd/$key ]; then
        cat $cwd/$key
        exit 0
    fi
done
