#include <string>

#include "HUDEventListener.h"

#include <memdbgon.h>

using namespace std;

HUDEventListener::HUDEventListener(void)
{
}


HUDEventListener::~HUDEventListener(void)
{
}


void HUDEventListener::FireGameEvent( IGameEvent *event )
{
	Msg("Object died.\r\nuserid: %d\r\nattacker: %d\r\nassister: %d\r\nweapon: %s\r\nweaponid: %d\r\n", event->GetInt("userid"), event->GetInt("attacker"), event->GetInt("assister"), event->GetString("weapon"), event->GetInt("weaponid"));
}
