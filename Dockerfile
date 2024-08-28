# Use an official Ubuntu image as the base
FROM ubuntu:20.04

# Install necessary dependencies for C++ and build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    cmake \
    libopencv-dev \
    curl \
    sudo \
    && rm -rf /var/lib/apt/lists/*

# Install Node.js
RUN curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash - \
    && apt-get install -y nodejs

# Set up the working directory
WORKDIR /app

# Copy the C++ source code and Node.js application files
COPY . .

# Compile the C++ application
RUN g++ seam_carving.cpp -o seam_carving `pkg-config --cflags --libs opencv4`

# Install Node.js dependencies
RUN npm install

# Expose the port your Node.js app runs on
EXPOSE 8080

# Copy and set up the start script
COPY start.sh /app/
RUN chmod +x /app/build.sh

# Run the start script
CMD ["/app/build.sh"]
