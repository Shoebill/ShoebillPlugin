#include "SimpleInlineHook.hpp"
#include "AmxHelper.h"

#include "AmxInstanceManager.hpp"
#include "NativeFunctionManager.hpp"

logprintf_t logprintf = NULL;

amx_Exec_t _amx_Exec = nullptr;
amx_Register_t _amx_Register = nullptr;
amx_FindPublic_t _amx_FindPublic = nullptr;

SimpleInlineHook _amx_Exec_hook;
SimpleInlineHook _amx_Register_hook;
SimpleInlineHook _amx_FindPublic_hook;

int AMXAPI amx_Exec_hooked(AMX *amx, cell *retval, int index)
{
	if (index == AMX_EXEC_MAIN) AmxInstanceManager::get().markMainAmx(amx);

	_amx_Exec_hook.unhook();
	int ret = _amx_Exec(amx, retval, index);
	_amx_Exec_hook.hook();
	return ret;
}

int AMXAPI amx_Register_hooked(AMX *amx, const AMX_NATIVE_INFO *nativelist, int number)
{
	_amx_Register_hook.unhook();
	int ret = _amx_Register(amx, nativelist, number);
	_amx_Register_hook.hook();

	for (int i = 0; (number >= 0 && i < number) || nativelist[i].name != nullptr; i++) NativeFunctionManager::get().registerFunction(nativelist[i].name, nativelist[i].func);
	return ret;
}

int AMXAPI amx_FindPublic_hooked(AMX *amx, const char *funcname, int *index)
{
	_amx_FindPublic_hook.unhook();
	int ret = _amx_FindPublic(amx, funcname, index);
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
