/**
* Copyright (C) 2014 MK124
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <unordered_map>

#include "AmxHelper.h"
#include "amx/amx.h"
#include "Callbacks.h"

struct hookedNative {
	AMX_NATIVE originalFunc;
	std::shared_ptr<SimpleInlineHook>* hook;
	std::string funcName;
};

class NativeFunctionManager
{
public:

	static NativeFunctionManager& get();

	NativeFunctionManager();
	~NativeFunctionManager();

	void registerFunction(AMX* amx, std::string name, AMX_NATIVE functionAddr, int number);
	std::map<std::string, hookedNative*> getNatives();
	AMX_NATIVE findFunction(std::string name);
	void overridePointer(AMX* amx, AMX_NATIVE function, const char *name);
	void clearFunctions();

private:
	std::unordered_map<std::string, AMX_NATIVE> functions;
	NativeFunctionManager(const NativeFunctionManager&) = delete;
	NativeFunctionManager& operator= (const NativeFunctionManager&) = delete;
};
