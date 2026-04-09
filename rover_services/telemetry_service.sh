#!/bin/bash

echo "Telemetry logging started"

while true; do
    date >> ~/rover_services/log.txt
    sleep 5
done
