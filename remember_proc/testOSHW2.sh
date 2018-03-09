#!/usr/bin/env bash
#David Mandelbaum
#Last updated: 11.26.17
echo git pull
echo make clean
wait 1
echo make
wait 3
echo sudo insmod remember_proc.ko
echo cat /proc/remember
echo "new test 1" > /proc/remember
echo cat /proc/remember
echo "new test 2" > /proc/remember
echo cat /proc/remember
echo "new test 3" > /proc/remember
echo cat /proc/remember
echo sudo rmmod remember_proc