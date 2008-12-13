#
# Makefile for a Video Disk Recorder plugin
#
# $Id: Makefile,v 1.17 2003/05/02 04:11:20 ragge Exp $

# The official name of this plugin.
# This name will be used in the '-P...' option of VDR to load the plugin.
# By default the main source file also carries this name.
#
PLUGIN = ttxtsubs

### The version number of this plugin (taken from the main source file):

VERSION = $(shell grep 'static const char \*VERSION *=' $(PLUGIN).c | awk '{ print $$6 }' | sed -e 's/[";]//g')

### The C++ compiler and options:

CXX      ?= g++
CXXFLAGS ?= -O2 -Wall -Woverloaded-virtual

### The directory environment:

DVBDIR = ../../../../DVB
VDRDIR = ../../..
LIBDIR = ../../lib
TMPDIR = /tmp

### Allow user defined options to overwrite defaults:

-include $(VDRDIR)/Make.config

### The version number of VDR (taken from VDR's "config.h"):

VDRVERSION = $(shell grep 'define VDRVERSION ' $(VDRDIR)/config.h | awk '{ print $$3 }' | sed -e 's/"//g')

### The name of the distribution archive:

ARCHIVE = $(PLUGIN)-$(VERSION)
PACKAGE = vdr-$(ARCHIVE)
BUPPACKAGE = vdr-$(ARCHIVE)-bup-$(shell date +%y%m%d-%H%M%S)

### Includes and Defines (add further entries here):

INCLUDES += -I$(VDRDIR)/include -I$(DVBDIR)/include

DEFINES += -DPLUGIN_NAME_I18N='"$(PLUGIN)"'

### The object files (add further files here):

OBJS = $(PLUGIN).o ttxtsubsdisplayer.o ttxtsubsdisplay.o teletext.o siinfo.o \
	ttxtsubsfilter.o ttxtsubsrecorder.o ttxtsubsreceiver.o

SOURCEFILES = *.c *.h [A-Z]???*

### Implicit rules:

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $(DEFINES) $(INCLUDES) $<

# Dependencies:

MAKEDEP = g++ -MM -MG
DEPFILE = .dependencies
$(DEPFILE): Makefile
	@$(MAKEDEP) $(DEFINES) $(INCLUDES) $(OBJS:%.o=%.c) > $@

-include $(DEPFILE)

### Targets:

all: libvdr-$(PLUGIN).so

libvdr-$(PLUGIN).so: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared $(OBJS) -o $@
	@cp $@ $(LIBDIR)/$@.$(VDRVERSION)

dist: clean
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@mkdir $(TMPDIR)/$(ARCHIVE)
	@cp -a $(SOURCEFILES) $(TMPDIR)/$(ARCHIVE)
	@tar czf $(PACKAGE).tgz -C $(TMPDIR) $(ARCHIVE)
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@echo Distribution package created as $(PACKAGE).tgz

bup: clean patch
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@mkdir $(TMPDIR)/$(ARCHIVE)
	@cp -a $(SOURCEFILES) RCS $(TMPDIR)/$(ARCHIVE)
	@tar czf $(BUPPACKAGE).tgz -C $(TMPDIR) $(ARCHIVE)
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@echo Distribution package created as $(BUPPACKAGE).tgz
	@echo making backup...
	@sh RCS/backup.sh $(BUPPACKAGE).tgz
	@echo done.

clean:
	@-rm -f $(OBJS) $(DEPFILE) *.so *.tgz core* *~

#ci: patch	
ci:
	ci -u $(SOURCEFILES)

patch:
	co -l xVDR.patch.temp vdrttxtsubshooks.c vdrttxtsubshooks.h
	(cd ../../../; diff -upr ./DIST/* .) | grep -v "Only in ." > xVDR.patch.temp
	cp -p ../../../vdrttxtsubshooks.c .
	cp -p ../../../vdrttxtsubshooks.h .
	ci -u xVDR.patch.temp vdrttxtsubshooks.c vdrttxtsubshooks.h
