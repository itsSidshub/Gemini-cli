#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <input phrase>"
    exit 1
fi

input="$*"
json_payload=$(jq -n --arg text "$input and return response as simple text no markdown or anything and dont use '''" '{"contents":[{"parts":[{"text":$text}]}]}')
response=$(curl -H 'Content-Type: application/json' \
    -d "$json_payload" \
    -X POST 'https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key=AIzaSyC0PjI51IXezzgqY5L1Ky7Tjy23VsbyrsY' -s)
if [ $? -eq 0 ]; then
    echo "$response" | jq -r '.candidates[0].content.parts[0].text'
else
    echo "Error making API request"
    exit 1
fi
