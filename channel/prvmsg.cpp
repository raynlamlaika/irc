
#include "parsing.hpp"

// void Parsing::prvmsghelpre(bool flag, std::string message, Channel &ref, Client& refClient) // 1 for users, 0 for channels

void Parsing::prvmsg(std::string line, Client& client,std::map<int, Client*> _allClients)
{
    std::vector<std::string> holder = HelperSplit(line, ' ');
    if (holder.size() < 2)
        {std::cout << " invalid number of args";return;}
    else
    {
        std::string message;
        std::string target = holder[1];
        size_t index = line.find(":");
        if (index == std::string::npos && holder.size() < 2)
            {std::cout<< line <<":\tinvalid format\n";return;}
        if (index != std::string::npos)
            message = line.substr(index + 1);
        else
            message = holder[2];
        if (holder[0] != "PRIVMSG")
            return;
        std::vector<std::string> targets = HelperSplit(target, ',');
        for (size_t i = 0; i < targets.size(); i++)
        {
            std::string target = targets[i];
            if (target.empty())
            {
                std::string errorMsg = MSG_ERR_NORECIPIENT("ircserv", client.getNick(), "PRIVMSG");
                client.sendMsg(errorMsg);
                continue;
            }
            if (target[0] == '#' || target[0] == '&' || target[0] == '+' || target[0] == '!')
            {
                if(!searchForChannel(target))
                {
                    // std::string errorMsg = "ircserv 401:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " +  " :No such nick/channel\r\n";
                    std::string errorMsg = MSG_ERR_NOSUCHCHANNEL("ircserv", client.getNick(), target);
                    client.sendMsg(errorMsg);
                    continue;
                }
                Channel *channelHolder = searchForChannelref(target);
                if (channelHolder == NULL)
                {
                    std::string errorMsg = MSG_ERR_NOSUCHCHANNEL("ircserv", client.getNick(), target);
                    client.sendMsg(errorMsg);
                    continue;
                }
                if (channelHolder->isBanned(client) || !channelHolder->hasClient(&client))
                {
                    std::string errorMsg = MSG_ERR_CANNOTSENDTOCHAN("ircserv", client.getNick(), target);
                    client.sendMsg(errorMsg);
                    continue;
                }
                if (message.empty())
                {
                    // std::string errorMsg = "ircserv 412:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " +  " :No text to send\r\n";
                    std::string errorMsg = MSG_ERR_NOTEXTTOSEND("ircserv", client.getNick());
                    std::cout << message << std::endl;
                    client.sendMsg(errorMsg);
                    continue;
                }
                std::string msg = MSG_PRIVMSG(client.getNick(), client.getName(), _gethostname(), target, message);
                channelHolder->broadcastMsg(msg, channelHolder->getMembers(), &client);
            }
            else if (isalpha(target[0]))
            {
                if(!searchForClient(target, _allClients))
                {
                    // ERR_NOSUCHNICK (401)  "<client> <nickname> :No such nick/channel"
                    // std::string errorMsg = "ircserv 401:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " +  " :No such nick/channel\r\n";
                    std::string errorMsg = MSG_ERR_NOSUCHNICK("ircserv", client.getNick(), target);
                    client.sendMsg(errorMsg);
                    continue;
                }
                Client *clientHolder = searchForClientref(target, _allClients);
                if (clientHolder == NULL)
                {
                    // std::string errorMsg = "ircserv 401:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " +  " :No such nick/channel\r\n";
                    std::string errorMsg = MSG_ERR_NOSUCHNICK("ircserv", client.getNick(), target);
                    client.sendMsg(errorMsg);
                    continue;
                }
                // std::string msg = "ircserv 412:" + client.getNick() + "!" + client.getName() + "@" + _gethostname() + " " + "PRIVMSG " + target + " :" + message + "\r\n";
                std::string msg = MSG_PRIVMSG(client.getNick(), client.getName(), _gethostname(), target, message);
                clientHolder->sendMsg(msg);
            }
            else
            {
                std::cout << "Invalid target: " << target << std::endl;
            }
        }
    }
}

