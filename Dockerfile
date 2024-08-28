# Use an official Ubuntu image as the base
FROM ubuntu:20.04

# Install required dependencies for C++ and build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    cmake \
    libopencv-dev \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Install Node.js
RUN curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash - \
    && apt-get install -y nodejs

# Set up the working directory for C++ and Node.js
WORKDIR /app

# Copy the C++ source code and Node.js application files
COPY . .

# Compile the C++ application
RUN g++ -o seam_carving seam_carving.cpp `pkg-config --cflags --libs opencv4`

# Install Node.js dependencies
RUN npm install

# Expose the port your Node.js app runs on
EXPOSE 8080

# Start the Node.js application
CMD ["npm", "start"]

