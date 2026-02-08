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


void Parsing::kick(std::string line)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');
    if (holder.size() < 3) {
        std::cout << "not enough arguments for KICK command\n";
        return;
    }

    std::string channelname = holder[1];
    std::string usertarget = holder[2];
    if (holder[0] != "KICK")
        return;

    Channel* channel = searchForChannelref(channelname);
    if (!channel) {
        std::cout << "can't find target channel\n";
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
        std::cout << "can't find target user in channel\n";
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