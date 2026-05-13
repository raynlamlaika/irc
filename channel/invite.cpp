#include "parsing.hpp"

void Parsing::sendInviteList(Parsing& parsing, Client& client)
{
    std::map<std::string, Channel>& channels = parsing.Getchannel();
    for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->second.isInvited(client))
        {
            // RPL_INVITELIST (336)  "<client> <channel>"
            // std::string msg = "ircserv 336: " + client.getName() + " " + it->second.getName() + "\r\n";
            //(server, nick, target, channel)
            std::string msg = MSG_RPL_INVITING("ircserv", client.getNick(), client.getName(), it->second.getName());
            client.appendSendBuffer(msg, _pollFds, 0);
        }
    }
}


void Parsing::invite(std::string line, Client& client, std::map<int, Client*> _allClients)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');


    if (holder.size() < 3)
    {
        //ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
        std::string msg =  MSG_ERR_NEEDMOREPARAMS("ircserv", client.getNick(), holder[0]);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    
    std::string target = holder[1];
    std::string channelName = holder[2];

   
    if (!searchForChannel(channelName))
    {
        //ERR_NOSUCHCHANNEL (403)  "<client> <channel> :No such channel"
        std::string msg = MSG_ERR_NOSUCHCHANNEL("ircserv", client.getNick(), channelName);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    Channel *channel = searchForChannelref(channelName);
    if (!channel->hasClient(&client))
    {
        //ERR_NOTONCHANNEL (442)  "<client> <channel> :You're not on that channel"
        std::string msg = MSG_ERR_NOTONCHANNEL("ircserv", client.getNick(), channelName);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    if (!channel->isOperator(client) && channel->getInviteOnly())
    {
        //ERR_CHANOPRIVSNEEDED (482)  "<client> <channel> :You're not channel operator"
        std::string msg = MSG_ERR_CHANOPRIVSNEEDED("ircserv", client.getNick(), channelName);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }

    if (!searchForClient(target, _allClients))
    {
        //ERR_NOSUCHNICK (401)  "<client> <nick> :No such nick"
        std::string msg = MSG_ERR_NOSUCHNICK("ircserv", client.getNick(), target);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    Client *targetClient = searchForClientref(target, _allClients);
    if (targetClient == NULL)
    {
        //ERR_NOSUCHNICK (401)  "<client> <nick> :No such nick"
        std::string msg = MSG_ERR_NOSUCHNICK("ircserv", client.getNick(), target);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    if (channel->hasClient(targetClient))
    {
        //ERR_USERONCHANNEL (443)  "<client> <nick> <channel> :User is already in the channel"
        std::string msg = MSG_ERR_USERONCHANNEL("ircserv", client.getNick(), target, channelName);
        client.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    channel->addInvited(targetClient);
    // channel->addClient(targetClient);
    // RPL_INVITING (341)  "<client> <nick> <channel>"
    std::string reply = MSG_RPL_INVITING("ircserv", client.getNick(), target, channelName);
    client.appendSendBuffer(reply,_pollFds, 0);
    std::string msg = MSG_RPL_INVITING("ircserv", client.getNick(), targetClient->getName(), channelName);
    client.invitedChannels.push_back(channel->getName());
    targetClient->appendSendBuffer(msg, _pollFds, 0);


}
