#include <D:\documents\c++\Computer graphics\graphics.h>
#include <iostream>
#include "graphics.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>

using namespace std;

int* rotate(int x0, int y0, int a, double alpha){
    /*
    M = | cos(a) -sin(a) |
        | sin(a)  cos(a) |  
    */
//    line(x0, y0, x0+cos(M_PI*alpha/180)*(x0 + a) - sin(M_PI*alpha/180)*)
    int x1;
    int y1;
    x1 = x0 + a * cos(alpha * M_PI/180);
    y1 = y0 - a * sin(alpha * M_PI/180);
    int *dot = new int[2];
    dot[0] = x1; dot[1] = y1;
    return dot;
}
        /* ----------------------- +x
        |                    
        |         B(x0, y0-a)--C(x0+a, y0-a)
        |         |            |
        |         |            |
        |         A(x0, y0)----D(x0+a, y0)          
        |
        |
        -y
        */  
int* drawSquare(int x0, int y0, double alpha, int a, double fi){

    // circle(x0, y0, 5);

    int *dotD = rotate(x0, y0, a, alpha);
    line(x0, y0, dotD[0], dotD[1]);


    int *dotB = rotate(x0, y0, a, 90+alpha);
    line(x0, y0, dotB[0], dotB[1]);
    // circle(dotB[0], dotB[1], 5);
 
    int* dotC = rotate(dotB[0], dotB[1], a, alpha);
    line(dotB[0], dotB[1], dotC[0], dotC[1]);
 
    line(dotD[0], dotD[1], dotC[0], dotC[1]);

    int* roof = new int[2];
    roof = rotate(dotB[0], dotB[1], a*cos((fi)*M_PI/180), alpha+fi); 
    // circle(roof[0], roof[1], 5);

    int *ptr = new int[4];
    ptr[0] = dotB[0]; 
    ptr[1] = dotB[1]; 
    ptr[2] = roof[0]; 
    ptr[3] = roof[1];
    return ptr;
}

int pifagor(int n, int x0, int y0, int a, double fi, double alpha){
    if(n < 1 || a*a <= 4){
        return 0;
    }
    else{
        int* ptr = drawSquare(x0, y0, fi, a, alpha);
        // circle(ptr[2], ptr[3], 5); 
        pifagor(n-1, ptr[0], ptr[1], a*cos(alpha*M_PI/180), fi+alpha, alpha);
        pifagor(n-1, ptr[2], ptr[3], a*sin(alpha*M_PI/180), -90+fi+alpha, alpha);      
    }
}
/*
    n -- глубина фрактала
    x0, y0 -- растровые координаты точки привязки домика
    a -- длина основания домика
    fi -- угол наклона основания домика
    alpha -- угол наклона крыши домика
    */

int main(){
    // printf("idk");
    int gddriver = DETECT, gmode, errorcode;
    initgraph(&gddriver, &gmode, "");

    srand(time(NULL));

    // int* ptr = drawSquare(1280, 720, 45, 30);
    pifagor(15, 700, 700, 150, 30, 30);


    getch();
    closegraph();

    return 0;
}