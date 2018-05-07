#ifndef data_receiver_H
#define data_receiver_H

#include <iostream>
#include <bits/stdc++.h>
#include "../../communications/sockets/include/sockets.hpp"

using namespace std;

class data_receiver {
  private: 
    Socket::TCPSocket server;
    std::shared_ptr<Socket::TCPSocket> client;
  public:
    data_receiver();
    void send(string data);
    string receive();
};

#endif