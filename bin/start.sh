#!/bin/bash

ROOTDIR=`git rev-parse --show-toplevel`

SCENE=$1
PORT=$2

if [ -z "$1" ]
then
    SCENE=${ROOTDIR}/data/scenes/default.json
fi

if [ -z "$2" ]
then
    PORT=8081
fi

./build/bin/web-app $PORT apps/web-app/web $SCENE