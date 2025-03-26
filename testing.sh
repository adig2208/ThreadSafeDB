#!/bin/bash

# Project 2 Testing Script - testing.sh
# Usage: chmod +x testing.sh && ./testing.sh

pkill -f dbserver

PORT=5010
PORT2=5011
PORT3=5012

SERVER=./dbserver
DBTEST=./dbtest

echo "Starting server on port $PORT..."
($SERVER $PORT &)
SERVER_PID=$!

sleep 2  

echo -e "\nRunning SET test..."
$DBTEST --port=$PORT -S key1 value1

echo -e "\nRunning GET test..."
$DBTEST --port=$PORT -G key1

echo -e "\nRunning DELETE test..."
$DBTEST --port=$PORT -D key1

echo -e "\nRunning GET test...(should fail)"
$DBTEST --port=$PORT -G key1

echo -e "\nRunning GET test...(non-existent)"
$DBTEST --port=$PORT -G key2

echo -e "\nRunning DELETE test...(non-existent)"
$DBTEST --port=$PORT -D key3

echo -e "\nRunning Value rewrite test..."
$DBTEST --port=$PORT -S key4 value4
$DBTEST --port=$PORT -G key4 
$DBTEST --port=$PORT -S key4 value3
$DBTEST --port=$PORT -G key4 

echo -e "\nRunning load test with 50 requests and 4 threads..."
$DBTEST --port=$PORT --count=50 --threads=4

echo -e "\nRunning random test mix (10 concurrent random requests)..."
$DBTEST --port=$PORT --test

sleep 2

wait $SERVER_PID
if [ $? -eq 0 ]; then
    echo -e "\nServer exited cleanly."
else
    echo -e "\nFAILED: Server did not exit cleanly."
fi

echo -e "\nRestarting server for quit test with -q"
($SERVER $PORT2 &)  
SERVER_PID=$!

sleep 2

echo -e "\nTesting quit via -q flag..."
$DBTEST --port=$PORT2 -q

wait $SERVER_PID
if [ $? -eq 0 ]; then
    echo -e "\nServer exited cleanly."
else
    echo -e "\nFAILED: Server did not exit cleanly."
fi

echo -e "\nRunning stats and quit test..."
(
  sleep 1     
  echo stats   
  sleep 1
  echo quit    
) | exec $SERVER $PORT3 &


wait $SERVER_PID
if [ $? -eq 0 ]; then
    echo -e "\nServer exited cleanly."
else
    echo -e "\nFAILED: Server did not exit cleanly."
fi

echo -e "\nTest run complete."
