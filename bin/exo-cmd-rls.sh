#!/bin/bash
source exo-utils.sh

function help {
    echo "Performs a 'recursive' listing of directories and files"
    exit 0
}

function usage {
    echo "rls [path]"
    exit 0
}

path=$1/

invoke help $1
invoke usage $1

files=$(ls $path)

# echo $files

for file in $files; do
    file=$path$file

    if [ -f $file ]; then
        echo "$file"
    elif [ -d $file ]; then
        $0 $file
    fi

done

