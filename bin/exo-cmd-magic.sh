#!/bin/bash
source exo-utils.sh

function help
{
    echo 'Takes a file path, and returns a checksum value for that file'
}

function usage
{
    echo 'magic [file-path]'
}

# the key-file whose value we are looking for
msg_name="$1"

invoke help $msg_name
invoke usage $msg_name

cksum $msg_name | awk '{split($0,a," "); print a[1]}'
