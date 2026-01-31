#include "parsing.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>


std::map<std::string, Channel>& Parsing::Getchannel()
{
    return _channels;
}

void Parsing::add_Channel(const Channel& channel)
{
    const std::string& name = channel.getName();

    if (_channels.find(name) != _channels.end())
        return;

    _channels.insert(std::make_pair(name, channel));
}

std::vector<std::string> parceCammandJoin(std::string line)
{
    // separate the names and keys to loop out of them

    std::vector<std::string> holder;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word)
    {
        // std::cout << word << std::endl;
        holder.push_back(word);
    }
    if (holder.size() >= 1)
    {
        std::string command = holder[0];
        if (command  != "JOIN")
        {
            std::cout  << "not the join cammand\n";
            return holder;
        }
    }
    if (holder.size() >= 2)
        std::string names = holder[1];
    if (holder.size() >= 3)
        std::string names = holder[2];
    return holder;
}

std::vector<std::string> Parsing::HelperSplit(std::string line, char del)
{
    std::vector<std::string> result;
    std::string token;
    std::string::size_type start = 0;
    std::string::size_type pos;

    while ((pos = line.find(del, start)) != std::string::npos)
    {
        result.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }
    result.push_back(line.substr(start));

    return result;
}

std::map<std::string, std::string> key_name(std::vector<std::string> line)
{
    std::vector<std::string> names;
    std::vector<std::string> keys;
    std::map<std::string, std::string> holder;
    if (!line[1].empty())
    {
        names = Parsing::HelperSplit(line[1], ',');
    }
    if (!line[2].empty())
    {
        keys = Parsing::HelperSplit(line[2], ',');
    }

    for(int i =  0 ; names.size() > i ; i++)
    {
        if (keys[i].empty())
            holder.insert(std::make_pair(names[i], ""));
        else if (!keys[i].empty())
            holder.insert(std::make_pair(names[i], keys[i]));
        
        // std::cout << " so this is the name :" << names[i]<<" and  key: " << keys[i] << "\n";
    }
    return holder;
}

void Parsing::join(Client &client,std::string line)
{
    std::map<std::string, Channel>& chs = Getchannel();

    
    // first parce the line 
    std::vector<std::string> parced = parceCammandJoin(line);

    std::map<std::string, std::string> NamesKeys = key_name(parced);
    std::map<std::string, std::string>::iterator helper;

    for (helper = NamesKeys.begin(); helper != NamesKeys.end(); ++helper)
    {
        // std::cout << " this is the name:" << helper->first << "and key" << helper->second << "\n"; 
        std::map<std::string, Channel>::iterator it = chs.find(helper->first);
        if (it == chs.end())
        {
            Channel NewChanel(helper->first);
            add_Channel(NewChanel);
            if (helper->second != "")
                NewChanel.setKey(helper->second );
            it = chs.find(line);
            if (it != chs.end())
                it->second.addClient(client);
        }
        else
        {
            Channel &channel = it->second;
            if (helper->second == "")
            {
                // check channel if the  key is set 
                if (channel.hasKey())
                    it->second.addClient(client);  // Add client to existing channel
                else
                    std::cout << " you need a keey SADI\n";
            }
            else if (helper->second != "") // entre with the key
            {

                if (channel.hasKey())
                {
                    std::string passedKey = channel.getKey();
                    if (passedKey == helper->second)
                        it->second.addClient(client); 
                    else
                        std::cout << " you need a keey SADI\n";
                }
                it->second.addClient(client);  // Add client to existing channel
                std::cout << " you need no key may i will reject this SADI\n";
            }
        }
    }
    std::cout << "\t\t\t------this is all of the channels------\t\t\t\n";
    std::map<std::string, Channel>::const_iterator it1;
    for (it1 = chs.begin(); it1 != chs.end(); ++it1)
    {
        std::cout << "Channel : "<< it1->first<< ": "<< it1->second.getName()<< std::endl;
    }
}

