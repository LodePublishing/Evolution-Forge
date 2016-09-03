#include "statistics.hpp"
#include "configuration.hpp"
#include <sstream>

StatisticsWindow::StatisticsWindow(UI_Object* stat_parent, const unsigned int stat_window_number):
	UI_Window(stat_parent, STATISTICS_WINDOW_TITLE_STRING, STATISTICS_WINDOW, stat_window_number, NOT_SCROLLED),
	ani(5),
	start_time(SDL_GetTicks()),
	anarace(NULL),
	averagecounter(0),
	graph_ani(0),
	wasResetted(false)
{
	resetData();
	Point l1 = Point(getRelativeClientRectPosition()+Point(210,0));
	Size l2 = Size(190, FONT_SIZE+7);
	for(unsigned int i = 0; i < MAX_STAT_ENTRY; i++)
	{
		statEntry[i] = new UI_Button(this, Rect(l1+Point(0,i*14), l2), (eString)(MINERALS_STAT_STRING+i), (eButton)(MINERALS_STAT_BUTTON+i), NO_TEXT_MODE, STATIC_BUTTON_MODE, DO_NOT_ADJUST, SMALL_NORMAL_BOLD_FONT, NO_AUTO_SIZE);
		statEntry[i]->updateToolTip((eString)(MINERALS_STAT_TOOLTIP_STRING+i));
		statEntry[i]->forcePress();
	}
}

StatisticsWindow::~StatisticsWindow()
{
	for(unsigned int i = 0; i < MAX_STAT_ENTRY; i++)
		delete statEntry[i];
}

void StatisticsWindow::assignAnarace(ANABUILDORDER* stat_anarace)
{
	anarace = stat_anarace;
}

void StatisticsWindow::showGraph(DC* dc, const unsigned int* graph_data, const unsigned int min, const unsigned int max, const Color col, const bool bold) const
{
	unsigned int j,k;
	if(getAbsoluteClientRectPosition().y <0)
		return;
	if(bold)
		dc->SetPen(Pen(col, 2, SOLID_PEN_STYLE));
	else
		dc->SetPen(Pen(col, 1, SOLID_PEN_STYLE));

	Point blub[200];
	j=1;
	k=0;
	for(unsigned int i = 1; i < 199; i++)
	{
		if((graph_data[i] != graph_data[i+1])||(k>9))
		{
			k=0;
		//TODO Hoehe 100 in clientarea.height aendern
			blub[j]=Point(i+getAbsoluteClientRectLeftBound(), getAbsoluteClientRectLowerBound() - 2 - (getClientRectHeight()-4)*(graph_data[i]-min)/(max+1));
			j++;
		} else k++;
	}
	blub[0]=Point(getAbsoluteClientRectPosition() + Point(2, getClientRectHeight() - 3 - (getClientRectHeight()-4)*(graph_data[0]-min)/(max+1)));
	blub[j]=Point(getAbsoluteClientRectPosition() + Point(201, getClientRectHeight() - 3 - (getClientRectHeight()-4)*(graph_data[199]-min)/(max+1)));
	dc->DrawSpline(j+1, blub);
}

void StatisticsWindow::resetData()
{
	averagecounter = 1;
	memset(average, 0, 100 * sizeof(int));

	memset(data, 0, 200 * MAX_STAT_ENTRY * sizeof(int));
	memset(oldData, 0, 20 * MAX_STAT_ENTRY * sizeof(int));
	memset(oldDataCounter, 0, 20 * MAX_STAT_ENTRY * sizeof(int));
	memset(maxdata, 0, MAX_STAT_ENTRY * sizeof(int));

//	for(unsigned int i = 200; i--;)
//		data[GENERATIONS_LEFT_STAT_ENTRY][i] = configuration.getMaxGenerations();

	wasResetted=true;
}

void StatisticsWindow::draw(DC* dc) const
{
	if(!isShown())
		return;
	UI_Window::draw(dc);

	if(!checkForNeedRedraw())
		return;

	dc->SetBrush(*theme.lookUpBrush( WINDOW_BACKGROUND_BRUSH ));
	dc->SetPen(*theme.lookUpPen( INNER_BORDER_PEN ));
	dc->DrawRectangle(getAbsoluteClientRectPosition(), Size(202, getClientRectHeight()));

	for(unsigned int i = 0; i < MAX_STAT_ENTRY; i++)
		if((!statEntry[i]->isCurrentlyHighlighted())&&(statEntry[i]->isCurrentlyActivated()))
			showGraph(dc, data[i], 0, maxdata[i], dc->mixColor(theme.lookUpColor((eColor)(MINERALS_TEXT_COLOR+i)), theme.lookUpColor(BRIGHT_TEXT_COLOR), statEntry[i]->getGradient()));
	for(unsigned int i = 0; i < MAX_STAT_ENTRY; i++)
		if((statEntry[i]->isCurrentlyHighlighted())&&(statEntry[i]->isCurrentlyActivated()))
			showGraph(dc, data[i], 0, maxdata[i], dc->mixColor(theme.lookUpColor((eColor)(MINERALS_TEXT_COLOR+i)), theme.lookUpColor(BRIGHT_TEXT_COLOR), statEntry[i]->getGradient()), true);

	for(unsigned int i = 0; i<=AVERAGE_BO_LENGTH_STAT_ENTRY;i++)
		if(statEntry[i]->isCurrentlyActivated())
			for(unsigned int k = 0; k < 20; k++)
				if(oldDataCounter[i][k]>0)
				{
					dc->SetTextForeground(dc->darkenColor( theme.lookUpColor((eColor)(MINERALS_TEXT_COLOR+i)), 80-oldDataCounter[i][k]*4));
					std::ostringstream os;
					if(oldData[i][k]>0) 
						os << "+";
					else if(oldData[i]<0)
						os << "-";
					if(i == TIME_STAT_ENTRY)
					{
//						if(oldData[i][k]<0) ??
//							os << formatTime(-oldData[i][k]);
//						else 
						os << formatTime(oldData[i][k]);
						dc->DrawText(os.str(), statEntry[i]->getAbsolutePosition() + Size(statEntry[i]->getTextWidth() + oldDataCounter[i][k]-5, 4));
					}
				}
}

// TODO ueberpruefen ob is shown
void StatisticsWindow::process()
{
	if(!isShown())
		return;
	
	UI_Window::process();
	
	for(unsigned int i=0;i<=AVERAGE_BO_LENGTH_STAT_ENTRY;i++)
		for(unsigned int k=0;k<20;k++)
			if(oldDataCounter[i][k]>0)
			{
				oldDataCounter[i][k]+=2;
				if(oldDataCounter[i][k]>20)
				{
					oldDataCounter[i][k]=0;
					oldData[i][k]=0;
				}
			}
	for(unsigned int i = 0; i < MAX_STAT_ENTRY; i++)
		if((statEntry[i]->isCurrentlyHighlighted())&&(statEntry[i]->isCurrentlyActivated()))
		{
			setNeedRedrawNotMoved();
			break;
		}

//	if((!wasResetted)&&(!anarace->isOptimizing()))
//		return; TODO

	setNeedRedrawNotMoved();
	
	wasResetted = false;

	long int difference = SDL_GetTicks() - start_time;
	start_time += difference;

	int av=0;
	for(unsigned int i=0;i<=AVERAGE_BO_LENGTH_STAT_ENTRY;i++)
		for(unsigned int j=0;j<20;j++)
			for(unsigned int k=0;k<j;k++)
			{
				if(oldDataCounter[i][j]>oldDataCounter[i][k])
				{
					std::swap(oldDataCounter[i][j], oldDataCounter[i][k]);
					std::swap(oldData[i][j], oldData[i][k]);
				}
			}
			
	data[MINERALS_STAT_ENTRY][199] = anarace->getHarvestedMinerals()/100;
	data[GAS_STAT_ENTRY][199] = anarace->getHarvestedGas()/100;
	data[TIME_STAT_ENTRY][199] = anarace->getRealTimer();
	data[FORCE_STAT_ENTRY][199] = anarace->getUnitsTotal();
	data[AVERAGE_BO_LENGTH_STAT_ENTRY][199] = anarace->getLength();
	data[FITNESS_AVERAGE_STAT_ENTRY][199] = anarace->fitnessAverage;
	data[FITNESS_VARIANCE_STAT_ENTRY][199] = (unsigned int)sqrt((double)anarace->fitnessVariance);
//	data[GENERATIONS_LEFT_STAT_ENTRY][199] = coreConfiguration.getMaxGenerations() - anarace->getUnchangedGenerations();

	for(unsigned int i=0;i<AVERAGE_BO_LENGTH_STAT_ENTRY;i++)
		if(data[i][199]!=data[i][198])
		{
			for(unsigned int l=0;l<20;l++)
				if(oldDataCounter[i][l]==0)
				{
					oldDataCounter[i][l]=1;
					oldData[i][l]=((signed int)(data[i][199])-(signed int)(data[i][198]));
					data[i][198]=data[i][199];
					break;
				}
		}


	ani++;
	if(ani>5)
	{
		if(averagecounter<100)
			averagecounter++;
		for(unsigned int i=averagecounter-1;i--;)
			average[i+1]=average[i];
		average[0]=difference;
	
		for(unsigned int i=0;i<averagecounter;i++)
			av+=average[i];
		av/=averagecounter;
		ani=0;
	
		for(unsigned int i=0;i<=AVERAGE_BO_LENGTH_STAT_ENTRY;i++)
			if(data[i][199]!=data[i][198])
			{
				for(unsigned int l=0;l<20;l++)
					if(oldDataCounter[i][l]==0)
					{
						oldDataCounter[i][l]=1;
						oldData[i][l]=((signed int)(data[i][199])-(signed int)(data[i][198]));
						break;
					}
			}
		
		for(unsigned int i=0;i<MAX_STAT_ENTRY;i++)
			for(unsigned int j=0;j<199;j++)
				data[i][j] = data[i][j+1];
		if(av==0)
			av=1;
		data[FPS_STAT_ENTRY][199] = efConfiguration.getCurrentFramerate();//1000/av; // ???? TODO
	}
	

	for(unsigned int i=0;i<MAX_STAT_ENTRY;i++)
		for(unsigned int j=0;j<200;j++)
		{
			if(data[i][j]>maxdata[i])
				maxdata[i]=data[i][j];
		}
	
	statEntry[MINERALS_STAT_ENTRY]->updateText(theme.lookUpFormattedString(MINERALS_STAT_STRING, data[MINERALS_STAT_ENTRY][199]));
	statEntry[GAS_STAT_ENTRY]->updateText(theme.lookUpFormattedString(GAS_STAT_STRING, data[GAS_STAT_ENTRY][199]));
	statEntry[TIME_STAT_ENTRY]->updateText(theme.lookUpFormattedString(TIME_STAT_STRING, formatTime(data[TIME_STAT_ENTRY][199])));
	statEntry[FORCE_STAT_ENTRY]->updateText(theme.lookUpFormattedString(FORCE_STAT_STRING, data[FORCE_STAT_ENTRY][199]));
	statEntry[AVERAGE_BO_LENGTH_STAT_ENTRY]->updateText(theme.lookUpFormattedString(AVERAGE_BO_LENGTH_STAT_STRING, data[AVERAGE_BO_LENGTH_STAT_ENTRY][199]));
	statEntry[FITNESS_AVERAGE_STAT_ENTRY]->updateText(theme.lookUpFormattedString(FITNESS_AVERAGE_STAT_STRING, data[FITNESS_AVERAGE_STAT_ENTRY][199], anarace->getMaxpFitness()));
	statEntry[FITNESS_VARIANCE_STAT_ENTRY]->updateText(theme.lookUpFormattedString(FITNESS_VARIANCE_STAT_STRING, data[FITNESS_VARIANCE_STAT_ENTRY][199]));
//	statEntry[GENERATIONS_LEFT_STAT_ENTRY]->updateText(theme.lookUpFormattedString(GENERATIONS_LEFT_STAT_STRING, data[GENERATIONS_LEFT_STAT_ENTRY][199], anarace->getTotalGeneration()));

	std::ostringstream os;
	os << data[FPS_STAT_ENTRY][199] << " fps ";/* [" << UI_Object::rectnumber << " updates]";*/
//	if(anarace->isOptimizing())
		os << "[" << (int) 
					(float)(100*data[FPS_STAT_ENTRY][199]*(MAX_PROGRAMS/*((*anarace->getMap())->getMaxPlayer()-1)*/) / 
					((float)((1+efConfiguration.getCurrentFramesPerGeneration())))) << " BOps]";
//		TODO alles in eine FPS Klasse oder so
	statEntry[FPS_STAT_ENTRY]->updateText(os.str());
}

