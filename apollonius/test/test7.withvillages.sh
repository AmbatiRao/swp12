#!/bin/bash
DATA="../data/brandenburg.withvillages.txt"
WKTS="wkts.withvillages"
IMG="brandenburg.withvillages.png"
mkdir -p "$WKTS"
./build/test7 "$DATA" "$IMG" "$WKTS"
