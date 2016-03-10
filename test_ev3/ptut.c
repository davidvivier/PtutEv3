#include<stdio.h>
#include<stdlib.h>


void droite()
{
    int d = 130;
    int vd = 128, vg = 66;
    while(d<254){
        d = getDistance(d,vd,vg);
        printf("La distance est %d, Moteur G : %d, Moteur D%d",d,vg,vd);
        if (d<105){
            vg = acceler(vg,(128-d)*(128-d)/2);
            vd = 128
        }
        if (d>151){
            vd = acceler(vd,(d-128)*(d-128)/2);
            vg = 128
        }

    }
}
/*
void aveugle()
{

}
*/
void acceler(int moteur, int acc){
    if(moteur+acc<0)
        return 0;
    if(moteur+acc<255)
        return moteur+acc;
    else
        return 255;
}


int getDistance(int d, int vd, int vg){
    return d + (vd/4) - (vg/4);
}

int main(int argc, char* argv[])
{
    droite()
}
