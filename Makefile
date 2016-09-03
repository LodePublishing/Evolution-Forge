EF=ef
UI=ui
SDL=sdl
CORE=core

CORE_OBJ=$(CORE)/starcraft.o $(CORE)/prebuildorder.o $(CORE)/anabuildorder.o $(CORE)/configuration.o $(CORE)/goal.o  $(CORE)/goalentry.o $(CORE)/harvest.o  $(CORE)/location.o  $(CORE)/basicmap.o  $(CORE)/buildorder.o  $(CORE)/database.o  $(CORE)/soup.o $(CORE)/building.o $(CORE)/start.o $(CORE)/unit.o $(CORE)/startcondition.o $(CORE)/statistics.o $(CORE)/program.o $(CORE)/boentry.o

EF_OBJ= $(EF)/mainwindow.o $(EF)/bodiagram.o $(EF)/bgentry.o $(EF)/bgline.o $(EF)/bgwindow.o $(EF)/bowindow.o $(EF)/player.o $(EF)/forceentry.o $(EF)/force.o $(EF)/message.o $(EF)/msgwindow.o $(EF)/score.o $(EF)/game.o $(EF)/racemenu.o $(EF)/bomenu.o $(EF)/unitmenu.o $(EF)/goalmenu.o $(EF)/mapmenu.o $(EF)/forcemenu.o $(EF)/languagemenu.o $(EF)/resolutionmenu.o $(EF)/bitdepthmenu.o $(EF)/thememenu.o $(EF)/boentry.o $(EF)/configuration.o stl/misc.o $(EF)/playerentry.o $(EF)/techtree.o $(EF)/setwindow.o $(EF)/guimain.o $(EF)/main.o $(EF)/helpwindow.o $(EF)/savebox.o $(EF)/locationmenu.o $(EF)/mapwindow.o $(EF)/datawindow.o $(EF)/intro.o

UI_OBJ=$(UI)/object.o $(UI)/window.o $(UI)/button.o $(UI)/bitmap.o $(UI)/radio.o $(UI)/statictext.o $(UI)/longtext.o $(UI)/theme.o $(UI)/group.o $(UI)/editfield.o $(UI)/checkbutton.o $(UI)/tooltip.o $(UI)/numberfield.o $(UI)/scrollbar.o $(UI)/configuration.o $(UI)/menuentry.o $(UI)/menu.o 

SDL_OBJ=$(SDL)/color.o $(SDL)/font.o $(SDL)/pen.o $(SDL)/surface.o $(SDL)/brush.o $(SDL)/dc.o $(SDL)/framerate.o $(SDL)/draw.o $(SDL)/size.o $(SDL)/rect.o $(SDL)/point.o $(SDL)/sound.o

CPPFLAGS =
#FLAGS = -g -pipe -ggdb -D_SCC_DEBUG -ansi -pedantic-errors -Wall -W -Wmissing-braces -Wredundant-decls -Wundef -Wunused -Wconversion -Woverloaded-virtual -Wmissing-noreturn -Wdisabled-optimization -Wsign-promo -Wwrite-strings -Wlong-long -Woverloaded-virtual -Wfloat-equal -Wreturn-type -Wparentheses -Wchar-subscripts -Wimplicit  -Wtrigraphs  -Wendif-labels -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wshadow
FLAGS = -O3 -pg -pipe -ftracer -fmerge-all-constants -ffast-math -march=pentium4 --param max-crossjump-edges=2000 --param max-gcse-memory=209715200 --param max-gcse-passes=40 --param max-pending-list-length=1000 --param max-delay-slot-insn-search=2000 --param max-delay-slot-live-search=4000
#FLAGS = -g -D_SCC_DEBUG -ansi -pedantic-errors -Wall -W -Wmissing-braces -Wredundant-decls -Wundef -Wunused -Wconversion -Woverloaded-virtual -Wmissing-noreturn -Winline -Wdisabled-optimization -Wsign-promo -Wwrite-strings -Wlong-long -Woverloaded-virtual -Wfloat-equal -Wreturn-type -Wparentheses -Wchar-subscripts -Wimplicit  -Wtrigraphs  -Wendif-labels -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wshadow #-Weffc++ 
#-Wold-style-cast
#-Wswitch-enum -Wpadded -Waggregate-return -Wunreachable-code -Wswitch -Wswitch-default 

#-fmerge-all-constants --param max-crossjump-edges=2000 --param max-gcse-memory=209715200 --param max-gcse-passes=40 --param max-pending-list-length=1000 --param max-delay-slot-insn-search=2000 --param max-delay-slot-live-search=4000
#-ffast-math
#-enable-stack-protector

CXXFLAGS = $(FLAGS) -I/usr/include/SDL
CXX = g++

INCLUDES = -lSDL -lSDL_ttf -lSDL_image -I/usr/include/SDL

.SUFFIXES: .o .cpp
release: $(EF_OBJ) $(UI_OBJ)
	$(CXX) $(CXXFLAGS) -o $(EF)b $(INCLUDES) $(EF_OBJ) $(UI_OBJ) core.a sdl.a

sdl: sdl.a($(SDL_OBJ))
core: core.a($(CORE_OBJ))

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
