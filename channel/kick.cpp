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


std::string Parsing::_gethostname()
{
    // return "localhost";
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0)
    {
        // std::cout << "Hostname: " << hostname << std::endl;
        return std::string(hostname);
    }
    else {
        return "localhost";
    }
    return std::string(hostname);
}
    
void Parsing::kick(std::string line, Client& client)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');
    if (holder.size() < 3){
        // ERR_NEEDMOREPARAMS (461) "<client> <command> :Not enough parameters"
        std::string msg = "461:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + " :Not enough parameters\r\n";
        client.sendMsg(msg);
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
        
        std::string msg = "403:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + " :No such channel\r\n";
        client.sendMsg(msg);
        return;
    }
    if (!channel->isOperator(client))
    {
        // ERR_CHANOPRIVSNEEDED (482)  "<client> <channel> :You're not channel operator"
        std::string msg = "482:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + " :KICK "+ channelname + " " + usertarget +   "\r\n";
        client.sendMsg(msg);
        return;
    }
    // check client is part of the channel
    if (!channel->hasClient(&client)) {
        // ERR_USERNOTINCHANNEL (441) "<client> <nick> <channel> :They aren't on that channel"
        std::string msg = "441:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + " :KICK "+ channelname + " " + usertarget +   "\r\n";
        client.sendMsg(msg);
        return;
    }

    Client* targetClient = NULL;
    std::map<int, Client*> members = channel->getmembers();

    for (int y = 0 ; usersTargeted.size() >  y; y++)
    {
        usertarget = usersTargeted[y];
        for (std::map<int, Client *>::iterator it = members.begin(); it != members.end(); ++it)
        {
            if (it->second && it->second->getName() == usertarget)
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
                std::string msg = "367:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + " :KICK "+ channelname + " " + usertarget +   "\r\n";
                client.sendMsg(msg);
                if (!reason.empty())
                {
                    msg = "367:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + " :KICK "+ channelname + " " + usertarget +   "\r\n";
                    client.sendMsg(msg);
                }
                break;
            }
        }
        if (!targetClient)
        {
            // ERR_USERNOTINCHANNEL (441) "<client> <nick> <channel> :They aren't on that channel"
            std::string msg = "441:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + " :KICK "+ channelname + " " + usertarget +   "\r\n";
            client.sendMsg(msg);
            // return;
        }
    }
}
