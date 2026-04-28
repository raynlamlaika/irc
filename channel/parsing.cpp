#include "parsing.hpp"
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>



std::string Channel::getTopicOwner() const
{
    return this->_topicOwner;
}

std::string	Channel::displayTimestamp()
{
	time_t rawtime;
    time(&rawtime);
    struct tm *timeinfo;
    timeinfo = localtime(&rawtime);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", timeinfo);
	return buffer;
}

// time_t Channel::getTopicSetTime() const
// {
//     return this->_topicSetTime;
// }

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

Client *Parsing::searchForClientref(std::string name , std::map<int, Client*> _allClients)
{
    std::map<std::string, Channel> chns = Getchannel();
    std::map<std::string, Channel>::iterator it;
    std::map<int, Client*>::iterator it2;

    for (it2 = _allClients.begin(); it2 != _allClients.end(); ++it2)
    {
        if (it2->second && it2->second->getName() == name)
            return it2->second;
    }

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
        std::string msg = "ircserv 461:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " :Not enough parameters\r\n";
        client.sendMsg(msg);
        return;
    }
    if (!searchForChannel(holder[1])) {
        // ERR_NOSUCHCHANNEL (403)  "<client> <channel> :No such channel"
        std::string msg = "ircserv 403:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + holder[1] + " :No such channel\r\n";
        client.sendMsg(msg);
        return;
    }
    Channel *channel = searchForChannelref(holder[1]);
    if (!channel->hasClient(&client)) {
        // ERR_NOTONCHANNEL (442) "<client> <channel> :You're not on that channel"
        std::string msg = "ircserv 442:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + holder[1] + " :You're not on that channel\r\n";
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
            std::string msg = "ircserv 331:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + holder[1] + " :No topic is set\r\n";
            client.sendMsg(msg);
        }
        else
        {
            std::string msg = "ircserv 332:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + holder[1] + " :" + channel->getTopic() + "\r\n";
            client.sendMsg(msg);
        }
        return;
    }
    if (!channel->isOperator(client))
    {
        // ERR_CHANOPRIVSNEEDED (482) "<client> <channel> :You're not channel operator"
        std::string msg = "ircserv 482:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + holder[1] + " :You're not channel operator\r\n";
        client.sendMsg(msg);
        return;
    }
    std::string topicUse = line.substr(index + 1); // skip the ':'
    std::cout << "line is : " << line << std::endl;
    std::cout << "topicUse: " << topicUse << std::endl;
    if (topicUse.empty())
    {
        // RPL_NOTOPIC (331)   "<client> <channel> :No topic is set"
        // std::string msg = client.getName() + " " + holder[1] + " :No topic is set\n";
        // client.sendMsg(msg);
        channel->setTopic("");
         // breadcast for all of the channel members
        std::string msg = "ircserv 461:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + holder[1] + " :has removed the topic\r\n";
        channel->broadcastMsg(msg, channel->getMembers());
        return;
    }
    else
    {    
        channel->setTopic(topicUse);
        channel->setTopicOwner(client.getName());
        channel->setTopicSetTime(time(NULL));
        // breadcast for all of the channel members
        std::string msg = "ircserv 461:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + holder[1] + " :has changed the topic to: " + topicUse + "\r\n";
        channel->broadcastMsg(msg, channel->getMembers());
    }
}

bool Parsing::newMessage(const std::string &line, Client &client, std::map<int, Client*> _allClients)
{
    if (line.empty())
        return false;

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
            mode(client, line,_allClients);
        else if (holder[0] == "KICK")
            kick(line, client);
        else if (holder[0] == "TOPIC")
            topic(line, client);
        else if (holder[0] == "PRIVMSG")
            prvmsg(line, client,_allClients);
        else if (holder[0] == "INVITE")
            invite(line, client,_allClients);
        else if (holder[0] == "GET" || holder[0] == "DONE")
            getfile(client, line, _allClients);
        else if (holder[0] == "SEND")
            sendfile(client, line, _allClients);
        else if (holder[0] == "BOOT")
            boot(client, line);
        else
        {
            //:<server> 421 <nick> <command> :Unknown command
            std::string msg =  "ircserv 421:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " :Unknown command\r\n";
            client.sendMsg(msg);
        }
    }
    else
    { 
        std::string msg = "ircserv 421:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " :You have not registered\r\n";
        client.sendMsg(msg);
    }
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