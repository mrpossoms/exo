#!/bin/sh

msg_name=$1

cksum $msg_name | awk '{split($0,a," "); print a[1]}'
