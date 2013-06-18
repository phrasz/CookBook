#!/bin/bash
filename="filenames.txt"
ls -al | grep -v "drwx" | awk '{print $9}' > $filename
awk NR==3 $filename

