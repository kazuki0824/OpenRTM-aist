## -*- Makefile -*-
##---------------------------------------------------------------------------
## Makefile.am for ConsoleIn/ConsoleOut components
##
## $Id$
##---------------------------------------------------------------------------

AUTOMAKE_OPTIONS = 1.9

AM_CPPFLAGS=-I$(top_srcdir)/src/lib -I$(top_srcdir)/src/lib/coil/include -I$(top_srcdir)/src/lib/rtm/idl
AM_LDFLAGS=-L$(top_builddir) -L$(top_builddir)/src/lib/rtm -L$(top_builddir)/src/lib/rtm/idl

#------------------------------------------------------------
# Targets
#------------------------------------------------------------
# Targets directories
exampledir = $(rtm_exampledir)
socompdir  = $(exampledir)/rtc
sourcedir  = $(exampledir)/src/ConfigSample

# Loadable RTCs
socomp_LTLIBRARIES = ConfigSample.la

ConfigSample_la_SOURCES = ConfigSample.cpp ConfigSample.h
ConfigSample_la_LDFLAGS = -module
ConfigSample_la_FLAGS   = -L$(top_builddir)/src/lib/rtm/.lib
ConfigSample_la_LIBADD  = $(top_builddir)/src/lib/rtm/libRTC.la \
	$(top_builddir)/src/lib/coil/lib/libcoil.la


# Standalone RTCs
example_PROGRAMS = ConfigSampleComp
example_DATA     = configsample.conf

ConfigSampleComp_SOURCES = ConfigSampleComp.cpp 
ConfigSampleComp_DEPENDENCIES = ConfigSample.la
ConfigSampleComp_LDFLAGS = -L$(top_builddir)/src/lib/rtm/.libs
ConfigSampleComp_LDADD   = ConfigSample.o \
	$(top_builddir)/src/lib/rtm/libRTC.la \
	$(top_builddir)/src/lib/coil/lib/libcoil.la

# Example sources
source_DATA = $(ALL_SRC) $(EXTRA_DIST)

#------------------------------------------------------------
# Extra distributed files
#------------------------------------------------------------
EXTRA_DIST =                  \
	gen.sh                \
	rtc.conf              \
	configsample.conf     \
	VectorConvert.h       \
	Makefile.ConfigSample \
	README.ConfigSample

ALL_SRC =                    \
	ConfigSampleComp.cpp \
	ConfigSample.cpp     \
	ConfigSample.h

#------------------------------------------------------------
# File list for deb/ports packages
#------------------------------------------------------------
lst:
	echo $(ALL_SRC) > src.lst
	echo $(EXTRA_DIST) > other.lst

#------------------------------------------------------------
# Visual Studio Project
#------------------------------------------------------------
win32_builddir=$(top_builddir)/win32/OpenRTM-aist/examples/ConfigSample/

vcproj: vc9proj vcxproj

vc9proj:
	$(top_builddir)/build/vcprojtool.py vcproj              \
		--type EXE                                      \
		--vcversion "9.00"                              \
		--projectname "ConfigSampleComp"                \
		--version $(RTM_VERSION)                        \
		--out $(win32_builddir)/ConfigSample_vc9.vcproj \
		--yaml ../rtc.vcproj.yaml                       \
		--source ConfigSample.cpp ConfigSampleComp.cpp  \
		--header ConfigSample.h VectorConvert.h
	$(top_builddir)/build/vcprojtool.py vcproj                 \
		--type DLL                                         \
		--vcversion "9.00"                                 \
		--projectname "ConfigSample"                       \
		--version $(RTM_VERSION)                           \
		--out $(win32_builddir)/ConfigSampleDll_vc9.vcproj \
		--yaml ../rtcdll.vcproj.yaml                       \
		--source ConfigSample.cpp                          \
		--header ConfigSample.h VectorConvert.h

vcxproj:
	for ver in $(VC_VERSION); do \
		$(top_builddir)/build/vcxprojtool.py vcxproj              \
			--type EXE                                      \
			--vcversion "$${ver}.00"                              \
			--projectname "ConfigSampleComp"                \
			--version $(RTM_VERSION)                        \
			--out $(win32_builddir)/ConfigSample_vc$${ver}.vcxproj \
			--yaml ../rtc.vcproj.yaml                       \
			--source ConfigSample.cpp ConfigSampleComp.cpp  \
			--header ConfigSample.h VectorConvert.h ; \
		$(top_builddir)/build/vcxprojtool.py vcxproj                 \
			--type DLL                                         \
			--vcversion "$${ver}.00"                                 \
			--projectname "ConfigSample"                       \
			--version $(RTM_VERSION)                           \
			--out $(win32_builddir)/ConfigSampleDll_vc$${ver}.vcxproj \
			--yaml ../rtcdll.vcproj.yaml                       \
			--source ConfigSample.cpp                          \
			--header ConfigSample.h VectorConvert.h ; \
	done

dist-hook: lst vcproj


clean-local:
	rm -f *.bak *.rpo *.sym lib*.*_pure_* 
	rm -f *.lst *.core *~ *.so *.log
	rm -f Makefile.old 

