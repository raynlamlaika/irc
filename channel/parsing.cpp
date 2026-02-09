#include "parsing.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>



static bool validStr(const std::string& str)
{
    for (int i = 0 ;i < str.size() ; i++)
    {
        if (!std::isprint(str[i]))
        {
            return false;
        }
    }
    return true;
}

Channel *Parsing::searchForChannelref(std::string channelName)
{
    std::map<std::string, Channel>& chs = Getchannel();
    std::map<std::string, Channel>::iterator it;
    for (it = chs.begin() ; it != chs.end() ; it++)
    {
        if (it->first == channelName)
            return &it->second;
    }
    return NULL;
}

Client *Parsing::searchForClientref(std::string name)
{
    std::map<std::string, Channel> chns = Getchannel();
    std::map<std::string, Channel>::iterator it;

    for(it = chns.begin();it != chns.end(); it++)
    {
        std::map<int, Client*> ClientList = it->second.getmembers();
        std::map<int, Client*>::iterator  Cit;
        for ( Cit =  ClientList.begin(); Cit !=  ClientList.end(); Cit++)
        {   
            if (Cit->second->getName() == name)
                return Cit->second;
        }
    }
    return NULL;
}

void Parsing::topic(std::string line)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');
    if (holder.size() < 2)
        return;
    if (!searchForChannel(holder[1])) {
        std::cout << "can't find target channel\n";
        return;
    }
    Channel *channel = searchForChannelref(holder[1]);
    size_t index = line.find(':');
    if (index == std::string::npos) {
        // No ':' found, just print the current topic
        std::cout << channel->getTopic() << std::endl;
        return;
    }
    std::string topicUse = line.substr(index + 1); // skip the ':'
    if (topicUse.empty())
        channel->setTopic("");
    else
        channel->setTopic(topicUse);
    

}

void Parsing::printListOfClients()
{

    std::string p = "-------------------------------------------------------------";
    std::cout << p <<"\n";
    std::map<std::string, Channel>& chns = Getchannel();

    for (std::map<std::string, Channel>::iterator it = chns.begin();
         it != chns.end(); ++it)
    {
        Channel& channel = it->second;
        std::map<int, Client*> members = channel.getmembers();
        std::cout << "-- Channel: " << channel.getName() << "\n";
        for (std::map<int, Client*>::iterator cit = members.begin();cit != members.end(); ++cit)
        {
            std::cout << "\t\t" << channel.getName() << " | Client: " << cit->second->getName() << std::endl;
        }
    }
    std::cout << p <<"\n";

}

void cccccl(std::map<int, Client*> _clients)
{
    for (std::map<int, Client*>::const_iterator it = _clients.begin();
         it != _clients.end();
         ++it)
    {
        Client* c = it->second;
        if (!c)
            continue;

        std::cout << "FD: " << it->first
                  << " | Name: " << c->getName()
                  << std::endl;
    }
}

bool Parsing::newMessage(const std::string &line, Client &client, std::map<int, Client*> _allClients)
{
    
    cccccl(_allClients);
    printListOfClients();
    // std::cout << "\n\n";

    std::vector<std::string> holder;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word)
        holder.push_back(word);
    if (holder[0] == "JOIN")
        join(client, line);
    else if (holder[0] == "MODE")
        mode(client, line);
    else if (holder[0] == "KICK")
        kick(line);
    else if (holder[0] == "TOPIC")
        topic(line);
    else if (holder[0] == "PRIVMSG")
        prvmsg(line);
    else if (holder[0] == "INVTE")
    {
        std::cout << holder[0] << std::endl;
    }
    
    return (true);
}