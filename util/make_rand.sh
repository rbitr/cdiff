#! /bin/bash

for ((i=1;i<=$1;i++)) 
  do
    echo -n $(bc <<<  "scale=5; $RANDOM / 32768") 
    # below is needed if running in zsh
    # https://stackoverflow.com/questions/63544826/unix-shell-why-are-the-same-random-numbers-repeated 
   # actually it's needed in older bash versions as well https://gist.github.com/rbitr/9c68379d3e0b79c9f06eb3f867624576
   RANDOM=$RANDOM
   for ((j=2;j<=$2;j++)) do
   echo -n ", "
   echo -n $(bc <<<  "scale=5; $RANDOM / 32768")
   RANDOM=$RANDOM
  done
  echo

  done
