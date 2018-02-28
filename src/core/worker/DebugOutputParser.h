﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "OutputParser.h"

namespace worker
{
    class CDebugOutputParser : public IOutputParser
    {
    public:
        IWorkerContext * ctx;
        CCommandLine *pCommandLine;
    public:
        bool Init(IWorkerContext* ctx, CCommandLine* pCommandLine)
        {
            this->ctx = ctx;
            this->pCommandLine = pCommandLine;
            return true;
        }
        bool Parse(const char *szLine)
        {
            OutputDebugStringA(szLine);
            OutputDebugStringA("\n");
            return this->ctx->IsRunning();
        }
    };
}
