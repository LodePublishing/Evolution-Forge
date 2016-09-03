WWXPATH=/home/clawg/temp/wxWindows-2.4.2/
LWXPATH=/home/clawg/temp2/wxGTK-2.4.2/
PROGRAM=scc2
WCXX=i386-mingw32msvc-g++
CXX=g++
DLLWRAP=i386-mingw32msvc-dllwrap
DLLPATH=$(PROGRAM)dll
OBJDLL=$(DLLPATH)/anarace.o $(DLLPATH)/debug.o $(DLLPATH)/ga.o $(DLLPATH)/goal.o  $(DLLPATH)/harvest.o  $(DLLPATH)/location.o  $(DLLPATH)/map.o  $(DLLPATH)/player.o  $(DLLPATH)/prerace.o  $(DLLPATH)/race.o  $(DLLPATH)/settings.o  $(DLLPATH)/soup.o
SOURCEDLL=$(DLLPATH)/anarace.cpp $(DLLPATH)/debug.cpp $(DLLPATH)/ga.cpp $(DLLPATH)/goal.cpp  $(DLLPATH)/harvest.cpp  $(DLLPATH)/location.cpp  $(DLLPATH)/map.cpp  $(DLLPATH)/player.cpp  $(DLLPATH)/prerace.cpp  $(DLLPATH)/race.cpp  $(DLLPATH)/settings.cpp  $(DLLPATH)/soup.cpp
OBJMAIN=$(PROGRAM)/scc2.o
DLLFLAGS=-DBUILD_DLL
LIBS=-L./ $(PROGRAM).so.1.0
WRES_PROGRAM_OBJ = $(PROGRAM)_resourcesw.o
LRES_PROGRAM_OBJ = $(PROGRAM)_resourcesl.o

WEXTRALIBS =  -Wl,--subsystem,windows -mwindows -mthreads  -Wl,--version-script,$(WWXPATH)/version-script -lrpcrt4 -loleaut32 -lole32 -luuid -lwinspool -lwinmm -lshell32 -lcomctl32 -lctl3d32 -ladvapi32 -lwsock32 -l$(PROGRAM)

EXTRALIBS = -Wl,--version-script,$(LWXPATH)/version-script -L/usr/lib -L/usr/X11R6/lib -lgtk -lgdk -rdynamic -lgmodule -lglib -ldl -lXi -lXext -lX11 -lm -lpng -ljpeg -ltiff -lz -ldl -lm
#-lgthread -lglib -ldl -lXi -lXext -lX11 -lm -lpng -ljpeg -ltiff -lz -ldl -lm
 
#-pthread   -Wl,--version-script,$(LWXPATH)/version-script -L/usr/lib -L/usr/X11R6/lib -lgtk -lgdk -rdynamic -lgmodule -lgthread -lglib -lpthread -ldl -lXi -lXext -lX11 -lm -lpng -ljpeg -ltiff -lz -ldl -lm $(PROGRAM).so.1.0

WLDLIBS=$(WWXPATH)/lib/libwx_msw-2.4-i386-mingw32msvc.dll $(WEXTRALIBS)
LDLIBS=$(LWXPATH)/lib/libwx_gtk-2.4.so.0.1.1 $(EXTRALIBS)

WXLIB=libwx_msw-2.4-i386-mingw32msvc.a
WXSHLIB=libwx_msw-2.4-i386-mingw32msvc.dll
WXLIBBASE=wx_msw
SHARED_LD=i386-mingw32msvc-c++ -shared -Wl,--out-implib,lib/libwx_msw-2.4-i386-mingw32msvc.dll.a -o
WRESFLAGS=--include-dir $(WWXPATH)/include --define __WIN32__ --define __WIN95__ --define __GNUWIN32__ 
LRESFLAGS=--include-dir $(LWXPATH)/include --define __WXGTK__ --define __UNIX__

#RESCOMP=i386-mingw32msvc-windres
WCPPFLAGS = -I$(WWXPATH)/lib/wx/include/msw-2.4-i386-mingw32msvc -I$(WWXPATH)/include -I$(WWXPATH)/src/regex -I$(WWXPATH)/src/zlib -I$(WWXPATH)/src/png -I$(WWXPATH)/src/jpeg -I$(WWXPATH)/src/tiff -D_WIN32_IE=0x400 $(EXTRADEFS)  -D__WXMSW__ -mthreads -DWXUSINGDLL=1

CPPFLAGS = -I$(LWXPATH)/lib/wx/include/gtk-2.4 -I$(LWXPATH)/include -I/user/include/gtk-1.2 -I/usr/include/glib-1.2 -I/usr/lib/glib/include -D_REENTRANT -I/usr/X11R6/include -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -DGTK_NO_CHECK_CASTS

WCXXFLAGS = $(WCPPFLAGS) -fno-pcc-struct-return -MMD -mthreads -Wall
CXXFLAGS = $(CPPFLAGS) -O3 -MMD -Wall -g
#-pthread -Wall 

#WPICFLAGS = -UWXUSINGDLL -DWXMAKINGDLL=1 -D_DLL=1 -D_WINDLL=1
#LPICFLAGS = -fPIC

.SUFFIXES: .o .cpp .rc _resourcesw.o _resourcesl.o
.rc_resourcesw.o:
	$(RESCOMP) -i $< -o $@ $(WRESFLAGS)

.rc_resourcesl.o:
	$(RESCOMP) -i $< -o $@ $(LRESFLAGS)


bin:	$(OBJMAIN)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM).bin $(OBJMAIN) $(LIBS) $(LDLIBS) $(LIBRARIES) $(LWXPATH)/lib/libwx_gtk-2.4.so.0.1.1

lib:	$(SOURCEDLL)
	$(CXX) $(CXXFLAGS) -fPIC -c $(SOURCEDLL)
	@mv *.o $(DLLPATH)
	$(CXX) $(CXXFLAGS) -shared -Wl,-soname,$(PROGRAM).so.1 -o $(PROGRAM).so.1.0 $(SOURCEDLL)
	
#$(PROGRAM).so.1.0.1 $(LIBFLAGS) $(SOURCEDLL)

clean:
	@rm -vf ./*.o
	@rm -vf $(PROGRAM)/*.o
	@rm -vf $(PROGRAM).exe
	@rm -vf $(PROGRAM).bin
	@echo "exe cleared."

allclean:
	@rm -vf ./*.o
	@rm -vf $(DLLPATH)/*.o
	@rm -vf $(PROGRAM)/*.o
	@rm -vf $(PROGRAM).dll
	@rm -vf $(PROGRAM).a
	@rm -vf $(PROGRAM).exe
	@echo "done."
