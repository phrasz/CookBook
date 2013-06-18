#!/bin/bash
#	echo Give me a Number:
#	echo $1
#	read $Number
#	let "Num = $1"
#	val=`seq 1 $Num`
#	echo $val
#	read uu

if [ -z "$1" ];then
echo "usage: ./runtime.sh <number>"
exit
fi
	start_time=$(date +%s.%N)
       	for i in `seq 1 $1`
       	do
	clear
        done
        finish_time=$(date +%s.%N)
	DIFF=$(echo "$finish_time - $start_time" | bc)
        echo "Time duration: $DIFF seconds"
