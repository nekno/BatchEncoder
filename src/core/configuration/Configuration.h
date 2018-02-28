﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <string>
#include "Strings.h"
#include "Settings.h"
#include "Options.h"
#include "FormatsList.h"
#include "ItemsList.h"
#include "ToolsList.h"
#include "utilities\LanguageContext.h"

namespace config
{
    class CConfiguration
    {
    public:
        CSettings m_Settings;
        COptions m_Options;
        CFormatsList m_Formats;
        CItemsList m_Items;
        CToolsList m_Tools;
        lang::CLanguageContext m_Language;
    public:
        std::wstring GetString(int nKey)
        {
            std::wstring rValue;
            if (this->m_Language.LookupString(nKey, rValue))
                return rValue;

            if (config::m_Strings.count(nKey) == 1)
                return config::m_Strings.at(nKey);

            return L"??";
        }
    };
}
