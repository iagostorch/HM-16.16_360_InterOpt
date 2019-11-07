#include <stdio.h>
#include "PU_SIZES_DISTRIBUTION.h"

extern float hitRate;

void calculateMaxDepths(){
    
    for(int i=0; i<26; i++){
        if(Res1664_PUs_Cumulative_64x64[i] >= hitRate){
            Res1664_MAX_DEPTH_PER_ROW[i] = 0;
        }
        else if(Res1664_PUs_Cumulative_32x32[i] >= hitRate){
            Res1664_MAX_DEPTH_PER_ROW[i] = 1;
        }
        else if(Res1664_PUs_Cumulative_16x16[i] >= hitRate){
            Res1664_MAX_DEPTH_PER_ROW[i] = 2;
        }
        else if(Res1664_PUs_Cumulative_8x8[i] >= hitRate){
            Res1664_MAX_DEPTH_PER_ROW[i] = 3;
        }
        else if(Res1664_PUs_Cumulative_4x4[i] >= hitRate){
            Res1664_MAX_DEPTH_PER_ROW[i] = 4;
        }
        else{
            cout << "ERROR -- UNREACHABLE HIT RATE FOR INTRA PU SIZES" << endl;
        }       
    }
}