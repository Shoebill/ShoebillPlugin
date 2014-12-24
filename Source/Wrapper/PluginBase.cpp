#include "SimpleInlineHook.hpp"
#include "AmxHelper.h"
#include "Callbacks.h"

#include "AmxInstanceManager.hpp"
#include "NativeFunctionManager.h"

logprintf_t logprintf = NULL;

amx_Exec_t _amx_Exec = nullptr;
amx_Register_t _amx_Register = nullptr;
amx_FindPublic_t _amx_FindPublic = nullptr;

SimpleInlineHook _amx_Exec_hook;
SimpleInlineHook _amx_Register_hook;
SimpleInlineHook _amx_FindPublic_hook;

std::map<int, std::string> callbackMap;

cell *amx_param_get_start(AMX *amx) { //sampgdk
	unsigned char *data = amx->data != NULL
		? amx->data
		: amx->base + ((AMX_HEADER *)amx->base)->dat;
	return (cell *)(data + amx->stk);
}

int AMXAPI amx_Exec_hooked(AMX *amx, cell *retval, int index)
{
	AmxInstanceManager::get().registerAmx(amx);
	if (AmxInstanceManager::get().getMainAmx() == nullptr) {
		StartShoebill();
		if (index == AMX_EXEC_MAIN) {
			AmxInstanceManager::get().markMainAmx(amx);
		}
	}
	cell params[33];
	params[0] = amx->paramcount * sizeof(cell);
	cell* adress = amx_param_get_start(amx);
	memcpy(&params[1], adress, params[0]);
	memcpy(adress, &params[1], params[0]);
	std::map<int, std::string>::iterator it = callbackMap.find(index);
	bool executedJava = false;
	if (it != callbackMap.end())
	{
		if (!strcmp(it->second.c_str(), "OnPlayerCommandText"))
		{
			int ret = invokeCallback(amx, it->second, params);
			executedJava = true;
			if (ret == 1) {
				*retval = ret;
				return 1;
			}
		}
	}
	_amx_Exec_hook.unhook();
	int ret = _amx_Exec(amx, retval, index);
	_amx_Exec_hook.hook();
	if (it != callbackMap.end())
	{
		if (!executedJava) {
			invokeCallback(amx, it->second, params);
		}
	}
	return ret;
}

int AMXAPI amx_Register_hooked(AMX *amx, const AMX_NATIVE_INFO *nativelist, int number)
{
	_amx_Register_hook.unhook();
	int ret = _amx_Register(amx, nativelist, number);
	_amx_Register_hook.hook();
	int index;
	for (int i = 0; (number >= 0 && i < number) || nativelist[i].name != nullptr; i++) {
		amx_FindNative(amx, nativelist[i].name, &index);
		NativeFunctionManager::get().registerFunction(amx, nativelist[i].name, nativelist[i].func, index);
	}
	return ret;
}

int AMXAPI amx_FindPublic_hooked(AMX *amx, const char *name, int *index)
{
	_amx_FindPublic_hook.unhook();
	int ret = _amx_FindPublic(amx, name, index);
	if (amx == AmxInstanceManager::get().getMainAmx())
		callbackMap[*index] = std::string(name);
	_amx_FindPublic_hook.hook();
	return ret;
}

void pluginInit(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	_amx_Exec = ((amx_Exec_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Exec];
	_amx_Register = ((amx_Register_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Register];
	_amx_FindPublic = ((amx_FindPublic_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_FindPublic];

	_amx_Exec_hook.init(_amx_Exec, &amx_Exec_hooked);
	_amx_Exec_hook.hook();

	_amx_Register_hook.init(_amx_Register, &amx_Register_hooked);
	_amx_Register_hook.hook();

	_amx_FindPublic_hook.init(_amx_FindPublic, &amx_FindPublic_hooked);
	_amx_FindPublic_hook.hook();
}