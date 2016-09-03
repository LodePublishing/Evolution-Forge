WXPATH=/home/clawg/temp/wxX/
PROGRAM=scc2
CXX=g++
DLLPATH=$(PROGRAM)dll
OBJDLL=$(DLLPATH)/anarace.o $(DLLPATH)/debug.o $(DLLPATH)/ga.o $(DLLPATH)/goal.o  $(DLLPATH)/harvest.o  $(DLLPATH)/location.o  $(DLLPATH)/map.o  $(DLLPATH)/player.o  $(DLLPATH)/prerace.o  $(DLLPATH)/race.o  $(DLLPATH)/settings.o  $(DLLPATH)/soup.o $(DLLPATH)/building.o $(DLLPATH)/blist.o
SOURCEDLL=$(DLLPATH)/anarace.cpp $(DLLPATH)/debug.cpp $(DLLPATH)/ga.cpp $(DLLPATH)/goal.cpp  $(DLLPATH)/harvest.cpp  $(DLLPATH)/location.cpp  $(DLLPATH)/map.cpp  $(DLLPATH)/player.cpp  $(DLLPATH)/prerace.cpp  $(DLLPATH)/race.cpp  $(DLLPATH)/settings.cpp  $(DLLPATH)/soup.cpp $(DLLPATH)/building.cpp $(DLLPATH)/blist.cpp

OBJMAIN=$(PROGRAM)/scc2.o $(PROGRAM)/graphics.o $(PROGRAM)/list.o $(PROGRAM)/input.o $(PROGRAM)/bodiagram.o $(PROGRAM)/bograph.o $(PROGRAM)/bowindow.o $(PROGRAM)/force.o $(PROGRAM)/info.o $(PROGRAM)/message.o $(PROGRAM)/player.o $(PROGRAM)/statistics.o $(PROGRAM)/timer.o $(PROGRAM)/util.o
DLLFLAGS=-DBUILD_DLL
LIBS=-L./ $(PROGRAM).so.1.0
RES_PROGRAM_OBJ = $(PROGRAM)_resourcesl.o

EXTRALIBS = -Wl -L/usr/lib -L/usr/X11R6/lib -lX11 
#-lgdk -rdynamic -lgmodule -lglib -ldl -lXi -lXext -lX11 -lm -lpng -ljpeg -lz -ldl -lm

DLIBS=$(WXPATH)/lib/libwx_x11univ-2.4.so.0.1.1 $(EXTRALIBS)
RESFLAGS=--include-dir $(WXPATH)/include --define __WXX11__ --define __UNIX__

CPPFLAGS = -I$(WXPATH)/lib/wx/include/x11univ-2.4 -I$(WXPATH)/include -I/usr/X11R6/include -D__WXUNIVERSAL__ -D__WXX11__ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -g
#-I/usr/include/glib-1.2 -I/usr/lib/glib/include -D_REENTRANT -I/usr/X11R6/include

CXXFLAGS = $(CPPFLAGS) -MMD -Wall

.SUFFIXES: .o .cpp .rc _resources.o
.rc_resources.o:
	$(RESCOMP) -i $< -o $@ $(LRESFLAGS)

bin:	$(OBJMAIN)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM).bin $(OBJMAIN) $(LIBS) $(LDLIBS) $(LIBRARIES) $(WXPATH)/lib/libwx_x11univ-2.4.so.0.1.1 

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
	@rm -vf exe/*.d
	@rm -vf exe/*.o
	@rm -vf exe/*.a
	@rm -vf a.out
	@rm -vf gmon.out
	@echo "done."
