#!/bin/bash
filename="filenames.txt"
ls -al | grep -v "drwx" | awk '{print $9}' > $filename
sed -n '3p' $filename
echo "Pleae enter your new value: "
read newval
echo "Please enter the line to insert: "
read newline
echo "Inserting: "$newval" into "$filename"["$newline"]..."

echo "Trying:sed "$newline" c\\"$newval $filename"

sed -e '\"$newline c\\\"$newval $filename\"'
