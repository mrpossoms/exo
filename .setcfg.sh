#!/bin/sh

# arguments
BASE_DIR=$1
PROMPT=$2
NAME=$3

# print prompt
echo $2

read -r value
echo $value > $BASE_DIR/$NAME	
