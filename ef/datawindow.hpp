#ifndef _GUI_DATABASE_WINDOW_HPP
#define _GUI_DATABASE_WINDOW_HPP

#include "../ui/window.hpp"
#include "dataentry.hpp"
#include "racemenu.hpp"
#include "game.hpp"

class DataBaseWindow : public UI_Window
{
	public:
		DataBaseWindow(UI_Object* data_window_parent);
		~DataBaseWindow();
		
		void resetData();
		void process();
		void draw(DC* dc) const;

		void reloadOriginalSize();

		// goal liste (loeschbar)
		// liste (aehnlich bowindow) von build-order-namen und Zeit (geordnet nach Zeit) (auch loeschbar)
		// wenn da eins angeklickt wird dann erscheint die build order in einem weiteren Fenster (split-screen)
	
	private:
//		void saveBoxDone(const std::string file_name);
//		void saveBoxCanceled();
		
		void activateGame();
		void mouseHasLeft();
		void updateList();
		void updateItemPositions();
	
		std::list<DataBaseEntry*> dataList;
		UI_Window* dataListWindow;
		UI_Button* raceMenuButton;
		RaceMenu* raceMenu;
		eRace currentRace;
 		Game* game;
		bool raceMenuOpenedExternally;
		unsigned int lastBoLoaded;
		const GOAL_ENTRY* lastGoalLoaded;
//		UI_Button* executeButton; // apply changes to the disk
//		UI_Button* exportDataBase; // write everything in one file
//		UI_Button* importDataBase; // import such a file and add it to the database without overwriting things ~~
//		
//		UI_Button* deleteButton; // delete all marked items
//		UI_Button* exportBoAsHTML;
//		UI_Button* exportBoAsDingens;
//		UI_Button* exportBoAsPicture;
//		UI_StaticText* ein paar Erklaerungen, ueberschriften

		
// jeder goal eintrag kann geoeffnet oder geschlossen werden (wie [+] [-] bei windows guis), am besten mit Pfeil > und Pfeil v
// auf der rechten Seite erscheint dann ein 'game' mit der aktuellen build order, sobald ein goal oder bo MARKIERT wurde
// wenn goals markiert werden => besondere situation, bowindow soll dann nur force window darstellen (bzw. den Rest auch)
// => erst mal keine Eingabemoeglichkeit auf der rechten Seite? mmmh... ne, ganz normal...
// 

// => DataEntry: bool isGoal, UI_Button* openclose, UI_Button* delete, UI_Button* this (text button bestehend aus goal/bonamen und Zeit falls kein goal bzw. (<ZAHL>) Anzahl der im Ordner gespeicherten bos), UI_Button* exportAsPicture, UI_Button* exportAsHTML, UI_Button* exportAsBoDingens...
		
/*

		UI_Button* sortBOsByGoal;

		UI_Button* importDatabase; // => saveBox (ersetzen?)
		UI_Button* exportDatabase; // => saveBox

		bool saveBoxImport;
		bool saveBoxExport;

		
// => aktualisierungsfunktion (sobald auf database umgeschaltet wird oder wenn geloescht wird)
		std::list<std::pair<GOAL_ENTRY*, std::list<BUILD_ORDER*> > > boList;*/
		
};

#endif

