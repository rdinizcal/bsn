#ifndef data_sender_H
#define data_sender_H

#include "../../sockets/include/sockets.hpp"
#include <bits/stdc++.h>

using namespace std;

class data_sender {
  private:
    Socket::TCPSocket client;
  public:
    data_sender();
    void send(string data);
    string receive();
};

#endif