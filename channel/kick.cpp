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


// check on 
/*
ERR_BADCHANMASK (476)  "<client> <channel> :Bad Channel Mask"
*/

void Parsing::kick(std::string line, Client& client)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');
    if (holder.size() < 3) {
        // ERR_NEEDMOREPARAMS (461) "<client> <command> :Not enough parameters"
        std::cout << client.getName() << " KICK :Not enough parameters\n";
        return ;
    }

    std::string channelname = holder[1];
    std::string usertarget = holder[2];
    if (holder[0] != "KICK")
        return;

    Channel* channel = searchForChannelref(channelname);
    if (!channel)
    {
        // ERR_NOSUCHCHANNEL (403)  "<client> <channel> :No such channel"
        std::cout << client.getName() << " " << channelname << " :No such channel\n";
        return;
    }
    if (!channel->isOperator(client)) {
        // ERR_CHANOPRIVSNEEDED (482)  "<client> <channel> :You're not channel operator"
        std::cout << client.getName() << " " << channelname << " :You're not channel operator\n";
        return;
    }
    // check client is part of the channel
    if (!channel->hasClient(client)) {
        // ERR_USERNOTINCHANNEL (441) "<client> <nick> <channel> :They aren't on that channel"
        std::cout << client.getName() << " " << usertarget << " " << channelname << " :They aren't on that channel\n";
        return;
    }

    Client* targetClient = NULL;
    std::map<int, Client*> members = channel->getmembers();
    for (auto it = members.begin(); it != members.end(); ++it) {
        if (it->second && it->second->getName() == usertarget) {
            targetClient = it->second;
            break;
        }
    }
    if (!targetClient) {
        // ERR_USERNOTINCHANNEL (441) "<client> <nick> <channel> :They aren't on that channel"
        std::cout << client.getName() << " " << usertarget << " " << channelname << " :They aren't on that channel\n";
        return;
    }

    std::string reason = "";
    if (holder.size() > 3 && holder[3][0] == ':') {
        size_t index = line.find(":");
        if (index != std::string::npos)
            reason = line.substr(index + 1);
    }

    channel->removeClient(*targetClient);
    std::cout << "User " << usertarget << " kicked from channel " << channelname;
    if (!reason.empty())
        std::cout << " for reason: " << reason;
    std::cout << std::endl;
}

