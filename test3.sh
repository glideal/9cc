#!/bin/bash
assert(){
    input="$1"
    ./9cc "$input" 
    echo "_____________________________________"
}



assert 'main(){
    return sum(1,2,3,4,5,6)
}
sum(a,b,c,d,e,f){
    return a+b+c+d+e+f;
}'