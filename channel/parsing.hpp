#ifndef PARSING_HPP
# define PARSING_HPP

#include <iostream>
#include <map>
#include <vector>
#include "channel.hpp"



class Parsing
{
private:
    std::map<std::string, Channel> _channels;


    
public:
    bool newMessage(const std::string &line, Client &client);
    void join(Client &clinet, std::string line);
    void mode(Client &clinet, std::string line);
    static std::vector<std::string> HelperSplit(std::string line, char del);
    
    
    // void addChannel(Client *client);
    // void listAllChannel();

    // ~Parsing();
    void add_Channel(const Channel& channel);
    std::map<std::string, Channel> &Getchannel();
    void printer();
};



#endif