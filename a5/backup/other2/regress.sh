#!/bin/bash

make phil

a="./phil"
b="./phil_INT_64"

p=$1       # philosophers
n=$2       # noodles
seed=$3 # seed

$a $p $n $seed > a 2>err
$b $p $n $seed > b

vimdiff a b
