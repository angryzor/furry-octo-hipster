#include "EngiHUD.h"

#include <icliententity.h>

#include "SigScanner.h"
#include "classtools.h"

#include <memdbgon.h>

using namespace angry_hax::classes::vtables;
using namespace angry_hax::classes::data;
using namespace angry_hax::memory;

EngiHUD::EngiHUD(HANDLE process, CreateInterfaceFn interfaceFactory)
{
	void* addr;
	HMODULE client(GetModuleHandle("client.dll"));
	HMODULE engine(GetModuleHandle("engine.dll"));

	Msg(" Initializing Engi HUD interceptor/modifier.\r\n");

	Msg("  Acquiring IClientEntityList reference: ");
	CreateInterfaceFn ci((CreateInterfaceFn)GetProcAddress(client,"CreateInterface"));
	elist_ = (IClientEntityList*)ci(VCLIENTENTITYLIST_INTERFACE_VERSION,NULL);
	Msg("0x%08x\r\n",elist_);

	Msg("  Acquiring ISchemeManager reference: ");
	sm_ = (vgui::ISchemeManager*)interfaceFactory(VGUI_SCHEME_INTERFACE_VERSION,NULL);
	Msg("0x%08x\r\n",sm_);

	Msg("  Initializing CBuildingStatusItem_TeleporterEntrance hooker...\r\n");
	RTTIScanner r(process,client);

	try
	{
		Msg("   RTTI-scanning for CBuildingStatusItem_TeleporterEntrance vtable... ");
		addr = r.FindVTableAddress("CBuildingStatusItem_TeleporterEntrance","CBuildingStatusItem_TeleporterEntrance");
		Msg("ok! (@ 0x%08x)\r\n", (unsigned long)addr);
	}
	catch(logic_error e)
	{
		Warning("FAIL! (%s)\r\n", e.what());
		throw logic_error("Could not initialize CBuildingStatusItem_TeleporterEntrance hooker!");
	}

	vth_ = new VTableHooker(process,client,addr);
	Msg("  CBuildingStatusItem_TeleporterEntrance hooker ready.\r\n");

	Msg("  Initializing CBuildingStatusItem_TeleporterExit hooker...\r\n");
	try
	{
		Msg("   RTTI-scanning for CBuildingStatusItem_TeleporterExit vtable... ");
		addr = r.FindVTableAddress("CBuildingStatusItem_TeleporterExit","CBuildingStatusItem_TeleporterExit");
		Msg("ok! (@ 0x%08x)\r\n", (unsigned long)addr);
	}
	catch(logic_error e)
	{
		Warning("FAIL! (%s)\r\n", e.what());
		throw logic_error("Could not initialize CBuildingStatusItem_TeleporterExit hooker!");
	}

	vth_exit_ = new VTableHooker(process,client,addr);
	Msg("  CBuildingStatusItem_TeleporterExit hooker ready.\r\n");



	SigScanner s(process, client);
	try
	{
		s.Begin();

		Msg("  Sigscanning for vgui::Panel::FindChildByName()... ");
		addr = s.FindUniqueSig(string("81C6D800000083C748E8????????8B4C241050518B4C241C"),0x4A);
		vgui_Panel_FindChildByName = (vgui_Panel_FindChildByName__Type)addr;
		Msg("ok! (@ 0x%08x)\r\n", (unsigned long)addr);

		s.End();
	}
	catch(logic_error e)
	{
		Warning("FAIL! (%s)\r\n", e.what());
		throw logic_error("Could not find vgui::Panel::FindChildByName()!");
	}



	Msg(" Engi HUD interceptor ready.\r\n");
}


EngiHUD::~EngiHUD(void)
{
	delete vth_exit_;
	delete vth_;
}


void EngiHUD::Hook()
{
	try
	{
		Msg(" Placing CBuildingStatusItem_TeleporterEntrance hooks...\r\n");
			Msg("  Hooking CBuildingStatusItem_TeleporterEntrance::PerformLayout() @ vtab+0x%02x... ", PERFORM_LAYOUT);
			True__CBuildingStatusItem_TeleporterEntrance__PerformLayout = vth_->Hook(PERFORM_LAYOUT,&Intercept__CBuildingStatusItem_TeleporterEntrance__PerformLayout);
			Msg("ok!\r\n");
			Msg("  Hooking CBuildingStatusItem_TeleporterExit::PerformLayout() @ vtab+0x%02x... ", PERFORM_LAYOUT);
			True__CBuildingStatusItem_TeleporterExit__PerformLayout = vth_exit_->Hook(PERFORM_LAYOUT,&Intercept__CBuildingStatusItem_TeleporterExit__PerformLayout);
			Msg("ok!\r\n");
		Msg(" CBuildingStatusItem_TeleporterEntrance hooks placed.\r\n");
	}
	catch(logic_error e)
	{
		Warning("FAIL! (%s)\r\n", e.what());
		throw logic_error("Could not place CBuildingStatusItem_TeleporterEntrance hooks!");
	}
}


void EngiHUD::Unhook()
{
	try
	{
		Msg(" Removing CBuildingStatusItem_TeleporterEntrance hooks... ");
		vth_->Unhook(PERFORM_LAYOUT);
		Msg("ok!\r\n");
		Msg(" Removing CBuildingStatusItem_TeleporterExit hooks... ");
		vth_exit_->Unhook(PERFORM_LAYOUT);
		Msg("ok!\r\n");
	}
	catch(logic_error e)
	{
		Warning("FAIL! (%s)\r\n", e.what());
		throw logic_error("Could not remove CBuildingStatusItem_TeleporterEntrance hooks! Your memory is corrupt now. Have fun crashing.");
	}
}

vgui::Panel* EngiHUD::GetChild(vgui::Panel* thisptr,const char* name)
{
		vgui::Panel* r(vgui_Panel_FindChildByName(thisptr,0,name,true));
		if(!r)
			throw logic_error(string("Could not access subpanel \"") + name + "\"");
		return r;
}

void __fastcall EngiHUD::Intercept__CBuildingStatusItem_TeleporterEntrance__PerformLayout(void* thisptr, void* edx)
{
	True__CBuildingStatusItem_TeleporterEntrance__PerformLayout(thisptr,edx);
	if(!thisptr) return;

	ten_ = (vgui::Panel*)thisptr;

	vgui::HScheme s(ten_->GetScheme());
	int w(sm_->GetProportionalScaledValueEx(s,150)),h(sm_->GetProportionalScaledValueEx(s,40));
	
	CBaseHandle& entTele(GetMemberAtOffset<CBaseHandle>(thisptr, 0x1B8));

	try
	{
		vgui::Panel* lb1(GetChild(ten_,"LevelBox1"));
		vgui::Panel* lb2(GetChild(ten_,"LevelBox2"));
		vgui::Panel* lb3(GetChild(ten_,"LevelBox3"));
	/*	vgui::Panel* mi(GetChild(ten_,"MetalIcon"));
		vgui::Panel* u(GetChild(ten_,"Upgrade"));
		vgui::Panel* cp(GetChild(ten_,"ChargingPanel"));
		vgui::Panel* fcp(GetChild(ten_,"FullyChargedPanel"));*/

		lb1->SetVisible(false);
		lb2->SetVisible(false);
		lb3->SetVisible(false);

		if(entTele != -1)
		{
			IClientEntity* e(elist_->GetClientEntityFromHandle(entTele));

			if(!e)
				throw logic_error("Could not access teleporter entrance entity");

			int upglvl(GetMemberAtOffset<int>(e,0xE04));

			ten_->SetSize(w,h);

			if(upglvl == 1) lb1->SetVisible(true);
			else if(upglvl == 2) lb2->SetVisible(true);
			else if(upglvl == 3) lb3->SetVisible(true);
		}
	}
	catch(logic_error e)
	{
		Warning("Cryan HUD: EngiHUD: ERROR: %s\r\n",e.what());
	}
}

void __fastcall EngiHUD::Intercept__CBuildingStatusItem_TeleporterExit__PerformLayout(void* thisptr, void* edx)
{
	True__CBuildingStatusItem_TeleporterExit__PerformLayout(thisptr,edx);
	if(!thisptr || !ten_) return;

	vgui::Panel* tex((vgui::Panel*)thisptr);

	vgui::HScheme s(ten_->GetScheme());
	int w(sm_->GetProportionalScaledValueEx(s,150)),h(sm_->GetProportionalScaledValueEx(s,40));

	CBaseHandle& entTele(GetMemberAtOffset<CBaseHandle>(thisptr, 0x1B8));

	try
	{
		vgui::Panel* rp(GetChild(tex,"RunningPanel"));
		vgui::Panel* lb1(GetChild(ten_,"LevelBoxExit1"));
		vgui::Panel* lb2(GetChild(ten_,"LevelBoxExit2"));
		vgui::Panel* lb3(GetChild(ten_,"LevelBoxExit3"));

		vgui::Panel* rpn(GetChild(ten_,"RunningPanel"));
		vgui::Panel* lbn1(GetChild(ten_,"LevelBox1"));
		vgui::Panel* lbn2(GetChild(ten_,"LevelBox2"));
		vgui::Panel* lbn3(GetChild(ten_,"LevelBox3"));

		lb1->SetVisible(false);
		lb2->SetVisible(false);
		lb3->SetVisible(false);

		if(entTele != -1)
		{
			IClientEntity* e(elist_->GetClientEntityFromHandle(entTele));

			if(!e)
				throw logic_error("Could not access teleporter entrance entity");

			int upglvl(GetMemberAtOffset<int>(e,0xE04));

			if(!rpn->IsVisible() && rp->IsVisible())
			{
				ten_->SetSize(w,h);

				if(upglvl == 1) lb1->SetVisible(true);
				else if(upglvl == 2) lb2->SetVisible(true);
				else if(upglvl == 3) lb3->SetVisible(true);
			}
		}
	}
	catch(logic_error e)
	{
		Warning("Cryan HUD: EngiHUD: ERROR: %s\r\n",e.what());
	}
}

EngiHUD::CBuildingStatusItem_TeleporterEntrance__PerformLayout__Type	EngiHUD::True__CBuildingStatusItem_TeleporterEntrance__PerformLayout	= NULL;
EngiHUD::CBuildingStatusItem_TeleporterExit__PerformLayout__Type		EngiHUD::True__CBuildingStatusItem_TeleporterExit__PerformLayout		= NULL;
EngiHUD::vgui_Panel_FindChildByName__Type								EngiHUD::vgui_Panel_FindChildByName										= NULL;

IClientEntityList* EngiHUD::elist_	= NULL;
vgui::ISchemeManager* EngiHUD::sm_	= NULL;
vgui::Panel* EngiHUD::ten_			= NULL;