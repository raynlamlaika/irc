#include "parsing.hpp"

bool Parsing::searchForChannel(std::string channelName)
{
    std::map<std::string, Channel>& chs = Getchannel();
    std::map<std::string, Channel>::iterator it;
    for (it = chs.begin() ; it != chs.end() ; it++)
    {
        if (it->first == channelName)
            return true;
    }
    return false;
}


std::string Parsing::_gethostname()
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) return std::string(hostname);
    else return "localhost";
}
    
void Parsing::kick(std::string line, Client& client)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');
    if (holder.size() < 3){
        // ERR_NEEDMOREPARAMS (461) "<client> <command> :Not enough parameters"
        std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", client.getNick(), holder[0]);
        client.appendSendBuffer(msg, _pollFds, 0);
        return ;
    }

    std::string channelname = holder[1];
    if (!validName(channelname, &client))
        return;
    std::string usertarget = holder[2];
    std::vector<std::string> usersTargeted = HelperSplit(holder[2], ',');
    if (holder[0] != "KICK")
        return;

    Channel* channel = searchForChannelref(channelname);
    if (!channel)
    {
        // ERR_NOSUCHCHANNEL (403)  "<client> <channel> :No such channel"
        
        std::string msg = MSG_ERR_NOSUCHCHANNEL("ircserv", client.getNick(), channelname);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    if (!channel->isOperator(client))
    {
        // ERR_CHANOPRIVSNEEDED (482)  "<client> <channel> :You're not channel operator"
        std::string msg = MSG_ERR_CHANOPRIVSNEEDED("ircserv", client.getNick(), channelname);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    // check client is part of the channel
    if (!channel->hasClient(&client)) {
        // ERR_USERNOTINCHANNEL (441) "<client> <nick> <channel> :They aren't on that channel"
        std::string msg = MSG_ERR_USERNOTINCHANNEL("ircserv", client.getNick(), usertarget, channelname);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }

    Client* targetClient = NULL;
    std::map<int, Client*> members = channel->getmembers();

    for (size_t y = 0 ; usersTargeted.size() >  y; y++)
    {
        usertarget = usersTargeted[y];
        for (std::map<int, Client *>::iterator it = members.begin(); it != members.end(); ++it)
        {
            if (it->second && it->second->getNick() == usertarget)
            {
                targetClient = it->second;
                std::string reason = "";
                if (holder.size() > 3 && holder[3][0] == ':') {
                    size_t index = line.find(":");
                    if (index != std::string::npos)
                        reason = line.substr(index + 1);
                }
                //check
                channel->removeClient(targetClient);
                
                // if (channel->getMembers().empty())
                //     chs.erase(it++); ALLO AKLLO ALALAO
                // else
                //     ++it;
                
                if (!reason.empty())
                {
                    std::string msg = MSG_KICK(client.getNick(), client.getName(), Parsing::_gethostname(), targetClient->getNick(), channelname, reason);
                    channel->broadcastMsg(msg, channel->getMembers(),&client,_pollFds);
                    client.appendSendBuffer(msg, _pollFds, 0);
                }
                else
                {
                    // (nick, user, host, target, channel)
                    std::string msg = MSG_KICK_WITHOUT_REASON(client.getNick(), client.getName(), Parsing::_gethostname(), targetClient->getNick(), channelname);
                    client.appendSendBuffer(msg, _pollFds, 0);
                    channel->broadcastMsg(msg, channel->getMembers(),&client, _pollFds);
                }
                break;
            }
        }
        if (!targetClient)
        {
            // ERR_USERNOTINCHANNEL (441) "<client> <nick> <channel> :They aren't on that channel"
            std::string msg = MSG_ERR_USERNOTINCHANNEL("ircserv", client.getNick(), usertarget, channelname);
            client.appendSendBuffer(msg, _pollFds, 0);
            // return;
        }
    }
}
