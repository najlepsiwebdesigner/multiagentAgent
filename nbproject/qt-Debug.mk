#############################################################################
# Makefile for building: dist/Debug/GNU-Linux-x86/DP_agent
# Generated by qmake (2.01a) (Qt 4.8.6) on: ?t m�j 7 10:30:05 2015
# Project:  nbproject/qt-Debug.pro
# Template: app
# Command: /usr/bin/qmake-qt4 VPATH=. -o qttmp-Debug.mk nbproject/qt-Debug.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m64 -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m64 -pipe -std=c++11 -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++-64 -Inbproject -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I. -Inbproject -I.
LINK          = g++
LFLAGS        = -m64
LIBS          = $(SUBLIBS)  -L/usr/lib/x86_64-linux-gnu -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = build/Debug/GNU-Linux-x86/

####### Files

SOURCES       = Agent.cpp \
		AkcnyZasah.cpp \
		CComport.cpp \
		CiCreate.cpp \
		CiCreateDef.cpp \
		KoordinacnaSur.cpp \
		Poloha.cpp \
		PolohyAgentov.cpp \
		Prekazka.cpp \
		Prekazky.cpp \
		SocketConnector.cpp \
		SocketUtil.cpp \
		agentForm.cpp \
		main.cpp moc_agentForm.cpp
OBJECTS       = build/Debug/GNU-Linux-x86/Agent.o \
		build/Debug/GNU-Linux-x86/AkcnyZasah.o \
		build/Debug/GNU-Linux-x86/CComport.o \
		build/Debug/GNU-Linux-x86/CiCreate.o \
		build/Debug/GNU-Linux-x86/CiCreateDef.o \
		build/Debug/GNU-Linux-x86/KoordinacnaSur.o \
		build/Debug/GNU-Linux-x86/Poloha.o \
		build/Debug/GNU-Linux-x86/PolohyAgentov.o \
		build/Debug/GNU-Linux-x86/Prekazka.o \
		build/Debug/GNU-Linux-x86/Prekazky.o \
		build/Debug/GNU-Linux-x86/SocketConnector.o \
		build/Debug/GNU-Linux-x86/SocketUtil.o \
		build/Debug/GNU-Linux-x86/agentForm.o \
		build/Debug/GNU-Linux-x86/main.o \
		build/Debug/GNU-Linux-x86/moc_agentForm.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		nbproject/qt-Debug.pro
QMAKE_TARGET  = DP_agent
DESTDIR       = dist/Debug/GNU-Linux-x86/
TARGET        = dist/Debug/GNU-Linux-x86/DP_agent

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: qttmp-Debug.mk $(TARGET)

$(TARGET): ui_agentForm.h $(OBJECTS)  
	@$(CHK_DIR_EXISTS) dist/Debug/GNU-Linux-x86/ || $(MKDIR) dist/Debug/GNU-Linux-x86/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)
	{ test -n "$(DESTDIR)" && DESTDIR="$(DESTDIR)" || DESTDIR=.; } && test $$(gdb --version | sed -e 's,[^0-9][^0-9]*\([0-9]\)\.\([0-9]\).*,\1\2,;q') -gt 72 && gdb --nx --batch --quiet -ex 'set confirm off' -ex "save gdb-index $$DESTDIR" -ex quit '$(TARGET)' && test -f $(TARGET).gdb-index && objcopy --add-section '.gdb_index=$(TARGET).gdb-index' --set-section-flags '.gdb_index=readonly' '$(TARGET)' '$(TARGET)' && rm -f $(TARGET).gdb-index || true

qttmp-Debug.mk: nbproject/qt-Debug.pro  /usr/share/qt4/mkspecs/linux-g++-64/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/shared.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/x86_64-linux-gnu/libQtGui.prl \
		/usr/lib/x86_64-linux-gnu/libQtCore.prl
	$(QMAKE) VPATH=. -o qttmp-Debug.mk nbproject/qt-Debug.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/shared.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/x86_64-linux-gnu/libQtGui.prl:
/usr/lib/x86_64-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) VPATH=. -o qttmp-Debug.mk nbproject/qt-Debug.pro

dist: 
	@$(CHK_DIR_EXISTS) build/Debug/GNU-Linux-x86/DP_agent1.0.0 || $(MKDIR) build/Debug/GNU-Linux-x86/DP_agent1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) build/Debug/GNU-Linux-x86/DP_agent1.0.0/ && $(COPY_FILE) --parents Agent.h AkcnyZasah.h CComport.h CiCreate.h CiCreateDef.h KoordinacnaSur.h Poloha.h PolohyAgentov.h Prekazka.h Prekazky.h SocketConnector.h SocketUtil.h agentForm.h build/Debug/GNU-Linux-x86/DP_agent1.0.0/ && $(COPY_FILE) --parents Agent.cpp AkcnyZasah.cpp CComport.cpp CiCreate.cpp CiCreateDef.cpp KoordinacnaSur.cpp Poloha.cpp PolohyAgentov.cpp Prekazka.cpp Prekazky.cpp SocketConnector.cpp SocketUtil.cpp agentForm.cpp main.cpp build/Debug/GNU-Linux-x86/DP_agent1.0.0/ && $(COPY_FILE) --parents agentForm.ui build/Debug/GNU-Linux-x86/DP_agent1.0.0/ && (cd `dirname build/Debug/GNU-Linux-x86/DP_agent1.0.0` && $(TAR) DP_agent1.0.0.tar DP_agent1.0.0 && $(COMPRESS) DP_agent1.0.0.tar) && $(MOVE) `dirname build/Debug/GNU-Linux-x86/DP_agent1.0.0`/DP_agent1.0.0.tar.gz . && $(DEL_FILE) -r build/Debug/GNU-Linux-x86/DP_agent1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) qttmp-Debug.mk


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_agentForm.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_agentForm.cpp
moc_agentForm.cpp: ui_agentForm.h \
		Agent.h \
		SocketConnector.h \
		SocketUtil.h \
		Poloha.h \
		KoordinacnaSur.h \
		Prekazka.h \
		Prekazky.h \
		PolohyAgentov.h \
		CiCreate.h \
		CiCreateDef.h \
		CComport.h \
		agentForm.h \
		AkcnyZasah.h \
		agentForm.h
	/usr/lib/x86_64-linux-gnu/qt4/bin/moc $(DEFINES) $(INCPATH) agentForm.h -o moc_agentForm.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_agentForm.h
compiler_uic_clean:
	-$(DEL_FILE) ui_agentForm.h
ui_agentForm.h: agentForm.ui
	/usr/lib/x86_64-linux-gnu/qt4/bin/uic agentForm.ui -o ui_agentForm.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

build/Debug/GNU-Linux-x86/Agent.o: Agent.cpp Agent.h \
		SocketConnector.h \
		SocketUtil.h \
		ui_agentForm.h \
		Poloha.h \
		KoordinacnaSur.h \
		Prekazka.h \
		Prekazky.h \
		PolohyAgentov.h \
		agentForm.h \
		CiCreate.h \
		CiCreateDef.h \
		CComport.h \
		AkcnyZasah.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/Agent.o Agent.cpp

build/Debug/GNU-Linux-x86/AkcnyZasah.o: AkcnyZasah.cpp AkcnyZasah.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/AkcnyZasah.o AkcnyZasah.cpp

build/Debug/GNU-Linux-x86/CComport.o: CComport.cpp CComport.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/CComport.o CComport.cpp

build/Debug/GNU-Linux-x86/CiCreate.o: CiCreate.cpp CiCreate.h \
		Agent.h \
		SocketConnector.h \
		SocketUtil.h \
		ui_agentForm.h \
		Poloha.h \
		KoordinacnaSur.h \
		Prekazka.h \
		Prekazky.h \
		PolohyAgentov.h \
		CiCreateDef.h \
		CComport.h \
		agentForm.h \
		AkcnyZasah.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/CiCreate.o CiCreate.cpp

build/Debug/GNU-Linux-x86/CiCreateDef.o: CiCreateDef.cpp CiCreateDef.h \
		CComport.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/CiCreateDef.o CiCreateDef.cpp

build/Debug/GNU-Linux-x86/KoordinacnaSur.o: KoordinacnaSur.cpp KoordinacnaSur.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/KoordinacnaSur.o KoordinacnaSur.cpp

build/Debug/GNU-Linux-x86/Poloha.o: Poloha.cpp Poloha.h \
		KoordinacnaSur.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/Poloha.o Poloha.cpp

build/Debug/GNU-Linux-x86/PolohyAgentov.o: PolohyAgentov.cpp PolohyAgentov.h \
		Poloha.h \
		KoordinacnaSur.h \
		Prekazka.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/PolohyAgentov.o PolohyAgentov.cpp

build/Debug/GNU-Linux-x86/Prekazka.o: Prekazka.cpp Prekazka.h \
		Poloha.h \
		KoordinacnaSur.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/Prekazka.o Prekazka.cpp

build/Debug/GNU-Linux-x86/Prekazky.o: Prekazky.cpp Prekazky.h \
		Prekazka.h \
		Poloha.h \
		KoordinacnaSur.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/Prekazky.o Prekazky.cpp

build/Debug/GNU-Linux-x86/SocketConnector.o: SocketConnector.cpp SocketConnector.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/SocketConnector.o SocketConnector.cpp

build/Debug/GNU-Linux-x86/SocketUtil.o: SocketUtil.cpp SocketUtil.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/SocketUtil.o SocketUtil.cpp

build/Debug/GNU-Linux-x86/agentForm.o: agentForm.cpp agentForm.h \
		ui_agentForm.h \
		Agent.h \
		SocketConnector.h \
		SocketUtil.h \
		Poloha.h \
		KoordinacnaSur.h \
		Prekazka.h \
		Prekazky.h \
		PolohyAgentov.h \
		CiCreate.h \
		CiCreateDef.h \
		CComport.h \
		AkcnyZasah.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/agentForm.o agentForm.cpp

build/Debug/GNU-Linux-x86/main.o: main.cpp agentForm.h \
		ui_agentForm.h \
		Agent.h \
		SocketConnector.h \
		SocketUtil.h \
		Poloha.h \
		KoordinacnaSur.h \
		Prekazka.h \
		Prekazky.h \
		PolohyAgentov.h \
		CiCreate.h \
		CiCreateDef.h \
		CComport.h \
		AkcnyZasah.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/main.o main.cpp

build/Debug/GNU-Linux-x86/moc_agentForm.o: moc_agentForm.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/Debug/GNU-Linux-x86/moc_agentForm.o moc_agentForm.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

