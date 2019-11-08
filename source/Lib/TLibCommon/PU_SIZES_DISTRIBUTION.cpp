#include <stdio.h>
#include "PU_SIZES_DISTRIBUTION.h"

extern float hitRate;

void calculateMaxDepths(int frameHeight){
    
    switch(frameHeight){
        case 1664:
            PUs_Cumulative_4x4 = Res1664_PUs_Cumulative_4x4;
            PUs_Cumulative_8x8 = Res1664_PUs_Cumulative_8x8;
            PUs_Cumulative_16x16 = Res1664_PUs_Cumulative_16x16;
            PUs_Cumulative_32x32 = Res1664_PUs_Cumulative_32x32;
            PUs_Cumulative_64x64 = Res1664_PUs_Cumulative_64x64;
            MAX_DEPTH_PER_ROW = Res1664_MAX_DEPTH_PER_ROW;
            break;
        case 2048:
            PUs_Cumulative_4x4 = Res2048_PUs_Cumulative_4x4;
            PUs_Cumulative_8x8 = Res2048_PUs_Cumulative_8x8;
            PUs_Cumulative_16x16 = Res2048_PUs_Cumulative_16x16;
            PUs_Cumulative_32x32 = Res2048_PUs_Cumulative_32x32;
            PUs_Cumulative_64x64 = Res2048_PUs_Cumulative_64x64;
            MAX_DEPTH_PER_ROW = Res2048_MAX_DEPTH_PER_ROW;
            break;
        default:
            cout << "ERROR -- Resolution " << frameHeight << " is not supported" << endl;
            break;
    }
    
    // Number of CTU rows
    int numRows = frameHeight/64;
    
    for(int i=0; i<numRows; i++){
  
        if(PUs_Cumulative_64x64[i] >= hitRate){
            MAX_DEPTH_PER_ROW[i] = 0;
        }
        else if(PUs_Cumulative_32x32[i] >= hitRate){
            MAX_DEPTH_PER_ROW[i] = 1;
        }
        else if(PUs_Cumulative_16x16[i] >= hitRate){
            MAX_DEPTH_PER_ROW[i] = 2;
        }
        else if(PUs_Cumulative_8x8[i] >= hitRate){
            MAX_DEPTH_PER_ROW[i] = 3;
        }
        else if(PUs_Cumulative_4x4[i] >= hitRate){
            MAX_DEPTH_PER_ROW[i] = 4;
        }
        else{
            cout << "ERROR -- UNREACHABLE HIT RATE FOR INTRA PU SIZES" << endl;
        }    
        // cout << "Row " << i << " Max " << MAX_DEPTH_PER_ROW[i];
    }
}