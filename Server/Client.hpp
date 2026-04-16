
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
    std::string realname;
    std::string name;
    std::string nick;
    std::vector<std::string> invitedChannels;
    bool pass;
    bool auth;
    bool statusFile;
    bool headerIsGet;
    size_t fileSize;

public:
    std::string password;
    std::string fileout;
    std::string _sendBuffer;

    void setBufferEmpty();
    void appendToSendBuffer(const std::string &data);
    std::string &getSendBuffer();
    Client();
    Client(int fd, std::string password);
    ~Client();

    int getFd() const;
    int receive(char *buffer, size_t size);
    void sendMsg(const std::string &msg);
    int numberOfChannels() const;
    void incrementChannels();
    void decrementChannels();
    void get_informatoin();
    std::string getName() const;
    std::string getNick() const;
    bool getAuth();
    bool getPass();
    void setNick(std::string &value);
    void setname(std::string nameHolder);
    void setrealname(std::string realname);
    void setAuth();

    std::string getfileout();
    void setfileout(std::string path);
    
    bool getstatusFile();
    void setstatusFile(bool val);

    bool getheaderIsGet();
    void setheaderIsGet(bool val);

    size_t getsizeFile();
    void setsizeFile(size_t size);
    void setpass();
};

#endif
