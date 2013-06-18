#!/bin/bash
date=`date | awk '{print $4}'`
echo "This is the time: $date"
echo "This is the time on tr: "`echo $date | tr : " "`
