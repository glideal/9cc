#!/bin/bash
assert(){
    expected="$1"
    input="$2"

    ./9cc.o "$input" > 9cc.s
    gcc 9cc.s -o 9cc.exe
    ./9cc.exe
    actual="$?"

    if [ "$actual"="$expected" ]; then
        echo "$input >= $actual"
    else
        echo "$input >= $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 0
assert 42 42

echo OK