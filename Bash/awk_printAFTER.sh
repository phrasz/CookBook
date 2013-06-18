#!/bin/bash

# have: ... | awk '{print $9}'
# WANT ==> everything AFTER, and including, column 9
string=`ls -alhtr | grep -v drwx | grep rwx | awk '{print substr($0, index($0,$5))}'`
echo "$string"

echo $string | awk '{print "Size: "$1" Date: "$2$3$4$5}'
#awk '{ s = ""; for (i = 9; i <= NF; i++) s = s $i " "; print s }
