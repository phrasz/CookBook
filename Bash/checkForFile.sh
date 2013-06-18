#!/bin/bash
DirLs="Directory.log"
if [ -e "$DirLs" ]; then
echo "It exists!"
else
echo "It doesn't exist!"
fi
