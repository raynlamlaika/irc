#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <string>

class Client
{
private:
    int _fd;
    sockaddr_in _addr;
    std::string name; // add 
    

public:
    Client()
    {
        _addr.sin_port = 133;
        std::cout << "Client connected fd[" << _fd << "]\n";
    }
    Client(int fd, const sockaddr_in &addr) : _fd(fd), _addr(addr)
    {
        std::cout << "Client connected fd[" << _fd << "]\n";
    }

    ~Client()
    {
        std::cout << "Client disconnected fd[" << _fd << "]\n";
        close(_fd);
    }
    int getFd() const;
    std::string getName() const
    {
        return name;
    }
    int receive(char *buffer, size_t size)
    {
        return recv(_fd, buffer, size, 0);
    }
    void sendMsg(const std::string &msg)
    {
        send(_fd, msg.c_str(), msg.size(), 0);
    }
    void setname(std::string nameHolder);
};

#endif
