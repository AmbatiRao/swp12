#!/bin/bash

BOUND="data/Brandenburg.hull.simple.0.001.wkt"
DIR1="wkts.withvillages"
DIR2="wkts.withvillages.cut"

./compute.intersections.sh "$BOUND" "$DIR1" "$DIR2"
