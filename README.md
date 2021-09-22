# HM-16.16_360_InterOpt
## A set of algorithms to accelerate the encoding of 360 videos in HEVC standard

This repository contains a set of algorithms to accelerate the intraframe and interframes prediction of 360 videos by exploring the properties of ERP projection. The algorithms are implemented over the [HEVC Test Model Version 16.16]  along [360Lib-5.0].

All implementations available in this repository were already published in journals and conferences, and the proper citations and access links are listed below:

- I. Storch, L. Agostini, B. Zatt, S. Bampi and D. Palomino, "[FastInter360: A Fast Inter Mode Decision for HEVC 360 Video Coding]," in IEEE Transactions on Circuits and Systems for Video Technology, doi: 10.1109/TCSVT.2021.3096752. 
- I. Storch, G. Correa, B. Zatt, L. Agostini and D. Palomino, "[ESA360 - Early SKIP Mode Decision Algorithm for Fast ERP 360 Video Coding]," 2020 28th European Signal Processing Conference (EUSIPCO), 2021, pp. 535-539, doi: 10.23919/Eusipco47968.2020.9287774.
- I. Storch, B. Zatt, L. Agostini, G. Correa, L. A. da Silva Cruz and D. Palomino, "[Spatially Adaptive Intra Mode Pre-Selection for ERP 360 Video Coding]," ICASSP 2020 - 2020 IEEE International Conference on Acoustics, Speech and Signal Processing (ICASSP), 2020, pp. 2178-2182, doi: 10.1109/ICASSP40776.2020.9053374.
- I. Storch, B. Zatt, L. Agostini, L. A. da Silva Cruz and D. Palomino, "[FastIntra360: A Fast Intra-Prediction Technique for 360-Degrees Video Coding]," 2019 Data Compression Conference (DCC), 2019, pp. 605-605, doi: 10.1109/DCC.2019.00117.


## Build Instructions
To build the encoder for a linux platform, use the following commands: 
```sh
cd HM-360Lib-5.0-build/linux/
make
```

## Branches and Algorithms
This repository contains multiple branches, each one with a different algorithm or combination of them. All algorithms explore the properties of ERP projection to adapt the encoder and reduce encoding time.

- master: This is the baseline encoder, without any modifications.
- DataExtraction: This encoder extracts some intermediate encoding information, such as the motion vectors for all stages of TZS and the final PUs sizes.
- EarlySkip: This encoder performs an early SKIP decision.
- IntraModes: This encoder reduces the number of intraframe prediction modes evaluated.
- IntraPuSizes: This encoder reduces the number of block sizes tested during intraframe prediction.
- PruningCTU: This encoder limits the minimum block size (exploratory only, not used in final algorithm).
- ReducedFME: This encoder adjusts the precision of FME.
- ReducedSR: This encoder adjusts the search range width of Raster Search.
- HybridTechnique: This encoder merges all others, except for PruningCTU and DataExtraction.

## Usage
After building the encoder, the main encoding parameters are the same as of HM, and available in ```doc/software-manual.pdf ```. The custom encoding parameters used to control the designed algorithms are listed in ```cfg/encoder_iago_acceleration.cfg ``` and summarized as follows:

| Parameter | Description|
| ------ | ------ |
| EarlySkip | Enable/disable Earky SKIP algorithm|
| EarkySkipAdaptiveQP | Enable/disable adaptive thresholds based on QP|
| EarlySkipBandsDistribution | Defines the borders between polar, mid-polar and middle bands for Early SKIP|
| EarlySkipIntegral | Defines intensity of complexity reduction in polar and mid-polar bands |
| ReducedFME | Enable/disable Reduced FME algorithm |
| ReducedFMEBandsDistribution | Defines the borders between polar, mid-polar and middle bands for Reduced FME|
| ReducedFMEBandsVerticalPrecision | Defines vertical precision of FME in polar and mid-polar bands |
| ReducedFMEBandsHorizontalPrecision | Defines horizontal precision of FME in polar and mid-polar bands |
| ReducedSR | Enable/disable Reduced SR algorithm |
| ReducedSRBandsDistribution | Defines the borders between polar, mid-polar and middle bands for Reduced SR |
| ReducedSRBandsScaleVerticalSR | Defines vertical scale of search range in polar and mid-polar bands |
| ReducedSRBandsScaleHorizontalSR | Defines horizontal scale of search range in polar and mid-polar bands |
| RdPUSizeReduction| Enable/disable Reduced Intra PU Sizes algorithm|
| RdPUSizeReductionRefreshRate| Defines the distance between two frames encoded without Reduced Intra PU Sizes algorithm|
| RdPUSizeReductionThreshold| Defines the threshold used when comparing co-located rd-costs to skip smaller block sizes|
| RdPUSizeReductionMinContribution| Defines the minimum occurrence rate of each PU size that allows skipping smaller blocks|
|ReducedIntraModes| Enable/disable Reduced Intra Modes algorithm|
|ReducedIntraModesBandsDistribution| Defines the borders between polar, mid-polar and middle bands for Reduced Intra Modes|


[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)

   [HEVC Test Model Version 16.16]: <https://hevc.hhi.fraunhofer.de/svn/svn_HEVCSoftware/tags/HM-16.16/>
   [360Lib-5.0]: <https://jvet.hhi.fraunhofer.de/svn/svn_360Lib/tags/360Lib-5.0/>
   [FastInter360: A Fast Inter Mode Decision for HEVC 360 Video Coding]: <https://ieeexplore.ieee.org/document/9481218>
   [FastIntra360: A Fast Intra-Prediction Technique for 360-Degrees Video Coding]: <https://ieeexplore.ieee.org/document/8712776>
   [ESA360 - Early SKIP Mode Decision Algorithm for Fast ERP 360 Video Coding]: <https://ieeexplore.ieee.org/document/9287774>
   [Spatially Adaptive Intra Mode Pre-Selection for ERP 360 Video Coding]: <https://ieeexplore.ieee.org/document/9053374>
   
   
