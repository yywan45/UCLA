#!/bin/bash

option=$1

gcc -std=c99 -o sfrob sfrob.c
printf '*~BO *{_CIA *hXE]D *LER #@_GZY #E\\OX #^BO #FKPS #NEM\4' |
./sfrob | od -ta > sfrob_output.txt

diff -u sfrob_output.txt eggert_output_.txt
rm sfrob_output.txt

map='/proc/self/maps'

cat $map | ./sfrob | od -ta > map_output.txt 

if [ $? -ne 0 ]
then
   echo "Error occurred when reading $map"
fi

jar='/usr/local/cs/jdk*/jre/lib/rt.jar' 

cat $jar | ./sfrob | od -ta > jar_output.txt

if [ $? -ne 0 ]
then
   echo "Error reading $jar"
fi

if [ $# -eq 1 ]
then
    if [ $option -eq 1 ]
    then
	rm *_output.txt
    fi
fi

