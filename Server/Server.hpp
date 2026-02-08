
#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include <vector>
#include <map>
#include <poll.h>
#include <arpa/inet.h>
#include "../channel/parsing.hpp"

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
    
};

#endif
