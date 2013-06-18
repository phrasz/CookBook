#!/bin/bash

RANGE=300
echo "This is your number(Range 1=300):"

number=$RANDOM
let "number %= $RANGE"
echo $number
