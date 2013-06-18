#!/bin/bash
Time=$(./time.sh)
echo "Saving time to CurrentTime.txt..."
echo $Time>>CurrentTime.txt
