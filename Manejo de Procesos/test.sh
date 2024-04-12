#!/bin/bash

cal=0

echo "Compiling..."
gcc $1 -o app > /dev/null 2>&1
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
echo " done"
cal=$((cal+10))

echo "Checking parameters..."
./app > /dev/null 2>&1
if [[ $? -ne 255 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
echo " done."
cal=$((cal+10))

echo "Verifying that the parameter is valid..."
echo "Test case 1: text"
./app texto > /dev/null 2>&1
if [[ $? -ne 254 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
echo " done."
cal=$((cal+10))

echo "Test case 2: negative number"
./app -10 > /dev/null 2>&1
if [[ $? -ne 254 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
echo " done."

echo "Test case 3: zero"
./app 0 > /dev/null 2>&1
if [[ $? -ne 254 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
echo " done."
cal=$((cal+10))

echo "Running with value 3..."
./app 3 > temp.txt
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	exit 1
fi
diff temp.txt output.txt > /dev/null 2>&1
if [[ $? -ne 0 ]] ; then 
	echo "grade: $cal"
	rm app temp.txt
	exit 1
fi
cal=$((cal+60))

echo "=>grade: $cal"
rm temp.txt app