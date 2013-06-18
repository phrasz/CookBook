#!/bin/bash
ls -alhtr | grep -v drwx | grep rwx | awk '{print "[Size: "$5"] [Date: "$6" "$7" "$8"] [File name: "$9"]"}'

