#include "DeathNotices.h"
#include "RTTIScanner.h"

#include <memdbgon.h>

using namespace angry_hax::classes::vtables;

DeathNotices::DeathNotices(HANDLE process, CreateInterfaceFn interfaceFactory)
{
	void* addr;
	IGameEventManager2* gem;
	HMODULE client(GetModuleHandle("client.dll"));
	HMODULE engine(GetModuleHandle("engine.dll"));

	Msg(" Initializing Death Notice interceptor/modifier.\r\n");

/*	Msg(" Acquiring IGameEventManager2 reference: ");
	gem = (IGameEventManager2 *)interfaceFactory(INTERFACEVERSION_GAMEEVENTSMANAGER2,NULL);
	Msg("0x%08x\r\n",gem);
*/
	Msg("  Initializing CTFHudDeathNotice hooker...\r\n");
	angry_hax::classes::vtables::RTTIScanner r(process,client);

	try
	{
		Msg("   RTTI-scanning for CTFHudDeathNotice vtable... ");
		addr = r.FindVTableAddress("CTFHudDeathNotice","CTFHudDeathNotice");
		Msg("ok! (@ 0x%08x)\r\n", (unsigned long)addr);

	}
	catch(logic_error e)
	{
		Warning("FAIL! (%s)\r\n", e.what());
		throw logic_error("Could not initialize CTFHudDeathNotice hooker!");
	}

	vth_		= new VTableHooker(process,client,addr);
//	vth_gem_	= new VTableHooker(process,engine,*((void**)gem));

	Msg("  CTFHudDeathNotice hooker ready.\r\n");

	Msg(" Death Notice interceptor ready.\r\n");
}


DeathNotices::~DeathNotices(void)
{
//	delete vth_gem_;
	delete vth_;
}

void DeathNotices::Hook()
{
	try
	{
		Msg(" Placing CTFHudDeathNotice hooks...\r\n");
/*			Msg("  Hooking CTFHUDDeathNotice::FireGameEvent() @ vtab+0x%02x... ", FIRE_GAME_EVENT);
			True__CTFHudDeathNotice__FireGameEvent = vth_->Hook(FIRE_GAME_EVENT,&Intercept__CTFHudDeathNotice__FireGameEvent);
			Msg("ok!\r\n");*/
			Msg("  Hooking CTFHUDDeathNotice::OnGameEvent() @ vtab+0x%02x... ", ON_GAME_EVENT);
			True__CTFHudDeathNotice__OnGameEvent = vth_->Hook(ON_GAME_EVENT,&Intercept__CTFHudDeathNotice__OnGameEvent);
			Msg("ok!\r\n");
		Msg(" CTFHudDeathNotice hooks placed.\r\n");
	}
	catch(logic_error e)
	{
		Warning("FAIL! (%s)\r\n", e.what());
		throw logic_error("Could not place CTFHudDeathNotice hooks!");
	}
/*	try
	{
		Msg(" Placing IGameEventManager2 hooks...\r\n");
			Msg("  Hooking CTFHUDDeathNotice::FireEvent() @ vtab+0x%02x... ", FIRE_EVENT_CLIENTSIDE);
			True__IGameEventManager2__FireEventClientSide = vth_gem_->Hook(FIRE_EVENT_CLIENTSIDE,&Intercept__IGameEventManager2__FireEventClientSide);
			Msg("ok!\r\n");
		Msg(" IGameEventManager2 hooks placed.\r\n");
	}
	catch(logic_error e)
	{
		Warning("FAIL! (%s)\r\n", e.what());
		throw logic_error("Could not place IGameEventManager2 hooks!");
	}*/
}


void DeathNotices::Unhook()
{
/*	try
	{
		Msg(" Removing IGameEventManager2 hooks... ");
		vth_gem_->Unhook(FIRE_EVENT_CLIENTSIDE);
		Msg("ok!\r\n");
	}
	catch(logic_error e)
	{
		Warning("FAIL! (%s)\r\n", e.what());
		throw logic_error("Could not remove IGameEventManager2 hooks! Your memory is corrupt now. Have fun crashing.");
	}*/
	try
	{
		Msg(" Removing CTFHudDeathNotice hooks... ");
//		vth_->Unhook(FIRE_GAME_EVENT);
		vth_->Unhook(ON_GAME_EVENT);
		Msg("ok!\r\n");
	}
	catch(logic_error e)
	{
		Warning("FAIL! (%s)\r\n", e.what());
		throw logic_error("Could not remove CTFHudDeathNotice hooks! Your memory is corrupt now. Have fun crashing.");
	}
}


void __fastcall DeathNotices::Intercept__CTFHudDeathNotice__FireGameEvent(void* thisptr, void* edx, IGameEvent* e)
{
/*	if(string("player_death") == e->GetName())
	{
		Msg("userid: %d\r\nattacker: %d\r\nassister: %d\r\nweapon: %s\r\nweaponid: %d\r\nweapon_logclassname: %s\r\ncustomkill: %d\r\ndamagebits: %x\r\ndeath_flags: %x\r\n",
			e->GetInt("userid"),
			e->GetInt("attacker"),
			e->GetInt("assister"),
			e->GetString("weapon"),
			e->GetInt("weaponid"),
			e->GetString("weapon_logclassname"),
			e->GetInt("customkill"),
			e->GetInt("damagebits"),
			e->GetInt("death_flags"));
	}
	if(string("object_destroyed") == e->GetName())
	{
		Warning("userid: %d\r\nattacker: %d\r\nassister: %d\r\nweapon: %s\r\nweaponid: %d\r\n",
			e->GetInt("userid"),
			e->GetInt("attacker"),
			e->GetInt("assister"),
			e->GetString("weapon"),
			e->GetInt("weaponid"));
	}
	*/
	True__CTFHudDeathNotice__FireGameEvent(thisptr, edx, e);
}

void __fastcall DeathNotices::Intercept__CTFHudDeathNotice__OnGameEvent(void* thisptr, void* edx, IGameEvent* e, DeathNoticeItem& item)
{
	if(string("player_death") == e->GetName())
	{
		string str(e->GetString("weapon_logclassname"));

		True__CTFHudDeathNotice__OnGameEvent(thisptr,edx,e,item);

		// Overwrite "texture to display"
		if(		str == "force_a_nature"
			||	str == "blutsauger"
			||	str == "natascha"    )
			strcpy(item.texture_name, (string("d_") + str).c_str());
		else if(str == "backburner" && e->GetInt("userid") != e->GetInt("attacker"))
		{
			if(e->GetInt("damagebits") & 0x100000)
				strcpy(item.texture_name, "d_backburner_back");
			else
				strcpy(item.texture_name, "d_backburner");
		}
		//strcpy(item.texture_name, "d_building_destroy");

	}
	else  // Sadly, this little trick doesnot work on object destructions. Need to find a different way.
		True__CTFHudDeathNotice__OnGameEvent(thisptr,edx,e,item);
}

bool __fastcall DeathNotices::Intercept__IGameEventManager2__FireEvent(void* thisptr, void* edx, IGameEvent* e, bool bDontBroadcast)
{
	Warning("Event captured: %s\r\n",e->GetName());
	return True__IGameEventManager2__FireEvent(thisptr,edx,e, bDontBroadcast);
}

bool __fastcall DeathNotices::Intercept__IGameEventManager2__FireEventClientSide(void* thisptr, void* edx, IGameEvent* e)
{
	if(string(e->GetName()).find(string("object")) != string::npos)
		Warning("Event captured CS: %s\r\n",e->GetName());
	return True__IGameEventManager2__FireEventClientSide(thisptr,edx,e);
}

void (__fastcall *DeathNotices::True__CTFHudDeathNotice__FireGameEvent)(void* thisptr, void* edx, IGameEvent* e)						= NULL;
void (__fastcall *DeathNotices::True__CTFHudDeathNotice__OnGameEvent)(void* thisptr, void* edx, IGameEvent* e, DeathNoticeItem& item)	= NULL;
bool (__fastcall *DeathNotices::True__IGameEventManager2__FireEvent)(void* thisptr, void* edx, IGameEvent* e, bool bDontBroadcast)		= NULL;
bool (__fastcall *DeathNotices::True__IGameEventManager2__FireEventClientSide)(void* thisptr, void* edx, IGameEvent* e)					= NULL;
