#!/bin/bash
echo "These are the files:"
ls
echo "This is the last file: "
ls | awk ' { print ( $(NF-1) ) }'
