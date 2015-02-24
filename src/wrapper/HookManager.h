#ifndef _HOOKMANAGER_H_
#define _HOOKMANAGER_H_

#include <iostream>
#include <map>
#include "AmxHelper.h"
#include "NativeFunctionManager.h"
#include <algorithm>

class HookManager
{
public:
	static HookManager& get();
	
	HookManager();
	~HookManager();

	void registerHookParameters(std::string funcName, std::string parameters);
	void hookFunction(std::string funcName, std::string parameters);
	const char* getParametersForHook(std::string funcName);
private:
	HookManager(const HookManager&) = delete;
	HookManager& operator= (const HookManager&) = delete;
};
#endif