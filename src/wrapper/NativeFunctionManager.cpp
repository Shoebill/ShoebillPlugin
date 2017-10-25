#include "NativeFunctionManager.h"

static std::map<std::string, HookedNative *> natives;

NativeFunctionManager::NativeFunctionManager()
{

}

NativeFunctionManager::~NativeFunctionManager()
{

}

NativeFunctionManager &NativeFunctionManager::GetInstance()
{
    static NativeFunctionManager instance;
    return instance;
}

void NativeFunctionManager::RegisterFunction(AMX *amx, std::string name, AMX_NATIVE functionAddr, int index)
{
    HookFunction(amx, functionAddr, name.c_str(), index);
}

/*std::string NativeFunctionManager::getFunctionName(int index)
{
	std::unordered_map<int, std::string>::iterator it = functionNames.find(index);
	return (it == functionNames.end()) ? "" : it->second;
}

AMX_NATIVE NativeFunctionManager::findFunction(char const *name)
{
	std::unordered_map<std::string, AMX_NATIVE>::iterator it = functions.find(name);
	return it != functions.end() ? it->second : nullptr;
}

std::map<std::string, HookedNative *> NativeFunctionManager::getNatives()
{
	return natives;
}*/