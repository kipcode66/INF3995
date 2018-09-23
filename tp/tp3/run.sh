#!/usr/bin/env bash
APP_NAME='tp3'

# Check if root
if [ "$(whoami)" == root ]; then
    PORT=80
    MAX_PORT=1023
else
    PORT=8080
    MAX_PORT=8500
fi

while [ "$PORT" != "$MAX_PORT" ]; do
    echo $'\n'"Trying port number $PORT"
    sudo ./build/$APP_NAME "$PORT"
    RETURN_CODE=$?
    if [ "$RETURN_CODE" == 255 ]; then
        PORT=$(($PORT + 1))
    fi
done

