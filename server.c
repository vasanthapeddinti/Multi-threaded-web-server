#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT         80
#define BUFFERSIZ    1024

int main()
{
    struct sockaddr_in address;

    int server_fd;
    int client_fd;
    ssize_t valread;
	ssize_t valwrite;
	int sockn;
	int addrlen = sizeof(address);

	char buffer[BUFFERSIZ] = { 0 };
	//char *msg = "Hello from server";

	
	char *msg = "HTTP/1.0 200 OK\r\n"
	"Server: webserver-c\r\n"
	"Content-type: test/html\r\n\r\n"
	"<html>hello, world</html>\r\n";

	//char *msg = "HTTP/1.1 200 OK\r\n\r\nRequested path: <the path>\r\n";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	printf("socket created %d\n", server_fd);

	// Create address to bind the socket to
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Bind socket to the address created
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("bind successful\n");

	// Listen to the incoming connections
	if (listen(server_fd, 1) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("listening on port %d... \n", PORT);


	while(1) {

		// Create client address
		struct sockaddr_in client_addr;
		int client_addrlen = sizeof(client_addr);

		// accept incoming connections
		if ((client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		printf("client connection accepted successfully\n");

		if(
			(sockn = getsockname(client_fd, (struct sockaddr*)&client_addr, (socklen_t *)&client_addrlen)) < 0) {
			perror("error in getsockname");
		}

		// recieve the data from client
		if ((valread = read(client_fd, buffer, 1024)) <= 0) {
			perror("No data received\n");
			exit(EXIT_FAILURE);
		}
		printf("%s:%u\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		printf("data recieved: %s\n\n", buffer);

		// send the data to client
		if (valwrite = send(client_fd, msg, strlen(msg), 0) < 0) {
			perror("error sending data..");
		}
		printf("Data sent\n");

		// closing the connected socket
		close(client_fd);

	}


	// closing the listening socket
	//shutdown(server_fd, SHUT_RDWR);
	//printf("closing and shutting down the connection\n");

	return 0;
}


