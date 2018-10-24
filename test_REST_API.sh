#!/bin/bash

i=1

while true; do
	echo "Executing request # $i ..." 
	curl -i -H "Accept: application/json" -H "Content-Type: application/json" -X GET http://INSERT_IP_HERE/celsius
	((i++))
	sleep 0.1
done

