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
ofstream mvFile;
ofstream finalCuInfo;
ofstream intermediateCuInfo;
Int extractOnlyRasterPUs = 1;
Int extractTZInfo = 1;
Int extractFinalCuInfo = 1;
Int extractIntermediateCuInfo = 1;
Int** samplesMatrix; // Used to store the samples value for future variancecalculation

int iagoEarlySkip; // Custom encoding parameter. Controls early skip based on block variance
double *iagoEarlySkipIntegral; // Custom parameter. Controls variance threshold for early skip in each band
double *iagoBandsDistribution; // Custom encoding parameter. Controle the size of each band
int iagoNdivisions;

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
// iagostorch end

//! \ingroup TAppEncoder
//! \{

#include "../Lib/TLibCommon/Debug.h"

// ====================================================================================================================
// Main function
// ====================================================================================================================

int main(int argc, char* argv[])
{
  // iagostorch begin
  if(extractTZInfo){
    mvFile.open("mvFile.csv");
    mvFile << "PU,Pos,Size,Pred,Inic,Rast,Refi" << endl;
  }
  if(extractFinalCuInfo){
    finalCuInfo.open("finalPU.csv");
    finalCuInfo << "Frame,CTU#,Pos,Depth,Type,Idx,Merge,Skip,Ref0,MV0,Ref1,MV1" << endl;
  }
  if(extractIntermediateCuInfo){
    intermediateCuInfo.open("intermediatePU.csv");
    intermediateCuInfo << "Frame,CTU#,Pos,Depth,Type,Idx,Merge,Skip,Ref0,MV0,Ref1,MV1" << endl;
  }
  // iagostorch end
  TAppEncTop  cTAppEncTop;

  // print information
  fprintf( stdout, "\n" );
  fprintf( stdout, "HM software: Encoder Version [%s] (including RExt)", NV_VERSION );
  fprintf( stdout, NVM_ONOS );
  fprintf( stdout, NVM_COMPILEDBY );
  fprintf( stdout, NVM_BITS );
  fprintf( stdout, "\n\n" );

  // create application encoder class
  cTAppEncTop.create();

  // parse configuration
  try
  {
    if(!cTAppEncTop.parseCfg( argc, argv ))
    {
      cTAppEncTop.destroy();
#if ENVIRONMENT_VARIABLE_DEBUG_AND_TEST
      EnvVar::printEnvVar();
#endif
      return 1;
    }
  }
  catch (df::program_options_lite::ParseFailure &e)
  {
    std::cerr << "Error parsing option \""<< e.arg <<"\" with argument \""<< e.val <<"\"." << std::endl;
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
  dResult = (Double)(clock()-lBefore) / CLOCKS_PER_SEC;
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
  cout << "|calcRdInter: " << calcRdInter << endl; 
  cout << "|checkBestModeInter: " << checkBestModeInter << endl; 
   
// iagostorch end 
 
  // destroy application encoder class
  cTAppEncTop.destroy();

  // iagostorch begin
  
  if(extractTZInfo){
    mvFile.close();
  }
  if(extractFinalCuInfo){
    finalCuInfo.close();
  }
  if(extractIntermediateCuInfo){
    intermediateCuInfo.close();
  }
  
  // iagostorch end
  
  return 0;
}

//! \}
