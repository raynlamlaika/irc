#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>
#include <string>

class Client
{
private:
    int _fd;
    sockaddr_in _addr;
    int numberOfChannelsJoined; 
    std::string name; // add 
    

public:
    Client();
    Client(int fd, const sockaddr_in &addr);
    ~Client();

    int getFd() const;
    std::string getName() const;
    int receive(char *buffer, size_t size);
    void sendMsg(const std::string &msg);
    void setname(std::string nameHolder);
    int numberOfChannels() const;
    void incrementChannels();
    void decrementChannels();

};

#endif
