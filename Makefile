EF=ef
UI=ui
SDL=sdl
CORE=core

CORE_OBJ=$(CORE)/defs.o $(CORE)/prebuildorder.o $(CORE)/anabuildorder.o $(CORE)/configuration.o $(CORE)/goal.o  $(CORE)/goalentry.o $(CORE)/harvest.o  $(CORE)/location.o  $(CORE)/basicmap.o  $(CORE)/buildorder.o  $(CORE)/database.o  $(CORE)/soup.o $(CORE)/building.o $(CORE)/start.o $(CORE)/unit.o $(CORE)/startcondition.o $(CORE)/statistics.o $(CORE)/program.o 

EF_OBJ=$(EF)/guimain.o $(EF)/mainwindow.o $(EF)/game.o $(EF)/main.o $(EF)/player.o $(EF)/bodiagram.o $(EF)/bograph.o $(EF)/bgwindow.o $(EF)/bowindow.o $(EF)/forceentry.o $(EF)/force.o $(EF)/info.o $(EF)/message.o $(EF)/score.o $(EF)/tutorial.o $(EF)/racemenu.o $(EF)/menuentry.o $(EF)/menu.o $(EF)/setwindow.o $(EF)/progressbar.o $(EF)/unitmenu.o $(EF)/goalmenu.o $(EF)/mapmenu.o $(EF)/forcemenu.o $(EF)/languagemenu.o $(EF)/resolutionmenu.o $(EF)/thememenu.o $(EF)/locmenu.o $(EF)/boentry.o $(EF)/configuration.o $(EF)/endrundialog.o stl/misc.o $(EF)/playerentry.o $(EF)/techtree.o

UI_OBJ=$(UI)/object.o $(UI)/window.o $(UI)/button.o $(UI)/radio.o $(UI)/statictext.o $(UI)/theme.o $(UI)/group.o $(UI)/editfield.o $(UI)/checkbutton.o $(UI)/tooltip.o $(UI)/numberfield.o $(UI)/scrollbar.o $(UI)/configuration.o

SDL_OBJ=$(SDL)/color.o $(SDL)/font.o $(SDL)/pen.o $(SDL)/surface.o $(SDL)/brush.o $(SDL)/dc.o $(SDL)/framerate.o $(SDL)/draw.o $(SDL)/size.o $(SDL)/rect.o $(SDL)/point.o

CPPFLAGS =
FLAGS = -pipe -g -D_SCC_DEBUG -ansi -pedantic-errors -Wall -W -Wmissing-braces -Wredundant-decls -Wundef -Wunused -Wconversion -Woverloaded-virtual -Wmissing-noreturn -Winline -Wdisabled-optimization -Wsign-promo -Wwrite-strings -Wlong-long -Woverloaded-virtual -Wfloat-equal -Wreturn-type -Wparentheses -Wchar-subscripts -Wimplicit  -Wtrigraphs  -Wendif-labels -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wshadow
#FLAGS = -O3 -pipe -ftracer #-fmerge-all-constants -ffast-math #-march=pentium4 --param max-crossjump-edges=2000 --param max-gcse-memory=209715200 --param max-gcse-passes=40 --param max-pending-list-length=1000 --param max-delay-slot-insn-search=2000 --param max-delay-slot-live-search=4000
#FLAGS = -g -D_SCC_DEBUG -ansi -pedantic-errors -Wall -W -Wmissing-braces -Wredundant-decls -Wundef -Wunused -Wconversion -Woverloaded-virtual -Wmissing-noreturn -Winline -Wdisabled-optimization -Wsign-promo -Wwrite-strings -Wlong-long -Woverloaded-virtual -Wfloat-equal -Wreturn-type -Wparentheses -Wchar-subscripts -Wimplicit  -Wtrigraphs  -Wendif-labels -Wpointer-arith #-Wcast-qual -Wcast-align -Wwrite-strings -Wshadow -Weffc++ 
#-Wold-style-cast
#-Wswitch-enum -Wpadded -Waggregate-return -Wunreachable-code -Wswitch -Wswitch-default 

#-fmerge-all-constants --param max-crossjump-edges=2000 --param max-gcse-memory=209715200 --param max-gcse-passes=40 --param max-pending-list-length=1000 --param max-delay-slot-insn-search=2000 --param max-delay-slot-live-search=4000
#-ffast-math
#-enable-stack-protector

CXXFLAGS = $(FLAGS) -I/usr/include/SDL
CXX = g++

INCLUDES = -lSDL -lSDL_ttf -I/usr/include/SDL

#/usr/X11R6/lib/libX11.a 
#-Wl,-Bdynamic -lSDL -lSDL_ttf -L/usr/X11R6/lib -lX11 -lXext -I/usr/local/include/SDL
#INCLUDES = -Wl,-ly /data2/SDL/SDL12/src/.libs/libSDL.a `sdl-config --cflags --static-libs` -lSDL_ttf -L/usr/X11R6/lib




.SUFFIXES: .o .cpp
release: $(EF_OBJ) $(UI_OBJ) $(CORE_OBJ)
	$(CXX) $(CXXFLAGS) -o $(EF)b $(INCLUDES) $(EF_OBJ) $(UI_OBJ) $(CORE_OBJ) sdl.a

sdl: sdl.a($(SDL_OBJ))

clean:
	@rm -vf ./*.o
	@rm -vf $(EF)/*.o
	@rm -vf $(EF)b
coreclean:
	@rm -vf $(CORE)/*.o
	@rm -vf $(CORE).a
uiclean:
	@rm -vf $(UI)/*.o
	@rm -vf $(UI).a
sdlclean:
	@rm -vf $(SDL)/*.o
	@rm -vf $(SDL).a
allclean:
	@rm -vf ./*.o
	
	@rm -vf $(CORE)/*.o
	@rm -vf $(EF)/*.o
	@rm -vf $(UI)/*.o
	@rm -vf stl/*.o
	
#	@rm -vf $(CORE).a
#	@rm -vf $(UI).a
#	@rm -vf $(SDL).a
	
#	@rm -vf $(EF).dll
	#@rm -vf $(EF)b
	#@rm -vf $(EF).exe
	
#	@rm -vf exe/*.o
#	@rm -vf exe/*.a
	
	@rm -vf gmon.out
	@echo "done."
