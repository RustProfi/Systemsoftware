#!/bin/sh
echo test started
START=$SECONDS
counter=1
for i in $(seq 1 10000)
do
curl  -s 127.0.0.1:8000/FScounter > /dev/null
((counter++))
done
ELAPSED=$(($SECONDS - $START))
echo "FScounter:"
echo $ELAPSED

START=$SECONDS
counter=1
for i in $(seq 1 10000)
do
curl  -s 127.0.0.1:8000/MEMcounter > /dev/null
((counter++))
done
ELAPSED=$(($SECONDS - $START))
echo "MEMcounter:"
echo $ELAPSED
