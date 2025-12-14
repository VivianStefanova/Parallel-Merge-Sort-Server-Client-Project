#include"mergeSort.h"
#include<iostream>
#include<vector>
#include<chrono>


#include <string.h>
#include <sys/types.h>        
#include <cstdio>          
#include <cstdlib>          
#include <cstring>          
#include <sys/socket.h>     
#include <netinet/in.h>     
#include <arpa/inet.h>      
#include <netdb.h>          
#include <unistd.h>    

#define PORT "4950"

bool sendAll(int sock, const void* data, size_t len) {
    const char* ptr = (const char*)data;
    while (len > 0) {
        ssize_t sent = send(sock, ptr, len, 0);
        if (sent <= 0) return false;
        ptr += sent;
        len -= sent;
    }
    return true;
}

int main() {

    //char serverIP[INET_ADDRSTRLEN];

    //for notifying new connection
    char remoteIP[INET6_ADDRSTRLEN];
    //stuctures for server setup
	struct addrinfo hints, * serverInfo, * p;

    int listener; // listening socket descriptor
    int newClient;        // newly accepted socket descriptor with accept()
	struct sockaddr_storage clientaddr; //new client address
	socklen_t addrlen;

    //set 0s to hints
    memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

    //get server ip address
    int rv;
	if ((rv = getaddrinfo(NULL, PORT, &hints, &serverInfo)) != 0) {
		fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
		exit(1);
	}
	p = serverInfo;
	// loop through all the results and bind to the first we can
	for(p = serverInfo; p != NULL; p = p->ai_next) {
		if ((listener = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(listener, p->ai_addr, p->ai_addrlen) == -1) {
			//close(sockfd);
			shutdown(listener, 2);
			perror("listener: bind");
			continue;
		}

		break;
	}

	// server starts listening on socket
	if (listen(listener, SOMAXCONN) == -1) {
		perror("listen");
		exit(3);
	}

    std::cout << "Server is listening on port " << PORT << std::endl;
    // struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
    // inet_ntop(p->ai_family, &(ipv4->sin_addr), serverIP, sizeof(serverIP));
    // std::cout << "Server IP address: " << serverIP << std::endl;
    std::cout << "Server IP address:  127.0.0.1" << std::endl;
   
    freeaddrinfo(serverInfo);
    std::cout << "Server is running..." << std::endl;

    while(true){
       //handle new connections
        addrlen = sizeof clientaddr;
		newClient = accept(listener, (struct sockaddr*)&clientaddr, &addrlen);

		if (newClient == -1) {
			perror("accept");
            continue;
		}
		printf("New connection from %s on socket %d\n",
            inet_ntop(clientaddr.ss_family,
								&(((struct sockaddr_in*) &clientaddr)->sin_addr),
								remoteIP, INET_ADDRSTRLEN),newClient); 

        //wait for client to send data size  
        uint32_t size, threads;

        recv(newClient, &size, sizeof(size), MSG_WAITALL);
        recv(newClient, &threads, sizeof(threads), MSG_WAITALL);

        size = ntohl(size);
        threads = ntohl(threads);

        std::vector<int> data(size);
        recv(newClient, data.data(), size * sizeof(int), MSG_WAITALL);

        std::cout << "Received array of size " << size << " with " << threads << " threads\n";   
        //printArray(data);
        //Sort data and send it back to client
        std::vector<int> data2;
        if(threads >1){
            //vector for multi sort comparison
            data2.resize(size);
            for(int i=0; i<(int)size; i++){
                data2[i]= data[i];
            }
            
            auto pStart =  std::chrono::high_resolution_clock::now();
            multiSort(data2, threads);
            auto pEnd =  std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> pDuration = pEnd - pStart;
            std::cout << "Multi sort time:  " << pDuration.count() << " seconds" <<  std::endl;
            //sent time measurement for multi sort
            double pdurationSec = pDuration.count();
            sendAll(newClient, &pdurationSec, sizeof(pdurationSec));
        }    
        auto start =  std::chrono::high_resolution_clock::now();
        singleSort(data);
        auto end =  std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Single sort time: " << duration.count() << " seconds" <<  std::endl;
        singleSort(data);
        //sent time measurement for single sort
        double durationSec = duration.count();
        sendAll(newClient, &durationSec, sizeof(durationSec));

        if(threads >1){
            std::cout << "Sending sorted array (multi-threaded) back to client.\n";
            sendAll(newClient, data2.data(), size * sizeof(int));
        }
        else{
            std::cout << "Sending sorted array (single-threaded) back to client.\n";
            sendAll(newClient, data.data(), size * sizeof(int));
        }
        std::cout << "Sorted array sent back to client.\n";

        
        close(newClient);                        
    }

    close(listener);
    return 0;
    
}