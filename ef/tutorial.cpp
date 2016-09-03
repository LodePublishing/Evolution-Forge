#include "tutorial.hpp"

TutorialWindow::TutorialWindow(UI_Window* parentWindow):UI_Window(parentWindow, TUTORIAL_WINDOW_TITLE_STRING, TUTORIAL_WINDOW, 0, NOT_SCROLLED, NO_AUTO_SIZE_ADJUST, TABBED)
{
	tutorialChapter=120; // TODO => in tabs einteilen
}

TutorialWindow::~TutorialWindow()
{
}

void TutorialWindow::process()
{
	if(!isShown()) return;
	UI_Window::process();
}


void TutorialWindow::draw(DC* dc) const
{
    if(!isShown()) return;
	UI_Window::draw(dc);
/*        int t=0;
        for(t=0;t<8;t++)
        if(tutorialWindow->isActivated(t))
        {
            tutorialAnimation=1;
            if(tutorialChapter==0)
                tutorialChapter=100+t*100;
            else if(tutorialChapter%100==0)
            {
                if(t==0)
                    tutorialChapter=0;
                else
                    tutorialChapter+=t*10;
            }
            else if(tutorialChapter%10==0)
            {
                if(t==0)
                    tutorialChapter-=tutorialChapter%100;
            }
        }
        tutorialAnimation++;
        if((tutorialAnimation==400)&&(!isOptimizing()))
            tutorialAnimation=399;
                                                                                                                                                            
        Rect edge;
        switch(tutorialChapter)
        {
            case 0:
                {
        //TODO info window wenn Maus drueber :o
        // evtl auch Mauszeiger aendern
                    tutorialWindow->writeHeader("Tutorial"),dc,0);
                    tutorialWindow->textButton("Chapter 1: Introduction"),dc);
                    tutorialWindow->textButton("Chapter 2: Detailed description"),dc);
                    tutorialWindow->textButton("Chapter 3: How to use the results"),dc);
                    tutorialWindow->textButton("Chapter 4: Advanced & Expert Mode"),dc);
                    tutorialWindow->textButton("Chapter 5: Gosu & Transcendend Mode"),dc);
                    tutorialWindow->textButton("Chapter 6: The core"),dc);
                    tutorialWindow->textButton("Chapter 7: The web"),dc);
                    tutorialWindow->textButton("Chapter 8: Development of EF"),dc);
                }break;
            case 100:
                {
                    tutorialWindow->writeHeader("1. Introduction"),dc,1);
                    tutorialWindow->textButton("1.1: What does this program?"),dc);
                    tutorialWindow->textButton("1.2: Quickstart"),dc);
                    tutorialWindow->textButton("1.3: Overview of the modes"),dc);
                    tutorialWindow->textButton("1.4: Is it useful?"),dc);
                    tutorialWindow->textButton("1.5: Where are the limits?"),dc);
                    tutorialWindow->textButton("1.6: Is it cheating?"),dc);
                }
                break; // Introduction
            case 110:
                {
                    tutorialWindow->writeHeader("1.1: What does this program?"),dc,2);
                    tutorialWindow->writeLongText("The program simulates an abstract $StarCraft:Broodwar$ environment and calculates the time a certain build order needs.#By comparing different build orders, randomly changing them and taking the best build order for the next generation, the build order is improved step by step.#$StarCraft$ build orders fit perfectly for this algorithm called $HillClimbing$.#"),dc);
                }break;
            case 120:
                {
                                                                                                                                                            
                    tutorialWindow->writeHeader("1.2: Quickstart"),dc,2);
                    chooseColor(tutorialAnimation/2,0);
                    tutorialWindow->writeLongText("It's easy:#"),dc);                     tutorialWindow->writeLongText(" - Click on the left side in the goal list window $'Click here to add goal'$, choose a category and pick your goals.#"),dc,chooseColor(tutorialAnimation/4,0));                     tutorialWindow->writeLongText("  - Click on the top right on the flashing $'Click to continue'$ button and sit back to watch the progress.#"),dc,chooseColor(tutorialAnimation/4,1));                     tutorialWindow->writeLongText("  - You will see items moving around in window named $'Build Order'$ on the bottom right. The program now will optimize the build order by rearranging the items.#"),dc,chooseColor(tutorialAnimation/4,2));                     tutorialWindow->writeLongText("  - On the top right you will see the current best time in the $'Timer window'$. If you read there a $'Searching'$ it means, the program is still searching for a valid solution which will need a moment depending how many goals you have entered while $'Optimizing'$ means that a valid solution is found and the program tries to optimize it.#"),dc,chooseColor(tutorialAnimation/4,3));                     tutorialWindow->writeLongText("  - On the top you will see the $'Statistical Data'$ window which displays changes in time, fitness and ressources of the best program over the last few seconds. It is interesting for the advanced user, this window is availible on $'Advanced'$ mode or higher.#"),dc,chooseColor(tutorialAnimation/4,4));
                                                                                                                                                                                 tutorialWindow->writeLongText("  - Evolution forge also allows you to see a graphical illustration (through the $'Graphical illustration of the build order'$ window) of the build order to see what buildings are built at the same time, how long they need and when to build them. In addition there is the $'Overview of the build order'$ window where you can see how much minerals, gas and supply you have at a certain time if you play this build order.#"),dc,chooseColor(tutorialAnimation/4,5));                     tutorialWindow->writeLongText("  - After a while you will notice that there are no more changes on the screen. Probably the best build order is found. Press $ALT+T$ to stop and let program print out the build order via the File menu.#"),dc,chooseColor(tutorialAnimation/4,6));
                    if(tutorialAnimation>1450)
                    {
                        tutorialAnimation=1;                         tutorialWindow->adjustClientRect(Rect(mainWindow->getAbsoluteClientRectLeftBound()+mainWindow->getClientRectWidth()-(tutorialWindow->getTargetWidth()),mainWindow->getClientRectUpperBound()+SECOND_ROW,tutorialWindow->getTargetWidth(),400));
                        stopOptimizing();
                    }
                                                                                                                                                            
                    if((tutorialAnimation<400)&&(isOptimizing()))
                        tutorialAnimation=400;
                                                                                                                                                            
                    if(tutorialAnimation<200)
                        player[0]->forceWindow->Show(1);
                    else
                    if(tutorialAnimation<400)
                        player[0]->timerWindow->Show(1);
                    else
                    if(tutorialAnimation<600)
                    {
                                                tutorialWindow->adjustClientRect(Rect(theCore->getRelativeLeftBound(),mainWindow->getClientRectUpperBound()+125,tutorialWindow->getTargetWidth(),500));
                        player[0]->timerWindow->Show(0);
                        player[0]->boWindow->Show(1);
                    }
                    else
                    if(tutorialAnimation<800)
                        player[0]->timerWindow->Show(1);
                    else if(tutorialAnimation<1000)
                    {
                        player[0]->statisticsWindow->Show(1);
                        tutorialWindow->adjustClientRect(Rect(theCore->getRelativeLeftBound(),200,tutorialWindow->getTargetWidth(),500));
                    }
                    else if(tutorialAnimation<1200)
                   {
                        player[0]->statisticsWindow->Show(0);
// Problem: da das hier ja oefters aufgerufen wird, wuerde jede Veraenderung durch andere Teile des Programms der Hoehe und Breite hier wieder zu nichte gemacht werden!                        boDiagramWindow->adjustClientRect(Rect(boDiagramWindow->getRelativeLeftBound(),theCore->getLowerBound()+5+150,boDiagramWindow->getWidth(),boDiagramWindow->getHeight
                        tutorialWindow->adjustClientRect(Rect(theCore->getRelativeLeftBound(),mainWindow->getClientRectUpperBound(),tutorialWindow->getTargetWidth(),500));
                    }
                }break;
            case 130:
                {
                    tutorialWindow->writeHeader("1.3: Overview of the modes"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 140:
                {
                    tutorialWindow->writeHeader("1.4: Is it useful?"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 150:
                {
                    tutorialWindow->writeHeader("1.5: Where are the limits?"),dc,2);
                    tutorialWindow->writeLongText(" - It only calculates a general build order. Depending on the map, the latency, your mousespeed and many other small factors there might be a better build order.# - Goals consisting of many units will result in exponential growth of time needed for the calculation.# - The program is based on one-second steps. This might lead to some inaccurate calculation.# - In most cases the build orders cannot be applied directly 1:1 in the game but you have to exercise them and adapt them to your needs.#"),dc);
                }break;
            case 160:
                {
                    tutorialWindow->writeHeader("1.6: Is it cheating?"),dc,2);
                    tutorialWindow->writeLongText("Well... in a way... yes.#If you use the program correctly you might get an advantage over your enemy: You learn to get a better starting in the game so that you can concentrate on the main thing, strategies and tactics!#While you can get the same information by watching replays, reading strategy articles or just playing the game it is much more fun to interactivly trying out combinations to compare them.#"),dc);
                }break;
                                                                                                                                                            
            case 200:
                {
                    tutorialWindow->writeHeader("2. Detailed Description"),dc,1);
                    tutorialWindow->textButton("2.1: The goal window"),dc);                     tutorialWindow->textButton("2.2: The build order window"),dc);
                    tutorialWindow->textButton("2.3: The statistics window"),dc);
                    tutorialWindow->textButton("2.4: The overview window"),dc);
                    tutorialWindow->textButton("2.5: The graphical window"),dc);
                    tutorialWindow->textButton("2.6: The timer window"),dc);
                }
                break; // Detail
            case 210:
                {
                    tutorialWindow->writeHeader("2.1: The goal window"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 220:
                {
                    tutorialWindow->writeHeader("2.2: The build order window"),dc,2);
                    tutorialWindow->writeLongText(""),dc);                 }break;             case 230:
                {                     tutorialWindow->writeHeader("2.3: The statistics window"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 240:
                {
                    tutorialWindow->writeHeader("2.4: The overview window"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 250:
                {
                    tutorialWindow->writeHeader("2.5: The graphical window"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 260:
                {
                    tutorialWindow->writeHeader("2.6: The timer window"),dc,2);
                    tutorialWindow->writeLongText(""),dc);                 }break;                                                                                                                                                                          case 300:
                {
                    tutorialWindow->writeHeader("3. How to use the results"),dc,1);
                    tutorialWindow->textButton("3.1: About time and numbers"),dc);
                    tutorialWindow->textButton("3.2: HTML output"),dc);
                    tutorialWindow->textButton("3.3: Printing output"),dc);                     tutorialWindow->textButton("3.4: During a game"),dc);
                    tutorialWindow->textButton("3.5: Using the database"),dc);
                    tutorialWindow->textButton("3.6: Publishing your database"),dc);
                }
                break; // I/O
            case 310:
                {
                    tutorialWindow->writeHeader("3.1: About time and numbers"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 320:
                {
                    tutorialWindow->writeHeader("3.2: HTML output"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 330:
                {
                    tutorialWindow->writeHeader("3.3: Printing output"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 340:
                {
                    tutorialWindow->writeHeader("3.4: During a game"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 350:
                {
                    tutorialWindow->writeHeader("3.5: Using the database"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 360:
                {
                    tutorialWindow->writeHeader("3.6: Publishing your database"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
                                                                                                                                                            
            case 400:
                {
                    tutorialWindow->writeHeader("4. Advanced & Expert Mode"),dc,1);
                    tutorialWindow->textButton("4.1: Testing the bo in Advanced mode"),dc);
                    tutorialWindow->textButton("4.2: New settings in Advanced mode"),dc);
                    tutorialWindow->textButton("4.3: New settings in Expert mode"),dc);
                    tutorialWindow->textButton("4.4: A guide to finetune your bo I"),dc);
                    tutorialWindow->textButton("4.5: A guide to finetune your bo II"),dc);
                    tutorialWindow->textButton("4.6: A guide to finetune your bo III"),dc);
                }
                break; // Expert & advanced mode
            case 410:
                {
                    tutorialWindow->writeHeader("4.1: Testing the bo in Advanced Mode"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 420:
                {
                    tutorialWindow->writeHeader("4.2: New settings in Advanced mode"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 430:
                {
                    tutorialWindow->writeHeader("4.3: New settings in Expert mode"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 440:
                {
                    tutorialWindow->writeHeader("4.4: A guide to finetune your bo I"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 450:
                {
                    tutorialWindow->writeHeader("4.5: A guide to finetune your bo II"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 460:
                {
                    tutorialWindow->writeHeader("4.6: A guide to finetune your bo III"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
                                                                                                                                                            
            case 500:
                {
                    tutorialWindow->writeHeader("5. Gosu & Transcendend Mode"),dc,1);
                    tutorialWindow->textButton("5.1: Gosu mode - Can you beat it?"),dc);
                    tutorialWindow->textButton("5.2: Strategies, hints, tricks"),dc);
                    tutorialWindow->textButton("5.3: What to learn from such tournaments"),dc);
                    tutorialWindow->textButton("5.4: Transcendend mode - silicium alife!"),dc);
                    tutorialWindow->textButton("5.5: What to learn from the computer"),dc);
                    tutorialWindow->textButton("5.6: Short analysis of such tournaments"),dc);
                }
                break; // Gosu & trancendent mode
            case 510:
                {
                    tutorialWindow->writeHeader("5.1: Gosu mode - Can you beat it?"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 520:
                {
                    tutorialWindow->writeHeader("5.2: Strategies, hints, tricks"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 530:
                {
                    tutorialWindow->writeHeader("5.3: What to learn from such tournaments"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 540:
                {
                    tutorialWindow->writeHeader("5.4: Transcendend mode - silicium alife!"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 550:
                {
                    tutorialWindow->writeHeader("5.5: What to learn from the computer"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 560:
                {
                    tutorialWindow->writeHeader("5.6: Short analysis of such tournaments"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
                                                                                                                                                            
            case 600:
                {
                    tutorialWindow->writeHeader("6. The Core"),dc,1);
                    tutorialWindow->textButton("6.1: Basic underlying algorithm"),dc);
                    tutorialWindow->textButton("6.2: Internal representation of an order list"),dc);
                    tutorialWindow->textButton("6.3: Flexibility/Scalability of the core"),dc);
                    tutorialWindow->textButton("6.4: A chapter about Genetic Algorithms"),dc);
                    tutorialWindow->textButton("6.5: Comparison with other algorithms"),dc);
                    tutorialWindow->textButton("6.6: Problems of crossing over"),dc);
                }
                break; // The Core
            case 610:
                {
                    tutorialWindow->writeHeader("6.1: Basic underlying algorithm"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 620:
                {
                    tutorialWindow->writeHeader("6.2: Internal representation of an order list"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 630:
                {
                    tutorialWindow->writeHeader("6.3: Flexibility/Scalability of the core"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 640:
                {
                    tutorialWindow->writeHeader("6.4: A chapter about Genetic Algorithms"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 650:
                {
                    tutorialWindow->writeHeader("6.5: Comparison with other algorithms"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 660:
                {
                    tutorialWindow->writeHeader("6.6: Problems of crossing over"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
                                                                                                                                                            
            case 700:
                {
                    tutorialWindow->writeHeader("7. The web"),dc,1);
                    tutorialWindow->textButton("7.1: www.clawsoftware.de"),dc);
                    tutorialWindow->textButton("7.2: www.nix-step.com"),dc);
                    tutorialWindow->textButton("7.3: www.my-friendly-publisher.com"),dc);
                    tutorialWindow->textButton("7.4: Read news about EF"),dc);
                    tutorialWindow->textButton("7.4: Browse the database"),dc);
                    tutorialWindow->textButton("7.6: Check for updates"),dc);
                }
                break; // The web
            case 710:
                {
                    tutorialWindow->writeHeader("7.1: www.clawsoftware.de"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 720:
                {
                    tutorialWindow->writeHeader("7.2: www.nix-step.com"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 730:
                {
                    tutorialWindow->writeHeader("7.3: www.my-friendly-publishers.com"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 740:
                {
                    tutorialWindow->writeHeader("7.4: Read news about EF"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 750:
                {
                    tutorialWindow->writeHeader("7.5: Browse the database"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 760:
                {
                    tutorialWindow->writeHeader("7.6: Check for updates"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
                                                                                                                                                            
            case 800:
                {
                    tutorialWindow->writeHeader("8. Development of EF"),dc,1);
                    tutorialWindow->textButton("8.1: Things 'todo'"),dc);
                    tutorialWindow->textButton("8.2: Things that will not be implemented"),dc);
                    tutorialWindow->textButton("8.3: About the past"),dc);
                    tutorialWindow->textButton("8.4: About the future"),dc);
                    tutorialWindow->textButton("8.5: About the programmer"),dc);
                    tutorialWindow->textButton("8.6: Join the crew - join the development"),dc);
                }
                break; // Development
            case 810:
                {
                    tutorialWindow->writeHeader("8.1: Things 'todo'"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 820:
                {
                    tutorialWindow->writeHeader("8.2: Things that will not be implemented"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 830:
                {
                    tutorialWindow->writeHeader("8.3: About the past"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 840:
                {
                    tutorialWindow->writeHeader("8.4: About the future"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 850:
                {
                    tutorialWindow->writeHeader("8.5: About the programmer"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
            case 860:
                {
                    tutorialWindow->writeHeader("8.6: Join the crew - join the development"),dc,2);
                    tutorialWindow->writeLongText(""),dc);
                }break;
                                                                                                                                                            
            default:break;
        }*/
}
