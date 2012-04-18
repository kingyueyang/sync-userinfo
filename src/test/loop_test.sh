#!/bin/sh

if [ $# -ne 0 ]; then
  for ((  i = 1 ;  i <= $1;  i++  ))
  do
    echo "running post $i times"
    ./test_user_basic_info &
    #./test_user_header_info &
    #./test_user_education_info &
    #./test_user_employment_info &
  done

else
  echo "Error - Number missing form command line argument"
  echo "Syntax : $0 number"
  echo "Use to print multiplication table for given number"
  exit -1
fi

