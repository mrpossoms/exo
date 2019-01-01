#!/bin/bash
source exo-utils.sh

function help
{
    echo 'Takes a file path, or list of file paths, and returns a checksum value for that file'
}

function usage
{
    echo 'magic [file-path]'
}


$(invoke help "$1")
help_shown=$?
$(invoke usage "$1")
usage_shown=$?

if [ $help_shown -gt 0 ] || [ $usage_shown -gt 0 ]; then
	exit 0
fi

files=$@
prefix=
if [ -z "$files" ]; then
	files=$(exo rls $EXO_ROOT/inc | grep '.hpp')
	prefix="-DEXO_MAGIC="
fi

sum=0
for file in $files; do
	chk=$(cksum $file | awk '{split($0,a," "); print a[1]}')
	sum=$(($sum ^ $chk))
done

printf "%s%s" $prefix $sum
