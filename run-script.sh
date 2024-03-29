#!/bin/bash

#Define cpp source and output file
CPP_SOURCE="db.cpp"
OUTPUT_FILE="db"

#Compiling
g++ $CPP_SOURCCE -o $OUTPUT_FILE

#Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful"
    #Run the output file
    ./$OUTPUT_FILE
else
    echo "Compilation failed"
fi

echo "Press any key to close..."
read -n1 -s