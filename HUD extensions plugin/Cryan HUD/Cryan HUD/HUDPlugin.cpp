#include <tier2/tier2.h>
#include <tier3/tier3.h>
#include <stdexcept>

#include "HUDPlugin.h"

#include <tier0/memdbgon.h>

using namespace std;

CHUDPlugin g_HUDPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CSteamIRCPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_HUDPlugin );

//static ConVar irc_nick("irc_nickname", "IIzPro", 0, "The nickname you will be using on IRC servers.");

CHUDPlugin::CHUDPlugin(void)
{
}

CHUDPlugin::~CHUDPlugin(void)
{
}

bool CHUDPlugin::Load(	CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory )
{
	// Connect tiers
	ConnectTier1Libraries( &interfaceFactory, 1 );
	ConnectTier2Libraries( &interfaceFactory, 1 );
	ConnectTier3Libraries( &interfaceFactory, 1 );

	Msg("================== The Visual Improve HUD Extensions v0.1 =================\r\n");
	Msg(" Build 0001\r\n");
	Msg(" Written by angryzor\r\n");
	Msg("--------------------------------------------------------------\r\n");
	Msg(" Booting...\r\n");

	try
	{
		hdnh_ = new DeathNotices(GetCurrentProcess(), interfaceFactory);
		hdnh_->Hook();

		ehud_ = new EngiHUD(GetCurrentProcess(), interfaceFactory);
		ehud_->Hook();

		Msg(" Registering ConVars...\r\n");
		ConVar_Register( 0 );
		Msg(" Boot successful! HUD Extensions ready.\r\n");
		Msg("===========================================================================\r\n");
		return true;
	}
	catch(logic_error e)
	{
		Warning(" Boot FAIL! (%s)\r\n", e.what());
		DisconnectTier3Libraries( );
		DisconnectTier2Libraries( );
		DisconnectTier1Libraries( );
		return false;
	}
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unloaded (turned off)
//---------------------------------------------------------------------------------
void CHUDPlugin::Unload( void )
{
	Msg("================ The Visual Improve HUD Extensions quitting ===============\r\n");
	try
	{
		ehud_->Unhook();
		delete ehud_;

		hdnh_->Unhook();
		delete hdnh_;
	}
	catch(logic_error e)
	{
		Warning(e.what());
	}
	Msg("===========================================================================\r\n");

	ConVar_Unregister( );
	DisconnectTier3Libraries( );
	DisconnectTier2Libraries( );
	DisconnectTier1Libraries( );
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is paused (i.e should stop running but isn't unloaded)
//---------------------------------------------------------------------------------
void CHUDPlugin::Pause( void )
{
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unpaused (i.e should start executing again)
//---------------------------------------------------------------------------------
void CHUDPlugin::UnPause( void )
{
}

//---------------------------------------------------------------------------------
// Purpose: the name of this plugin, returned in "plugin_print" command
//---------------------------------------------------------------------------------
const char *CHUDPlugin::GetPluginDescription( void )
{
	return "The Visual Improve HUD Extensions";
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CHUDPlugin::LevelInit( char const *pMapName )
{
}

//---------------------------------------------------------------------------------
// Purpose: called on level start, when the server is ready to accept client connections
//		edictCount is the number of entities in the level, clientMax is the max client count
//---------------------------------------------------------------------------------
void CHUDPlugin::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
}

//---------------------------------------------------------------------------------
// Purpose: called once per server frame, do recurring work here (like checking for timeouts)
//---------------------------------------------------------------------------------
void CHUDPlugin::GameFrame( bool simulating )
{
}

//---------------------------------------------------------------------------------
// Purpose: called on level end (as the server is shutting down or going to a new map)
//---------------------------------------------------------------------------------
void CHUDPlugin::LevelShutdown( void ) // !!!!this can get called multiple times per map change
{
}

//---------------------------------------------------------------------------------
// Purpose: called when a client spawns into a server (i.e as they begin to play)
//---------------------------------------------------------------------------------
void CHUDPlugin::ClientActive( edict_t *pEntity )
{
}

//---------------------------------------------------------------------------------
// Purpose: called when a client leaves a server (or is timed out)
//---------------------------------------------------------------------------------
void CHUDPlugin::ClientDisconnect( edict_t *pEntity )
{
}

//---------------------------------------------------------------------------------
// Purpose: called on 
//---------------------------------------------------------------------------------
void CHUDPlugin::ClientPutInServer( edict_t *pEntity, char const *playername )
{
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CHUDPlugin::SetCommandClient( int index )
{
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CHUDPlugin::ClientSettingsChanged( edict_t *pEdict )
{
}

//---------------------------------------------------------------------------------
// Purpose: called when a client joins a server
//---------------------------------------------------------------------------------
PLUGIN_RESULT CHUDPlugin::ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a client types in a command (only a subset of commands however, not CON_COMMAND's)
//---------------------------------------------------------------------------------
PLUGIN_RESULT CHUDPlugin::ClientCommand( edict_t *pEntity, const CCommand &args )
{
	return PLUGIN_CONTINUE; 
}

//---------------------------------------------------------------------------------
// Purpose: called when a client is authenticated
//---------------------------------------------------------------------------------
PLUGIN_RESULT CHUDPlugin::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
{
	return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a cvar value query is finished
//---------------------------------------------------------------------------------
void CHUDPlugin::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
{
}

void CHUDPlugin::OnEdictAllocated( edict_t *edict )
{
}

void CHUDPlugin::OnEdictFreed( const edict_t *edict )
{
}

