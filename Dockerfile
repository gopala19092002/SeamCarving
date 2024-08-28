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

# Copy the source code and Node.js application files into the container
COPY . .

# Make sure the build script is executable
RUN chmod +x /app/build.sh

# Run the build script
CMD ["/app/build.sh"]
