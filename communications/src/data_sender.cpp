#include "data_sender.hpp"

using namespace std;

data_sender::data_sender(){
  client.connect("localhost", 8080);
  cout << "Connected to server" << endl;
}
void data_sender::send(string data){
  std::cout << "Sending data: " << data << std::endl;
  client.send(data);
}
string data_sender::receive(){
  string data = client.recv(256);
  cout << "Receiving data " << data << endl;
  return data;
}
