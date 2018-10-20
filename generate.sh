#!/bin/bash

rm LIFO.values
rm Stealing.values
for i in $(seq 1 $1)
do
./workstealing -t $i
./worksharing -t $i
done;
gnuplot trace.gnuplot 
