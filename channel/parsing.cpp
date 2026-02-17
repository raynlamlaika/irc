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


//check for this  : RPL_TOPICWHOTIME (333)  "<client> <channel> <nick> <setat>"
void Parsing::topic(std::string line, Client& client)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');
    if (holder.size() < 2)
    {
        // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
        std::cout << client.getName() << " TOPIC :Not enough parameters\n";
        return;
    }
    if (!searchForChannel(holder[1])) {
        // ERR_NOSUCHCHANNEL (403)  "<client> <channel> :No such channel"
        std::cout << client.getName() << " " << holder[1] << " :No such channel\n";
        return;
    }
    Channel *channel = searchForChannelref(holder[1]);
    if (!channel->hasClient(client)) {
        // ERR_NOTONCHANNEL (442) "<client> <channel> :You're not on that channel"
        std::cout << client.getName() << " " << holder[1] << " :You're not on that channel\n";
        return;
    }
    size_t index = line.find(':');
    if (index == std::string::npos) {
        // No ':' found, just print the current topic
        // RPL_TOPIC (332)  "<client> <channel> :<topic>"
        std::cout << client.getName() << " " << holder[1] << " :" << channel->getTopic() << std::endl;
        return;
    }
    if (!channel->isOperator(client)) {
        // ERR_CHANOPRIVSNEEDED (482) "<client> <channel> :You're not channel operator"
        std::cout << client.getName() << " " << holder[1] << " :You're not channel operator\n";
        return;
    }
    std::string topicUse = line.substr(index + 1); // skip the ':'
    if (topicUse.empty())
    {
        // RPL_NOTOPIC (331)   "<client> <channel> :No topic is set"
        std::cout << client.getName() << " " << holder[1] << " :No topic is set\n";
        return;
    }
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
    // printListOfClients();

    std::vector<std::string> holder;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word)
        holder.push_back(word);
    if (holder[0] == "JOIN")
        join(client, line);
    // else if (holder[0] == "MODE")
    //     mode(client, line);
    else if (holder[0] == "KICK")
        kick(line, client);
    else if (holder[0] == "TOPIC")
        topic(line, client);
    // else if (holder[0] == "PRIVMSG")
    //     prvmsg(line);m
    // else if (holder[0] == "INVTE")
    // {
    //     std::cout << holder[0] << std::endl;
    // }
    
    return (true);
}





/*
the numrical reply i should handel

ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_TOOMANYCHANNELS (405)
ERR_BADCHANNELKEY (475)
ERR_BANNEDFROMCHAN (474)
ERR_CHANNELISFULL (471)
ERR_INVITEONLYCHAN (473)
ERR_BADCHANMASK (476)
RPL_TOPIC (332)
RPL_TOPICWHOTIME (333)
RPL_NAMREPLY (353)
RPL_ENDOFNAMES (366)

*/