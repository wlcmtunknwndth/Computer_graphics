#include <D:\documents\c++\Computer graphics\graphics.h>
#include <iostream>
#include "graphics.h"
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <cstdlib>

using namespace std;


void myLine(int x0, int y0, int x1, int y1, int c){
    
    int dx = abs(x1-x0); int dy = abs(y1-y0);
    int error = 0;
    int deltaerr = (dy+1);

    int direction = y1-y0;    
    if (direction > 0) direction = 1;
    else direction = -1;

    int y = y0;
    for(int x = x0; x <= x1; x++){
        putpixel(x, y, c);
        error += deltaerr;
        if(error >= dx + 1){
            y = y + direction; 
            error -= (dx + 1);
        } 
    }
}

void myCirc(int x0, int y0, int r, int c){
    int x = 0; 
    int y = r;
    int delta = 1 - 2*r;
    int error = 0;
    while(y>=x){
        putpixel(x0 + x, y0 + y, c);
        putpixel(x0 + x, y0 + y, c);
        putpixel(x0 + x, y0 - y, c);
        putpixel(x0 - x, y0 + y, c);
        putpixel(x0 - x, y0 - y, c);
        putpixel(x0 + y, y0 + x, c);
        putpixel(x0 + y, y0 - x, c);
        putpixel(x0 - y, y0 + x, c);
        putpixel(x0 - y, y0 - x, c);
        error = 2 * (delta+y) - 1;
        if((delta < 0) && (error <= 0)){
            x += 1;
            delta += 2 * x + 1;
            continue;
        }
        if((delta > 0) && (error > 0)){
            y -= 1; 
            delta -= 2 * y + 1;
            continue;
        }
        x += 1; 
        y -= 1;
        delta += 2 * (x - y);
    }
}

int main(){
    int gddriver = DETECT, gmode, errorcode;
    initgraph(&gddriver, &gmode, "");

    srand(time(NULL));
    // myLine(200, 200, 1000, 1000, 15);
    myCirc(500, 500, 400, 15);
    getch();
    closegraph();

    return 0;
}