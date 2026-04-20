#ifndef PARSING_HPP
# define PARSING_HPP

#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include <string>
#include "channel.hpp"
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>


class Parsing 
{
private:
    std::map<std::string, Channel> _channels;


    
public:
    void printListOfClients();
    bool newMessage(const std::string &line, Client &client, std::map<int, Client*> _allClients);
    void join(Client &clinet, std::string line);
    void kick(std::string line, Client& client);
    void mode(Client &clinet, std::string line);
    void topic(std::string line, Client& client);
    void prvmsghelpre(bool flag, std::string message, Channel &ref, Client& refClient);
    void prvmsg(std::string line);
    static std::vector<std::string> HelperSplit(std::string line, char del);
    
    // void addChannel(Client *client);
    // void listAllChannel();

    ~Parsing();
    static std::string _gethostname();
    void add_Channel(const Channel& channel);
    std::map<std::string, Channel> &Getchannel();
    bool searchForClient(std::string clinet);
    bool searchForChannel(std::string channelName);
    Channel *searchForChannelref(std::string channelName);
    Client *searchForClientref(std::string channelName);
    bool canJoin(const Channel& channel, Client& client);
    void invite(std::string line, Client& client);

    // file handling 
    void getfile(Client &client, std::string line, std::map<int, Client*> _allClients);
    void sendfile(Client &client, std::string line, std::map<int, Client*> _allClients);
    void handleFirstCommand(Client &client, std::string line, std::map<int, Client*> _allClients);
    //boot
    void boot(Client &client, std::string msg);
    // auth
    void pass(Client &client, std::string line);
    void user(Client &client, std::string line);
    void nick(Client &client, std::string line, std::map<int, Client*> _allClients);
    bool checkNick(std::map<int, Client*> _allClients, std::string& value);

    void printer();
};



#endif