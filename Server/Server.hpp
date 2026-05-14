
#ifndef SERVER_HPP
#define SERVER_HPP

#define RED   "\033[31m"
#define GREEN "\033[32m"
#define BLUE  "\033[34m"
#define RESET "\033[0m"

#include "Client.hpp"
#include <vector>
#include <map>
// #include <cstring>
#include "../channel/parsing.hpp"
#include <csignal>
#include <fcntl.h>

#define ERR_INPUTTOOLONG(nick) \
    ":ircserv 412 " + nick + " :Input line was too long\r\n"

class Server : public Parsing
{
private:
    std::string password;
    int _serverFd;
    std::map<int, Client*> _clients;

public:
    Server(int port, std::string password);
    ~Server();
    void run();

private:
    void acceptClient();
    void handleClient(size_t index);
    void removeClient(size_t index);
    void handleWrite(size_t i);
};

#endif
