#include "uart.h"
#include "lfb.h"
#include "sprintf.h"

struct WindowProperties {
    int windowHeight;
    int maxInputLineLen;
    int inputStartLine;
    int padding;
    int charWidth;
    int lineHeight;
    int indentLenX;
    int indentLenY;
    int indentX;
    int indentY;
    int inputWindowWidth;
    int inputWindowStartX;
    int inputWindowStartY;
    int outputWindowStartX;
    int outputStartLine;
    int maxExpressionLineLen;
    int windowTop;

} WindowProperties;

void pushLine(char line[], int lineNumber, int mode)
{
    int currentY = WindowProperties.indentY + (lineNumber * WindowProperties.lineHeight);
    int currentX;
    // output lines
    if(mode==0)
    {
        currentX = WindowProperties.indentX + ((WindowProperties.inputWindowWidth+WindowProperties.padding+1+WindowProperties.padding) * WindowProperties.charWidth);
    }

    // output title
    if(mode==1)
    {
        currentX = WindowProperties.indentX + ((WindowProperties.inputWindowWidth+WindowProperties.padding+1+(WindowProperties.padding/2)) * WindowProperties.charWidth);
    }

    // input title
    if(mode==2)
    {
        currentX = WindowProperties.indentX + ((1+(WindowProperties.padding/2)) * WindowProperties.charWidth);
    }

    // description
    if(mode==3)
    {
        currentX = WindowProperties.indentX + ((WindowProperties.padding) * WindowProperties.charWidth);
    }

    if (mode==4)
    {
        currentX = WindowProperties.indentX;
    }


    lfb_print(currentX, currentY, line);
}

void pushChar(char character, int charsBefore, int lineNumber, int mode)
{
    int currentY = WindowProperties.inputWindowStartY + (lineNumber * WindowProperties.lineHeight);
    int currentX;
    // input
    if (mode==0)
    {
        currentX = WindowProperties.inputWindowStartX + (charsBefore * WindowProperties.charWidth);
    }
    // far left
    else
    {
        currentX = WindowProperties.indentX + (charsBefore * WindowProperties.charWidth);
    }

    char charString[2];
    sprintf(charString, "%c", character);

    lfb_print(currentX, currentY, charString);


}
void setupScreen()
{
    WindowProperties.charWidth = 10;
    WindowProperties.lineHeight = 20;

    WindowProperties.windowHeight = 20;
    WindowProperties.windowTop = 0;

    WindowProperties.padding = 4;

    WindowProperties.indentLenX = 8;
    WindowProperties.indentLenY = WindowProperties.indentLenX/2;

    WindowProperties.maxInputLineLen = 15;
    WindowProperties.maxExpressionLineLen = WindowProperties.maxInputLineLen*2;

    WindowProperties.inputStartLine = WindowProperties.windowTop+6;
    WindowProperties.outputStartLine = WindowProperties.inputStartLine-1;

    WindowProperties.inputWindowWidth = (1 + WindowProperties.padding + WindowProperties.maxInputLineLen + WindowProperties.padding + 1);
    WindowProperties.indentX = WindowProperties.indentLenX * WindowProperties.charWidth;
    WindowProperties.indentY = WindowProperties.indentLenY * WindowProperties.lineHeight;

    WindowProperties.inputWindowStartX = WindowProperties.indentX + ((1 + WindowProperties.padding) * WindowProperties.charWidth);
    WindowProperties.inputWindowStartY = WindowProperties.indentY + (2  * WindowProperties.lineHeight);

    int line;
    for(line=0; line<=WindowProperties.windowHeight; line++)
    {

        // middle line
        pushChar('|',WindowProperties.inputWindowWidth+WindowProperties.padding,line,1);

        // box sides
        if(line>=WindowProperties.inputStartLine-2 && line != WindowProperties.windowHeight)
        {
            pushChar('|',0,line,1);
            pushChar('|',WindowProperties.inputWindowWidth-1,line,1);

        }

        // box top

        if(line == WindowProperties.inputStartLine-3)
        {
            int i;
            for (i=0; i<WindowProperties.inputWindowWidth; i++)
            {
                pushChar('-',i,line,1);
            }
        }

        // box bottom
        if(line == WindowProperties.windowHeight)
        {
            int i;
            for (i=0; i<WindowProperties.inputWindowWidth; i++)
            {
                pushChar('-',i,line,1);
            }
        }


        // push title
        if(line==WindowProperties.windowTop)
        {
            pushLine("Home Baked Pi Arithmetic v2", line, 4);
            //
        }

        // push description
        if (line==WindowProperties.windowTop+1)
        {
            pushLine("A simple calculator made w/",line,2);
            pushLine("bare metal on Raspberry Pi.",line+1,2);
        }


        // push input label
        if(line == WindowProperties.inputStartLine)
        {
            pushLine("Input:", line, 2);

        }

        // push output label
        if (line==WindowProperties.outputStartLine-2)
        {
            pushLine("Output:",line,1);
        }


    }


}

void empty(char a[], int len){
    int i;
    for (i=0; i<len; i++){
        a[i]='\0';
    }
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
    //char test[2] = " ";
    //sprintf(test,"%c",c);
    if(c == '\b') return c;
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
unsigned long div(unsigned long x, unsigned long y, int remainder)
{
    if (y==0){

        char temp[100];
        sprintf(temp,"Error - Divided by 0");
        pushLine(temp,WindowProperties.outputStartLine,0);

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
unsigned long raise(unsigned long x, unsigned long y)
{
    unsigned long z = 1;
    int i;
    for (i=0; i<y; i++){
        z=mult(z,x);
    }
    return z;
}


unsigned long printExpr(char terms[][100], char operator[], int numTerms, int maxLen, char expr[])
{
    int line=WindowProperties.outputStartLine;
    char temp[100];
    char copy[100];
    sprintf(temp,"Calculator Results:");
    pushLine(temp,line,0);
    line++;
    sprintf(temp," ");
    pushLine(temp,line,0);
    line++;
    printf("~Calculator Results~\n\n");

    //temp=copy;
    sprintf(temp,"    terms[0] length: \"%s\" %d", terms[0],strLen(terms[0],100));
    pushLine(temp,line,0);
    line++;
    sprintf(temp,"    terms[0] as long: \"%s\" %d \n\n", terms[0],charArrayToLong(terms[0],100));
    pushLine(temp,line,0);
    line++;
    sprintf(temp," ");
    pushLine(temp,line,0);
    line++;

    printf("    terms[0] length: %s %d \n", terms[0],strLen(terms[0],100));
    printf("    terms[0] as long: %s %d \n\n", terms[0],charArrayToLong(terms[0],100));


    unsigned long ad=add(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen));
    unsigned long su=sub(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen));
    unsigned long mul=mult(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen));
    unsigned long rais=raise(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen));
    unsigned long di=div(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen),0);
    unsigned long re=div(charArrayToLong(terms[0], maxLen),charArrayToLong(terms[1], maxLen),1);

    printf("    Operations on terms[0:1]:\n        Addition: %d\n        Subtraction: %d\n        Multiplication: %d\n        Division: %d\n        Remainder: %d\n        Raise (bugged with big numbers): %d\n",
           ad, su, mul, di, re, rais);

    sprintf(temp," ");
    pushLine(temp,line,0);
    line++;
    sprintf(temp,"    Operations on terms[0:1]:");
    pushLine(temp,line,0);
    line++;

    sprintf(temp," ");
    pushLine(temp,line,0);
    line++;

    sprintf(temp,"        Addition: %d",ad);
    pushLine(temp,line,0);
    line++;

    sprintf(temp,"        Subtraction: %d",su);
    pushLine(temp,line,0);
    line++;

    sprintf(temp,"        Multiplication: %d",mul);
    pushLine(temp,line,0);
    line++;

    sprintf(temp,"        Division: %d",di);
    pushLine(temp,line,0);
    line++;

    sprintf(temp,"        Remainder: %d",re);
    pushLine(temp,line,0);
    line++;

    sprintf(temp,"        Raise: %d",rais);
    pushLine(temp,line,0);
    line++;
    sprintf(temp," ");
    pushLine(temp,line,0);
    line++;

    int i=0;
    printf("\n    Expression:\n        ");
    sprintf(temp,"Expression:");
    pushLine(temp,line,0);
    line++;
    sprintf(temp," ");
    pushLine(temp,line,0);
    line++;

    empty(temp,100);

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
            mySum=div(mySum, charArrayToLong(terms[i+1],maxLen), 1);
        }
        if (operator[i]=='^'){
            mySum=raise(mySum, charArrayToLong(terms[i+1],maxLen));
        }

        sprintf(temp,"%s %s %c", temp, terms[i], operator[i]);

        if(strLen(temp,100)>=WindowProperties.maxExpressionLineLen){
            pushLine(temp,line,0);
            line++;
            empty(temp,100);
        }

        printf("%s ",terms[i]);
        printf("%c ", operator[i]);

        i++;
    }

    sprintf(temp,"%s %s = %d",temp, terms[i], mySum);
    pushLine(temp,line,0);
    line++;
    printf("%s = %d",terms[i], mySum);
    printf("\n");


    return mySum;

}





void main()
{
    //printf("\nHomebaked Pi Arithmetic\n\nInput positive integers separated by operators:\n\n    ");
    char special[]={'\n','+','-','*','x','/','^','%'};

    // set up serial console
    uart_init();
    lfb_init();

    int windowHeight = 20;
    int maxInputLineLen = 10;
    int inputStartLine = 3;
    int padding = 4;

    setupScreen();

    int cursorLine = WindowProperties.inputStartLine;
    int cursorChar = 0;

    int x, placeX, opX;
    x=placeX=opX=0;

    char c;
    c=input();

    char terms[50][100];
    empty2(terms,50,100);
    char operator[100]="\0";
    char expr[100]="\0";
    char fullExpr[214]="\0";
    unsigned long result;





    while(1) {


        if (c == '\n') {
            printf("\n");
            result=printExpr(terms,operator,opX+1,100, expr);
            printf("\n");
            break;
        }
        else if(c=='\b'){
            uart_send(c);
            cursorChar--;
            if (cursorChar<0){
                cursorChar=WindowProperties.maxInputLineLen-1;
                cursorLine--;
            }

            pushChar(' ',cursorChar,cursorLine,0);
        }
        else{

            pushChar(c,cursorChar,cursorLine,0);
            if(cursorChar==WindowProperties.maxInputLineLen-1)
            {
                cursorLine++;
                cursorChar=0;
            }
            else
            {
                cursorChar++;
            }

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
