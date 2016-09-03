PROGRAM=ef
UI=ui
COREPATH=../core
DEPENDFILE=.depend
CXX=i386-mingw32msvc-g++
DLLWRAP=i386-mingw32msvc-dllwrap
OBJDLL=$(COREPATH)/anarace.o $(COREPATH)/ga.o $(COREPATH)/goal.o  $(COREPATH)/harvest.o  $(COREPATH)/location.o  $(COREPATH)/basicmap.o  $(COREPATH)/prerace.o  $(COREPATH)/race.o  $(COREPATH)/settings.o  $(COREPATH)/soup.o $(COREPATH)/building.o $(COREPATH)/start.o $(COREPATH)/unit.o $(COREPATH)/blist.o $(COREPATH)/startcondition.o

SOURCEDLL=$(OBJDLL:%.o=%.cpp)

OBJMAIN=../$(UI)/sdlwrapper.o ../$(UI)/object.o ../$(UI)/window.o ../$(UI)/button.o ../$(UI)/statictext.o ../$(UI)/theme.o ../$(UI)/controls.o ../$(PROGRAM)/bodiagram.o ../$(PROGRAM)/bograph.o ../$(PROGRAM)/bowindow.o ../$(PROGRAM)/force.o ../$(PROGRAM)/info.o ../$(PROGRAM)/message.o ../$(PROGRAM)/statistics.o ../$(PROGRAM)/timer.o ../$(PROGRAM)/mainwindow.o ../$(PROGRAM)/tutorial.o ../$(PROGRAM)/corewindow.o ../$(PROGRAM)/player.o ../$(PROGRAM)/main.o

SOURCEMAIN=$(OBJMAIN:%.o=%.cpp)

DLLFLAGS=-DBUILD_DLL
LIBS=-L./ ../$(PROGRAM).dll
RESFLAGS=--include-dir $(WXPATH)/include --define __WIN32__ --define __WIN95__ --define __GNUWIN32__
PICFLAGS = -UWXUSINGDLL -DWXMAKINGDLL=1 -D_DLL=1 -D_WINDLL=1


CPPFLAGS =
CXXFLAGS = -g -Wall -D_SCC_DEBUG
CFLAGS = -UWXUSINGDLL -DWXMAKINGDLL=1 -D_DLL=1 -D_WINDLL=1

-include $(DEPENDFILE)

.SUFFIXES: .o .cpp
exe:	$(OBJMAIN) $(OBJHEADER)
	$(CXX) $(CXXFLAGS) -o ../$(PROGRAM).exe $(OBJMAIN) $(LIBS) $(LDLIBS) $(LIBRARIES) -lSDL -lSDL_ttf -lSDL_image -DIMPORT_DLL
		
dll:	$(SOURCEDLL) $(SOURCEHEADER)
	$(CXX) $(CXXFLAGS) -c $(SOURCEDLL)
	@mv *.o $(COREPATH)
	$(DLLWRAP) --output-lib=$(PROGRAM).a --dllname=../$(PROGRAM).dll --driver-name=$(CXX) $(OBJDLL)
	
	

dep: $(SOURCEDLL) $(SOURCEMAIN)
	$(CXX) -MM $(SOURCEDLL) $(SOURCEMAIN) > $(DEPENDFILE)
	
clean:
	@rm -vf ./*.o
	@rm -vf $(PROGRAM)/*.o
	@rm -vf $(PROGRAM).bin

allclean:
	@rm -vf ./*.o
	@rm -vf ./*.d
	@rm -vf $(COREPATH)/*.d
	@rm -vf $(COREPATH)/*.o
	@rm -vf $(PROGRAM)/*.d
	@rm -vf $(PROGRAM)/*.o
	@rm -vf $(UI)/*.d
	@rm -vf $(UI)/*.o
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
