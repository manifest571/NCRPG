#include <sourcemod>
#include <sdktools>
#include "NCIncs/nc_rpg.inc"
#include <clientprefs>

#pragma semicolon 1
#pragma tabsize 4

public Plugin myinfo = {
	name = "[NC RPG] Bonus",
	author = "Manifest"
};

#define ThisSkillShortName "bonus"

int cfg_minAmount = 20;
int cfg_maxAmount = 100;
int g_iLastBonus[MAXPLAYERS+1];

Handle g_hCookie;

public void OnPluginStart()
{
	g_hCookie = RegClientCookie("ncrpg_bonus", "", CookieAccess_Private);
	RegConsoleCmd("sm_bonus", Cmd_Bonus);
}

public Action Cmd_Bonus(int iClient, int iArgs)
{
	if (GetTime() > g_iLastBonus[iClient])
	{
		int iRand = GetRandomInt(cfg_minAmount, cfg_maxAmount);
		NCRPG_SetCredits(iClient, NCRPG_GetCredits(iClient)+iRand);
		PrintToChat(iClient, "\x07FFFFFFВы получили бонус\x07FF0000: %d кредитов.", iRand);
		char sTime[64];
		IntToString(GetTime()+86400, sTime, sizeof sTime);
		SetClientCookie(iClient, g_hCookie, sTime);
		g_iLastBonus[iClient] = GetTime()+86400;
	}
	else {
		int iTime = g_iLastBonus[iClient] - GetTime();
		int time[3];
		time[0] = iTime / 3600;
		time[1] = (iTime % 3600) / 60;
		time[2] = (iTime % 3600) % 60;
		PrintToChat(iClient, "\x07FFFFFFСледующий бонус будет доступен только через\x07FF0000: %d ч. %d мин. %d сек.", time[0], time[1], time[2]);
	}
	return Plugin_Handled;
}

public void OnMapStart()
{
	NCRPG_Configs RPG_Configs = NCRPG_Configs(ThisSkillShortName,CONFIG_ADDON);
	cfg_minAmount = RPG_Configs.GetInt(ThisSkillShortName,"min_credits",20);
	cfg_maxAmount = RPG_Configs.GetInt(ThisSkillShortName,"max_credits",100);
	RPG_Configs.SaveConfigFile(ThisSkillShortName,CONFIG_ADDON);
}

public void OnClientCookiesCached(int iClient)
{
	char szValue[16];
	GetClientCookie(iClient, g_hCookie, szValue, sizeof(szValue));
	if(szValue[0])
		g_iLastBonus[iClient] = StringToInt(szValue);
    else
        g_iLastBonus[iClient] = 0;
}
