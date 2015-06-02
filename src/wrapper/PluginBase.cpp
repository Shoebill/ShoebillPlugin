#include <hook/SimpleInlineHook.hpp>
#include "AmxHelper.h"
#include "Callbacks.h"

#include "AmxInstanceManager.hpp"
#include "NativeFunctionManager.h"
#include "amx/amx.h"

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
	auto data = amx->data != NULL
		? amx->data
		: amx->base + reinterpret_cast<AMX_HEADER *>(amx->base)->dat;
	return reinterpret_cast<cell *>(data + amx->stk);
}

int AMXAPI amx_Exec_hooked(AMX *amx, cell *retval, int index)
{
	auto mainAmx = AmxInstanceManager::get().getMainAmx();
	if (mainAmx == nullptr) {
		StartShoebill();
		if (index == AMX_EXEC_MAIN) {
			AmxInstanceManager::get().markMainAmx(amx);
			invokeCallback(amx, "OnGameModeInit", NULL);
		}
	}
	if (mainAmx == amx && index != AMX_EXEC_CONT)
	{
		std::string callbackName;
		if (index > SHOEBILL_OFFSET) {
			auto publics = reinterpret_cast<AMX_FUNCSTUBNT *>(mainAmx->base + reinterpret_cast<AMX_HEADER *>(mainAmx->base)->publics);
			callbackName = std::string(reinterpret_cast<char*>(publics[index].nameofs + amx->base));
		}
		else {
			callbackName = shoebill_callbacks[index];
		}

		auto do_clean = false;

		auto paramcount = amx->paramcount;
		auto reset_stk = amx->reset_stk;
		amx->reset_stk = amx->stk;
		amx->paramcount = 0;

		auto address = amx_param_get_start(amx);
		auto params = new cell[1 + paramcount];
		params[0] = sizeof(cell) * paramcount;
		memcpy(&params[1], address, params[0]);

		auto hook = callHookedCallback(amx, callbackName, params);
		if (hook) {
			if (hook[1] == 1) {
				*retval = hook[0];
				do_clean = true;
			}
		}

		if (!do_clean) {
			auto ret = invokeCallback(amx, callbackName, params);
			if (shouldCancelCallback(callbackName, ret))
			{
				*retval = ret;
				do_clean = true;
			}
		}

		delete[] params;
		delete[] hook;

		if (do_clean || index <= SHOEBILL_OFFSET) {
			amx->stk += paramcount * sizeof(cell);
			amx->paramcount = 0;
			amx->error = AMX_ERR_NONE;
			return AMX_ERR_NONE;
		}
		amx->reset_stk = reset_stk;
		amx->paramcount = paramcount;
	}
	_amx_Exec_hook.unhook();
	auto ret = _amx_Exec(amx, retval, index);
	_amx_Exec_hook.hook();
	return ret;
}

int AMXAPI amx_Register_hooked(AMX *amx, const AMX_NATIVE_INFO *nativelist, int number)
{
    _amx_Register_hook.unhook();
	auto ret = _amx_Register(amx, nativelist, number);
    _amx_Register_hook.hook();
	for (auto i = 0; (number >= 0 && i < number) || nativelist[i].name != nullptr; i++)
	{
		NativeFunctionManager::get().registerFunction(amx, nativelist[i].name, nativelist[i].func, i);
	}
	return ret;
}

int AMXAPI amx_FindPublic_hooked(AMX *amx, const char *name, int *index)
{
    _amx_FindPublic_hook.unhook();
	auto ret = _amx_FindPublic(amx, name, index);
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
			return AMX_ERR_NONE;
		}
	}
	return ret;
}

void pluginInit(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = static_cast<logprintf_t>(ppData[PLUGIN_DATA_LOGPRINTF]);

	_amx_Exec = static_cast<amx_Exec_t*>(pAMXFunctions)[PLUGIN_AMX_EXPORT_Exec];
	_amx_Register = static_cast<amx_Register_t*>(pAMXFunctions)[PLUGIN_AMX_EXPORT_Register];
	_amx_FindPublic = static_cast<amx_FindPublic_t*>(pAMXFunctions)[PLUGIN_AMX_EXPORT_FindPublic];

    _amx_Exec_hook.init(static_cast<void*>(_amx_Exec), static_cast<void*>(&amx_Exec_hooked));
	_amx_Register_hook.init(static_cast<void*>(_amx_Register), static_cast<void*>(&amx_Register_hooked));
	_amx_FindPublic_hook.init(static_cast<void*>(_amx_FindPublic), static_cast<void*>(&amx_FindPublic_hooked));

    _amx_Exec_hook.hook();
    _amx_FindPublic_hook.hook();
    _amx_Register_hook.hook();
}
