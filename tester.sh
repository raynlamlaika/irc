#!/bin/bash
echo "\033[0;31m TEST JOIN COMMAND  \033[0m\n"
{
  sleep 0.2;echo "PASS ra\n"
  sleep 0.2;echo "NICK r2lamlaik\n"
  sleep 0.2;echo "USER 42ayon 0 * :rlamlaik\n"


  sleep 0.2;echo "JOIN #test \n"



} | nc 127.0.0.1 1234

