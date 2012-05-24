#!/bin/sh

while true ;
do
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
  sleep 1

#SEC=`date +%S`
#MOD=`expr $SEC % 2`
#if [ $MOD == 0 ]
#then
#sleep 1
#fi

done
