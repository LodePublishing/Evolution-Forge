PROGRAM=ef
UI=ui
DEPENDFILE=.depend
CXX=g++
COREPATH=core
OBJDLL=$(COREPATH)/anarace.o $(COREPATH)/ga.o $(COREPATH)/goal.o  $(COREPATH)/harvest.o  $(COREPATH)/location.o  $(COREPATH)/basicmap.o  $(COREPATH)/prerace.o  $(COREPATH)/race.o  $(COREPATH)/settings.o  $(COREPATH)/soup.o $(COREPATH)/building.o $(COREPATH)/start.o $(COREPATH)/unit.o $(COREPATH)/startcondition.o

SOURCEDLL=$(OBJDLL:%.o=%.cpp)

OBJMAIN=$(UI)/sdlwrapper.o $(UI)/object.o $(UI)/window.o $(UI)/button.o $(UI)/statictext.o $(UI)/theme.o $(UI)/controls.o $(PROGRAM)/bodiagram.o $(PROGRAM)/bograph.o $(PROGRAM)/bowindow.o $(PROGRAM)/force.o $(PROGRAM)/info.o $(PROGRAM)/message.o $(PROGRAM)/statistics.o $(PROGRAM)/timer.o $(PROGRAM)/mainwindow.o $(PROGRAM)/tutorial.o $(PROGRAM)/corewindow.o $(PROGRAM)/player.o $(PROGRAM)/main.o $(PROGRAM)/unitmenu.o $(PROGRAM)/order.o

SOURCEMAIN=$(OBJMAIN:%.o=%.cpp)

DLLFLAGS=-DBUILD_DLL
LIBS=-L./ $(PROGRAM).so.1.0

CPPFLAGS =
CXXFLAGS = -O2
#-g -Wall -D_SCC_DEBUG

-include $(DEPENDFILE)

.SUFFIXES: .o .cpp
bin:	$(OBJMAIN) $(OBJHEADER)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM)b $(OBJMAIN) $(LIBS) $(LDLIBS) $(LIBRARIES) -lSDL -lSDL_ttf -lSDL_image
		
lib:	$(SOURCEDLL) $(SOURCEHEADER)
	$(CXX) $(CXXFLAGS) -fPIC -c $(SOURCEDLL)
	@mv *.o $(COREPATH)
	$(CXX) $(CXXFLAGS) -shared -Wl,-soname,$(PROGRAM).so.1 -o $(PROGRAM).so.1.0 $(SOURCEDLL)

dep: $(SOURCEDLL) $(SOURCEMAIN)
	$(CXX) -MM $(SOURCEDLL) $(SOURCEMAIN) > $(DEPENDFILE)
	
clean:
	@rm -vf ./*.o
	@rm -vf $(PROGRAM)/*.o
	@rm -vf $(PROGRAM)b
	@rm -vf $(UI)/*.o
libclean:
	@rm -vf $(COREPATH)/*.o
	@rm -vf $(COREPATH)/*.d
	@rm $(PROGRAM).so.1.0

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
