#include "bograph.hpp"

#include <sstream>


BOGRAPH::BOGRAPH(const BOGRAPH& object) :
    type(object.type),
    position(object.position),
    height(object.height),
    lines(object.lines),
    edge(object.edge)
{ }

BOGRAPH& BOGRAPH::operator=(const BOGRAPH& object)
{
	type = object.type;
	position = object.position;
	height = object.height;
	lines = object.lines;
	edge = object.edge;
	return(*this);
}

BOGRAPH::BOGRAPH():
	type(0),
	position(0),
	height(0),
	lines(0),
	edge()
{ }

BOGRAPH::~BOGRAPH()
{ }


BoGraphWindow::BoGraphWindow(const BoGraphWindow& object) :
    UI_Window((UI_Window)object),
    lastbographY(object.lastbographY),
    targetbographY(object.lastbographY),
    startbographY(object.startbographY),
    markedUnit(object.markedUnit),
    markedIP(object.markedIP),
    ownMarkedUnit(object.ownMarkedUnit),
    ownMarkedIP(object.ownMarkedIP),
    markAni(object.markAni),
    orderList(object.orderList),
    infoWindow(object.infoWindow),
    anarace(object.anarace)
{
	for(int i = 20;i--;)
		bograph[i] = object.bograph[i];
}

BoGraphWindow& BoGraphWindow::operator=(const BoGraphWindow& object)
{
    ((UI_Window)(*this)) = ((UI_Window)object);
    lastbographY = object.lastbographY;
    targetbographY = object.lastbographY;
    startbographY = object.startbographY;
    markedUnit = object.markedUnit;
    markedIP = object.markedIP;
    ownMarkedUnit = object.ownMarkedUnit;
    ownMarkedIP = object.ownMarkedIP;
    markAni = object.markAni;
    orderList = object.orderList;
    infoWindow = object.infoWindow;
    anarace = object.anarace;
    for(int i = 20;i--;)
        bograph[i] = object.bograph[i];
	return(*this);
}

BoGraphWindow::BoGraphWindow(UI_Object* bograph_parent, ANARACE* bograph_anarace, InfoWindow* bograph_info_window, std::map<long, Order>* bograph_order_list, const unsigned int bograph_window_number) :
	UI_Window(bograph_parent, BOGRAPH_WINDOW_TITLE_STRING, BO_GRAPH_WINDOW, bograph_window_number, SCROLLED),
	lastbographY((unsigned int)(getAbsoluteClientRectUpperBound()+(FONT_SIZE+11))),
	targetbographY(lastbographY),
	startbographY(lastbographY),
	markedUnit(0),
	markedIP(0),
	ownMarkedUnit(0),
	ownMarkedIP(0),
	markAni(0),
	orderList(bograph_order_list),
	infoWindow(bograph_info_window),
	anarace(bograph_anarace)
{
	resetData(); // TODO
}

BoGraphWindow::~BoGraphWindow()
{ }

void BoGraphWindow::assignAnarace(ANARACE* bograph_anarace)
{
	anarace = bograph_anarace;
}

const unsigned int BoGraphWindow::getMarkedIP() const 
{
#ifdef _SCC_DEBUG
    if(ownMarkedIP > MAX_LENGTH) {
        toLog("DEBUG: (BoGraphWindow::getMarkedIP): Value ownMarkedIP out of range.");return(0);
    }
#endif
	return(ownMarkedIP);
}

void BoGraphWindow::setMarkedIP(const unsigned int marked_ip) 
{
#ifdef _SCC_DEBUG
    if(marked_ip > MAX_LENGTH) {
        toLog("DEBUG: (BoGraphWindow::setMarkedIP): Value marked_ip out of range.");return;
	}
#endif
	markedIP = marked_ip;
}

const unsigned int BoGraphWindow::getMarkedUnit() const 
{
#ifdef _SCC_DEBUG
    if(ownMarkedUnit >= UNIT_TYPE_COUNT) {
        toLog("DEBUG: (BoGraphWindow::getMarkedUnit): Value ownMarkedUnit out of range.");return(0);
    }
#endif
	return(ownMarkedUnit);
}

void BoGraphWindow::setMarkedUnit(const unsigned int marked_unit) {
#ifdef _SCC_DEBUG
    if(marked_unit >= UNIT_TYPE_COUNT) {
        toLog("DEBUG: (BoGraphWindow::setMarkedUnit): Value marked_unit out of range.");return;
    }
#endif
	markedUnit = marked_unit;
}

void BoGraphWindow::checkForInfoWindow()
{
	for(std::map<long, Order>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
		if(order->second.getIP()==ownMarkedIP)
		{
//		  int row=((boInsertPoint>-1)&&(order->row>=boInsertPoint))*(boEndPoint-boInsertPoint);
//	  mouse on order in player reinschieben, da is ja auch orderList zuhause
			Rect edge(getRelativeClientRectPosition()+order->second.brect.GetTopLeft()/*-Point(0,getScrollY() TODO)*/,order->second.brect.GetSize());
/*			if((fitItemToRelativeClientRect(edge)&& edge.Inside(controls.getCurrentPosition()-getAbsolutePosition())))
			{
				ownMarkedUnit = order->second.getUnit();
				ownMarkedIP = order->second.getIP();*/
			
				infoWindow->setUnit(order->second.getUnit());
				infoWindow->setIP(order->second.getIP());
				infoWindow->setBx(order->second.brect.GetLeft());
				infoWindow->setBWidth(order->second.brect.GetWidth());
				infoWindow->Show(1);
				infoWindow->setFreeMove();
				if(edge.GetLeft() + 200 > (signed int)(getWidth()))
		            infoWindow->adjustRelativeRect(Rect(getRelativeRightBound() - 200, getRelativeLowerBound() + 5, 200, 110));
				else
	        	    infoWindow->adjustRelativeRect(Rect(edge.GetLeft() + getRelativeLeftBound(), getRelativeLowerBound() + 5, 200, 110));
				return;
//			}*/
		}
}
	
void BoGraphWindow::resetData()
{
	markedUnit=0;markedIP=0;
	ownMarkedUnit=0;ownMarkedIP=0;
	lastbographY=(unsigned int)(getAbsoluteClientRectUpperBound()+(signed int)(FONT_SIZE+11));
	startbographY=lastbographY;
	targetbographY=lastbographY;
	for(int i=20;i--;)
	{
		bograph[i].type=0;
		bograph[i].position=0;
		bograph[i].height=0;
		bograph[i].lines=0;
		bograph[i].edge=Rect(0,0,0,0);
	}
}

void BoGraphWindow::process()
{
	if(!isShown()) 
		return;
	if(!orderList) return;

	markedUnit=0;markedIP=0;
	
	UI_Window::process();
	
	checkForInfoWindow();

	Size::mv(lastbographY, startbographY, targetbographY);
}

void BoGraphWindow::copyBoGraph(const BOGRAPH* bo_graph)
{
	for(int i=20;i--;)
	{
	    bograph[i].type = bo_graph[i].type;
	    bograph[i].position = bo_graph[i].position;
	    bograph[i].height = bo_graph[i].height;
	    bograph[i].lines = bo_graph[i].lines;
	    bograph[i].edge = bo_graph[i].edge;
	}
}

void BoGraphWindow::setBoGraphY(unsigned int last_bograph_y)
{
	if(last_bograph_y != targetbographY)
	{
		targetbographY = last_bograph_y;
		startbographY = lastbographY;
	}
}

void BoGraphWindow::mouseHasMoved()
{
	if(!getAbsoluteRect().Inside(mouse))
		return;
	ownMarkedUnit=0;ownMarkedIP=0;
	for(std::map<long, Order>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		if(anarace->getProgramFacility(order->second.getIP()))
		{
			Rect edge=Rect(getAbsolutePosition()+getRelativeClientRectPosition()+order->second.brect.GetTopLeft(), order->second.brect.GetSize());
			if(edge.Inside(mouse))
			{
				ownMarkedUnit=order->second.getUnit();
				ownMarkedIP=order->second.getIP();
				return;
			}
		}
	}

}

void BoGraphWindow::draw(DC* dc) const
{
	if(!isShown()) 
		return;
	UI_Window::draw(dc);

	if(!orderList) return;
	// now print the lines...
	dc->SetPen(*theme.lookUpPen(INNER_BORDER_PEN));
	for(int i=0;i<20;i++)
		if(bograph[i].type>0)
	//		for(int j=0;j<bograph[i].lines;j++)
			{
				Rect rec=Rect(bograph[i].edge.GetTopLeft()/*+Point(0,j*(FONT_SIZE+5))*/, bograph[i].edge.GetSize());
//				if(insideAbsoluteClientRect(rec)) // TODO
				// TODO BUTTONS drausmachen...
				{
				//	if(j%2==0)
				//		dc->SetBrush(*theme.lookUpBrush(BO_DARK_BRUSH));
				//	else 
					dc->SetBrush(*theme.lookUpBrush(TRANSPARENT_BRUSH));
					dc->DrawEdgedRoundedRectangle(rec,3);
				}
			}
	if((signed int)lastbographY > getAbsoluteClientRectUpperBound()+(signed int)(FONT_SIZE+10))
	{
// and the time steps on the top
	dc->SetTextForeground(*theme.lookUpColor(TIMESTEPS_TEXT_COLOR));
// dc->SetPen(*BLACK_PEN); TODO
	unsigned int timesteps=((anarace->getRealTimer())/30)/10+1; // TODO <- wird 0? bei Protoss? :-/
	dc->SetPen(*theme.lookUpPen(GREEN_TIMESTEPS_PEN));
	for(unsigned int i=0;i<(anarace->getRealTimer())/30;i++)
		if(i%timesteps==0)
		{
			if(i>0) 
			{
				dc->DrawVerticalLine(getAbsoluteClientRectLeftBound()+5+i*((getClientRectWidth()-20)/(anarace->getRealTimer()/30)),
							       getAbsoluteClientRectUpperBound()+(FONT_SIZE+5), getAbsoluteClientRectLowerBound()-5/*lastbographY*/);
				dc->DrawHorizontalLine(getAbsoluteClientRectLeftBound()+5+i*((getClientRectWidth()-20)/(anarace->getRealTimer()/30)), getAbsoluteClientRectUpperBound()+(FONT_SIZE+5), getAbsoluteClientRectLeftBound()+5+i*((getClientRectWidth()-20)/(anarace->getRealTimer()/30)) + 5);
			}
				
			std::ostringstream os;
			os << i/2 << ":" << 3*(i%2) << "0";
			dc->DrawText(os.str(), getAbsoluteClientRectPosition()+Point(5+i*((getClientRectWidth()-20)/((anarace->getRealTimer())/30)), 0));
		}

// --------------------------------- END BUILD ORDER GRAPH ------------------------------
	}																			
// ... and finally the orders
	for(std::map<long, Order>::const_iterator order=orderList->begin(); order!=orderList->end(); ++order)
	{
		if(anarace->getProgramFacility(order->second.getIP()))
		{
			Rect edge=Rect(getRelativeClientRectPosition()+order->second.brect.GetTopLeft(), order->second.brect.GetSize());
//			if(insideRelativeClientRect(edge))
			{
				if(((markedUnit>0)&&(order->second.getUnit()==markedUnit))||
				   ((markedIP>0)&&(order->second.getIP()==markedIP))||
//				   ((ownMarkedUnit>0)&&(order->second.getUnit()==ownMarkedUnit))||
				   ((ownMarkedIP>0)&&(order->second.getIP()==ownMarkedIP)))
					dc->SetBrush(*theme.lookUpBrush(BRIGHT_CONTINUE_BUTTON_BRUSH));
				else
					dc->SetBrush(*theme.lookUpBrush((eBrush)(UNIT_TYPE_0_BRUSH+stats[anarace->getRace()][order->second.getUnit()].unitType)));
				
				dc->SetPen(*theme.lookUpPen((ePen)(BRIGHT_UNIT_TYPE_0_PEN+stats[anarace->getRace()][order->second.getUnit()].unitType)));
				dc->DrawEdgedRoundedRectangle(Rect(getAbsolutePosition()+edge.GetTopLeft(), edge.GetSize()),3);
			}
		}
	}
	//finally print the legend
	dc->SetFont(UI_Object::theme.lookUpFont(SMALL_MIDDLE_NORMAL_FONT));
	dc->SetTextForeground(*theme.lookUpColor(BRIGHT_TEXT_COLOR));
	for(int i=0;i<20;i++)
		if(bograph[i].type>0)
			dc->DrawText(" "+*UI_Object::theme.lookUpString((eString)(UNIT_TYPE_COUNT*anarace->getRace()+bograph[i].type+UNIT_NULL_STRING)), getAbsoluteClientRectPosition()+Point(0, 4+(i+1)*(FONT_SIZE+10)));
}





