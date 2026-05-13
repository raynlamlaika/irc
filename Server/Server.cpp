#include "Server.hpp"

Server::Server(int port, std::string password) : password(password)
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0)
        throw std::runtime_error("socket failed");

    int opt = 1;
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        throw std::runtime_error("setsockopt failed");

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_serverFd, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind failed");


    if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("fcntl set failed");

    if (listen(_serverFd, SOMAXCONN) < 0)
        throw std::runtime_error("listen failed");

    pollfd p; 
    p.fd = _serverFd;
    p.events = POLLIN;
    p.revents = 0;
    _pollFds.push_back(p);

    std::cout << BLUE << "Server running on port " << port << "..." << RESET << std::endl;
}

Server::~Server()
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        close(it->first);
        delete it->second;
    }
    close(_serverFd);
}

void Server::acceptClient(size_t index)
{
    (void)index;

    int clientFd = accept(_serverFd, NULL, NULL);

    if (clientFd < 0)
    {
        throw std::runtime_error("accept failed");
    }
    fcntl(clientFd, F_SETFL, O_NONBLOCK);
    pollfd p;
    p.fd = clientFd;
    p.events = POLLIN;
    p.revents = 0;
    _pollFds.push_back(p);

    Client *client = new Client(clientFd, password);
    _clients[clientFd] = client;

    std::cout << GREEN << "New connection " << clientFd << RESET << std::endl;
}

void Server::removeClient(size_t index)
{
    int fd = _pollFds[index].fd;
    Client *client = _clients[fd];

    std::map<std::string, Channel>& channels = Getchannel();
    for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->second.hasClient(client))
        {
            it->second.removeClient(client);
            it->second.removeOperator(client);
            it->second.removeInvited(client);
            if (it->second.getMembers().empty())
                channels.erase(it++);
            else
                ++it;
        }
    }

    delete client;
    _clients.erase(fd);

    _pollFds.erase(_pollFds.begin() + index);
    close(fd);
}

void Server::handleClient(size_t index)
{

    int fd = _pollFds[index].fd;
    Client *client = _clients[fd];
    std::string &msg = client->buffer;
    char buffer[1024];
    int bytes = recv(fd, buffer, sizeof(buffer), 0);

    if (bytes > 0)
    {
        msg.append(buffer, bytes);
    }
    else if (bytes == 0)
    {
        removeClient(index);
        return;
    }
    try {
        if (msg.find("\r\n") != std::string::npos)
        {
            while (msg.find("\r\n") != std::string::npos)
            {
                std::string m = msg.substr(0, msg.find("\r\n"));
                msg.erase(msg.begin(), msg.begin() + msg.find("\r\n") + 2);
                newMessage(m, *client, _clients);
            }
        }
        else
        {
            while (msg.find("\n") != std::string::npos)
            {
                std::string m = msg.substr(0, msg.find("\n"));
                msg.erase(msg.begin(), msg.begin() + msg.find("\n") + 1);
                newMessage(m, *client, _clients);
                
            }
        }  
    } catch (std::exception &e)
    {
        std::cerr << RED <<  e.what() << RESET << std::endl;
        removeClient(index);
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
            if (_pollFds[i].revents & (POLLHUP | POLLERR))
            {
                removeClient(i);
                i--;
            }
            else if (_pollFds[i].revents & POLLOUT)
            {
                std::cout << "index "<< _pollFds[i].fd << std::endl;
                handleWrite(i);
            }
            else if (_pollFds[i].revents & POLLIN)
            {
                if (_pollFds[i].fd == _serverFd)
                    acceptClient(i);
                else
                {
                    size_t prevSize = _pollFds.size();
                    handleClient(i);
                    if (_pollFds.size() < prevSize)
                        i--;
                }
            }
            
        }
    }
}

void Server::handleWrite(size_t i)
{
    int fd = _pollFds[i].fd;

    Client *client = _clients[fd];

    if (!client)
        return;

    std::string &buffer = client->getSendBuffer();

    if (buffer.empty())
    {
        _pollFds[i].events &= ~POLLOUT;
        return;
    }
    ssize_t n = send(fd, buffer.c_str(), buffer.size(), 0);
    if (n <= 0)
    {
        removeClient(i);
        return;
    }

    buffer.erase(0, n);

    if (buffer.empty())
    {
        _pollFds[i].events &= ~POLLOUT;
    }
}