#!/bin/bash

ip=$1

if [ -z "$ip" ];
	then echo "Please provide an IP address as the first parameter!"
	exit 1
fi

echo "Trying to GET data from http://$ip/celsius ..."

i=1

while true; do
	echo "Executing request # $i ..." 
	curl -i -H "Accept: application/json" -H "Content-Type: application/json" -X GET http://$ip/celsius
	((i++))
	sleep 1
done

