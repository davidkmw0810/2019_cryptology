#include <stdio.h>

int Extended_Euclid(int a, int p);

void main()
{
    int num1, num2, invers=0;

    printf("* 두 수를 입력하시오 : ");
    scanf("%d %d", &num1, &num2);

    printf("   q   r1   r2    r   s1   s2    s   t1   t2    t\n");

    if(num1 > num2)
        invers = Extended_Euclid(num1, num2);
    else
        invers = Extended_Euclid(num2, num1);

    if(invers)
        printf("* 두수의 역원 : %d \n", invers);
    else
        printf("역원이 존재하지 않습니다. \n");

}

int Extended_Euclid(int r1, int r2)
{
    int r, q, s, s1=1, s2=0, t, t1=0, t2=1, tmp = r1;

    while(r2)
    {
        q = r1/r2;
        r = r1%r2;
        s = s1 - q*s2;
        t = t1 - q*t2;

        printf("%4d %4d %4d %4d %4d %4d %4d %4d %4d %4d\n", q, r1, r2, r, s1, s2, s, t1, t2, t);

        r1 = r2;
        r2 = r;
        s1 = s2;
        s2 = s;
        t1 = t2;
        t2 = t;
    }
    printf("%4d %4d %4d      %4d %4d      %4d          \n\n", q, r1, r2, s1, s2, t1);

    printf("s : %d , t : %d \n", s1, t1);

    if(r1 == 1) //역원이 있음
    {
        if(t1 < 0)
            t1 += tmp;
        return t1;
    }

    return 0;
}