#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>  // close()

#include <cstring>  // sizeof()
#include <iostream>
#include <string>
#include <vector>

#include "cli.h"
#include "kvslib.h"

void plain_send(int fd, std::string msg) {
  std::cout << "Sending " << msg << std::endl;
  unsigned int msg_len = msg.length();
  auto prompt_bytes_sent = send(fd, msg.data(), msg_len, 0);
  if (prompt_bytes_sent != msg_len) {
    // TODO(dmiller): actual retry
    throw 42;
  }
}

std::string send_and_recv(int fd, std::string prompt) {
  plain_send(fd, prompt);

  std::vector<char> buf(2048);
  // TODO(dmiller): what should I do with this variable?
  auto response_bytes_received = recv(fd, buf.data(), buf.size(), 0);

  std::string str(buf.begin(), buf.end());

  return str;
}

// TODO(dmiller): create a function that takes a command and a KVS and executes
// it
int main() {
  auto myKVS = InMemoryKVS<std::string>();
  auto portNum = "9669";
  const unsigned int backlog =
      8;  // number of connections allowed on the incoming queue

  addrinfo hints, *res,
      *p;  // we need 2 points, res to hold and p to iterate over
  memset(&hints, 0, sizeof(hints));

  // for more explanation, `man socket`
  hints.ai_family = AF_UNSPEC;      // don't specify which IP version to use
  hints.ai_socktype = SOCK_STREAM;  // TCP
  hints.ai_flags = AI_PASSIVE;  // bind to the IP of the host, in other words:
                                // don't specify it

  // man getaddrinfo
  auto gAddRes = getaddrinfo(NULL, portNum, &hints, &res);
  if (gAddRes != 0) {
    std::cerr << gai_strerror(gAddRes) << "\n";
    return -2;
  }

  std::cout << "Detecting addresses" << std::endl;

  auto foundAddress = false;
  char ipStr[INET6_ADDRSTRLEN];  // ipv6 length

  // find address to bind to
  // ... always bind to 0.0.0.0 heh
  for (p = res; p != NULL; p = p->ai_next) {
    void *addr;

    // if address is ipv4 address
    sockaddr_in *ipv4 = reinterpret_cast<sockaddr_in *>(p->ai_addr);
    addr = &(ipv4->sin_addr);

    // convert IPv4 from binary to text form
    inet_ntop(p->ai_family, addr, ipStr, sizeof(ipStr));
    auto stdIpStr = std::string(ipStr);

    if (stdIpStr == "0.0.0.0") {
      foundAddress = true;
      break;
    }
  }

  // if no addresses found :(
  if (!foundAddress) {
    std::cerr << "Found no host address to use" << std::endl;
    return -3;
  }

  std::cout << "Found address: " << ipStr << std::endl;

  // let's create a new socket, socketFD is returned as descriptor
  // man socket for more information
  // these calls usually return -1 as result of some error
  int sockFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
  if (sockFD == -1) {
    std::cerr << "Error while creating socket" << std::endl;
    freeaddrinfo(res);
    return -4;
  }

  // Let's bind the address to the socket we've just created
  int bindR = bind(sockFD, p->ai_addr, p->ai_addrlen);
  if (bindR == -1) {
    std::cerr << "Error while binding socket" << std::endl;

    // If some error occurs, make sure we close socket and free resources
    close(sockFD);
    freeaddrinfo(res);
    return -5;
  }

  // finally start listening for connections on our socket
  int listenR = listen(sockFD, backlog);
  if (listenR == -1) {
    std::cerr << "Error while listening on socket" << std::endl;

    // If some error occurs, make sure to close socket and free resources
    close(sockFD);
    freeaddrinfo(res);
    return -6;
  }

  // structure large enough to hold client's address
  sockaddr_storage client_addr;
  socklen_t client_addr_size = sizeof(client_addr);

  const std::string response = "Hello World\n";

  // infinite loop to communicate with incoming connections
  // this will take clioent connections one at a time
  // in future use `fork()`
  // TODO(dmiller): why does this quit out after one command?
  // TODO(dmiller): maybe it's time to use GDB?
  while (true) {
    // accept call will give us a new socket descriptor
    int newFD = accept(sockFD, (sockaddr *)&client_addr, &client_addr_size);
    if (newFD == -1) {
      std::cerr << "Error while Accepting on socket" << std::endl;
      continue;
    }

    // send call sends the data you specify as second param and it's length as
    // third param also returns how many bytes were actually written
    unsigned int response_len = response.length();
    auto bytes_sent = send(newFD, response.data(), response.length(), 0);

    std::cout << "Sent " << bytes_sent << " bytes" << std::endl;
    if (bytes_sent != response_len) {
      std::cerr << "Which is not what we thought we would send ("
                << response_len << ")" << std::endl;
    }

    auto resp = send_and_recv(newFD, "Please enter something:\n");

    std::cout << "You entered: " << resp << std::endl;

    try {
      auto cmd = parse(resp);
      std::cout << cmd << std::endl;
      // TODO huh, why can't we retrieve values out of this??
      switch (cmd.getOp()) {
        case getOp: {
          auto value = myKVS.get(cmd.getKey());
          std::cout << "Retrieved: " << value << "for key " << cmd.getKey()
                    << std::endl;
          plain_send(newFD, value);
          break;
        }
        case setOp:
          assert(cmd.getValue().has_value());
          std::cout << "Setting " << cmd.getKey() << " to "
                    << cmd.getValue().value() << std::endl;
          myKVS.put(cmd.getKey(), cmd.getValue().value());
          plain_send(newFD, myKVS.get(cmd.getKey()));
          break;
        case deleteOp:
          myKVS.remove(cmd.getKey());
          break;
      }
    } catch (std::string e) {
      std::cerr << "Error parsing cmd: " << e << std::endl;
    }

    close(newFD);
  }

  close(sockFD);
  freeaddrinfo(res);

  return 0;
}