#!/bin/bash
awk -v name=john -v age=12 'BEGIN{print name, age}'
