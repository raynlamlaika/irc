
#ifndef SERVER_HPP
#define SERVER_HPP

#define RED   "\033[31m"
#define GREEN "\033[32m"
#define BLUE  "\033[34m"
#define RESET "\033[0m"

#include "Client.hpp"
#include <vector>
#include <map>
#include <poll.h>
#include <cstring>
#include "../channel/parsing.hpp"
#include <csignal>

class Server : public Parsing
{
private:
    std::string password;
    int _serverFd;
    std::vector<pollfd> _pollFds;
    std::map<int, Client*> _clients;

public:
    Server(int port, std::string password);
    ~Server();
    void run();

private:
    void acceptClient(size_t index);
    void handleClient(size_t index);
    void removeClient(size_t index);
    void printClients();

    std::map<int, Client*>  getClient() const
    {
        return (_clients);
    }

    void addClient(Client* client)
    {
        if (!client)
            return;

        _clients[client->getFd()] = client;
    }
    std::string readLine(Client *client);
};

#endif
