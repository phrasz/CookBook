#!/bin/bash
LinesReturned=$(ls|wc -l)
echo ""

echo "Lines returned: " $LinesReturned

echo ""
echo ""

if [ $LinesReturned == "24" ]
	then
		echo "There are Ten lines!"
	else
		echo "There are NOT Ten Lines!"
		echo "There are " $LinesReturned " Lines."
fi


