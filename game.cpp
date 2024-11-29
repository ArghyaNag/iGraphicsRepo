#include "iGraphics.h"
#include<algorithm>

//textures
#include "textures/T_NUMBERS.h"
#include "textures/T_VIEW2D.h"
#include "textures/T_00.h"
#include "textures/T_01.h"
#include "textures/T_02.h"
#include "textures/T_03.h"
#include "textures/T_04.h"
#include "textures/T_05.h"
#include "textures/T_06.h"
#include "textures/T_07.h"
#include "textures/T_08.h"
#include "textures/T_09.h"
#include "textures/T_10.h"
#include "textures/T_11.h"
#include "textures/T_12.h"
#include "textures/T_13.h"
#include "textures/T_14.h"
#include "textures/T_15.h"
#include "textures/T_16.h"
#include "textures/T_17.h"
#include "textures/T_18.h"
#include "textures/T_19.h"
int numText=19;                          //number of textures
int numSect= 0;                          //number of sectors
int numWall= 0;                          //number of walls

enum {IDLE,FIRE};
int fire_idx=0;
int state=IDLE;
char gun_fire[15][100];
char gun_idle[100];
char* gun_image;

int sx[2], sz[4];
int mx, my , mz ;
float t, g=1 ;

struct walls{
    int wx0, wy0;
    int wx1, wy1;
    int red,green,blue;
    int wt,u,v;
    int shade;
};

walls W[256];

struct sectors{
    int ws,we;
    int wz1,wz2;
    int topred, topgreen, topblue;
    int bottomred, bottomgreen, bottomblue;
    int toporbottom;
    int points[1920];
    int cx,cy;
    int d;
    int st,ss;
};

sectors S[128];

typedef struct 
{
 int w,h;                             
 const unsigned char *name;           
}TexureMaps; TexureMaps Textures[64]; 

void load()
{
 FILE *fp = fopen("level.h","r");
 if(fp == NULL){ printf("Error opening level.h"); return;}
 int s,w;

 fscanf(fp,"%i",&numSect);   //number of sectors 
 for(s=0;s<numSect;s++)      //load all sectors
 {
  fscanf(fp,"%i",&S[s].ws);  
  fscanf(fp,"%i",&S[s].we); 
  fscanf(fp,"%i",&S[s].wz1);  
  fscanf(fp,"%i",&S[s].wz2); 
  fscanf(fp,"%i",&S[s].st); 
  fscanf(fp,"%i",&S[s].ss);  
 }
 fscanf(fp,"%i",&numWall);   //number of walls 
 for(s=0;s<numWall;s++)      //load all walls
 {
  fscanf(fp,"%i",&W[s].wx0);  
  fscanf(fp,"%i",&W[s].wy0); 
  fscanf(fp,"%i",&W[s].wx1);  
  fscanf(fp,"%i",&W[s].wy1); 
  fscanf(fp,"%i",&W[s].wt);
  fscanf(fp,"%i",&W[s].u); 
  fscanf(fp,"%i",&W[s].v);  
  fscanf(fp,"%i",&W[s].shade);  
 }
 fscanf(fp,"%i %i %i %i %i",&mx,&my,&mz, &t,&g); //player position, angle, look direction 
 fclose(fp); 
}

void iDrawLine(int sx0, int sx1, int sz0, int sz1);
void iDrawWall(int sx0, int sx1, int sz0, int sz1, int sz2, int sz3, int red, int green, int blue, int s, int i);
void clipBehindPlayer(int *x1,int *y1,int *z1, int x2,int y2,int z2);
int dist(int x1,int y1, int x2,int y2);


void iDraw() {
    iClear();
    
    int ax[2] = {40, 40}, ay[2] = {10, 290}, az[2] = {0, 0}, loop;
    int x1[2], y1_new[2], z1[2];
    int px[4], py[4], pz[4];

    for(int s=0;s<numSect-1;s++){    
        for(int w=0;w<numSect-s-1;w++){
            if(S[w].d<S[w+1].d){ 
                sectors stemp=S[w]; S[w]=S[w+1]; S[w+1]=stemp; 
            }
        }
    }
    
    for(int s=0; s<numSect; s++){
        
        S[s].d=0;

        if(mz<S[s].wz1){S[s].toporbottom=1; loop=2; for(int i=0; i<1920; i++){S[s].points[i]=1040;}}
        else if(mz>S[s].wz2){S[s].toporbottom=2; loop=2; for(int i=0; i<1920; i++){S[s].points[i]= 0;}}
        else {S[s].toporbottom=0; loop=1;}

        for(int i=0; i<loop; i++){

            for(int w=S[s].ws; w<S[s].we; w++){

                x1[0] = W[w].wx0 - mx;
                x1[1] = W[w].wx1 - mx;
                y1_new[0] = W[w].wy0 - my;
                y1_new[1] = W[w].wy1 - my;
                z1[0] = S[s].wz1 - mz;
                z1[1] = S[s].wz1 - mz;

                if(i==1) {int temp=x1[0]; x1[0]=x1[1]; x1[1]=temp; temp=y1_new[0]; y1_new[0]=y1_new[1]; y1_new[1]=temp;} 

                px[0] = (x1[0] * cos(t) - y1_new[0] * sin(t));
                px[1] = (x1[1] * cos(t) - y1_new[1] * sin(t));
                px[2] = px[0];
                px[3] = px[1];

                py[0] = (y1_new[0] * cos(t) + x1[0] * sin(t));
                py[1] = (y1_new[1] * cos(t) + x1[1] * sin(t));
                py[2]=py[0];
                py[3]=py[1];

                S[s].d += dist(0,0, (px[0]+px[1])/2, (py[0]+py[1])/2 );

                pz[0] = z1[0] + (g * py[0] / 32.0);
                pz[1] = z1[1] + (g * py[1] / 32.0);
                pz[2] = S[s].wz2 - mz + (g * py[0] / 32.0);
                pz[3] = S[s].wz2 - mz + (g * py[1] / 32.0);
                
                if(py[0]<1 && py[1]<1){continue;}
                if(py[0]<1){

                    clipBehindPlayer(&px[0],&py[0],&pz[0],px[1],py[1],pz[1]);
                    clipBehindPlayer(&px[2],&py[2],&pz[2],px[3],py[3],pz[3]);

                }

                if(py[1]<1){

                    clipBehindPlayer(&px[1],&py[1],&pz[1],px[0],py[0],pz[0]);
                    clipBehindPlayer(&px[3],&py[3],&pz[3],px[2],py[2],pz[2]);

                }

                sx[0] = (px[0]* 1100) / py[0]  + 960; 
                sx[1] = (px[1]* 1100) / py[1]  + 960;
                sz[0] = (pz[0]* 1100) / py[0]  + 520;
                sz[1] = (pz[1]* 1100) / py[1]  + 520;
                //these two lines are later additions for a wall
                sz[2] = (pz[2]* 1100) / py[0]  + 520;
                sz[3] = (pz[3]* 1100) / py[1]  + 520;

                /*iPoint(sx[0], sz[0]); 
                iPoint(sx[1], sz[1]);*/                         //made redundant by the iDrawLine

                /*iDrawLine(sx[0],sx[1],sz[0],sz[1]);
                iDrawLine(sx[0],sx[1],sz[2],sz[3]);*/           //made redundant by the iDrawWall 
                iDrawWall(sx[0],sx[1],sz[0],sz[1],sz[2],sz[3],W[w].red,W[w].green,W[w].blue,s,i);
            }
            S[s].d/=(S[s].we-S[s].ws);  
            
        }
    }
    iShowBMP2(800,80,gun_image,0);
    iShowBMP2(900,550,"shotgun\\crosshair.bmp",0);
    iShowBMP2(0,0,"shotgun\\status_bar.bmp",-1);
}

void iDrawLine(int sx0, int sx1, int sz0, int sz1) {

    for(int screenx=sx0; screenx<=sx1; screenx++)
    {
        int screenz = ((sz1-sz0)*(screenx-sx0+0.5))/(sx1-sx0)+sz0;
        iPoint(screenx,screenz);
    }

}

void iDrawWall(int sx0, int sx1, int sz0, int sz1, int sz2, int sz3, int red, int green, int blue, int s, int i) {

    int screenx,screenz;

    int dy = sz1 - sz0;
    int dz = sz3 - sz2;
    int dx = sx1 - sx0; if(dx==0){dx=1;}
    int sx0backup = sx0;

    if(sx0<1){sx0=1;}
    if(sx1<1){sx1=1;}
    if(sx0>1920){sx0=1920;}
    if(sx1>1920){sx1=1920;}

    for(screenx=sx0; screenx<sx1; screenx++){

        int screenz1 = dy*(screenx-sx0backup+0.5)/dx + sz0;
        int screenz2 = dz*(screenx-sx0backup+0.5)/dx + sz2;

        if(screenz1<1){screenz1=1;}
        if(screenz2<1){screenz2=1;}
        if(screenz1>1040){screenz1=1040;}
        if(screenz2>1040){screenz2=1040;}

        /*if(S[s].toporbottom==1){ S[s].points[screenx]=screenz1; continue;}
        if(S[s].toporbottom==2){S[s].points[screenx]=screenz2; continue;}
        if(S[s].toporbottom==-1){ for(int screenz=S[s].points[screenx]; screenz<screenz1; screenz++){iSetColor(S[s].bottomred,S[s].bottomgreen,S[s].bottomblue); iPoint(screenx,screenz);}}
        if(S[s].toporbottom==-2){ for(int screenz=screenz2; screenz<S[s].points[screenx]; screenz++){iSetColor(S[s].topred,S[s].topgreen,S[s].topblue); iPoint(screenx,screenz);}}*/
        
        if(i==0)
        {
            if(S[s].toporbottom==1){ S[s].points[screenx] = screenz1;}
            if(S[s].toporbottom==2){ S[s].points[screenx] = screenz2;}
            for(int screenz=screenz1; screenz<screenz2; screenz++){ iSetColor(0,155,200); iPoint(screenx,screenz);}
        } 

        if(i==1)
        {
            if(S[s].toporbottom==1){ screenz2 = S[s].points[screenx]; iSetColor(100,78,90);}
            if(S[s].toporbottom==2){ screenz1 = S[s].points[screenx]; iSetColor(100,78,90);}
            for(int screenz=screenz1; screenz<screenz2; screenz++){iPoint(screenx,screenz);}
        }

    }
    
}

void populate_gun_images(){
    sprintf(gun_idle, "shotgun\\file_0-triangle.bmp");
    for(int i=0; i<15; i++){
        sprintf(gun_fire[i], "shotgun\\file_%d-triangle.bmp",i);
    }
    gun_image = gun_idle;
}

void update_gun(){
    switch(state){
        case IDLE:
            gun_image = gun_idle;
            break;
        case FIRE:
            gun_image = gun_fire[fire_idx];
            fire_idx = (fire_idx + 1) % 15;
            if(fire_idx == 0){
                state = IDLE;
            }
            break;
    }
}

void iKeyboard(unsigned char key) {
	if (key == 'j') {t-=0.07; if(t<0){t+=6.30;}}
    if( key == 'l') {t+=0.07; if(t>6.30){t-=6.30;}}
    if (key == 'i') {g-=1;}
    if (key == 'k') {g+=1;}

    int dx= sin(t)*10;
    int dy= cos(t)*10;

    if (key == 'a') {mx-=dy; my+=dx;}
    if (key == 'd') {mx+=dy; my-=dx;}
    if (key == 'w') {mx+=dx; my+=dy;}
    if (key == 's') {mx-=dx; my-=dy;}

    if(key == 't') {mz+=4;}
    if(key == 'g') {mz-=4;}

    if(key == 'f') {state=FIRE;}

    if(key == 'b') {load();}

	}

	
void pixelhishab() {

}

int dist(int x1,int y1, int x2,int y2)
{
 int distance = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
 return distance;
}


void clipBehindPlayer(int *x1,int *y1,int *z1, int x2,int y2,int z2) 
{
 float da=*y1;                                 
 float db= y2;                                 
 float d=da-db; if(d==0){ d=1;}
 float s = da/(da-db);                         
 *x1 = *x1 + s*(x2-(*x1));
 *y1 = *y1 + s*(y2-(*y1)); if(*y1==0){ *y1=1;} 
 *z1 = *z1 + s*(z2-(*z1));
}

void iMouseMove(int mx, int my) {
	//place your codes here
}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		//place your codes here
		//	printf("x = %d, y= %d\n",mx,my);
		
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		//place your codes here
		
	}
}

void iSpecialKeyboard(unsigned char key) {

	if (key == GLUT_KEY_END) {
		exit(0);
	}
	//place your codes for other keys here
}

void check(){
    printf("x %d y %d z %d t %f g %f sx0 %d sz0 %d sx1 %d sz1 %d sz2 %d sz3 %d\n",mx,my,mz,t*57.3,g,sx[0],sz[0],sx[1],sz[1],sz[2],sz[3]);
}

int main() {

    //initwalls();
    populate_gun_images();
    iSetTimer(100, update_gun);
    iSetTimer(3000, check);
    mx=70,my=-110,mz=20;
    //float t=0,g=0;

    iInitialize(1920,1040, "demo");
    return 0;
}
