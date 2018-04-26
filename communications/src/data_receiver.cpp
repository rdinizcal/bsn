#include "data_receiver.hpp"

using namespace std;

data_receiver::data_receiver(){
  // Binds the server to port 8080
  server.bind(8080);

  // Listens for incoming connections
  server.listen();

  // Blocks until it accepts a client
  client = server.accept();
  cout << "Connected client" << endl;
}
void data_receiver::send(string data){
  std::cout << "Sending data: " << data << std::endl;
  client->send(data); 
}
string data_receiver::receive(){
  string data = client->recv(256); 
  std::cout << "Received data " << data <<  std::endl;
  return data;
}