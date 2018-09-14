#!/bin/sh

MSG_DIR=$1
echo "Enter a new message header name"
read -r name
if [ -f $MSG_DIR/$name ]; then
	echo "Error:" $name "already exists"
	exit 1
else
	$(cat ../.editor-pref) $MSG_DIR/$name.h	
fi
