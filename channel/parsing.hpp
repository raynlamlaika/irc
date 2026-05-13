#ifndef PARSING_HPP
# define PARSING_HPP

#include "replices.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include <string>
#include "channel.hpp"
#include <fstream>
#include <unistd.h>
#include <poll.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>

typedef struct s_mode
{
    char mode;
    char sign;
    std::string param;
} t_mode;

class Parsing
{
private:
    std::map<std::string, Channel> _channels;
public:

    std::vector<pollfd> _pollFds;
    bool validName(std::string name, Client *client);
    bool checkBan(const Channel& channel, Client& client);
    void printTopic(const Channel& channel, Client *client);
    void sendInviteList(Parsing& parsing, Client& client);
    // void printListOfClients();
    bool newMessage(const std::string &line, Client &client, std::map<int, Client*> _allClients);
    void join(Client &clinet, std::string line);
    void kick(std::string line, Client& client);
    void mode(Client &clinet, std::string line, std::map<int, Client*> _allClients);
    void topic(std::string line, Client& client);
    // void prvmsghelpre(bool flag, std::string message, Channel &ref, Client& refClient);
    void prvmsg(std::string line, Client& client,  std::map<int, Client*> _allClients);
    static std::vector<std::string> HelperSplit(std::string line, char del);
    std::string printListOfUsers(std::map<int, Client*> clients, const Channel& channel);
    
    ~Parsing();
    static std::string _gethostname();
    void add_Channel(const Channel& channel);
    std::map<std::string, Channel> &Getchannel();
    bool searchForClient(std::string clinet ,std::map<int, Client*> _allClients);
    bool searchForChannel(std::string channelName);
    Channel *searchForChannelref(std::string channelName);
    Client *searchForClientref(std::string channelName, std::map<int, Client*> _allClients);
    bool canJoin(const Channel& channel, Client& client);
    void invite(std::string line, Client& client,std::map<int, Client*> _allClients);

    // file handling 
    // void getfile(Client &client, std::string line, std::map<int, Client*> _allClients);
    // void sendfile(Client &client, std::string line, std::map<int, Client*> _allClients);
    // void handleFirstCommand(Client &client, std::string line, std::map<int, Client*> _allClients);
    //boot
    void boot(Client &client, std::string msg);
    // auth
    void pass(Client &client, std::string line);
    void user(Client &client, std::string line);
    void nick(Client &client, std::string line, std::map<int, Client*> _allClients);
    bool checkNick(std::map<int, Client*> _allClients, std::string& value);
    void sendWelcome(Client& client);
    void printer();

    //mode 
    void modeInviteOnly(Client &client, std::map<std::string, Channel>::iterator& it, t_mode& mode, std::vector<pollfd>& _pollFds);
    void modeKey(Client &client, std::map<std::string, Channel>::iterator& it, t_mode& mode, std::vector<pollfd>& _pollFds);
    void modeOperator(Client &client, std::map<std::string, Channel>::iterator& it, t_mode& mode, std::vector<pollfd>& _pollFds,std::map<int, Client*> _allClients);
    void modeTopic(Client &client, std::map<std::string, Channel>::iterator& it, t_mode& mode, std::vector<pollfd>& _pollFds);
    void modeUserLimit(Client &client, std::map<std::string, Channel>::iterator& it, t_mode& mode, std::vector<pollfd>& _pollFds);
};



#endif