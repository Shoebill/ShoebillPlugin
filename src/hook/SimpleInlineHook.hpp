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

#ifndef __SIMEPLINLINEHOOK_H__
#define __SIMEPLINLINEHOOK_H__

#include <memory>

#include "SimpleAsm.hpp"

class SimpleInlineHook
{
public:
    SimpleInlineHook() : _isHooked(false)
    {
    }

    void init(void *target, void *address, void *exParam = nullptr)
    {
        _simpleAsm.reset(new SimpleAsm());
        _simpleAsm->init(target);
        _address = address;
        _exParam = exParam;

        removePageProtect(target, exParam ? 12 : 5); //exParam ? 5+7 : 5
    }

    void hook()
    {
        if (_isHooked) return;

        if (_exParam != nullptr)
            _simpleAsm->movEBP(-4, _exParam);
        _simpleAsm->jmp(_address);

        _isHooked = true;
    }

    void unhook()
    {
        if (!_isHooked) return;

        _simpleAsm->reset();
        _isHooked = false;
    }

private:

    std::unique_ptr<SimpleAsm> _simpleAsm;
    void *_address;
    void *_exParam;

    bool _isHooked;

    static void removePageProtect(void *address, size_t size);

    SimpleInlineHook(const SimpleInlineHook &) = delete;

    SimpleInlineHook &operator=(const SimpleInlineHook &) = delete;
};

#endif