#include "uart.h"

int valueInArray(char val, char arr[], int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        if(arr[i] == val)
            return 1;
    }
    return 0;
}

char input(){
    char nums[]={'0','1','2','3','4','5','6','7','8','9'};
    char special[]={'\n','+','-','*','x','/','^','%'};

    char c = uart_getc();
    if (valueInArray(c,nums,10)==1 || valueInArray(c,special,8)==1){
        uart_send(c);
        return c;
    };
    return input();
}



int strLen(char str[], int maxLen){
    int len=0;
    int i;

    for (i=0; i<maxLen; i++){
        if (str[i]!='\0'){
            len++;
        }
    }

    return len;
}


unsigned long charArrayToLong(char *arr, int maxLen) {

    unsigned long i, flag, value, r;

    flag = 1;
    i = value = 0;

    for( i = 0 ; i<strLen(arr, maxLen) ; ++i){

        if( i==0 && arr[i]=='-' ){
            flag = 0;
            continue;
        }

        r = arr[i] - '0';
        value = value * 10 + r;
    }

    value = value * flag;

    return value;

}


unsigned long add(unsigned long x, unsigned long y){

    unsigned long z;
    z = x + y;
    return z;
}


unsigned long sub(unsigned long x, unsigned long y){
    unsigned long z;
    z = x - y;
    return z;
}


unsigned long mult(unsigned long x, unsigned long y)
{

    unsigned long z =0;
    int i;
    for (i=0; i<y; i++){
        z=add(z,x);
    }

    return z;
}

unsigned long raise(unsigned long x, unsigned long y)
{

    unsigned long z = 1;
    int i;
    for (i=0; i<y; i++){
        z=mult(z,x);
    }

    return z;
}

unsigned long div(unsigned long x, unsigned long y, int remainder)
{
    if (y==0){
        printf("Error - Divided by 0");
        while(1){
            input();
        }
    }
    unsigned long z = 0;
    while (x >= y)
    {
        x = sub(x,y);
        z++;
    }

    if(remainder==1){
        return x;
    }
    return z;
}


void printExpr(char terms[][100], char operator[], int numTerms, int maxLen){


    printf("~Calculator Results~\n\n");
    printf("    terms[0] length: %s %d \n", terms[0],strLen(terms[0],100));
    printf("    terms[0] as long: %s %d \n\n", terms[0],charArrayToLong(terms[0],100));
    unsigned long ad=add(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen));
    unsigned long su=sub(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen));
    unsigned long mul=mult(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen));
    unsigned long rais=raise(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen));
    unsigned long di=div(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen),0);
    unsigned long re=div(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen),1);

    printf("    Operations on terms[0:1]:\n        Addition: %d\n        Subtraction: %d\n        Multiplication%d\n        Division: %d\n        Remainder: %d\n        Raise (bugged with big numbers): %d\n", ad, su, mul, di, re, rais);

    int i=0;
    printf("\n    Expression:\n        ");
    unsigned long mySum= charArrayToLong(terms[0],maxLen);
    while(i!=numTerms-1){

        if (operator[i]=='+'){
            mySum=add(mySum, charArrayToLong(terms[i+1],maxLen));
        }
        if (operator[i]=='-'){
            mySum=sub(mySum, charArrayToLong(terms[i+1],maxLen));
        }
        if (operator[i]=='*' || operator[i]=='x'){
            mySum=mult(mySum, charArrayToLong(terms[i+1],maxLen));
        }
        if (operator[i]=='/'){
            mySum=div(mySum, charArrayToLong(terms[i+1],maxLen), 0);
        }
        if (operator[i]=='%'){
            mySum=div(mySum, charArrayToLong(terms[i+1],maxLen), 0);
        }
        if (operator[i]=='^'){
            mySum=raise(mySum, charArrayToLong(terms[i+1],maxLen));
        }

        printf("%s ",terms[i]);
        printf("%c ", operator[i]);

        i++;
    }

    printf("%s = %d",terms[i], mySum);
    printf("\n");
}



void empty2(char a[50][100], int termNum, int termLen){
    int i;
    for (i=0; i<termNum; i++){
        int x;
        for (x=0; x<termLen; x++){
            a[i][x]='\0';
        }
    }
}

void empty(char a[], int len){
    int i;
    for (i=0; i<len; i++){
        a[i]='\0';
    }
}



void main()
{

    //char nums[]={'0','1','2','3','4','5','6','7','8','9'};
    char special[]={'\n','+','-','*','x','/','^','%'};



    // set up serial console
    uart_init();

    // test our printf implementation
    //printf( "Hello %s!\n"
    //        "This is character '%c', a hex number: %x and in decimal: %d\n"
    //        "Padding test: '%8x', '%8d'\n",
    //        "World", 'A', 32767, 32767, 0x7FFF, -123);

    int x=0;
    int placeX=0;
    int opX=0;

    char c;
    c=input();

    char terms[50][100];
    empty2(terms,50,100);
    char operator[100]="\0";
    char expr[100]="\0";
    while(1) {
        if (c == '\n') {
            printf("\n");
            printExpr(terms,operator,opX+1,100);
            printf("\n");
            break;
        }
        else{
            expr[x] = c;
            if(valueInArray(c,special,8)==1){
                operator[opX]=c;
                opX++;
                placeX=0;
            }
            else{
                terms[opX][placeX]=c;
                placeX++;
            }
            x++;
        };
        c=input();
    }
}
