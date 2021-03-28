#!/bin/sh

FILES="DefaultFiles/"
KEY="DefaultKey"
DEMO="./hash-sigs/demo"

if [ "$#" -gt 3 ]; then
    echo "Too many parameters"
    echo "Parameter 1: Keyname; Parameter 2: Folder with files to sign; Optional Parameter 3: Path to executeable, example ./program/exec"
    exit 1
fi

if [ "$#" -le 1 ]; then
    echo "Using default parameters"
    echo "Parameter 1: Keyname; Parameter 2: Folder with files to sign; Optional Parameter 3: Path to executeable, example ./program/exec"
fi

if [ "$#" -gt 1 ]; then
    KEY="$1"
    FILES="$2"
fi

if [ "$#" -gt 2 ]; then
    DEMO="$3"
fi

echo "Using files in $FILES to sign all files in there"
find $FILES -type f \( -iname "*" ! -iname "*.sig" \) -printf '"%p" ' | xargs -t $DEMO sign $KEY
