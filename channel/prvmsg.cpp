
#include "parsing.hpp"

// void Parsing::prvmsghelpre(bool flag, std::string message, Channel &ref, Client& refClient) // 1 for users, 0 for channels

void Parsing::prvmsg(std::string line, Client& client,std::map<int, Client*> _allClients)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');
    if (holder.size() < 2)
        {std::cout << " invalid number of args";return;}
    else
    {
        std::string target = holder[1];
        size_t index = line.find(":");
        if (index == std::string::npos)
            {std::cout <<"invalid format\n";return;}
        std::string message = line.substr(index + 1);
        if (holder[0] != "PRIVMSG")
            return;
        std::vector<std::string> targets = HelperSplit(target, ',');
        for (size_t i = 0; i < targets.size(); i++)
        {
            std::string target = targets[i];
            if (target.empty())
            {
                std::cout << "Empty target in PRIVMSG command\n";
                continue;
            }
            if (target[0] == '#')
            {
                if(!searchForChannel(target))
                {
                    std::string errorMsg = "401:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " +  " :No such nick/channel\r\n";
                    client.sendMsg(errorMsg);
                    continue;
                }
                Channel *channelHolder = searchForChannelref(target);
                if (channelHolder == NULL)
                {
                    std::string errorMsg = "401:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " +  " :No such nick/channel\r\n";
                    client.sendMsg(errorMsg);
                    continue;
                }
                if (channelHolder->isBanned(client) || !channelHolder->isOperator(client) || !channelHolder->hasClient(&client))
                {
                    std::string errorMsg = "404:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " +  " :Cannot send to channel\r\n";
                    client.sendMsg(errorMsg);
                    continue;
                }
                if (message.empty())
                {
                    std::string errorMsg = "412:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " +  " :No text to send\r\n";
                    client.sendMsg(errorMsg);
                    continue;
                }
                std::string msg = "412:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + "PRIVMSG " + target + " :" + message + "\r\n";
                // std::cout << "Broadcasting message to channel: " << target << " - " << message << std::endl;
                channelHolder->broadcastMsg(msg, channelHolder->getMembers());
            }
            else if (isalpha(target[0]))
            {
                if(!searchForClient(target, _allClients))
                {
                    // ERR_NOSUCHNICK (401)  "<client> <nickname> :No such nick/channel"
                    std::string errorMsg = "401:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " +  " :No such nick/channel\r\n";
                    client.sendMsg(errorMsg);
                    continue;
                }
                Client *clientHolder = searchForClientref(target, _allClients);
                if (clientHolder == NULL)
                {
                    std::string errorMsg = "401:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " +  " :No such nick/channel\r\n";
                    client.sendMsg(errorMsg);
                    continue;
                }
                std::string msg = "412:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + "PRIVMSG " + target + " :" + message + "\r\n";
                clientHolder->sendMsg(msg);
            }
            else
            {
                std::cout << "Invalid target: " << target << std::endl;
            }
        }
    }
}