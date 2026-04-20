#!/bin/bash
echo "\033[0;31m TEST JOIN COMMAND  \033[0m\n"
{
  sleep 0.4;echo "PASS ra\n"
  sleep 0.4;echo "NICK rlamlaik\n"
  sleep 0.4;echo "USER 4ayon 0 * :rlamlaik\n"

  sleep 0.4;echo "JOIN #te3st\n"
  sleep 0.4;echo "TOPIC #te3st :hello sahbi lghzaaal\n"
  sleep 0.4;echo "KICK #te3st ray,ra,ra,ra,er,erers,ef\n"
  sleep 0.4;echo "KICK 3st ray,ra,ra,ra,er,erers,ef\n"
  sleep 0.4;echo "KICK 3st ray,ra,ra,ra,er,erers,ef\n"
  sleep 0.4;echo "KICK  ray,ra,ra,ra,er,erers,ef\n"
  sleep 0.4;echo "KICK &te3st ray,ra,ra,ra,er,erers,ef\n"
  sleep 0.4;echo "KICK +te3st ray,ra,ra,ra,er,erers,ef\n"
  sleep 0.4;echo "KICK -te3st ray,ra,ra,ra,er,erers,ef\n"
  sleep 0.4;echo "KICK te3st ray,ra,ra,ra,er,erers,ef\n"
  sleep 0.4;echo "KICK #te3st ray,ra,ra,ra,er,erers,ef\n"
  sleep 0.4;echo "KICK #te3st ray,ra,ra,ra,er,erers,ef\n"

} | nc 127.0.0.1 1234

