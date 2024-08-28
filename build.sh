#!/bin/bash

# Install necessary dependencies (if any)
sudo apt-get update
sudo apt-get install -y build-essential

# Compile your C++ code
g++ seam_carving.cpp -o seam_carving `pkg-config --cflags --libs opencv4`


