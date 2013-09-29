#!/bin/bash
filename="filenames.txt"
ls -al | grep -v "drwx" | awk '{print $9}' > $filename
LineNumber=3
LineNumber2=5
awk NR==$LineNumber || NR==$LineNumber2 $filename

