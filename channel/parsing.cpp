#include "parsing.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

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

Parsing::~Parsing()
{}

//check for this  : RPL_TOPICWHOTIME (333)  "<client> <channel> <nick> <setat>"
void Parsing::topic(std::string line, Client& client)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');
    if (holder.size() < 2)
    {
        // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
        std::string msg = client.getName() + " TOPIC :Not enough parameters\n";
        client.sendMsg(msg);
        return;
    }
    if (!searchForChannel(holder[1])) {
        // ERR_NOSUCHCHANNEL (403)  "<client> <channel> :No such channel"
        std::string msg = client.getName() + " " + holder[1] + " :No such channel\r\n";
        client.sendMsg(msg);
        return;
    }
    Channel *channel = searchForChannelref(holder[1]);
    if (!channel->hasClient(&client)) {
        // ERR_NOTONCHANNEL (442) "<client> <channel> :You're not on that channel"
        std::string msg = client.getName() + " " + holder[1] + " :You're not on that channel\n";
        client.sendMsg(msg);
        return;
    }
    size_t index = line.find(':');
    if (index == std::string::npos) {
        // No ':' found, just print the current topic
        // RPL_TOPIC (332)  "<client> <channel> :<topic>"
        if (channel->getTopic().empty())
        {
            // RPL_NOTOPIC (331)   "<client> <channel> :No topic is set"
            std::string msg = client.getName() + " " + holder[1] + " :No topic is set\n";
            client.sendMsg(msg);
        }
        else
        {
            std::string msg = client.getName() + " " + holder[1] + " :" + channel->getTopic() + "\n";
            client.sendMsg(msg);
        }
        return;
    }
    if (!channel->isOperator(client))
    {
        // ERR_CHANOPRIVSNEEDED (482) "<client> <channel> :You're not channel operator"
        std::string msg = client.getName() + " " + holder[1] + " :You're not channel operator\n";
        client.sendMsg(msg);
        return;
    }
    std::string topicUse = line.substr(index + 1); // skip the ':'
    std::cout << "line is : " << line << std::endl;
    std::cout << "topicUse: " << topicUse << std::endl;
    if (topicUse.empty())
    {
        // RPL_NOTOPIC (331)   "<client> <channel> :No topic is set"
        std::string msg = client.getName() + " " + holder[1] + " :No topic is set\n";
        client.sendMsg(msg);
        return;
    }
    else
    {    
        channel->setTopic(topicUse);
        // breadcast for all of the channel members
        std::string msg = client.getName() + " has changed the topic to: " + topicUse + "\n";
        channel->broadcastMsg(msg, channel->getMembers());
        // channel->setTopicSetBy(client.getName());
        // channel->setTopicSetAt();
    }
}

void Parsing::printListOfClients()
{

    std::cout << "\033[0;32m" <<"\n";
    std::map<std::string, Channel>& chns = Getchannel();

    for (std::map<std::string, Channel>::iterator it = chns.begin();
         it != chns.end(); ++it)
    {
        Channel& channel = it->second;
        std::map<int, Client*> members = channel.getmembers();
        std::cout << "-- Channel: " << channel.getName()  << " Password: " << (channel.hasKey() ?("\e[33m" + channel.getKey() + "\033[0;32m") : "\033[0;31mNO Password set\033[0;32m") << "\n";
        for (std::map<int, Client*>::iterator cit = members.begin();cit != members.end(); ++cit)
        {
            std::cout << "\t\t" << channel.getName() << " | Client: " << cit->second->getName() << std::endl;
        }
    }
    std::cout <<"\033[0m\n\n";

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
    if (line.empty())
        return false;
    // cccccl(_allClients);
    // printListOfClients();

    std::vector<std::string> holder;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word)
        holder.push_back(word);
    if (holder[0] == "PASS")
        pass(client, line);
    else if (holder[0] == "NICK")
        nick(client, line, _allClients);
    else if (holder[0] == "USER")
        user(client, line);
    else if (client.getAuth())
    {
        if (holder[0] == "JOIN")
            join(client, line);
        else if (holder[0] == "MODE")
            mode(client, line);
        else if (holder[0] == "KICK")
            kick(line, client);
        else if (holder[0] == "TOPIC")
            topic(line, client);
        else if (holder[0] == "PRIVMSG")
            prvmsg(line);
        else if (holder[0] == "INVITE")
            invite(line, client);
        else if (holder[0] == "GET" || holder[0] == "DONE")
            getfile(client, line, _allClients);
        else if (holder[0] == "SEND")
            sendfile(client, line, _allClients);
        else if (holder[0] == "BOOT")
            boot(client, line);
        else
        {
            //:<server> 421 <nick> <command> :Unknown command
            std::string msg =  ":ircserv 421 " + client.getName() + " " + holder[0] + " :Unknown command\r\n";
            client.sendMsg(msg);
        }
    }
    else
        client.sendMsg(":You have not registered\r\n");
    if (!client.getAuth() && client.getPass() && !client.getNick().empty() && !client.getName().empty())
    {
        client.setAuth();
        client.sendMsg("wellcome to server\r\n");
    }
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