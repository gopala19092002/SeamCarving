# Use an official Ubuntu image
FROM ubuntu:20.04

# Install required dependencies for C++ and build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    cmake \
    libopencv-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory in the container
WORKDIR /app

# Copy the source code into the container
COPY . .

# Compile the C++ application
RUN g++ -o seam_carving seam_carving.cpp `pkg-config --cflags --libs opencv4`

# Specify the command to run the executable
CMD ["./seam_carving"]

