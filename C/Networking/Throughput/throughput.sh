#!/bin/bash
#	echo Give me a Number:
#	echo $1
#	read $Number
#	let "Num = $1"
#	val=`seq 1 $Num`
#	echo $val
#	read uu

if [[ -z "$1" || -z "$2" ]];then
echo "usage: ./runtime.sh <packets to send number> <packet message>"
exit
fi
	start_time=$(date +%s.%N)
	packets=1
	let "packets= $packets - 1"
       	for i in `seq 1 $1`
       	do
	let "packets= $packets + 1"
	echo [$packets] `./broadcaster 192.168.1.255 $2`
        done
        finish_time=$(date +%s.%N)
	DIFF=$(echo "$finish_time - $start_time" | bc)
        echo "Time duration: $DIFF seconds"
	echo "Total bits sent:"
	let "bits=${#2} * 8 * $packets"
	echo $bits
echo
echo Total Packets sent: $packets
echo
echo "Bit Rate:"
bitrate=`echo "$bits / $DIFF"| bc` 
echo $bitrate bits/sec

#echo Length of bitrate:
#echo ${#bitrate}

if [[ ${#bitrate} = "4" || ${#bitrate} = "5" || ${#bitrate} = "6" ]];then
bitrate2=`echo "$bitrate / 1024"| bc` 
echo $bitrate2 KBits/sec
fi

if [[ ${#bitrate} = "7" || ${#bitrate} = "8" || ${#bitrate} = "9" ]];then
bitrate2=`echo "$bitrate / 1024 / 1024"| bc` 
echo $bitrate2 MBits/sec
fi

#echo sent message: $2
#echo Message length: ${#2}
