#include <hook/SimpleInlineHook.hpp>
#include "AmxHelper.h"
#include "Callbacks.h"

#include "AmxInstanceManager.hpp"
#include "NativeFunctionManager.h"
#include "amx/amx.h"
#include "CallbackManager.h"
#include "ShoebillMain.h"

logprintf_t logprintf = NULL;

amx_Exec_t _amx_Exec = nullptr;
amx_Register_t _amx_Register = nullptr;
amx_FindPublic_t _amx_FindPublic = nullptr;

SimpleInlineHook _amx_Exec_hook;
SimpleInlineHook _amx_Register_hook;
SimpleInlineHook _amx_FindPublic_hook;

#define SHOEBILL_OFFSET (-5000)

std::map<int, std::string> shoebill_callbacks;

cell *amx_param_get_start(AMX *amx) {
	unsigned char *data = amx->data != NULL
		? amx->data
		: amx->base + ((AMX_HEADER *)amx->base)->dat;
	return (cell *)(data + amx->stk);
}

int AMXAPI amx_Exec_hooked(AMX *amx, cell *retval, int index)
{
	if (AmxInstanceManager::get().getMainAmx() == nullptr) {
		StartShoebill();
		if (index == AMX_EXEC_MAIN) {
			AmxInstanceManager::get().markMainAmx(amx);
			invokeCallback(amx, "OnGameModeInit", NULL);
		}
	}
	if (AmxInstanceManager::get().getMainAmx() == amx)
	{
		std::string callbackName = std::string();

		if (index > SHOEBILL_OFFSET) {
			AMX* main_amx = AmxInstanceManager::get().getMainAmx();
			AMX_FUNCSTUBNT *publics = (AMX_FUNCSTUBNT *)(main_amx->base + ((AMX_HEADER *)main_amx->base)->publics);
			callbackName = std::string((char*)(publics[index].nameofs + amx->base));
		}
		else {
			callbackName = shoebill_callbacks[index];
		}

		cell reset_stk = amx->reset_stk;
		cell paramcount = amx->paramcount;
		amx->reset_stk = amx->stk;
		amx->paramcount = 0;
		bool do_clean = false;

		cell* address = amx_param_get_start(amx);
		cell* params = new cell[1 + paramcount];
		params[0] = sizeof(cell) * paramcount;
		memcpy(&params[1], address, params[0]);
		amx->error = AMX_ERR_NONE;
		
		int *hook = callHookedCallback(amx, callbackName, params);
		if (hook) {
			if (hook[1] == 1) {
				*retval = hook[0];
				do_clean = true;
			}
		}

		if (!do_clean) {
			int ret = invokeCallback(amx, callbackName, params);
			if (shouldCancelCallback(callbackName, ret))
			{
				*retval = ret;
				do_clean = true;
			}
		}
		delete[] params;

		if (do_clean) {
			amx->paramcount = 0;
			amx->stk += paramcount * sizeof(cell);
			return AMX_ERR_NONE;
		}
		else {
			amx->reset_stk = reset_stk;
			amx->paramcount = paramcount;
		}
	}
	_amx_Exec_hook.unhook();
	int ret = _amx_Exec(amx, retval, index);
	_amx_Exec_hook.hook();
	amx->paramcount = 0;
	return ret;
}

int AMXAPI amx_Register_hooked(AMX *amx, const AMX_NATIVE_INFO *nativelist, int number)
{
    _amx_Register_hook.unhook();
	int ret = _amx_Register(amx, nativelist, number);
    _amx_Register_hook.hook();
	for (int i = 0; (number >= 0 && i < number) || nativelist[i].name != nullptr; i++)
	{
		NativeFunctionManager::get().registerFunction(amx, nativelist[i].name, nativelist[i].func, i);
	}
	return ret;
}

int AMXAPI amx_FindPublic_hooked(AMX *amx, const char *name, int *index)
{
    _amx_FindPublic_hook.unhook();
	int ret = _amx_FindPublic(amx, name, index);
    _amx_FindPublic_hook.hook();
	if (amx == AmxInstanceManager::get().getMainAmx()) {
		if (ret != AMX_ERR_NONE) {
			auto it = shoebill_callbacks.begin();
			while (it != shoebill_callbacks.end()) {
				if (it->second == std::string(name)) {
					*index = it->first;
					return AMX_ERR_NONE;
				}
				++it;
			}
			*index = SHOEBILL_OFFSET - shoebill_callbacks.size();
			shoebill_callbacks[*index] = std::string(name);
			logprintf("Register %s with idx %i", name, *index);
			return AMX_ERR_NONE;
		}
	}
	return ret;
}

void pluginInit(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

	_amx_Exec = ((amx_Exec_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Exec];
	_amx_Register = ((amx_Register_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_Register];
	_amx_FindPublic = ((amx_FindPublic_t*)pAMXFunctions)[PLUGIN_AMX_EXPORT_FindPublic];

    _amx_Exec_hook.init((void*)_amx_Exec, (void*)&amx_Exec_hooked);
	_amx_Register_hook.init((void*)_amx_Register, (void*)&amx_Register_hooked);
	_amx_FindPublic_hook.init((void*)_amx_FindPublic, (void*)&amx_FindPublic_hooked);

    _amx_Exec_hook.hook();
    _amx_FindPublic_hook.hook();
    _amx_Register_hook.hook();
}
