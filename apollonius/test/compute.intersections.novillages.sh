#!/bin/bash

BOUND="data/Brandenburg.hull.simple.0.001.wkt"
DIR1="wkts.novillages"
DIR2="wkts.novillages.cut"

./compute.intersections.sh "$BOUND" "$DIR1" "$DIR2"
