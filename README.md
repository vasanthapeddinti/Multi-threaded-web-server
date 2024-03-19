# Multi-threaded-web-server


# Build the docker image
docker build -t webserver .

# Run the docker image
docker run -d -p 80:80 -it webserver

# Check the docker container
docker ps

# Check the docker logs
docker logs -f `<container-id>`

# Send client requests
curl --verbose http://localhost/index.html