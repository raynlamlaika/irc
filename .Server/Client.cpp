#include "Client.hpp"



int Client::getFd() const
{
    return _fd;
}

int Client::numberOfChannels() const 
{
    return numberOfChannelsJoined;
}

void Client::setname(std::string nameHolder)
{
    name = nameHolder;
}
Client::Client()
{
    _addr.sin_port = 133;
    numberOfChannelsJoined = 0;
    std::cout << "Client connected fd[" << _fd << "]\n";
}
Client::Client(int fd, const sockaddr_in &addr) : _fd(fd), _addr(addr)
{
    std::cout << "Client connected fd[" << _fd << "]\n";
}

Client::~Client()
{
    std::cout << "Client disconnected fd[" << _fd << "]\n";
    close(_fd);
}
std::string Client::getName() const
{
    return name;
}

int Client::receive(char *buffer, size_t size)
{
    return recv(_fd, buffer, size, 0);
}

void Client::sendMsg(const std::string &msg)
{
    send(_fd, msg.c_str(), msg.size(), 0);
}


void Client::incrementChannels()
{
    numberOfChannelsJoined++;
}

void Client::decrementChannels()
{
    numberOfChannelsJoined--;
}