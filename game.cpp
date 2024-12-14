#include "iGraphics.h"
#include<algorithm>
#include<cmath>

int debug=0;

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
#include "textures/T_20.h"
#include "textures/T_21.h"
#include "textures/T_22.h"
#include "textures/T_23.h"
#include "textures/T_24.h"
#include "textures/T_25.h"
#include "textures/T_26.h"
#include "textures/T_27.h"
#include "textures/T_28.h"
#include "textures/T_29.h"
#include "textures/T_30.h"
#include "textures/T_31.h"
#include "textures/T_32.h"
#include "textures/T_33.h"
#include "textures/T_34.h"
#include "textures/T_35.h"
#include "textures/T_36.h"
#include "textures/T_37.h"
#include "textures/T_38.h"
#include "textures/T_39.h"
#include "textures/T_40.h"
#include "textures/T_41.h"
#include "textures/T_42.h"

int numText=20;                          
int numSect= 0;                          
int numWall= 0;         

int access[1000][1000]; 
int leaderboard[5], leadcount=0;
int runorfire[3]={0,0,0};
int score=0;
int timer=0;
int health=100;
int backx0,backy0,backx1,backy1,frontx0,fronty0,frontx1,fronty1;
enum {IDLE,FIRE};
enum {jIDLE,jRUN,jFIRE,jDEATH};
int menu=0;
int fire_idx=0;
int menu_idx=0;
int state=IDLE;
int jstate[3];
char leaderstring[5][100];
char healthstring[100]="HEALTH= 0";
char timerstring[100]="00:00:00";
char scorestring[100]="SCORE= 0";
char gun_fire[15][100];
char gun_idle[100];
char* gun_image;
char menupics[2100][100];
char* menupic = "menu\\00001.bmp";
int jagind[3]={23,23,23};

int sx[2], sz[4];
int mx, my , mz ;
float t, g=1 ;

struct enemystats{
    int isEnemy;
    int shotcount;
};

enemystats enemy[100];

struct walls{
    int wx0, wy0;
    int wx1, wy1;
    int red,green,blue;
    int wt,u,v;
    int shade;
};

walls W[600];

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

sectors S[128], S1[128];

typedef struct 
{
 int w,h;                             
 const unsigned char *name;           
}TextureMaps; TextureMaps Textures[64]; 

void load()
{
 FILE *fp = fopen("level.h","r");
 if(fp == NULL){return;}
 int s,w;

 fscanf(fp,"%i",&numSect);   
 for(s=0;s<numSect;s++)      
 {
  fscanf(fp,"%i",&S[s].ws);  S1[s].ws=S[s].ws;
  fscanf(fp,"%i",&S[s].we);  S1[s].we=S[s].we;
  fscanf(fp,"%i",&S[s].wz1);  
  fscanf(fp,"%i",&S[s].wz2); 
  fscanf(fp,"%i",&S[s].st); 
  fscanf(fp,"%i",&S[s].ss);   
 }
 fscanf(fp,"%i",&numWall);   
 for(s=0;s<numWall;s++)      
 {
  fscanf(fp,"%i",&W[s].wx0);  
  fscanf(fp,"%i",&W[s].wy0); 
  fscanf(fp,"%i",&W[s].wx1);  
  fscanf(fp,"%i",&W[s].wy1); 
  fscanf(fp,"%i",&W[s].wt);
  fscanf(fp,"%i",&W[s].u); 
  fscanf(fp,"%i",&W[s].v);  
  fscanf(fp,"%i",&W[s].shade);  

  if(s%4==0 && s>11 && s<227){backx0=W[s].wx0; backy0=W[s].wy0; backx1=W[s].wx1; backy1=W[s].wy1;}  
  else if(s%2==0 && s>11 && s<227){

    frontx0=W[s].wx0; fronty0=W[s].wy0; frontx1=W[s].wx1; fronty1=W[s].wy1;

    if(backy0==backy1){
        for(int i=std::min(backx0,backx1); i<=std::max(backx0,backx1);i++){
            for(int j=std::min(backy0,fronty1); j<=std::max(backy0,fronty1); j++){
                access[i][j]=1;
            }
        }
    }
    else if(backx0==backx1){
        for(int i=backy0; i<=backy1;i++){
            for(int j=frontx1; j<=backx0; j++){
                access[j][i]=1;
            }
        }
    }
   }
    }
  

 fscanf(fp,"%i %i %i %i %i",&mx,&my,&mz, &t,&g); 
 fclose(fp); 
}

void iDrawLine(int sx0, int sx1, int sz0, int sz1);
void iDrawWall(int sx0, int sx1, int sz0, int sz1, int sz2, int sz3, int red, int green, int blue, int s, int i, int w);
void clipBehindPlayer(int *x1,int *y1,int *z1, int x2,int y2,int z2);
int dist(int x1,int y1, int x2,int y2);
void update_leaderboard(); 

/*void testTextures(){
    int x,y,t;
    t=0;
    for(y=0;y<Textures[t].h;y++){
        for(x=0;x<Textures[t].w;x++){
            int flag=(Textures[t].h-y-1)*3*Textures[t].w+x*3;
            iSetColor(Textures[t].name[flag],Textures[t].name[flag+1],Textures[t].name[flag+2]);
            iPoint(x,y+200);
        }
    }
}*/

void iDraw() {                                                                  
    iClear();

    if(menu==0){ iShowBMP2(0,0,menupic,-1);}

    if(menu==1 && score<3){
    int ax[2] = {40, 40}, ay[2] = {10, 290}, az[2] = {0, 0}, loop;
    int x1[2], y1_new[2], z1[2];
    int px[4], py[4], pz[4];

    for(int s=0;s<numSect-1;s++){    
        for(int w=1;w<numSect-s-1;w++){
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

                //if(S[s].we-S[s].ws==2){

                    /*int centerx = ((x1[0]+x1[1])/2);
                    int centery = ((y1_new[0]+y1_new[1])/2);
                    float hw = 0.5 * sqrt((x1[1]-x1[0])*(x1[1]-x1[0])+(y1_new[1]-y1_new[0])*(y1_new[1]-y1_new[0]));

                    float slop = ((((x1[0]+x1[1])/2)-mx)/((1.0)*(((y1_new[0]+y1_new[1])/2)-my)));
                    float theta = atan(abs((((x1[0]+x1[1])/2)-mx)/((1.0)*(((y1_new[0]+y1_new[1])/2)-my))));  printf("%f ",theta);

                    x1[0] = ((x1[0]+x1[1])/2) - hw*cos(atan(((((x1[0]+x1[1])/2)-mx)/((1.0)*(((y1_new[0]+y1_new[1])/2)-my)))));
                    x1[1] = ((x1[0]+x1[1])/2) + hw*cos(atan(((((x1[0]+x1[1])/2)-mx)/((1.0)*(((y1_new[0]+y1_new[1])/2)-my)))));

                    y1_new[0] = ((y1_new[0]+y1_new[1])/2) + hw*sin(atan(((((x1[0]+x1[1])/2)-mx)/((1.0)*(((y1_new[0]+y1_new[1])/2)-my)))));
                    y1_new[1] = ((y1_new[0]+y1_new[1])/2) - hw*sin(atan(((((x1[0]+x1[1])/2)-mx)/((1.0)*(((y1_new[0]+y1_new[1])/2)-my)))));}*/

                    //if(i==0){y1_new[1] -= 50 ;}
                    //if(i==1){y1_new[1] += 500 ;}
                

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
                iDrawWall(sx[0],sx[1],sz[0],sz[1],sz[2],sz[3],W[w].red,W[w].green,W[w].blue,s,i,w);
            }
            S[s].d/=(S[s].we-S[s].ws);  
            
        }
    }
        iShowBMP2(790,80,gun_image,0);
        iShowBMP2(910,550,"shotgun\\crosshair.bmp",0);
        iShowBMP2(0,0,"shotgun\\status_bar.bmp",-1);
        iSetColor(100,255,100);
        iText(100,900,scorestring,GLUT_BITMAP_TIMES_ROMAN_24);
        iText(1500,900,healthstring,GLUT_BITMAP_TIMES_ROMAN_24);
        iText(800,900,timerstring,GLUT_BITMAP_TIMES_ROMAN_24);
        //testTextures();
    }
    if(score==3 && menu==1){
        iSetColor(100,255,100);
        iText(800,600,"GAME OVER");
        if(leadcount==0){update_leaderboard(); leadcount++;}
    }
    if(menu==2){
        iShowBMP(0,0,"shotgun\\leaderboard.bmp");
        iSetColor(100,255,100);
        iText(1000,700,leaderstring[0],GLUT_BITMAP_TIMES_ROMAN_24);
        iText(1000,600,leaderstring[1],GLUT_BITMAP_TIMES_ROMAN_24);
        iText(1000,500,leaderstring[2],GLUT_BITMAP_TIMES_ROMAN_24);
        iText(1000,400,leaderstring[3],GLUT_BITMAP_TIMES_ROMAN_24);
        iText(1000,300,leaderstring[4],GLUT_BITMAP_TIMES_ROMAN_24);
    }
}



void iDrawLine(int sx0, int sx1, int sz0, int sz1) { 

    for(int screenx=sx0; screenx<=sx1; screenx++)
    {
        int screenz = ((sz1-sz0)*(screenx-sx0+0.5))/(sx1-sx0)+sz0;
        iPoint(screenx,screenz);
    }

}

void iDrawWall(int sx0, int sx1, int sz0, int sz1, int sz2, int sz3, int red, int green, int blue, int s, int i, int w) {

    int screenx,screenz;
    int wt = W[w].wt; if(wt==200){wt=jagind[0];} if(wt==201){wt=jagind[1];} if(wt==202){wt=jagind[2];}
    float ht = 0, ht_step=(float)Textures[wt].w*W[w].u/(float)(sx1-sx0);

    int dy = sz1 - sz0;
    int dz = sz3 - sz2;
    int dx = sx1 - sx0; if(dx==0){dx=1;}
    int sx0backup = sx0;

    if(sx0<1){ht-=ht_step*sx0; sx0=1;}
    if(sx1<1){sx1=1;}
    if(sx0>1920){sx0=1920;}
    if(sx1>1920){sx1=1920;}

    for(screenx=sx0; screenx<sx1; screenx++){

        int screenz1 = dy*(screenx-sx0backup+0.5)/dx + sz0;
        int screenz2 = dz*(screenx-sx0backup+0.5)/dx + sz2;

        float vt = 0, vt_step=(float)Textures[wt].h*W[w].v/(float)(screenz2-screenz1);
 
        if(screenz1<1){vt-=vt_step*screenz1; screenz1=1;}
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
            for(int screenz=screenz1; screenz<screenz2; screenz++){ 
                int flag=(Textures[wt].h-((int)vt%Textures[wt].h)-1)*3*Textures[wt].w+((int)ht%Textures[wt].w)*3;
                int red=Textures[wt].name[flag]-W[w].shade/2 ; if(red<0){red=0;} 
                int green=Textures[wt].name[flag+1]-W[w].shade/2 ; //if(green<0){green=0;}
                int blue=Textures[wt].name[flag+2]-W[w].shade/2 ; //if(blue<0){blue=0;}
                if((W[w].wt==200||W[w].wt==201||W[w].wt==202) && 95<red && red<126 && 110<green && green<149 && 120<blue && blue<160){}
                else{iSetColor(red,green,blue);  //debug++; if(debug%100000000==0){printf("%d %d %d %d\n",red,green,blue,flag);}
                iPoint(screenx,screenz);}
                vt+=vt_step;
            }
            ht+=ht_step;
        } 

        if(i==1)
        {
            if(S[s].toporbottom==1){ screenz2 = S[s].points[screenx]; iSetColor(100,78,90);}
            if(S[s].toporbottom==2){ screenz1 = S[s].points[screenx]; iSetColor(100,78,90);}
            for(int screenz=screenz1; screenz<screenz2; screenz++){iPoint(screenx,screenz);}
        }

    }
    
}

void inittexture(){
    Textures[ 0].name=(const unsigned char*)T_00; Textures[ 0].h=T_00_HEIGHT; Textures[ 0].w=T_00_WIDTH; 
    Textures[ 1].name=(const unsigned char*)T_01; Textures[ 1].h=T_01_HEIGHT; Textures[ 1].w=T_01_WIDTH;
    Textures[ 2].name=(const unsigned char*)T_02; Textures[ 2].h=T_02_HEIGHT; Textures[ 2].w=T_02_WIDTH;
    Textures[ 3].name=(const unsigned char*)T_03; Textures[ 3].h=T_03_HEIGHT; Textures[ 3].w=T_03_WIDTH;
    Textures[ 4].name=(const unsigned char*)T_04; Textures[ 4].h=T_04_HEIGHT; Textures[ 4].w=T_04_WIDTH;
    Textures[ 5].name=(const unsigned char*)T_05; Textures[ 5].h=T_05_HEIGHT; Textures[ 5].w=T_05_WIDTH;
    Textures[ 6].name=(const unsigned char*)T_06; Textures[ 6].h=T_06_HEIGHT; Textures[ 6].w=T_06_WIDTH;
    Textures[ 7].name=(const unsigned char*)T_07; Textures[ 7].h=T_07_HEIGHT; Textures[ 7].w=T_07_WIDTH;
    Textures[ 8].name=(const unsigned char*)T_08; Textures[ 8].h=T_08_HEIGHT; Textures[ 8].w=T_08_WIDTH;
    Textures[ 9].name=(const unsigned char*)T_09; Textures[ 9].h=T_09_HEIGHT; Textures[ 9].w=T_09_WIDTH;
    Textures[10].name=(const unsigned char*)T_10; Textures[10].h=T_10_HEIGHT; Textures[10].w=T_10_WIDTH;
    Textures[11].name=(const unsigned char*)T_11; Textures[11].h=T_11_HEIGHT; Textures[11].w=T_11_WIDTH;
    Textures[12].name=(const unsigned char*)T_12; Textures[12].h=T_12_HEIGHT; Textures[12].w=T_12_WIDTH;
    Textures[13].name=(const unsigned char*)T_13; Textures[13].h=T_13_HEIGHT; Textures[13].w=T_13_WIDTH;
    Textures[14].name=(const unsigned char*)T_14; Textures[14].h=T_14_HEIGHT; Textures[14].w=T_14_WIDTH;
    Textures[15].name=(const unsigned char*)T_15; Textures[15].h=T_15_HEIGHT; Textures[15].w=T_15_WIDTH;
    Textures[16].name=(const unsigned char*)T_16; Textures[16].h=T_16_HEIGHT; Textures[16].w=T_16_WIDTH;
    Textures[17].name=(const unsigned char*)T_17; Textures[17].h=T_17_HEIGHT; Textures[17].w=T_17_WIDTH;
    Textures[18].name=(const unsigned char*)T_18; Textures[18].h=T_18_HEIGHT; Textures[18].w=T_18_WIDTH;
    Textures[19].name=(const unsigned char*)T_19; Textures[19].h=T_19_HEIGHT; Textures[19].w=T_19_WIDTH;
    Textures[20].name=(const unsigned char*)T_20; Textures[20].h=T_20_HEIGHT; Textures[20].w=T_20_WIDTH;
    Textures[21].name=(const unsigned char*)T_21; Textures[21].h=T_21_HEIGHT; Textures[21].w=T_21_WIDTH;
    Textures[22].name=(const unsigned char*)T_22; Textures[22].h=T_22_HEIGHT; Textures[22].w=T_22_WIDTH;
    Textures[23].name=(const unsigned char*)T_23; Textures[23].h=T_23_HEIGHT; Textures[23].w=T_23_WIDTH;
    Textures[24].name=(const unsigned char*)T_24; Textures[24].h=T_24_HEIGHT; Textures[24].w=T_24_WIDTH;
    Textures[25].name=(const unsigned char*)T_25; Textures[25].h=T_25_HEIGHT; Textures[25].w=T_25_WIDTH;
    Textures[26].name=(const unsigned char*)T_26; Textures[26].h=T_26_HEIGHT; Textures[26].w=T_26_WIDTH;
    Textures[27].name=(const unsigned char*)T_27; Textures[27].h=T_27_HEIGHT; Textures[27].w=T_27_WIDTH;
    Textures[28].name=(const unsigned char*)T_28; Textures[28].h=T_28_HEIGHT; Textures[28].w=T_28_WIDTH;
    Textures[29].name=(const unsigned char*)T_29; Textures[29].h=T_29_HEIGHT; Textures[29].w=T_29_WIDTH;
    Textures[30].name=(const unsigned char*)T_30; Textures[30].h=T_30_HEIGHT; Textures[30].w=T_30_WIDTH;
    Textures[31].name=(const unsigned char*)T_31; Textures[31].h=T_31_HEIGHT; Textures[31].w=T_31_WIDTH;
    Textures[32].name=(const unsigned char*)T_32; Textures[32].h=T_32_HEIGHT; Textures[32].w=T_32_WIDTH;
    Textures[33].name=(const unsigned char*)T_33; Textures[33].h=T_33_HEIGHT; Textures[33].w=T_33_WIDTH;
    Textures[34].name=(const unsigned char*)T_34; Textures[34].h=T_34_HEIGHT; Textures[34].w=T_34_WIDTH;
    Textures[35].name=(const unsigned char*)T_35; Textures[35].h=T_35_HEIGHT; Textures[35].w=T_35_WIDTH;
    Textures[36].name=(const unsigned char*)T_36; Textures[36].h=T_36_HEIGHT; Textures[36].w=T_36_WIDTH;
    Textures[37].name=(const unsigned char*)T_37; Textures[37].h=T_37_HEIGHT; Textures[37].w=T_37_WIDTH;
    Textures[38].name=(const unsigned char*)T_38; Textures[38].h=T_38_HEIGHT; Textures[38].w=T_38_WIDTH;
    Textures[39].name=(const unsigned char*)T_39; Textures[39].h=T_39_HEIGHT; Textures[39].w=T_39_WIDTH;
    Textures[40].name=(const unsigned char*)T_40; Textures[40].h=T_40_HEIGHT; Textures[40].w=T_40_WIDTH;
    Textures[41].name=(const unsigned char*)T_41; Textures[41].h=T_41_HEIGHT; Textures[41].w=T_41_WIDTH;
    Textures[42].name=(const unsigned char*)T_42; Textures[42].h=T_42_HEIGHT; Textures[42].w=T_42_WIDTH;
    
    /*char texture_name[100];
    char texture_height[100];
    char texture_width[100];
    for (int i = 0; i < 20; i++) {
        sprintf(texture_name, "T_%02d", i);
        sprintf(texture_height, "T_%02d_HEIGHT", i);
        sprintf(texture_width, "T_%02d_WIDTH", i);
        Textures[i].name = (const unsigned char*) texture_name;
        Textures[i].h = texture_height ;
        Textures[i].w = texture_width ;
    }*/

}

void initialize_leaderboard(){
    FILE *fp3 = fopen("leaderboard.h","r");
    if(fp3 == NULL){return;}

    for(int i=0; i<5; i++){
        fscanf(fp3,"%d",&leaderboard[i]);
        sprintf(leaderstring[i],"%02d:%02d:%02d",(leaderboard[i]/3600),(leaderboard[i]%3600)/60,(leaderboard[i]%60));
    }
    fclose(fp3);
}

void update_leaderboard(){
    FILE *fp1 = fopen("leaderboard.h","r");
    if(fp1 == NULL){return;}

    for(int i=0; i<5; i++){
        fscanf(fp1,"%d",&leaderboard[i]);
    }
    fclose(fp1);

    FILE *fp2 = fopen("leaderboard.h","w");
    if(fp2 == NULL){return;}
    int leadflag=0;

    for(int i=0; i<5; i++){
        if(leaderboard[i]>timer && leadflag==0){
            fprintf(fp2,"%d ",timer); leadflag++;
            fprintf(fp2,"%d ",leaderboard[i]);
        }
        else{fprintf(fp2,"%d ",leaderboard[i]);}
    }
    fclose(fp2);

    FILE *fp3 = fopen("leaderboard.h","r");
    if(fp3 == NULL){return;}

    for(int i=0; i<5; i++){
        fscanf(fp3,"%d",&leaderboard[i]);
        sprintf(leaderstring[i],"%02d:%02d:%02d",(leaderboard[i]/3600),(leaderboard[i]%3600)/60,(leaderboard[i]%60));
    }
    fclose(fp3);

}

void populate_gun_images(){
    sprintf(gun_idle, "shotgun\\file_0-triangle.bmp");
    for(int i=0; i<15; i++){
        sprintf(gun_fire[i], "shotgun\\file_%d-triangle.bmp",i);
    }
    gun_image = gun_idle;
}

void populate_menu_images(){
    for(int i=0; i<2000; i++){
        sprintf(menupics[i], "menu\\%05d.bmp",i+1);
    }
   
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

void update_menu(){
   
            menupic = menupics[menu_idx];
            menu_idx = (menu_idx + 1) % 2000; 

}

void update_jaguar(){

        for(int i=0; i<2; i++){
            if(jstate[i]==jIDLE) {jagind[i]=20;}
        else if(jstate[i]==jRUN) {jagind[i]++; if(jagind[i]>26){jagind[i]=23;}}
        else if(jstate[i]==jFIRE) {jagind[i]++; if(jagind[i]>22){jagind[i]=20;} if(jagind[i]==21){health--; sprintf(healthstring,"HEALTH: %d",health);}}
        else if(jstate[i]==jDEATH) {jagind[i]++; if(jagind[i]>31){jagind[i]=31;} if(jagind[i]==29){score++; sprintf(scorestring,"SCORE: %d",score);}}
        }

        for(int i=2; i<3; i++){
            if(jstate[i]==jIDLE) {jagind[i]=32;}
        else if(jstate[i]==jRUN) {jagind[i]++; if(jagind[i]>38){jagind[i]=35;}}
        else if(jstate[i]==jFIRE) {jagind[i]++; if(jagind[i]>34){jagind[i]=32;} if(jagind[i]==33){health--; sprintf(healthstring,"HEALTH: %d",health);}}
        else if(jstate[i]==jDEATH) {jagind[i]++; if(jagind[i]>42){jagind[i]=42;} if(jagind[i]==39){score++; sprintf(scorestring,"SCORE: %d",score);}}
        }

        /*if(jstate[0]==jIDLE) {jagind[0]++; if(jagind[0]>26){jagind[0]=23;}}
        else if(jstate[0]==jFIRE) {jagind[0]++; if(jagind[0]>22){jagind[0]=20;}}
        else if(jstate[0]==jDEATH) {jagind[0]++; if(jagind[0]>31){jagind[0]=31;}}

        if(jstate[1]==jIDLE) {jagind[1]++; if(jagind[1]>26){jagind[1]=23;}}
        else if(jstate[1]==jFIRE) {jagind[1]++; if(jagind[1]>22){jagind[1]=20;}}
        else if(jstate[1]==jDEATH) {jagind[1]++; if(jagind[1]>31){jagind[1]=31;}}

        if(jstate[2]==jIDLE) {jagind[2]++; if(jagind[2]>26){jagind[2]=23;}}
        else if(jstate[2]==jFIRE) {jagind[2]++; if(jagind[2]>22){jagind[2]=20;}}
        else if(jstate[2]==jDEATH) {jagind[2]++; if(jagind[2]>31){jagind[2]=31;}}*/
   
}

void update_jstate(){
    for(int i=0; i<3; i++){
        if(jstate[i]!=jDEATH){
        int seeflag=0;
        int jx0 = W[S1[i+101].ws].wx0;
        int jx1 = W[S1[i+101].ws].wx1;
        int jx = (jx0+jx1)/2;
        int jy = W[S1[i+101].ws].wy0;
        for(int k=my; k<jy; k++){
            int j = mx + (jx0-mx)*(k-my)/(jy-my);
            if(access[j][k]==1){seeflag++; printf("j %d k %d mx %d my %d jx %d jy %d \n",j,k,mx,my,jx,jy); jstate[i]=jIDLE; break;}
        }
        if(seeflag==0){
            runorfire[i]=rand()%6;
            if(runorfire[i]==5){jstate[i]=jFIRE;}
            else{jstate[i]=jRUN;}
        }
        }
    }
}

void iKeyboard(unsigned char key) {
	if (key == 'j') {t-=0.035; if(t<0){t+=6.30;}}
    if( key == 'l') {t+=0.035; if(t>6.30){t-=6.30;}}
    if (key == 'i') {g-=1;}
    if (key == 'k') {g+=1;}

    int dx= sin(t)*7;
    int dy= cos(t)*7;

    if (key == 'a') {if(access[mx-dy][my+dx]==0){mx-=dy; my+=dx;}}
    if (key == 'd') {if(access[mx+dy][my-dx]==0){mx+=dy; my-=dx;}}
    if (key == 'w') {if(access[mx+dx][my+dy]==0){mx+=dx; my+=dy;}}
    if (key == 's') {if(access[mx-dx][my-dy]==0){mx-=dx; my-=dy;}}

    if(key == 't') {mz+=4;}
    if(key == 'g') {mz-=4;}

    if(key == 'f') {
        if(state!=FIRE){
        state=FIRE; 
        
        for(int i=0; i<3; i++){
            if(jstate[i]!=jIDLE){
                state=FIRE; 
                int bx = (W[S1[i+101].ws].wy0-my)*tan(t) + mx; 
                if(bx<(W[S1[i+101].ws].wx1-2) && bx>(W[S1[i+101].ws].wx0+12) && jstate[i]!=jDEATH){
                    jstate[i]=jDEATH; 
                    if(i==2){jagind[i]=35;}
                    else{jagind[i]=24;}
                }
            }
        }
        }
    }

    if(key == 'b') {for(int i=0; i<1000; i++){for(int j=0; j<1000; j++){access[i][j]=0;}} load(); /*for(int i=0; i<600; i++){ int sum=0; for(int j=0; j<=600; j++){sum+=access[i][j];}printf("%d\n",sum);}*/ }
    if(key == 'c') {menu=1; score=0; timer=0; health=100; for(int i=0; i<3; i++){jstate[i]=jRUN; jagind[i]=23;} sprintf(healthstring,"HEALTH: %d",health); sprintf(scorestring,"SCORE: %d",score); load();} 
    if(key == 'q' && score==3 && menu==1) {menu=2;}
    if(key == 'h' && menu==0) {menu=2;}
    if(key == 'x' && menu==2) {menu=0;} 

    /*if(key == 'v') {for(int i=0; i<3; i++){jstate[i]=jRUN; jagind[i]=23;}}
    if(key == 'n') {for(int i=0; i<3; i++){jstate[i]=jFIRE; jagind[i]=20;}}
    if(key == 'm') {for(int i=0; i<3; i++){jstate[i]=jDEATH; jagind[i]=27;}}*/

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

void update_time(){
    timer++;
    sprintf(timerstring,"%02d:%02d:%02d",(timer/3600),(timer%3600)/60,(timer%60));
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
    initialize_leaderboard();
    srand(time(0));
    inittexture();
    populate_gun_images();
    populate_menu_images();
    iSetTimer(100, update_gun);
    iSetTimer(57, update_menu);
    iSetTimer(150,update_jaguar);
    iSetTimer(600,update_jstate);
    iSetTimer(17,update_time);
    //iSetTimer(3000, check);
    mx=0,my=0,mz=0;
    //float t=0,g=0;

    iInitialize(1920,1040, "demo");
    return 0;
}