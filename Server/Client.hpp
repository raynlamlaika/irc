
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>
#include <string>
#include <sstream>

class Client
{
private:
    int _fd;
    sockaddr_in _addr;
    int numberOfChannelsJoined; 
    std::string name; // add 
    std::string nick;
    bool pass;
    bool auth;
    
public:
    std::string password;
    Client();
    Client(int fd, const sockaddr_in &addr, std::string password);
    ~Client();

    int getFd() const;
    std::string getName() const;
    int receive(char *buffer, size_t size);
    void sendMsg(const std::string &msg);
    int numberOfChannels() const;
    void incrementChannels();
    void decrementChannels();
    void get_informatoin();
    bool getAuth();
    bool getPass();
    void setNick(std::string &value);
    void setname(std::string nameHolder);
    // void setnick(std::string nickHolder);
    void setpass();
    std::string readLine();
    void handleCommand(const std::string &line);
};

#endif
