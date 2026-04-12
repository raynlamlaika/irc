#include "parsing.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>


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
    if (channel.isInviteOnly())
        return false; 
    // check if the channel has a user limit and if it's reached
    // ERR_CHANNELISFULL (471)  "<client> <channel> :Cannot join channel (+l)"
    if (channel.hasUserLimit() && channel.getMembers().size() >= channel.hasUserLimit())
        {std::string msg = client.getName() + " " + channel.getName() + " :Cannot join channel (+l)\n";client.sendMsg(msg);return false;}
    return true;
}

static bool validName(std::string name, Client *client)
{
    std::cout << "\033[0;31m TEST JOIN COMMAND  \033[0m\n";
    
    // ERR_BADCHANMASK (476)  "<client> <channel> :Bad Channel Mask"
    if (name.empty()){std::string msg = client->getName() + " " + name + " :Bad Channel Mask\n";client->sendMsg(msg);return false;}
    if (name.length() > 50){std::string msg = client->getName() + " " + name +" :Bad Channel Mask\n";client->sendMsg(msg);return false;}
    if (name[0] != '#' && name[0] != '&'&& name[0] != '!'&& name[0] != '+'){std::string msg = client->getName() + " " + name +" :Bad Channel Mask\n";client->sendMsg(msg);return false;}
    for (size_t i = 0; i < name.length(); ++i)
    {
        if (!std::isalnum(name[i]) && name[i] != '-' && name[i] != '_' && name[i] != '#')
        {std::string msg = client->getName() +  " " + name + " :Bad Channel Mask\n";client->sendMsg(msg);return false;}
    }
    return true;
}

void printTopic(const Channel& channel, Client *client)
{
    // RPL_TOPIC (332)  "<client> <channel> :<topic>"
    if (channel.getTopic().empty())
    {
        std::string msg = client->getName() + " " + channel.getName() + " :No topic is set\n";
        client->sendMsg(msg);
    }
    else
    {
        std::string msg = client->getName() + " " + channel.getName() + " :" + channel.getTopic() + "\n";
        client->sendMsg(msg);
        // RPL_TOPICWHOTIME (333)  "<client> <channel> <nick> <setat>"
        // std::string msg = client->getName() + " " + channel.getName() + " " + channel.getTopic() + " " + "std::to_string(channel.getTopicSetTime()) "+ "\n";
    }
}
bool checkBan(const Channel& channel, Client& client)
{
    // Check if the client is banned from the channel
    if (channel.isBanned(client))
    {
        std::string msg = client.getName() + " :You are banned from this channel\n";
        client.sendMsg(msg);
        return true; // Return true to indicate the client is banned
    }
    return false; // Return false if the client is not banned
}

void Parsing::join(Client &client, std::string line)
{
    std::map<std::string, Channel>& chs = Getchannel();

    std::vector<std::string> parsed = parceCammandJoin(line);
    if (parsed.size() < 2)
        {std::string msg = client.getName() + " JOIN :Not enough parameters\n";client.sendMsg(msg);return;}
    
    std::map<std::string, std::string> NamesKeys = key_name(parsed);
    for (std::map<std::string, std::string>::iterator it = NamesKeys.begin(); it != NamesKeys.end(); ++it) {
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
            // add channel operator (make first joiner operator)
            newChannel.addOperator(&client);
            add_Channel(newChannel);
            std::string msg = client.getName() + " JOIN " + newChannel.getName() + "\n";
            client.sendMsg(msg);
            // printTopic(newChannel, &client);
        }
        else
        {
            Channel& channel = chIt->second;
            // pass the check of the channel: now check for requierment of the channel
            if (!canJoin(channel, client)) return;
            if (client.numberOfChannels() >= 10)
            {
                std::string msg = client.getName() + " :You have joined too many channels\n";
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
                        channel.addClient(&client);//printTopic(channel, &client);
                        std::string msg = client.getName() + " JOIN " + channel.getName() + "\n";
                        client.sendMsg(msg);
                    }
                    else
                    {
                        std::cout << "Incorrect key for channel: " << channelName << " with key: " << key << " this is the correct key: " << channel.getKey() << "\n";
                        std::string msg = client.getName() + " " + channelName + " :Incorrect key\n";
                        client.sendMsg(msg);
                    }
                }
                else
                {
                    // ERR_CANNOTJOINCHANNEL (475)   "<client> <channel> :Cannot join channel (+k)"
                    std::string msg = client.getName() + " " + channelName + " :Cannot join channel (+k)\n";
                    client.sendMsg(msg);
                }
            }
            else 
            {
                channel.addClient(&client);
                std::string msg = client.getName() + " JOIN " + channel.getName() + "\n";
                client.sendMsg(msg);//printTopic(channel, &client);}
            }
        }
    }

    // std::cout << "\t\t\t------this is all of the channels------\t\t\t\n \033[0;32m";
    // for (auto it1 = chs.begin(); it1 != chs.end(); ++it1) {
    //     std::cout << "Channel : " << it1->first << ": " << it1->second.getName() << std::endl;
    // }
    // std::cout << "\033[0m";
}



// RPL_NAMREPLY (353)  "<client> = <channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
