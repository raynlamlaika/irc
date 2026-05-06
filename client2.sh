#!/bin/bash
echo -e "\033[0;34m TEST: User 1 Script  \033[0m"
{
  sleep 0.2; echo "PASS ra"
  sleep 0.2; echo "NICK n1"
  sleep 0.2; echo "USER test1 0 * :N1 Client"

  sleep 0.2; echo "JOIN #pele"
  sleep 0.2; echo "MODE #pele +i"   # Make channel invite-only
  sleep 0.2; echo "INVITE n2 #pele" # Invite user 2

  sleep 0.2; echo "TOPIC #pele :Test Topic"
  sleep 2;   echo "QUIT :Leaving"
} | nc 127.0.0.1 1234