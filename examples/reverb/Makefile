#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")
endif

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

ifeq ($(strip $(FEOSSDK)),)
$(error "Please set FEOSSDK in your environment. export FEOSSDK=<path to>FeOS/sdk")
endif

FEOSMK = $(FEOSSDK)/mk

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
#---------------------------------------------------------------------------------
TARGET        := mmreverb
BUILD         := build
SOURCES       := source
DATA          := data
INCLUDES      := build include
AUDIO         := audio

CONF_DEFINES =
CONF_USERLIBS = maxmod libfar
CONF_LIBS = -lmaxmod -llibfar
CONF_FSDIR = fs

CONF_SNDBNKFOLDER = $(CONF_FSDIR)
CONF_SNDBNKHFOLDER = $(BUILD)
CONF_SNDBNKNAME = soundbank

SNDBNK   := $(CURDIR)/$(CONF_SNDBNKFOLDER)/$(CONF_SNDBNKNAME).bin
SNDBNK_H := $(CURDIR)/$(CONF_SNDBNKHFOLDER)/$(CONF_SNDBNKNAME).h
CONF_PREREQUISITES = $(SNDBNK)
CONF_EXTRACLEAN += $(SNDBNK) $(SNDBNK_H)

#AUDIOFILES := $(foreach dir,$(notdir $(wildcard $(AUDIO)/*.*)),$(CURDIR)/$(MUSIC)/$(dir))
AUDIOFILES := $(wildcard $(CURDIR)/$(AUDIO)/*.*)

include $(FEOSMK)/app.mk

install: all
	@cp $(TARGET).fx2 $(FEOSDEST)/data/FeOS/bin/$(TARGET).fx2 || exit 1

$(SNDBNK): $(AUDIOFILES)
	@echo Building soundbank...
	@[ -d $(CONF_SNDBNKFOLDER) ] || mkdir -p $(CONF_SNDBNKFOLDER)
	@[ -d $(CONF_SNDBNKHFOLDER) ] || mkdir -p $(CONF_SNDBNKHFOLDER)
	@mmutil $^ -d -o$(SNDBNK) -h$(SNDBNK_H)
