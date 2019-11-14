/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2017, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     encmain.cpp
    \brief    Encoder application main
 */

#include <time.h>
#include <iostream>
#include <fstream>
#include "TAppEncTop.h"
#include "TAppCommon/program_options_lite.h"

// iagostorch begin

// Variables to control the PU size reduction in intra prediction
float hitRate = 1.00;   // Control the precision of the PU size skip technique
int statisticalPUSizeReduction = 1; // Enable the reduction of PU size based on PU size distribution statistics
int extractRDInfo = 1;
ofstream RDs_64x64, RDs_32x32, RDs_16x16, RDs_8x8, RDs_4x4;

// Variables to control the Early Skip technique
int iagoEarlySkip; // Custom encoding parameter. Controls early skip based on block variance
int iagoEarlySkipAdaptiveQP; // Custom encodin parameter. When enabled, variance cutoff for early skip varies according to QP
double *iagoEarlySkipIntegral; // Custom parameter. Controls variance threshold for early skip in each band
double *iagoEarlySkipBandsDistribution; // Custom encoding parameter. Controle the size of each band
int iagoEarlySkipNdivisions;
int iagoIs10bitsVideo = 0; // Detects if video is 10 bits. If it is 10 bits, it must be converted to 8 bits to employ the same variance cutoff
int keyQP;      // Used to index the appropriate variance cutoff when adaptive QP is enabled
Int** samplesMatrix; // Used to store the samples value for future variance calculation

// Variables to control reduced FME schedule
int iagoReducedFME;
int iagoReducedFMENdivisions;
double *iagoReducedFMEBandsDistribution;
Int *iagoReducedFMEBandsHorizontalPrecision;
Int *iagoReducedFMEBandsVerticalPrecision;

// Variables to control reduced search range in raster step
int iagoReducedSR;
int iagoReducedSRNdivisions;
double *iagoReducedSRBandsDistribution;
double *iagoReducedSRBandsScaleVerticalSR;
double *iagoReducedSRBandsScaleHorizontalSR;

// Variables to control the max depth reached in each CTU
int maxDepthMatrixPrevFrame[depthMatrixHeight][depthMatrixWidth];

ofstream mvFile;
ofstream finalCuInfo;
ofstream intermediateCuInfo;

Int extractOnlyRasterPUs = 0;
Int extractTZInfo = 0;
Int extractFinalCuInfo = 0;
Int extractIntermediateCuInfo = 0;


// Variables to track the execution time of some encoding steps
double rasterTime = 0.0;
double checkInterTime = 0.0;
double predInterSearchTime = 0.0;
double xMotionEstimationTime = 0.0;
double xPatternSearchFastTime = 0.0;
double xTZSearchTime = 0.0;
double xPatternSearchTime = 0.0;
double checkIntraTime = 0.0;
double predTime = 0.0;
double firstTime = 0.0;
double refinTime = 0.0;
double calcRdInter = 0.0;
double checkBestModeInter = 0.0;
double unipredTime = 0.0;
double bipredTime = 0.0;
double motionCompTime = 0.0;
double fmeTime = 0.0;
double varTime = 0.0;
double halfGenTime = 0.0;
double quartGenTime = 0.0;
// iagostorch end

//! \ingroup TAppEncoder
//! \{

#include "../Lib/TLibCommon/Debug.h"

// ====================================================================================================================
// Main function
// ====================================================================================================================

int main(int argc, char* argv[]) {
    // iagostorch begin
    if (extractTZInfo) {
        mvFile.open("mvFile.csv");
        mvFile << "PU,Pos,Size,Pred,Inic,Rast,Refi" << endl;
    }
    if (extractFinalCuInfo) {
        finalCuInfo.open("finalPU.csv");
        finalCuInfo << "Frame,CTU#,Pos,Depth,Type,Idx,Merge,Skip,Ref0,MV0,Ref1,MV1" << endl;
    }
    if (extractIntermediateCuInfo) {
        intermediateCuInfo.open("intermediatePU.csv");
        intermediateCuInfo << "Frame,CTU#,Pos,Depth,Type,Idx,Merge,Skip,Ref0,MV0,Ref1,MV1" << endl;
    }
    
    if (extractRDInfo){
        RDs_64x64.open("RD64.csv");
        RDs_64x64 << "Frame,CTU#,PosY,PosX,RD" << endl;
        RDs_32x32.open("RD32.csv");
        RDs_32x32 << "Frame,CTU#,PosY,PosX,RD" << endl;
        RDs_16x16.open("RD16.csv");
        RDs_16x16 << "Frame,CTU#,PosY,PosX,RD" << endl;
        RDs_8x8.open("RD8.csv");
        RDs_8x8 << "Frame,CTU#,PosY,PosX,RD" << endl;
        RDs_4x4.open("RD4.csv");
        RDs_4x4 << "Frame,CTU#,PosY,PosX,RD" << endl;
    }
    
    // Initialize max depth of CTUs with depth 0 (64x64)
    for (int i=0; i<depthMatrixHeight; i++)
        for(int j=0; j<depthMatrixWidth; j++)
            maxDepthMatrixPrevFrame[i][j] = 0;
    
    // iagostorch end
    TAppEncTop cTAppEncTop;

    // print information
    fprintf(stdout, "\n");
    fprintf(stdout, "HM software: Encoder Version [%s] (including RExt)", NV_VERSION);
    fprintf(stdout, NVM_ONOS);
    fprintf(stdout, NVM_COMPILEDBY);
    fprintf(stdout, NVM_BITS);
    fprintf(stdout, "\n\n");

    // create application encoder class
    cTAppEncTop.create();

    // parse configuration
    try {
        if (!cTAppEncTop.parseCfg(argc, argv)) {
            cTAppEncTop.destroy();
#if ENVIRONMENT_VARIABLE_DEBUG_AND_TEST
            EnvVar::printEnvVar();
#endif
            return 1;
        }
    } catch (df::program_options_lite::ParseFailure &e) {
        std::cerr << "Error parsing option \"" << e.arg << "\" with argument \"" << e.val << "\"." << std::endl;
        return 1;
    }
    
#if PRINT_MACRO_VALUES
    printMacroSettings();
#endif

#if ENVIRONMENT_VARIABLE_DEBUG_AND_TEST
    EnvVar::printEnvVarInUse();
#endif

    // starting time
    Double dResult;
    clock_t lBefore = clock();

    // call encoding function
    cTAppEncTop.encode();

    // ending time
    dResult = (Double) (clock() - lBefore) / CLOCKS_PER_SEC;
    printf("\n Total Time: %12.3f sec.\n", dResult);

    // iagostorch begin 

    cout << endl;
    cout << "checkIntraTime:  " << checkIntraTime << endl;
    cout << "checkInterTime:  " << checkInterTime << endl;
    cout << "calcVarTime: " << varTime << endl;

    cout << "|predInterSearchTime:  " << predInterSearchTime << endl;
    cout << "||xMotionEstimationTime:  " << xMotionEstimationTime << endl;
    cout << "|||xPatternSearchFastTime:  " << xPatternSearchFastTime << endl;
    cout << "||||xTZSearchTime:  " << xTZSearchTime << endl;
    cout << "|||||predTime: " << predTime << endl;
    cout << "|||||firstTime: " << firstTime << endl;
    cout << "|||||rasterTime: " << rasterTime << endl;
    cout << "|||||refinTime: " << refinTime << endl;
    cout << "|||xPatternSearchTime:  " << xPatternSearchTime << endl;
    cout << "|||FME:  " << fmeTime << endl;
    cout << "||||halfGenTime:  " << halfGenTime << endl;
    cout << "||||quartGenTime:  " << quartGenTime << endl;
    cout << "|calcRdInter: " << calcRdInter << endl;
    cout << "|checkBestModeInter: " << checkBestModeInter << endl;

    // iagostorch end 

    // destroy application encoder class
    cTAppEncTop.destroy();

    // iagostorch begin

    if (extractTZInfo) {
        mvFile.close();
    }
    if (extractFinalCuInfo) {
        finalCuInfo.close();
    }
    if (extractIntermediateCuInfo) {
        intermediateCuInfo.close();
    }
    if (extractRDInfo){
        RDs_64x64.close();
        RDs_32x32.close();
        RDs_16x16.close();
        RDs_8x8.close();
        RDs_4x4.close();
    }
    
    // iagostorch end

    return 0;
}

//! \}
