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

void Client::setrealname(std::string realname)
{
    realname = realname;
}

Client::Client(){}

Client::Client(int fd, std::string password) 
    : _fd(fd), numberOfChannelsJoined(0), realname(""), name(""), nick(""), pass(false), auth(false), statusFile(false), headerIsGet(false), fileSize(0), password(password)
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

std::string Client::getfileout()
{
    return this->fileout;
}

void Client::setfileout(std::string path)
{
    this->fileout = path;
}

bool Client::getstatusFile()
{
    return this->statusFile;
}

void Client::setstatusFile(bool val)
{
    this->statusFile = val;
}


bool Client::getheaderIsGet()
{
    return this->headerIsGet;
}

void Client::setheaderIsGet(bool val)
{
    this->headerIsGet = val;
}

size_t Client::getsizeFile()
{
    return this->fileSize;
}

void Client::setsizeFile(size_t size)
{
    this->fileSize = size;
}

void Client::setBufferEmpty()
{
        _sendBuffer = "";
};
void Client::appendToSendBuffer(const std::string &data)
{
    _sendBuffer += data;
}

std::string& Client::getSendBuffer()
{
    return _sendBuffer;
}