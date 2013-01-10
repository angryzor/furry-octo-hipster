#pragma once
#include "RTTIScanner.h"
#include "VTableHooker.h"

#include <Windows.h>
#include <tier0/dbg.h>
#include <iserverplugin.h>
#include <vgui_controls/Panel.h>
#include <icliententitylist.h>
#include <vgui/IScheme.h>


class EngiHUD
{
public:
	EngiHUD(HANDLE process, CreateInterfaceFn interfaceFactory);
	virtual ~EngiHUD(void);

	void Hook();
	void Unhook();
protected:
	angry_hax::classes::vtables::VTableHooker* vth_;
	angry_hax::classes::vtables::VTableHooker* vth_exit_;
	enum vtable_offsets_CBuildingStatusItem {
		PERFORM_LAYOUT = 0x80
	};

	class CIconPanel {
		char padding[0x170]; // Size of CIconPanel
	};

	static IClientEntityList* elist_;
	static vgui::ISchemeManager* sm_;

	static vgui::Panel* GetChild(vgui::Panel* thisptr,const char* name);


	static vgui::Panel* ten_;


	typedef void (__fastcall *CBuildingStatusItem_TeleporterEntrance__PerformLayout__Type)(void*, void*);
	typedef void (__fastcall *CBuildingStatusItem_TeleporterExit__PerformLayout__Type)(void*, void*);
	typedef vgui::Panel* (__fastcall *vgui_Panel_FindChildByName__Type)(vgui::Panel*, void*, const char*, bool);


	static void __fastcall Intercept__CBuildingStatusItem_TeleporterEntrance__PerformLayout(void* thisptr, void* edx);
	static CBuildingStatusItem_TeleporterEntrance__PerformLayout__Type True__CBuildingStatusItem_TeleporterEntrance__PerformLayout;

	static void __fastcall Intercept__CBuildingStatusItem_TeleporterExit__PerformLayout(void* thisptr, void* edx);
	static CBuildingStatusItem_TeleporterExit__PerformLayout__Type True__CBuildingStatusItem_TeleporterExit__PerformLayout;

	static vgui_Panel_FindChildByName__Type vgui_Panel_FindChildByName;
};

