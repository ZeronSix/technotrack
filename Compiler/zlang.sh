#!/bin/sh

./bin/zcc $1 > ./bin/tmp.zasm
if [-eq $? 0]
  then
    ./bin/zasm ./bin/tmp.zasm $2
