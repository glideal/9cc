#!/bin/bash
assert(){
    expected="$1"
    input="$2"

    ./9cc "$input" > tmp.s
    gcc tmp.s -o tmp.exe
    ./tmp.exe
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
    echo "__________________________________________________________"
 }
#int
assert 42 'main(){
    int x;
    x=42;
    return x;
}'


assert 42 'main() return 42;'
assert 42 'main() return 45+3-6;'
assert 21 'main() return (3*35)/5;'
assert 21 'main() return --21;'
# assert 42 '6*(3+4);'
# assert 42 '(7*30)/5;'
# assert 10 '-10+20;'
# assert 10 '- -10;'
# assert 10 '- - +10;'

assert 0 'main() return 2==3;'
assert 1 'main() return 2!=3;'
assert 1 'main() return 2==2;'
assert 0 'main() return 2!=2;'
# assert 0 '0==1;'
# assert 1 '42==42;'
# assert 1 '0!=1;'
# assert 0 '42!=42;'

assert 1 'main() return 2<3;'
assert 0 'main() return 2>3;'
assert 1 'main() return 2<=3;'
assert 0 'main() return 2>=3;'
# assert 1 '0<1;'
# assert 0 '1<1;'
# assert 0 '2<1;'
# assert 1 '0<=1;'
# assert 1 '1<=1;'
# assert 0 '2<=1;'

# assert 1 '1>0;'
# assert 0 '1>1;'
# assert 0 '1>2;'
# assert 1 '1>=0;'
# assert 1 '1>=1;'
# assert 0 '1>=2;'

assert 42 'main(){
    int x;
    x=42;
    return x;
}'
assert 21 'main(){
    int num;
    num=21;
    return num;
}'
assert 42 'main(){
    int num;
    int mul;
    num=42;
    mul=17;
    return num;
}'
# assert 42 'num=42; return num;'
# assert 42 'num=42;mul=17;return num;'
# assert 42 'num=3;mul=2;return num*(3+4)*mul;'
# assert 42 'num=21;num=42;return num;'
# assert 42 'num=21;num=num+21; return num;'
# assert 42 'num=7*6; return num;'
# assert 42 'num;num=42;return num;'

assert 42 'main(){
        if(2*3>5) return 42;
    }'
assert 42 'main(){
        if(2*3==5)37;
        else if(2*3<5) return 39; 
        else return 42;
    }'
# assert 42 'if(2*3>5)42;'
# assert 42 'if(2*3==5)39; else 42;'
# assert 42 'if(2*3==5)37; else if(2*3<5)39; else 42;'

assert 21 'main(){
    int i;
    i=0;
    while(i<21){
        i=i+1;
    }
    return i;
}'
# assert 42 'num=0;while(num<42)num=num+1;return num;'

assert 42 'main(){
    int num;
    int answer;
    for(num=1;num<=42;num=num+1){
        answer=num;
    }
    return answer;
}'
# assert 42 'for(num=1;num<=42;num=num+1) answer=num;return answer;'
# assert 42 'num=0; for(;num<=42;num=num+1) answer=num;return answer;'

assert 21 'main(){
    int num;
    int val;
    if(2*5>9){
        num=3;
        val=num*7;
        return val;
    }
}'
assert 42 'main(){
    int num;
    if(2*4==7){
        if(5+2==7){
            num=3;
        }else{
            num=4;
        }
    }else{
        return 42;
    }
}'
# assert 42 'if(2*5>9){num=3;val=num*7;return val*2;}'
# assert 42 'if(2+5==7){if(2*5==10){if(8-7>0)return 42;}}'
# assert 42 'if(2*5<9)39; else {num=3;val=num*7;return val*2;}'
# assert 42 'for(i=0;i<5;i=i+1){if(i==2){num=19;}if(i==4){val=23;}}return num+val;'
# assert 42 'num=0;i=0;while(i<42){num=num+1;i=i+1;}return num;'
# assert 42 'if(2*4==7){if(5+2==7){num=3;}else {num=4;}}else{return 42;}'

assert 21 'main(){
    return sum(1,2,3,4,5,6);
}
sum(int a,int b,int c,int d,int e,int f){
    return a+b+c+d+e+f;
}'


assert 15 'main(){
    int num;
    num=5;
    int val;
    val=num+sum(1,2,3,4);
    return val;
} 
sum(int a,int b,int c,int d){
    return a+b+c+d;
}'

assert 47 'main(){
    int x;
    int y;
    int z;
    x=5;
    y=6;
    z=2;
    int a;
    int b;
    a=sum(x,y,z);
    b=val(x,y,z);
    return sub(b,a);
}
sum(int a,int b,int c){
    int sum;
    sum=a+b+c;
    return sum;
}
val(int a,int b,int c){
    int val;
    val=a*b*c;
    return val;
}
sub(int a,int b){
    return a-b;
}'

#fibonacci sequence
assert 8 'main(){
    int c;
    c=6;
    return fib(c);
}
fib(int c){
    int a;
    int b;
    a=1;
    b=1;
    int i;
    for(i=0;i<c-2;i=i+1){
        int tmp;
        tmp=a+b;
        a=b;
        b=tmp;
    }
    return tmp;
}'


assert 3 'main(){
    int x;
    int y;
    int z;
    x=3;
    y=5;
    z=&y+8;
    return *z;
}'

echo OK
