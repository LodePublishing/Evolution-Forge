#include "corewindow.hpp"

CoreWindow::CoreWindow(UI_Window* parentWindow):UI_Window(parentWindow, CORE_WINDOW_TITLE_STRING, THE_CORE_WINDOW, NOT_SCROLLED)		
{
};

CoreWindow::~CoreWindow()
{
};

void CoreWindow::process()
{
	if(!isShown()) return;
	UI_Window::process();
};


void CoreWindow::draw(DC* dc) const
{
    if(!isShown()) return;
	UI_Window::draw(dc);
};

/*	if(theCore->isShown())
	{
		if(theCore->isPressed(0))
		{ 
			int old=ga->getMutationFactor();
			if(ga->getMutationFactor()<10) 
				ga->setMutationFactor(ga->getMutationFactor()+1);
			else if(ga->getMutationFactor()<30)
				ga->setMutationFactor(ga->getMutationFactor()+5);
			else if(ga->getMutationFactor()<100)
				ga->setMutationFactor(ga->getMutationFactor()+10);
			else if(ga->getMutationFactor()<200)
				ga->setMutationFactor(ga->getMutationFactor()+20);
			if(old!=ga->getMutationFactor())
				msgWindow->addMessage(string::Format(T("Increased mutation factor to %i%%."),ga->getMutationFactor())));
			else if(old==200)
			{
				msgWindow->addMessage("Mutation factor is already at maximum (200)."));
				msgWindow->addMessage("Take a look in the ""help"" menu (settings / mutation factor)."));
			}
		}
		if(theCore->isPressed(1))
		{
			int old=ga->getMutationFactor();
			if(ga->getMutationFactor()>100)
				ga->setMutationFactor(ga->getMutationFactor()-20);
			else if(ga->getMutationFactor()>30)
				ga->setMutationFactor(ga->getMutationFactor()-10);
			   	else if(ga->getMutationFactor()>10)
				ga->setMutationFactor(ga->getMutationFactor()-5);
			else if(ga->getMutationFactor()>0)
				ga->setMutationFactor(ga->getMutationFactor()-1);
			if(old!=ga->getMutationFactor())
			{
				if(ga->getMutationFactor()==0)
				{
					if(ga->getCrossOver()==0)
			   				msgWindow->addMessage("Deactivated mutations! Evolution will no longer occur."));
					else
						msgWindow->addMessage("Deactivated mutations. From now on evolution will only occur through cross over."));
				}
				else
					msgWindow->addMessage(String::Format(T("Decreased mutation factor to %i%%."),ga->getMutationFactor())));
			} else if(old==0)
				msgWindow->addMessage("Mutation factor is already at minimum (0%%)."));
		}

		if(theCore->isPressed(2))
		{
			if(ga->getMutationFactor()>0)
			{
				ga->setMutationFactor(0);
				if(ga->getCrossOver()==0)
					msgWindow->addMessage("Deactivated mutations! Evolution will no longer occur."));
				else
					msgWindow->addMessage("Deactivated mutations. From now on evolution will only occur through cross over."));
			}
		}

		if(theCore->isPressed(3))
		{
			int old=ga->getBreedFactor();
			if(ga->getBreedFactor()<10)
				ga->setBreedFactor(ga->getBreedFactor()+1);
			else if(ga->getBreedFactor()<30)
				ga->setBreedFactor(ga->getBreedFactor()+5);
			else if(ga->getBreedFactor()<100)
				ga->setBreedFactor(ga->getBreedFactor()+10);
			if(old!=ga->getBreedFactor())
				msgWindow->addMessage(String::Format(T("Increased breed factor to %i%%."),ga->getMutationFactor())));
			else if(old==100)
			{
				msgWindow->addMessage("Breed factor is already at maximum (100%%)."));
				msgWindow->addMessage("Take a look in the ""help"" menu (settings / breed factor)."));
			}
		}
																				
		if(theCore->isPressed(4))
		{
			int old=ga->getBreedFactor();
			if(ga->getBreedFactor()>30)
				ga->setBreedFactor(ga->getBreedFactor()-10);
			else if(ga->getBreedFactor()>10)
				ga->setBreedFactor(ga->getBreedFactor()-5);
			else if(ga->getBreedFactor()>0)
				ga->setBreedFactor(ga->getBreedFactor()-1);
			if(old!=ga->getBreedFactor())
			{
				if(ga->getBreedFactor()==0)
				{
		//TODO hyperlinks in die Hilfe!
					if(ga->getCrossOver()==0)
						msgWindow->addMessage("Deactivated breeding! Evolution will no longer occur."));
					else
						msgWindow->addMessage("Deactivated breeding. From now on evolution will only occur through cross over."));
				}

																			   
		if(theCore->isPressed(5))
		{
			if(ga->getCrossOver()==0)
			msgWindow->addMessage("Deactivated breeding! Evolution will no longer occur."));
				else
			msgWindow->addMessage("Deactivated breeding. From now on evolution will only occur through cross over."));
			ga->setBreedFactor(0);
		}
	

		if(theCore->isPressed(6))
		{
			int old=ga->getCrossOver();
			if(ga->getCrossOver()<10)
				ga->setCrossOver(ga->getCrossOver()+1);
			else if(ga->getCrossOver()<30)
				ga->setCrossOver(ga->getCrossOver()+5);
			else if(ga->getCrossOver()<100)
				ga->setCrossOver(ga->getCrossOver()+10);
			if(old!=ga->getCrossOver())
				msgWindow->addMessage(String::Format(T("Increased cross over to %i%%."),ga->getCrossOver())));
			else if(old==100)
			{
				msgWindow->addMessage("Cross over is already at maximum (100%%)."));
				msgWindow->addMessage("Take a look in the ""help"" menu (settings / cross over)."));
			}
		}
		if(theCore->isPressed(7))
		{
			int old=ga->getCrossOver();
			if(ga->getCrossOver()>30)
				ga->setCrossOver(ga->getCrossOver()-10);
			else if(ga->getCrossOver()>10)
				ga->setCrossOver(ga->getCrossOver()-5);
			else if(ga->getCrossOver()>0)
				ga->setCrossOver(ga->getCrossOver()-1);
			if(old!=ga->getCrossOver())
			{
				if(ga->getCrossOver()==0)
				{
					if((ga->getBreedFactor()==0)||(ga->getMutationFactor()==0))
					msgWindow->addMessage("Deactivated cross over! Evolution will no longer occur."));
						else
					msgWindow->addMessage("Deactivated crossover. From now on evolution will only occur through breeding and mutations."));
				}
				else msgWindow->addMessage(String::Format(T("Decreased cross over to %i%%."),ga->getCrossOver())));
			}
			else if(old==0)
				msgWindow->addMessage("Cross over is already at minimum (0%%)."));
		}
																				
		if(theCore->isPressed(8))
		{
			if((ga->getBreedFactor()==0)||(ga->getMutationFactor()==0))
			msgWindow->addMessage("Deactivated cross over! Evolution will no longer occur."));
				else
			msgWindow->addMessage("Deactivated crossover. From now on evolution will only occur through breeding and mutations."));
			ga->setCrossOver(0);
		}
	}*/

