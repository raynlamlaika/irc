#include "parsing.hpp"
#include <cctype>

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
    if (splitMode.size() < 2)
    {
        std::string msg = MSG_ERR_NEEDMOREPARAMS("ircserv", clinet.getNick(), splitMode[0]);
        clinet.appendSendBuffer(msg, _pollFds, 0);
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
        std::string msg = MSG_ERR_NOSUCHCHANNEL("ircserv", clinet.getNick(), splitMode[1]);
        clinet.appendSendBuffer(msg, _pollFds, 0);
        return;
    }
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
        return ;
    }
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
                continue;

            t_mode mode;
            mode.mode = c;
            mode.sign = currentSign;
            mode.param = "";
            holder.push_back(mode);
        }
        else
        {
            std::string msg = MSG_ERR_UNKNOWNMODE("ircserv", clinet.getNick(), std::string(1, c));
            clinet.appendSendBuffer(msg, _pollFds, 0);
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
}
