/*Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
*
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*/
#include "rt_String.h"

#include <Windows.h>

using namespace doubango_rt::BackEnd;
using namespace Platform;
using namespace std;

vector<char> rtString::toUtf8(String^ str)
{
    if(str != nullptr && !str->IsEmpty()) {
        int len = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str->Data(), str->Length(), nullptr, 0, nullptr, nullptr);
        if (len > 0) {
            vector<char> vec(len + 1);
            if (WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str->Data(), str->Length(), vec.data(), len, nullptr, nullptr) == len) {
                return std::move(vec);
            }
        }
    }
    return std::move(vector<char>(0));
}

String^ rtString::toString(char const* str)
{
    if(str) {
        int len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, -1, nullptr, 0);
        if (len > 0) {
            vector<wchar_t> vec(len);
            if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, -1, vec.data(), len) == len) {
                return ref new String(vec.data());
            }
        }
    }
    return nullptr;
}