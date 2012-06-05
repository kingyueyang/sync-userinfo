#!/bin/sh

ps aux |grep -v "grep" | grep -v "vi" | grep -v "\.c" |grep "sync-userinfo" > /dev/null

if [ $? -eq 0 ]
then
  :
  #echo sync-userinfo is running!
else
  cd `dirname $0`/../src
  ./sync-userinfo
  #echo sync-userinfo restart ok !
fi 
