#!/bin/bash

START_TIME=$SECONDS
LC_ALL=C sort /input/data-2GB.in -o/tmp/data-2GB.in
ELAPSED_TIME=$(($SECONDS-$START_TIME))
echo "$ELAPSED_TIME"
valsort /tmp/data-2GB.in