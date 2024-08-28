#!/bin/bash

# Compile the C++ application
g++ seam_carving.cpp -o seam_carving `pkg-config --cflags --libs opencv4`

# Install Node.js dependencies
npm install


# Start the Node.js application
node app.js
