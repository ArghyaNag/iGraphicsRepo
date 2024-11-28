#include "iGraphics.h"
#include<algorithm>

int sx[2], sz[4];
int mx, my , mz ;
float t, g ;

struct walls{
    int wx0, wy0;
    int wx1, wy1;
    int red,green,blue;
};

int numWall=20;

walls W[30];

struct sectors{
    int ws,we;
    int wz1,wz2;
    int topred, topgreen, topblue;
    int bottomred, bottomgreen, bottomblue;
    int toporbottom;
    int points[1920];
    int cx,cy;
    int d;
};

int numSect=5;

sectors S[30];

int loadSectors[]=
{//wall start, wall end, z1 height, z2 height, bottom color, top color
 0,  4,-10, 40, 0, 255, 0, 0, 160, 0,
 4,  8,-10, 40, 0, 255, 255, 0, 160, 160,
 8, 12,-10, 40, 160, 100, 0, 110, 50, 0,
 12,16,-10, 40, 0, 60, 130, 255, 255, 0,
 16,20,-10,-5, 100, 100, 100, 200, 200, 200
};

int loadWalls[]=
{//x1,y1, x2,y2, color
  0, 0, 32, 0, 255, 255, 0,  
 32, 0, 32,32, 160, 160, 0,
 32,32,  0,32, 255, 255, 0,
  0,32,  0, 0, 160, 160, 0,

 64, 0, 96, 0, 0, 255, 0,
 96, 0, 96,32, 0, 160, 0,
 96,32, 64,32, 0, 255, 0,
 64,32, 64, 0, 0, 160, 0,

 64, 64, 96, 64, 0, 255, 255,
 96, 64, 96, 96, 0, 160, 160,
 96, 96, 64, 96, 0, 255, 255,
 64, 96, 64, 64, 0, 160, 160,

  0, 64, 32, 64, 160, 100, 0,
 32, 64, 32, 96, 110, 50, 0,
 32, 96,  0, 96, 160, 100, 0,
  0, 96,  0, 64, 110, 50, 0,

  -320, -320, 320, -320, 255, 255, 0,  
 320, -320, 320,320, 160, 160, 0,
 320,320, -320,320, 255, 255, 0,
  -320,320, -320, -320, 160, 160, 0,

};

void initwalls(){
 int v1=0,v2=0;
 for(int s=0;s<numSect;s++)
 {
  S[s].ws=loadSectors[v1+0];                   
  S[s].we=loadSectors[v1+1];                   
  S[s].wz1=loadSectors[v1+2];                  
  S[s].wz2=loadSectors[v1+3]-loadSectors[v1+2];
  S[s].bottomred=loadSectors[v1+4]; 
  S[s].bottomgreen=loadSectors[v1+5];
  S[s].bottomblue=loadSectors[v1+6];
  S[s].topred=loadSectors[v1+7];
  S[s].topgreen=loadSectors[v1+8];
  S[s].topblue=loadSectors[v1+9];                  
  v1+=10;
  for(int w=S[s].ws;w<S[s].we;w++)
  {
   W[w].wx0=loadWalls[v2+0]; 
   W[w].wy0=loadWalls[v2+1]; 
   W[w].wx1=loadWalls[v2+2]; 
   W[w].wy1=loadWalls[v2+3];
   W[w].red =loadWalls[v2+4];
   W[w].green =loadWalls[v2+5];
   W[w].blue =loadWalls[v2+6]; 
   v2+=7;
  }
 }
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
            for(int screenz=screenz1; screenz<screenz2; screenz++){ iSetColor(red,green,blue); iPoint(screenx,screenz);}
        } 

        if(i==1)
        {
            if(S[s].toporbottom==1){ screenz2 = S[s].points[screenx]; iSetColor(S[s].bottomred,S[s].bottomgreen,S[s].bottomblue);}
            if(S[s].toporbottom==2){ screenz1 = S[s].points[screenx]; iSetColor(S[s].topred,S[s].topgreen,S[s].topblue);}
            for(int screenz=screenz1; screenz<screenz2; screenz++){iPoint(screenx,screenz);}
        }

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

    initwalls();
    iSetTimer(5, pixelhishab);
    iSetTimer(3000, check);
    mx=70,my=-110,mz=20;
    float t=0,g=0;

    iInitialize(1920,1040, "demo");
    return 0;
}
