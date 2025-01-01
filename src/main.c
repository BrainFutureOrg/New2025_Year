#include<stdio.h>
#include<termios.h>
#include"libraries/terminal_bfo/colors_bfo/colors.h"
#include"libraries/terminal_bfo/terminal_funcs.h"
#include<sys/ioctl.h>
#include <unistd.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    }
    while (res && errno == EINTR);

    return res;
}

void fill_bg(struct winsize size)
{
        
    terminal_goto_xy(0, 0);
    color_to_rgb_background(40,50,60);
    for (int i = 0; i < size.ws_row; i++)
    {
        for (int j = 0; j < size.ws_col; j++)
        {
            printf(" ");
        }
    }
}

struct spark{
    double x,y,v_x,v_y,v_multiplier;
    unsigned char r,g,b;
    char visible,moving;
    char c;
};

struct firework{
    int len;
    struct spark* sparks;
    int offset;
};

void draw_firework(struct firework f, struct winsize size, char* img, int img_w, int img_h){
    for(;f.len>0;f.sparks++,f.len--){
        if(f.sparks->visible){
            int x_i = (int)f.sparks->x, y_i = (int)f.sparks->y;
            int x=x_i+size.ws_col/2, y=y_i+size.ws_row/2;
            if(x>0&&x<=size.ws_col&&y>0&&y<=size.ws_row){
                terminal_goto_xy(x,y);
                color_to_rgb_foreground(f.sparks->r, f.sparks->g, f.sparks->b);
                putchar(f.sparks->c);
            }
            if(f.sparks->moving){
                int x_2 = x_i + img_w/2, y_2 =y_i+img_h/2;
                if(x_2>=0&&x_2<img_w&&y_2>=0&&y_2<img_h&&img[x_2+y_2*img_w]=='1'&&rand()<RAND_MAX/(10+15*(f.sparks->v_x*f.sparks->v_x+f.sparks->v_y*f.sparks->v_y))){
                    f.sparks->moving=0;
                }
                else if(rand()<RAND_MAX/20){
                    f.sparks->visible=0;
                }else{
                    f.sparks->x+=f.sparks->v_x;
                    f.sparks->y+=f.sparks->v_y;
                    f.sparks->v_x*=f.sparks->v_multiplier;
                    f.sparks->v_y*=f.sparks->v_multiplier;
                }
            }
        }
    }
}

struct firework create_firework(int x,int y, int offset, unsigned char *color_settings, int n, double v_max){
    struct firework r;
    r.len=n;
    r.offset=offset;
    r.sparks = malloc(sizeof(struct spark)*r.len);
    for(int i=0; i<r.len; i++){
        r.sparks[i].r=color_settings[0]+color_settings[1]*(double)rand()/RAND_MAX;
        r.sparks[i].g=color_settings[2]+color_settings[3]*(double)rand()/RAND_MAX;
        r.sparks[i].b=color_settings[4]+color_settings[5]*(double)rand()/RAND_MAX;
        r.sparks[i].c='*';
        r.sparks[i].x=x;
        r.sparks[i].y=y;
        float a = rand()/(double)RAND_MAX*2*M_PI;
        float v = rand()/(double)RAND_MAX*v_max;
        r.sparks[i].v_x=cos(a)*v*2;
        r.sparks[i].v_y=sin(a)*v;
        r.sparks[i].v_multiplier=0.93;
        r.sparks[i].visible=1;
        r.sparks[i].moving=1;
    }
    return r;
}

void draw(int frame_no, struct firework *f, int firework_n,char *img, int img_w, int img_h){
    struct winsize size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
    fill_bg(size);
    for (int i=0; i<firework_n; i++){
        if(!f[i].offset)draw_firework(f[i], size, img, img_w, img_h);
    }
}

char *img=
"1111111111111111000011111111111111110000111111111111111100001111111111111111"
"1111111111111111000011111111111111110000111111111111111100001111111111111111"
"0000000000001111000011110000000011110000000000000000111100001111000000000000"
"0000000000001111000011110000000011110000000000000000111100001111000000000000"
"0000000000001111000011110000000011110000000000000000111100001111000000000000"
"0000000000001111000011110000000011110000000000000000111100001111000000000000"
"1111111111111111000011110000000011110000111111111111111100001111111111111111"
"1111111111111111000011110000000011110000111111111111111100001111111111111111"
"1111000000000000000011110000000011110000111100000000000000000000000000001111"
"1111000000000000000011110000000011110000111100000000000000000000000000001111"
"1111000000000000000011110000000011110000111100000000000000000000000000001111"
"1111000000000000000011110000000011110000111100000000000000000000000000001111"
"1111111111111111000011111111111111110000111111111111111100001111111111111111"
"1111111111111111000011111111111111110000111111111111111100001111111111111111"
;// bad code style? Good code portability

int img_w = 76,img_h=14;

int main(){
    unsigned char color_settings_1[] ={100,155,0,155,120,135};
    unsigned char color_settings_2[] ={70,150,0,120,170,85};
    unsigned char color_settings_3[] ={170,85,50,205,0,120};
    unsigned char color_settings_4[] ={50,100,150,105,80,100};
    unsigned char color_settings_5[] ={150,105,50,120,0,120};
    struct firework f[] = {
        create_firework(0,0,0,color_settings_1,2000,4),
        create_firework(30,-5,20,color_settings_2,2000,4),
        create_firework(-30,-3,40,color_settings_3,2000,4),
        create_firework(20,6,60,color_settings_4,800,2),
        create_firework(-22,6,75,color_settings_5,800,2),
        create_firework(4,-7,90,color_settings_2,800,2),
    };
    terminal_invisible_cursor;
    
    for(int i=0; i<100000; i++){
        for(int j=0; j<6; j++){
            if(f[j].offset)f[j].offset--;
        }
        draw(i,&f[0], 6,img, img_w, img_h);
        fflush(stdout);
        msleep(100);
    }
    terminal_visible_cursor;
}
