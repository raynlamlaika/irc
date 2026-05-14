#include "replices.hpp"
#include "parsing.hpp"

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

time_t Channel::getTopicSetTime() const
{
    return this->_topicSetTime;
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

Client *Parsing::searchForClientref(std::string Nick , std::map<int, Client*> _allClients)
{
    std::map<std::string, Channel> chns = Getchannel();
    std::map<std::string, Channel>::iterator it;
    std::map<int, Client*>::iterator it2;

    for (it2 = _allClients.begin(); it2 != _allClients.end(); ++it2)
    {
        if (it2->second && it2->second->getNick() == Nick)
            return it2->second;
    }

    for(it = chns.begin();it != chns.end(); it++)
    {
        std::map<int, Client*> ClientList = it->second.getmembers();
        std::map<int, Client*>::iterator  Cit;
        for ( Cit =  ClientList.begin(); Cit !=  ClientList.end(); Cit++)
        {   
            if (Cit->second->getNick() == Nick)
                return Cit->second;
        }
    }
    return NULL;
}

Parsing::~Parsing(){}

void Parsing::topic(std::string line, Client& client)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');
    if (holder.size() < 2)
    {
        // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
        std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", client.getNick(), holder[0]);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    if (!searchForChannel(holder[1])) {
        // ERR_NOSUCHCHANNEL (403)  "<client> <channel> :No such channel"
        std::string msg = MSG_ERR_NOSUCHCHANNEL("ircserv", client.getNick(), holder[1]);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    Channel *channel = searchForChannelref(holder[1]);
    if (!channel->hasClient(&client)) {
        // ERR_NOTONCHANNEL (442) "<client> <channel> :You're not on that channel"
        std::string msg = MSG_ERR_NOTONCHANNEL("ircserv", client.getNick(), holder[1]);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    size_t index = line.find(':');
    if (index == std::string::npos) {
        if (channel->getTopic().empty())
        {
            std::string msg = MSG_RPL_NOTOPIC("ircserv", client.getNick(), holder[1]);
            client.appendSendBuffer(msg, _pollFds, 0);
        }
        else
        {
            std::string msg = MSG_RPL_TOPIC("ircserv", client.getNick(), holder[1], channel->getTopic());
            client.appendSendBuffer(msg, _pollFds, 0);
        }
        return;
    }
    if (!channel->isOperator(client))
    {
        std::string msg = MSG_ERR_CHANOPRIVSNEEDED("ircserv", client.getNick(), holder[1]);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    std::string topicUse = line.substr(index + 1);
    std::cout << "line is : " << line << std::endl;
    std::cout << "topicUse: " << topicUse << std::endl;
    if (topicUse.empty())
    {
        channel->setTopic("");
        std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", client.getNick(), holder[1]) + " :has removed the topic\r\n";
        channel->broadcastMsg(msg, channel->getMembers(), &client,_pollFds);
        return;
    }
    else
    {    
        channel->setTopic(topicUse);
        channel->setTopicOwner(client.getName());
        channel->setTopicSetTime(time(NULL));
        std::string msg = MSG_RPL_TOPIC("ircserv", client.getNick(), holder[1], topicUse);
        channel->broadcastMsg(msg, channel->getMembers(), &client,_pollFds);
        msg = MSG_RPL_TOPICWHOTIME("ircserv", client.getNick(), holder[1], channel->getTopicOwner(), channel->displayTimestamp());
        channel->broadcastMsg(msg, channel->getMembers(), &client, _pollFds);
    }
}

bool Parsing::newMessage(const std::string &line,
                         Client &client,
                         std::map<int, Client*> _allClients)
{
    if (line.empty())
        return false;
    std::vector<std::string> holder;
    std::stringstream ss(line);
    std::string word;

    while (ss >> word)
        holder.push_back(word);

    if (holder.empty())
        return false;

    const std::string &cmd = holder[0];

    if (cmd == "PASS")
    {
        pass(client, line);
    }
    else if (cmd == "NICK")
    {
        nick(client, line, _allClients);
    }
    else if (cmd == "USER")
    {
        user(client, line);
    }
    else if (cmd == "QUIT")
    {
        throw std::runtime_error("Client quit");
    }
    else if (client.getAuth())
    {
        if (cmd == "JOIN")
        {
            join(client, line);
        }
        else if (cmd == "MODE")
        {
            mode(client, line, _allClients);
        }
        else if (cmd == "KICK")
        {
            kick(line, client);
        }
        else if (cmd == "TOPIC")
        {
            topic(line, client);
        }
        else if (cmd == "PRIVMSG")
        {
            prvmsg(line, client, _allClients);
        }
        else if (cmd == "INVITE")
        {
            invite(line, client, _allClients);
        }
        else if (cmd == "BOOT")
        {
            boot(client, line);
        }
        else
        {
            std::string msg =
                MSG_ERR_UNKNOWNCOMMAND("ircserv",
                                       client.getNick(),
                                       cmd);

            client.appendSendBuffer(msg, _pollFds, 0);
        }
    }

    else
    {
        std::string msg =
            MSG_ERR_NOTREGISTERED("ircserv",
                                  client.getNick());

        client.appendSendBuffer(msg, _pollFds, 0);
    }
    if (!client.getAuth()
        && client.getPass()
        && !client.getNick().empty()
        && !client.getName().empty())
    {
        client.setAuth();
        sendWelcome(client);
    }

    return true;
}

void Parsing::sendWelcome(Client& client)
{
    std::string server = "ircserv";
    std::string nick = client.getNick();
    std::string user = client.getName();
    std::string host = _gethostname();

    client.appendSendBuffer(":ircserv 001 " + nick +
        " :Welcome to the IRC Network " + nick +
        "!" + user + "@"+ host +"\r\n", _pollFds, 0);

    client.appendSendBuffer(":ircserv 002 " + nick +
        " :Your host is ircserv, running version 1.0\r\n", _pollFds, 0);

    client.appendSendBuffer(":ircserv 003 " + nick +
        " :This server was created today\r\n", _pollFds, 0);

    client.appendSendBuffer(":ircserv 004 " + nick +
        " ircserv 1.0 o o\r\n", _pollFds, 0);
}

