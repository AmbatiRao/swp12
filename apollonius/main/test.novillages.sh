#!/bin/bash
DATA="../data/brandenburg.novillages.txt"
WKTS="wkts.novillages"
mkdir -p "$WKTS"
./build/test "$DATA" "$WKTS"
