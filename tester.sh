#!/bin/bash
{
  sleep 0.2; echo "PASS ra"
  sleep 0.2; echo "NICK rlamlaik"
  sleep 0.2; echo "USER rlamlaik 0 * :rlamlaik"

  sleep 0.7; echo "JOIN #test"
  sleep 0.4; echo "JOIN #test2"

  # Expect: server treats it like PART from all joined channels
  sleep 0.6; echo "JOIN 0"
  sleep 0.6; echo "JOIN wewe"
  sleep 0.6; echo "JOIN 0we"

  sleep 0.6; echo "QUIT :done"
} | nc 127.0.0.1 1234