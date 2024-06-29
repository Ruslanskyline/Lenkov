// Online C compiler to run C program online
#include<stdio.h>
#include "lodepng.c"
#include<math.h>
#include<stdlib.h>
#include<time.h>
#define eps 29

typedef struct point{
    int x,y;
}pt;

void ff(unsigned char* image,int x,int y,int w,int h,int cl1,int cl2,int cl3){
    pt*mas=malloc(4*w*h*sizeof(pt));
    long t=0;
    mas[t++]=(pt){x,y};
    while(t>0){
        pt sd=mas[--t];
        int ind=4*(sd.y*w+sd.x);
        if(image[ind]>eps) continue;
        image[ind]=cl1;
        image[ind+1]=cl2;
        image[ind+2]=cl3;
        if(sd.x>0){
            ind=4*(sd.y *w+sd.x-1);
            if(image[ind]<=eps) mas[t++]=(pt){sd.x-1,sd.y};
        }
        if(sd.y<h-1){
            ind=4*((sd.y+1)*w+sd.x);
            if(image[ind]<=eps) mas[t++]=(pt){sd.x,sd.y+1};
        }
        if(sd.x<w-1){
            ind=4*(sd.y*w+sd.x+1);
            if(image[ind]<=eps) mas[t++]=(pt){sd.x+1,sd.y};
        }
        if(sd.y>0){
            ind=4*((sd.y-1)*w+sd.x);
            if(image[ind]<=eps) mas[t++]=(pt){sd.x,sd.y-1};
        }
    }
    free(mas);
    return;
}

void SchFilter(unsigned char* image,int w,int h){
    int mx[3][3];
    int my[3][3];
    mx[0][0]=4;mx[0][1]=0;mx[0][2]=-4;
    mx[1][0]=8;mx[1][1]=0;mx[1][2]=-8;
    mx[2][0]=4;mx[2][1]=0;mx[2][2]=-4;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++) my[i][j]=mx[j][i];
    unsigned char*rt=malloc(h*w*4*sizeof(unsigned char));
    for(int y=1;y<h-1;y++){
        for(int x=1;x<w-1;x++){
            int sx=0;
            int sy=0;
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    int ind=4*((y+i-1)*w+x+j-1);
                    int ser=(image[ind]+image[ind+1]+image[ind+2])/10;
                    sx+=mx[i][j]*ser;
                    sy+=my[i][j]*ser;
                }
            }
            int g=sqrt(sx*sx+sy*sy);
            if(g<0) g=0;
            if(g>255) g=255;
            int rtind=4*(y*w+x);
            rt[rtind]=(unsigned char) g;
            rt[rtind+1]=(unsigned char) g;
            rt[rtind+2]=(unsigned char) g;
            rt[rtind+3]=image[rtind+3];
        }
    }
    for(int i=0;i<4*w*h;i++) image[i]=rt[i];
    free(rt);
    return;
}

char*png(const char*file,int*w,int*h){
    unsigned char*image=NULL;
    int er=lodepng_decode32_file(&image,w,h,file);
    return(image);
}

int main() {
    int w,h;
    char*file="pic.png";
    char *image = png(file, &w, &h);
    SchFilter(image,w,h);
    for(int y=1;y<h-1;y++)
        for(int x=1;x<w-1;x++)
            if(image[4*(y*w+x)]<eps) {
                int cl1=rand()%(255-eps*2)+eps*2;
                int cl2=rand()%(255-eps*2)+eps*2;
                int cl3=rand()%(255-eps*2)+eps*2;
                ff(image,x,y,w,h,cl1,cl2,cl3);
            }
    char *end_file="end_pic.png";
    lodepng_encode32_file(end_file,image,w,h);
    free(image);
    return 0;
}

