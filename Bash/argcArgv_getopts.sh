#!/bin/bash
FORMAT=0
while getopts :u:d:p:f option; # First : == NO ERRORS; Variable names; IF : present => value being passed
do
        case $option in
                u) USER=$OPTARG;; #VALUE
                d) DATE=$OPTARG;; #VALUE
                p) PRODUCT=$OPTARG;; #VALUE
                f) FORMAT=1;; #FLAG
        esac
done

echo "u: $USER"
echo "d: $DATE"
echo "p: $PRODUCT"
echo "f: $FORMAT"
