#include<iostream>
#include<vector>

#include<thread>
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

#define SERVERPORT "4950"

//Set random values in the arrays- two identical arrays for sorting speed comparison
void setRandomValues(std::vector<int> &arr1, int size){
    for (int i = 0; i < size; ++i) {
        // Random numbers between 0 and 9999
        arr1[i] = rand() % 10000;  
    }
}
void printArray(const std::vector<int> &arr){
    int n = arr.size();
    std::cout<< "Array size: " << n << std::endl;
    std::cout<<"[";
    for(int i=0; i <n-1; i++){
        std::cout<< arr[i] << " ";
    }
    std::cout<< arr[n-1] << "]"<< std::endl;
    
}

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
    int sockfd;
	struct addrinfo hints, * serverInfo, * p;

    std::this_thread::sleep_for(std::chrono::seconds(2));

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; 
	hints.ai_socktype = SOCK_STREAM;
	
    int rv;
	if ((rv = getaddrinfo("127.0.0.1", SERVERPORT, &hints, & serverInfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
    // Loop through results and connect
    for (p =  serverInfo; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            perror("socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("connect");
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

    freeaddrinfo(serverInfo);

    std::cout << "Connected to server.\n";
    std::cout << "Client is running..." << std::endl;

    //get user input
    unsigned int maxThreads = std::thread::hardware_concurrency();

    uint32_t threads;
    uint32_t size;
    do {
        std::cout << "Enter number of threads (1 - " << maxThreads << "): ";
        std::cin >> threads;

        if (!std::cin || threads < 1 || threads > maxThreads) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input.\n";
        }
    } while (threads < 1 || threads > maxThreads);
    //TO DO- validation for integer input
    std::cout << "Array size: ";
    std::cin >> size;
    std::vector<int> data(size);

    int choice;
    std::cout << "\nChoose input method:\n";
    std::cout << "1 - Generate random values\n";
    std::cout << "2 - Enter values manually\n";
    std::cout << "Choice: ";
    std::cin >> choice;

    if (choice == 1) {
        setRandomValues(data, size);
    }
    else if (choice == 2) {
        std::cout << "Enter " << size << " integers:\n";
        for (uint32_t i = 0; i < size; ++i)
            std::cin >> data[i];
    }
    else {
        std::cerr << "Invalid choice\n";
        close(sockfd);
        return 1;
    }
    printArray(data);
    uint32_t netSize = htonl(size);
    uint32_t netThreads = htonl(threads);

    sendAll(sockfd, &netSize, sizeof(netSize));
    sendAll(sockfd, &netThreads, sizeof(netThreads));
    sendAll(sockfd, data.data(), data.size() * sizeof(int));
    std::cout << "Data sent to server for sorting using " << threads << " threads.\n";

    //Receive sorted data
    recv(sockfd, data.data(), size * sizeof(int), MSG_WAITALL);
    std::cout << "Sorted array received from server:\n";
    printArray(data);
    
    close(sockfd);
    return 0;
    
}