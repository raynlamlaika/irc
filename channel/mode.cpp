#include "parsing.hpp"
#include <cctype>


std::map<char, char> modeSpliter(const std::vector<std::string>& splitMode)
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

bool Parsing::searchForClient(std::string name)
{
    std::map<std::string, Channel> chns = Getchannel();
    std::map<std::string, Channel>::iterator it;

    for(it = chns.begin();it != chns.end(); it++)
    {
        std::map<int, Client*> ClientList = it->second.getmembers();
        std::map<int, Client*>::iterator  Cit;
        for ( Cit =  ClientList.begin(); Cit !=  ClientList.end(); Cit++)
        {   
            // Check if Client pointer is valid before dereferencing
            if (Cit->second != NULL && Cit->second->getName() == name)
                return true;
        }
    }
    
    return false;
}

void Parsing::mode(Client &clinet, std::string line)
{
    (void)clinet;
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
    // std::cout << "Processing MODE command for target: TTTTTTTTTTTTTTTTTTTTT&WIITTTTTTTTTTTTTTTTTTTTIKARI" << splitMode[1] << "\n";

    // check valid name for channel
    std::map<std::string, Channel>::iterator it = chs.find(splitMode[1]);
    if (it == chs.end())
    {
        // irc.example.com 403 <nick> <channel> :No such channel
        std::string msg = clinet.getName() + " " + splitMode[1] + " :No such channel\r\n";
        clinet.sendMsg(msg);
        return;
    }
    std::map<char, char> helper = modeSpliter(splitMode);
    if (splitMode.size() < 2)
    {
        // std::cout << "Processing MODE command for target: TTTTTTTTTTTTTTTTTTTTT&WIITTTTTTTTTTTTTTTTTTTTIKARI" << splitMode[1] << "\n";
        return;
    }

    int sign = 0 ;

    for (size_t j = 0 ; splitMode[2].size() > j ; j++)
    {
        char c = splitMode[2][j];
        if (c == '+' || c == '-')
        {
            sign = c;
            j++;c = splitMode[2][j];
            if (splitMode[2].size() > j &&  (c == 'i' || c == 't' || c == 'k' || c == 'o' || c == 'l'))
            {
                // i
                if (c == 'i')
                {
                    if (sign == '+')
                    {
                        // Enable invite-only mode
                        it->second.setInviteOnly(true);
                        //broadcast to all the client in channel
                        it->second.broadcastMsg(clinet.getName() + " has set the channel to invite-only.", it->second.getmembers());
                    }
                    else
                        it->second.setInviteOnly(false);
                }
                // t
                if (c == 't')
                {
                    if (sign == '+')
                    {
                        // Enable topic restriction mode
                        if (splitMode.size() < 4) // Check if topic is provided and is printable
                        {
                            // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
                            std::string msg = clinet.getNick() + " T " + it->first + " :Not enough parameters\n";
                            clinet.sendMsg(msg);
                            return ;
                        }
                        //check if topic is printable
                        for (size_t i = 0; i < splitMode[3].size(); ++i)
                        {
                            if (!std::isprint(static_cast<unsigned char>(splitMode[3][i])))
                            {
                                std::string msg = clinet.getNick() + " T " + it->first + " :Invalid topic format (contains non-printable characters)\n";
                                clinet.sendMsg(msg);
                                return ;
                            }
                        }
                        size_t index = line.find(":");
                        if (index == std::string::npos)
                        {
                            std::string msg = clinet.getNick() + " T " + it->first + " :Not enough parameters\r\n";
                            clinet.sendMsg(msg);
                            return ;
                        }
                        std::string topic = line.substr(index + 1);
                        it->second.setTopic(topic);
                    }
                    else
                    {
                        // Disable topic restriction mode
                        it->second.setTopic("");
                    }
                }
                // k
                if (c == 'k')
                {
                    if (sign == '+')
                    {
                        if (splitMode.size() < 4)
                        {
                            // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
                            std::string msg = clinet.getNick() + " K " + it->first + " :Not enough parameters\n";
                            clinet.sendMsg(msg);
                            return ;
                        }
                        // Check if it is a valid key
                        // for (size_t i = 0; i < splitMode[3].size(); ++i)
                        // {
                        //     if (!std::isprint(static_cast<unsigned char>(splitMode[3][i])))
                        //     {
                        //         std::cout << "Channel key contains non-printable characters\n";
                        //         return ;
                        //     }
                        // }
                        it->second.setKey(splitMode[3]);
                        // std::cout << "Channel key set to: '" << splitMode[3] << "' (length: " << splitMode[3].length() << ")\n";
                    }
                    else
                    {
                        // Disable channel key mode
                        if (it->second.hasKey())
                            it->second.setKey("");
                    }
                }
                // o
                if (c == 'o')
                {
                    if (sign == '+')
                    {
                        // Enable channel operator mode
                        if (splitMode.size() < 4)
                        { 
                            // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
                            std::string msg = clinet.getNick() + " O " + it->first + " :Not enough parameters\n";
                            clinet.sendMsg(msg);
                            return ;
                        }
                        std::string operatorName = splitMode[3];
                        if (!searchForClient(operatorName))
                        {
                            std::string msg = clinet.getNick() + " O " + it->first + " :Client with nickname '" + operatorName + "' not found\n";
                            clinet.sendMsg(msg);
                            return ;
                        }
                        Client* operatorClient = searchForClientref(operatorName);
                        // check if the client is already an operator
                        if (it->second.isOperator(*operatorClient))
                        {
                            std::string msg = clinet.getNick() + " O " + it->first + " :Client with nickname '" + operatorName + "' is already an operator\n";
                            clinet.sendMsg(msg);
                            return ;
                        }
                        if (operatorClient)
                        {
                            it->second.getoperators().insert(operatorClient);
                        }
                        else
                        {
                            std::string msg = clinet.getNick() + " O " + it->first + " :Client with nickname '" + operatorName + "' not found\n";
                            clinet.sendMsg(msg);
                            return ;
                        }
                    }
                    else
                    {
                        // Disable channel operator mode
                        if (splitMode.size() < 4)
                        { 
                            // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
                            std::string msg = clinet.getNick() + " O " + it->first + " :Not enough parameters\n";
                            clinet.sendMsg(msg);
                            return ;
                        }
                        std::string operatorName = splitMode[3];
                        if (!searchForClient(operatorName))
                        {
                            std::string msg = clinet.getNick() + " O " + it->first + " :Client with nickname '" + operatorName + "' not found\n";
                            clinet.sendMsg(msg);
                            return ;
                        }
                        Client* operatorClient = searchForClientref(operatorName);
                        if (it->second.isOperator(*operatorClient))
                        {
                            it->second.getoperators().erase(operatorClient);
                            return ;
                        }
                    }
                }
                // l
                if (c == 'l')
                {
                    if (sign == '+')
                    {
                        // Enable user limit mode
                        size_t userLimit;
                        if (splitMode.size() < 4)
                        {
                            // ERR_NEEDMOREPARAMS (461)  "<client> <command> :Not enough parameters"
                            std::string msg = clinet.getNick() + " L " + it->first + " :Not enough parameters\n";
                            clinet.sendMsg(msg);
                            return ;
                        }
                        try 
                        {
                            userLimit = std::strtoul(splitMode[3].c_str(), NULL, 10);
                            if (userLimit == 0)
                            {
                                std::string msg = clinet.getNick() + " O " + it->first + " :User limit must be greater than 0\n";
                                clinet.sendMsg(msg);
                                return ;
                            }
                        }
                        catch (const std::invalid_argument& e) 
                        {
                            std::cout << "Invalid user limit\n";
                            return ;
                        }
                        it->second.setUserLimit(userLimit);
                    }
                    else
                    {
                        // Disable user limit mode
                        it->second.setUserLimit(0);
                    }
                }

            }

        }
        else
        {
            // std::cout << "Invalid mode format\n";
            
            return ;
            // wtf is happend;}
        }
    }
}
