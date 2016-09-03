EF=../ef
UI=../ui
SDL=../sdl
CORE=../core

CORE_OBJ=$(CORE)/defs.o $(CORE)/anarace.o $(CORE)/ga.o $(CORE)/goal.o $(CORE)/goalentry.o $(CORE)/harvest.o  $(CORE)/location.o  $(CORE)/basicmap.o  $(CORE)/prerace.o  $(CORE)/race.o  $(CORE)/settings.o  $(CORE)/soup.o $(CORE)/building.o $(CORE)/start.o $(CORE)/unit.o $(CORE)/startcondition.o

EF_OBJ=$(EF)/bodiagram.o $(EF)/bograph.o $(EF)/bowindow.o $(EF)/forceentry.o $(EF)/force.o $(EF)/info.o $(EF)/message.o $(EF)/statistics.o $(EF)/timer.o $(EF)/mainwindow.o $(EF)/tutorial.o $(EF)/corewindow.o $(EF)/player.o  $(EF)/order.o $(EF)/main.o  $(EF)/racemenu.o $(EF)/menuentry.o $(EF)/menu.o $(EF)/setwindow.o $(EF)/progressbar.o $(EF)/numberfield.o $(EF)/unitmenu.o $(EF)/goalmenu.o $(EF)/forcemenu.o

UI_OBJ=$(UI)/object.o $(UI)/window.o $(UI)/button.o $(UI)/radio.o $(UI)/scrollbar.o $(UI)/statictext.o $(UI)/theme.o 

SDL_OBJ=$(SDL)/color.o $(SDL)/font.o $(SDL)/pen.o $(SDL)/surface.o $(SDL)/brush.o $(SDL)/dc.o $(SDL)/framerate.o $(SDL)/bitmap.o $(SDL)/draw.o $(SDL)/size.o $(SDL)/rect.o $(SDL)/point.o


CPPFLAGS =
CXXFLAGS = -ansi -pedantic-errors -Wall -W -O3 #-g -D_SCC_DEBUG #-Weffc++ #-Wabi -Wmissing-braces -Wredundant-decls -Wundef -Wunused -Wconversion -Woverloaded-virtual -Wmissing-noreturn -Winline -Wdisabled-optimization -Wsign-promo -Wwrite-strings -Wlong-long -Woverloaded-virtual -Wfloat-equal -Wreturn-type -Wparentheses
#-Wchar-subscripts -Wimplicit -Wswitch -Wswitch-default -Wswitch-enum -Wtrigraphs  -Wendif-labels -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wpadded -Wshadow  -Waggregate-return  -Wunreachable-code

#-Wold-style-cast 

CXX=i386-mingw32-g++

LIBS = core.a ui.a sdl.a
INCLUDES = -I/usr/local/include/SDL -I/usr/include -I/usr/local/include -I/usr/local/i386-mingw32/include/SDL 

.SUFFIXES: .o .cpp
bin:	$(EF_OBJ) 
	./ranlib
	$(CXX) $(CXXFLAGS) -o $(EF)b.exe $(EF_OBJ) $(INCLUDES) $(LIBS) /usr/local/i386-mingw32/lib/libSDLmain.a -lmingw32 -lSDLmain -lSDL -mwindows -lSDL_ttf 
	
core: core.a($(CORE_OBJ))
ui: ui.a($(UI_OBJ))
sdl: sdl.a($(SDL_OBJ))

clean:
	@rm -vf ./*.o
	@rm -vf $(EF)/*.o
	@rm -vf $(EF)b.exe
efclean:
	@rm -vf ef.a
	@rm $(EF)/*.o
coreclean:
	@rm -vf core.a
	@rm $(CORE)/*.o
uiclean:
	@rm -vf ui.a
	@rm $(UI)/*.o
sdlclean:
	@rm -vf sdl.a
	@rm $(SDL)/*.o

allclean:
	
	@rm -vf $(CORE)/*.o
	@rm -vf $(EF)/*.o
	@rm -vf $(UI)/*.o
	@rm -vf $(SDL)/*.o

	@rm -vf $(CORE).a
	@rm -vf $(UI).a
	@rm -vf $(SDL).a
	
	@rm -vf $(EF)b.exe
	
	@rm -vf *.o
	@rm -vf *.a
	
	@rm -vf gmon.out
	@echo "done."
