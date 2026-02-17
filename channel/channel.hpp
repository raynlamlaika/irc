
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include  "../Server/Client.hpp"

class Client;

class Channel : public Client
{
private:
    std::string _name;
    std::string _topic;
    std::string _key;


    std::set<int> _invited; // invated client "fds"
    std::map<int, Client*> _members;
    std::set<Client*> _operators;
    std::vector<Client*> _banned;



    bool _inviteOnly; // +i
    bool _topicRestricted; // +t
    size_t _userLimit; // +l 
    

public:

// make a defulaut constractor
    Channel(const std::string& name);
    ~Channel();
// make a  parameteras 
    Channel(std::string key ,const std::string& name);


    // Basic info
    const std::string& getName() const;
    const std::string& getTopic() const;

    // Membership
    void addClient(Client& c);
    void removeClient(Client& c);
    bool hasClient(Client& c);

    bool hasKey() const;
    std::string getKey() const;
    bool isInviteOnly() const;
    bool isTopicRestricted() const;
    void setInviteOnly(bool inviteOnly);
    bool getInviteOnly() const;
    std::map<int, Client*> getMembers() const;
    bool hasUserLimit() const;
    void  setKey(std::string key);
    void setTopic(std::string key);
    std::map<int, Client*> getmembers ();
    std::set<Client*> getoperators() const;
    void setUserLimit(size_t limit);
    bool isBanned(const Client& client) const;
    bool isOperator(const Client& client) const;
    // bool addOperator(const Client& client) const;
};


#endif