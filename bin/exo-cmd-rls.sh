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
include_files=true

invoke help "$1"
invoke usage "$1"

if [ '-d' = $2 ]; then
    include_files=false
fi

files=$(ls $path)

# echo $files

for file in $files; do
    file=$path$file

    if [ -f $file ]; then
        if [ $include_files = true ]; then
            echo "$file"
        fi
    elif [ -d $file ]; then
        if [ $include_files = false ]; then
            echo "$file"
        fi        
        $0 $file $2
    fi

done

