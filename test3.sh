#!/bin/bash
assert(){
    input="$1"
    ./9cc "$input" 
    ./9cc "$input" > test3.s
    echo "_____________________________________"
}
assert  'main(){
    int x;
    x=21;
    return 21;
}'
assert  'main(){
    num=21;
    return num;
}'