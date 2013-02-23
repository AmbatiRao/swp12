#!/bin/bash

if [ "$#" -lt "1" ]; then
	echo "usage: $0 <input>"
	exit 1
fi

INPUT=$1
sed -e "s/::/ \& /g" -e "s/$/ \\\\\\\\\\\hline/g" "$INPUT"
