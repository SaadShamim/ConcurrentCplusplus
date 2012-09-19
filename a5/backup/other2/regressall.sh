#!/bin/bash

make phil

a="./phil"
b="./phil_INT_64"

ps="2 3 4 5 6"
ns="10 20"
seeds="1 2 3 4 5 6 1000 2000 3000 4000"

for p in $ps; do
    for n in $ns; do
        for seed in $seeds; do
            rm a b 2> /dev/null
            ./regress $a "$p $n $seed" $b "$p $n $seed" ""
            if [ $? -ne 0 ] ; then
                echo "Failed with $p $n $seed" > a
                echo "Failed with $p $n $seed" > b
                $a $p $n $seed >> a 2>err
                $b $p $n $seed >> b
                vimdiff a b
                echo "Waiting one second..."
                sleep 1
            fi
        done
    done
done

#vimdiff a b
