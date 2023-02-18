#!/bin/bash
assert(){
    input="$1"
    ./9cc "$input" 
    ./9cc "$input" > test3.s
    echo "_____________________________________"
}
# assert  'main(){
#         num=21;
#         return num;
#     }'

# assert 'main(){
#     return sum(1,2,3,4,5,6);
# }
# sum(a,b,c,d,e,f){
#     return a+b+c+d+e+f;
# }'
assert 'main(){
    num=5;
    val=num+sum(1,2,3,4);
    return val;
    } 
    sum(a,b,c,d){
        return a+b+c+d;
    }'