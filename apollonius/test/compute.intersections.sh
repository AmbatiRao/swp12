#!/bin/bash

if [ "$#" -lt "3" ]; then
	echo "usage $0 <BOUNDARY> <DIR INPUT> <DIR OUTPUT>"
	exit 1
fi

BOUND="$1"
DIR1="$2"
DIR2="$3"

mkdir -p "$DIR2"

for i in "$DIR1"/*; do
	bi=$(basename "$i");
	o="$DIR2""/""$bi"
	echo "$i -> $o"
	GeometryOperation -operation intersection "$o" "$BOUND" "$i"
done
