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

Client::Client() {}

Client::Client(int fd, std::string password)
    : _fd(fd), numberOfChannelsJoined(0), name(""), nick(""), realname("") ,pass(false), auth(false), password(password) , buffer("")
{}

Client::~Client()
{
    std::cout <<"\033[31mClient disconnected fd[" << _fd << "]\033[0m" << std::endl;
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

void Client::appendSendBuffer(const std::string &msg, std::vector<pollfd> &_pollFds, int isbroadcast)
{
    (void)isbroadcast;
    sendBuffer += msg;

    for (size_t i = 0; i < _pollFds.size(); i++)
    {
        if (_pollFds[i].fd == _fd)
        {
            _pollFds[i].events |= POLLOUT;
        }
    }
}


std::string &Client::getSendBuffer()
{
    return sendBuffer;
}


void Client::incrementChannels()
{
    numberOfChannelsJoined++;
}

void Client::decrementChannels()
{
    numberOfChannelsJoined--;
}

bool Client::getAuth()
{
    return auth;
}

bool Client::getPass()
{
    return pass;
}

std::string Client::getNick() const
{
    return nick;
}

void Client::setpass()
{
    pass = true;
}

void Client::setAuth()
{
    auth = true;
}

void Client::setNick(std::string &value)
{
    nick = value;
}
void Client::setrealname(std::string &_realname)
{
    realname = _realname;
}
