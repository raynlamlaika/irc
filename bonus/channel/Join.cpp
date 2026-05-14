#include "parsing.hpp"


std::map<std::string, Channel>& Parsing::Getchannel()
{
    return _channels;
}

void Parsing::add_Channel(const Channel& channel)
{
    const std::string& name = channel.getName();
    
    if (_channels.find(name) != _channels.end())
        return;
    _channels.insert(std::make_pair(name, channel));
}

std::vector<std::string> parceCammandJoin(std::string line)
{
    std::vector<std::string> holder;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word)
    {
        holder.push_back(word);
    }
    if (holder.size() >= 1)
    {
        std::string command = holder[0];
        if (command  != "JOIN")
        {
            std::cout  << "not the join cammand\n";
            return holder;
        }
    }
    if (holder.size() >= 2)
        std::string names = holder[1];
    if (holder.size() >= 3)
        std::string names = holder[2];
    return holder;
}

std::vector<std::string> Parsing::HelperSplit(std::string line, char del)
{
    std::vector<std::string> result;

    if (line.find_first_not_of(" \t\r\n") == std::string::npos)
        return result;

    std::string::size_type start = 0;
    std::string::size_type pos;

    while ((pos = line.find(del, start)) != std::string::npos)
    {
        std::string token = line.substr(start, pos - start);

        size_t first = token.find_first_not_of(" \t\r\n");
        if (first != std::string::npos)
        {
            size_t last = token.find_last_not_of(" \t\r\n");
            result.push_back(token.substr(first, last - first + 1));
        }
        start = pos + 1;
    }
    std::string token = line.substr(start);
    size_t first = token.find_first_not_of(" \t\r\n");
    if (first != std::string::npos)
    {
        size_t last = token.find_last_not_of(" \t\r\n");
        result.push_back(token.substr(first, last - first + 1));
    }

    return result;
}

std::vector<std::pair<std::string, std::string> > key_name(std::vector<std::string> line)
{
    std::vector<std::string> names;
    std::vector<std::string> keys;
    std::vector<std::pair<std::string, std::string> > holder;

    if (line.size() > 1 && !line[1].empty())
        names = Parsing::HelperSplit(line[1], ',');

    if (line.size() > 2 && !line[2].empty())
        keys = Parsing::HelperSplit(line[2], ',');

    for (size_t i = 0; i < names.size(); ++i)
    {
        if (i < keys.size() && !keys[i].empty())
            holder.push_back(std::make_pair(names[i], keys[i]));
        else
            holder.push_back(std::make_pair(names[i], ""));
    }

    return holder;
}

bool Parsing::canJoin(const Channel& channel, Client& client)
{

    if (channel.isInviteOnly() && !channel.isInvited(client))
    {
        std::string msg = MSG_ERR_INVITEONLYCHAN("ircserv", client.getNick(), channel.getName());
        client.appendSendBuffer(msg, _pollFds, 0);
        return false;
    }
    // ERR_CHANNELISFULL (471)  "<client> <channel> :Cannot join channel (+l)"
    if (channel.hasUserLimit() && channel.getMembers().size() >= channel.getUserLimit())
        {
            std::string msg = MSG_ERR_CHANNELISFULL("ircserv", client.getNick(), channel.getName());
            client.appendSendBuffer(msg, _pollFds, 0);
            return false;
        }
    return true;
}

bool Parsing::validName(std::string name, Client *client)
{
    // ERR_BADCHANMASK (476)  "<client> <channel> :Bad Channel Mask"
    if (name.empty()){std::string msg = MSG_ERR_BADCHANMASK("ircserv", client->getNick(), name);client->appendSendBuffer(msg, _pollFds, 0);return false;}
    if (name.length() > 50){std::string msg = MSG_ERR_BADCHANMASK("ircserv", client->getNick(), name);client->appendSendBuffer(msg, _pollFds, 0);return false;}
    if (name[0] != '#' && name[0] != '&' && name[0] != '!'&& name[0] != '+'){std::string msg = MSG_ERR_BADCHANMASK("ircserv", client->getNick(), name);client->appendSendBuffer(msg, _pollFds, 0);return false;}
    for (size_t i = 0; i < name.length(); ++i)
    {
        if (!std::isalnum(name[i]) && name[i] != '-' && name[i] != '_' && name[i] != '#' && name[0] != '&' && name[0] != '!'&& name[0] != '+')
        {std::string msg = MSG_ERR_BADCHANMASK("ircserv", client->getNick(), name);client->appendSendBuffer(msg, _pollFds, 0);return false;}
    }
    return true;
}



void Parsing::printTopic(const Channel& channel, Client *client)
{
    std::string prefix = ":ircserv ";

    if (channel.getTopic().empty())
    {
        std::string msg = MSG_RPL_TOPIC("ircserv", client->getNick(), channel.getName(), "No topic is set");
        client->appendSendBuffer(msg, _pollFds, 0);
    }
    else
    {
        std::string msg = MSG_RPL_TOPIC("ircserv", client->getNick(), channel.getName(), channel.getTopic());
        client->appendSendBuffer(msg, _pollFds, 0);

        std::stringstream ss;
        ss << channel.getTopicSetTime();

        std::string msg2 = MSG_RPL_TOPICWHOTIME("ircserv", client->getNick(), channel.getName(), channel.getTopicOwner(), ss.str());
        client->appendSendBuffer(msg2, _pollFds, 0);
    }
}

bool Parsing::checkBan(const Channel& channel, Client& client)
{
    if (channel.isBanned(client))
    {
        std::string msg = MSG_ERR_BANNEDFROMCHAN("ircserv", client.getNick(), channel.getName());
        client.appendSendBuffer(msg, _pollFds, 0);
        return true;
    }
    return false;
}

std::string Parsing::printListOfUsers(std::map<int, Client*> clients, const Channel& channel)
{
    std::string namesList;

    for (std::map<int, Client*>::const_iterator it = clients.begin();
         it != clients.end(); ++it)
    {
        Client* client = it->second;
        if (!client)
            continue;

        std::string prefix = "";

        if (channel.isOperator(*client))
            prefix = "@";

        namesList += prefix + client->getNick() + " ";
    }

    if (!namesList.empty())
        namesList.erase(namesList.size() - 1);

    return namesList;
}

void Parsing::join(Client &client, std::string line)
{
    std::map<std::string, Channel>& chs = Getchannel();

    std::vector<std::string> parsed = parceCammandJoin(line);
    if (parsed.size() < 2)
        {std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", client.getNick(), parsed[0]);client.appendSendBuffer(msg, _pollFds, 0);return;}
    std::vector<std::pair<std::string, std::string> > NamesKeys = key_name(parsed);
    for (std::vector<std::pair<std::string, std::string> >::iterator it = NamesKeys.begin(); it != NamesKeys.end(); ++it)
    {
        const std::string& channelName = it->first;
        const std::string& key = it->second;
        std::map<std::string, Channel>::iterator chIt = chs.find(channelName);
        if (chIt != chs.end() && chIt->second.hasClient(&client))
        {
            std::string msg = "ircserv 443:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + channelName + " :is already on channel\r\n";
            client.appendSendBuffer(msg, _pollFds, 0);
            continue;
        }
        else if (channelName == "0")
        {
            std::map<std::string, Channel>::iterator it2 = chs.begin();
            while (it2 != chs.end())
            {
                Channel& channel = it2->second;
                if (channel.hasClient(&client))      
                {
                    channel.removeClient(&client);
                    std::string msg = "ircserv 331:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " PART " + channel.getName() + "\r\n";
                    client.appendSendBuffer(msg, _pollFds, 0);
                    if (channel.getMembers().empty())
                        chs.erase(it2++); 
                    else
                        ++it2;
                }
                else
                    ++it2;
            }
        }
        else if (chIt == chs.end())
        {
            if(!validName(channelName, &client)) continue;
            Channel newChannel(channelName);
            if (!key.empty())
                newChannel.setKey(key);
            newChannel.addClient(&client);
            newChannel.addOperator(&client);
            add_Channel(newChannel);
            std::string prefix = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname();
            std::string msg = prefix + " JOIN " + newChannel.getName() + "\r\n";
            client.appendSendBuffer(msg, _pollFds, 0);
            std::string names = printListOfUsers(newChannel.getMembers(), newChannel);
            std::string owner = MSG_RPL_NAMREPLY("ircserv", client.getNick(), "=", newChannel.getName(), names);
            client.appendSendBuffer(owner, _pollFds, 0);
            if (names.empty())
                MSG_RPL_NAMREPLY("ircserv", client.getNick(), "=", newChannel.getName(), names);
            client.appendSendBuffer(MSG_RPL_ENDOFNAMES("ircserv", client.getNick(), newChannel.getName()), _pollFds, 0);
            if (!newChannel.getTopic().empty())
                printTopic(newChannel, &client);
        }
        else
        {
            Channel& channel = chIt->second;
            if (!canJoin(channel, client)) continue;
            if (client.numberOfChannels() >= 10)
            {
                client.appendSendBuffer(MSG_ERR_TOOMANYCHANNELS("ircserv", client.getNick(), channel.getName()), _pollFds, 0);
                continue;
            }
            
            if (channel.hasKey())
            {
                if (!key.empty())
                {
                    if (channel.getKey() == key)
                    {
                        if (checkBan(channel, client)) {client.appendSendBuffer(MSG_ERR_BANNEDFROMCHAN("ircserv", client.getNick(), channel.getName()), _pollFds, 0);continue;};
                        channel.addClient(&client);
                        channel.removeInvited(&client);
                        std::string prefix = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname();
                        std::string msg = prefix + " JOIN " + channelName + "\r\n";
                        client.appendSendBuffer(msg, _pollFds, 0);
                        std::string names = printListOfUsers(channel.getMembers(), channel);
                        if (!names.empty())
                            client.appendSendBuffer(MSG_RPL_NAMREPLY("ircserv", client.getNick(), "=", channel.getName(), names), _pollFds, 0);
                        client.appendSendBuffer(MSG_RPL_ENDOFNAMES("ircserv", client.getNick(), channel.getName()), _pollFds, 0);
                        if (!channel.getTopic().empty())
                        {    
                            printTopic(channel, &client);
                            std::stringstream ss;
                            ss << channel.getTopicSetTime();
                            std::string topicSetTime = ss.str();
                            std::string msg = "ircserv 333:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + channel.getName() + " " + channel.getTopicOwner() + " " + topicSetTime + "\r\n";
                            client.appendSendBuffer(msg, _pollFds, 0);
                        }
                    }
                    else
                    {
                        std::string msg = MSG_ERR_BADCHANNELKEY("ircserv", client.getNick(), channel.getName());
                        client.appendSendBuffer(msg, _pollFds, 0);
                    }
                }
                else
                {
                    // ERR_CANNOTJOINCHANNEL (475)   "<client> <channel> :Cannot join channel (+k)"
                    std::string msg = MSG_ERR_BADCHANNELKEY("ircserv", client.getNick(), channel.getName());
                    client.appendSendBuffer(msg, _pollFds, 0);
                }
            }
            else 
            {
                channel.addClient(&client);
                std::string prefix = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname();
                std::string msg = prefix + " JOIN " + channelName + "\r\n";
                if (!channel.getTopic().empty())
                    printTopic(channel, &client);
                client.appendSendBuffer(msg, _pollFds, 0);
                std::string names = printListOfUsers(channel.getMembers(), channel);
                if (!names.empty())
                    client.appendSendBuffer(MSG_RPL_NAMREPLY("ircserv", client.getNick(), "=", channel.getName(), names), _pollFds, 0);
                std::string msg2 = MSG_RPL_ENDOFNAMES("ircserv", client.getNick(), channel.getName());
                client.appendSendBuffer(msg2, _pollFds, 0);
                channel.removeInvited(&client);
                channel.broadcastMsg(msg, channel.getMembers(), &client, _pollFds );
                
            }
        }
    }
}
