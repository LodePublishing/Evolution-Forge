WXPATH=/home/clawg/temp/wxWindows-2.4.2/
PROGRAM=scc2
CXX=i386-mingw32msvc-g++
DLLWRAP=i386-mingw32msvc-dllwrap
DLLPATH=../$(PROGRAM)dll
OBJDLL=$(DLLPATH)/anarace.o $(DLLPATH)/debug.o $(DLLPATH)/ga.o $(DLLPATH)/goal.o  $(DLLPATH)/harvest.o  $(DLLPATH)/location.o  $(DLLPATH)/map.o  $(DLLPATH)/player.o  $(DLLPATH)/prerace.o  $(DLLPATH)/race.o  $(DLLPATH)/settings.o  $(DLLPATH)/soup.o $(DLLPATH)/building.o 

SOURCEDLL=$(DLLPATH)/anarace.cpp $(DLLPATH)/debug.cpp $(DLLPATH)/ga.cpp $(DLLPATH)/goal.cpp  $(DLLPATH)/harvest.cpp  $(DLLPATH)/location.cpp  $(DLLPATH)/map.cpp  $(DLLPATH)/player.cpp  $(DLLPATH)/prerace.cpp  $(DLLPATH)/race.cpp  $(DLLPATH)/settings.cpp  $(DLLPATH)/soup.cpp $(DLLPATH)/building.cpp
OBJMAIN=../$(PROGRAM)/scc2.o ../$(PROGRAM)/graphics.o
DLLFLAGS=-DBUILD_DLL
RES_PROGRAM_OBJ = ../$(PROGRAM)_resources.o
LIBS=-L./ -L./../ -l$(PROGRAM)


EXTRALIBS =  -Wl,--subsystem,windows -mwindows -mthreads  -Wl,--version-script,$(WXPATH)/version-script -lrpcrt4 -loleaut32 -lole32 -luuid -lwinspool -lwinmm -lshell32 -lcomctl32 -lctl3d32 -ladvapi32 -lwsock32 -l../$(PROGRAM) -l$(DLLPATH)

LDLIBS=$(WXPATH)/lib/libwx_msw-2.4-i386-mingw32msvc.dll $(WEXTRALIBS)

WXLIB=libwx_msw-2.4-i386-mingw32msvc.a
WXSHLIB=libwx_msw-2.4-i386-mingw32msvc.dll
WXLIBBASE=wx_msw
SHARED_LD=i386-mingw32msvc-c++ -shared -Wl,--out-implib,lib/libwx_msw-2.4-i386-mingw32msvc.dll.a -o
RESFLAGS=--include-dir $(WXPATH)/include --define __WIN32__ --define __WIN95__ --define __GNUWIN32__ 

CPPFLAGS = -I$(WXPATH)/lib/wx/include/msw-2.4-i386-mingw32msvc -I$(WXPATH)/include -I$(WXPATH)/src/regex -I$(WXPATH)/src/zlib -I$(WXPATH)/src/png -I$(WXPATH)/src/jpeg -I$(WXPATH)/src/tiff -D_WIN32_IE=0x400 $(EXTRADEFS)  -D__WXMSW__ -mthreads -DWXUSINGDLL=1 -I$(DLLPATH)

CXXFLAGS = $(CPPFLAGS) -fno-pcc-struct-return -MMD -mthreads -Wall -O3
RESCOMP=i386-mingw32msvc-windres
PICFLAGS = -UWXUSINGDLL -DWXMAKINGDLL=1 -D_DLL=1 -D_WINDLL=1

.SUFFIXES: .o .cpp .rc _resources.o
.rc_resources.o:
	$(RESCOMP) -i $< -o $@ $(RESFLAGS)

dll:	$(SOURCEDLL)
	$(CXX) $(CXXFLAGS) -c $(DLLFLAGS) $(SOURCEDLL)
	@mv *.o $(DLLPATH)
	$(DLLWRAP) --output-lib=$(PROGRAM).a --dllname=../$(PROGRAM).dll --driver-name=$(CXX) $(OBJDLL)

exe:	$(OBJMAIN) $(WXPATH)/lib/libwx_msw-2.4-i386-mingw32msvc.dll $(RES_PROGRAM_OBJ)
	$(CXX) $(CXXFLAGS) -o ../$(PROGRAM).exe $(OBJMAIN) $(LIBS) $(LDLIBS) $(LIBRARIES) $(RES_PROGRAM_OBJ)  -DIMPORT_DLL

clean:
	@rm -vf ./*.o
	@rm -vf $(PROGRAM)/*.o
	@rm -vf $(PROGRAM).exe
	@echo "exe cleared."

allclean:
	@rm -vf ./*.o
	@rm -vf $(DLLPATH)/*.o
	@rm -vf $(PROGRAM)/*.o
	@rm -vf $(PROGRAM).dll
	@rm -vf $(PROGRAM).a
	@rm -vf $(PROGRAM).exe
	@echo "done."
