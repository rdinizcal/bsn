/**
 * Copyright (c) 2018 Mikael Mello
 * Licensed under the MIT license.
 * https://opensource.org/licenses/MIT
 */
 
#include "sockets.hpp"

#include <iostream>

using namespace Socket;

BaseSocket::BaseSocket(int type, int flags) {

    // Inicializa socket_info
    socket_info = (addrinfo *) calloc(1, sizeof(addrinfo));

    // Prenche a estrutura de endereço do socket, sempre IPv4
    socket_info->ai_family = AF_INET;
    socket_info->ai_socktype = type;
    socket_info->ai_flags = flags;

    // Mesma família (IPv4) e tipo de socket da socket_info
    socketfd = socket(AF_INET, type, 0);
    if (socketfd == -1) {
        throw SocketException("Could not create socket. Error: " + std::string(strerror(errno)));
    }

    // Permite reuso de endereço (evita "Address already in use")
    int optval = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

BaseSocket::BaseSocket(int socket, int type, addrinfo sInfo, const std::string& client_ip, uint32_t port, bool is_bound) :
    socketfd(socket), ip_address_str(client_ip), port_used(port), is_bound(is_bound) {

    // Verifica que o endereço é IPv4 e do mesmo tipo definido pelo parâmetro
    if (sInfo.ai_family != AF_INET || sInfo.ai_socktype != type) {
        throw SocketException("sInfo passed is not defined as the defined type and protocol IPv4");
    }

    // Copia as inforamações do sInfo (sockaddr).
    socket_info = (addrinfo *) malloc(sizeof(addrinfo));
    memcpy(socket_info, &sInfo, sizeof(addrinfo));

    // Permite reuso de endereço (evita "Address already in use")
    int optval = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

BaseSocket::~BaseSocket() {
    // Libera memória e encerra o socket
    if (socket_info) freeaddrinfo(socket_info);
    close();
}

void BaseSocket::bind(uint32_t port) {

    // Não é possivel vincular um socket que já está vinculado
    // Logo se a porta desejada for a mesma que já está vinculada, nada acontece.
    // Uma exceção é lançada caso contrário
    if (is_bound && port_used != port) {
        throw ConnectionException("Socket is already bound to port " 
            + std::to_string(port_used));
    }
    else if (is_bound && port_used == port) {
        return;
    }

    // Pela documentação:
    //     If the AI_PASSIVE bit is set it indicates that the
    // returned socket address structure is intended for use
    // in a call to bind(2). In this case, if the hostname 
    // argument is the null pointer, then the IP address portion
    // of the socket address structure will be set to INADDR_ANY 
    // for an IPv4 address
    socket_info->ai_flags |= AI_PASSIVE;
    getAddrInfo("NULL", port);


    int result;
    int total_ipv4_addrs = 0; // Conta quandos endereços IPv4 foram encontrados
    addrinfo * new_addrinfo;

    for (new_addrinfo = socket_info; new_addrinfo; new_addrinfo = new_addrinfo->ai_next) {

        // Ignora qualquer endereço não IPv4
        if (new_addrinfo->ai_family == AF_INET) total_ipv4_addrs++;
        else continue;

        // Tenta vincular a qualquer endereço IPv4 encontrado.
        result = ::bind(socketfd, new_addrinfo->ai_addr, new_addrinfo->ai_addrlen);
        if (result == -1) continue;

        // Se a operação é bem-sucedida, seta as variáveis e retorna
        port_used = port;
        is_bound = true;
        return;
    }

    // Se nenhum endereço IPv4 é lançado, lança exceção
    if (total_ipv4_addrs == 0) {
        throw ConnectionException("Could not find IPv4 addresses to bind on port " 
            + std::to_string(port) + ".");
    }

    // Lança exceção caso não seja possível vincular a nenhum IPv4 encontrado.
    throw ConnectionException("Could not bind to port " + std::to_string(port) 
            + ". Error: " + std::string(strerror(errno)));
}

void BaseSocket::set_timeout(uint16_t seconds) {
    struct timeval tv;
    tv.tv_sec = seconds; 
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
    setsockopt(this->socketfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv,sizeof(struct timeval));
}

void BaseSocket::close() {

    // Se a socket não foi setada ou já foi fechada, nada acontece.
    if (socketfd == -1 || is_closed) return;

    int result = ::close(socketfd);
    if (result == -1) {
        throw SocketException("Could not close socket. Error: " + 
            std::string(strerror(errno)));
    }

    is_bound = false;
    is_closed = true;
    port_used = -1;
    socketfd = -1;

}

void BaseSocket::getAddrInfo(const std::string address, uint32_t port) {

    addrinfo   hints = *socket_info;
    addrinfo * new_socket_info;
    const char * c_address;

    // c_address = NULL significa que o getaddrinf() buscará por endereços
    // locais da maquina
    if (address == "NULL") c_address = NULL;
    else c_address = address.c_str();

    int result = getaddrinfo(c_address, std::to_string(port).c_str(), &hints, &new_socket_info);

    if (result != 0) {
        throw ConnectionException("Error on getaddrinfo(): " + 
            std::string(gai_strerror(result)));
    }

    // Libera a memoria da socket_info atual para ela ser realocada por getaddrinfo()
    if (socket_info) freeaddrinfo(socket_info);
    socket_info = new_socket_info;  
}

bool BaseSocket::validateIpAddress(const std::string &ip_address) {
    sockaddr_in sa;
    int result = ::inet_pton(AF_INET, ip_address.c_str(), &(sa.sin_addr));
    return result != 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                  TCPSOCKET
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

TCPSocket::TCPSocket(int flags) : BaseSocket(SOCK_STREAM, flags) {}

TCPSocket::TCPSocket(int socket, addrinfo socket_info, uint32_t port_used, const std::string& client_ip, bool is_bound, bool is_listening, bool is_connected) : 
    BaseSocket(socket, SOCK_STREAM, socket_info, client_ip, port_used, is_bound), is_listening(is_listening), is_connected(is_connected) {
}

void TCPSocket::connect(const std::string& address, uint32_t port) {

    if (is_listening) {
        throw SocketException("Socket is already listening incoming connections on port " 
                                + std::to_string(port_used));
    }
    if (is_connected) {
        throw SocketException("Socket is already connected to an address");
    }
    
    getAddrInfo(address, port);

    int result;
    int total_ipv4_addrs = 0;
    addrinfo * new_addrinfo;

    for (new_addrinfo = socket_info; new_addrinfo; new_addrinfo = new_addrinfo->ai_next) {

        if (new_addrinfo->ai_family == AF_INET) total_ipv4_addrs++;
        else continue;

        result = ::connect(socketfd, new_addrinfo->ai_addr, new_addrinfo->ai_addrlen);
        if (result == -1) continue;

        is_connected = true;
        return;
    }

    if (total_ipv4_addrs == 0) {
        throw ConnectionException("Could not find IPv4 addresses to connect to " + address + 
                                  " on port " + std::to_string(port) + ".");
    }

    throw ConnectionException("Could not connect to address " + address +
                              " on port " + std::to_string(port) + 
                              ". Error: " + std::string(strerror(errno)));

}
        
void TCPSocket::listen(uint32_t backlog) {

    if (!is_bound) {
        throw SocketException("Can not listen when socket is not bound to any port");
    }
    if (is_listening) {
        throw SocketException("Socket is already listening incoming connections on port " 
                                + std::to_string(port_used));
    }

    int result = ::listen(socketfd, backlog);
    if (result == -1) {
        throw ConnectionException("Could not listen on port " + std::to_string(port_used) 
                                + ". Error: " + std::string(strerror(errno)));
    }

    is_listening = true;

}

std::shared_ptr<TCPSocket> TCPSocket::accept() {

    if (!is_listening) {
        throw SocketException("Socket is not listening to incoming connections to accept one");
    }

    sockaddr client_address;
    socklen_t sin_size = sizeof(sockaddr);
    int clientfd = ::accept(socketfd, &client_address, &sin_size);

    if (clientfd == -1) {
        throw ConnectionException("Error while accepting a connection. Error: " 
            + std::string(strerror(errno)));
    }

    addrinfo client_info;
    memset(&client_info, 0, sizeof(addrinfo));

    client_info.ai_family = AF_INET;
    client_info.ai_socktype = SOCK_STREAM;
    client_info.ai_addr = &client_address;

    sockaddr_in* client_address_in = (sockaddr_in*)&client_address;
    std::string client_ip = std::string(::inet_ntoa(client_address_in->sin_addr));

    return std::make_shared<TCPSocket>(clientfd, client_info, 
        client_address_in->sin_port, client_ip, false, false, true);

}
        
void TCPSocket::send(const std::string& message, int flags) {
    send((uint8_t *) message.c_str(), message.length(), flags);
}
        
void TCPSocket::send(const uint8_t* message, int length, int flags) {

    if (is_listening || !is_connected) {
        throw SocketException("Can't send message from a socket that is not connected");
    }

    int bytes_left = length;
    int bytes_sent = 0;
    const uint8_t* message_left;

    while (bytes_sent < length) {

        message_left = message + bytes_sent;

        int result = ::send(socketfd, message_left, bytes_left, flags | MSG_NOSIGNAL);
        if (result == -1) {
            throw ConnectionException("Could not send message. Error: " 
                + std::string(strerror(errno)));
        }

        bytes_left -= result;
        bytes_sent += result;

    }

}

std::string TCPSocket::recv(uint64_t maxlen, int flags) {

    uint8_t* message    = recv(&maxlen, flags);
    uint8_t* message_nt = (uint8_t *) malloc(maxlen+1);
    memcpy(message_nt, message, maxlen);
    message_nt[maxlen]  = '\0';

    std::string string_msg((char *) message_nt);

    free(message);
    if (message_nt != message) free(message_nt);
    return string_msg;

}

uint8_t* TCPSocket::recv(uint64_t* length, int flags) {

    if (is_listening || !is_connected) {
        throw SocketException("Can't receive message in a socket that is not connected");
    }

    uint64_t maxlen = *length;

    uint8_t buffer[maxlen];
    int result = ::recv(socketfd, buffer, maxlen, flags);

    if (result == -1) {
        throw ConnectionException("Could not receive message. Error: " 
            + std::string(strerror(errno)));
    }
    if (result ==  0) {
        is_connected = false;
        throw ClosedConnection("Client closed connection.");
    }

    uint8_t* message = (uint8_t *) malloc(result);
    memcpy(message, buffer, result);
    *length = result;

    return message;

}

void TCPSocket::close() {

    BaseSocket::close();

    is_connected = false;
    is_listening = false;

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                  UDPRECV
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UDPRecv::UDPRecv(const std::string& name, const std::string& address, const std::string& msg, uint32_t port) :
                 name(name), address(address), msg(msg), port(port) {
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                  UDPSOCKET
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UDPSocket::UDPSocket(int flags) : BaseSocket(SOCK_DGRAM, flags) {

int broadcast_enable = 1;
int ret = setsockopt(socketfd, SOL_SOCKET, SO_BROADCAST, 
    &broadcast_enable, sizeof(broadcast_enable));
}

void UDPSocket::sendto(const std::string& address, uint32_t port, 
    const std::string& message, int flags) {

    int msg_length = message.length();
    int bytes_left = msg_length;
    int bytes_sent = 0;
    const char* cMessage = message.c_str();

    socklen_t server_length;
    sockaddr_in server_address;
    hostent *server;

    /* build the server's Internet address */
    ::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;

    if (!validateIpAddress(address)) {
        server = ::gethostbyname(address.c_str());
        if (server == NULL) {
            throw ConnectionException("No such host as " + address);
        }
        ::memcpy(server->h_addr, &server_address.sin_addr.s_addr, server->h_length);

    }
    else {
        ::inet_pton(AF_INET, address.c_str(), &(server_address.sin_addr));
    }

    server_address.sin_port = ::htons(port);
    server_length = sizeof(server_address);

    while (bytes_sent < msg_length) {

        const char* message_left = cMessage + bytes_sent;

        int result = ::sendto(socketfd, message_left, bytes_left, flags | MSG_NOSIGNAL, 
                             (sockaddr*) &server_address, server_length);
        if (result == -1) {
            throw ConnectionException("Could not send message. Error: " 
                + std::string(strerror(errno)));
        }

        bytes_left -= result;
        bytes_sent += result;

    }

}

UDPRecv UDPSocket::recvfrom(uint64_t maxlen, int flags) {
    
    sockaddr_in client_address;
    hostent* client_info;
    socklen_t clientLength = sizeof(client_address);

    char buffer[maxlen+1];
    int result = ::recvfrom(socketfd, buffer, maxlen, flags, (sockaddr *) &client_address, &clientLength);

    if (result == -1) {
        throw ConnectionException("Could not receive message. Error: " 
            + std::string(strerror(errno)));
    }
    buffer[result] = '\0';


    std::string host_name("Nao importa mais");
    std::string host_address(::inet_ntoa(client_address.sin_addr));
    std::string message(buffer);

    int port = ::ntohs(client_address.sin_port);

    return UDPRecv(host_name, host_address, message, port);


}

