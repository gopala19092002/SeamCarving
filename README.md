---

# Installation Guide

Follow the steps below to set up your environment:

## Update and Install Dependencies

First, ensure your system's package list is up-to-date and install essential packages:

```bash
sudo apt-get update
sudo apt-get install -y build-essential g++ cmake libopencv-dev curl sudo tzdata
```

## Install Node.js

Install Node.js version 18.x:

```bash
curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash -
sudo apt-get install -y nodejs
```

## Set Up the Working Directory

Make sure to set up the working directory for your project.

## Make the Build Script Executable

Change the permissions of the build script to make it executable:

```bash
chmod +x build.sh
```

## Run the Build Script

Execute the build script to complete the setup:

```bash
./build.sh
```
---

