#include "parsing.hpp"
#include <cctype>


std::map<char, char> modeSpliter(std::vector<std::string>& splitMode)
{
    std::map<char, char> modes;
    if (splitMode.size() < 3) return modes;

    char sign = 0;
    for (size_t j = 0; j < splitMode[2].size(); ++j)
    {
        char c = splitMode[2][j];
        if (c == '+' || c == '-')
        {
            sign = c;
            continue;
        }
        else if (sign && std::isalpha(static_cast<unsigned char>(c)))
        {
            modes[c] = sign;
        }
    }
    return modes;
}


void PrintMap(std::map<char,char> helper)
{
    std::map<char,char>::iterator  it = helper.begin();
    while (it != helper.end())
    {
        std::cout <<  " the first param: " << it->first <<  " the second param: " << it->second<< "\n";
        it++;
    }
}

bool Parsing::searchForClient(std::string name, std::map<int, Client*> _allClients)
{
    std::map<std::string, Channel> chns = Getchannel();
    std::map<std::string, Channel>::iterator it;
    std::map<int, Client*>::iterator it2;

    for (it2 = _allClients.begin(); it2 != _allClients.end(); ++it2)
    {
        if (it2->second && it2->second->getNick() == name)
            return true;
    }

    for(it = chns.begin();it != chns.end(); it++)
    {
        std::map<int, Client*> ClientList = it->second.getmembers();
        std::map<int, Client*>::iterator  Cit;
        for ( Cit =  ClientList.begin(); Cit !=  ClientList.end(); Cit++)
        {   
            // Check if Client pointer is valid before dereferencing
            if (Cit->second != NULL && Cit->second->getNick() == name)
                return true;
        }
    }
    
    return false;
}

void Parsing::modeInviteOnly(Client &client, std::map<std::string, Channel>::iterator& it, t_mode& mode, std::vector<pollfd>& _pollFds)
{
    if (mode.sign == '+')
    {
        it->second.setInviteOnly(true);
        std::string msg = MSG_MODE_I("ircserv", client.getNick(), client.getName(), it->first, "+");
        it->second.broadcastMsg(msg, it->second.getMembers(), &client, _pollFds);
    }
    else
    {
        it->second.setInviteOnly(false);
        std::string msg = MSG_MODE_I("ircserv", client.getNick(), client.getName(), it->first, "-");
        it->second.broadcastMsg(msg, it->second.getMembers(), &client, _pollFds);
    }
    return ;
}
void Parsing::modeKey(Client &client, std::map<std::string, Channel>::iterator& it, t_mode& mode, std::vector<pollfd>& _pollFds)
{
    if (mode.sign == '+')
    {
        it->second.setKey(mode.param);
        std::string msg = MSG_MODE_K("ircserv", client.getNick(), client.getName(), it->first, "+", mode.param);
        it->second.broadcastMsg(msg, it->second.getMembers(), &client, _pollFds);
    }
    else
    {
        it->second.setKey("");
        std::string msg = MSG_MODE_K("ircserv", client.getNick(), client.getName(), it->first, "-", mode.param);
        it->second.broadcastMsg(msg, it->second.getMembers(), &client, _pollFds);
    }
    return ;
}
void Parsing::modeOperator(Client &client, std::map<std::string, Channel>::iterator& it, t_mode& mode, std::vector<pollfd>& _pollFds,std::map<int, Client*> _allClients)
{
    Client* targetClient = searchForClientref(mode.param, _allClients);
    if (!targetClient)
    {
        std::string msg = MSG_ERR_NOSUCHNICK("ircserv", client.getNick(), mode.param);
        client.appendSendBuffer(msg, _pollFds, 0);
        return ;
    }
    if (mode.sign == '+')
    {
        it->second.addOperator(targetClient);
        std::string msg = MSG_MODE_O("ircserv", client.getNick(), client.getName(), it->first, "+", mode.param);
        it->second.broadcastMsg(msg, it->second.getMembers(), &client, _pollFds);
    }
    else
    {
        it->second.removeOperator(targetClient);
        std::string msg = MSG_MODE_O("ircserv", client.getNick(), client.getName(), it->first, "-", mode.param);
        it->second.broadcastMsg(msg, it->second.getMembers(), &client, _pollFds);
    }
    return ;
}
void Parsing::modeTopic(Client &client, std::map<std::string, Channel>::iterator& it, t_mode& mode, std::vector<pollfd>& _pollFds)
{   
    if (mode.sign == '+')
    {
        it->second.setTopic(mode.param);
        // (std::string(":") + server + " MODE " + channel + " " + sign + "t\r\n")
        std::string msg = MSG_MODE_T("ircserv",client.getNick(),client.getName(),it->first,std::string(1, mode.sign));
        it->second.broadcastMsg(msg, it->second.getMembers(), &client, _pollFds);
    }
    else
    {
        it->second.setTopic("");
        std::string msg = MSG_MODE_T("ircserv",client.getNick(),client.getName(),it->first,std::string(1, mode.sign));
        it->second.broadcastMsg(msg, it->second.getMembers(), &client, _pollFds);
    }
    return ;
}
void Parsing::modeUserLimit(Client &client, std::map<std::string, Channel>::iterator& it, t_mode& mode, std::vector<pollfd>& _pollFds)
{
    if (mode.sign == '+')
    {
        int limit = std::atoi(mode.param.c_str());
        it->second.setUserLimit(limit);
        std::string msg = MSG_MODE_L("ircserv", client.getNick(), client.getName(), it->first, "+", mode.param);
        it->second.broadcastMsg(msg, it->second.getMembers(), &client, _pollFds);
    }
    else
    {
        it->second.setUserLimit(0);
        std::string msg = MSG_MODE_L("ircserv", client.getNick(), client.getName(), it->first, "-", mode.param);
        it->second.broadcastMsg(msg, it->second.getMembers(), &client, _pollFds);
    }
    return ;
}

bool itNeedsParam(const t_mode& modeer)
{
    if (modeer.mode == 'k' && modeer.sign == '+')
        return true;
    if (modeer.mode == 'o')
        return true;
    if (modeer.mode == 'l' )
    {
        if (modeer.sign == '+')
            return true;
        return false;
    }
    if (modeer.mode == 't' && modeer.sign == '+')
        return true;
    return false;
}

void Parsing::mode(Client &clinet, std::string line,std::map<int, Client*> _allClients)
{
    std::map<std::string, Channel>& chs = Getchannel();
    std::vector<std::string> splitMode = Parsing::HelperSplit(line, ' ');
    if (splitMode.size() < 2) {
        std::cout << "invalid number of arguments\n";
        return;
    }

    if (splitMode[0] != "MODE") {
        std::cout << "Not a MODE command\n";
        return;
    }
    std::map<std::string, Channel>::iterator it = chs.find(splitMode[1]);
    if (it == chs.end())
    {
        // irc.example.com 403 <nick> <channel> :No such channel
        // std::string msg = clinet.getName() + " " + splitMode[1] + " :No such channel\r\n";
        std::cout << "Channel not found: " << splitMode[1] << "\n";
        std::string msg = MSG_ERR_NOSUCHCHANNEL("ircserv", clinet.getNick(), splitMode[1]);
        clinet.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
    std::map<char, char> helper = modeSpliter(splitMode);
    if (splitMode.size() < 2)
    {
        return;
    }


    if (splitMode.size() < 3)
    {
        // RPL_CHANNELMODEIS (324)  "<client> <channel> <mode> <mode params>"
        std::string msg = MSG_RPL_CHANNELMODEIS("ircserv", clinet.getNick(), it->first, it->second.getModes());
        clinet.appendSendBuffer(msg, _pollFds, 0);
        return ;
    }


    
    if (splitMode.size() < 3)
    {
        std::string mdg = MSG_ERR_NEEDMOREPARAMS("ircserv", clinet.getNick(), splitMode[0]);
        clinet.appendSendBuffer(mdg, _pollFds, 0);
    }
    // check is client is operator in channel
    if (!it->second.isOperator(clinet))
    {
        std::string msg = MSG_ERR_CHANOPRIVSNEEDED("ircserv", clinet.getNick(), it->first);
        clinet.appendSendBuffer(msg, _pollFds, 0);
        return ;
    }

   std::string ModesSigns = splitMode[2];

    std::cout << ModesSigns << std::endl;

    std::vector<t_mode> holder;

    char currentSign = 0;

    for (size_t i = 0; i < ModesSigns.size(); i++)
    {
        char c = ModesSigns[i];

        if (c == '+' || c == '-')
        {
            currentSign = c;
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c)))
        {
            if (currentSign == 0)
            {
                std::cout << "Mode without sign: " << c << "\n";
                continue;
            }

            t_mode mode;

            mode.mode = c;
            mode.sign = currentSign;
            mode.param = "";

            holder.push_back(mode);
        }
        else
        {
            std::cout << "Invalid mode character: " << c << "\n";
        }
    }

    size_t paramIndex = 3;

    for (size_t i = 0; i < holder.size(); ++i)
    {
        if (itNeedsParam(holder[i]))
        {
            if (paramIndex >= splitMode.size())
            {
                std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv",clinet.getNick(),splitMode[0]);
                clinet.appendSendBuffer(msg, _pollFds, 0);
                return ;
            }
            holder[i].param = splitMode[paramIndex];
            paramIndex++;
        }
    }
    for (size_t i = 0; i < holder.size(); ++i)
    {
        std::cout << "Mode: " << holder[i].mode << " Sign: " << holder[i].sign << " Param: " << holder[i].param << "\n";
        if (holder[i].mode == 'i') 
            modeInviteOnly(clinet, it, holder[i], _pollFds);
        else if (holder[i].mode == 'k') 
            modeKey(clinet, it, holder[i], _pollFds);
        else if (holder[i].mode == 'o') 
            modeOperator(clinet, it, holder[i], _pollFds, _allClients);
        else if (holder[i].mode == 't') 
            modeTopic(clinet, it, holder[i], _pollFds);
        else if (holder[i].mode == 'l')
            modeUserLimit(clinet, it, holder[i], _pollFds);
        else
        {
            std::string msg = MSG_ERR_UNKNOWNMODE("ircserv", clinet.getNick(), std::string(1, holder[i].mode));
            clinet.appendSendBuffer(msg, _pollFds, 0);
        }

    }
    // for (size_t j = 0 ; splitMode[2].size() > j ; j++)
    // {
    //     char c = splitMode[2][j];
    //     if (c == '+' || c == '-')
    //     {
    //         sign = c;
    //         j++;c = splitMode[2][j];
    //         if (splitMode[2].size() > j &&  (c == 'i' || c == 't' || c == 'k' || c == 'o' || c == 'l'))
    //         {
    //             // i
    //             if (c == 'i')
    //             {
    //                 if (sign == '+')
    //                 {
    //                     // Enable invite-only mode
    //                     it->second.setInviteOnly(true);
    //                     //broadcast to all the client in channel
    //                     // std::string msg = clinet.getName() + " has set the channel to invite-only.\r\n";
    //                     std::string msg = MSG_MODE_I("ircserv", clinet.getNick(), clinet.getName(), it->first, "+");
    //                     it->second.broadcastMsg(msg, it->second.getMembers(), &clinet, _pollFds, 0);
    //                 }
    //                 else
    //                 {
    //                     it->second.setInviteOnly(false);
    //                     std::string msg = MSG_MODE_I("ircserv", clinet.getNick(), clinet.getName(), it->first, "-");
    //                     it->second.broadcastMsg(msg, it->second.getMembers(), &clinet, _pollFds, 0);
    //                 }
    //             }
    //             // t
    //             if (c == 't')
    //             {
    //                 if (sign == '+')
    //                 {
    //                     // Enable topic restriction mode
    //                     if (splitMode.size() < 4) // Check if topic is provided and is printable
    //                     {
    //                         // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
    //                         // std::string msg = "ircserv 461:" + clinet.getNick() + "!" + clinet.getName() + "@" + Parsing::_gethostname() + " " + it->first + " :Not enough parameters\n";
    //                         std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", clinet.getNick(), splitMode[0]);
    //                         clinet.appendSendBuffer(msg, _pollFds, 0);
    //                         return ;
    //                     }
    //                     //check if topic is printable
    //                     for (size_t i = 0; i < splitMode[3].size(); ++i)
    //                     {
    //                         if (!std::isprint(static_cast<unsigned char>(splitMode[3][i])))
    //                         {
    //                             // std::string msg = "ircserv 461:" + clinet.getNick() + "!" + clinet.getName() + "@" + Parsing::_gethostname() + " " + it->first + " :Invalid topic format (contains non-printable characters)\n";
    //                             std::string msg = MSG_ERR_INVALIDTOPIC("ircserv", clinet.getNick(), it->first);
    //                             clinet.appendSendBuffer(msg, _pollFds, 0);
    //                             return ;
    //                         }
    //                     }
    //                     size_t index = line.find(":");
    //                     if (index == std::string::npos)
    //                     {
    //                         std::string msg = "ircserv 461:" + clinet.getNick() + "!" + clinet.getName() + "@" + Parsing::_gethostname() + " " + it->first + " :Not enough parameters\r\n";
    //                         clinet.appendSendBuffer(msg, _pollFds, 0);
    //                         return ;
    //                     }
    //                     std::string topic = line.substr(index + 1);
    //                     it->second.setTopic(topic);
    //                     it->second.setTopicOwner(clinet.getName());
    //                     it->second.setTopicSetTime(time(NULL));
    //                     //broadcast to all the client in channel
    //                     // std::string msg = clinet.getName() + " has set the topic to: " + topic + "\r\n";
    //                     std::string msg = MSG_MODE_T_SET("ircserv", clinet.getNick(), clinet.getName(), it->first, topic);
    //                     it->second.broadcastMsg(msg, it->second.getMembers(), &clinet, _pollFds, 0);
    //                 }
    //                 else
    //                 {

    //                     it->second.setTopic("");
    //                     std::string msg = MSG_MODE_T_SET("ircserv", clinet.getNick(), clinet.getName(), it->first, "");
    //                     it->second.broadcastMsg(msg, it->second.getMembers(), &clinet, _pollFds, 0);
    //                 }
    //             }
    //             // k
    //             if (c == 'k')
    //             {
    //                 if (sign == '+')
    //                 {
    //                     if (splitMode.size() < 4)
    //                     {
    //                         // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
    //                         // std::string msg = "ircserv 461:" + clinet.getNick() + "!" + clinet.getName() + "@" + Parsing::_gethostname() + " " + it->first + " :Not enough parameters\n";
    //                         std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", clinet.getNick(), splitMode[0]);
    //                         clinet.appendSendBuffer(msg, _pollFds, 0);
    //                         return ;
    //                     }
    //                     // Check if it is a valid key
    //                     // for (size_t i = 0; i < splitMode[3].size(); ++i)
    //                     // {
    //                     //     if (!std::isprint(static_cast<unsigned char>(splitMode[3][i])))
    //                     //     {
    //                     //         std::cout << "Channel key contains non-printable characters\n";
    //                     //         return ;
    //                     //     }
    //                     // }
    //                     it->second.setKey(splitMode[3]);
    //                     // std::cout << "Channel key set to: '" << splitMode[3] << "' (length: " << splitMode[3].length() << ")\n";
    //                 }
    //                 else
    //                 {
    //                     // Disable channel key mode
    //                     if (it->second.hasKey())
    //                         it->second.setKey("");
    //                     std::string msg = MSG_MODE_K_UNSET("ircserv", clinet.getNick(), clinet.getName(), it->first);
    //                     it->second.broadcastMsg(msg, it->second.getMembers(), &clinet, _pollFds, 0);
    //                 }
    //             }
    //             // o
    //             if (c == 'o')
    //             {
    //                 if (sign == '+')
    //                 {
    //                     // Enable channel operator mode
    //                     if (splitMode.size() < 4)
    //                     { 
    //                         // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
    //                         std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", clinet.getNick(), splitMode[0]);
    //                         clinet.appendSendBuffer(msg, _pollFds, 0);
    //                         return ;
    //                     }
    //                     std::string operatorName = splitMode[3];
    //                     if (!searchForClient(operatorName, _allClients))
    //                     {
    //                         std::string msg = MSG_ERR_NOSUCHNICK("ircserv", clinet.getNick(), operatorName);
    //                         clinet.appendSendBuffer(msg, _pollFds, 0);
    //                         return ;
    //                     }
    //                     Client* operatorClient = searchForClientref(operatorName, _allClients);
    //                     // check if the client is already an operator
    //                     if (it->second.isOperator(*operatorClient))
    //                     {
    //                         // std::string msg = "ircserv 461:" + clinet.getNick() + "!" + clinet.getName() + "@" + Parsing::_gethostname() + " " + it->first + " :Client with nickname '" + operatorName + "' is already an operator\n";
    //                         std::string msg = MSG_ERR_USERNOTINCHANNEL("ircserv", clinet.getNick(), operatorName, it->first);
    //                         clinet.appendSendBuffer(msg, _pollFds, 0);
    //                         return ;
    //                     }
    //                     else if (operatorClient)
    //                     {
    //                         it->second.getoperators().insert(operatorClient);
    //                         //(server, nick, channel, mode, target)
    //                         std::string msg = RPL_MODE("ircserv", clinet.getNick(), it->first, "+o ", operatorName);
    //                         it->second.broadcastMsg(msg, it->second.getMembers(), &clinet,_pollFds, 0);
    //                     }
    //                     else
    //                     {
    //                         // std::string msg = "ircserv 461:" + clinet.getNick() + "!" + clinet.getName() + "@" + Parsing::_gethostname() + " " + it->first + " :Client with nickname '" + operatorName + "' not found\n";
    //                         std::string msg = MSG_ERR_NOSUCHNICK("ircserv", clinet.getNick(), operatorName);
    //                         clinet.appendSendBuffer(msg, _pollFds, 0);
    //                         return ;
    //                     }
    //                 }
    //                 else
    //                 {
    //                     // Disable channel operator mode
    //                     if (splitMode.size() < 4)
    //                     { 
    //                         // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
    //                         std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", clinet.getNick(), splitMode[0]);
    //                         clinet.appendSendBuffer(msg, _pollFds, 0);
    //                         return ;
    //                     }
    //                     std::string operatorName = splitMode[3];
    //                     if (!searchForClient(operatorName, _allClients))
    //                     {
    //                         std::string msg = MSG_ERR_NOSUCHNICK("ircserv", clinet.getNick(), operatorName);
    //                         clinet.appendSendBuffer(msg, _pollFds, 0);
    //                         return ;
    //                     }
    //                     Client* operatorClient = searchForClientref(operatorName, _allClients);
    //                     if (it->second.isOperator(*operatorClient))
    //                     {
    //                         it->second.getoperators().erase(operatorClient);
    //                         return ;
    //                     }
    //                 }
    //             }
    //             // l
    //             if (c == 'l')
    //             {
    //                 if (sign == '+')
    //                 {
    //                     // Enable user limit mode
    //                     size_t userLimit;
    //                     if (splitMode.size() < 4)
    //                     {
    //                         // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
    //                         // std::string msg = "ircserv 461:" + clinet.getNick() + "!" + clinet.getName() + "@" + Parsing::_gethostname() + " " + it->first + " :Not enough parameters\n";
    //                         std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", clinet.getNick(), splitMode[0]);
    //                         clinet.appendSendBuffer(msg, _pollFds, 0);
    //                         return ;
    //                     }
    //                     try 
    //                     {
    //                         userLimit = std::strtoul(splitMode[3].c_str(), NULL, 10);
    //                         if (userLimit == 0)
    //                         {
    //                             // std::string msg = "ircserv 461:" + clinet.getNick() + "!" + clinet.getName() + "@" + Parsing::_gethostname() + " " + it->first + " :User limit must be greater than 0\n";
    //                             std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", clinet.getNick(), splitMode[0]);
    //                             clinet.appendSendBuffer(msg, _pollFds, 0);
    //                             return ;
    //                         }
    //                     }
    //                     catch (const std::invalid_argument& e) 
    //                     {
    //                         std::cout << "Invalid user limit\n";
    //                         return ;
    //                     }
    //                     it->second.setUserLimit(userLimit);
                        
    //                     // std::string msg = "ircserv 461:" + clinet.getNick() + "!" + clinet.getName() + "@" + Parsing::_gethostname() + " " + it->first + " :User limit set to " + splitMode[3] + "\r\n";
    //                     std::string msg = MSG_MODE_L_SET("ircserv", clinet.getNick(), clinet.getName(), it->first, splitMode[3]);
    //                     it->second.broadcastMsg(msg, it->second.getMembers(), &clinet,_pollFds, 0);                 
    //                 }
    //                 else
    //                 {
    //                     // Disable user limit mode
    //                     it->second.setUserLimit(0);
    //                     std::string msg = MSG_MODE_L_UNSET("ircserv", clinet.getNick(), clinet.getName(), it->first);
    //                     it->second.broadcastMsg(msg, it->second.getMembers(), &clinet, _pollFds, 0);
    //                 }
    //             }

    //         }

    //     }
    //     else
    //     {
    //         // std::string msg = "ircserv 461:" + clinet.getNick() + "!" + clinet.getName() + "@" + Parsing::_gethostname() + " " + it->first + " :Unknown mode character '" + c + "'\n";
    //         std::string msg = MSG_ERR_UNKNOWNMODE("ircserv", clinet.getNick(), c);
    //         clinet.appendSendBuffer(msg, _pollFds, 0);
    //         return ;
    //     }
    // }
}
