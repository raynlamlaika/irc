#!/usr/bin/env bash
set -euo pipefail

BASE_URL="http://localhost:8080/irc"

HOST=127.0.0.1
PORT=6667
PASSWORD=password
NICK=rayn
REALNAME=rayne
CHANNEL="#ultra"
MESSAGE="hello from tester"

echo "1) Connect (POST /irc/connect)"
curl -i -s -X POST "$BASE_URL/connect" \
  -H "Content-Type: application/json" \
  -d "{\"host\": \"$HOST\", \"port\": $PORT, \"password\": \"$PASSWORD\", \"nickname\": \"$NICK\", \"realName\": \"$REALNAME\" }"

echo -e "\n2) Status (GET /irc/status)"
curl -i -s "$BASE_URL/status"

echo -e "\n3) Join channel (POST /irc/join)"
curl -i -s -X POST "$BASE_URL/join" \
  -H "Content-Type: application/json" \
  -d "{\"channel\": \"$CHANNEL\" }"

echo -e "\n4) Send message (POST /irc/message)"
curl -i -s -X POST "$BASE_URL/message" \
  -H "Content-Type: application/json" \
  -d "{\"channel\": \"$CHANNEL\", \"message\": \"$MESSAGE\" }"

echo -e "\n5) Connect+Join+Send (POST /irc/connect-message)"
curl -i -s -X POST "$BASE_URL/connect-message" \
  -H "Content-Type: application/json" \
  -d "{\"host\": \"$HOST\", \"port\": $PORT, \"password\": \"$PASSWORD\", \"nickname\": \"$NICK\", \"realName\": \"$REALNAME\", \"channel\": \"$CHANNEL\", \"message\": \"$MESSAGE\" }"

echo -e "\n6) Subscribe to events for 5 seconds (GET /irc/events)"
# Use timeout if available to limit SSE listening
if command -v timeout >/dev/null 2>&1; then
  timeout 5s curl -N -s "$BASE_URL/events"
else
  # curl --max-time will close after some seconds
  curl --max-time 5 -N -s "$BASE_URL/events" || true
fi

echo -e "\n7) History (GET /irc/history)"
curl -i -s "$BASE_URL/history"

echo -e "\n8) Disconnect (POST /irc/disconnect)"
curl -i -s -X POST "$BASE_URL/disconnect"

echo -e "\nDone."
