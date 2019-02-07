# the SOURCE definiton lets move your makefile to another position
CONFIG 			= LIBRARY

# set pathes to the correct directories
SRC_DIR			= ../../../../source/Lib/TLib360
INC_DIR			= ../../../../source/Lib
LIB_DIR			= ../../../../lib
BIN_DIR			= ../../../../bin

SRC_DIR1		= 
SRC_DIR2		=
SRC_DIR3		=
SRC_DIR4		=


USER_INC_DIRS	= -I$(SRC_DIR)
USER_LIB_DIRS	=

ifeq ($(HIGHBITDEPTH), 1)
HBD=HighBitDepth
else
HBD=
endif

# intermediate directory for object files
OBJ_DIR			= ./objects$(HBD)

# the library name
PRJ_NAME	 	= TLib360$(HBD)

# version information
MAJOR_VER		= 0
MINOR_VER		= 1
VER				= $(MAJOR_VER).$(MINOR_VER)

# defines to set
DEFS			= -DMSYS_LINUX

# set objects
OBJS          	= \
			$(OBJ_DIR)/TGeometry.o \
			$(OBJ_DIR)/TCubeMap.o \
			$(OBJ_DIR)/TEqualArea.o \
			$(OBJ_DIR)/TEquiRect.o \
			$(OBJ_DIR)/TIcosahedron.o \
			$(OBJ_DIR)/TOctahedron.o \
			$(OBJ_DIR)/TPSNRMetricCalc.o \
			$(OBJ_DIR)/TSPSNRMetricCalc.o \
			$(OBJ_DIR)/TViewPort.o \
			$(OBJ_DIR)/TViewPortPSNR.o \
			$(OBJ_DIR)/TWSPSNRMetricCalc.o \
			$(OBJ_DIR)/TSPSNRIMetricCalc.o \
			$(OBJ_DIR)/TCrastersParabolic.o \
			$(OBJ_DIR)/TCPPPSNRMetricCalc.o \
			$(OBJ_DIR)/TTsp.o \
			$(OBJ_DIR)/TSegmentedSphere.o \
			$(OBJ_DIR)/TAdjustedCubeMap.o \
			$(OBJ_DIR)/TRotatedSphere.o \
			$(OBJ_DIR)/TEquatorialCylindrical.o \
            $(OBJ_DIR)/TAdjustedEqualArea.o \
            $(OBJ_DIR)/TEquiAngularCubeMap.o \
       
LIBS				= -lpthread

DEBUG_LIBS			=
RELEASE_LIBS		=

STAT_LIBS			=
DYN_LIBS			= -ldl

# the libraries to link with
STAT_DEBUG_LIBS		=
STAT_RELEASE_LIBS	=
DYN_DEBUG_LIBS		=
DYN_RELEASE_LIBS	=

# name of the base makefile
MAKE_FILE_NAME		= ../../common/makefile.base

# include the base makefile
include $(MAKE_FILE_NAME)
