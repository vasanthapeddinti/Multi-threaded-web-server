# use ubuntu as base image
FROM ubuntu:latest as build-env

# install build-base meta package inside build-env container
RUN apt-get update && apt-get install -y gcc

# change directory to /app
WORKDIR /app

# copy all files from current directory inside the build-env container
COPY . .

# Compile the source code and generate binary executable file
RUN gcc -o server -std=c11 -Wall -Wextra -Wwrite-strings -Wno-parentheses -Wpedantic -Warray-bounds -Wconversion -I. server.c

# use another container to run the program
FROM ubuntu

# copy binary executable to new container
COPY --from=build-env /app/server /app/server
COPY --from=build-env /app/index.html /app/index.html

WORKDIR /app

# at last run the program
CMD ["/app/server"] 
