#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

const int port = 9090;
const int buffer_size = 1<<10;

void handleError(const string &message);

int main()
{
	int server_sock;
	int client_sock;
	char buffer[buffer_size];
	
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	socklen_t client_address_size;

	char status[] = "HTTP/1.0 200 OK\r\n";
    char header[] = "Server: A Simple Web Server\r\nContent-Type: text/html\r\n\r\n";
    char body[] = "<html><head><title>A Simple Web Server</title></head><body><h2>Welcome!</h2><p>This is a simple server.</p></body></html>";

    server_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (server_sock == -1)
    {
    	handleError("socket error");
    }

    memset(&server_address,0,sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    if(bind(server_sock,(struct sockaddr*)&server_address, sizeof(server_address)) == -1){
    	handleError("bind error");
    }

    if(listen(server_sock, 5) == -1) {
    	handleError("listen error");
    }

    client_address_size = sizeof(client_address);
    client_sock = accept(server_sock, (struct sockaddr*) &client_address, &client_address_size);

    if (client_sock == -1) {
    	handleError("accept error");
    }

    read(client_sock, buffer, sizeof(buffer)-1);
    cout<<buffer;
    write(client_sock,status,sizeof(status));
    write(client_sock,header,sizeof(header));
    write(client_sock,body,sizeof(body));

    close(client_sock);
    close(server_sock);

	return 0;
}

void handleError(const string &message) {
	cout<<message;
	exit(1);
}
