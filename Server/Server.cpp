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
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_serverFd, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind failed");

    if (listen(_serverFd, SOMAXCONN) < 0)
        throw std::runtime_error("listen failed");

    pollfd p1;
    p1.fd = STDIN_FILENO;
    p1.events = POLLIN;
    _pollFds.push_back(p1);
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
        delete it->second;
    close(_serverFd);
}

void Server::acceptClient(size_t index)
{   
    (void)index;

    int clientFd = accept(_serverFd, NULL, NULL);
    if (clientFd < 0)
        return;
    pollfd p;
    p.fd = clientFd;
    p.events = POLLIN;
    p.revents = 0;
    _pollFds.push_back(p);
    Client *client = new Client(clientFd, password);
    _clients[clientFd] = client;
    std::cout << GREEN << "New connection "<< clientFd << RESET << std::endl;
}

void Server::removeClient(size_t index)
{
    int fd = _pollFds[index].fd;

    delete _clients[fd];
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
    int bytes = recv(fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT); // or 0 
    if (bytes > 0)
    {
        buffer[bytes] = '\0';
        msg.append(buffer, bytes);
    }
    else if (bytes == 0)
    {
        removeClient(index);
        return;
    }
    else
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        else
        {
            removeClient(index);
            return;
        }
    }
    std::cout << msg << std::endl;
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
                else if (_pollFds[i].fd == STDIN_FILENO)
                {
                    char buf[128];
                    int n = read(STDIN_FILENO, buf, sizeof(buf));
                    if (n == 0)
                        throw std::runtime_error("\nShutting down server...");
                }
                else
                {
                    handleClient(i);
                    // break;
                }
            }
        }

    }
}
