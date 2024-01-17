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

class Edge{
public:
    Vertex* startvertex;
    Vertex* finishvertex;
    
    Edge* next;
    Edge(){}
    void drawedge(V_matrix v, int d, int c1, int c2){
        srand(time(0));
        startvertex->setviewcoord(v);
        finishvertex->setviewcoord(v);
        setcolor(GREEN);
        line(int(d * startvertex->viewcoord.x / startvertex->viewcoord.z + c1/2), int(d * startvertex->viewcoord.y / startvertex->viewcoord.z + c2/2), 
        int(d * finishvertex->viewcoord.x / finishvertex->viewcoord.z + c1/2), int(d * finishvertex->viewcoord.y / finishvertex->viewcoord.z + c2/2));  
    }
};

class Surface{
    Vertex* vlist;
    Edge* elist;
    V_matrix* v;
    int num;
public:  
    Surface(){}
    void load(){
        vlist = nullptr;elist = nullptr; Vertex* vtmp; Edge* etmp;
        int i, n, m;
        double xx, yy, zz, n1, n2;
        //Обращение к файлу с вершинами и ребрами
        ifstream InputFile("pira.txt");
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
            InputFile >> n1 >> n2;
            etmp = new Edge();
            etmp->next == nullptr;
            vtmp = vlist;
            etmp->startvertex=nullptr;
            while(etmp->startvertex == nullptr && vtmp != nullptr){
                if(vtmp->num == n1)
                    etmp->startvertex = vtmp;
                else
                    vtmp=vtmp->next;
            }
            vtmp = vlist;
            etmp->finishvertex=nullptr;
            while(etmp->finishvertex == nullptr && vtmp != nullptr){
                if(vtmp->num==n2)
                    etmp->finishvertex=vtmp;
                else
                    vtmp = vtmp->next;
            }

            if(elist == nullptr)
                elist = etmp;
            else{
                etmp->next = elist;
                elist = etmp;
            }
        }
        InputFile.close();
    }
    
    void drawsurface(double ro, double phi, double theta, int d, int c1, int c2){
        this->v = new V_matrix();
        this->v->calculate(ro, phi, theta);
        Edge* etmp = elist;
        int i = 0;
        while(etmp != nullptr && this->num > i ){
            // cout << "Drawedge" << endl;
            etmp->drawedge(*this->v, d, c1, c2);
            etmp = etmp->next;
            i++;
        }
        delete v;
    }
};


int main(){
    int gddriver = DETECT, gmode, errorcode;
    initgraph(&gddriver, &gmode, "");
    double phi=30;
    double theta=50;
    double ro = 300;
    Surface* s1 = new Surface();
    s1->load();
    int screenWidth = 400; 
    int screenHeight = 300;

    int dist = 150;
    for(int i = 1; i < 100; i++){
        s1->drawsurface(ro, phi, theta, dist, screenWidth, screenHeight);
        phi = phi + 10;
        theta = theta + 30;
        delay(500);
        cleardevice();
    }
    getch();
    closegraph();
    return 0;
}