#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>


#define ERR_NEEDMOREPARAMS "461"
#define ERR_NOSUCHCHANNEL  "403"
#define ERR_TOOMANYCHANNELS "405"
#define ERR_BADCHANNELKEY  "475"
#define ERR_BANNEDFROMCHAN "474"
#define ERR_CHANNELISFULL  "471"
#define ERR_INVITEONLYCHAN "473"
#define ERR_BADCHANMASK    "476"

#define RPL_TOPIC          "332"
#define RPL_TOPICWHOTIME   "333"
#define RPL_NAMREPLY       "353"
#define RPL_ENDOFNAMES     "366"
#define RPL_NOTOPIC         "331"

#define MSG_RPL_INVITELIST(server, nick, channel) \
    (std::string(":") + server + " " + "336" + " " + nick + " " + channel + " :Invite list\r\n")
#define MSG_INVITE(server, nick, target, channel) \
    (std::string(":") + nick + "!" + "user@host" + " INVITE " + target + " " + channel + "\r\n")
#define MSG_ERR_UNKNOWNCOMMAND(server, nick, cmd) \
    (std::string(":") + server + " " + "421" + " " + nick + " " + cmd + " :Unknown command\r\n")
#define MSG_RPL_NOTOPIC(server, nick, channel) \
    (std::string(":") + server + " " + RPL_NOTOPIC + " " + nick + " " + channel + " :No topic is set\r\n")

#define MSG_ERR_NOTREGISTERED(server, nick) \
    (std::string(":") + server + " " + "451" + " " + nick + " :You have not registered\r\n")
    
#define MSG_ERR_NEEDMOREPARAMS(server, nick, cmd) \
    (std::string(":") + server + " " + ERR_NEEDMOREPARAMS + " " + nick + " " + cmd + " :Not enough parameters\r\n")

#define MSG_ERR_NOSUCHCHANNEL(server, nick, channel) \
    (std::string(":") + server + " " + ERR_NOSUCHCHANNEL + " " + nick + " " + channel + " :No such channel\r\n")

#define MSG_ERR_TOOMANYCHANNELS(server, nick, channel) \
    (std::string(":") + server + " " + ERR_TOOMANYCHANNELS + " " + nick + " " + channel + " :You have joined too many channels\r\n")

#define MSG_ERR_BADCHANNELKEY(server, nick, channel) \
    (std::string(":") + server + " " + ERR_BADCHANNELKEY + " " + nick + " " + channel + " :Cannot join channel (+k)\r\n")

#define MSG_ERR_BANNEDFROMCHAN(server, nick, channel) \
    (std::string(":") + server + " " + ERR_BANNEDFROMCHAN + " " + nick + " " + channel + " :Cannot join channel (+b)\r\n")

#define MSG_ERR_CHANNELISFULL(server, nick, channel) \
    (std::string(":") + server + " " + ERR_CHANNELISFULL + " " + nick + " " + channel + " :Cannot join channel (+l)\r\n")

#define MSG_ERR_INVITEONLYCHAN(server, nick, channel) \
    (std::string(":") + server + " " + ERR_INVITEONLYCHAN + " " + nick + " " + channel + " :Cannot join channel (+i)\r\n")

#define MSG_ERR_BADCHANMASK(server, nick, channel) \
    (std::string(":") + server + " " + ERR_BADCHANMASK + " " + nick + " " + channel + " :Bad Channel Mask\r\n")


#define MSG_RPL_TOPIC(server, nick, channel, topic) \
    (std::string(":") + server + " " + RPL_TOPIC + " " + nick + " " + channel + " :" + topic + "\r\n")

#define MSG_RPL_TOPICWHOTIME(server, nick, channel, user, time) \
    (std::string(":") + server + " " + RPL_TOPICWHOTIME + " " + nick + " " + channel + " " + user + " " + time + "\r\n")

#define MSG_RPL_NAMREPLY(server, nick, symbol, channel, names) \
    (std::string(":") + server + " " + RPL_NAMREPLY + " " + nick + " " + symbol + " " + channel + " :" + names + "\r\n")

#define MSG_RPL_ENDOFNAMES(server, nick, channel) \
    (std::string(":") + server + " " + RPL_ENDOFNAMES + " " + nick + " " + channel + " :End of /NAMES list\r\n")



#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOTONCHANNEL     "442"


#define MSG_ERR_CHANOPRIVSNEEDED(server, nick, channel) \
    (std::string(":") + server + " " + ERR_CHANOPRIVSNEEDED + " " + nick + " " + channel + " :You're not channel operator\r\n")

#define MSG_ERR_USERNOTINCHANNEL(server, nick, target, channel) \
    (std::string(":") + server + " " + ERR_USERNOTINCHANNEL + " " + nick + " " + target + " " + channel + " :They aren't on that channel\r\n")

#define MSG_ERR_NOTONCHANNEL(server, nick, channel) \
    (std::string(":") + server + " " + ERR_NOTONCHANNEL + " " + nick + " " + channel + " :You're not on that channel\r\n")




#define MSG_KICK_WITHOUT_REASON(server, nick, user, channel) \
    (std::string(":") + nick + "!" + "user@host" + " KICK " + channel + " " + user + "\r\n")

#define MSG_KICK(server, nick, user, channel, reason) \
    (std::string(":") + nick + "!" + "user@host" + " KICK " + channel + " " + user + " :" + reason + "\r\n")



#define ERR_NOSUCHNICK      "401"
#define ERR_NOSUCHSERVER    "402"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_TOOMANYTARGETS  "407"
#define ERR_NORECIPIENT     "411"
#define ERR_NOTEXTTOSEND    "412"
#define ERR_NOTOPLEVEL      "413"
#define ERR_WILDTOPLEVEL    "414"

#define RPL_AWAY            "301"


#define MSG_ERR_NOSUCHNICK(server, nick, target) \
    (std::string(":") + server + " " + ERR_NOSUCHNICK + " " + nick + " " + target + " :No such nick/channel\r\n")

#define MSG_ERR_NOSUCHSERVER(server, nick, target) \
    (std::string(":") + server + " " + ERR_NOSUCHSERVER + " " + nick + " " + target + " :No such server\r\n")

#define MSG_ERR_CANNOTSENDTOCHAN(server, nick, channel) \
    (std::string(":") + server + " " + ERR_CANNOTSENDTOCHAN + " " + nick + " " + channel + " :Cannot send to channel\r\n")

#define MSG_ERR_TOOMANYTARGETS(server, nick, target) \
    (std::string(":") + server + " " + ERR_TOOMANYTARGETS + " " + nick + " " + target + " :Too many targets\r\n")

#define MSG_ERR_NORECIPIENT(server, nick, command) \
    (std::string(":") + server + " " + ERR_NORECIPIENT + " " + nick + " :No recipient given (" + command + ")\r\n")

#define MSG_ERR_NOTEXTTOSEND(server, nick) \
    (std::string(":") + server + " " + ERR_NOTEXTTOSEND + " " + nick + " :No text to send\r\n")

#define MSG_ERR_NOTOPLEVEL(server, nick, mask) \
    (std::string(":") + server + " " + ERR_NOTOPLEVEL + " " + nick + " " + mask + " :No toplevel domain specified\r\n")

#define MSG_ERR_WILDTOPLEVEL(server, nick, mask) \
    (std::string(":") + server + " " + ERR_WILDTOPLEVEL + " " + nick + " " + mask + " :Wildcard in toplevel domain\r\n")



#define MSG_RPL_AWAY(server, nick, target, message) \
    (std::string(":") + server + " " + RPL_AWAY + " " + nick + " " + target + " :" + message + "\r\n")


#define RPL_INVITING        "341"
#define ERR_USERONCHANNEL   "443"

#define MSG_RPL_INVITING(server, nick, target, channel) \
    (std::string(":") + server + " " + RPL_INVITING + " " + nick + " " + target + " " + channel + "\r\n")

#define MSG_ERR_USERONCHANNEL(server, nick, target, channel) \
    (std::string(":") + server + " " + ERR_USERONCHANNEL + " " + nick + " " + target + " " + channel + " :is already on channel\r\n")





#endif