
#include "parsing.hpp"

// bool isStringPrintable(const std::string& str) {
//     for (size_t i = 0; i < str.length(); ++i) {
//         if (!std::isprint(str[i])) {
//             return false;
//         }
//     }
//     return true;
// }

// bool Parsing::cheack(std::string& value)
// {
//     return false;
// }

void Parsing::user(Client &client, std::string line)
{
    std::string cmd, value;
    std::stringstream ss(line);
    ss >> cmd >> value;
    if (!client.getPass())
        client.sendMsg(": You may not reregister\r\n");
    else
    {
        if (value.empty())
            client.sendMsg(": Not enough parameters\r\n");
        else if (!client.getName().empty())
            client.sendMsg(": You may not reregister\r\n");
        else
            client.setname(value);
    }
}


