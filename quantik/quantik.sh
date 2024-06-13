#!/bin/bash

function usage() { 
    echo "Usage: quantik.sh [TARGET] [ACTION]"
    echo 
    echo -e "Available Targets and Actions:"
    echo -e "\t TARGET = data_generation:"
    echo -e "\t\t ACTION = build: Builds the C++ Project for Data Generation"
    echo -e "\t\t ACTION = run: Runs the Data Generation"
    echo -e "\t\t ACTION = save: Saves the data files from the last run to the ./data folder (overwriting old files)"
    echo -e "\t\t ACTION = clean: Removes all data files from ./build and ./data folder"
    echo -e "\t TARGET = website:"
    echo -e "\t\t ACTION = run_dev: Starts the developement webserver at localhost:5000"
    echo -e "\t\t ACTION = run: Starts the production webserver at localhost:5000 (gunicorn required)"
}

case "$1" in
    "data_generation") 
        case "$2" in
            "build") mkdir ./build ; cd ./build ; cmake .. ; make data_generation ;;
            "run") cd ./build ; ./data_generation ;;
            "save") find ./build -type f -name '*.qtk' -exec cp '{}' ./data/ ';' ;;
            "clean") find ./build ./data -type f \( -name '*.qtk' -o -name '*.tmp' \) -exec rm '{}' ';'  ;;
            *) echo "Invalid Action \"$2\"" ; usage ;;
        esac
    ;;
    "website")
        case "$2" in
            "run_dev") python3 ./website/main.py ;;
            "run") cd ./website ; gunicorn -w 4 -b 0.0.0.0:5000 main:app ;;
            *) echo "Invalid Action \"$2\"" ; usage ;;
        esac
    ;;
    *) echo "Invalid Target!" ; usage ;;
esac