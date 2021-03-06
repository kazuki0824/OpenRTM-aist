## -*- Makefile -*-
##---------------------------------------------------------------------------
## Makefile.am for coil (Component Object Infrastructure Layer)
##
## $Id$
##---------------------------------------------------------------------------

# Win32 API dependent sources
COIL_PLATFORM_SRC =    \
	Condition.cpp  \
	DynamicLib.cpp \
	Mutex.cpp      \
	Process.cpp    \
	Routing.cpp    \
	Signal.cpp     \
	Task.cpp       \
	Time.cpp       \
	UUID.cpp       \
	SharedMemory.cpp \
	Affinity.cpp

# Win32 API dependent headers
COIL_PLATFORM_H =      \
	config_coil.h  \
	atomic.h       \
	File.h         \
	OS.h           \
	$(COIL_PLATFORM_SRC:.cpp=.h)

# coil common source
COIL_COMMON_SRC =        \
	Allocator.cpp    \
	PeriodicTask.cpp \
	Properties.cpp   \
	TimeMeasure.cpp  \
	TimeValue.cpp    \
	Timer.cpp        \
	ClockManager.cpp \
	crc.cpp          \
	stringutil.cpp

# coil common headers
COIL_COMMON_H   =           \
	Allocator.h         \
	Async.h             \
	Factory.h           \
	Guard.h             \
	Listener.h          \
	Logger.h            \
	NonCopyable.h       \
	PeriodicTask.h      \
	PeriodicTaskBase.h  \
	Properties.h        \
	Singleton.h         \
	TimeMeasure.h       \
	TimeValue.h         \
	Timer.h             \
	ClockManager.h      \
	crc.h               \
	stringutil.h

# coil all sources and headers
COIL_SRC = $(COIL_COMMON_SRC) $(COIL_PLATFORM_SRC)
COIL_H   = $(COIL_COMMON_H) $(COIL_PLATFORM_H)
COIL_ALL = $(COIL_SRC) $(COIL_H)


$(COIL_COMMON_SRC):
	cp -p $(top_srcdir)/common/*.cpp .

$(COIL_COMMON_H):
	cp -p $(top_srcdir)/common/*.h .


clean-local:
	rm -f *~
	rm -f $(COIL_COMMON_SRC) $(COIL_COMMON_H)
	rm -f *.vcproj
	rm -f *.vcxproj
	rm -f *.user
	rm -f *.def
	rm -f *.lst
	rm -rf Release Debug

EXTRA_DIST =                \
	$(COIL_ALL)         \
	libcoil_vc9.vcproj  \
	libcoil_vc10.vcxproj  \
	libcoil.vcproj.yaml

#------------------------------------------------------------
# File list for deb/ports packages
#------------------------------------------------------------
lst:
	echo $(COIL_SRC) > src.lst
	echo $(COIL_H) > header.lst

#------------------------------------------------------------
# vcproj file build rules
#------------------------------------------------------------
win32_builddir = .

vcproj: libcoil_vc9.vcproj libcoil.vcxproj

libcoil_vc9.vcproj: libcoil.vcproj.yaml $(COIL_ALL)
	$(top_builddir)/build/vcprojtool.py vcproj \
		--type DLL \
		--vcversion "9.00" \
		--version $(COIL_VERSION) \
		--out $(win32_builddir)/libcoil_vc9.vcproj \
		--yaml libcoil.vcproj.yaml \
		--source $(COIL_SRC) \
		--header $(COIL_H)

libcoil.vcxproj: libcoil.vcproj.yaml $(COIL_ALL)
	for ver in $(VC_VERSION); do \
		$(top_builddir)/build/vcxprojtool.py vcxproj \
			--type DLL \
			--vcversion "$${ver}.00" \
			--version $(COIL_VERSION) \
			--out $(win32_builddir)/libcoil_vc$${ver}.vcxproj \
			--yaml libcoil.vcproj.yaml \
			--source $(COIL_SRC) \
			--header $(COIL_H) ; \
	done

dist-hook: lst vcproj

