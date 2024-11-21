#include "iGraphics.h"
#include<algorithm>

    int sx[2], sz[4];
    int mx, my , mz ;
    float t, g ;

void iDrawLine(int sx0, int sx1, int sz0, int sz1);
void iDrawWall(int sx0, int sx1, int sz0, int sz1, int sz2, int sz3);
void clipBehindPlayer(int *x1,int *y1,int *z1, int x2,int y2,int z2);

void iDraw() {
    iClear();
    iSetColor(20, 200, 0);

    /*iPoint(sx[0], sz[0]); 
    iPoint(sx[1], sz[1]);*/                         //made redundant by the iDrawLine

    /*iDrawLine(sx[0],sx[1],sz[0],sz[1]);
    iDrawLine(sx[0],sx[1],sz[2],sz[3]);*/           //made redundant by the iDrawWall 

    iDrawWall(sx[0],sx[1],sz[0],sz[1],sz[2],sz[3]);
}

void iDrawLine(int sx0, int sx1, int sz0, int sz1) {

    for(int screenx=sx0; screenx<=sx1; screenx++)
    {
        int screenz = ((sz1-sz0)*(screenx-sx0+0.5))/(sx1-sx0)+sz0;
        iPoint(screenx,screenz);
    }

}

void iDrawWall(int sx0, int sx1, int sz0, int sz1, int sz2, int sz3) {

    int sx0backup=sx0;
    int sx1backup=sx1;
    
    for(int screenx=sx0; screenx<=sx1; screenx++)
    {
        int screenz1 = ((sz1-sz0)*(screenx-sx0backup+0.5))/(sx1backup-sx0backup)+sz0;
        int screenz2 = ((sz3-sz2)*(screenx-sx0backup+0.5))/(sx1backup-sx0backup)+sz2;

        for(int screenz=screenz1; screenz<=screenz2; screenz++)
        {
            iPoint(screenx,screenz);
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
    
    int ax[2] = {40, 240}, ay[2] = {10, 10}, az[2] = {0, 0};
    int x1[2], y1_new[2], z1[2];
    int px[2], py[2], pz[4];
    

    x1[0] = ax[0] - mx;
    x1[1] = ax[1] - mx;
    y1_new[0] = ay[0] - my;
    y1_new[1] = ay[1] - my;
    z1[0] = az[0] - mz;
    z1[1] = az[1] - mz;

    px[0] = (x1[0] * cos(t) - y1_new[0] * sin(t));
    px[1] = (x1[1] * cos(t) - y1_new[1] * sin(t));
    py[0] = (y1_new[0] * cos(t) + x1[0] * sin(t));
    py[1] = (y1_new[1] * cos(t) + x1[1] * sin(t));
    pz[0] = z1[0] + (g * py[0] / 32.0);
    pz[1] = z1[1] + (g * py[1] / 32.0);
    // these two lines are later additions for a wall
    pz[2] = pz[0] + 40;
    pz[3] = pz[1] + 40;

    sx[0] = (px[0]* 200) / abs(py[0])  + 200; 
    sx[1] = (px[1]* 200) / abs(py[1])  + 200;
    sz[0] = (pz[0]* 200) / abs(py[0])  + 200;
    sz[1] = (pz[1]* 200) / abs(py[1])  + 200;
    //these two lines are later additions for a wall
    sz[2] = (pz[2]* 200) / abs(py[0])  + 200;
    sz[3] = (pz[3]* 200) / abs(py[1])  + 200;

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
    printf("x %d y %d z %d t %f g %f sx0 %d sz0 %d sx1 %d sz1 %d \n",mx,my,mz,t*57.3,g,sx[0],sz[0],sx[1],sz[1]);
}

int main() {

    iSetTimer(5, pixelhishab);
    iSetTimer(3000, check);
    mx=70,my=-110,mz=20;
    float t=0,g=0;

    iInitialize(400, 400, "demo");
    return 0;
}
