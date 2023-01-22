#!/bin/bash
assert(){
    input="$1"
    ./9cc "$input" 
    echo "_____________________________________"
}

assert 'if(2*4==7){if(5+2==7){num=3;}else {num=4;}}else{return 42;}'