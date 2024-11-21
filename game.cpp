#include "iGraphics.h"
#include<algorithm>
    
    int mx, my , mz ;
    float t, g ;
    int px[2], py[2], pz[4];

void iDrawWall(int px0, int px1, int py0, int py1, int pz0, int pz1);

void iDraw() {
    iClear();
    iSetColor(20, 200, 0);
    iDrawWall(px[0],px[1],py[0],py[1],pz[0],pz[1]);
}


void iDrawWall(int px0, int px1, int py0, int py1, int pz0, int pz1) {

    int x,y,z;

    for(x=px0; x<=px1; x++){
        y = (py1-py0)*(x-px0+0.5)/(px1-px0)+py0;
        z = (pz1-pz0)*(x-px0+0.5)/(px1-px0)+pz0;

        if(y>0){

            int screenx = (x*200)/y + 200;
            int screenz = (z*200)/y + 200;

            iPoint(screenx,screenz);
        
        }

    }

}



void iKeyboard(unsigned char key) {
	if (key == 'j') {t-=0.07; if(t<0){t+=6.3;}}
    if( key == 'l') {t+=0.07; if(t>6.30){t-=6.3;}}
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

/*void check(){
    printf("x %d y %d z %d t %f g %f sx0 %d sz0 %d sx1 %d sz1 %d \n",mx,my,mz,t*57.3,g,sx[0],sz[0],sx[1],sz[1]);
}*/

int main() {

    iSetTimer(5, pixelhishab);
    //iSetTimer(3000, check);
    mx=70,my=-110,mz=20;
    float t=0,g=0;

    iInitialize(400, 400, "demo");
    return 0;
}
