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

#include "amx/amx.h"

class NativeFunctionManager
{
public:
	static NativeFunctionManager& get()
	{
		static NativeFunctionManager instance;
		return instance;
	}

	NativeFunctionManager()
	{

	}

	~NativeFunctionManager()
	{

	}

	void registerFunction(std::string name, AMX_NATIVE functionAddr)
	{
		functions[name] = functionAddr;
	}

	AMX_NATIVE findFunction(std::string name)
	{
		auto it = functions.find(name);
		return it != functions.end() ? it->second : nullptr;
	}

private:
	std::unordered_map<std::string, AMX_NATIVE> functions;

	NativeFunctionManager(const NativeFunctionManager&) = delete;
	NativeFunctionManager& operator= (const NativeFunctionManager&) = delete;
};
