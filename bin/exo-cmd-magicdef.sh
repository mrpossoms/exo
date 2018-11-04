#!/bin/sh

function magic_msg_def {
    name=$1
    magic=$(exo get $name)
    echo "-D"$(basename $name | sed "s|\.|_|" | awk '{print toupper($0)}')=$magic
}

basepath=$1

if [ -z $basepath ]; then
    basepath='.'
fi

msgs=$(exo rls $basepath/msg | grep .h)

# generate checksums
for msg in $msgs; do
    echo $(exo magic $msg) > $(echo $msg | sed "s|\.h|\.magic|")
done

# print all the symbol flags
SYMBOLS=""
for msg in $msgs; do
    symbol=$(magic_msg_def $(echo $msg | sed "s|\.h|\.magic|"))
    # SYMBOLS="$SYMBOLS $symbol"
    printf "%s " $symbol
done

# printf  $SYMBOLS
