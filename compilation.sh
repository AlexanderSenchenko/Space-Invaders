#!/bin/bash

if !(which gcc >/dev/null)
then
  echo "I require gcc but it's not installed. Aborting."
  echo "Try: sudo apt-get install build-essential"
  exit 1
fi

if !(which make >/dev/null)
then
  echo "I require make but it's not installed. Aborting."
  echo "Try: sudo apt-get install build-essential"
  exit 1
fi

if !(which ncursesw5-config >/dev/null);
then
  echo "I require ncursesw5-config but it's not installed. Aborting."
  echo "sudo apt-get install libncurses5-dev libncursesw5-dev"
  exit 1
fi

make