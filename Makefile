WXPATH=/home/clawg/temp2/wxGTK-2.4.2/
PROGRAM=scc2
CXX=g++
DLLPATH=$(PROGRAM)dll
OBJDLL=$(DLLPATH)/anarace.o $(DLLPATH)/debug.o $(DLLPATH)/ga.o $(DLLPATH)/goal.o  $(DLLPATH)/harvest.o  $(DLLPATH)/location.o  $(DLLPATH)/map.o  $(DLLPATH)/player.o  $(DLLPATH)/prerace.o  $(DLLPATH)/race.o  $(DLLPATH)/settings.o  $(DLLPATH)/soup.o
SOURCEDLL=$(DLLPATH)/anarace.cpp $(DLLPATH)/debug.cpp $(DLLPATH)/ga.cpp $(DLLPATH)/goal.cpp  $(DLLPATH)/harvest.cpp  $(DLLPATH)/location.cpp  $(DLLPATH)/map.cpp  $(DLLPATH)/player.cpp  $(DLLPATH)/prerace.cpp  $(DLLPATH)/race.cpp  $(DLLPATH)/settings.cpp  $(DLLPATH)/soup.cpp
OBJMAIN=$(PROGRAM)/scc2.o
DLLFLAGS=-DBUILD_DLL
LIBS=-L./ $(PROGRAM).so.1.0
RES_PROGRAM_OBJ = $(PROGRAM)_resourcesl.o

EXTRALIBS = -Wl,--version-script,$(WXPATH)/version-script -L/usr/lib -L/usr/X11R6/lib -lgtk -lgdk -rdynamic -lgmodule -lglib -ldl -lXi -lXext -lX11 -lm -lpng -ljpeg -ltiff -lz -ldl -lm

DLIBS=$(WXPATH)/lib/libwx_gtk-2.4.so.0.1.1 $(EXTRALIBS)

WXLIB=libwx_msw-2.4-i386-mingw32msvc.a
WXSHLIB=libwx_msw-2.4-i386-mingw32msvc.dll
WXLIBBASE=wx_msw
SHARED_LD=i386-mingw32msvc-c++ -shared -Wl,--out-implib,lib/libwx_msw-2.4-i386-mingw32msvc.dll.a -o
RESFLAGS=--include-dir $(WXPATH)/include --define __WXGTK__ --define __UNIX__

CPPFLAGS = -I$(WXPATH)/lib/wx/include/gtk-2.4 -I$(WXPATH)/include -I/user/include/gtk-1.2 -I/usr/include/glib-1.2 -I/usr/lib/glib/include -D_REENTRANT -I/usr/X11R6/include -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -DGTK_NO_CHECK_CASTS

CXXFLAGS = $(CPPFLAGS) -O3 -MMD -Wall -g

.SUFFIXES: .o .cpp .rc _resources.o
.rc_resources.o:
	$(RESCOMP) -i $< -o $@ $(LRESFLAGS)

bin:	$(OBJMAIN)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM).bin $(OBJMAIN) $(LIBS) $(LDLIBS) $(LIBRARIES) $(WXPATH)/lib/libwx_gtk-2.4.so.0.1.1

lib:	$(SOURCEDLL)
	$(CXX) $(CXXFLAGS) -fPIC -c $(SOURCEDLL)
	@mv *.o $(DLLPATH)
	$(CXX) $(CXXFLAGS) -shared -Wl,-soname,$(PROGRAM).so.1 -o $(PROGRAM).so.1.0 $(SOURCEDLL)
	
clean:
	@rm -vf ./*.o
	@rm -vf $(PROGRAM)/*.o
	@rm -vf $(PROGRAM).bin

allclean:
	@rm -vf ./*.o
	@rm -vf ./*.d
	@rm -vf $(DLLPATH)/*.d
	@rm -vf $(DLLPATH)/*.o
	@rm -vf $(PROGRAM)/*.d
	@rm -vf $(PROGRAM)/*.o
	@rm -vf $(PROGRAM).dll
	@rm -vf $(PROGRAM).a
	@rm -vf $(PROGRAM).bin
	@rm -vf $(PROGRAM).exe
	@rm -vf $(PROGRAM).so.1.0
	@rm -vf a.out
	@rm -vf gmon.out
	@echo "done."
