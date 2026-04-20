#include "parsing.hpp"

void sendInviteList(Parsing& parsing, Client& client)
{
    std::map<std::string, Channel>& channels = parsing.Getchannel();
    for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->second.isInvited(client))
        {
            // RPL_INVITELIST (336)  "<client> <channel>"
            std::string msg = ":ircserv 336 " + client.getName() + " " + it->second.getName() + "\r\n";
            client.sendMsg(msg);
        }
    }
}


void Parsing::invite(std::string line, Client& client)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');


    if (holder.size() < 3)
    {
        //ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
        std::string msg =  ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + " INVITE :Not enough parameters\r\n";
        client.sendMsg(msg);
        return;
    }
    
    std::string target = holder[1];
    std::string channelName = holder[2];

   
    if (!searchForChannel(channelName))
    {
        //ERR_NOSUCHCHANNEL (403)  "<client> <channel> :No such channel"
        std::string msg = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + channelName + " :No such channel\r\n";
        client.sendMsg(msg);
        return;
    }
    Channel *channel = searchForChannelref(channelName);
    if (!channel->hasClient(&client))
    {
        //ERR_NOTONCHANNEL (442)  "<client> <channel> :You're not on that channel"
        std::string msg = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + channelName + " :You're not on that channel\r\n";
        client.sendMsg(msg);
        return;
    }
    if (!channel->isOperator(client) && channel->getInviteOnly())
    {
        //ERR_CHANOPRIVSNEEDED (482)  "<client> <channel> :You're not channel operator"
        std::string msg = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + channelName + " :You're not channel operator\r\n";
        client.sendMsg(msg);
        return;
    }

    if (!searchForClient(target))
    {
        //ERR_NOSUCHNICK (401)  "<client> <nick> :No such nick"
        std::string msg = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + target + " :No such nick\r\n";
        client.sendMsg(msg);
        return;
    }
    Client *targetClient = searchForClientref(target);
    if (targetClient == NULL)
    {
        //ERR_NOSUCHNICK (401)  "<client> <nick> :No such nick"
        std::string msg = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + target + " :No such nick\r\n";
        client.sendMsg(msg);
        return;
    }
    if (channel->hasClient(targetClient))
    {
        //ERR_USERONCHANNEL (443)  "<client> <nick> <channel> :User is already in the channel"
        std::string msg = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " " + target + " " + channelName + " :User is already in the channel\r\n";
        client.sendMsg(msg);
        return;
    }
    channel->addInvited(targetClient);
    // channel->addClient(targetClient);
    // RPL_INVITING (341)  "<client> <nick> <channel>"
    std::string reply = ":" + client.getNick() + "!" + client.getName() + "@" + Parsing::_gethostname() + " 341 " + client.getName() + " " + targetClient->getName() + " " + channel->getName() + "\r\n";
    client.sendMsg(reply);
    std::string msg = "You have been invited to join " + channel->getName() + " by " + client.getName() + "\r\n";
    client.invitedChannels.push_back(channel->getName());
    targetClient->sendMsg(msg);


}
