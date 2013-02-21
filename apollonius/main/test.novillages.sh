#!/bin/bash
DATA="../data/brandenburg.novillages.txt"
WC="0.2"
WT="0.04"
WV="0.01"
WKTS="wkts.novillages_$WC""_$WT""_$WV"
FORMAT="wkt"
mkdir -p "$WKTS"
./build/create_polygons "$DATA" "$WKTS" "$FORMAT" "$WC" "$WT" "$WV"
