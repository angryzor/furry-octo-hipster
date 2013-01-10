#pragma once
#include "VTableHooker.h"

#include <Windows.h>
#include <igameevents.h>

class DeathNotices
{
public:
	DeathNotices(HANDLE process, CreateInterfaceFn interfaceFactory);
	virtual ~DeathNotices(void);
	
	void Hook();
	void Unhook();
protected:
	angry_hax::classes::vtables::VTableHooker* vth_;
	angry_hax::classes::vtables::VTableHooker* vth_gem_;
	enum CTFHudDeathNotice_vtable_offset
	{
		FIRE_GAME_EVENT			= 0x01,
		ON_GAME_EVENT			= 0x0F
	};
	enum IGameEventManager2_vtable_offset
	{
		FIRE_EVENT				= 0x07,
		FIRE_EVENT_CLIENTSIDE	= 0x08
	};

private:
	struct DeathNoticeItem
	{
		unsigned long	unknown_0[34];
		char			texture_name[128]; // Don't know the exact size. This should suffice.
	};

	static bool __fastcall Intercept__IGameEventManager2__FireEvent(void* thisptr, void* edx, IGameEvent* e, bool bDontBroadcast = false);
	static bool (__fastcall *True__IGameEventManager2__FireEvent)(void* thisptr, void* edx, IGameEvent* e, bool bDontBroadcast);

	static bool __fastcall Intercept__IGameEventManager2__FireEventClientSide(void* thisptr, void* edx, IGameEvent* e);
	static bool (__fastcall *True__IGameEventManager2__FireEventClientSide)(void* thisptr, void* edx, IGameEvent* e);

	static void __fastcall Intercept__CTFHudDeathNotice__FireGameEvent(void* thisptr, void* edx, IGameEvent* e);
	static void (__fastcall *True__CTFHudDeathNotice__FireGameEvent)(void* thisptr, void* edx, IGameEvent* e);

	static void __fastcall Intercept__CTFHudDeathNotice__OnGameEvent(void* thisptr, void* edx, IGameEvent* e, DeathNoticeItem& item);
	static void (__fastcall *True__CTFHudDeathNotice__OnGameEvent)(void* thisptr, void* edx, IGameEvent* e, DeathNoticeItem& item);
};

