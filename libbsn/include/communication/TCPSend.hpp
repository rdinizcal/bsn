#include <stdint.h>
#include <iostream>
#include <string>
#include <memory>
#include <opendavinci/odcore/io/tcp/TCPConnection.h>
#include <opendavinci/odcore/io/tcp/TCPFactory.h>

class TCPSend {
    private:
        bool is_connected;
        std::string ip;
        int32_t port;
        std::shared_ptr<odcore::io::tcp::TCPConnection> thisConnection;
    public:
        void disconnect();
        TCPSend(int);
        // Construtor recebe uma string com o ip e uma int32_t porta        
        TCPSend(std::string this_ip, int32_t this_port);
        // Conecta à porta desejada e 
        // retorna true se foi feita com sucesso e false caso nao
        bool connect();        
        // Envia um pacote por rede
        void send(std::string package);
        // Retorna a porta à qual tenta-se conectar        
        int32_t get_port();
        void set_port(int32_t p);
        void setIP(std::string vIP);
};