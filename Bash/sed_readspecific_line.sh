#!/bin/bash
filename="filenames.txt"
ls -al | grep -v "drwx" | awk '{print $9}' > $filename
sed -n '3p' $filename
