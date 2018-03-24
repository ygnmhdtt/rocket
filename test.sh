#!/bin/bash

function compile {
  echo "$1" | ./rocket > tmp.s
  if [ ! $? ]; then
    echo "Failed to compile $1"
    exit
  fi
  gcc -o tmp.out driver.c tmp.s
  if [ ! $? ]; then
    echo "Failed to gcc $1"
    exit
  fi
}

function test {
  expected="$1"
  expr="$2"

  compile "$expr"
  result="`./tmp.out`"
  if [ "$result" != "$expected" ]; then
    echo "Test failed: $expected expected but got $result"
    exit
  fi
}

make -s rocket

test 0 0
test 42 42
test abc '"abc"'

rm -f tmp.out tmp.s
echo "All tests passed"
