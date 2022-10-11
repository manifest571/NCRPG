
stock int GetConfigValue(KeyValues hConfig,const char[] section, const char[] key,int defvalue = 0,bool rewrite = true) {
	if(hConfig == INVALID_HANDLE)
		return 0;

	hConfig.Rewind();
	if(hConfig.JumpToKey(section,rewrite))
	{
		int newvalue = hConfig.GetNum(key, defvalue);
		if(rewrite) hConfig.SetNum(key,defvalue);
		hConfig.SetNum(key, newvalue);
		return newvalue;
	}
	return defvalue;
}

stock float GetConfigValueFloat(KeyValues hConfig,const char[] section, const char[] key, float defvalue = 0.0,bool rewrite = true) {
	if(hConfig == INVALID_HANDLE)
		return 0.0;

	hConfig.Rewind();
	if(hConfig.JumpToKey(section,rewrite))
	{
		float newvalue = hConfig.GetFloat(key, defvalue);
		if(rewrite) hConfig.SetFloat(key,defvalue);
		hConfig.SetFloat(key,newvalue);
		return newvalue;
	}
	return defvalue;
}

stock bool GetConfigValueString(KeyValues hConfig,const char[] section, const char[] key, char[] buffer,int maxlength, const char[] defvalue = "",bool rewrite = true) {
	if(hConfig == INVALID_HANDLE)
	{
		Format(buffer, maxlength, "ERROR");
		return false;
	}
	
	hConfig.Rewind();
	if(hConfig.JumpToKey(section,rewrite))
	{
		hConfig.GetString( key, buffer, maxlength, defvalue);
		if(rewrite) hConfig.SetString(key,defvalue);
		hConfig.SetString(key,buffer);
		return true;
	}
	strcopy(buffer, maxlength, defvalue);	
	return false;
}


KeyValues NCRPG_LoadConfigFile(char[] filename,char[] config_type = CONFIG_NULL,char[] config_format = CONFIG_TXT, bool create = true)
{
	KeyValues hConfig = new KeyValues("ncrpg");
	char full_path[PLATFORM_MAX_PATH];
	
	
	FormatEx(full_path,sizeof full_path,"configs/ncrpg/");
	BuildPath(Path_SM, full_path, sizeof full_path, full_path);
	if (!DirExists(full_path))
	{
		CreateDirectory(full_path, FPERM_U_READ|FPERM_U_WRITE|FPERM_U_EXEC|FPERM_G_READ|FPERM_G_WRITE|FPERM_O_READ|FPERM_O_WRITE);
		PrintToServer("[NC RPG] Root directory created!", full_path);
	}
	
	FormatEx(full_path,sizeof full_path,"configs/ncrpg/%s%s%s",config_type,filename,config_format);
	BuildPath(Path_SM, full_path, sizeof full_path, full_path);
	
	hConfig.Rewind();
	if (FileExists(full_path))
		hConfig.ImportFromFile(full_path);
	else {
		if (create)
			hConfig.ExportToFile(full_path);
	}
	return hConfig;
}

stock void NCRPG_SaveConfigFile(KeyValues hConfig,char[] filename,char[] config_type = CONFIG_NULL,char[] config_format = CONFIG_TXT,close = true)
{
	char full_path[512];
	FormatEx(full_path,sizeof full_path,"configs/ncrpg/%s%s%s",config_type,filename,config_format);
	BuildPath(Path_SM, full_path, sizeof full_path, full_path);
	
//	PrintToServer("[DEBUG] Save %s", full_path);
	hConfig.Rewind();//hConfig.GoBack(); // какой GoBack? При сохранении делают Rewind
	hConfig.ExportToFile(full_path);
	
	if(close) 
		delete hConfig;
}

File CreateFile(const char[] path, const char[] mode = "w+")
{
	char dir[12][PLATFORM_MAX_PATH]; char splitter[2] = "/"; 
	int count = FindCharInString(path, splitter[0], false);
	if(count==-1) splitter = "\\";
	count = ExplodeString(path, splitter, dir, 12, sizeof(dir[]));
	for(int i = 0; i < count-1; i++)
	{
		if(i > 0)
			Format(dir[i], sizeof(dir[]), "%s%s%s", dir[i-1],splitter, dir[i]);
			
		if(!DirExists(dir[i]))
			if(!CreateDirectory(dir[i], FPERM_U_READ|FPERM_U_WRITE|FPERM_U_EXEC|FPERM_G_READ|FPERM_G_WRITE|FPERM_O_READ|FPERM_O_WRITE)) NCRPG_LogMessage(LogType_Error,"Не удалось создать путь %s",dir[i]);
	}
	
	return OpenFile(path, mode);
}


methodmap NCRPG_Configs
{
	public NCRPG_Configs(char[] filename,char[] config_type = CONFIG_NULL,char[] config_format = CONFIG_TXT, bool create=true)
	{
		return view_as<NCRPG_Configs>(NCRPG_LoadConfigFile(filename,config_type,config_format,create));
	}
	property KeyValues Kv
	{
		public get() { return view_as<KeyValues>(this); }
	}
	
	public int GetInt(const char[] section, const char[] key,int defvalue = 0,bool rewrite = true)
	{
		return GetConfigValue(this.Kv,section,key,defvalue,rewrite);
	}	
	public float GetFloat(const char[] section, const char[] key,float defvalue = 0.0,bool rewrite = true)
	{
		return GetConfigValueFloat(this.Kv,section,key,defvalue,rewrite);
	}	
	public bool GetString(const char[] section, const char[] key,char[] buffer,int maxlength, const char[] defvalue = "",bool rewrite = true)
	{
		return GetConfigValueString(this.Kv,section,key,buffer,maxlength,defvalue,rewrite);
	}
	public void SaveConfigFile(char[] filename,char[] config_type = CONFIG_NULL,char[] config_format = CONFIG_TXT,bool close = true)
	{
		NCRPG_SaveConfigFile(this.Kv,filename,config_type,config_format,close);
	}
};
