#include "parsing.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>

// static std::string buildClientPrefix(const Client &client)
// {
//     std::string nick = client.getNick();
//     std::string user = client.getName();

//     if (nick.empty())
//         nick = user;
//     if (nick.empty())
//         nick = "unknown";
//     if (user.empty())
//         user = nick;

//     return ":" + nick + "!" + user + "@localhost";
// }

// static std::string buildJoinMsg(const Client &client, const std::string &channelName)
// {
//     return buildClientPrefix(client) + " JOIN " + channelName + "\r\n";
// }


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
        std::string msg = MSG_ERR_INVITEONLYCHAN("ircserv", client.getNick(), channel.getName());
        client.sendMsg(msg);
        return false;
    }
    // check if the channel has a user limit and if it's reached
    // ERR_CHANNELISFULL (471)  "<client> <channel> :Cannot join channel (+l)"
    if (channel.hasUserLimit() && channel.getMembers().size() >= channel.getUserLimit())
        {
            std::string msg = MSG_ERR_CHANNELISFULL("ircserv", client.getNick(), channel.getName());
            client.sendMsg(msg);
            return false;
        }
    return true;
}

bool validName(std::string name, Client *client)
{
    
    // ERR_BADCHANMASK (476)  "<client> <channel> :Bad Channel Mask"
    if (name.empty()){std::string msg = MSG_ERR_BADCHANMASK("ircserv", client->getNick(), name);client->sendMsg(msg);return false;}
    if (name.length() > 50){std::string msg = MSG_ERR_BADCHANMASK("ircserv", client->getNick(), name);client->sendMsg(msg);return false;}
    if (name[0] != '#' && name[0] != '&' && name[0] != '!'&& name[0] != '+'){std::string msg = MSG_ERR_BADCHANMASK("ircserv", client->getNick(), name);client->sendMsg(msg);return false;}
    for (size_t i = 0; i < name.length(); ++i)
    {
        if (!std::isalnum(name[i]) && name[i] != '-' && name[i] != '_' && name[i] != '#')
        {std::string msg = MSG_ERR_BADCHANMASK("ircserv", client->getNick(), name);client->sendMsg(msg);return false;}
    }
    return true;
}



void printTopic(const Channel& channel, Client *client)
{
    std::string prefix = ":ircserv ";

    // 332
    if (channel.getTopic().empty())
    {
        // std::string msg = MSG_RPL_TOPICWHOTIME("ircserv", client->getNick(), channel.getName(), channel.getTopicOwner(), std::to_string(channel.getTopicSetTime()));

        std::string msg = MSG_RPL_TOPIC("ircserv", client->getNick(), channel.getName(), "No topic is set");
        client->sendMsg(msg);
    }
    else
    {
        // std::string msg = MSG_RPL_TOPICWHOTIME("ircserv", client->getNick(), channel.getName(), channel.getTopicOwner(), std::to_string(channel.getTopicSetTime()));

        std::string msg = MSG_RPL_TOPIC("ircserv", client->getNick(), channel.getName(), channel.getTopic());
        client->sendMsg(msg);

        std::stringstream ss;
        ss << channel.getTopicSetTime();

        std::string msg2 = MSG_RPL_TOPICWHOTIME("ircserv", client->getNick(), channel.getName(), channel.getTopicOwner(), ss.str());
        // std::string msg2 = prefix + "333 " + client->getNick() +
        //     " " + channel.getName() + " " +
        //     ss.str() + "\r\n";

        client->sendMsg(msg2);
    }
}

bool checkBan(const Channel& channel, Client& client)
{
    // Check if the client is banned from the channel
    if (channel.isBanned(client))
    {
        std::string msg = MSG_ERR_BANNEDFROMCHAN("ircserv", client.getNick(), channel.getName());
        client.sendMsg(msg);
        return true;
    }
    return false;
}


// std::string Parsing::printListOfUsers(std::map<int, Client*> clients)
// {
//     std::string namesList  ="{ ";
//     if(clients.size() == 0) return namesList;
//     for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
//     {   
//         Client* client = it->second;
//         if (client)
//         {
//             namesList += client->getNick() + " ";
//         }
//     }
//     namesList+= "}";
//     return namesList;
// }

std::string Parsing::printListOfUsers(std::map<int, Client*> clients)
{
    std::string namesList;

    if (clients.empty())
        return "";

    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
    {   
        Client* client = it->second;
        if (client)
        {
            namesList += client->getNick() + " ";
        }
    }
    return namesList;
}
void Parsing::join(Client &client, std::string line)
{
    std::map<std::string, Channel>& chs = Getchannel();

    std::vector<std::string> parsed = parceCammandJoin(line);
    if (parsed.size() < 2)
        {std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", client.getNick(), parsed[0]);client.sendMsg(msg);return;}
    // std::cout << "Parsed JOIN command: "    << "Command: " << parsed[0] << ", Channels: " << parsed[1] << ", Keys: " << (parsed.size() > 2 ? parsed[2] : "None") << "\n";
    if (parsed[1] == "0" && parsed.size() == 2)
    {
        std::map<std::string, Channel>::iterator it = chs.begin();
        
        while (it != chs.end())
        {
            Channel& channel = it->second;
            
            if (channel.hasClient(&client))      
            {
                channel.removeClient(&client);
                
                std::string msg = "ircserv 331:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " PART " + channel.getName() + "\r\n";
                //(nick, user, host, target, channel, reason)
                // std::string msg = MSG_KICK(this.getNick(), client.getName(), Parsing::_gethostname(), client.getName() ,channel.getName(), "Leaving channel\r\n");

                client.sendMsg(msg);
                if (channel.getMembers().empty())
                    chs.erase(it++); 
                else
                    ++it;
            }
            else
                ++it;
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
            // std::string msg = ":ircserv " + client.getNick()  + " :JOIN " + newChannel.getName() +"\r\n"; // fix this one
            std::string prefix = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname();
            std::string msg = prefix + " JOIN " + newChannel.getName() + "\r\n";
            client.sendMsg(msg);
            // newvchannel.broadcastMsg(msg, channel.getMembers());

            // list of users in here
            std::string names = printListOfUsers(newChannel.getMembers());
            if (names.empty())
                MSG_RPL_NAMREPLY("ircserv", client.getNick(), "=", newChannel.getName(), names);//client.sendMsg("ircserv 353:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " = " + newChannel.getName() + " :" + names + "\r\n");
            // std::string msg2 = "ircserv 332:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " :End of /NAMES list\r\n";
            client.sendMsg(MSG_RPL_ENDOFNAMES("ircserv", client.getNick(), newChannel.getName()));
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
                // std::string msg = "ircserv 405:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " :You have joined too many channels\r\n";
                client.sendMsg(MSG_ERR_TOOMANYCHANNELS("ircserv", client.getNick(), channel.getName()));
                return;
            }
            // check if invited the order get firsteven if the  channel +k +l
            if (channel.hasKey())
            {
                if (!key.empty())
                {
                    if (channel.getKey() == key)
                    {
                        if (checkBan(channel, client)) {client.sendMsg(MSG_ERR_BANNEDFROMCHAN("ircserv", client.getNick(), channel.getName()));return;};
                        channel.addClient(&client);
                        channel.removeInvited(&client);
                        // std::string msg = "ircserv: " + client.getNick()  + " :JOIN " + channel.getName() +"\r\n"; // fix this one
                        std::string prefix = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname();
                        std::string msg = prefix + " JOIN " + channelName + "\r\n";
                        client.sendMsg(msg);
                        // list of users in here
                        std::string names = printListOfUsers(channel.getMembers());
                        if (!names.empty())
                            client.sendMsg(MSG_RPL_NAMREPLY("ircserv", client.getNick(), "=", channel.getName(), names));
                        client.sendMsg(MSG_RPL_ENDOFNAMES("ircserv", client.getNick(), channel.getName()));
                        if (!channel.getTopic().empty())
                        {    
                            printTopic(channel, &client);
                            //RPL_TOPICWHOTIME (333)  "<client> <channel> <nick> <setat>"
                            std::stringstream ss;
                            ss << channel.getTopicSetTime();
                            std::string topicSetTime = ss.str();//// chheeekc
                            std::string msg = "ircserv 333:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + channel.getName() + " " + channel.getTopicOwner() + " " + topicSetTime + "\r\n";
                            client.sendMsg(msg);
                        }
                        
                        
                    }
                    else
                    {
                        // std::cout << "Incorrect key for channel: " << channelName << " with key: " << key << " this is the correct key: " << channel.getKey() << "\n";
                        std::string msg = MSG_ERR_BADCHANNELKEY("ircserv", client.getNick(), channel.getName());
                        client.sendMsg(msg);
                    }
                }
                else
                {
                    // ERR_CANNOTJOINCHANNEL (475)   "<client> <channel> :Cannot join channel (+k)"
                    std::string msg = MSG_ERR_BADCHANNELKEY("ircserv", client.getNick(), channel.getName());
                    client.sendMsg(msg);
                }
            }
            else 
            {
                channel.addClient(&client);
                std::string names = printListOfUsers(channel.getMembers());
                if (!names.empty())
                    client.sendMsg(MSG_RPL_NAMREPLY("ircserv", client.getNick(), "=", channel.getName(), names)); //client.sendMsg("ircserv 353:" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " = " + channel.getName() + " :" + names + "\r\n");
                std::string msg2 = MSG_RPL_ENDOFNAMES("ircserv", client.getNick(), channel.getName());
                channel.removeInvited(&client);
                // std::string msg = buildJoinMsg(client, channel.getName());
                //  std::string msg = ":ircserv " + client.getNick()  + " :JOIN " + channel.getName() +"\r\n"; // fix this one
                std::string prefix = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname();
                std::string msg = prefix + " JOIN " + channelName + "\r\n";
                // client.sendMsg(msg);
                channel.broadcastMsg(msg, channel.getMembers());
                if (!channel.getTopic().empty())
                {    
                    printTopic(channel, &client);
                    std::stringstream ss;
                    ss << channel.getTopicSetTime();
                    std::string topicSetTime = ss.str();
                    //RPL_TOPICWHOTIME (333)  "<client> <channel> <nick> <setat>"
                    client.sendMsg(MSG_RPL_TOPICWHOTIME("ircserv", client.getNick(), channel.getName(), channel.getTopicOwner(), topicSetTime));
                }
            }
        }
    }
}
