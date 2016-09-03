EF=../ef
UI=../ui
SDL=../sdl
CORE=../core

CORE_OBJ=$(CORE)/configuration.o $(CORE)/goal.o $(CORE)/harvest.o  $(CORE)/location.o  $(CORE)/basicmap.o  $(CORE)/database.o  $(CORE)/soup.o $(CORE)/building.o $(CORE)/start.o $(CORE)/unit.o $(CORE)/startcondition.o $(CORE)/statistics.o $(CORE)/program.o $(CORE)/boentry.o $(CORE)/race.o $(CORE)/starcraft.o $(CORE)/broodwar.o $(CORE)/warcraft3.o $(CORE)/prebuildorder.o $(CORE)/anabuildorder.o $(CORE)/buildorder.o $(CORE)/goalentry.o $(CORE)/game.o 

EF_OBJ= $(EF)/mainwindow.o $(EF)/bodiagram.o $(EF)/bodiapoint.o $(EF)/bgentry.o $(EF)/bgline.o $(EF)/bgwindow.o $(EF)/bowindow.o $(EF)/player.o $(EF)/forceentry.o $(EF)/force.o $(EF)/message.o $(EF)/msgwindow.o $(EF)/score.o $(EF)/game.o $(EF)/racemenu.o $(EF)/bomenu.o $(EF)/unitmenu.o $(EF)/goalmenu.o $(EF)/mapmenu.o $(EF)/languagemenu.o $(EF)/resolutionmenu.o $(EF)/bitdepthmenu.o $(EF)/thememenu.o $(EF)/boentry.o $(EF)/configuration.o ../stl/misc.o $(EF)/playerentry.o $(EF)/techtree.o $(EF)/setwindow.o $(EF)/guimain.o $(EF)/main.o $(EF)/helpwindow.o $(EF)/savebox.o $(EF)/locationmenu.o $(EF)/datawindow.o $(EF)/dataentry.o $(EF)/intro.o $(EF)/debug.o $(EF)/files.o

UI_OBJ=$(UI)/object.o $(UI)/window.o $(UI)/button.o $(UI)/bitmap.o $(UI)/radio.o $(UI)/statictext.o $(UI)/longtext.o $(UI)/theme.o $(UI)/sound.o $(UI)/group.o $(UI)/editfield.o $(UI)/tooltip.o $(UI)/numberfield.o $(UI)/scrollbar.o $(UI)/configuration.o $(UI)/menuentry.o $(UI)/menu.o 

SDL_OBJ=$(SDL)/color.o $(SDL)/font.o $(SDL)/pen.o $(SDL)/surface.o $(SDL)/brush.o $(SDL)/dc.o $(SDL)/framerate.o $(SDL)/draw.o $(SDL)/size.o $(SDL)/rect.o $(SDL)/point.o $(SDL)/rotozoom.o


#CPPFLAGS = -O3 -I/usr/local/include/fmodex_win -I/usr/local/include/SDL -D__WIN32__ 
#CXXFLAGS = -I/usr/local/include/fmodex_win -I/usr/local/include/SDL -g -ftracer -pipe #-fforce-addr -finline-limit=1000 #-march=pentium4 --param max-crossjump-edges=4000 --param max-gcse-memory=409715200 --param max-gcse-passes=100 --param max-pending-list-length=4000 --param max-delay-slot-insn-search=5000 --param max-delay-slot-live-search=8000 -ffast-math #-fno-math-errno -funsafe-math-optimizations -ffinite-math-only -fno-trapping-math  -fforce-addr
CXXFLAGS = -D_SDL_MIXER_SOUND -I/usr/local/include/SDL -DWIN32 -g -ansi -pedantic-errors -Wall -W -pipe -D_SCC_DEBUG -Wmissing-braces -Wredundant-decls -Wundef -Wunused -Wconversion -Woverloaded-virtual -Wmissing-noreturn -Winline -Wdisabled-optimization -Wsign-promo -Wwrite-strings -Wlong-long -Woverloaded-virtual -Wfloat-equal -Wreturn-type -Wparentheses -DNO_STDIO_REDIRECT #-Wchar-subscripts -Wimplicit -Wswitch -Wswitch-default -Wswitch-enum -Wtrigraphs  -Wendif-labels -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wpadded -Wshadow  -Waggregate-return  -Wunreachable-code

#-Wold-style-cast 

CXX=i386-mingw32msvc-g++
#LIBS = core.a ui.a sdl.a
INCLUDES =  -L/usr/local/lib -I/usr/local/include/SDL -lmingw32 -lSDLmain -lSDL -mwindows -lSDL_ttf -lSDL_image -lSDL_mixer

.SUFFIXES: .o .cpp
release: $(EF_OBJ) $(UI_OBJ) $(SDL_OBJ) $(CORE_OBJ)
	$(CXX) $(CXXFLAGS) -o $(EF)b.exe $(EF_OBJ) $(UI_OBJ) $(SDL_OBJ) $(CORE_OBJ) $(INCLUDES)
	
clean:
	@rm -vf ./*.o
	@rm -vf $(EF)/*.o
	@rm -vf $(EF)b.exe
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
