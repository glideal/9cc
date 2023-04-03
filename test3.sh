#!/bin/bash
assert(){
    input="$1"
    ./9cc "$input" 
    ./9cc "$input" > test3.s
    echo "_____________________________________"
}
assert 'main(){
    int x;
    int*y;
    y=&x;
    *y=21;
    return x;
}'