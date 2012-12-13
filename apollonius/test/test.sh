#!/bin/bash

for CIN in "data/"*.cin; do
	echo "input: $CIN";
	BASE=$(basename "$CIN" ".cin");
	IMAGE="$BASE.png";
	echo "image: $IMAGE";
	
	./build/test5 "$CIN" "$IMAGE"
done
