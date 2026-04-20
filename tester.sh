#!/bin/bash
echo "\033[0;31m TEST JOIN COMMAND  \033[0m\n"
{
  sleep 0.4;echo "PASS ra\n"
  sleep 0.4;echo "NICK rlamlaik\n"
  sleep 0.4;echo "USER 4ayon 0 * :rlamlaik\n"

  sleep 0.4;echo "JOIN #te3st\n"
  sleep 0.4;echo "JOIN #ra\n"
  sleep 0.4;echo "MODE #te3st +t :hello sa7bi rayane\n"
  # sleep 0.4;echo "MODE #te3st +i\n"
  # sleep 0.4;echo "INVITE raw #te3st\n"
  # sleep 0.4;echo "JOIN #ewewe1 \n"
  # sleep 0.4;echo "JOIN #ewewe2 \n"
  # sleep 0.4;echo "JOIN #ewewe31 \n"
  # sleep 0.4;echo "JOIN #ewewe14 \n"
  # sleep 0.4;echo "JOIN #ewewe14 \n"
  # sleep 0.4;echo "JOIN 0 \n"
  # sleep 0.4;echo "JOIN #ee \n"
  # sleep 0.4;echo "JOIN #0 \n"


  # sleep 0.1; echo "JOIN #test2"


# # ------------------- to be fixwed
#   # Expect: server treats it like PART from all joined channels
#   sleep 0.6; echo "JOIN #" # need to fix
#   sleep 0.6; echo "JOIN #dsd sdsd sds d" ## if > 3 
#   sleep 0.6; echo "JOIN # ,4232" ## if > 3 
#   sleep 0.6; echo "JOIN 0we"
#   sleep 0.6; echo "JOIN #UltraW,#Green,#UAR,#####ra wi,egals"
#   sleep 0.6; echo "JOIN #444444444444444444444444444444444444"
  
# # MODE #qq +t :chanelltopic is l7wa
# # TOPIC #qq
# # ray #qq ::chanelltopic

# #   TOPIC #test :; Clearing the topic on "#test"



#   # sleep 0.6; echo "QUIT :done"
#   #create new client in the server

#   sleep 0.1; echo "KICK  #qq rai" ## check in jhere is that i need to send any msg in here
#   sleep 0.1; echo "KICK #UltraW ray"


#   sleep 0.1; echo "Test TOPIC ------------"
#   sleep 0.1; echo "TOPIC #UltraW :New topic for UltraW"
#   sleep 0.1; echo "TOPIC #UltraW"
#   sleep 0.1; echo "TOPIC #UltraW :"
#   sleep 0.1; echo "TOPIC #UltraW"





  # sleep 0.1; echo "JOIN #UltraW ray"
  # sleep 0.1; echo "ls ls"
  # sleep 0.1; echo "ls ls"
  # sleep 0.1; echo "ls ls"
  # sleep 0.1; echo "TOPIC #UltraW :chanelltopic is l7wa TTTTTTTTTTTTTTTTTTTTTTTTT"
  # sleep 0.1; echo "TOPIC #UltraW"
  # sleep 0.2; echo "MODE #UltraW +t "
  # sleep 0.2; echo "TOPIC #UltraW"



  # sleep 0.1; echo "MODE #UltraW +o rlamlaik"
  # sleep 0.1; echo "MODE #UltraW +o rlamlaik"
  # sleep 0.1; echo "MODE #UltraW +l 1234"
  # sleep 0.1; echo "MODE #UltraW -l"
  # sleep 0.1; echo "MODE #UltraW +t rerererererererer"
  # sleep 0.1; echo "MODE #UltraW +o rlamlaik"
  # sleep 0.1; echo "MODE #UltraW -o rlamlaik"

} | nc 127.0.0.1 1234

