#!/bin/bash
tmp=$(mktemp -d)
cd $tmp
touch -d '1918-11-11 11:00 GMT' wwi-armistice
touch now
sleep 1
touch now1
TZ=UTC0 ~/Classes/CS35L/Assignment5/coreutils/bin/ls -lt --full-time wwi-armistice now now1
cd ~/Classes/CS35L/Assignment5/
rm -rf $tmp

