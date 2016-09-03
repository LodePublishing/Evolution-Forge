#include "object.hpp"

UI_Object::UI_Object(UI_Object* parent_object, const Rect relative_rect, const Size distance_bottom_right, const ePositionMode position_mode, const eAutoSize auto_size) :
	isClipped(false),
	clipRect(),
	children(NULL),
	childrenWereChanged(false),

	positionMode(position_mode),
	
	blitRectList(),
	pufferInvalid(true),
	drawPuffer(NULL),
	drawType(SOLID_OBJECT),
	
	relativeRect(relative_rect),
	startRect(relative_rect),
	targetRect(relative_rect),
	originalRect(relative_rect),
	distanceBottomRight(distance_bottom_right),
	oldRect(relative_rect),
	sizeHasChanged(true),

	autoSize(auto_size),
	shown(true),
	oldShown(true),

	parent(NULL),
	positionParent(NULL),
	prevBrother(this),
	nextBrother(this),
	toolTipEString(NULL_STRING),
	toolTipString()
{
	setParent(parent_object);
	UI_Object::objectList.push_back(this);
	Show();
}

UI_Object::UI_Object(UI_Object* parent_object, UI_Object* position_parent_object, const Rect relative_rect, const Size distance_bottom_right, const ePositionMode position_mode, const eAutoSize auto_size) :
	isClipped(false),
	clipRect(),
	children(NULL),
	childrenWereChanged(false),
	positionMode(position_mode),

	blitRectList(),
	pufferInvalid(true),
	drawPuffer(NULL),
	drawType(SOLID_OBJECT),

	relativeRect(relative_rect),
	startRect(relative_rect),
	targetRect(relative_rect),
	originalRect(relative_rect),
	distanceBottomRight(distance_bottom_right),
	oldRect(relative_rect),
	sizeHasChanged(true),

	autoSize(auto_size),
	shown(true),
	oldShown(true),

	parent(NULL),
	positionParent(NULL),
	prevBrother(this),
	nextBrother(this),
	toolTipEString(NULL_STRING),
	toolTipString()
{
	setParent(parent_object);
	setPositionParent(position_parent_object);
	UI_Object::objectList.push_back(this);
	Show();
}

UI_Object::~UI_Object()
{
	oldRectList.push_back(getAbsoluteRect());
	if(UI_Object::focus == this)
		UI_Object::focus = NULL;
	removeFromFamily(); // !!!!!
	for(std::list<UI_Object*>::iterator i = objectList.begin(); i != objectList.end();)
		if(*i == this)
		{
			i = objectList.erase(i);
//			toErrorLog("erase " + getAbsoluteRect().toString());
			return;
		} else i++;
}




const bool UI_Object::initSDL(std::list<std::string>& arguments, std::string window_title)
{
	std::list<std::string> string_files = findFiles("data", "strings", "help", "");

	setResolution(uiConfiguration.getResolution(), true);
	
	toInitLog("* " + theme.lookUpString(uiConfiguration.isFullScreen() ? START_SET_FULLSCREEN_MODE_STRING : START_SET_WINDOW_MODE_STRING));
	Uint32 flags = SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_HWACCEL|SDL_HWPALETTE|SDL_SRCCOLORKEY|SDL_RLEACCEL|SDL_SRCALPHA|SDL_PREALLOC|SDL_DOUBLEBUF;
	if(uiConfiguration.isFullScreen())
		flags |= SDL_FULLSCREEN;
#ifdef _SDL_MIXER_SOUND
	flags |= SDL_INIT_AUDIO;
#endif
	dc = new DC(UI_Object::theme.getCurrentResolutionSize(), UI_Object::theme.getBitDepth(), flags, SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO);
		
	if(!dc->initializationOK())	{
		toErrorLog(UI_Object::theme.lookUpString(START_UNABLE_TO_INIT_SDL_STRING) + " [SDL ERROR: \"" + SDL_GetError() + "\"]");
		delete dc;
		return(false);
	}
	
	if ( !dc->valid() ) {
		toErrorLog(UI_Object::theme.lookUpString(START_ERROR_SETTING_VIDEO_MODE_STRING) + " [SDL ERROR: \"" + SDL_GetError() + "\"]");
		delete dc;
		return(false);
	}

	UI_Object::theme.initCursors();
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	toInitLog(UI_Object::theme.printHardwareInformation());
	toInitLog("* " + UI_Object::theme.lookUpString(START_CREATED_SURFACE_STRING) + " " + UI_Object::theme.printSurfaceInformation(dc));
// ------ END INIT SDL AND WINDOW ------

	SDL_WM_SetCaption(window_title.c_str(),"");

// ------ INIT SOUND ENGINE -------
	bool sound_not_initialized = true;
	if((uiConfiguration.isSound())||(uiConfiguration.isMusic()))
	{
		toInitLog(UI_Object::theme.lookUpString(START_INIT_SOUND_STRING)); 
		if(!UI_Object::sound.init())
		{
			uiConfiguration.setSound(false);
			uiConfiguration.setMusic(false);
		} else
			sound_not_initialized = false;
	}
// ------ END INIT SOUND ENGINE -------


// ------ INIT SDL_TTF ------
	toInitLog("* " + UI_Object::theme.lookUpString(START_INIT_SDL_TRUETYPE_FONTS_STRING));
	if(TTF_Init()==-1) {
		toErrorLog(UI_Object::theme.lookUpString(START_INIT_SDL_TTF_ERROR_STRING) + " [\"" + TTF_GetError() + "\"]");
		delete dc;
		return(false);
	}
	atexit(TTF_Quit); 

	return(true);
}

const bool UI_Object::addKey(unsigned int key, unsigned int mod)
{
	return(false);
}

void UI_Object::reloadOriginalSize()
{

//=> Bool Variable und dann in process pruefen (nach show) damit erst dann upgedatet wird wenn das Teil wieder gezeigt wird.  TODO
	
	UI_Object* tmp = children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->reloadOriginalSize();
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}

}

void UI_Object::adjustSize(const eAdjustMode adjust_mode, const Size& size)
{
	signed int left = originalRect.getLeft();
	unsigned int full_width;
	if(getParent() != NULL)
	{
		if(left + distanceBottomRight.getWidth() > getParent()->getWidth())
			full_width = 0;
		else 
			full_width = getParent()->getWidth() - left - distanceBottomRight.getWidth();
	} 
	else
		full_width = getWidth();

	if(adjust_mode == PARENT_WAS_CHANGED)
	{
// called by PARENT after PARENT's size was changed
		switch(autoSize)
		{
			case NOTHING:break;
			case NO_AUTO_SIZE:break;
			case AUTO_SIZE:break;
			case AUTO_HEIGHT_FULL_WIDTH:
			case AUTO_DOUBLE_HEIGHT_FULL_WIDTH:
			case FULL_WIDTH:setWidth(full_width);break;
			case AUTO_HEIGHT_CONST_WIDTH:break;
			case AUTO_DOUBLE_HEIGHT_CONST_WIDTH:break;
			case CONST_SIZE:break;
			default:break; // TODO ERROR
		}
	} else
	if(adjust_mode == CHILD_WAS_CHANGED)
	{
		childrenWereChanged = true;
// called if the object is dependant on the child's size (e.g. Button - StaticText)
// TODO maybe update other children too
		switch(autoSize)
		{
			case NOTHING:break;
			case NO_AUTO_SIZE:setSize(originalRect.getSize());break;
			case AUTO_SIZE:setSize(size+Size(6, 0));break;
			case AUTO_HEIGHT_FULL_WIDTH:setSize(Size(full_width, size.getHeight()));break;
			case AUTO_DOUBLE_HEIGHT_FULL_WIDTH:setSize(Size(full_width, 2*size.getHeight()));break;
			case FULL_WIDTH:setSize(Size(full_width, getHeight()));break;
			case AUTO_HEIGHT_CONST_WIDTH:setSize(Size(originalRect.getWidth(), size.getHeight()));break;
			case AUTO_DOUBLE_HEIGHT_CONST_WIDTH:setSize(Size(originalRect.getWidth(), 2*size.getHeight()));break;
			case CONST_SIZE:setSize(Size(originalRect.getWidth(), originalRect.getHeight()));break;
			default:break; // TODO ERROR
		}
	}
}
	
void UI_Object::adjustPosition()
{
	signed int left = originalRect.getLeft() + distanceBottomRight.getWidth();
	signed int top = originalRect.getTop() + distanceBottomRight.getHeight();
	unsigned int full_width;
	unsigned int full_height;
	if(getParent() != NULL)
	{
		full_width = getParent()->getWidth() - left;// - distanceBottomRight.getWidth();
		full_height = getParent()->getHeight() - top;// - distanceBottomRight.getHeight();
	} 
	else
	{
		full_width = getWidth();
		full_height = getHeight();
	}
	
	signed int hor_center = left + ((signed int)full_width - (signed int)getWidth())/2;
	signed int right = (signed int)full_width - left - (signed int)getWidth();

	signed int ver_center = top + ((signed int)full_height - (signed int)getHeight())/2;
	if(ver_center < top) ver_center = top;
	signed int bottom = (signed int)full_height - top - (signed int)getHeight();

//	setPosition(originalButtonRect.getTopLeft());
	//relativeRect ok... aber was ist mit startRect, targetRect etc??

 	switch(positionMode)
	{
		case DO_NOT_ADJUST:break;//setPosition(originalButtonRect.getTopLeft());break;
		case TOTAL_CENTERED:setPosition(hor_center, ver_center);break;
		case HORIZONTALLY_CENTERED:setLeft(hor_center);break;
		case VERTICALLY_CENTERED:setTop(ver_center);break;
		case TOP_LEFT:setPosition(left, top);break;
		case TOP_CENTER:setPosition(hor_center, top);break;
		case TOP_RIGHT:setPosition(right, top);break;
		case CENTER_RIGHT:setPosition(right, ver_center);break;
		case BOTTOM_RIGHT:setPosition(right, bottom);break;
		case BOTTOM_CENTER:setPosition(hor_center, bottom);break;
		case BOTTOM_LEFT:setPosition(left, bottom);break;
		case CENTER_LEFT:setPosition(left, ver_center);break;
		default:break;
	}
}


void UI_Object::adjustRelativeRect(const Rect& edge)
{
	if(edge != targetRect)
	{
// neues Ziel?
/*
		if((edge.getLeft()!=targetRect.getLeft())||(edge.getRight()!=targetRect.getRight()))
		{
			startRect.setLeft(getRelativeLeftBound());
//			if(edge.getRight()!=targetRect.getRight())
			startRect.setRight(getRelativeRightBound());
		}
			
		if((edge.getTop()!=targetRect.getTop())||(edge.getHeight()!=targetRect.getHeight()))
		{
			startRect.setTop(getRelativeUpperBound());
//			if(edge.getHeight()!=targetRect.getHeight())
			startRect.setBottom(getRelativeLowerBound());
		}*/

		if(edge.getTopLeft()!=targetRect.getTopLeft())
			startRect.setTopLeft(getRelativePosition());
		if(edge.getSize()!=targetRect.getSize())
			startRect.setSize(getSize());
 
		targetRect=edge;
	}
//	targetRect.width=edge.width;
  //  targetRect.height=edge.height;
}


void UI_Object::removeFromFamily()
{
	if((parent)&&(parent->children == this))
	{
		if (nextBrother == this)  // the only child?
			parent->children = NULL;  // if so, parent now has now no children
		else
			parent->children = nextBrother;  // next sibling is now the oldest
	} 
	parent = NULL;
	if (nextBrother != this) {  // does this object have siblings?
		nextBrother->prevBrother = prevBrother;
		prevBrother->nextBrother = nextBrother;
	}
	
	nextBrother = prevBrother = this;
}

void UI_Object::setPositionParent(UI_Object* daddy)
{
	positionParent = daddy;
}

void UI_Object::setParent(UI_Object* daddy) 
{
	setPositionParent(daddy);
	removeFromFamily();
	parent = daddy;
	if(parent)
		setZ(parent->getZ()-5);
	else setZ(100);

	if(!daddy)
		return;
	daddy->addChild(this);
}

void UI_Object::addChild(UI_Object* child)
{
	childrenWereChanged = true;
	if (!children) 
		children = child;
	else 
	{
		UI_Object *eldest_sibling, *youngest_sibling;

		eldest_sibling = children;
		youngest_sibling = eldest_sibling->prevBrother;

		child->nextBrother = eldest_sibling;
		child->prevBrother = youngest_sibling;

		eldest_sibling->prevBrother = child;
		youngest_sibling->nextBrother = child;
	}
}

struct sort_by_z
{
	bool operator()(UI_Object const* p1, UI_Object const* p2)
	{
		if(!p1)
			return true;
		if(!p2)
			return false;
		return p1->getZ() < p2->getZ();
	}
};

void UI_Object::updateScreen()
{
	objectList.sort(sort_by_z());

	for(std::list<UI_Object*>::const_iterator i = objectList.begin(); i != objectList.end(); ++i)
		if((*i)->isShown() && ((*i)->isPufferInvalid()) && ((*i)->getDrawType() == ANTI_ALIASED_OBJECT))
			oldRectList.push_back((*i)->getAbsoluteRect());
	
	Rect::connectRects(oldRectList);

// Objekt befindet sich vor dem neu zu malenden Bereich => diesen Bereich nicht neukopieren => aus newRectList loeschen
	for(std::list<UI_Object*>::const_iterator i = objectList.begin(); i != objectList.end(); ++i)
		if((*i)->isShown() && ((*i)->getDrawType() == SOLID_OBJECT))
		{
			for(std::list<std::pair<signed int, Rect> >::iterator j = newRectList.begin(); j != newRectList.end(); )
	// objekt vor dem zu uebermalendem Gebiet
				if((*i)->getZ() < j->first)
				{
					std::list<Rect> t;
					t = j->second.withoutRect((*i)->getAbsoluteRect());
					for(std::list<Rect>::iterator l = t.begin(); l != t.end(); ++l)
					{
						newRectList.insert(j, std::pair<signed int, Rect>(j->first, *l));
					}
					j = newRectList.erase(j);
				} else ++j;				
		}
	int zz = 0;
	for(std::list<UI_Object*>::const_iterator i = objectList.begin(); i != objectList.end(); ++i)
		if((*i)->isShown())
	{
//		++zz;
//		toErrorLog(zz);

//		von allen 'oldrects' die absolut-rects abziehen
		for(std::list<Rect>::iterator j = oldRectList.begin(); j != oldRectList.end(); )
		{
	//		toErrorLog(".");
	//		toErrorLog(oldRectList.size());
			Rect r = j->commonRect((*i)->getAbsoluteRect());
			if(r != Rect())
			{
//				std::ostringstream os;
//				os << r.getLeft() << " " << r.getTop() << " " << r.getRight() << " " << r.getBottom();
//				os << " / " << j->getLeft() << " " << j->getTop() << " " << j->getRight() << " " << j->getBottom();
//				toErrorLog("push " + os.str());
//				toErrorLog(j->touches((*i)->getAbsoluteRect()));
				(*i)->blitRectList.push_back(r);
				if((*i)->getDrawType() == SOLID_OBJECT)
				{
					std::list<Rect> t;
					t = j->withoutRect((*i)->getAbsoluteRect());
					oldRectList.splice(j, t);
					j = oldRectList.erase(j);	
				} 
				else ++j;
			} else ++j;
		}
// newrect Maske ueber Objekt legen und Schnitt dem Objekt mitteilen
		for(std::list<std::pair<signed int, Rect> >::iterator j = newRectList.begin(); j != newRectList.end(); )
		// z vergleich unnoetig weil ich ja sortiert von vorne nach hinten durchgehe
		{
			Rect r = j->second.commonRect((*i)->getAbsoluteRect());
//			toErrorLog(".");
			if(r != Rect())
			{
//				toErrorLog("---");
//				toErrorLog(j->second.toString());
//				toErrorLog((*i)->getAbsoluteRect().toString());
//				toErrorLog(r.toString());
				(*i)->blitRectList.push_back(r);
				if((*i)->getDrawType() == SOLID_OBJECT) //?
				{
					std::list<Rect> t;
					t = j->second.withoutRect((*i)->getAbsoluteRect());
					for(std::list<Rect>::iterator l = t.begin(); l != t.end(); ++l)
					{
						newRectList.insert(j, std::pair<signed int, Rect>(j->first, *l));
					}
					j = newRectList.erase(j);
				} else ++j;
			} else ++j;
		}	
	}
	for(std::list<UI_Object*>::const_iterator i = objectList.end(); i != objectList.begin(); )
	{
		--i;
		if((*i)->isShown())
		{
			if(((*i)->getWidth()>0)&&((*i)->getHeight()>0)) // TODO
				(*i)->putOnScreen();
		}
	}

	oldRectList.clear();
	newRectList.clear();
// put it in the graphic memory
	dc->updateScreen();
}

void UI_Object::processAll() // TODO, processList machen
{
	for(std::list<UI_Object*>::const_iterator i = objectList.begin(); i != objectList.end(); ++i)
//		if((*i)->isShown())
			(*i)->process();			
}


void UI_Object::process()
{
//	if(!isShown())
//		return; //?
	if(uiConfiguration.isSmoothMovements())
		relativeRect.moveSmooth(startRect, targetRect);
	else relativeRect.move(targetRect);

	if((oldRect.getSize() != getSize()) || (hasSizeChanged()))
	{
//		adjustSize(CHILD_WAS_CHANGED, getSize()); // TODO ? muesste hier nicht die Groesse des Kinds stehen?
//		if(getParent())
//			getParent()->adjustSize(CHILD_WAS_CHANGED, getSize()); // problematisch TODO
		UI_Object* tmp=children;  // process all children of gadget
		if (tmp) {
			do {
				tmp->adjustSize(PARENT_WAS_CHANGED, getSize());
				tmp->adjustPosition();
				tmp = tmp->nextBrother;
			} while (tmp != children);
		}
		
		adjustPosition();
	}
	checkRectHasChanged();
}

void UI_Object::addNewRectToList(const Rect rect, const unsigned int z_coord)
{
	UI_Object::newRectList.push_back(std::pair<signed int, Rect> (z_coord, rect));
}


void UI_Object::setClipRect(const Rect& rect)
{
	UI_Object* tmp=children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->setClipRect(rect);
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
	clipRect = rect;
}

void UI_Object::checkRectHasChanged()
{
	if((!isShown())||(oldRect == getAbsoluteRect()))
		return;
	sizeHasChanged = (oldRect.getSize() != getSize());
	newRectList.push_back(std::pair<signed int, Rect> (getZ(), getAbsoluteRect()));
	oldRectList.push_back(oldRect);
	oldRect = getAbsoluteRect();
}

void UI_Object::resetData()
{ 
	UI_Object* tmp = children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->resetData();
			tmp = tmp->nextBrother;
																				
		} while (tmp != children);
	}
}

// returns the object the mouse cursor is currently over and owns a tooltip
UI_Object* UI_Object::checkToolTip()
{
	if(!isShown())
		return(NULL);
	UI_Object* tmp=children;  // process all children of gadget
	
	if(!tmp)
		return(NULL); // return 0 as this is an object and no button!
	UI_Object* result=NULL;
	do 
	{
		result = tmp->checkToolTip();
		if((result!=NULL)&&(!result->hasToolTip()))
			result = NULL;
		tmp = tmp->nextBrother;
	} while((tmp!=children)&&(result==NULL));
	
	return(result);
}

UI_Object* UI_Object::checkHighlight()
{
	if(!isShown())
		return(NULL);
	UI_Object* tmp=children;  // process all children of gadget
	if(!tmp)
		return(NULL); // return 0 as this is an object and no button!
	UI_Object* result=NULL;
	do 
	{
		result = tmp->checkHighlight();
		tmp = tmp->nextBrother;
	} while((tmp!=children)&&(result==NULL));
	
	return(result);
}

void UI_Object::object_info()
{
	toErrorLog("ui_object");
}
	
void UI_Object::putOnScreen()
{
	if (!isShown())
		return;

	if(getDrawType() != ANTI_ALIASED_OBJECT)
	{
		bool clear_puffer = false;
		if(sizeHasChanged)
		{
//			toErrorLog("size has changed");
			SDL_FreeSurface(drawPuffer);
			drawPuffer = NULL;
		}
		if(drawPuffer == NULL)
		{
//			toErrorLog("create new surface");
			if((getWidth() == 0) || (getHeight() == 0))
			{
				toErrorLog(":/");
			}
			drawPuffer = SDL_CreateRGBSurface( SDL_SWSURFACE, getWidth(), getHeight(), dc->getBits(), 0,0,0,0);
			if(getDrawType() == TRANSPARENT_OBJECT)
				SDL_SetColorKey(drawPuffer, SDL_SRCCOLORKEY , SDL_MapRGB(drawPuffer->format, 0, 0, 0));
			makePufferInvalid();
		} else 
			if(getDrawType() == TRANSPARENT_OBJECT)
				clear_puffer = true;
		Rect::connectRects(blitRectList);
		if(isPufferInvalid())
		{
//			toErrorLog("puffer invalid");
			if(clear_puffer)
				SDL_FillRect(drawPuffer, NULL, 0);
			dc->switchToSurface(drawPuffer);
		}
	} else 
		dc->drawFromPoint(getAbsolutePosition());
	
	if(isPufferInvalid())
	{
//		toErrorLog("draw");
		if(getDrawType() == ANTI_ALIASED_OBJECT)
			draw();
		else
		{
//			toErrorLog("puffer invalid -> draw");
			draw(); // fill puffer or draw directly to screen
			dc->switchToOriginalSurface();
			makePufferValid();
		}
//		toErrorLog("draw ok");
	}

	if(getDrawType() != ANTI_ALIASED_OBJECT)
	{
//		if(blitRectList.size()>0)
//			object_info();
		for(std::list<Rect>::iterator i = blitRectList.begin(); i != blitRectList.end(); )
		{
//			toErrorLog("blit " + i->toString());
			SDL_Rect source;
			SDL_Rect target;
			target.x = i->getLeft();
			target.y = i->getTop();
			Point p = getAbsolutePosition();
			source.x = i->getLeft() - p.x;
			source.y = i->getTop() - p.y;
			source.w = i->getWidth();
			source.h = i->getHeight();
			SDL_BlitSurface(drawPuffer, &source, dc->getSurface(), &target); // TODO
			DC::addRectangle(*i);
			i = blitRectList.erase(i);
//			toErrorLog("blit ok");
		}
/*		if(blitRectList.size()>0)
		{
			toErrorLog(blitRectList.size());
			SDL_Rect target;
			target.x = getAbsolutePosition().x;
			target.y = getAbsolutePosition().y;
			SDL_BlitSurface(drawPuffer, NULL, dc->getSurface(), &target); // TODO
			DC::addRectangle(getAbsoluteRect());
			for(std::list<Rect>::iterator i = blitRectList.begin(); i != blitRectList.end();)
			{
				toErrorLog("long draw " + i->toString());
				i = blitRectList.erase(i);
			}
		}*/
	} 
	else if(blitRectList.size()>0)
	{ // TODO
		blitRectList.clear();
		DC::addRectangle(getAbsoluteRect());
		dc->drawFromPoint(Point(0,0));
	}
}

void UI_Object::draw() const
{}

	// if hidden, hide children as well
// for 'redraw' optimizations - to see which window is redrawn
//	dc->setPen(*theme.lookUpPen((ePen)(5+(rand()%2))));
//	dc->DrawEmptyRectangle(getAbsoluteRect());

void UI_Object::setZ(const signed int zcoord)
{
	zCoordinate = zcoord;
	UI_Object* tmp = children;
	if (tmp) {
		do {
			tmp->setZ(zcoord - 10);
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
}

void UI_Object::setOriginalRect(const Rect& rect) {
	originalRect = rect;
	setRect(rect);
}

void UI_Object::setOriginalSize(const Size& size) {
	originalRect.setSize(size);
	setSize(size);
}

void UI_Object::setOriginalWidth(const unsigned int width) {
	originalRect.setWidth(width);
	setWidth(width);
}


const bool UI_Object::isMoving() const
{
	return((isShown())&&(relativeRect.getLeft() != targetRect.getLeft()));
}

void UI_Object::setRect(const Rect& rect) 
{
	relativeRect = rect;
	startRect = rect;
	targetRect = rect;
}

void UI_Object::setPosition(const Point& position)
{
	if(position == relativeRect.getTopLeft())
		return;
#ifdef _SCC_DEBUG
	if((position.y > 900000)||(position.x > 900000))
	{
		toErrorLog("DEBUG (UI_Object::setPosition()): Value position out of range.");return;
	}
#endif
	startRect.setTopLeft(position);
	targetRect.setTopLeft(position);
	relativeRect.setTopLeft(position);
}

void UI_Object::setHeight(const unsigned int height) 
{
	if(getTargetHeight() == height)
		return;
	relativeRect.setHeight(height);
	startRect.setHeight(height);
	targetRect.setHeight(height);
}

void UI_Object::setWidth(const unsigned int width) 
{
	if(relativeRect.getWidth() == width)
		return;
	relativeRect.setWidth(width);
	startRect.setWidth(width);
	targetRect.setWidth(width);
}

void UI_Object::setSize(const Size size)
{
	if(relativeRect.getSize() == size)
		return;
	relativeRect.setSize(size);
	startRect.setSize(size);
	targetRect.setSize(size);
}

void UI_Object::setLeft(const signed int x) 
{
	if(relativeRect.getLeft() == x)
		return;
	relativeRect.setLeft(x);
	startRect.setLeft(x);
	targetRect.setLeft(x);
}
		
void UI_Object::setTop(const signed int y) 
{
	if(relativeRect.getTop() == y)
		return;
	relativeRect.setTop(y);
	startRect.setTop(y);
	targetRect.setTop(y);
}

void UI_Object::showRemember(const bool show)
{
	if((show)&&(!shown)&&(oldShown))
	{
		shown = true;
		newRectList.push_back(std::pair<signed int, Rect>(getZ(), getAbsoluteRect()));
		if(getParent())
			getParent()->childrenWereChanged = true;
	} 
	else if((!show)&&(shown))
	{
		oldRectList.push_back(getAbsoluteRect());
		shown = false;
		if(getParent())
			getParent()->childrenWereChanged = true;
		if(UI_Object::focus == this)
			UI_Object::focus = NULL;
	}
	UI_Object* tmp = children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->showRemember(show);
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
}

void UI_Object::Show(const bool show)
{
	if((show)&&(!shown))
	{
		oldShown = true;
		shown = true;
		newRectList.push_back(std::pair<signed int, Rect>(getZ(), getAbsoluteRect()));
		if(getParent())
			getParent()->childrenWereChanged = true;
	} 
	else if((!show)&&(shown))
	{
		oldShown = false;
		shown = false;
		oldRectList.push_back(getAbsoluteRect());
		if(getParent())
			getParent()->childrenWereChanged = true;
		if(UI_Object::focus == this)
			UI_Object::focus = NULL;
	}
	UI_Object* tmp = children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->showRemember(show);
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
}

void UI_Object::redrawWholeObject() {
	if(!isShown())
		return;
	pufferInvalid = true;
	newRectList.push_back(std::pair<signed int, Rect> (getZ(), getAbsoluteRect()));
}

void UI_Object::makePufferInvalid() {
	pufferInvalid = true;
}

void UI_Object::makePufferValid() {
	pufferInvalid = false;
}




void UI_Object::makeFirstChild()
{
	if((!getParent())||((getParent()->children == this)&&(getParent()->children->getNextBrother() == this)))
		return;
	UI_Object* old_parent = getParent();
	removeFromFamily();
	setParent(old_parent);
}

void UI_Object::clearFlags()
{
	sizeHasChanged = false;
}

void UI_Object::clearAllFlags()
{
	for(std::list<UI_Object*>::iterator i = objectList.begin(); i != objectList.end(); ++i)
		(*i)->clearFlags();
}

void UI_Object::addMessage(const eString msg)
{
	remainingMessages.push_back(UI_Object::theme.lookUpString(msg));
}

void UI_Object::addMessage(const std::string& msg)
{
	remainingMessages.push_back(msg);
}


const bool UI_Object::setResolution(const eResolution resolution, const bool first_call)
{
	if((resolution > RESOLUTION_1280x1024) || (resolution < RESOLUTION_640x480))
		return(false);
	if(!first_call)
		SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 0));
	theme.setResolution(resolution);
	if(dc)
		dc->setResolution(theme.getCurrentResolutionSize());
	Size s = theme.getCurrentResolutionSize();
	max_x = s.getWidth();
	max_y = s.getHeight();
	uiConfiguration.setResolution(UI_Object::theme.getResolution());
	std::ostringstream os;os.str("");
	os << max_x << "x" << max_y;
	toInitLog(theme.lookUpFormattedString(CHANGED_RESOLUTION_STRING, os.str()));
	resetWindow();
	if(!first_call)
		SDL_SetCursor(UI_Object::theme.lookUpCursor(ARROW_CURSOR, 0));

	return(true);
}

const bool UI_Object::setBitDepth(const eBitDepth bitdepth)
{
	if((bitdepth > DEPTH_32BIT) || (bitdepth < DEPTH_8BIT))
		return(false);
	SDL_SetCursor(UI_Object::theme.lookUpCursor(CLOCK_CURSOR, 0));
	theme.setBitDepth(bitdepth);
	dc->setBitDepth(theme.getBitDepth());
	uiConfiguration.setBitDepth(theme.getBitDepth());
	theme.updateColors(UI_Object::dc->getSurface());
	// TODO bitDepth im theme aendern! ?
	toInitLog(theme.lookUpFormattedString(CHANGED_BIT_DEPTH_STRING, (unsigned int)dc->getSurface()->format->BitsPerPixel));
	resetWindow();
	SDL_SetCursor(UI_Object::theme.lookUpCursor(ARROW_CURSOR, 0));
	return(true);
}

void UI_Object::resetWindow()
{
	UI_Object::windowSelected = false;
	UI_Object::currentWindow = NULL;
}

unsigned int UI_Object::max_x(0);
unsigned int UI_Object::max_y(0);
Point UI_Object::mouse(Point(0,0));

unsigned int UI_Object::mouseType(0);

UI_Window* UI_Object::currentWindow = NULL;
bool UI_Object::windowSelected = false;
unsigned int UI_Object::redrawnObjects(0);
std::list<std::string> UI_Object::msgList;
std::list<UI_Object*> UI_Object::objectList;

std::list<std::pair<signed int, Rect> > UI_Object::newRectList;
std::list<Rect> UI_Object::oldRectList;

DC* UI_Object::dc = NULL;
std::list<std::string> UI_Object::remainingMessages;
