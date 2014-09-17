# Copyright 1994-2006 The MathWorks, Inc.
#
# File    : ert_unix.tmf   $Revision: 1.55.4.34 $
#
# Abstract:
#	Real-Time Workshop template makefile for building a UNIX-based
#	stand-alone embedded real-time version of Simulink model using
#	generated C code.
#
# 	This makefile attempts to conform to the guidelines specified in the
# 	IEEE Std 1003.2-1992 (POSIX) standard. It is designed to be used
#       with GNU Make which is located in matlabroot/rtw/bin.
#
# 	Note that this template is automatically customized by the Real-Time
#	Workshop build procedure to create "<model>.mk"
#
#       The following defines can be used to modify the behavior of the
#	build:
#	  OPT_OPTS       - Optimization options. Default is -O. To enable 
#                          debugging specify as OPT_OPTS=-g. 
#                          Because of optimization problems in IBM_RS, 
#                          default is no-optimization. 
#         CPP_OPTS       - C++ compiler options.	
#	  OPTS           - User specific compile options.
#	  USER_SRCS      - Additional user sources, such as files needed by
#			   S-functions.
#	  USER_INCLUDES  - Additional include paths
#			   (i.e. USER_INCLUDES="-Iwhere-ever -Iwhere-ever2")
#
#       This template makefile is designed to be used with a system target
#       file that contains 'rtwgensettings.BuildDirSuffix' see ert.tlc


#------------------------ Macros read by make_rtw ------------------------------
#
# The following macros are read by the Real-Time Workshop build procedure:
#
#  MAKECMD         - This is the command used to invoke the make utility
#  HOST            - What platform this template makefile is targeted for
#                    (i.e. PC or UNIX)
#  BUILD           - Invoke make from the Real-Time Workshop build procedure
#                    (yes/no)?
#  SYS_TARGET_FILE - Name of system target file.

MAKECMD         = make
HOST            = UNIX
BUILD           = yes
SYS_TARGET_FILE = any

#---------------------- Tokens expanded by make_rtw ----------------------------
#
# The following tokens, when wrapped with "|>" and "<|" are expanded by the
# Real-Time Workshop build procedure.
#
#  MODEL_NAME          - Name of the Simulink block diagram
#  MODEL_MODULES       - Any additional generated source modules
#  MAKEFILE_NAME       - Name of makefile created from template makefile <model>.mk
#  MATLAB_ROOT         - Path to were MATLAB is installed.
#  S_FUNCTIONS         - List of additional S-function modules.
#  S_FUNCTIONS_LIB     - List of S-functions libraries to link. 
#  NUMST               - Number of sample times
#  NCSTATES            - Number of continuous states
#  COMPUTER            - Computer type. See the MATLAB computer command.
#  BUILDARGS           - Options passed in at the command line.
#  MULTITASKING        - yes (1) or no (0): Is solver mode multitasking
#  INTEGER_CODE        - yes (1) or no (0): Is generated code purely integer
#  MAT_FILE            - yes (1) or no (0): Should mat file logging be done,
#                        if 0, the generated code runs indefinitely
#  EXT_MODE            - yes (1) or no (0): Build for external mode
#  TMW_EXTMODE_TESTING - yes (1) or no (0): Build ext_test.c for external mode
#                        testing.
#  EXTMODE_TRANSPORT   - Index of transport mechanism (e.g. tcpip, serial) for extmode
#  MULTI_INSTANCE_CODE - Is the generated code multi instantiable (1/0)?
#  ADD_MDL_NAME_TO_GLOBALS - Do we add the model name to global 
#                        identifiers (1/0)?
#  GEN_MDLREF_SFCN     - (1/0): are we generating model reference wrapper s-function
#  MODELREFS           - List of referenced models
#  EXTMODE_STATIC      - yes (1) or no (0): Use static instead of dynamic mem alloc.
#  EXTMODE_STATIC_SIZE - Size of static memory allocation buffer.
#  TGT_FCN_LIB         - Target Funtion library to use

MODEL                   = PHAT
MODULES                 = PHAT_data.c 
MAKEFILE                = PHAT.mk
MATLAB_ROOT             = /Applications/MATLAB73
ALT_MATLAB_ROOT         = /Applications/MATLAB73
S_FUNCTIONS             = 
S_FUNCTIONS_LIB         = 
NUMST                   = 1
NCSTATES                = 0
COMPUTER                = MACI
BUILDARGS               =  GENERATE_REPORT=1 GENERATE_ASAP2=0
MULTITASKING            = 0
INTEGER_CODE            = 0
MAT_FILE                = 0
ONESTEPFCN              = 1
TERMFCN                 = 1
B_ERTSFCN               = 0
MEXEXT                  = mexmaci
EXT_MODE                = 0
TMW_EXTMODE_TESTING     = 0
EXTMODE_TRANSPORT       = 0
EXTMODE_STATIC          = 0
EXTMODE_STATIC_SIZE     = 1000000
MULTI_INSTANCE_CODE     = 0
ADD_MDL_NAME_TO_GLOBALS = 1
TGT_FCN_LIB             = ANSI_C
MODELREFS               = 
SHARED_SRC              = 
SHARED_SRC_DIR          = 
SHARED_BIN_DIR          = 
SHARED_LIB              = 
GEN_SAMPLE_MAIN         = 1
TARGET_LANG_EXT         = c


#--------------------------- Model and reference models -----------------------
MODELLIB                  = PHATlib.a
MODELREF_LINK_LIBS        = 
MODELREF_INC_PATH         = 
RELATIVE_PATH_TO_ANCHOR   = ..
# NONE: standalone, SIM: modelref sim, RTW: modelref rtw
MODELREF_TARGET_TYPE       = NONE

#-- In the case when directory name contains space ---
ifneq ($(MATLAB_ROOT),$(ALT_MATLAB_ROOT))
MATLAB_ROOT := $(ALT_MATLAB_ROOT)
endif

#----------------------------- External mode -----------------------------------
# Uncomment -DVERBOSE to have information printed to stdout
# To add a new transport layer, see the comments in
#   <matlabroot>/toolbox/simulink/simulink/extmode_transports.m
ifeq ($(EXT_MODE),1)
  EXT_CC_OPTS = -DEXT_MODE -D$(COMPUTER) #-DVERBOSE
  EXT_LIB     =
  EXT_SRC     =
  LINT_EXT_COMMON_SRC =
  LINT_EXT_TCPIP_SRC  =
  ifeq ($(EXTMODE_TRANSPORT),0) #tcpip
    EXT_SRC = ext_svr.c updown.c ext_work.c ext_svr_tcpip_transport.c
    LINT_EXT_COMMON_SRC = ext_svr.c updown.c ext_work.c
    LINT_EXT_TCPIP_SRC  = ext_svr_tcpip_transport.c
  endif
  ifeq ($(EXTMODE_TRANSPORT),1) #serial_win32
    err:
	@echo
	@echo "### ERROR: External mode serial transport only available on win32"
	@echo
  endif
  ifeq ($(TMW_EXTMODE_TESTING),1)
    EXT_SRC     += ext_test.c
    EXT_CC_OPTS += -DTMW_EXTMODE_TESTING
  endif
  ifeq ($(EXTMODE_STATIC),1)
    EXT_SRC             += mem_mgr.c
    LINT_EXT_COMMON_SRC += mem_mgr.c
    EXT_CC_OPTS         += -DEXTMODE_STATIC -DEXTMODE_STATIC_SIZE=$(EXTMODE_STATIC_SIZE)
  endif
  ifeq ($(COMPUTER),SOL2)
    EXT_LIB = -lsocket -lnsl
  endif
endif

#--------------------------- Tool Specifications -------------------------------

include $(MATLAB_ROOT)/rtw/c/tools/unixtools.mk

# Determine if we are generating an s-function
SFCN = 0
ifeq ($(MODELREF_TARGET_TYPE),SIM)
  SFCN = 1
endif
ifeq ($(B_ERTSFCN),1)
  SFCN = 1
endif

# Use GCC_TEST to do a test compile of the local source (add DO_GCC_TEST=1)
ifeq ($(DO_GCC_TEST), 1)
  GCC_TEST     = echo "Doing gcc test compile"; gcc -c -o /dev/null -Wall 
  GCC_TEST_OUT = 
else
  GCC_TEST     = echo
  GCC_TEST_OUT = > /dev/null
endif

#------------------------------ Include Path -----------------------------------

MATLAB_INCLUDES = \
	-I$(MATLAB_ROOT)/rtw/c/ert \
	-I$(MATLAB_ROOT)/extern/include \
	-I$(MATLAB_ROOT)/simulink/include \
	-I$(MATLAB_ROOT)/rtw/c/src \
	-I$(MATLAB_ROOT)/rtw/c/src/ext_mode/common

# Additional includes 
ADD_INCLUDES = \
	-I/Users/cameraman/Desktop/cameraman/mac/MaxAudioLocalizer/MaxMSP_UB_SDK/evans-externs/controller/PHAT_ert_rtw \
	-I/Users/cameraman/Desktop/cameraman/mac/MaxAudioLocalizer/MaxMSP_UB_SDK/evans-externs/controller \
	-I$(MATLAB_ROOT)/rtw/c/libsrc \
	-I$(MATLAB_ROOT)/toolbox/dspblks/include \


SHARED_INCLUDES =
ifneq ($(SHARED_SRC_DIR),)
SHARED_INCLUDES = -I$(SHARED_SRC_DIR) 
endif

INCLUDES = -I. -I$(RELATIVE_PATH_TO_ANCHOR) $(MATLAB_INCLUDES) $(ADD_INCLUDES) $(USER_INCLUDES) \
	$(INSTRUMENT_INCLUDES) $(MODELREF_INC_PATH) $(SHARED_INCLUDES)


#-------------------------------- C Flags --------------------------------------

# Optimization Options
ifndef OPT_OPTS
OPT_OPTS = $(DEFAULT_OPT_OPTS)
endif

# General User Options
OPTS =

# Compiler options, etc: 
CPP_REQ_DEFINES1 = -DMODEL=$(MODEL) -DNUMST=$(NUMST) -DNCSTATES=$(NCSTATES) \
		  -DUNIX -DMAT_FILE=$(MAT_FILE) -DINTEGER_CODE=$(INTEGER_CODE) \
		  -DONESTEPFCN=$(ONESTEPFCN) -DTERMFCN=$(TERMFCN) \
		  -DHAVESTDIO -DMULTI_INSTANCE_CODE=$(MULTI_INSTANCE_CODE)\
		  -DADD_MDL_NAME_TO_GLOBALS=$(ADD_MDL_NAME_TO_GLOBALS)

CPP_REQ_DEFINES = $(CPP_REQ_DEFINES1)
ifeq ($(MODELREF_TARGET_TYPE),SIM)
CPP_REQ_DEFINES += -DMDL_REF_SIM_TGT=1
else
CPP_REQ_DEFINES += -DMT=$(MULTITASKING)
endif

CC_OPTS = $(OPT_OPTS) $(OPTS) $(EXT_CC_OPTS)
CFLAGS = $(CC_OPTS) $(ANSI_OPTS) $(CPP_REQ_DEFINES) $(INCLUDES)
CPPFLAGS = $(CPP_OPTS) $(CC_OPTS) $(CPP_ANSI_OPTS) $(CPP_REQ_DEFINES) $(INCLUDES)
ifeq ($(SFCN),1)
CC_OPTS_SFCN = COPTIMFLAGS="$(OPT_OPTS) $(ANSI_OPTS)"
CFLAGS_SFCN = $(CC_OPTS_SFCN) $(CPP_REQ_DEFINES1) $(INCLUDES)
endif

#-------------------------- Additional Libraries ------------------------------

SYSLIBS = $(EXT_LIB)
ifeq ($(SFCN),0)
SYSLIBS += -lm
endif

LIBS =

ifeq ($(OPT_OPTS),$(DEFAULT_OPT_OPTS))
ifeq ($(INTEGER_CODE),0)
ifeq ($(SFCN),1)
LIBS += $(MATLAB_ROOT)/toolbox/dspblks/lib/maci/dsp_rt_rtwsfcn.a
else
LIBS += $(MATLAB_ROOT)/toolbox/dspblks/lib/maci/dsp_rt_ert.a
endif
else
LIBS += $(MATLAB_ROOT)/toolbox/dspblks/lib/maci/dsp_rt_int_ert.a
endif
else
LIBS += dsp_rt.a
endif

ifeq ($(OPT_OPTS),$(DEFAULT_OPT_OPTS))
ifeq ($(INTEGER_CODE),0)
ifeq ($(SFCN),1)
LIBS += $(MATLAB_ROOT)/rtw/c/lib/maci/rtwlib_rtwsfcn.a
else
LIBS += $(MATLAB_ROOT)/rtw/c/lib/maci/rtwlib_ert.a
endif
else
LIBS += $(MATLAB_ROOT)/rtw/c/lib/maci/rtwlib_int_ert.a
endif
else
LIBS += rtwlib.a
endif
 
LIBS += $(S_FUNCTIONS_LIB) $(INSTRUMENT_LIBS)

ifeq ($(SFCN),1)	
LIBFIXPT = -L$(MATLAB_ROOT)/bin/$(ARCH) -lfixedpoint
else
LIBFIXPT = 
endif

#----------------------------- Source Files ------------------------------------
ADD_SRCS =
ifeq ($(MAT_FILE), 1)
ADD_SRCS += rtwlog.c
endif

ifeq ($(SFCN),0)
  SRCS  = $(MODULES) $(ADD_SRCS) $(S_FUNCTIONS)
  SRC_DEP =
  ifeq ($(MODELREF_TARGET_TYPE), NONE)
    # ERT standalone
    PRODUCT            = $(RELATIVE_PATH_TO_ANCHOR)/$(MODEL)
    BIN_SETTING        = $(LD) $(LDFLAGS) -o $(PRODUCT) $(SYSLIBS) 
    BUILD_PRODUCT_TYPE = "executable"
    SRCS               += $(MODEL).$(TARGET_LANG_EXT) $(EXT_SRC)
    ifeq ($(GEN_SAMPLE_MAIN),0)
      SRCS += ert_main.c
    else
      SRCS += ert_main.$(TARGET_LANG_EXT)
    endif
  else
    # Model reference rtw target
    PRODUCT            = $(MODELLIB)
    BUILD_PRODUCT_TYPE = "library"
  endif
else
  # Model Reference Simulation target, ERT S-function target
  MEX                 = $(MATLAB_ROOT)/bin/mex
  ifeq ($(MODELREF_TARGET_TYPE), SIM)
  PRODUCT            = $(RELATIVE_PATH_TO_ANCHOR)/$(MODEL)_msf.$(MEXEXT)
  RTW_SFUN_SRC       = $(MODEL)_msf.$(TARGET_LANG_EXT)
  SRCS               = $(MODULES) $(ADD_SRCS)
  else
  PRODUCT            = $(RELATIVE_PATH_TO_ANCHOR)/$(MODEL)_sf.$(MEXEXT)
  RTW_SFUN_SRC       = $(MODEL)_sf.$(TARGET_LANG_EXT)
  SRCS               = $(MODULES) $(ADD_SRCS) $(S_FUNCTIONS)
  endif
  BIN_SETTING        = $(MEX) -MATLAB_ARCH=$(ARCH) $(CFLAGS_SFCN) $(RTW_SFUN_SRC)  -outdir $(RELATIVE_PATH_TO_ANCHOR) -silent
  BUILD_PRODUCT_TYPE = "mex file"
  ifeq ($(B_ERTSFCN),1)
    SRCS              += $(MODEL).$(TARGET_LANG_EXT)
  endif
  SRC_DEP            = $(RTW_SFUN_SRC)
endif

USER_SRCS =

USER_OBJS       = $(addsuffix .o, $(basename $(USER_SRCS)))
LOCAL_USER_OBJS = $(notdir $(USER_OBJS))

OBJS      = $(addsuffix .o, $(basename $(SRCS))) $(USER_OBJS)
LINK_OBJS = $(addsuffix .o, $(basename $(SRCS))) $(LOCAL_USER_OBJS)

SHARED_SRC := $(wildcard $(SHARED_SRC))
SHARED_OBJS = $(addsuffix .o, $(basename $(SHARED_SRC)))

ifneq ($(findstring .cpp,$(suffix $(SRCS), $(USER_SRCS))),)
  LD = $(CPP)
  SYSLIBS += $(CPP_SYS_LIBS)
endif
#----------------------------- Lint (sol2 only) --------------------------------

LINT_SRCS = $(MATLAB_ROOT)/rtw/c/ert/ert_main.c \
	    $(MATLAB_ROOT)/rtw/c/src/rtwlog.c
LINT_SRCS += $(MODEL).$(TARGET_LANG_EXT) $(MODULES) $(USER_SRCS) $(S_FUNCTIONS)
LINTOPTSFILE = $(MODEL).lintopts

ifneq ($(LINT_EXT_COMMON_SRC), )
  LINT_SRCS += $(MATLAB_ROOT)/rtw/c/src/ext_mode/common/$(LINT_EXT_COMMON_SRC)
endif
ifneq ($(LINT_EXT_TCPIP_SRC), )
  LINT_SRCS += $(MATLAB_ROOT)/rtw/c/src/ext_mode/tcpip/$(LINT_EXT_TCPIP_SRC)
endif

LINT_ERROFF1 = E_NAME_DEF_NOT_USED2,E_NAME_DECL_NOT_USED_DEF2
LINT_ERROFF2 = $(LINT_ERROFF1),E_FUNC_ARG_UNUSED
LINT_ERROFF  = $(LINT_ERROFF2),E_INDISTING_FROM_TRUNC2,E_NAME_USED_NOT_DEF2

#--------------------------------- Rules ---------------------------------------
ifeq ($(MODELREF_TARGET_TYPE),NONE)
$(PRODUCT) : $(OBJS) $(SHARED_LIB) $(LIBS) $(SRC_DEP) $(MODELREF_LINK_LIBS)
	$(BIN_SETTING) $(LINK_OBJS) $(MODELREF_LINK_LIBS) $(SHARED_LIB) $(LIBFIXPT) $(LIBS) 
	@echo "### Created $(BUILD_PRODUCT_TYPE): $@"	
else
 ifeq ($(MODELREF_TARGET_TYPE),SIM)
  $(PRODUCT) : $(OBJS) $(SHARED_LIB) $(LIBS) $(SRC_DEP)
	@rm -f $(MODELLIB)
	ar ruvs $(MODELLIB) $(LINK_OBJS)
	@echo "### Created $(MODELLIB)"
	$(BIN_SETTING) $(MODELLIB) $(MODELREF_LINK_LIBS) $(SHARED_LIB) $(LIBFIXPT) $(LIBS) 
	@echo "### Created $(BUILD_PRODUCT_TYPE): $@"	
 else
  $(PRODUCT) : $(OBJS) $(SHARED_LIB) $(SRC_DEP)
	@rm -f $(MODELLIB)
	ar ruvs $(MODELLIB) $(LINK_OBJS)
	@echo "### Created $(MODELLIB)"
	@echo "### Created $(BUILD_PRODUCT_TYPE): $@"	
 endif
endif


#--------------- Support for building referenced models -----------------------


#-------------------------- Support for building modules ----------------------

ifneq ($(SHARED_SRC_DIR),)
$(SHARED_BIN_DIR)/%.o : $(SHARED_SRC_DIR)/%.c
	cd $(SHARED_BIN_DIR); $(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $(notdir $?)

$(SHARED_BIN_DIR)/%.o : $(SHARED_SRC_DIR)/%.cpp
	cd $(SHARED_BIN_DIR); $(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $(notdir $?)
endif

%.o : %.c
	@$(GCC_TEST) $(CPP_REQ_DEFINES) $(INCLUDES) $< $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG) $<

%.o : %.cpp
	@$(GCC_TEST) $(CPP_REQ_DEFINES) $(INCLUDES) $< $(GCC_TEST_OUT)
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG) $<

%.o : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	@$(GCC_TEST) $(CPP_REQ_DEFINES) $(INCLUDES) $< $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG) $<

%.o : $(RELATIVE_PATH_TO_ANCHOR)/%.cpp
	@$(GCC_TEST) $(CPP_REQ_DEFINES) $(INCLUDES) $< $(GCC_TEST_OUT)
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG) $<

ifeq ($(GEN_SAMPLE_MAIN),0)
%.o : $(MATLAB_ROOT)/rtw/c/ert/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<
endif

%.o : $(MATLAB_ROOT)/rtw/c/src/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/ext_mode/common/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/ext_mode/tcpip/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/ext_mode/custom/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/rtw/c/src/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/dspblks/src/v3/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspacf/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspallpole/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspavi/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspbiquad/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspblms/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspburg/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspchol/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspconvcorr/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspdct/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspendian/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspeph/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspfbsub/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspfft/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspfilterbank/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspfir/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspg711/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspgivensrot/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsphist/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspic/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspiir/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspinterp/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspisfinite/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspldl/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsplms/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsplpc2cc/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsplsp2poly/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsplu/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspmmutils/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsppinv/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsppoly2lsf/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsppolyval/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspqrdc/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspqsrt/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsprandsrc/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsprc2ac/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsprc2lpc/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsprebuff/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspsvd/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspunwrap/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspupfir/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspvfdly/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/include/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/modver/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/dspblks/src/v4/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/rtw/c/libsrc/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<



%.o : $(MATLAB_ROOT)/rtw/c/src/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/dspblks/src/v3/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspacf/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspallpole/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspavi/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspbiquad/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspblms/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspburg/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspchol/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspconvcorr/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspdct/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspendian/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspeph/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspfbsub/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspfft/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspfilterbank/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspfir/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspg711/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspgivensrot/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsphist/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspic/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspiir/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspinterp/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspisfinite/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspldl/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsplms/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsplpc2cc/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsplsp2poly/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsplu/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspmmutils/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsppinv/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsppoly2lsf/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsppolyval/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspqrdc/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspqsrt/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsprandsrc/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsprc2ac/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsprc2lpc/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dsprebuff/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspsvd/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspunwrap/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspupfir/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/dspvfdly/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/include/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/rtw/dspblks/c/modver/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/toolbox/dspblks/src/v4/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/rtw/c/libsrc/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<



%.o : $(MATLAB_ROOT)/simulink/src/%.cpp
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $<

%.o : $(MATLAB_ROOT)/simulink/src/%.c
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $<

#------------------------------- Libraries -------------------------------------



MODULES_dsp_rt = \
    acf_fd_c_rt.o \
    acf_fd_d_rt.o \
    acf_fd_r_rt.o \
    acf_fd_z_rt.o \
    acf_fft_interleave_zpad_d_rt.o \
    acf_fft_interleave_zpad_r_rt.o \
    acf_td_c_rt.o \
    acf_td_d_rt.o \
    acf_td_r_rt.o \
    acf_td_z_rt.o \
    copy_and_zero_pad_cc_nchan_rt.o \
    copy_and_zero_pad_zz_nchan_rt.o \
    allpole_df_a0scale_cc_rt.o \
    allpole_df_a0scale_cr_rt.o \
    allpole_df_a0scale_dd_rt.o \
    allpole_df_a0scale_dz_rt.o \
    allpole_df_a0scale_rc_rt.o \
    allpole_df_a0scale_rr_rt.o \
    allpole_df_a0scale_zd_rt.o \
    allpole_df_a0scale_zz_rt.o \
    allpole_df_cc_rt.o \
    allpole_df_cr_rt.o \
    allpole_df_dd_rt.o \
    allpole_df_dz_rt.o \
    allpole_df_rc_rt.o \
    allpole_df_rr_rt.o \
    allpole_df_zd_rt.o \
    allpole_df_zz_rt.o \
    allpole_lat_cc_rt.o \
    allpole_lat_cr_rt.o \
    allpole_lat_dd_rt.o \
    allpole_lat_dz_rt.o \
    allpole_lat_rc_rt.o \
    allpole_lat_rr_rt.o \
    allpole_lat_zd_rt.o \
    allpole_lat_zz_rt.o \
    allpole_tdf_a0scale_cc_rt.o \
    allpole_tdf_a0scale_cr_rt.o \
    allpole_tdf_a0scale_dd_rt.o \
    allpole_tdf_a0scale_dz_rt.o \
    allpole_tdf_a0scale_rc_rt.o \
    allpole_tdf_a0scale_rr_rt.o \
    allpole_tdf_a0scale_zd_rt.o \
    allpole_tdf_a0scale_zz_rt.o \
    allpole_tdf_cc_rt.o \
    allpole_tdf_cr_rt.o \
    allpole_tdf_dd_rt.o \
    allpole_tdf_dz_rt.o \
    allpole_tdf_rc_rt.o \
    allpole_tdf_rr_rt.o \
    allpole_tdf_zd_rt.o \
    allpole_tdf_zz_rt.o \
    avi_rt.o \
    bq5_df2t_1fpf_1sos_cc_rt.o \
    bq5_df2t_1fpf_1sos_cr_rt.o \
    bq5_df2t_1fpf_1sos_dd_rt.o \
    bq5_df2t_1fpf_1sos_dz_rt.o \
    bq5_df2t_1fpf_1sos_rc_rt.o \
    bq5_df2t_1fpf_1sos_rr_rt.o \
    bq5_df2t_1fpf_1sos_zd_rt.o \
    bq5_df2t_1fpf_1sos_zz_rt.o \
    bq5_df2t_1fpf_nsos_cc_rt.o \
    bq5_df2t_1fpf_nsos_cr_rt.o \
    bq5_df2t_1fpf_nsos_dd_rt.o \
    bq5_df2t_1fpf_nsos_dz_rt.o \
    bq5_df2t_1fpf_nsos_rc_rt.o \
    bq5_df2t_1fpf_nsos_rr_rt.o \
    bq5_df2t_1fpf_nsos_zd_rt.o \
    bq5_df2t_1fpf_nsos_zz_rt.o \
    blms_an_wn_cc_rt.o \
    blms_an_wn_dd_rt.o \
    blms_an_wn_rr_rt.o \
    blms_an_wn_zz_rt.o \
    blms_an_wy_cc_rt.o \
    blms_an_wy_dd_rt.o \
    blms_an_wy_rr_rt.o \
    blms_an_wy_zz_rt.o \
    blms_ay_wn_cc_rt.o \
    blms_ay_wn_dd_rt.o \
    blms_ay_wn_rr_rt.o \
    blms_ay_wn_zz_rt.o \
    blms_ay_wy_cc_rt.o \
    blms_ay_wy_dd_rt.o \
    blms_ay_wy_rr_rt.o \
    blms_ay_wy_zz_rt.o \
    burg_a_c_rt.o \
    burg_a_d_rt.o \
    burg_a_r_rt.o \
    burg_a_z_rt.o \
    burg_ak_c_rt.o \
    burg_ak_d_rt.o \
    burg_ak_r_rt.o \
    burg_ak_z_rt.o \
    burg_k_c_rt.o \
    burg_k_d_rt.o \
    burg_k_r_rt.o \
    burg_k_z_rt.o \
    chol_c_rt.o \
    chol_d_rt.o \
    chol_r_rt.o \
    chol_z_rt.o \
    conv_td_cc_rt.o \
    conv_td_dd_rt.o \
    conv_td_dz_rt.o \
    conv_td_rc_rt.o \
    conv_td_rr_rt.o \
    conv_td_zz_rt.o \
    copy_and_zpad_cc_rt.o \
    copy_and_zpad_dz_rt.o \
    copy_and_zpad_rc_rt.o \
    copy_and_zpad_zz_rt.o \
    corr_td_cc_rt.o \
    corr_td_cr_rt.o \
    corr_td_dd_rt.o \
    corr_td_dz_rt.o \
    corr_td_rc_rt.o \
    corr_td_rr_rt.o \
    corr_td_zd_rt.o \
    corr_td_zz_rt.o \
    br_c_ip_rt.o \
    br_c_op_rt.o \
    br_d_ip_rt.o \
    br_d_op_rt.o \
    br_r_ip_rt.o \
    br_r_op_rt.o \
    br_z_ip_rt.o \
    br_z_op_rt.o \
    copy_col_as_row_br_c_rt.o \
    copy_col_as_row_br_d_rt.o \
    copy_col_as_row_br_r_rt.o \
    copy_col_as_row_br_z_rt.o \
    copy_col_as_row_d_rt.o \
    copy_col_as_row_r_rt.o \
    copy_row_as_col_br_d_rt.o \
    copy_row_as_col_br_r_rt.o \
    copy_row_as_col_d_rt.o \
    copy_row_as_col_r_rt.o \
    dct_br_d_4_rt.o \
    dct_br_r_4_rt.o \
    dct_br_rt.o \
    dct_c_4_rt.o \
    dct_c_8_rt.o \
    dct_c_rt.o \
    dct_d_4_rt.o \
    dct_d_8_rt.o \
    dct_d_rt.o \
    dct_dbr_8_rt.o \
    dct_r_4_rt.o \
    dct_r_8_rt.o \
    dct_r_rt.o \
    dct_rbr_8_rt.o \
    dct_tbl_c_4_rt.o \
    dct_tbl_c_8_rt.o \
    dct_tbl_c_rt.o \
    dct_tbl_d_4_rt.o \
    dct_tbl_d_8_rt.o \
    dct_tbl_d_rt.o \
    dct_tbl_dbr_8_rt.o \
    dct_tbl_r_4_rt.o \
    dct_tbl_r_8_rt.o \
    dct_tbl_r_rt.o \
    dct_tbl_z_4_rt.o \
    dct_tbl_z_8_rt.o \
    dct_tbl_z_rt.o \
    dct_z_4_rt.o \
    dct_z_8_rt.o \
    dct_z_rt.o \
    idct_c_4_rt.o \
    idct_c_8_rt.o \
    idct_c_rt.o \
    idct_d_4_rt.o \
    idct_d_8_rt.o \
    idct_d_rt.o \
    idct_r_4_rt.o \
    idct_r_8_rt.o \
    idct_r_rt.o \
    idct_tbl_c_4_rt.o \
    idct_tbl_c_8_rt.o \
    idct_tbl_c_rt.o \
    idct_tbl_d_4_rt.o \
    idct_tbl_d_8_rt.o \
    idct_tbl_d_rt.o \
    idct_tbl_r_4_rt.o \
    idct_tbl_r_8_rt.o \
    idct_tbl_r_rt.o \
    idct_tbl_z_4_rt.o \
    idct_tbl_z_8_rt.o \
    idct_tbl_z_rt.o \
    idct_z_4_rt.o \
    idct_z_8_rt.o \
    idct_z_rt.o \
    is_little_endian_rt.o \
    eph_zc_fcn_rt.o \
    bsub_nu_cc_c_rt.o \
    bsub_nu_cr_c_rt.o \
    bsub_nu_dd_d_rt.o \
    bsub_nu_dz_z_rt.o \
    bsub_nu_rc_c_rt.o \
    bsub_nu_rr_r_rt.o \
    bsub_nu_zd_z_rt.o \
    bsub_nu_zz_z_rt.o \
    bsub_rd_cc_c_rt.o \
    bsub_rd_cr_c_rt.o \
    bsub_rd_zd_z_rt.o \
    bsub_rd_zz_z_rt.o \
    bsub_u_cc_c_rt.o \
    bsub_u_cr_c_rt.o \
    bsub_u_dd_d_rt.o \
    bsub_u_dz_z_rt.o \
    bsub_u_rc_c_rt.o \
    bsub_u_rr_r_rt.o \
    bsub_u_zd_z_rt.o \
    bsub_u_zz_z_rt.o \
    fsub_nu_cc_c_rt.o \
    fsub_nu_cr_c_rt.o \
    fsub_nu_dd_d_rt.o \
    fsub_nu_dz_z_rt.o \
    fsub_nu_rc_c_rt.o \
    fsub_nu_rr_r_rt.o \
    fsub_nu_zd_z_rt.o \
    fsub_nu_zz_z_rt.o \
    fsub_rd_cc_c_rt.o \
    fsub_rd_cr_c_rt.o \
    fsub_rd_zd_z_rt.o \
    fsub_rd_zz_z_rt.o \
    fsub_u_cc_c_rt.o \
    fsub_u_cr_c_rt.o \
    fsub_u_dd_d_rt.o \
    fsub_u_dz_z_rt.o \
    fsub_u_rc_c_rt.o \
    fsub_u_rr_r_rt.o \
    fsub_u_zd_z_rt.o \
    fsub_u_zz_z_rt.o \
    copy_adjrow_intcol_br_c_rt.o \
    copy_adjrow_intcol_br_z_rt.o \
    copy_adjrow_intcol_c_rt.o \
    copy_adjrow_intcol_z_rt.o \
    copy_col_as_row_c_rt.o \
    copy_col_as_row_z_rt.o \
    copy_row_as_col_br_c_rt.o \
    copy_row_as_col_br_dz_rt.o \
    copy_row_as_col_br_rc_rt.o \
    copy_row_as_col_br_z_rt.o \
    copy_row_as_col_c_rt.o \
    copy_row_as_col_dz_rt.o \
    copy_row_as_col_rc_rt.o \
    copy_row_as_col_z_rt.o \
    fft_dbllen_tbl_c_rt.o \
    fft_dbllen_tbl_z_rt.o \
    fft_dbllen_trig_c_rt.o \
    fft_dbllen_trig_z_rt.o \
    fft_dblsig_br_c_rt.o \
    fft_dblsig_br_z_rt.o \
    fft_dblsig_c_rt.o \
    fft_dblsig_z_rt.o \
    fft_interleave_br_d_rt.o \
    fft_interleave_br_r_rt.o \
    fft_interleave_d_rt.o \
    fft_interleave_r_rt.o \
    fft_r2br_c_oop_rt.o \
    fft_r2br_c_rt.o \
    fft_r2br_dz_oop_rt.o \
    fft_r2br_rc_oop_rt.o \
    fft_r2br_z_oop_rt.o \
    fft_r2br_z_rt.o \
    fft_r2dif_tblm_c_rt.o \
    fft_r2dif_tblm_z_rt.o \
    fft_r2dif_tbls_c_rt.o \
    fft_r2dif_tbls_z_rt.o \
    fft_r2dif_trig_c_rt.o \
    fft_r2dif_trig_z_rt.o \
    fft_r2dit_tblm_c_rt.o \
    fft_r2dit_tblm_z_rt.o \
    fft_r2dit_tbls_c_rt.o \
    fft_r2dit_tbls_z_rt.o \
    fft_r2dit_trig_c_rt.o \
    fft_r2dit_trig_z_rt.o \
    fft_scaledata_dd_rt.o \
    fft_scaledata_dz_rt.o \
    fft_scaledata_rc_rt.o \
    fft_scaledata_rr_rt.o \
    ifft_addcssignals_c_c_oop_rt.o \
    ifft_addcssignals_c_cbr_oop_rt.o \
    ifft_addcssignals_d_z_oop_rt.o \
    ifft_addcssignals_d_zbr_oop_rt.o \
    ifft_addcssignals_r_c_oop_rt.o \
    ifft_addcssignals_r_cbr_oop_rt.o \
    ifft_addcssignals_z_z_oop_rt.o \
    ifft_addcssignals_z_zbr_oop_rt.o \
    ifft_dbllen_tbl_c_cbr_oop_rt.o \
    ifft_dbllen_tbl_cbr_cbr_oop_rt.o \
    ifft_dbllen_tbl_d_zbr_oop_rt.o \
    ifft_dbllen_tbl_dbr_zbr_oop_rt.o \
    ifft_dbllen_tbl_r_cbr_oop_rt.o \
    ifft_dbllen_tbl_rbr_cbr_oop_rt.o \
    ifft_dbllen_tbl_z_zbr_oop_rt.o \
    ifft_dbllen_tbl_zbr_zbr_oop_rt.o \
    ifft_dbllen_trig_c_cbr_oop_rt.o \
    ifft_dbllen_trig_cbr_cbr_oop_rt.o \
    ifft_dbllen_trig_d_zbr_oop_rt.o \
    ifft_dbllen_trig_dbr_zbr_oop_rt.o \
    ifft_dbllen_trig_r_cbr_oop_rt.o \
    ifft_dbllen_trig_rbr_cbr_oop_rt.o \
    ifft_dbllen_trig_z_zbr_oop_rt.o \
    ifft_dbllen_trig_zbr_zbr_oop_rt.o \
    ifft_deinterleave_d_d_inp_rt.o \
    ifft_deinterleave_r_r_inp_rt.o \
    2chabank_fr_df_cc_rt.o \
    2chabank_fr_df_cr_rt.o \
    2chabank_fr_df_dd_rt.o \
    2chabank_fr_df_rr_rt.o \
    2chabank_fr_df_zd_rt.o \
    2chabank_fr_df_zz_rt.o \
    2chsbank_df_cc_rt.o \
    2chsbank_df_cr_rt.o \
    2chsbank_df_dd_rt.o \
    2chsbank_df_rr_rt.o \
    2chsbank_df_zd_rt.o \
    2chsbank_df_zz_rt.o \
    fir_df_cc_rt.o \
    fir_df_cr_rt.o \
    fir_df_dd_rt.o \
    fir_df_dz_rt.o \
    fir_df_rc_rt.o \
    fir_df_rr_rt.o \
    fir_df_zd_rt.o \
    fir_df_zz_rt.o \
    fir_lat_cc_rt.o \
    fir_lat_cr_rt.o \
    fir_lat_dd_rt.o \
    fir_lat_dz_rt.o \
    fir_lat_rc_rt.o \
    fir_lat_rr_rt.o \
    fir_lat_zd_rt.o \
    fir_lat_zz_rt.o \
    fir_tdf_cc_rt.o \
    fir_tdf_cr_rt.o \
    fir_tdf_dd_rt.o \
    fir_tdf_dz_rt.o \
    fir_tdf_rc_rt.o \
    fir_tdf_rr_rt.o \
    fir_tdf_zd_rt.o \
    fir_tdf_zz_rt.o \
    g711_enc_a_sat_rt.o \
    g711_enc_a_wrap_rt.o \
    g711_enc_mu_sat_rt.o \
    g711_enc_mu_wrap_rt.o \
    rotg_d_rt.o \
    rotg_r_rt.o \
    hist_binsearch_s08_rt.o \
    hist_binsearch_s16_rt.o \
    hist_binsearch_s32_rt.o \
    hist_binsearch_u08_rt.o \
    hist_binsearch_u16_rt.o \
    hist_binsearch_u32_rt.o \
    hist_c_rt.o \
    hist_d_rt.o \
    hist_r_rt.o \
    hist_z_rt.o \
    ic_copy_channel_rt.o \
    ic_copy_matrix_rt.o \
    ic_copy_scalar_rt.o \
    ic_copy_vector_rt.o \
    ic_old_copy_fcns_rt.o \
    iir_df1_a0scale_cc_rt.o \
    iir_df1_a0scale_cr_rt.o \
    iir_df1_a0scale_dd_rt.o \
    iir_df1_a0scale_dz_rt.o \
    iir_df1_a0scale_rc_rt.o \
    iir_df1_a0scale_rr_rt.o \
    iir_df1_a0scale_zd_rt.o \
    iir_df1_a0scale_zz_rt.o \
    iir_df1_cc_rt.o \
    iir_df1_cr_rt.o \
    iir_df1_dd_rt.o \
    iir_df1_dz_rt.o \
    iir_df1_rc_rt.o \
    iir_df1_rr_rt.o \
    iir_df1_zd_rt.o \
    iir_df1_zz_rt.o \
    iir_df1t_a0scale_cc_rt.o \
    iir_df1t_a0scale_cr_rt.o \
    iir_df1t_a0scale_dd_rt.o \
    iir_df1t_a0scale_dz_rt.o \
    iir_df1t_a0scale_rc_rt.o \
    iir_df1t_a0scale_rr_rt.o \
    iir_df1t_a0scale_zd_rt.o \
    iir_df1t_a0scale_zz_rt.o \
    iir_df1t_cc_rt.o \
    iir_df1t_cr_rt.o \
    iir_df1t_dd_rt.o \
    iir_df1t_dz_rt.o \
    iir_df1t_rc_rt.o \
    iir_df1t_rr_rt.o \
    iir_df1t_zd_rt.o \
    iir_df1t_zz_rt.o \
    iir_df2_a0scale_cc_rt.o \
    iir_df2_a0scale_cr_rt.o \
    iir_df2_a0scale_dd_rt.o \
    iir_df2_a0scale_dz_rt.o \
    iir_df2_a0scale_rc_rt.o \
    iir_df2_a0scale_rr_rt.o \
    iir_df2_a0scale_zd_rt.o \
    iir_df2_a0scale_zz_rt.o \
    iir_df2_cc_rt.o \
    iir_df2_cr_rt.o \
    iir_df2_dd_rt.o \
    iir_df2_dz_rt.o \
    iir_df2_rc_rt.o \
    iir_df2_rr_rt.o \
    iir_df2_zd_rt.o \
    iir_df2_zz_rt.o \
    iir_df2t_a0scale_cc_rt.o \
    iir_df2t_a0scale_cr_rt.o \
    iir_df2t_a0scale_dd_rt.o \
    iir_df2t_a0scale_dz_rt.o \
    iir_df2t_a0scale_rc_rt.o \
    iir_df2t_a0scale_rr_rt.o \
    iir_df2t_a0scale_zd_rt.o \
    iir_df2t_a0scale_zz_rt.o \
    iir_df2t_cc_rt.o \
    iir_df2t_cr_rt.o \
    iir_df2t_dd_rt.o \
    iir_df2t_dz_rt.o \
    iir_df2t_rc_rt.o \
    iir_df2t_rr_rt.o \
    iir_df2t_zd_rt.o \
    iir_df2t_zz_rt.o \
    interp_fir_d_rt.o \
    interp_fir_r_rt.o \
    interp_lin_d_rt.o \
    interp_lin_r_rt.o \
    isfinite_d_rt.o \
    isfinite_r_rt.o \
    ldl_c_rt.o \
    ldl_d_rt.o \
    ldl_r_rt.o \
    ldl_z_rt.o \
    lms_an_wn_cc_rt.o \
    lms_an_wn_dd_rt.o \
    lms_an_wn_rr_rt.o \
    lms_an_wn_zz_rt.o \
    lms_an_wy_cc_rt.o \
    lms_an_wy_dd_rt.o \
    lms_an_wy_rr_rt.o \
    lms_an_wy_zz_rt.o \
    lms_ay_wn_cc_rt.o \
    lms_ay_wn_dd_rt.o \
    lms_ay_wn_rr_rt.o \
    lms_ay_wn_zz_rt.o \
    lms_ay_wy_cc_rt.o \
    lms_ay_wy_dd_rt.o \
    lms_ay_wy_rr_rt.o \
    lms_ay_wy_zz_rt.o \
    lmsn_an_wn_cc_rt.o \
    lmsn_an_wn_dd_rt.o \
    lmsn_an_wn_rr_rt.o \
    lmsn_an_wn_zz_rt.o \
    lmsn_an_wy_cc_rt.o \
    lmsn_an_wy_dd_rt.o \
    lmsn_an_wy_rr_rt.o \
    lmsn_an_wy_zz_rt.o \
    lmsn_ay_wn_cc_rt.o \
    lmsn_ay_wn_dd_rt.o \
    lmsn_ay_wn_rr_rt.o \
    lmsn_ay_wn_zz_rt.o \
    lmsn_ay_wy_cc_rt.o \
    lmsn_ay_wy_dd_rt.o \
    lmsn_ay_wy_rr_rt.o \
    lmsn_ay_wy_zz_rt.o \
    lmssd_an_wn_dd_rt.o \
    lmssd_an_wn_rr_rt.o \
    lmssd_an_wy_dd_rt.o \
    lmssd_an_wy_rr_rt.o \
    lmssd_ay_wn_dd_rt.o \
    lmssd_ay_wn_rr_rt.o \
    lmssd_ay_wy_dd_rt.o \
    lmssd_ay_wy_rr_rt.o \
    lmsse_an_wn_dd_rt.o \
    lmsse_an_wn_rr_rt.o \
    lmsse_an_wy_dd_rt.o \
    lmsse_an_wy_rr_rt.o \
    lmsse_ay_wn_dd_rt.o \
    lmsse_ay_wn_rr_rt.o \
    lmsse_ay_wy_dd_rt.o \
    lmsse_ay_wy_rr_rt.o \
    lmsss_an_wn_dd_rt.o \
    lmsss_an_wn_rr_rt.o \
    lmsss_an_wy_dd_rt.o \
    lmsss_an_wy_rr_rt.o \
    lmsss_ay_wn_dd_rt.o \
    lmsss_ay_wn_rr_rt.o \
    lmsss_ay_wy_dd_rt.o \
    lmsss_ay_wy_rr_rt.o \
    cc2lpc_d_rt.o \
    cc2lpc_r_rt.o \
    lpc2cc_d_rt.o \
    lpc2cc_r_rt.o \
    lsp2poly_evenord_d_rt.o \
    lsp2poly_evenord_r_rt.o \
    lsp2poly_oddord_d_rt.o \
    lsp2poly_oddord_r_rt.o \
    lu_c_rt.o \
    lu_d_rt.o \
    lu_r_rt.o \
    lu_z_rt.o \
    mmpcmaudio_rt.o \
    mmrgb24convert2gray_rt.o \
    mmrgb24convert_rt.o \
    pinv_c_rt.o \
    pinv_d_rt.o \
    pinv_r_rt.o \
    pinv_z_rt.o \
    poly2lsfn_d_rt.o \
    poly2lsfn_r_rt.o \
    poly2lsfr_d_rt.o \
    poly2lsfr_r_rt.o \
    poly2lsp_d_rt.o \
    poly2lsp_r_rt.o \
    polyval_cc_rt.o \
    polyval_cr_rt.o \
    polyval_dd_rt.o \
    polyval_dz_rt.o \
    polyval_rc_rt.o \
    polyval_rr_rt.o \
    polyval_zd_rt.o \
    polyval_zz_rt.o \
    qrcompqy_c_rt.o \
    qrcompqy_d_rt.o \
    qrcompqy_mixd_c_rt.o \
    qrcompqy_mixd_z_rt.o \
    qrcompqy_r_rt.o \
    qrcompqy_z_rt.o \
    qrdc_c_rt.o \
    qrdc_d_rt.o \
    qrdc_r_rt.o \
    qrdc_z_rt.o \
    qre_c_rt.o \
    qre_d_rt.o \
    qre_r_rt.o \
    qre_z_rt.o \
    qreslv_c_rt.o \
    qreslv_d_rt.o \
    qreslv_mixd_c_rt.o \
    qreslv_mixd_z_rt.o \
    qreslv_r_rt.o \
    qreslv_z_rt.o \
    sort_ins_idx_d_rt.o \
    sort_ins_idx_r_rt.o \
    sort_ins_idx_s08_rt.o \
    sort_ins_idx_s16_rt.o \
    sort_ins_idx_s32_rt.o \
    sort_ins_idx_u08_rt.o \
    sort_ins_idx_u16_rt.o \
    sort_ins_idx_u32_rt.o \
    sort_ins_val_d_rt.o \
    sort_ins_val_r_rt.o \
    sort_ins_val_s08_rt.o \
    sort_ins_val_s16_rt.o \
    sort_ins_val_s32_rt.o \
    sort_ins_val_u08_rt.o \
    sort_ins_val_u16_rt.o \
    sort_ins_val_u32_rt.o \
    sort_qk_idx_d_rt.o \
    sort_qk_idx_r_rt.o \
    sort_qk_idx_s08_rt.o \
    sort_qk_idx_s16_rt.o \
    sort_qk_idx_s32_rt.o \
    sort_qk_idx_u08_rt.o \
    sort_qk_idx_u16_rt.o \
    sort_qk_idx_u32_rt.o \
    sort_qk_val_d_rt.o \
    sort_qk_val_r_rt.o \
    sort_qk_val_s08_rt.o \
    sort_qk_val_s16_rt.o \
    sort_qk_val_s32_rt.o \
    sort_qk_val_u08_rt.o \
    sort_qk_val_u16_rt.o \
    sort_qk_val_u32_rt.o \
    srt_qid_findpivot_d_rt.o \
    srt_qid_findpivot_r_rt.o \
    srt_qid_partition_d_rt.o \
    srt_qid_partition_r_rt.o \
    srt_qkrec_c_rt.o \
    srt_qkrec_d_rt.o \
    srt_qkrec_r_rt.o \
    srt_qkrec_z_rt.o \
    randsrc_gc_c_rt.o \
    randsrc_gc_d_rt.o \
    randsrc_gc_r_rt.o \
    randsrc_gc_z_rt.o \
    randsrc_gz_c_rt.o \
    randsrc_gz_d_rt.o \
    randsrc_gz_r_rt.o \
    randsrc_gz_z_rt.o \
    randsrc_u_c_rt.o \
    randsrc_u_d_rt.o \
    randsrc_u_r_rt.o \
    randsrc_u_z_rt.o \
    randsrccreateseeds_32_rt.o \
    randsrccreateseeds_64_rt.o \
    randsrcinitstate_gc_32_rt.o \
    randsrcinitstate_gc_64_rt.o \
    randsrcinitstate_gz_rt.o \
    randsrcinitstate_u_32_rt.o \
    randsrcinitstate_u_64_rt.o \
    lpc2ac_d_rt.o \
    lpc2ac_r_rt.o \
    rc2ac_d_rt.o \
    rc2ac_r_rt.o \
    lpc2rc_d_rt.o \
    lpc2rc_r_rt.o \
    rc2lpc_d_rt.o \
    rc2lpc_r_rt.o \
    buf_copy_frame_to_mem_OL_1ch_rt.o \
    buf_copy_frame_to_mem_OL_rt.o \
    buf_copy_input_to_output_1ch_rt.o \
    buf_copy_input_to_output_rt.o \
    buf_copy_scalar_to_mem_OL_1ch_rt.o \
    buf_copy_scalar_to_mem_OL_rt.o \
    buf_copy_scalar_to_mem_UL_1ch_rt.o \
    buf_copy_scalar_to_mem_UL_rt.o \
    buf_output_frame_1ch_rt.o \
    buf_output_frame_rt.o \
    buf_output_scalar_1ch_rt.o \
    buf_output_scalar_rt.o \
    svd_c_rt.o \
    svd_d_rt.o \
    svd_r_rt.o \
    svd_z_rt.o \
    svdcopy_rt.o \
    unwrap_d_nrip_rt.o \
    unwrap_d_nrop_rt.o \
    unwrap_d_ripf_rt.o \
    unwrap_d_rips_rt.o \
    unwrap_d_ropf_rt.o \
    unwrap_d_rops_rt.o \
    unwrap_r_nrip_rt.o \
    unwrap_r_nrop_rt.o \
    unwrap_r_ripf_rt.o \
    unwrap_r_rips_rt.o \
    unwrap_r_ropf_rt.o \
    unwrap_r_rops_rt.o \
    upfir_copydata_rt.o \
    upfir_df_dblbuf_cc_rt.o \
    upfir_df_dblbuf_cr_rt.o \
    upfir_df_dblbuf_dd_rt.o \
    upfir_df_dblbuf_dz_rt.o \
    upfir_df_dblbuf_rc_rt.o \
    upfir_df_dblbuf_rr_rt.o \
    upfir_df_dblbuf_zd_rt.o \
    upfir_df_dblbuf_zz_rt.o \
    vfdly_clip_d_rt.o \
    vfdly_clip_r_rt.o \
    vfdly_fir_c_rt.o \
    vfdly_fir_d_rt.o \
    vfdly_fir_r_rt.o \
    vfdly_fir_z_rt.o \
    vfdly_lin_c_rt.o \
    vfdly_lin_d_rt.o \
    vfdly_lin_r_rt.o \
    vfdly_lin_z_rt.o \


dsp_rt.a : $(MAKEFILE) rtw_proj.tmw $(MODULES_dsp_rt)
	@echo "### Creating $@ "
	ar rs $@ $(MODULES_dsp_rt)
	@echo "### $@ Created "

MODULES_rtwlib = \
    rt_atan2.o \
    rt_atan232.o \
    rt_backsubcc_dbl.o \
    rt_backsubcc_sgl.o \
    rt_backsubrc_dbl.o \
    rt_backsubrc_sgl.o \
    rt_backsubrr_dbl.o \
    rt_backsubrr_sgl.o \
    rt_enab.o \
    rt_forwardsubcc_dbl.o \
    rt_forwardsubcc_sgl.o \
    rt_forwardsubcr_dbl.o \
    rt_forwardsubcr_sgl.o \
    rt_forwardsubrc_dbl.o \
    rt_forwardsubrc_sgl.o \
    rt_forwardsubrr_dbl.o \
    rt_forwardsubrr_sgl.o \
    rt_hypot.o \
    rt_hypot32.o \
    rt_i32zcfcn.o \
    rt_intrp1flatcd.o \
    rt_intrp1flatcf.o \
    rt_intrp1flatd.o \
    rt_intrp1flatf.o \
    rt_intrp1lincd.o \
    rt_intrp1lincf.o \
    rt_intrp1linxd.o \
    rt_intrp1linxf.o \
    rt_intrp2flatcd.o \
    rt_intrp2flatcf.o \
    rt_intrp2flatd.o \
    rt_intrp2flatf.o \
    rt_intrp2lincd.o \
    rt_intrp2lincf.o \
    rt_intrp2lind.o \
    rt_intrp2linf.o \
    rt_intrp2linxd.o \
    rt_intrp2linxf.o \
    rt_intrp3lind.o \
    rt_intrp3linf.o \
    rt_intrp4lind.o \
    rt_intrp4linf.o \
    rt_intrp5lind.o \
    rt_intrp5linf.o \
    rt_intrpnflatcd.o \
    rt_intrpnflatcf.o \
    rt_intrpnflatd.o \
    rt_intrpnflatf.o \
    rt_intrpnlincd.o \
    rt_intrpnlincf.o \
    rt_intrpnlind.o \
    rt_intrpnlinf.o \
    rt_intrpnlinxd.o \
    rt_intrpnlinxf.o \
    rt_intrpnspld.o \
    rt_intrpnsplf.o \
    rt_logging.o \
    rt_look.o \
    rt_look1d.o \
    rt_look1d32.o \
    rt_look2d32_general.o \
    rt_look2d32_normal.o \
    rt_look2d_general.o \
    rt_look2d_normal.o \
    rt_look32.o \
    rt_lookflat1bincsd.o \
    rt_lookflat1bincsf.o \
    rt_lookflat1binczd.o \
    rt_lookflat1binczf.o \
    rt_lookflat1evncsd.o \
    rt_lookflat1evncsf.o \
    rt_lookflat1evnczd.o \
    rt_lookflat1evnczf.o \
    rt_lookflat1lincsd.o \
    rt_lookflat1lincsf.o \
    rt_lookflat1linczd.o \
    rt_lookflat1linczf.o \
    rt_lookflat2bincsd.o \
    rt_lookflat2bincsf.o \
    rt_lookflat2binczd.o \
    rt_lookflat2binczf.o \
    rt_lookflat2evncsd.o \
    rt_lookflat2evncsf.o \
    rt_lookflat2evnczd.o \
    rt_lookflat2evnczf.o \
    rt_lookflat2lincsd.o \
    rt_lookflat2lincsf.o \
    rt_lookflat2linczd.o \
    rt_lookflat2linczf.o \
    rt_lookflatnbincsd.o \
    rt_lookflatnbincsf.o \
    rt_lookflatnbinczd.o \
    rt_lookflatnbinczf.o \
    rt_lookflatnevncsd.o \
    rt_lookflatnevncsf.o \
    rt_lookflatnevnczd.o \
    rt_lookflatnevnczf.o \
    rt_lookflatnlincsd.o \
    rt_lookflatnlincsf.o \
    rt_lookflatnlinczd.o \
    rt_lookflatnlinczf.o \
    rt_looklin1bincsd.o \
    rt_looklin1bincsf.o \
    rt_looklin1binczd.o \
    rt_looklin1binczf.o \
    rt_looklin1binxsd.o \
    rt_looklin1binxsf.o \
    rt_looklin1binxzd.o \
    rt_looklin1binxzf.o \
    rt_looklin1evncsd.o \
    rt_looklin1evncsf.o \
    rt_looklin1evnczd.o \
    rt_looklin1evnczf.o \
    rt_looklin1evnxsd.o \
    rt_looklin1evnxsf.o \
    rt_looklin1evnxzd.o \
    rt_looklin1evnxzf.o \
    rt_looklin1lincsd.o \
    rt_looklin1lincsf.o \
    rt_looklin1linczd.o \
    rt_looklin1linczf.o \
    rt_looklin1linxsd.o \
    rt_looklin1linxsf.o \
    rt_looklin1linxzd.o \
    rt_looklin1linxzf.o \
    rt_looklin2bincsd.o \
    rt_looklin2bincsf.o \
    rt_looklin2binczd.o \
    rt_looklin2binczf.o \
    rt_looklin2binxsd.o \
    rt_looklin2binxsf.o \
    rt_looklin2binxzd.o \
    rt_looklin2binxzf.o \
    rt_looklin2evncsd.o \
    rt_looklin2evncsf.o \
    rt_looklin2evnczd.o \
    rt_looklin2evnczf.o \
    rt_looklin2evnxsd.o \
    rt_looklin2evnxsf.o \
    rt_looklin2evnxzd.o \
    rt_looklin2evnxzf.o \
    rt_looklin2lincsd.o \
    rt_looklin2lincsf.o \
    rt_looklin2linczd.o \
    rt_looklin2linczf.o \
    rt_looklin2linxsd.o \
    rt_looklin2linxsf.o \
    rt_looklin2linxzd.o \
    rt_looklin2linxzf.o \
    rt_looklinnbincsd.o \
    rt_looklinnbincsf.o \
    rt_looklinnbinczd.o \
    rt_looklinnbinczf.o \
    rt_looklinnbinxsd.o \
    rt_looklinnbinxsf.o \
    rt_looklinnbinxzd.o \
    rt_looklinnbinxzf.o \
    rt_looklinnevncsd.o \
    rt_looklinnevncsf.o \
    rt_looklinnevnczd.o \
    rt_looklinnevnczf.o \
    rt_looklinnevnxsd.o \
    rt_looklinnevnxsf.o \
    rt_looklinnevnxzd.o \
    rt_looklinnevnxzf.o \
    rt_looklinnlincsd.o \
    rt_looklinnlincsf.o \
    rt_looklinnlinczd.o \
    rt_looklinnlinczf.o \
    rt_looklinnlinxsd.o \
    rt_looklinnlinxsf.o \
    rt_looklinnlinxzd.o \
    rt_looklinnlinxzf.o \
    rt_looksplnbincsd.o \
    rt_looksplnbincsf.o \
    rt_looksplnbinczd.o \
    rt_looksplnbinczf.o \
    rt_looksplnbinssd.o \
    rt_looksplnbinssf.o \
    rt_looksplnbinszd.o \
    rt_looksplnbinszf.o \
    rt_looksplnbinxsd.o \
    rt_looksplnbinxsf.o \
    rt_looksplnbinxzd.o \
    rt_looksplnbinxzf.o \
    rt_looksplnevncsd.o \
    rt_looksplnevncsf.o \
    rt_looksplnevnczd.o \
    rt_looksplnevnczf.o \
    rt_looksplnevnssd.o \
    rt_looksplnevnssf.o \
    rt_looksplnevnszd.o \
    rt_looksplnevnszf.o \
    rt_looksplnevnxsd.o \
    rt_looksplnevnxsf.o \
    rt_looksplnevnxzd.o \
    rt_looksplnevnxzf.o \
    rt_looksplnlincsd.o \
    rt_looksplnlincsf.o \
    rt_looksplnlinczd.o \
    rt_looksplnlinczf.o \
    rt_looksplnlinssd.o \
    rt_looksplnlinssf.o \
    rt_looksplnlinszd.o \
    rt_looksplnlinszf.o \
    rt_looksplnlinxsd.o \
    rt_looksplnlinxsf.o \
    rt_looksplnlinxzd.o \
    rt_looksplnlinxzf.o \
    rt_lu_cplx.o \
    rt_lu_cplx_sgl.o \
    rt_lu_real.o \
    rt_lu_real_sgl.o \
    rt_matdivcc_dbl.o \
    rt_matdivcc_sgl.o \
    rt_matdivcr_dbl.o \
    rt_matdivcr_sgl.o \
    rt_matdivrc_dbl.o \
    rt_matdivrc_sgl.o \
    rt_matdivrr_dbl.o \
    rt_matdivrr_sgl.o \
    rt_matmultandinccc_dbl.o \
    rt_matmultandinccc_sgl.o \
    rt_matmultandinccr_dbl.o \
    rt_matmultandinccr_sgl.o \
    rt_matmultandincrc_dbl.o \
    rt_matmultandincrc_sgl.o \
    rt_matmultandincrr_dbl.o \
    rt_matmultandincrr_sgl.o \
    rt_matmultcc_dbl.o \
    rt_matmultcc_sgl.o \
    rt_matmultcr_dbl.o \
    rt_matmultcr_sgl.o \
    rt_matmultrc_dbl.o \
    rt_matmultrc_sgl.o \
    rt_matmultrr_dbl.o \
    rt_matmultrr_sgl.o \
    rt_matrx.o \
    rt_nrand.o \
    rt_plookbincd.o \
    rt_plookbincf.o \
    rt_plookbinkcd.o \
    rt_plookbinkcf.o \
    rt_plookbinxd.o \
    rt_plookbinxf.o \
    rt_plookevncd.o \
    rt_plookevncf.o \
    rt_plookevnkcd.o \
    rt_plookevnkcf.o \
    rt_plookevnxd.o \
    rt_plookevnxf.o \
    rt_plooklincd.o \
    rt_plooklincf.o \
    rt_plooklinkcd.o \
    rt_plooklinkcf.o \
    rt_plooklinxd.o \
    rt_plooklinxf.o \
    rt_printf.o \
    rt_sat_div_int16.o \
    rt_sat_div_int32.o \
    rt_sat_div_int8.o \
    rt_sat_div_uint16.o \
    rt_sat_div_uint32.o \
    rt_sat_div_uint8.o \
    rt_sat_prod_int16.o \
    rt_sat_prod_int32.o \
    rt_sat_prod_int8.o \
    rt_sat_prod_uint16.o \
    rt_sat_prod_uint32.o \
    rt_sat_prod_uint8.o \
    rt_urand.o \
    rt_zcfcn.o \


rtwlib.a : $(MAKEFILE) rtw_proj.tmw $(MODULES_rtwlib)
	@echo "### Creating $@ "
	ar rs $@ $(MODULES_rtwlib)
	@echo "### $@ Created "



#----------------------------- Dependencies ------------------------------------

$(OBJS) : $(MAKEFILE) rtw_proj.tmw

$(SHARED_LIB) : $(SHARED_OBJS)
	@echo "### Creating $@ "
	ar ruvs $@ $(SHARED_OBJS)
	@echo "### $@ Created "


#--------- Miscellaneous rules to purge, clean and lint (sol2 only) ------------

purge : clean
	@echo "### Deleting the generated source code for $(MODEL)"
	@\rm -f $(MODEL).c $(MODEL).h $(MODEL)_types.h $(MODEL)_data.c \
	        $(MODEL)_private.h $(MODEL).rtw $(MODULES) rtw_proj.tmw $(MAKEFILE)

clean :
	@echo "### Deleting the objects, $(PRODUCT)"
	@\rm -f $(LINK_OBJS) $(PRODUCT) 

lint  : rtwlib.ln
	@lint -errchk -errhdr=%user -errtags=yes -F -L. -lrtwlib -x -Xc \
	      -erroff=$(LINT_ERROFF) \
	      -D_POSIX_C_SOURCE $(CFLAGS) $(LINT_SRCS)
	@\rm -f $(LINTOPTSFILE)
	@echo
	@echo "### Created lint output only, no executable"
	@echo

rtwlib.ln : $(MAKEFILE) rtw_proj.tmw
	@echo
	@echo "### Linting ..."
	@echo
	@\rm -f llib-lrtwlib.ln $(LINTOPTSFILE)
	@echo "-dirout=. -errchk -errhdr=%user " >> $(LINTOPTSFILE)
	@echo "-errtags -F -ortwlib -x -Xc " >> $(LINTOPTSFILE)
	@echo "-erroff=$(LINT_ERROFF) " >> $(LINTOPTSFILE)
	@echo "-D_POSIX_C_SOURCE $(CFLAGS) " >> $(LINTOPTSFILE)
	@for file in $(MATLAB_ROOT)/rtw/c/libsrc/*.c; do \
	  echo "$$file " >> $(LINTOPTSFILE); \
	done
	lint -flagsrc=$(LINTOPTSFILE)


# EOF: ert_unix.tmf
