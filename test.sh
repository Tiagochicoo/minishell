#!/bin/bash
filename="LICENSE"
if [ -f "$filename" ]; then
echo "File exists"
else
echo "File does not exist"
fi