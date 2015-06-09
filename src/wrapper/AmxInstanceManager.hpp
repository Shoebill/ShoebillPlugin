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
#ifndef __AMXINSTANCEMANAGER__
#define __AMXINSTANCEMANAGER__
#include <unordered_set>
#include "amx/amx.h"
#include <map>

class AmxInstanceManager
{
public:
	static AmxInstanceManager& get()
	{
		static AmxInstanceManager instance;
		return instance;
	}

	AmxInstanceManager()
	{

	}

	~AmxInstanceManager()
	{

	}

	void registerAmx(AMX *amx)
	{
		if (!isVaild(amx)) {
			amxInstances.insert(amx);
			registeredFunctions[amx] = std::map<std::string, std::vector<std::string>>();
		}
	}

	void unregisterAmx(AMX *amx)
	{
		if (isVaild(amx))
		{
			registeredFunctions.erase(registeredFunctions.find(amx));
			amxInstances.erase(amx);
			if (mainAmx == amx) mainAmx = nullptr;
		}
	}

	bool isVaild(AMX *amx)
	{
		return amxInstances.find(amx) != amxInstances.end();
	}

	void markMainAmx(AMX *amx)
	{
		if (!isVaild(amx)) return;
		mainAmx = amx;
	}

	inline bool registeredFunctionExists(AMX* amx, std::string functionName)
	{
		return registeredFunctions[amx].find(functionName) != registeredFunctions[amx].end();
	}

	inline bool registerFunction(AMX* amx, std::string functionName, std::vector<std::string> classes)
	{
		if (registeredFunctionExists(amx, functionName))
			return false;
		registeredFunctions[amx][functionName] = classes;
		return true;
	}

	inline std::vector<std::string> getRegisteredParamters(AMX* amx, std::string functionName)
	{
		if (!registeredFunctionExists(amx, functionName))
			return std::vector<std::string>();
		return registeredFunctions[amx][functionName];
	}

	inline bool unregisterFunction(AMX* amx, std::string functionName)
	{
		if (!registeredFunctionExists(amx, functionName))
			return false;
		registeredFunctions[amx].erase(registeredFunctions[amx].find(functionName));
		return true;
	}

	inline AMX* getMainAmx()
	{
		return mainAmx;
	}

	inline AMX* getAvailableAmx()
	{
		if (mainAmx != nullptr) return mainAmx;
		if (amxInstances.empty()) return nullptr;
		return *amxInstances.begin();
	}

	inline std::unordered_set<AMX*> getInstances()
	{
		std::unordered_set<AMX*> copyOfInstances;
		for (std::unordered_set<AMX*>::iterator it = amxInstances.begin(); it != amxInstances.end(); ++it) {
			copyOfInstances.insert(*it);
		}
		return copyOfInstances;
	}

private:
	std::unordered_set<AMX*> amxInstances;
	AMX *mainAmx;
	std::map<AMX*, std::map<std::string, std::vector<std::string>>> registeredFunctions;

	AmxInstanceManager(const AmxInstanceManager&) = delete;
	AmxInstanceManager& operator= (const AmxInstanceManager&) = delete;
};
#endif