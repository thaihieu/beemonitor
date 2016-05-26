
#!/bin/bash


# connecting to genuino 101 bluetooth device 
/usr/bin/gatttool -b 98:4f:ee:0d:16:6e -I <<< connect 

echo "connected"
sleep 5

i=20;
for (( ; ; )) # condition is true
do
            # reading the values for which the handle is for temp, mass and humility and writing to a file
   	Temp=$(/usr/bin/gatttool -b 98:4f:ee:0d:16:6e --char-read  --handle=0x000b | cut -f 2 -d ":")	
  	Humid=$(/usr/bin/gatttool -b 98:4f:ee:0d:16:6e --char-read  --handle=0x000e | cut -f 2 -d ":")
	Mass=$(/usr/bin/gatttool -b 98:4f:ee:0d:16:6e --char-read  --handle=0x0011 | cut -f 2 -d ":")
 	
	#print the values to temp folder
	echo $Humid,$Temp,$Mass > /tmp/bee.txt 
	#echo $Humid,$Temp,$Mass
   	#echo "Number: $((i++))"  
	sleep $i
	# starting the reading script to past data to the cloud service
	node readData.js

done



