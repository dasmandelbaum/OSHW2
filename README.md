# OSHW2
##David Mandelbaum, 3/11/18
##Instructions: compatible with included Makefile. Just type "make clean", then "make" 
as such (https://stackoverflow.com/a/33006018): 
![Alt text](./image1.png?raw=true "First picture")
![Alt text](./image2.png?raw=true "Second picture")
###As seen above, you can initiate the file in kernel normally with "insmod", then "cat" the strings back to you. Max length is 80!
###One issue I ran into is when you enter a string that is too long, the next "cat" call will cut off the first letter of the previous valid string entered. Might be able to fix this with another if statement.. 
##Any code that is not original (dealing with char array) has a source in comment next to it, but mostly redacted hello_proc.c 
##Thanks to Prof. Kelly for all your help!