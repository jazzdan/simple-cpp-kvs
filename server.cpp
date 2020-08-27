#include <cstring> // sizeof()
#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h> // close()

#include "kvslib.h"

int main()
{
    auto portNum = "9669";
    const unsigned int backlog = 8; // number of connections allowed on the incoming queue

    addrinfo hints, *res, *p; // we need 2 points, res to hold and p to iterate over
    memset(&hints, 0, sizeof(hints));

    // for more explanation, `man socket`
    hints.ai_family = AF_UNSPEC;     // don't specify which IP version to use
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;

    // man getaddrinfo
    auto gAddRes = getaddrinfo(NULL, portNum, &hints, &res);
    if (gAddRes != 0)
    {
        std::cerr << gai_strerror(gAddRes) << "\n";
        return -2;
    }

    std::cout << "Detecting addresses" << std::endl;

    auto foundAddress = false;
    char ipStr[INET6_ADDRSTRLEN]; // ipv6 length

    // Now since `getaddrinfo()` has given us a list of addresses
    // we're going to iterate over them and ask usr to choose one address for program to bind to
    for (p = res; p != NULL; p = p->ai_next)
    {
        void *addr;
        std::string ipVer;

        // if address is ipv4 address
        if (p->ai_family == AF_INET)
        {
            ipVer = "IPv4";
            sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in *>(p->ai_addr);
            addr = &(ipv4->sin_addr);
        }
        else
        {
            continue;
        }

        // convert IPv4 from binary to text form
        inet_ntop(p->ai_family, addr, ipStr, sizeof(ipStr));
        auto stdIpStr = std::string(ipStr);

        if (stdIpStr == "0.0.0.0")
        {
            foundAddress = true;
            break;
        }
    }

    // if no addresses found :(
    if (!foundAddress)
    {
        std::cerr << "Found no host address to use\n";
        return -3;
    }

    std::cout << "Found address: " << ipStr << std::endl;

    // let's create a new socket, socketFD is returned as descriptor
    // man socket for more information
    // these calls usually return -1 as result of some error
    int sockFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockFD == -1)
    {
        std::cerr << "Error while creating socket" << std::endl;
        freeaddrinfo(res);
        return -4;
    }

    // Let's bind the address to the socket we've just created
    int bindR = bind(sockFD, p->ai_addr, p->ai_addrlen);
    if (bindR == -1)
    {
        std::cerr << "Error while binding socket" << std::endl;

        // If some error occurs, make sure we close socket and free resources
        close(sockFD);
        freeaddrinfo(res);
        return -5;
    }

    // finally start listening for connections on our socket
    int listenR = listen(sockFD, backlog);
    if (listenR == -1)
    {
        std::cerr << "Error while listening on socket" << std::endl;

        // If some error occurs, make sure to close socket and free resources
        close(sockFD);
        freeaddrinfo(res);
        return -6;
    }

    // structure large enough to hold client's address
    sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    const std::string response = "Hello World";

    // infinite loop to communicate with incoming connections
    // this will take clioent connections one at a time
    // in future use `fork()`
    while (true)
    {
        // accept call will give us a new socket descriptor
        int newFD = accept(sockFD, (sockaddr *)&client_addr, &client_addr_size);
        if (newFD == -1)
        {
            std::cerr << "Error while Accepting on socket" << std::endl;
            continue;
        }

        // send call sends the data you specify as second param and it's length as third param
        // also returns how many bytes were actually written
        auto bytes_sent = send(newFD, response.data(), response.length(), 0);
        std::cout << "Sent " << bytes_sent << " bytes" << std::endl;
        close(newFD);
    }

    close(sockFD);
    freeaddrinfo(res);

    return 0;
    // InMemoryKVS<int> x = InMemoryKVS<int>();
    // x.put("hello", 5);

    // std::cout << x.get("hello") << std::endl;

    // x.remove("hello");

    // std::cout << x.get("hello") << std::endl;

    // InMemoryKVS<std::string> y = InMemoryKVS<std::string>();
    // std::string str("world");
    // y.put("hello", str);
    // std::cout << y.get("foo") << std::endl;
    // return 0;
}