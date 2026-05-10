
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>

class Client
{
private:
    int _fd;
    int numberOfChannelsJoined; 
    std::string name;
    std::string nick;
    std::string realname;

    bool pass;
    bool auth;
public:
    std::vector<std::string> invitedChannels;
    std::string password;
    std::string buffer;
    Client();
    Client(int fd, std::string password);
    ~Client();
    int getFd() const;
    int receive(char *buffer, size_t size);
    void sendMsg(const std::string &msg);
    int numberOfChannels() const;
    void incrementChannels();
    void decrementChannels();
    std::string getName() const;
    std::string getNick() const;
    bool getAuth();
    bool getPass();
    void setNick(std::string &value);
    void setname(std::string nameHolder);
    void setrealname(std::string &_realname);
    void setAuth();
    void setpass();
};

#endif
