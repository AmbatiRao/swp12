#!/bin/bash
DATA="../data/brandenburg.novillages.txt"
WC="0.2"
WT="0.04"
WV="0.01"
FORMAT="wkt"

for WC in $(seq 0.1 0.05 0.5); do
	for WT in $(seq 0.1 0.05 0.3); do
		for WV in $(seq 0.1 0.05 0.2); do
			echo "WC: $WC, WT: $WT, WV: $WV"
			WKTS="wkts.novillages_$WC""_$WT""_$WV"
			mkdir -p "$WKTS"
			./build/create_polygons "$DATA" "$WKTS" "$FORMAT" "$WC" "$WT" "$WV"
		done
	done
done
