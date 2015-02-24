#include "HookManager.h"


HookManager::HookManager()
{
}


HookManager::~HookManager()
{
}

HookManager& HookManager::get()
{
	static HookManager instance;
	return instance;
}

std::map<std::string, const char*> hooks;

void HookManager::registerHookParameters(std::string funcName, std::string parameters)
{
	hooks[funcName] = parameters.c_str();
}

const char* HookManager::getParametersForHook(std::string funcName)
{
    std::map<std::string, const char*>::iterator it = hooks.find(funcName);
	return (it == hooks.end()) ? "" : it->second;
}

void HookManager::hookFunction(std::string funcName, std::string parameters)
{

}