##
## Common settings for Thredbo
##

## Configuration: release/debug

ifeq ($(MAKECMDGOALS),all)
CONFIG					:=debug
endif
ifeq ($(MAKECMDGOALS),debug)
CONFIG					:=debug
endif
ifeq ($(CONFIG),debug)
CONFIG					:=debug
#CONFIGFLAGS				:=-g3 -gdwarf-2 -O0
CONFIGFLAGS				:=-fno-tree-vrp -fexpensive-optimizations -fomit-frame-pointer -frename-registers -g -D_THREDBO_DEBUG
PLATFORM				:=thredbo
endif
ifeq ($(MAKECMDGOALS),package)
CONFIG					:=release
CONFIGFLAGS				:=-O3 -fexpensive-optimizations -fomit-frame-pointer -frename-registers 
PLATFORM				:=thredbo
endif
ifeq ($(MAKECMDGOALS),release)
#CONFIGFLAGS				:=-O3 -fexpensive-optimizations -fomit-frame-pointer -frename-registers 
CONFIGFLAGS				:=-O3 -fno-tree-vrp -fexpensive-optimizations -fomit-frame-pointer -frename-registers 
PLATFORM				:=thredbo
CONFIG					:=release
endif
ifeq ($(MAKECMDGOALS),clean)
CONFIG					:=clean
PLATFORM				:=thredbo
endif
##this is for the linux host machine build - valid only for API
ifeq ($(MAKECMDGOALS),linux)
CONFIG					:=debug
CONFIGFLAGS				:=-g3 -gdwarf-2 -O0
PLATFORM				:=linux
endif
##this is for the hercules platform build - valid only for API
ifeq ($(MAKECMDGOALS),hercules_all)
CONFIG					:=hercules_debug
endif
ifeq ($(MAKECMDGOALS),hercules_debug)
CONFIG					:=hercules_debug
endif
ifeq ($(CONFIG),hercules_debug)
CONFIG					:=hercules_debug
#CONFIGFLAGS				:=-g3 -gdwarf-2 -O0
CONFIGFLAGS				:=-fno-tree-vrp -fexpensive-optimizations -fomit-frame-pointer -frename-registers -g -D_THREDBO_DEBUG
PLATFORM				:=hercules
endif
ifeq ($(MAKECMDGOALS),hercules_package)
CONFIG					:=hercules_release
CONFIGFLAGS				:=-O3 -fexpensive-optimizations -fomit-frame-pointer -frename-registers 
PLATFORM				:=hercules
endif
ifeq ($(MAKECMDGOALS),hercules_release)
#CONFIGFLAGS				:=-O3 -fexpensive-optimizations -fomit-frame-pointer -frename-registers 
CONFIGFLAGS				:=-O3 -fno-tree-vrp -fexpensive-optimizations -fomit-frame-pointer -frename-registers 
PLATFORM				:=hercules
CONFIG					:=hercules_release
endif
ifeq ($(MAKECMDGOALS),hercules_clean)
CONFIG					:=hercules_clean
PLATFORM				:=hercules
endif
##this is for the Ubuntu host machine build - valid only for API
ifeq ($(MAKECMDGOALS),ubuntu_release)
CONFIG					:=ubuntu_release
CONFIGFLAGS				:=-O3 -fexpensive-optimizations -fomit-frame-pointer -frename-registers
PLATFORM				:=ubuntu
endif
ifeq ($(MAKECMDGOALS),ubuntu_debug)
CONFIG					:=ubuntu_debug
CONFIGFLAGS				:=-fno-tree-vrp -fexpensive-optimizations -fomit-frame-pointer -frename-registers -g -D_THREDBO_DEBUG
PLATFORM				:=ubuntu
endif
ifeq ($(MAKECMDGOALS),ubuntu_clean)
CONFIG					:=ubuntu_clean
PLATFORM				:=ubuntu
endif

ifeq ($(MAKECMDGOALS),linux64_release)
CONFIG					:=linux64_release
CONFIGFLAGS				:=-O3 -fexpensive-optimizations -fomit-frame-pointer -frename-registers
PLATFORM				:=linux64
endif
ifeq ($(MAKECMDGOALS),linux64_debug)
CONFIG					:=linux64_debug
#CONFIGFLAGS				:=-fno-tree-vrp -fexpensive-optimizations -fomit-frame-pointer -frename-registers -g -D_THREDBO_DEBUG
CONFIGFLAGS				:=-g -D_THREDBO_DEBUG
PLATFORM				:=linux64
endif
ifeq ($(MAKECMDGOALS),linux64_clean)
CONFIG					:=linux64_clean
PLATFORM				:=linux64
endif


ifeq ($(OS),Windows_NT)
sep					:=\\
MKDIR				:=mkdir
COPY				:=copy
COPYDIR				:=xcopy /s /y /E
RM					:=cs-rm -f
RMDIR				:=$(RM) -r -d 
COMMANDSEPARATOR	:= &
TAR					:=..\build-tools\\bsdtar\\bin\\bsdtar.exe
else
sep					:=/
MKDIR				:=mkdir -p
COPY				:=cp
COPYDIR				:=cp -r -v
RM					:=rm -f
RMDIR				:=$(RM) -r
COMMANDSEPARATOR	:= \
TAR					:=tar
endif

ifeq ($(PLATFORM),linux)
TARGET				:=
else ifeq ($(PLATFORM),hercules)
TARGET				:=
else
#TARGET				:=
TARGET				:=-mcpu=cortex-a8
endif
ifeq ($(PLATFORM),ubuntu)
CXX					:=g++
CC					:=gcc
SOCXX				:=g++ -shared
AR					:=ar
STRIP				:=strip
TARGET				:=-m32
endif
ifeq ($(PLATFORM),linux64)
SDK_PATH			:=/home/symbol/embedded_sdk_rfid/fxr90/Zebra-FXR90_Embedded-Native-SDK-C-CPP_Linux_V1.0.0
#CXX					:=g++
CXX					:=$(SDK_PATH)/tools/wrappers/aarch64-poky-linux-g++
#CC					:=gcc
CC					:=$(SDK_PATH)/tools/wrappers/aarch64-poky-linux-gcc
SOCXX				:=$(SDK_PATH)/tools/wrappers/aarch64-poky-linux-g++ -shared
AR					:=$(SDK_PATH)/tools/wrappers/aarch64-poky-linux-ar
STRIP				:=$(SDK_PATH)/tools/wrappers/aarch64-poky-linux-strip
#TARGET				:=-m64
#ARGET				:=-mcpu=cortex-a8 -mfloat-abi=hard
#TARGET				:=-mfloat-abi=hard
TARGET				:=--sysroot=$(SDK_PATH)/rootfs
else
CXX					:=aarch64-poky-linux-g++
CC					:=aarch64-poky-linux-gcc
SOCXX				:=aarch64-poky-linux-g++ -shared
AR					:=aarch64-poky-linux-ar
STRIP				:=aarch64-poky-linux-strip
endif
GLOBALCXXFLAGS		:=$(CONFIGFLAGS) -Wall -c -fmessage-length=0
GLOBALOUTDIR		:=BuildResults
REMOTELIBPATH		:=/platform/lib /usr/lib /apps/bin/libs 
RPATHOPTIONS		:="-Wl,-rpath,$(REMOTELIBPATH)"
