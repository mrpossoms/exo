#!/bin/sh

path=$1/

# echo "enter: $path"

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

