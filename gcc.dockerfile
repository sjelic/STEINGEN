# Use a base image with Debian
FROM debian:latest

# Install gcc, gdb, make, and other necessary tools
RUN apt-get update && apt-get install -y \
    gcc \
    gdb \
    make \
    doxygen \
    valgrind \
    docker.io
RUN apt-get clean
RUN ulimit -s unlimited
# Set working directory within the container
WORKDIR /steingen
