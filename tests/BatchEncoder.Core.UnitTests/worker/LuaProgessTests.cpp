﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "stdafx.h"
#include "CppUnitTest.h"
#include "worker\LuaProgess.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BatchEncoderCoreUnitTests
{
    TEST_CLASS(CLuaProgess_Tests)
    {
    public:
        TEST_METHOD(CLuaProgess_Constructor)
        {
            #pragma warning(push)
            #pragma warning(disable:4101)
            worker::CLuaProgess m_Progess;
            #pragma warning(pop)
        }
    };
}