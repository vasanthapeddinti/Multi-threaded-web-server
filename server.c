#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT         80
#define BUFFERSIZ    2048

void handle_client_request(int client_fd);

int main()
{
    struct sockaddr_in address;

    int server_fd;
    int client_fd;
	int addrlen = sizeof(address);

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

		// accept incoming connections
		if ((client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}
		printf("\nclient connection accepted successfully\n");

		handle_client_request(client_fd);

		// closing the connected socket
		close(client_fd);

	}


	// closing the listening socket
	//shutdown(server_fd, SHUT_RDWR);
	//printf("closing and shutting down the connection\n");

	return 0;
}

void handle_client_request(int client_fd) {

	ssize_t valread;
	ssize_t valwrite;
	char buffer[BUFFERSIZ] = { 0 };
	char filebuffer[BUFFERSIZ] = { 0 };
	char method[20];
	char page[20];
	char protocol[20];


	const char *header = "HTTP/1.0 200 OK\r\n"
	"Server: webserver\r\n"
	"Content-type: text/html\r\n\r\n";

	// recieve the data from client
	if ((valread = recv(client_fd, buffer, 1024, 0)) <= 0) {
		perror("No data received\n");
		exit(EXIT_FAILURE);
	}
	printf("\ndata recieved: \n%s\n\n", buffer);

	sscanf(buffer, "%s %s %s", method, page, protocol);
	printf("page trying to reach is %s", page);





	// send the data to client
	if (strcmp(page, "/index.html") == 0 || strcmp(page, "/") == 0) {
		
	    FILE* fptr;
		size_t n;
		fptr = fopen("index.html", "r");

	
		if (NULL == fptr) {
			printf("file can't be opened \n");
		}

		if (valwrite = send(client_fd, header, strlen(header), 0) < 0) {
			perror("error sending headers..");
		}
		while ((n = fread(filebuffer, 1, sizeof(BUFFERSIZ), fptr)) > 0) {
			if (valwrite = send(client_fd, filebuffer, strlen(filebuffer), 0) < 0) {
				printf("error sending data..");
			}
		}

		fclose(fptr);

		printf("\n\nData sent\n");
	} else {

		const char *msg = "HTTP/1.1 404 Not Found\r\n";

		// send the data to client
		if (valwrite = send(client_fd, msg, strlen(msg), 0) < 0) {
			perror("error sending data..");
		}

		printf("\nClient tried to reach undefined or restricted page...\n");
	}

	return;
}
