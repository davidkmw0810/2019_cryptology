/*
 * @file    rsa.c
 * @author  작성자 이름 / 학번
 * @date    작성 일자
 * @brief   mini RSA implementation code
 * @details 세부 설명
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rsa.h"

llint p, q, e, d, n;

llint Mod(llint num, llint n){
    if(num >= n)
        return Mod(num - n, n);
    else
        return num;
}

llint Qu(llint a, llint b){

    llint qu = 0;

    while(a >= b){
        a = a - b;
        qu ++;
    }

    return qu;
}

/*
 * @brief     모듈러 덧셈 연산을 하는 함수.
 * @param     llint a     : 피연산자1.
 * @param     llint b     : 피연산자2.
 * @param     byte op    : +, - 연산자.
 * @param     llint n      : 모듈러 값.
 * @return    llint result : 피연산자의 덧셈에 대한 모듈러 연산 값. (a op b) mod n
 * @todo      모듈러 값과 오버플로우 상황을 고려하여 작성한다.
 */
llint ModAdd(llint a, llint b, byte op, llint n) {

    if(op == '+'){
        if(a + b > n)
            return Mod(a + b - n, n);
        else
            return Mod(a + b, n);
    } else {
        if(a - b > n)
            return Mod(a - b - n, n);
        else
            return Mod(a - b, n);
    }

}

/*
 * @brief      모듈러 곱셈 연산을 하는 함수.
 * @param      llint x       : 피연산자1.
 * @param      llint y       : 피연산자2.
 * @param      llint n       : 모듈러 값.
 * @return     llint result  : 피연산자의 곱셈에 대한 모듈러 연산 값. (a x b) mod n
 * @todo       모듈러 값과 오버플로우 상황을 고려하여 작성한다.
 */
llint ModMul(llint x, llint y, llint n) {

    if(y == 0){
        return 0;
    }
    if(y - ((y >> 1) << 1) == 1){
        return Mod(x + ModMul(x, y >> 1, n) + ModMul(x, y >> 1, n), n);
    } else {
        return Mod(ModMul(x, y >> 1, n) + ModMul(x, y >> 1, n), n);
    }

}

/*
 * @brief      모듈러 거듭제곱 연산을 하는 함수.
 * @param      llint base   : 피연산자1.
 * @param      llint exp    : 피연산자2.
 * @param      llint n      : 모듈러 값.
 * @return     llint result : 피연산자의 연산에 대한 모듈러 연산 값. (base ^ exp) mod n
 * @todo       모듈러 값과 오버플로우 상황을 고려하여 작성한다.
               'square and multiply' 알고리즘을 사용하여 작성한다.
 */
llint ModPow(llint base, llint exp, llint n) {

    if(exp == 0){
        return base;
    } else if(base - ((base >> 1) << 1) == 1){
        return Mod(base*ModPow(base - 1, exp>>1, n), n);
    } else if(base - ((base >> 1) << 1) == 0){
        return Mod(ModPow(ModMul(base, base, n), exp>>1, n), n);
    }

}

/*
 * @brief      입력된 수가 소수인지 입력된 횟수만큼 반복하여 검증하는 함수.
 * @param      llint testNum   : 임의 생성된 홀수.
 * @param      llint repeat    : 판단함수의 반복횟수.
 * @return     llint result    : 판단 결과에 따른 TRUE, FALSE 값.
 * @todo       Miller-Rabin 소수 판별법과 같은 확률적인 방법을 사용하여,
               이론적으로 4N(99.99%) 이상 되는 값을 선택하도록 한다.
 */
bool IsPrime(llint testNum, llint repeat) {

    if(repeat == 0)
        return TRUE;
    llint s = testNum - 1;
    llint d = s;
    while(d - ((d >> 1) << 1) == 0) {d = d >> 1;}
    llint a = (s-2)*WELLRNG512a() + 1;
    llint r = (s-1)*WELLRNG512a();


    if(ModPow(a, d, n) != 1){
        for(int i = 0; i < r; i++){
            if (!(ModPow(a, ModMul(ModPow(2, i, n), d, n), n) != n-1))
                return IsPrime(testNum, repeat - 1);
        }
        return FALSE;
    }

}

/*
 * @brief       모듈러 역 값을 계산하는 함수.
 * @param       llint a      : 피연산자1.
 * @param       llint m      : 모듈러 값.
 * @return      llint result : 피연산자의 모듈러 역수 값.
 * @todo        확장 유클리드 알고리즘을 사용하여 작성하도록 한다.
 */
llint ModInv(llint a, llint m) {
    llint s0 = 1, s1 = 0;
    llint t0 = 0, t1 = 1;
    llint r0 = a, r1 = m;
    llint q0 = 0, q1 = Qu(r0, r1);
    llint buffer;
    llint n = 1;
    while(TRUE){

       buffer = r0;
       r0 = r1;
       r1 = Mod(buffer, r0);
       if(r1 == 0) {break;}

       buffer = q0;
       q0 = q1;
       q1 = Qu(r0, r1);

       buffer = s0;
       s0 = s1;
       s1 = buffer - ModMul(s0, q0, m);

       buffer = t0;
       t0 = t1;
       t1 = buffer - ModMul(t0, q0, m);

       n++;
    }

    return s1;
}

/*
 * @brief     RSA 키를 생성하는 함수.
 * @param     llint *p   : 소수 p.
 * @param     llint *q   : 소수 q.
 * @param     llint *e   : 공개키 값.
 * @param     llint *d   : 개인키 값.
 * @param     llint *n   : 모듈러 n 값.
 * @return    void
 * @todo      과제 안내 문서의 제한사항을 참고하여 작성한다.
 */
void miniRSAKeygen(llint *p, llint *q, llint *e, llint *d, llint *n) {

    do{
        p = (llint)WELLRNG512a()*((2^64 - 1) - (2^53 + 1)) + (2^53 + 1);
    } while(IsPrime(p, 4));

    do{
        q = (llint)WELLRNG512a()*((2^64 - 1) - (2^53 + 1)) + (2^53 + 1);
    } while(IsPrime(q, 4));

    n = p*q;
    llint pi_n = (p - 1)*(q - 1);

    while(GCD(pi_n, e) != 1)
        e = (llint)WELLRNG512a()*(pi_n - 4) + 3;

    d = e*pi_n;

}

/*
 * @brief     RSA 암복호화를 진행하는 함수.
 * @param     llint data   : 키 값.
 * @param     llint key    : 키 값.
 * @param     llint n      : 모듈러 n 값.
 * @return    llint result : 암복호화에 결과값
 * @todo      과제 안내 문서의 제한사항을 참고하여 작성한다.
 */
llint miniRSA(llint data, llint key, llint n) {
    return 0;
}

llint GCD(llint a, llint b) {
    llint prev_a;

    while(b != 0) {
        printf("GCD(%lld, %lld)\n", a, b);
        prev_a = a;
        a = b;
        while(prev_a >= b) prev_a -= b;
        b = prev_a;
    }
    printf("GCD(%lld, %lld)\n\n", a, b);
    return a;
}

int main(int argc, char* argv[]) {
    /*
    byte plain_text[4] = {0x12, 0x34, 0x56, 0x78};
    llint plain_data, encrpyted_data, decrpyted_data;
    uint seed = time(NULL);

    memcpy(&plain_data, plain_text, 4);

    // 난수 생성기 시드값 설정
    seed = time(NULL);
    InitWELLRNG512a(&seed);

    // RSA 키 생성
    miniRSAKeygen(&p, &q, &e, &d, &n);
    printf("0. Key generation is Success!\n ");
    printf("p : %llu\n q : %llu\n e : %llu\n d : %llu\n N : %llu\n\n", p, q, e, d, n);

    // RSA 암호화 테스트
    encrpyted_data = miniRSA(plain_data, e, n);
    printf("1. plain text : %llu\n", plain_data);
    printf("2. encrypted plain text : %llu\n\n", encrpyted_data);

    // RSA 복호화 테스트
    decrpyted_data = miniRSA(encrpyted_data, d, n);
    printf("3. cipher text : %llu\n", encrpyted_data);
    printf("4. Decrypted plain text : %llu\n\n", decrpyted_data);

    // 결과 출력
    printf("RSA Decryption: %s\n", (decrpyted_data == plain_data) ? "SUCCESS!" : "FAILURE!");


    printf("%lf", WELLRNG512a());
    */
    llint a = 12;
    llint b = 18;
    double c = WELLRNG512a()*1000;//(WELLRNG512a()*(1<<54));

    llint m = GCD(3, 7);
    printf("%llu, %llu, %lf, %llu \n", a, b, c, m);

    return 0;
}