#include<iostream>
#include "include/iGraphics.h"
#include<math.h>
#include<string.h>
#include<fstream>
#define pi 3.14159
#define screen_width 1280
#define screen_height 720
#define gravity 9.8/100
using namespace std;
void tank2_controller();

bool twoplayer=0,get_HighScore=1,bg_sound=1;
char bg_images[10][30] = {"images//Home_page//0.bmp","images//Home_page//1.bmp","images//Home_page//2.bmp","images//Home_page//3.bmp","images//Home_page//4.bmp","images//Home_page//5.bmp","images//Home_page//6.bmp","images//Home_page//7.bmp","images//Home_page//8.bmp"};
char click_images[4][30]={"images//play.bmp","images//high_score.bmp","images//about.bmp","images//exit.bmp"};
char lifer[27][30]={"images//lifer//0.bmp","images//lifer//1.bmp","images//lifer//2.bmp","images//lifer//3.bmp","images//lifer//4.bmp","images//lifer//5.bmp","images//lifer//6.bmp","images//lifer//7.bmp","images//lifer//8.bmp","images//lifer//9.bmp","images//lifer//10.bmp","images//lifer//11.bmp","images//lifer//12.bmp","images//lifer//13.bmp","images//lifer//14.bmp","images//lifer//15.bmp","images//lifer//16.bmp","images//lifer//17.bmp","images//lifer//18.bmp","images//lifer//19.bmp","images//lifer//20.bmp","images//lifer//21.bmp","images//lifer//22.bmp","images//lifer//23.bmp","images//lifer//24.bmp","images//lifer//25.bmp","images//lifer//26.bmp"};
char lifel[27][30]={"images//lifel//0.bmp","images//lifel//1.bmp","images//lifel//2.bmp","images//lifel//3.bmp","images//lifel//4.bmp","images//lifel//5.bmp","images//lifel//6.bmp","images//lifel//7.bmp","images//lifel//8.bmp","images//lifel//9.bmp","images//lifel//10.bmp","images//lifel//11.bmp","images//lifel//12.bmp","images//lifel//13.bmp","images//lifel//14.bmp","images//lifel//15.bmp","images//lifel//16.bmp","images//lifel//17.bmp","images//lifel//18.bmp","images//lifel//19.bmp","images//lifel//20.bmp","images//lifel//21.bmp","images//lifel//22.bmp","images//lifel//23.bmp","images//lifel//24.bmp","images//lifel//25.bmp","images//lifel//26.bmp"};
int home_click_val=0;
int game_state = -1;
int i,effect=0,img_nmbr = -1,seq = 0;
int tank1_x,tank2_x,life_1=2,life_2=2;
int tank1_score,tank2_score;
int change_counter;
int h_score;
char H_score[100];

string set_score(char s[],int n){
    char arr[100];
    int p=0;
    while(s[p]!='\0'){
        arr[p]=s[p];
            p++;
    }p--;
    arr[p++]=' ';
    arr[p++]=(n/1000)+'0';
    n%=1000;
    arr[p++]=(n/100)+'0';
    n%=100;
    arr[p++]=(n/10)+'0';
    n%=10;
    arr[p++]=n+'0';
    arr[p]='\0';
    return arr;
}

class prashs{
private:
    int bx,by,angle=45,radius=0;
    int ix,iy;
    int v,timechange=1;
    int timeMax=0,rMax=0;
    bool shoot=0;
public:
    void end(){
        shoot=0;
        timeMax=0,rMax=0;
        timechange=1;
        radius=0;
    }
    void shoot_prash(int x,int y,int ve=10,int a=45){
        bx=ix=x;
        by=iy=y;
        angle=a;
        v=ve;
        timechange=1;
        timeMax=v*sin(angle*pi/180)/gravity;
        rMax=v*v*sin(2*angle*pi/180)/gravity;
        shoot=1;
    }void change_position(){
        if(shoot){
            bx=v*cos(angle*pi/180)*timechange;///x=v0*cos(angle)*t
            by=bx*tan(angle*pi/180)-0.5*bx*bx*gravity/(v*v*cos(angle*pi/180)*cos(angle*pi/180));
            ///y=xtan(angle)-(g*x2/v2*cos2(angle))
            bx+=ix;
            by+=iy;
            if(timechange!=timeMax){
                timechange++;
            }if(by<50){
                timechange=1;
                timeMax=0;
                bx=ix,by=iy;
                shoot=radius=0;
            }
            if(bx>=tank1_x+20&&bx<=tank1_x+150&&by<145&&by>120){
                if(life_1>1){
                    life_1--;
                }
                PlaySound("music\\hit.wav",NULL,SND_ASYNC);
                tank2_score+=10;
                timechange=1;
                timeMax=0;
                bx=ix,by=iy;
                shoot=radius=0;
            }
            if(bx>=tank2_x+20&&bx<=tank2_x+150&&by<160){
                if(life_2>1){
                    life_2--;
                }
                PlaySound("music\\hit.wav",NULL,SND_ASYNC);
                tank1_score+=10;
                timechange=1;
                timeMax=0;
                bx=ix,by=iy;
                shoot=radius=0;
            }
            radius=5;
        }
        else radius=0;
    }void drow(){
        iSetColor(220,20,60);
        iFilledCircle(bx,by,radius);
    }
};

class tank{
private:
    int lradius=50;
    int ix,iy;
    int tx=ix+lradius*cos(angle*pi/180);
    int ty=iy+lradius*sin(angle*pi/180);
    prashs prash[3];
    int prash_number=0;
public:
    int through_time=1;
    int life=5;
    int tscore;
    int v=10;
    double angle=45;
    int tank_x=50,tank_y=50;
    char velocity[100];
    char score[100];
    string vel="Velocity : 0000";
    tank(){
        tank_x=200,tank_y=120;
        position();
    }tank(int x,int y){
        tank_x=x,tank_y=y;
        position();
    }
    tank(int x,int y,int angle){
        tank_x=x,tank_y=y;
        this->angle=angle;
        position();
    }
    void set_tank(int x){
        tscore=0;
        tank_x=x;
        position();
    }
    void set_velocity(){
        vel=set_score("Velocity : ",v);
        for(int i=0;vel[i]!='\0';i++){
            velocity[i]=vel[i];
        }
    }
    void position(){
        ix=tank_x+90;
        iy=tank_y+50;
        if(angle>=180) angle=0;
        tx=ix+lradius*cos(angle*pi/180);
        ty=iy+lradius*sin(angle*pi/180);
        set_velocity();
    }void shoot(){
        if(!through_time){
            if(angle>90){
                PlaySound("music\\shoot1.wav",NULL,SND_ASYNC);
            }else if(angle<40){
                PlaySound("music\\shoot2.wav",NULL,SND_ASYNC);
            }
            else PlaySound("music\\angle0.wav",NULL,SND_ASYNC);
            prash_number++;
            prash_number%=3;
            prash[prash_number].shoot_prash(ix,iy,v,angle);
            if(angle<90){tank_x-=3;}
            else {tank_x+=3;}
            position();
            through_time=20;
        }
    }
    void change(){
        prash[0].change_position();
        prash[1].change_position();
        prash[2].change_position();
        if(through_time>0) through_time--;
        vel=set_score("Score : ",tscore);
        for(int i=0;vel[i]!='\0';i++){
            score[i]=vel[i];
        }
    }
    void end(){
        PlaySound("music\\background.wav",NULL,SND_LOOP|SND_ASYNC);
        prash[0].end();
        prash[1].end();
        prash[2].end();
        prash_number=0;
        twoplayer=0;
        tscore=0;
        life=5;
        v=10;
        if(angle<90){
            tank_x=10,tank_y=130;
        }else tank_x=1000,tank_y=120;
        position();
    }
    void display(){
        prash[0].drow();
        prash[1].drow();
        prash[2].drow();
        iSetColor(60,179,113);
        if(angle<=90){
            iLine(ix,iy,tx,ty);
            iLine(ix+0.5,iy-0.5,tx+0.5,ty-0.5);
            iLine(ix-0.5,iy+0.5,tx-0.5,ty+0.5);
            iLine(ix+1,iy-1,tx+1,ty-1);
            iLine(ix-1,iy+1,tx-1,ty+1);
            iLine(ix+1.5,iy-1.5,tx+1.5,ty-1.5);
            iLine(ix-1.5,iy+1.5,tx-1.5,ty+1.5);
        }else {
            iLine(ix,iy,tx,ty);
            iLine(ix+0.5,iy+0.5,tx+0.5,ty+0.5);
            iLine(ix-0.5,iy-0.5,tx-0.5,ty-0.5);
            iLine(ix+1,iy+1,tx+1,ty+1);
            iLine(ix-1,iy-1,tx-1,ty-1);
            iLine(ix+1.5,iy+1.5,tx+1.5,ty+1.5);
            iLine(ix-1.5,iy-1.5,tx-1.5,ty-1.5);
        }
    }
}tank1(1000,100,130),tank2;

void iDraw()
{
	iClear();
    ///Home page
	if(game_state == -1){
        iShowBMP(0,0,bg_images[img_nmbr]);
        iShowBMP2(880,480,"images//Home_page//ww3.bmp",255);
	}
	/// Pause button
    else if(game_state==0){
        iShowBMP(0,0,click_images[home_click_val]);
        iShowBMP2(880,480,"images//Home_page//ww3.bmp",255);
    }
    /// High score
    else if(game_state=='h'){
        if(!twoplayer){

            iShowBMP(0,0,"images\\H.bmp");
        }
        else if(tank1.life>tank2.life){
            iShowBMP(0,0,"images\\one.bmp");
        }
        else{
            iShowBMP(0,0,"images\\two.bmp");
        }
        if(!twoplayer&&tank1.tscore){
            iText(550,300,"Yout ",GLUT_BITMAP_TIMES_ROMAN_24);
            iText(600,300,tank1.score,GLUT_BITMAP_TIMES_ROMAN_24);
        }if(twoplayer){
            iText(485,300,"Player one",GLUT_BITMAP_TIMES_ROMAN_24);
            iText(600,300,tank1.score,GLUT_BITMAP_TIMES_ROMAN_24);
            iText(485,250,"Player two",GLUT_BITMAP_TIMES_ROMAN_24);
            iText(600,250,tank2.score,GLUT_BITMAP_TIMES_ROMAN_24);
        }
        iText(670,365,H_score,GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(255,0,0);
        iText((screen_width/2)-100,100,"Press to back",GLUT_BITMAP_TIMES_ROMAN_24);
	}
	///chose player
	else if(game_state=='z'){
        iShowBMP(0,0,"images\\chose_player.bmp");
        iText((screen_width/2)-100,260,"One player",GLUT_BITMAP_TIMES_ROMAN_24);
        iRectangle((screen_width/2)-110,250,130,35);
        iText((screen_width/2)-50,200,"Or",GLUT_BITMAP_TIMES_ROMAN_24);
        iText((screen_width/2)-100,150,"Two player",GLUT_BITMAP_TIMES_ROMAN_24);
        iRectangle((screen_width/2)-110,140,130,35);
	}
    /// About us
	else if(game_state == 1){
        iShowBMP(0,0,"images\\about_us.bmp");
	}
	///Play game
	else if(game_state=='p'){
        iShowBMP(0,0,"images\\background.bmp");
        iShowBMP2(tank1.tank_x,tank1.tank_y,"images\\tank_l.bmp",0);
        iShowBMP2(tank2.tank_x,tank2.tank_y,"images\\tank_r.bmp",0);
        iShowBMP2(screen_width/2-110,screen_height-60,"images\\mww3.bmp",255);
        iShowBMP2(screen_width-210,screen_height-35,lifer[tank1.life-1],255);
        iShowBMP2(10,screen_height-35,lifel[tank2.life-1],255);
        iSetColor(128,57,0);
        iText(screen_width-380,screen_height-30,tank1.velocity,GLUT_BITMAP_TIMES_ROMAN_24);
        iText(screen_width-510,screen_height-30,tank1.score,GLUT_BITMAP_TIMES_ROMAN_24);
        iText(230,screen_height-30,tank2.velocity,GLUT_BITMAP_TIMES_ROMAN_24);
        iText(390,screen_height-30,tank2.score,GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(255,255,255);
        iText(screen_width-50,10,"Exit",GLUT_BITMAP_TIMES_ROMAN_24);
        if(life_1==1){
            iFilledCircle(tank1.tank_x,tank1.tank_y,50);
        }
        if(life_1==1){
            iFilledCircle(tank2.tank_x,tank2.tank_y,50);
        }
        tank1.display();
        tank2.display();
        iRectangle(tank1.tank_x+20,120,130,35);
	}
}

void iMouseMove(int mx, int my)
{

}

void iPassiveMouseMove(int mx, int my)
{

}

void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
        ///Homepage menu
        if(game_state==-1){
            if(mx>225 && mx<450 && my<507 && my>464){
                effect = 1;
                game_state='z';
                home_click_val=0;
            }
            else if(mx>180 && mx<400 && my>375 && my<420){
                game_state=0;
                home_click_val=1;
                game_state='h';
            }
            else if(mx>130 && mx<360 && my>290 && my<330){
                game_state=0;
                home_click_val=2;
            }
            else if(mx>95 && mx<325 && my>210 && my<250){
                game_state=0;
                home_click_val=3;
                exit(1);
            }
        }
        else if(game_state==0){
            game_state=-1;
        }
        else if(game_state==1){
            if(mx>1000&&mx<1200&&my>80&&my<120){
                game_state=-1;
            }
        }
        ///chose menu
        else if(game_state=='z'){
            if(mx>=(screen_width/2)-110&&my>=250&&mx<=(screen_width/2)+20&&my<=285){
                twoplayer=0;
                game_state='p';
            }else if(mx>=(screen_width/2)-110&&my>=140&&mx<=(screen_width/2)+20&&my<=175){
                twoplayer=1;
                game_state='p';
            }
        }
        ///Play
        else if(game_state=='p'){
            if(mx>screen_width-100 && mx<screen_width && my>0 && my<350){
                game_state=-1;
                tank1.end();
                tank2.end();
            }
        }///High score
        else if(game_state=='h'){
            life_1=27;
            life_2=27;
            tank1.end();
            tank2.end();
            game_state=-1;
            get_HighScore=1;
            tank1_score=0;
            tank2_score=0;
        }
	}
}


void iKeyboard(unsigned char key)
{
	if(game_state=='p'){
        if(key=='0'){
            tank1.shoot();
        }
        ///second player control
        if(twoplayer){
            if (key == 'w'&&tank2.angle<=80)
            {
                tank2.angle+=5;
                tank2.position();
            }
            else if (key == 's'&&tank2.angle>=40)
            {
                tank2.angle-=5;
                tank2.position();
            }
            else if (key == 'a'&&tank2.tank_x>=10)
            {
                tank2.tank_x-=5;
                tank2.position();
            }
            else if (key == 'd'&&tank2.tank_x<=tank1.tank_x-200)
            {
                tank2.tank_x+=5;
                tank2.position();
            }
            else if (key == 'e'&&tank2.v>1){
                tank2.v--;
                tank2.position();
            }
            else if (key == 'r'){
                tank2.v++;
                tank2.position();
            }
            else if(key=='f'){
                tank2.shoot();
            }
        }
	}
	if(key=='m'||key=='M'){
        if(bg_sound){
            bg_sound=0;
            PlaySound(0,0,0);
        }else {
            bg_sound=1;
            PlaySound("music\\background.wav",NULL,SND_LOOP|SND_ASYNC);
        }
	}
}

void iSpecialKeyboard(unsigned char key)
{
    if(key==GLUT_KEY_HOME){
        tank1.end();
        tank2.end();
        game_state=-1;
    }
    if(game_state=='p'){
        if (key == GLUT_KEY_UP&&tank1.angle>=100)
        {
            tank1.angle-=5;
            tank1.position();
        }
        if (key == GLUT_KEY_DOWN&&tank1.angle<=140)
        {
            tank1.angle+=5;
            tank1.position();
        }
        if (key == GLUT_KEY_LEFT&&tank1.tank_x>tank2.tank_x+200)
        {
            tank1.tank_x-=5;
            tank1.position();
        }
        if (key == GLUT_KEY_RIGHT)
        {
            tank1.tank_x+=5;
            tank1.position();
        }
        if (key == GLUT_KEY_PAGE_DOWN&&tank1.v>1){
            tank1.v--;
            tank1.position();
        }
        if (key == GLUT_KEY_PAGE_UP){
            tank1.v++;
            tank1.position();
        }
    }
}

void change(){
    change_counter++;
    change_counter%=1001;
    if(game_state==0){
        if(home_click_val==3){
            effect++;
            if(effect==8){
                exit(1);
            }
        }else if(home_click_val==2){
            effect++;
            if(effect==8){
                game_state=1;
                effect=0;
            }
        }
    }
    else if(game_state==-1){
        if(seq == 0){
            img_nmbr++;
        }else if(seq == 1){
            img_nmbr--;
        }

        if(img_nmbr == 8){
            seq = 1;
        }
        if(img_nmbr == 2){
            seq = 0;
        }
    }else if(game_state=='p'){
        ///game over condition
        if((tank1.life==1&&!twoplayer)||((tank1.life==1||tank2.life==1)&&twoplayer)){
            game_state='h';
        }
        ///rebirth enemy
        if(tank2.life==1&&!twoplayer){
            tank2.set_tank(0);
            life_2=20;
        }
        tank1.change();
        tank2.change();
        if(!twoplayer){
            tank2_controller();
        }
        tank1_x=tank1.tank_x;
        tank1.tscore=tank1_score;
        tank2_x=tank2.tank_x;
        tank2.tscore=tank2_score;
        tank1.life=life_1;
        tank2.life=life_2;
    }
    ///High score
    else if(game_state=='h'&&get_HighScore){

        get_HighScore=0;
        fstream fob;
        fob.open("highscore.txt",ios::in);
        fob>>h_score;
        fob.close();

        if(tank1.tscore>h_score){
            h_score=tank1.tscore;
            fob.open("highscore.txt",ios::out);
            fob<<tank1.tscore<<endl;
            fob.close();
        }
        string temp;
        temp=set_score(" ",h_score);
        for(int i=0;temp[i]!='\0';i++){
            H_score[i]=temp[i];
        }
    }
}

int move_=500;
int R;
void tank2_controller(){
    if(move_>tank2.tank_x){
        tank2.tank_x++;
        tank2.position();
    }if(move_<tank2.tank_x){
        tank2.tank_x--;
        tank2.position();
    }if(move_==tank2.tank_x){
        move_=rand()%600;
        if(move_<0) move_*=-1;
    }
    if(change_counter%500==0){
        tank2.v=(rand()%10)+8;
        R=tank2.tank_x-tank1.tank_x;
        if(R<0){
            R*=-1;
            R-=200-rand()%100;;
        }
        tank2.angle=(0.5*sinh((R)*gravity/(tank2.v*tank2.v)))*180/pi;
        tank2.shoot();
    }
    if(change_counter%90==0){
        tank2.angle=(rand()%40)+40;
        R=tank2.tank_x-tank1.tank_x;
        if(R<0){
            R*=-1;
        }
        tank2.v=sqrt((R*gravity)/sin(2*tank2.angle*pi/180));
        tank2.shoot();
    }
}

int main()
{
    iSetTimer(50,change);

    //PlaySound("music\\background.wav",NULL,SND_LOOP|SND_ASYNC);
	iInitialize(1280, 720, "WW3");
	return 0;
}
