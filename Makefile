PROGRAM=scc2
DEPENDFILE=.depend
CXX=g++
DLLPATH=$(PROGRAM)dll
OBJDLL=$(DLLPATH)/anarace.o $(DLLPATH)/debug.o $(DLLPATH)/ga.o $(DLLPATH)/goal.o  $(DLLPATH)/harvest.o  $(DLLPATH)/location.o  $(DLLPATH)/map.o  $(DLLPATH)/player.o  $(DLLPATH)/prerace.o  $(DLLPATH)/race.o  $(DLLPATH)/settings.o  $(DLLPATH)/soup.o $(DLLPATH)/building.o $(DLLPATH)/default.o $(DLLPATH)/start.o $(DLLPATH)/units.o $(DLLPATH)/blist.o

SOURCEDLL=$(OBJDLL:%.o=%.cpp)
#$(DLLPATH)/anarace.cpp $(DLLPATH)/debug.cpp $(DLLPATH)/ga.cpp $(DLLPATH)/goal.cpp  $(DLLPATH)/harvest.cpp  $(DLLPATH)/location.cpp  $(DLLPATH)/map.cpp  $(DLLPATH)/player.cpp  $(DLLPATH)/prerace.cpp  $(DLLPATH)/race.cpp  $(DLLPATH)/settings.cpp  $(DLLPATH)/soup.cpp $(DLLPATH)/building.cpp $(DLLPATH)/default.cpp $(DLLPATH)/start.cpp $(DLLPATH)/units.cpp $(DLLPATH)/blist.cpp

OBJMAIN=$(PROGRAM)/scc2.o $(PROGRAM)/defs.o $(PROGRAM)/UI_Theme.o $(PROGRAM)/UI_Window.o $(PROGRAM)/UI_Button.o $(PROGRAM)/UI_Object.o $(PROGRAM)/controls.o $(PROGRAM)/bodiagram.o $(PROGRAM)/bograph.o $(PROGRAM)/bowindow.o $(PROGRAM)/force.o $(PROGRAM)/info.o $(PROGRAM)/message.o $(PROGRAM)/player.o $(PROGRAM)/statistics.o $(PROGRAM)/timer.o $(PROGRAM)/util.o $(PROGRAM)/UI_StaticText.o $(PROGRAM)/mainwindow.o $(PROGRAM)/tutorial.o $(PROGRAM)/corewindow.o

SOURCEMAIN=$(OBJMAIN:%.o=%.cpp)

DLLFLAGS=-DBUILD_DLL
LIBS=-L./ $(PROGRAM).so.1.0

CPPFLAGS =
CXXFLAGS = -g -Wall -D_SCC_DEBUG

-include $(DEPENDFILE)

.SUFFIXES: .o .cpp .rc _resources.o
.rc_resources.o:
	$(RESCOMP) -i $< -o $@ $(LRESFLAGS)

bin:	$(OBJMAIN) $(OBJHEADER)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM).bin $(OBJMAIN) $(LIBS) $(LDLIBS) $(LIBRARIES) -lSDL -lSDL_ttf -lSDL_image
		
lib:	$(SOURCEDLL) $(SOURCEHEADER)
	$(CXX) $(CXXFLAGS) -fPIC -c $(SOURCEDLL)
	@mv *.o $(DLLPATH)
	$(CXX) $(CXXFLAGS) -shared -Wl,-soname,$(PROGRAM).so.1 -o $(PROGRAM).so.1.0 $(SOURCEDLL)

dep: $(SOURCEDLL) $(SOURCEMAIN)
	$(CXX) -MM $(SOURCEDLL) $(SOURCEMAIN) > $(DEPENDFILE)
	
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
