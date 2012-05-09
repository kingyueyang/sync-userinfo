#!/bin/sh

while true ;
do
  ps aux |grep -v "grep" | grep -v "vim" | grep -v "\.c" |grep "sync-userinfo" > /dev/null

  if [ $? -eq 0 ]
  then
    echo sync-userinfo is running!
  else
    `dirname $0`/../src/sync-userinfo
    echo sync-userinfo restart ok !
  fi 

  SEC=`date +%S`
  echo $SEC
  MOD=`expr $SEC % 3`
  if [ $MOD == 0 ]
  then
      echo "ok"
      sleep 1
  fi

done
