#!/bin/bash
#ls -la >> temp_array_ex
names_array=(save this)
echo "Looking for files from MAY"
lines=`ls -al | grep May | grep -v "dwrx" | wc -l`
echo "Lines to Process: $lines"
echo "saving Array.."
names_array=(`ls -al | grep May | grep -v "drwx" | awk '{print $9}'`)
#rm temp_array_ex
echo "This is the array size: "${#names_array[*]}
adjusted_lines=${#names_array[*]}
adjusted_lines=$(( adjusted_lines - 1 ))

	for i in `seq 0 $adjusted_lines`;
	do
                echo "names_array["$i"]: "${names_array[$i]}
        done

