#!/bin/sh
# Auto-generated code below aims at helping you parse
# the standard input according to the problem statement.


# game loop
while true; do
    maxH=0
    index=0
    for (( i=0; i<8; i++ )); do
        # mountainH: represents the height of one mountain, from 9 to 0.
	echo -n "please input mountain "$i" height: "
        read mountainH
        if [ $mountainH -gt $maxH ];then
            maxH=$mountainH
	    index=$i
        fi
    done

    # Write an action using echo
    # To debug: echo "Debug messages..." >&2

    #echo $index # The number of the mountain to fire on.
    echo "The heighter mountain is number "$index
done
