#pragma once

#include <igameevents.h>

class HUDEventListener :
	public IGameEventListener2
{
public:
	HUDEventListener(void);
	virtual ~HUDEventListener(void);
	
	// FireEvent is called by EventManager if event just occured
	// KeyValue memory will be freed by manager if not needed anymore
	virtual void FireGameEvent( IGameEvent *event );
};

