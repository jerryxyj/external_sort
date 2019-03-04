#!/bin/bash

START_TIME=$SECONDS
LC_ALL=C sort /input/data-20GB.in -o/tmp/data-20GB.in
ELAPSED_TIME=$(($SECONDS-$START_TIME))
echo "$ELAPSED_TIME"
valsort /tmp/data-20GB.in
