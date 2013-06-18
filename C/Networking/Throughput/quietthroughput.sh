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
	`./ninjabroadcast 192.168.1.255 $2`
        done
        finish_time=$(date +%s.%N)
	DIFF=$(echo "$finish_time - $start_time" | bc)

	echo
	echo "====================================="
        echo "Time duration: $DIFF seconds"
	let "bits=${#2} * 8 * $packets"
	echo "Total bits sent: $bits"
	echo "Total Packets sent: $packets"
	echo
	echo "Bit Rates:"
	bitrate=`echo "$bits / $DIFF"| bc` 
	echo "    $bitrate bits/sec"

	#echo Length of bitrate:
	#echo ${#bitrate}

	if [[ ${#bitrate} = "4" || ${#bitrate} = "5" || ${#bitrate} = "6" ]];then
	bitrate2=`echo "$bitrate / 1024.0"| bc` 
	echo "    $bitrate2 Kbits/sec"
	fi

	if [[ ${#bitrate} = "7" || ${#bitrate} = "8" || ${#bitrate} = "9" ]];then
	bitrate2=`echo "$bitrate / 1024.0"| bc` 
	echo "    $bitrate2 Kbits/sec"
	bitrate3=`echo "$bitrate / 1024.0 / 1024.0"| bc` 
	echo "    $bitrate3 Mbits/sec"
	fi

	echo "====================================="
	#echo sent message: $2
	#echo Message length: ${#2}
	echo "~Throughput calculator~"
	echo "Throughput = Bitrate * (sniffed packets/sent packets)"
	echo "Please enter the packets seen:"
	read val
	let "tput=$val"

	if [[ ${#bitrate} = "4" || ${#bitrate} = "5" || ${#bitrate} = "6" ]];then
	trate2=`echo "$bitrate / 1024.0 * $tput / $packets"| bc` 
	echo "    $trate2 Kbits/sec"
	fi

	if [[ ${#bitrate} = "7" || ${#bitrate} = "8" || ${#bitrate} = "9" ]];then
	trate2=`echo "$bitrate / 1024.0 * $tput / $packets"| bc` 
	echo "    $trate2 Kbits/sec"
	trate3=`echo "$bittrate / 1024.0 / 1024.0 * $tput / $packets"| bc` 
	echo "    $trate3 Mbits/sec"
	fi
	
	#echo I saw val: $val
