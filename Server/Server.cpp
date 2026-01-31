#include "Server.hpp"
#include <cstring>
#include <iostream>

Server::Server(int port, std::string password) : password(password)
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0)
        throw std::runtime_error("socket failed");

    int opt = 1;
    setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_serverFd, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind failed");

    if (listen(_serverFd, 10) < 0)
        throw std::runtime_error("listen failed");

    pollfd p;
    p.fd = _serverFd;
    p.events = POLLIN;
    p.revents = 0;
    _pollFds.push_back(p);

    std::cout << "Server running on port " << port << std::endl;
}

Server::~Server()
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        delete it->second;
    close(_serverFd);
}

void Server::acceptClient(size_t index)
{
    sockaddr_in addr;
    socklen_t len = sizeof(addr);

    int clientFd = accept(_serverFd, (sockaddr*)&addr, &len);
    if (clientFd < 0)
        return;

    pollfd p;
    p.fd = clientFd;
    p.events = POLLIN;
    p.revents = 0;
    _pollFds.push_back(p);

    Client *client = new Client(clientFd, addr);
    _clients[clientFd] = client;

    client->sendMsg("What is the Password ? \n\r");
    char buffer[512];
    int bytes = client->receive(buffer, sizeof(buffer) - 1);

    if (bytes <= 0)
    {
        std::cout << "Client disconnected fd[" << clientFd << "]\n";
        return;
    }
    buffer[bytes - 1] = '\0';
    std::string pass = buffer;
    if (pass != password)
    {
        client->sendMsg("sorry bad password\n\r");
        removeClient(index);
        close(clientFd);
    }
    else
    {
        std::cout << "from Client : " << buffer << std::endl;
        client->sendMsg("Welcome to the server\n\r");
    }
}

void Server::removeClient(size_t index)
{
    int fd = _pollFds[index].fd;

    delete _clients[fd];
    _clients.erase(fd);

    _pollFds.erase(_pollFds.begin() + index);
}

void Server::handleClient(size_t index)
{
    int fd = _pollFds[index].fd;
    Client *client = _clients[fd];

    char buffer[512];
    int bytes = client->receive(buffer, sizeof(buffer) - 1);

    if (bytes == 0) // wwaaa team mate
    {
        std::cout << "Client disconnected fd[" << fd << "]\n";
        removeClient(index);
        return;
    }

    buffer[bytes] = '\0';

    // normalize CRLF (nc / telnet safe)
    for (int i = 0; buffer[i]; i++)
    {
        if (buffer[i] == '\r')
            buffer[i] = '\n';
    }

    std::cout << "Client[" << fd << "] -> " << buffer;

    // ensure message ends with \r\n
    std::string msg(buffer);
    if (msg.size() < 2 || msg.substr(msg.size() - 2) != "\r\n")
        msg += "\r\n";

    /// you'r part start from here 
    newMessage(msg, *client);

    // broadcast to ALL (including sender)
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second->getFd() != fd)
            it->second->sendMsg(msg);
    }
}

void Server::run()
{
    while (true)
    {
        if (poll(&_pollFds[0], _pollFds.size(), -1) < 0)
            throw std::runtime_error("poll failed");

        for (size_t i = 0; i < _pollFds.size(); i++)
        {
            if (_pollFds[i].revents & POLLIN)
            {
                if (_pollFds[i].fd == _serverFd)
                    acceptClient(i);
                else
                {
                    handleClient(i);
                    break; // IMPORTANT: vector may change
                }
            }
        }
    }
}
