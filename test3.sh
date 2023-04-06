#!/bin/bash
assert(){
    input="$1"
    ./9cc "$input" 
    ./9cc "$input" > test3.s
    echo "_____________________________________"
}
assert 'main(){
    int a;
    int b;
    a=12;
    b=25;
    int *p;
    p=&a;
    p=p+1;
    return *p;
}'