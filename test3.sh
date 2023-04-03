#!/bin/bash
assert(){
    input="$1"
    ./9cc "$input" 
    ./9cc "$input" > test3.s
    echo "_____________________________________"
}
assert 'main(){
    return sum(1,2,3,4,5,6);
}
sum(int a,int b,int c,int d,int e,int f){
    return a+b+c+d+e+f;
}'