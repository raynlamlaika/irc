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
    (void)index;
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
    Client *client = new Client(clientFd, addr, password);
    _clients[clientFd] = client;

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
    if (bytes == 0)
    {
        removeClient(index);
        return;
    }
    buffer[bytes] = '\0';
    std::string msg(buffer);
    if (msg.size() > 0 && msg.substr(msg.size() - 1) == "\n")
        msg = msg.erase(msg.size() - 1);
    std::cout << "msg is : ["<< msg << "]" << std::endl;
    newMessage(msg, *client, _clients);
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
                    break;
                }
            }

            // if (_pollFds[i].revents & POLLOUT)
            // {
            //     int fd = _pollFds[i].fd;
            //     Client *client = _clients[fd];

            //     std::string &buffer = client->getSendBuffer();
            //     std::cout << "BUFFER : " << buffer << std::endl;
            //     if (!buffer.empty())
            //     {
            //         int sent = send(fd, buffer.c_str(), buffer.size(), 0);
            //         if (sent > 0)
            //             buffer.erase(0, sent);
            //     }
            // }
        }
        // for (size_t i = 0; i < _pollFds.size(); i++)
        // {
        //     if (_pollFds[i].revents & POLLIN)
        //     {
        //         if (_pollFds[i].fd == _serverFd)
        //             acceptClient(i);
        //         else
        //         {
        //             handleClient(i);
        //             break; // IMPORTANT: vector may change
        //         }
        //     }
        // }
    }
}
