#include "parsing.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>



static bool validStr(const std::string& str)
{
    for (int i = 0 ;i < str.size() ; i++)
    {
        if (!std::isprint(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool Parsing::newMessage(const std::string &line, Client &client)
{
    std::vector<std::string> holder;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word)
    {
        // std::cout << word << std::endl;
        holder.push_back(word);
    }
// here is the parcing of the line  before exec of the command

    if (holder[0] == "JOIN")
        join(client, line);
    else if (holder[0] == "MODE")
    {
        mode(client, line);
    }
    else if (holder[0] == "PRVMSG")
    {
        std::cout << holder[0] << std::endl;
    }
    else if (holder[0] == "KICK")
    {
        std::cout << holder[0] << std::endl;
    }
    else if (holder[0] == "INVTE")
    {
        std::cout << holder[0] << std::endl;
    }
    else if (holder[0] == "TOPIC")
    {
        std::cout << holder[0] << std::endl;
    }
    return (true);
}