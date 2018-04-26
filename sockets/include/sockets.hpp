#ifndef SOCKETS_H
#define SOCKETS_H

/**
 * Copyright (c) 2018 Mikael Mello
 * Licensed under the MIT license.
 * https://opensource.org/licenses/MIT
 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <stdexcept>
#include <cstring>
#include <string>
#include <memory>

namespace Socket {
 
class ConnectionException : public std::runtime_error {
    public:
        ConnectionException(const std::string& error) : std::runtime_error(error) {}
};
 
class SocketException : public std::runtime_error {
    public:
        SocketException(const std::string& error) : std::runtime_error(error) {}
};

class ClosedConnection : public ConnectionException {
    public:
        ClosedConnection(const std::string& error) : ConnectionException(error) {}
};

/** Classe base de um wrapper de um socket padrão IPv4.
 */
class BaseSocket {
    public:

        /// Vincula o socket a alguma porta.
        /// @param port Número da porta que o socket será vinculado.
        void bind(uint32_t port);

        /// Permite configurar tempo limite para funções de input/output
        /// como recv() e send().
        void set_timeout(uint16_t seconds);
        
        std::string get_ip_address() {
            return this->ip_address_str;
        }

    protected:

        /// Cria um socket IPv4 de acordo com o protocolo selecionado.
        /// @param type Tipo do socket a ser criado, SOCK_STREAM ou SOCK_DGRAM.
        /// @param flags Flags opcionais para a chamada de getaddrinfo()
        BaseSocket(int type, int flags = 0);

        /// Cria um novo objeto com informações de uma socket IPv4 já criada.
        /// @param socket       Descritor de arquivo da socket já criada.
        /// @param type         Tipo do socket (SOCK_DGRAM ou SOCK_STREAM).
        /// @param socket_info   Addrinfo TCP/IPv4 que já contêm endereços preenchidos.
        /// @param port_used    Porta na qual o socket foi vinculado.
        /// @param is_bound     Indica se o socket está vinculado a alguma porta.
        BaseSocket(int socket, int type, addrinfo socket_info, const std::string& client_ip,
            uint32_t port_used, bool is_bound);

        /// Destrutor. Libera memórias alocadas e encerra o socket.
        ~BaseSocket();
        
        /// Encerra o socket.
        void close();

        /// Preenche socket_info com endereços possíveis de se realizar chamadas
        /// de bind() ou connect().
        /// @param address Endereço a ser procurado (IPv4 ou domínio).
        /// @param port Porta a ser realizada a busca.
        void getAddrInfo(const std::string address, uint32_t port);

        /// Verifica se a string é um endereço IP válido.
        /// @param ip_address std::string a ser avaliada
        /// @return True caso ip_address seja um enderçeo IP válido, false caso contrário.
        bool validateIpAddress(const std::string &ip_address);

        /// Contém as informações do socket.
        /// Socket será sempre TCP/IPv4 ou UDP/IPv4.
        addrinfo * socket_info;
        
        /// Descritor do arquivo da socket
        int socketfd = -1;

        /// Porta usada pelo bind()
        uint32_t port_used = -1;

        /// Se a socket está vinculada a alguma porta.
        bool is_bound = false;

        /// Se a socket foi fechada.
        bool is_closed = false;

        std::string ip_address_str;

};


/** Wrapper de um socket TCP/IPv4.
 *  
 *      Para usar como um servidor (recebendo conexões) construa o objeto,
 *  faça o bind() e o listen() e aceite diferentes conexões com diferentes
 *  chamadas à accept(). Para trocar informações com um cliente conectado
 *  utilize as funções send() e recv() do objeto retornado por accept().
 *      Para usar como um cliente basta construir o objeto e usar a função
 *  connect() para se conectar a um servidor, utilizando as funções send()
 *  e recv() para trocar informações com o servidor.
 *      A conexão é fechada normalmente usando a função close().
 */
class TCPSocket : public BaseSocket {
    public:

        /// Cria um socket TCP/IPv4.
        /// @param flags Flags opcionais para a chamada de getaddrinfo()
        TCPSocket(int flags = 0);

        /// Cria um socket TCP/IPv4 copiando as variaveis definidas nos parametros.
        /// @param socket       Descritor de arquivo da socket já criada.
        /// @param socket_info   Addrinfo TCP/IPv4 que já contêm endereços preenchidos.
        /// @param port_used    Porta na qual o socket foi vinculado.
        /// @param is_bound      Indica se o socket está vinculado a alguma porta.
        /// @param is_listening  Indica se o socket já está ouvindo em alguma porta.
        /// @param is_connected  Indica se o socket está conectado a algum servidor.
        TCPSocket(int socket, addrinfo socket_info, uint32_t port_used, const std::string& client_ip,
            bool is_bound, bool is_listening, bool is_connected);
        
        /// Conecta a socket a algum endereço.
        /// @param address std::string contendo o endereço IP ou domínio a ser conectado.
        /// @param port    Porta a qual se conectar.
        void connect(const std::string& address, uint32_t port);
        
        /// Recebe conexões na porta definida previamente pela função bind().
        /// @param backlog Número máximo de conexões pendentes.
        void listen(uint32_t backlog = 5);
        
        /// Aceita uma conexão pendente após chamar a função listen().
        /// @return Objeto TCPSocket conectado ao cliente aceito.
        std::shared_ptr<TCPSocket> accept(); 
        
        /// Envia uma quantidade específica de bytes para o endereço conectado.
        /// @param message Endereço inicial da mensagem a ser enviada.
        /// @param length Indica quantos bytes devem ser enviados
        /// @param flags   Flags opcionais para o envio da mensagem ao socket.
        void send(const uint8_t* message, int length, int flags = 0);  
        
        /// Recebe uma mensagem do endereço conectado.
        /// @param maxlen Tamanho máximo da mensagem a ser recebida.
        /// @param flags  Flags opcionais para o recebimento da mensagem.
        /// @return Endereço com os bytes recebidos, maxlen é alterado de modo
        ///         a refletir quantos bytes foram recebidos.
        uint8_t* recv(uint64_t* maxlen, int flags = 0);
        
        /// Envia uma string ao endereço conectado.
        /// @param message Mensagem a ser enviada.
        /// @param flags   Flags opcionais para o envio da mensagem ao socket.
        void send(const std::string& message, int flags = 0);
        
        /// Recebe uma string do endereço conectado.
        /// @param maxlen Tamanho máximo da mensagem a ser recebida.
        /// @param flags  Flags opcionais para o recebimento da mensagem.
        /// @return std::string contendo a mensagem recebida.
        std::string recv(uint64_t maxlen, int flags = 0);

        
        
        /// Encerra o socket.
        void close();

    private:
        
        /// Se o socket está conectada a algum endereço
        bool is_connected = false;

        /// Se o socket está ouvindo conexões
        bool is_listening = false;

};


/** Objeto que representa um datagrama recebido por uma socket UDP.
 *  
 *      Um datagrama UDP além de conter a mensagem, contém informações
 *  a respeito do endereço que enviou a mensagem ao servidor.
 */
class UDPRecv {
    /// Garante que apenas instâncias de UDPSocket possam criar um objeto UDPRecv.
    friend class UDPSocket;

    public:
        /// Get para a variável address.
        std::string get_address() {
            return this->address;
        }

        /// Get para a variável name.
        std::string get_name() {
            return this->name;
        }

        /// Get para a variável msg.
        std::string get_msg() {
            return this->msg;
        }

        /// Get para a variável port.
        uint32_t get_port() {
            return this->port;
        }

    private:

        /// Construtor privado, apenas instâncias de UDPSocket podem instanciar UDPRecv.
        /// @param name Nome canônico do transmissor da mensagem.
        /// @param address Endereço do transmissor da mensagem.
        /// @param msg Mensagem enviada pelo transmissor.
        /// @param port Porta na qual o transmissor enviou a mensagem.
        UDPRecv(const std::string& name, const std::string& address, const std::string& msg, 
                uint32_t port);

        /// Endereço do transmissor da mensagem.
        std::string address;

        /// Nome canônico do transmissor da mensagem.
        std::string name;

        /// Mensagem enviada pelo transmissor.
        std::string msg;

        /// Porta na qual o transmissor enviou a mensagem.
        uint32_t port;
};


/** Wrapper de um socket UDP/IPv4 sem possiblidade de connect().
 *  
 *      Para usar como um servidor (recebendo conexões) construa o objeto,
 *  faça o bind() receba mensagens usando recv() e as responda com send().
 *      Para usar como um cliente basta construir o objeto e usar a função
 *  send() e recv() para trocar informações com o servidor.
 *      A conexão é fechada normalmente usando a função close().
 */
class UDPSocket : public BaseSocket {
    public:

        /// Cria um socket UDP/IPv4.
        /// @param flags Flags opcionais para a chamada de getaddrinfo()
        UDPSocket(int flags = 0);

        /// Envia uma string ao endereço definido.
        /// @param address std::string contendo o endereço IP ou domínio de destino.
        /// @param port    Porta de destino.
        /// @param message Mensagem a ser enviada.
        /// @param flags   Flags opcionais para o envio da mensagem ao socket.
        void sendto(const std::string& address, uint32_t port,
                    const std::string& message, int flags = 0);

        /// Recebe uma string do endereço definido.
        /// @param maxlen Tamanho máximo da mensagem a ser recebida.
        /// @param flags  Flags opcionais para o recebimento da mensagem.
        /// @return std::string contendo a mensagem recebida.
        UDPRecv recvfrom(uint64_t maxlen, int flags = 0);

    private:

};



} // End namespace Socket

#endif