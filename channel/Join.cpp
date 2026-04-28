#include "parsing.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

static std::string buildClientPrefix(const Client &client)
{
    std::string nick = client.getNick();
    std::string user = client.getName();

    if (nick.empty())
        nick = user;
    if (nick.empty())
        nick = "unknown";
    if (user.empty())
        user = nick;

    return ":" + nick + "!" + user + "@localhost";
}

static std::string buildJoinMsg(const Client &client, const std::string &channelName)
{
    return buildClientPrefix(client) + " JOIN " + channelName + "\r\n";
}


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
    std::string token;
    std::string::size_type start = 0;
    std::string::size_type pos;

    while ((pos = line.find(del, start)) != std::string::npos)
    {
        std::string token = line.substr(start, pos - start);
        // Trim whitespace from token
        size_t token_start = token.find_first_not_of(" \t\n\r");
        if (token_start != std::string::npos) {
            size_t token_end = token.find_last_not_of(" \t\n\r");
            token = token.substr(token_start, token_end - token_start + 1);
        } else {
            token = ""; // Token is all whitespace
        }
        result.push_back(token);
        start = pos + 1;
    }
    
    // Handle last token
    std::string last_token = line.substr(start);
    size_t token_start = last_token.find_first_not_of(" \t\n\r");
    if (token_start != std::string::npos) {
        size_t token_end = last_token.find_last_not_of(" \t\n\r");
        last_token = last_token.substr(token_start, token_end - token_start + 1);
    } else {
        last_token = ""; // Token is all whitespace
    }
    result.push_back(last_token);

    return result;
}

std::map<std::string, std::string> key_name(std::vector<std::string> line)
{
    std::vector<std::string> names;
    std::vector<std::string> keys;
    std::map<std::string, std::string> holder;
    if (line.size() > 1 && !line[1].empty()) {
        names = Parsing::HelperSplit(line[1], ',');
    }
    if (line.size() > 2 && !line[2].empty()) {
        keys = Parsing::HelperSplit(line[2], ',');
    }

    for (size_t i = 0; i < names.size(); ++i) {
        if (i < keys.size() && !keys[i].empty())
            holder.insert(std::make_pair(names[i], keys[i]));
        else
            holder.insert(std::make_pair(names[i], ""));
    }
    return holder;
}

bool Parsing::canJoin(const Channel& channel, Client& client)
{
    // check if the channel is invite only
    if (channel.isInviteOnly() && !channel.isInvited(client))
    {
        std::string msg = "ircserv 473:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + " :Cannot join channel (+i)\r\n";
        client.sendMsg(msg);
        return false;
    }
    // check if the channel has a user limit and if it's reached
    // ERR_CHANNELISFULL (471)  "<client> <channel> :Cannot join channel (+l)"
    if (channel.hasUserLimit() && channel.getMembers().size() >= channel.getUserLimit())
        {
            std::string msg = "ircserv 471:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + " :Cannot join channel (+l)\r\n";
            client.sendMsg(msg);
            return false;
        }
    return true;
}

bool validName(std::string name, Client *client)
{
    
    // ERR_BADCHANMASK (476)  "<client> <channel> :Bad Channel Mask"
    if (name.empty() || name.size() < 2){std::string msg = "ircserv 476:" + client->getNick() + "!" + client->getName() + "@" + Parsing::_gethostname() + " " + " :Bad Channel Mask\r\n";client->sendMsg(msg);return false;}
    if (name.length() > 50){std::string msg = "ircserv 476:" + client->getNick() + "!" + client->getName() + "@" + Parsing::_gethostname() + " " + " :Bad Channel Mask\r\n";client->sendMsg(msg);return false;}
    if (name[0] != '#' && name[0] != '&'&& name[0] != '!'&& name[0] != '+'){std::string msg = "ircserv 476:" + client->getNick() + "!" + client->getName() + "@" + Parsing::_gethostname() + " " + " :Bad Channel Mask\r\n";client->sendMsg(msg);return false;}
    for (size_t i = 0; i < name.length(); ++i)
    {
        if (!std::isalnum(name[i]) && name[i] != '-' && name[i] != '_' && name[i] != '#')
        {std::string msg = "ircserv 476:" + client->getNick() + "!" + client->getName() + "@" + Parsing::_gethostname() + " " + " :Bad Channel Mask\r\n";client->sendMsg(msg);return false;}
    }
    return true;
}

void printTopic(const Channel& channel, Client *client)
{
    // RPL_TOPIC (332)  "<client> <channel> :<topic>"
    if (channel.getTopic().empty())
    {
        std::string msg = "ircserv 332:" + client->getNick() + "!" + client->getName() + "@" + Parsing::_gethostname() + " " + " :No topic is set\r\n";
        client->sendMsg(msg);
    }
    else
    {
        std::string msg = "ircserv 332:" + client->getNick() + "!" + client->getName() + "@" + Parsing::_gethostname() + " " + " :" + channel.getTopic() + "\r\n";
        client->sendMsg(msg);
        // RPL_TOPICWHOTIME (333)  "<client> <channel> <nick> <setat>"
        std::string msg = "ircserv 333:" + client->getNick() + "!" + client->getName() + "@" + Parsing::_gethostname() + " " + channel.getName() + " " + channel.getTopic() + " " + channel.getTopicSetTime() + "\r\n";
        client->sendMsg(msg);
    }
}
bool checkBan(const Channel& channel, Client& client)
{
    // Check if the client is banned from the channel
    if (channel.isBanned(client))
    {
        std::string msg = "ircserv 467:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " :You are banned from this channel\r\n";
        client.sendMsg(msg);
        return true;
    }
    return false;
}


void Parsing::join(Client &client, std::string line)
{
    std::map<std::string, Channel>& chs = Getchannel();

    std::vector<std::string> parsed = parceCammandJoin(line);
    if (parsed.size() < 2)
        {std::string msg = "ircserv 461:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " :Not enough parameters\r\n";client.sendMsg(msg);return;}
    std::cout << "Parsed JOIN command: "    << "Command: " << parsed[0] << ", Channels: " << parsed[1] << ", Keys: " << (parsed.size() > 2 ? parsed[2] : "None") << "\n";    
    if (parsed[1]  == "0" && parsed.size() == 2)
    {
        std::map<std::string, Channel>::iterator it;
        for (it = chs.begin(); it != chs.end(); ++it)
        {
            Channel& channel = it->second;
            if (channel.hasClient(&client))      
            {
                channel.removeClient(&client);
                std::string msg = "ircserv 331:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " PART " + channel.getName() + "\r\n";
                client.sendMsg(msg);
            }
        }
        return;
    }
    std::map<std::string, std::string> NamesKeys = key_name(parsed);
    for (std::map<std::string, std::string>::iterator it = NamesKeys.begin(); it != NamesKeys.end(); ++it)
    {
        const std::string& channelName = it->first;
        const std::string& key = it->second;
        std::map<std::string, Channel>::iterator chIt = chs.find(channelName);
        if (chIt == chs.end())
        {
            // Channel does not exist, create it
            if(!validName(channelName, &client)) return;
            Channel newChannel(channelName);
            if (!key.empty())
                newChannel.setKey(key);
            newChannel.addClient(&client);
            newChannel.addOperator(&client);
            add_Channel(newChannel);
            std::string msg = "ircserv 332:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + newChannel.getName() + " :" + newChannel.getTopic() + "\r\n";
            client.sendMsg(msg);
            if (!newChannel.getTopic().empty())
            {
                printTopic(newChannel, &client);
                //RPL_TOPICWHOTIME (333)  "<client> <channel> <nick> <setat>"
            }
        }
        else
        {
            Channel& channel = chIt->second;
            if (!canJoin(channel, client)) return;
            if (client.numberOfChannels() >= 10)
            {
                std::string msg = "ircserv 405:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " :You have joined too many channels\r\n";
                client.sendMsg(msg);
                return;
            }
            if (channel.hasKey())
            {
                if (!key.empty())
                {
                    if (channel.getKey() == key)
                    {
                        if (checkBan(channel, client)) return;
                        channel.addClient(&client);
                            channel.removeInvited(&client);
                        std::string msg = buildJoinMsg(client, channel.getName());
                        client.sendMsg(msg);
                        if (!channel.getTopic().empty())
                        {    
                            printTopic(channel, &client);
                            //RPL_TOPICWHOTIME (333)  "<client> <channel> <nick> <setat>"
                            std::stringstream ss;
                            ss << channel.getTopicSetTime();
                            std::string topicSetTime = ss.str();
                            std::string msg = "ircserv 333:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + channel.getName() + " " + channel.getTopicOwner() + " " + topicSetTime + "\r\n";
                            client.sendMsg(msg);
                        }
                        // // RPL_NAMREPLY (353)  "<client> = <channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
                        // std::string namesList;
                        // std::set<Client*> members = channel.getMembers();
                        // for (std::set<Client*>::iterator it = members.begin(); it != members.end(); ++it)
                        // {   
                        //     if (channel.isOperator(**it))
                        //         namesList += "@" + (*it)->getNick() + " ";
                        //     else                                
                        //         namesList += (*it)->getNick() + " ";
                        // }
                        // std::string msg = client.getName() + " = " + channel.getName() + " :" + namesList + "\n";
                        // client.sendMsg(msg);
                        // Servers MAY restrict the number of channels a client 
                        // may be joined to at one time. This limit SHOULD be defined in the
                        // CHANLIMIT RPL_ISUPPORT parameter. If the client cannot join this 
                        // channel because they would be over their limit, they will receive 
                        // an ERR_TOOMANYCHANNELS (405) reply and the command will fail.
                    }
                    else
                    {
                        std::cout << "Incorrect key for channel: " << channelName << " with key: " << key << " this is the correct key: " << channel.getKey() << "\n";
                        std::string msg = "ircserv 475:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " :Incorrect key\r\n";
                        client.sendMsg(msg);
                    }
                }
                else
                {
                    // ERR_CANNOTJOINCHANNEL (475)   "<client> <channel> :Cannot join channel (+k)"
                    std::string msg = "ircserv 475:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " :Cannot join channel (+k)\r\n";
                    client.sendMsg(msg);
                }
            }
            else 
            {
                channel.addClient(&client);
                channel.removeInvited(&client);
                std::string msg = buildJoinMsg(client, channel.getName());
                client.sendMsg(msg);
                if (!channel.getTopic().empty())
                {    
                    printTopic(channel, &client);
                    std::stringstream ss;
                    ss << channel.getTopicSetTime();
                    std::string topicSetTime = ss.str();
                    //RPL_TOPICWHOTIME (333)  "<client> <channel> <nick> <setat>"
                    std::string msg = "ircserv 333:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + channel.getName() + " " + channel.getTopicOwner() + " " + topicSetTime + "\r\n";
                    client.sendMsg(msg);
                }
            }
        }
    }
}
