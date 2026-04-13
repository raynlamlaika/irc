#!/bin/bash
echo "\033[0;31m TEST JOIN COMMAND  \033[0m\n"
{
  sleep 0.2; echo "PASS ra"
  sleep 0.2; echo "NICK rlamlaik"
  sleep 0.2; echo "USER rlamlaik 0 * :rlamlaik"

  sleep 0.1; echo "JOIN #test"
  sleep 0.1; echo "JOIN #test2"

  # Expect: server treats it like PART from all joined channels
  sleep 0.6; echo "JOIN 0"
  sleep 0.6; echo "JOIN dd"
  sleep 0.6; echo "JOIN 0we"
  sleep 0.6; echo "JOIN #UltraW,#Green,#UAR,#####ra wi,egals"
  sleep 0.6; echo "JOIN #444444444444444444444444444444444444"

  

  # sleep 0.6; echo "QUIT :done"
  #create new client in the server

  sleep 0.1; echo "KICK #UltraW"
  sleep 0.1; echo "KICK #UltraW ray"


  sleep 0.1; echo "Test TOPIC ------------"
  sleep 0.1; echo "TOPIC #UltraW :New topic for UltraW"
  sleep 0.1; echo "TOPIC #UltraW"
  sleep 0.1; echo "TOPIC #UltraW :"
  sleep 0.1; echo "TOPIC #UltraW"


  sleep 0.1; echo "TEST MOOOOODE ---"

  sleep 0.1; echo "MODE #UltraW +o rlamlaik"
  sleep 0.1; echo "MODE #UltraW +l 1234"
  sleep 0.1; echo "MODE #UltraW -l"
  sleep 0.1; echo "MODE #UltraW +t rerererererererer"
  sleep 0.1; echo "MODE #UltraW +o rlamlaik"
  sleep 0.1; echo "MODE #UltraW -o rlamlaik"

} | nc 127.0.0.1 1234

