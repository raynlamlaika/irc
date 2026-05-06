#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

/*
** =========================
** NUMERIC CODES
** =========================
*/
#define ERR_NEEDMOREPARAMS "461"
#define ERR_NOSUCHCHANNEL  "403"
#define ERR_TOOMANYCHANNELS "405"
#define ERR_BADCHANNELKEY  "475"
#define ERR_BANNEDFROMCHAN "474"
#define ERR_CHANNELISFULL  "471"
#define ERR_INVITEONLYCHAN "473"
#define ERR_BADCHANMASK    "476"

#define ERR_CHANOPRIVSNEEDED "482"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOTONCHANNEL     "442"
#define ERR_USERONCHANNEL    "443"

#define ERR_NOSUCHNICK      "401"
#define ERR_NOSUCHSERVER    "402"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_TOOMANYTARGETS  "407"
#define ERR_NORECIPIENT     "411"
#define ERR_NOTEXTTOSEND    "412"
#define ERR_NOTOPLEVEL      "413"
#define ERR_WILDTOPLEVEL    "414"

#define RPL_TOPIC          "332"
#define RPL_TOPICWHOTIME   "333"
#define RPL_NAMREPLY       "353"
#define RPL_ENDOFNAMES     "366"
#define RPL_NOTOPIC        "331"
#define RPL_AWAY           "301"
#define RPL_INVITING       "341"

/*
** =========================
** GENERIC ERRORS
** =========================
*/
#define MSG_ERR_NEEDMOREPARAMS(server, nick, cmd) \
    (std::string(":") + server + " 461 " + nick + " " + cmd + " :Not enough parameters\r\n")

#define MSG_ERR_NOTREGISTERED(server, nick) \
    (std::string(":") + server + " 451 " + nick + " :You have not registered\r\n")

#define MSG_ERR_UNKNOWNCOMMAND(server, nick, cmd) \
    (std::string(":") + server + " 421 " + nick + " " + cmd + " :Unknown command\r\n")

/*
** =========================
** CHANNEL ERRORS
** =========================
*/
#define MSG_ERR_NOSUCHCHANNEL(server, nick, channel) \
    (std::string(":") + server + " 403 " + nick + " " + channel + " :No such channel\r\n")

#define MSG_ERR_TOOMANYCHANNELS(server, nick, channel) \
    (std::string(":") + server + " 405 " + nick + " " + channel + " :You have joined too many channels\r\n")

#define MSG_ERR_BADCHANNELKEY(server, nick, channel) \
    (std::string(":") + server + " 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n")

#define MSG_ERR_BANNEDFROMCHAN(server, nick, channel) \
    (std::string(":") + server + " 474 " + nick + " " + channel + " :Cannot join channel (+b)\r\n")

#define MSG_ERR_CHANNELISFULL(server, nick, channel) \
    (std::string(":") + server + " 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n")

#define MSG_ERR_INVITEONLYCHAN(server, nick, channel) \
    (std::string(":") + server + " 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n")

#define MSG_ERR_BADCHANMASK(server, nick, channel) \
    (std::string(":") + server + " 476 " + nick + " " + channel + " :Bad Channel Mask\r\n")

#define MSG_ERR_CHANOPRIVSNEEDED(server, nick, channel) \
    (std::string(":") + server + " 482 " + nick + " " + channel + " :You're not channel operator\r\n")

#define MSG_ERR_USERNOTINCHANNEL(server, nick, target, channel) \
    (std::string(":") + server + " 441 " + nick + " " + target + " " + channel + " :They aren't on that channel\r\n")

#define MSG_ERR_NOTONCHANNEL(server, nick, channel) \
    (std::string(":") + server + " 442 " + nick + " " + channel + " :You're not on that channel\r\n")

#define MSG_ERR_USERONCHANNEL(server, nick, target, channel) \
    (std::string(":") + server + " 443 " + nick + " " + target + " " + channel + " :is already on channel\r\n")

/*
** =========================
** CHANNEL REPLIES
** =========================
*/
#define MSG_RPL_TOPIC(server, nick, channel, topic) \
    (std::string(":") + server + " 332 " + nick + " " + channel + " :" + topic + "\r\n")

#define MSG_RPL_NOTOPIC(server, nick, channel) \
    (std::string(":") + server + " 331 " + nick + " " + channel + " :No topic is set\r\n")

#define MSG_RPL_TOPICWHOTIME(server, nick, channel, user, time) \
    (std::string(":") + server + " 333 " + nick + " " + channel + " " + user + " " + time + "\r\n")

#define MSG_RPL_NAMREPLY(server, nick, symbol, channel, names) \
    (std::string(":") + server + " 353 " + nick + " " + symbol + " " + channel + " :" + names + "\r\n")

#define MSG_RPL_ENDOFNAMES(server, nick, channel) \
    (std::string(":") + server + " 366 " + nick + " " + channel + " :End of /NAMES list\r\n")
#define RPL_MODE(server, nick, channel, mode, target) \
    (std::string(":") + nick + " MODE " + channel + " " + mode + \
    (target.empty() ? "" : " " + target) + "\r\n")
/*
** =========================
** MESSAGING ERRORS
** =========================
*/
#define MSG_ERR_NOSUCHNICK(server, nick, target) \
    (std::string(":") + server + " 401 " + nick + " " + target + " :No such nick/channel\r\n")

#define MSG_ERR_NOSUCHSERVER(server, nick, target) \
    (std::string(":") + server + " 402 " + nick + " " + target + " :No such server\r\n")

#define MSG_ERR_CANNOTSENDTOCHAN(server, nick, channel) \
    (std::string(":") + server + " 404 " + nick + " " + channel + " :Cannot send to channel\r\n")

#define MSG_ERR_TOOMANYTARGETS(server, nick, target) \
    (std::string(":") + server + " 407 " + nick + " " + target + " :Too many targets\r\n")

#define MSG_ERR_NORECIPIENT(server, nick, command) \
    (std::string(":") + server + " 411 " + nick + " :No recipient given (" + command + ")\r\n")

#define MSG_ERR_NOTEXTTOSEND(server, nick) \
    (std::string(":") + server + " 412 " + nick + " :No text to send\r\n")

#define MSG_ERR_NOTOPLEVEL(server, nick, mask) \
    (std::string(":") + server + " 413 " + nick + " " + mask + " :No toplevel domain specified\r\n")

#define MSG_ERR_WILDTOPLEVEL(server, nick, mask) \
    (std::string(":") + server + " 414 " + nick + " " + mask + " :Wildcard in toplevel domain\r\n")

#define MSG_RPL_AWAY(server, nick, target, message) \
    (std::string(":") + server + " 301 " + nick + " " + target + " :" + message + "\r\n")

/*
** =========================
** INVITE / KICK
** =========================
*/
#define MSG_RPL_INVITING(server, nick, target, channel) \
    (std::string(":") + server + " 341 " + nick + " " + target + " " + channel + "\r\n")

#define MSG_INVITE(nick, user, host, target, channel) \
    (std::string(":") + nick + "!" + user + "@" + host + " INVITE " + target + " " + channel + "\r\n")

#define MSG_KICK(nick, user, host, target, channel, reason) \
    (std::string(":") + nick + "!" + user + "@" + host + " KICK " + channel + " " + target + " :" + reason + "\r\n")

#define MSG_KICK_WITHOUT_REASON(nick, user, host, target, channel) \
    (std::string(":") + nick + "!" + user + "@" + host + " KICK " + channel + " " + target + " :Kicked\r\n")



/*
** =========================
** MODE NUMERICS
** =========================
*/
#define RPL_UMODEIS        "221"
#define RPL_CHANNELMODEIS  "324"
#define RPL_CREATIONTIME   "329"

#define ERR_USERSDONTMATCH "502"
#define ERR_UMODEUNKNOWNFLAG "501"

/*
** =========================
** MODE ERRORS
** =========================
*/

#define MSG_ERR_USERSDONTMATCH(server, nick) \
    (std::string(":") + server + " 502 " + nick + " :Cannot change mode for other users\r\n")

#define MSG_ERR_UMODEUNKNOWNFLAG(server, nick) \
    (std::string(":") + server + " 501 " + nick + " :Unknown MODE flag\r\n")

/*
** =========================
** MODE REPLIES
** =========================
*/

#define MSG_ERR_UNKNOWNMODE(server, nick, mode) \
    (std::string(":") + server + " 472 " + nick + " " + mode + " :is unknown mode char to me\r\n")
#define MSG_ERR_INVALIDTOPIC(server, nick, channel) \
    (std::string(":") + server + " 461 " + nick + " " + channel + " :Invalid topic format (contains non-printable characters)\r\n")

/* User mode (e.g. MODE nick) */
#define MSG_RPL_UMODEIS(server, nick, modes) \
    (std::string(":") + server + " 221 " + nick + " " + modes + "\r\n")

/* Channel mode query (MODE #chan) */
#define MSG_RPL_CHANNELMODEIS(server, nick, channel, modes) \
    (std::string(":") + server + " 324 " + nick + " " + channel + " " + modes + "\r\n")

#define MSG_RPL_CREATIONTIME(server, nick, channel, timestamp) \
    (std::string(":") + server + " 329 " + nick + " " + channel + " " + timestamp + "\r\n")

/*
** =========================
** MODE COMMAND BROADCAST
** =========================
** Sent to users when modes change
*/

#define MSG_MODE_T_SET(nick, user, host, channel, topic) \
    (std::string(":") + nick + "!" + user + "@" + host + " MODE " + channel + " +t :" + topic + "\r\n")
/* +i (invite-only) */
#define MSG_MODE_I(nick, user, host, channel, sign) \
    (std::string(":") + nick + "!" + user + "@" + host + " MODE " + channel + " " + sign + "i\r\n")

/* +l (user limit) */
#define MSG_MODE_L_SET(nick, user, host, channel, limit) \
    (std::string(":") + nick + "!" + user + "@" + host + " MODE " + channel + " +l " + limit + "\r\n")

#define MSG_MODE_L_UNSET(nick, user, host, channel) \
    (std::string(":") + nick + "!" + user + "@" + host + " MODE " + channel + " -l\r\n")

/* +k (channel key) */
#define MSG_MODE_K_SET(nick, user, host, channel, key) \
    (std::string(":") + nick + "!" + user + "@" + host + " MODE " + channel + " +k " + key + "\r\n")

#define MSG_MODE_K_UNSET(nick, user, host, channel) \
    (std::string(":") + nick + "!" + user + "@" + host + " MODE " + channel + " -k\r\n")

/* +o (operator) */
#define MSG_MODE_O(nick, user, host, channel, sign, target) \
    (std::string(":") + nick + "!" + user + "@" + host + " MODE " + channel + " " + sign + "o " + target + "\r\n")


// #define MSG_RPL_CHANNELMODEIS(server, nick, channel, modes, params) \
//     (std::string(":") + server + " 324 " + nick + " " + channel + " " + modes + (params.empty() ? "" : " " + params) + "\r\n")


/*
** =========================
** PRIVMSG NUMERICS
** =========================
*/
#define ERR_NOSUCHNICK      "401"
#define ERR_NOSUCHSERVER    "402"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_TOOMANYTARGETS  "407"
#define ERR_NORECIPIENT     "411"
#define ERR_NOTEXTTOSEND    "412"
#define ERR_NOTOPLEVEL      "413"
#define ERR_WILDTOPLEVEL    "414"

#define RPL_AWAY            "301"

/*
** =========================
** PRIVMSG ERRORS
** =========================
*/

#define MSG_ERR_NOSUCHNICK(server, nick, target) \
    (std::string(":") + server + " 401 " + nick + " " + target + " :No such nick/channel\r\n")

#define MSG_ERR_NOSUCHSERVER(server, nick, target) \
    (std::string(":") + server + " 402 " + nick + " " + target + " :No such server\r\n")

#define MSG_ERR_CANNOTSENDTOCHAN(server, nick, channel) \
    (std::string(":") + server + " 404 " + nick + " " + channel + " :Cannot send to channel\r\n")

#define MSG_ERR_TOOMANYTARGETS(server, nick, target) \
    (std::string(":") + server + " 407 " + nick + " " + target + " :Too many targets\r\n")

#define MSG_ERR_NORECIPIENT(server, nick, command) \
    (std::string(":") + server + " 411 " + nick + " :No recipient given (" + command + ")\r\n")

#define MSG_ERR_NOTEXTTOSEND(server, nick) \
    (std::string(":") + server + " 412 " + nick + " :No text to send\r\n")

#define MSG_ERR_NOTOPLEVEL(server, nick, mask) \
    (std::string(":") + server + " 413 " + nick + " " + mask + " :No toplevel domain specified\r\n")

#define MSG_ERR_WILDTOPLEVEL(server, nick, mask) \
    (std::string(":") + server + " 414 " + nick + " " + mask + " :Wildcard in toplevel domain\r\n")

/*
** =========================
** PRIVMSG REPLY
** =========================
*/

#define MSG_RPL_AWAY(server, nick, target, message) \
    (std::string(":") + server + " 301 " + nick + " " + target + " :" + message + "\r\n")

/*
** =========================
** PRIVMSG DELIVERY (not numeric)
** =========================
*/

#define MSG_PRIVMSG(nick, user, host, target, message) \
    (std::string(":") + nick + "!" + user + "@" + host + " PRIVMSG " + target + " :" + message + "\r\n")
    
#endif