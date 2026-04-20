#!/bin/bash
echo "\033[0;31m TEST JOIN COMMAND  \033[0m\n"
{
  sleep 0.4;echo "PASS ra\n"
  sleep 0.4;echo "NICK ray\n"
  sleep 0.4;echo "USER ray 0 * :rlamlaik\n"

  sleep 0.4;echo "JOIN #te3st,&rerre,ererer\n"
  sleep 0.4;echo "JOIN #test,&helo,#ULTRA WIN,WIN,WIN\n"
  sleep 0.4;echo "JOIN 0\n"
  sleep 0.4;echo "JOIN #te3st,&rerre,ererer\n"
  sleep 0.4;echo "JOIN #test,&helo,#ULTRA WIN,WIN,WIN\n"


  sleep 0.4;echo "TOPIC #te3st :ray hello eloeihvean fhr s,rg gzldf f,\n"
  sleep 0.4;echo "TOPIC ererer :ray hello eloeihvean fhr s,rg gzldf f,\n"
  sleep 0.4;echo "TOPIC &rerre :ray hello eloeihvean fhr s,rg gzldf f,\n"




  sleep 0.4;echo "KICK #te3st ray\n"
  sleep 0.4;echo "KICK ererer ray\n"
  sleep 0.4;echo "KICK &rerre ray\n"
  sleep 0.4;echo "KICK TTTTTTTTTTTTTTTT ray\n"
  sleep 0.4;echo "KICK 34343434 ray\n"
  sleep 0.4;echo "KICK #1 ray\n"

} | nc 127.0.0.1 1234

