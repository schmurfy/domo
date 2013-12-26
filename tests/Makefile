
#Set this to @ to keep the makefile quiet
# ifndef SILENCE
# 	SILENCE = @
# endif

#--- Inputs ----#
COMPONENT_NAME = LibTests
CPPUTEST_HOME = /usr/local/Cellar/cpputest/3.5/

CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = Clang

# CPPUTEST_CPPFLAGS += -fprofile-arcs -ftest-coverage
# LD_LIBRARIES += -lprofile_rt

# This line is overriding the default new macros. This is helpful
# when using std library includes like <list> and other containers
# so that memory leak detection does not conflict with stl.
# CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE = -include ApplicationLib/ExamplesNewOverrides.h
SRC_DIRS = \
	../libs/serial

TEST_SRC_DIRS = \
  libs

INCLUDE_DIRS =\
	../libs/serial \
  stubs

include $(CPPUTEST_HOME)/build/MakefileWorker.mk

