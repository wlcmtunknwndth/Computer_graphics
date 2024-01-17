#include <iostream>
#include "graphics.h"
#include <fstream>
#include <math.h>
#include <time.h>
using namespace std;
const double PI = 3.1415;

class V_matrix{
public:
    double ro, phi, theta;
    double sinth, costh, sinph, cosph;
    V_matrix(){
        this->ro = 0;
        this->theta = 0;
        this->phi = 0;
    }

    void calculate(double ro, double phi, double theta){
        this->ro = ro;
        this->phi = phi;
        this->theta = theta;
        
        this->sinth = sin(this->theta*PI/180);
        this->costh = cos(this->theta*PI/180);
        this->sinph = sin(this->phi*PI/180);
        this->cosph = cos(this->phi*PI/180);
    }
};

struct vector3d{
    double x, y, z; // int
    vector3d(){
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
};

struct Vertex{
    vector3d worldcoord, viewcoord;
    int num;

    Vertex* next;
    
    Vertex(double x, double y, int z, int num){ // int 
        this->worldcoord.x = x;
        this->worldcoord.y = y;
        this->worldcoord.z = z;
        this->num = num;
    }

    void setviewcoord(V_matrix v){
        this->viewcoord.x = -this->worldcoord.x * v.sinth + this->worldcoord.y * v.costh;
        this->viewcoord.y = -this->worldcoord.x * v.cosph * v.costh - this->worldcoord.y * v.cosph * v.sinth  + this->worldcoord.z * v.sinph;
        this->viewcoord.z = -this->worldcoord.x * v.sinph * v.costh - this->worldcoord.y * v.sinph * v.sinth  - this->worldcoord.z * v.cosph + v.ro;
    }
};


// class Edge{
// public:
//     Vertex* startvertex;
//     Vertex* finishvertex;
    
//     Edge* next;
//     Edge(){}
//     void drawedge(V_matrix v, int d, int c1, int c2){
//         srand(time(0));
//         // startvertex->setviewcoord(v);
//         // finishvertex->setviewcoord(v);
//         setcolor(rand()%15);
//         line(int(d * startvertex->viewcoord.x / startvertex->viewcoord.z + c1), int(d * startvertex->viewcoord.y / startvertex->viewcoord.z + c2), 
//         int(d * finishvertex->viewcoord.x / finishvertex->viewcoord.z + c1), int(d * finishvertex->viewcoord.y / finishvertex->viewcoord.z + c2));  
//     }
// };

// xa ya 1
// xb yb 1 = > xa(yb - yc) - ya(xb - xc) + xb*yc - yb * xc = xa*yb - xa*yc - ya * xb + ya*xc + xb*yc - yb * xc =  
// xc yc 1

class Triangle{
public:
    Vertex* one;
    Vertex* two;
    Vertex* three;

    Triangle* next;

    bool is_Face(){
        double det = one->viewcoord.x * (two->viewcoord.y - three->viewcoord.y) - one->viewcoord.y*(two->viewcoord.x - three->viewcoord.x) + two->viewcoord.x*three->viewcoord.y - 
            two->viewcoord.y*three->viewcoord.x;
        if(det < 0) return true;
        else return false;
    }

    void drawTriangle(int d, int c1, int c2){
        setcolor(GREEN);
        if(is_Face()) return;
        line(int(d * one->viewcoord.x / one->viewcoord.z + c1/2), int(d * one->viewcoord.y / one->viewcoord.z + c2/2), 
        int(d * two->viewcoord.x / two->viewcoord.z + c1/2), int(d * two->viewcoord.y / two->viewcoord.z + c2/2));

        line(int(d * one->viewcoord.x / one->viewcoord.z + c1/2), int(d * one->viewcoord.y / one->viewcoord.z + c2/2), 
        int(d * three->viewcoord.x / three->viewcoord.z + c1/2), int(d * three->viewcoord.y / three->viewcoord.z + c2/2));  

        line(int(d * three->viewcoord.x / three->viewcoord.z + c1/2), int(d * three->viewcoord.y / three->viewcoord.z + c2/2), 
        int(d * two->viewcoord.x / two->viewcoord.z + c1/2), int(d * two->viewcoord.y / two->viewcoord.z + c2/2));
    }
};


class Surface{
    Vertex* vlist;
    // Edge* elist;
    V_matrix* v;
    Triangle* tlist;
    int num;
public:  
    Surface(){}
    void load2(){
        vlist = nullptr;tlist = nullptr; Vertex* vtmp; Triangle* etmp;
        int i, n, m;
        double xx, yy, zz, n1, n2, n3;
        //Обращение к файлу с вершинами и ребрами
        ifstream InputFile("triang2.txt");
        //Считывание координат вершин
        InputFile >> n;
        for(i = 0; i < n; i++){
            InputFile>>xx>>yy>>zz;
            vtmp = new Vertex(xx, yy, zz, i+1);
            if(vlist == nullptr)
                vlist = vtmp;
            else{
                vtmp->next = vlist;
                vlist = vtmp;
            }
        }

        InputFile>>m;
        this->num = m;
        for(i = 0; i < m; i++){
            InputFile >> n1 >> n2 >> n3;
            etmp = new Triangle();
            etmp->next == nullptr;
            vtmp = vlist;
            etmp->one=nullptr;
            while(etmp->one == nullptr && vtmp != nullptr){
                if(vtmp->num == n1)
                    etmp->one = vtmp;
                else
                    vtmp=vtmp->next;
            }
            vtmp = vlist;
            etmp->two=nullptr;
            while(etmp->two == nullptr && vtmp != nullptr){
                if(vtmp->num==n2)
                    etmp->two = vtmp;
                else
                    vtmp = vtmp->next;
            }
            vtmp = vlist;
            etmp->three=nullptr;
            while(etmp->three == nullptr && vtmp != nullptr){
                if(vtmp->num==n3)
                    etmp->three=vtmp;
                else
                    vtmp = vtmp->next;
            }

            if(tlist == nullptr)
                tlist = etmp;
            else{
                etmp->next = tlist;
                tlist = etmp;
            }
        }
        InputFile.close();
    }
    
    void drawsurface2(double ro, double phi, double theta, int d, int c1, int c2){
        this->v = new V_matrix();
        this->v->calculate(ro, phi, theta);
        Triangle* etmp = tlist;
        Vertex* vtmp = vlist;
        int i = 0;
        while(vtmp != nullptr){
            vtmp->setviewcoord(*this->v);
            vtmp = vtmp->next; 
            i++;
        }
        i=0;
        while(etmp != nullptr && this->num > i ){
            // etmp->one->setviewcoord(*this->v);
            // etmp->two->setviewcoord(*this->v);
            // etmp->three->setviewcoord(*this->v);
            etmp->drawTriangle(d, c1, c2); 
            etmp = etmp->next;
            i++;
        }
        delete v;
    }

};


int main(){
    int gddriver = DETECT, gmode, errorcode;
    initgraph(&gddriver, &gmode, "");
    double phi=0;
    double theta=0;
    double ro = 500;
    Surface* s1 = new Surface();
    s1->load2();
    int screenWidth = 1920; 
    int screenHeight = 1080;

    int dist = 150;
    for(int i = 1; i < 100; i++){
        s1->drawsurface2(ro, phi, theta, dist, screenWidth, screenHeight);
        cout << "further";
        phi = phi + 10;
        theta = theta + 30;
        delay(500);
        cleardevice();
    }
    getch();
    closegraph();
    return 0;
}