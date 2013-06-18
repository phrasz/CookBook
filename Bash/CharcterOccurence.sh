#!/bin/bash
echo "When running ls there are :"
echo $(ls -al | wc -c) - $(ls  -al | tr -d 'o' | wc -c) | bc
echo "o's"
