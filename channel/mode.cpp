
#include "parsing.hpp"
#include <cctype>

// std::map<char,int> modeSpliter(std::vector<std::string> splitMode)
// {
//     std::map<char ,int> *n = new std::map<char ,int>();

//     for (int i = 2 ; splitMode.size() > i  ; i++)
//     {
//         for (int j = 0 ; splitMode[i].size() > j; j++)
//         {
//             if (splitMode[i][j] == '+')
//             {
//                 j++;
//                 if (splitMode[i].size() > j)
//                     break ;
//                 while (std::isalpha(splitMode[i][j]))
//                 {
//                     n->insert(std::make_pair('+', splitMode[i][j]));
//                     j++;
//                     if (splitMode[i].size() > j)
//                         break ;
//                 }
                
                
//             }
//             else if (splitMode[i][j] == '-')
//             {
//                 j++;
//                 if (splitMode[i].size() > j)
//                     break ;
//                 while (std::isalpha(splitMode[i][j]))
//                 {
//                     n->insert(std::make_pair('-', splitMode[i][j]));
//                     j++;
//                     if (splitMode[i].size() > j)
//                         break ;
//                 }
//             }
//         }
//     }   
// }

// bool Parsing::searchClient()
// {
// }


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
        if (sign && std::isalpha(static_cast<unsigned char>(c)))
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

bool Parsing::seachForClient(std::string name)
{
    std::map<std::string, Channel> chns = Getchannel();
    std::map<std::string, Channel>::iterator it;

    for(it = chns.begin();it != chns.end(); it++)
    {
        std::map<int, Client*> ClientList = it->second.getmembers();
        std::map<int, Client*>::iterator  Cit;
        for ( Cit =  ClientList.begin(); Cit !=  ClientList.end(); Cit++)
        {   
            if (Cit->second->getName() == name)
                return true;
        }
    }
    return false;
}

void Parsing::mode(Client &clinet, std::string line)
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

    if (splitMode[1][0] == '#') { // deals with channel
        auto it = chs.find(splitMode[1]);
        std::cout << "this is the channel name: " << splitMode[1] << "\n";
        if (it == chs.end()) {
            std::cout << "channel not exist\n";
            return;
        }
        std::map<char, char> helper = modeSpliter(splitMode);
        PrintMap(helper);
        Channel& channel = it->second;
        // · i: Set/remove Invite-only channel
        if (helper.find('i') != helper.end())
        {
            std::cout << "Setting invite-only mode: " << (helper['i'] == '+' ? "ON" : "OFF") << "\n";
            if (helper['i'] == '+')
            {
                // Enable invite-only mode
                channel.setInviteOnly(true);
            }
            else
            {
                // Disable invite-only mode
                channel.setInviteOnly(false);
            }
        }
        // · t: Set/remove the restrictions of the TOPIC command to channel operators
        if (helper.find('t') != helper.end())
        {
            std::cout << "Setting topic restriction mode: " << (helper['t'] == '+' ? "ON" : "OFF") << "\n";
            if (helper['t'] == '+')
            {
                // Enable topic restriction mode
                if (splitMode.size() < 4) // Check if topic is provided and is printable
                {
                    std::cout << "Topic not provided\n";
                    return ;
                }
                //check if topic is printable
                for (size_t i = 0; i < splitMode[3].size(); ++i)
                {
                    if (!std::isprint(static_cast<unsigned char>(splitMode[3][i])))
                    {
                        std::cout << "Topic contains non-printable characters\n";
                        return ;
                    }
                }
                channel.setTopic(splitMode[3]);
            }
            else
            {
                // Disable topic restriction mode
                channel.setTopic("");
            }
        }
        // · k: Set/remove the channel key (password)
        if (helper.find('k') != helper.end())
        {
            std::cout << "Setting channel key mode: " << (helper['k'] == '+' ? "ON" : "OFF") << "\n";
            if (helper['k'] == '+')
            {
                if (splitMode.size() < 4)
                {
                    std::cout << "Channel key not provided\n";
                    return ;
                }
                // Check if the key contains only printable characters
                // for (size_t i = 0; i < splitMode[3].size(); ++i)
                // {
                //     if (!std::isprint(static_cast<unsigned char>(splitMode[3][i])))
                //     {
                //         std::cout << "Channel key contains non-printable characters\n";
                //         return ;
                //     }
                // }
                channel.setKey(splitMode[3]);
                std::cout << "Channel key set to: '" << splitMode[3] << "' (length: " << splitMode[3].length() << ")\n";
            }
            else
            {
                // Disable channel key mode
                if (channel.hasKey())
                    channel.setKey("");
            }
        
        }
        // · o: Give/take channel operator privilege
        if (helper.find('o') != helper.end())
        {
            std::cout << "Setting channel operator mode: " << (helper['o'] == '+' ? "ON" : "OFF") << "\n";
            if (helper['o'] == '+')
            {
                // Enable channel operator mode
                if (splitMode.size() < 4)
                {
                    std::cout << "Operator nickname not provided\n";
                    return ;
                }
                std::string operatorName = splitMode[3];
                if (!seachForClient(operatorName))
                {
                    std::cout << "Client with nickname '" << operatorName << "' not found\n";
                    return ;
                }
                Client* operatorClient = searchForClientref(operatorName);
                if (operatorClient)
                {
                    channel.getoperators().insert(operatorClient);
                }
                else
                {
                    std::cout << "Client with nickname '" << operatorName << "' not found\n";
                    return ;
                }
            }
            else
            {
                // Disable channel operator mode
                ;
            }
        }
        // · l: Set/remove the user limit to channel
        if (helper.find('l') != helper.end())
        {
            std::cout << "Setting user limit mode: " << (helper['l'] == '+' ? "ON" : "OFF") << "\n";
            if (helper['l'] == '+')
            {
                // Enable user limit mode
                size_t userLimit;
                if (splitMode.size() < 4)
                {
                    std::cout << "User limit not provided\n";
                    return ;
                }
                try 
                {
                    
                    userLimit = std::strtoul(splitMode[3].c_str(), NULL, 10);
                    if (userLimit == 0)
                    {
                        std::cout << "User limit must be greater than 0\n";
                        return ;
                    }
                }
                catch (const std::invalid_argument& e) 
                {
                    std::cout << "Invalid user limit\n";
                    return ;
                }
                channel.setUserLimit(userLimit);
            }
            else
            {
                // Disable user limit mode
                channel.setUserLimit(0);
            }
        }
    }
    else
    {
        // mode just for channel not for client
        // put the error right here
        return ;
        
    }
}
