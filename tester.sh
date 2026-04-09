#!/bin/bash

{
    sleep 0.2
    echo "PASS ra"
    sleep 0.2
    echo "NICK rlamlaik"
    sleep 0.2
    echo "USER rlamlaik 0 * :rlamlaik"
    sleep 0.2
    echo "JOIN"
    sleep 0.2
    echo "JOIN #test"
    sleep 0.2
    echo "KICK #test rlamlaik :You are kicked"
} | nc 127.0.0.1 1234