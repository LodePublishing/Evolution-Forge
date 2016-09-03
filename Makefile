PROJECTNAME=scc2
CXX=i386-mingw32msvc-g++
DLLWRAP=i386-mingw32msvc-dllwrap
DLLPATH=$(PROJECTNAME)dll
MAINPATH=$(PROJECTNAME)
OBJDLL=$(DLLPATH)/anarace.o $(DLLPATH)/debug.o $(DLLPATH)/ga.o $(DLLPATH)/goal.o  $(DLLPATH)/harvest.o  $(DLLPATH)/location.o  $(DLLPATH)/map.o  $(DLLPATH)/player.o  $(DLLPATH)/prerace.o  $(DLLPATH)/race.o  $(DLLPATH)/settings.o  $(DLLPATH)/soup.o
SOURCEDLL=$(DLLPATH)/anarace.cpp $(DLLPATH)/debug.cpp $(DLLPATH)/ga.cpp $(DLLPATH)/goal.cpp  $(DLLPATH)/harvest.cpp  $(DLLPATH)/location.cpp  $(DLLPATH)/map.cpp  $(DLLPATH)/player.cpp  $(DLLPATH)/prerace.cpp  $(DLLPATH)/race.cpp  $(DLLPATH)/settings.cpp  $(DLLPATH)/soup.cpp
OBJMAIN=$(MAINPATH)/io.o $(MAINPATH)/scc2.o
DLLFLAGS=-DBUILD_DLL
CXXFLAGS= -O4 -Wall -march=pentium4 -mcpu=pentium4 -ffast-math -pipe -funroll-loops -fforce-mem -malign-double -fforce-addr -fomit-frame-pointer
LIBS=-L./ -l$(PROJECTNAME)

dll:  $(SOURCEDLL)
	$(CXX) $(CXXFLAGS) -c $(DLLFLAGS) $(SOURCEDLL)
	@mv *.o $(DLLPATH)
dllwrap: 
	$(DLLWRAP) --output-lib=$(PROJECTNAME).a --dllname=$(PROJECTNAME).dll --driver-name=$(CXX) $(OBJDLL)
main:  $(OBJMAIN)
	$(CXX) -c $(OBJMAIN)
exe:  $(OBJMAIN)
	$(CXX) $(CXXFLAGS) -o $(PROJECTNAME).exe $(OBJMAIN) $(LIBS)

clean: 
	@rm -f $(DLLPATH)/*.o
	@rm -f $(MAINPATH)/*.o
	@rm -vf $(PROJECTNAME).dll
	@rm -vf $(PROJECTNAME).a
	@rm -vf $(PROJECTNAME).exe
	@echo "done."
