#!/bin/bash
DATA="../data/brandenburg.novillages.txt"
WKTS="wkts.novillages"
IMG="brandenburg.novillages.png"
mkdir -p "$WKTS"
./build/test7 "$DATA" "$IMG" "$WKTS"
