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

#include <unordered_set>

#include "amx/amx.h"
#include "Callbacks.h"

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
		}
	}

	void unregisterAmx(AMX *amx)
	{
		amxInstances.erase(amx);
		if (mainAmx == amx) mainAmx = nullptr;
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

private:
	std::unordered_set<AMX*> amxInstances;
	AMX *mainAmx;

	AmxInstanceManager(const AmxInstanceManager&) = delete;
	AmxInstanceManager& operator= (const AmxInstanceManager&) = delete;
};
