﻿//
// BatchEncoder (Audio Conversion GUI)
// Copyright (C) 2005-2008 Wiesław Šoltés <wisodev@users.sourceforge.net>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "StdAfx.h"
#include "BatchEncoder.h"
#include "Utf8String.h"
#include "UnicodeUtf8.h"

CUtf8String::CUtf8String()
{
    szBuffUtf8 = NULL;
}

CUtf8String::~CUtf8String()
{
    Clear();
}

char *CUtf8String::Create(CString szData)
{
#ifdef _UNICODE
    // UNICODE to UTF-8
    if(szData.GetLength() > 0)
    {
        szBuffUtf8 = (char *) MakeUtf8String(szData);
    }
    else
    {
        szBuffUtf8 = (char *) malloc(1);
        szBuffUtf8[0] = '\0';
    }

    return szBuffUtf8;
#else
    // ANSI to UTF-8
    if(szData.GetLength() > 0)
    {
        Utf8Encode(szData, &szBuffUtf8);
    }
    else
    {
        szBuffUtf8 = (char *) malloc(1);
        szBuffUtf8[0] = '\0';
    }

    return szBuffUtf8;
#endif
}

void CUtf8String::Clear()
{
    if(szBuffUtf8 != NULL)
    {
        free(szBuffUtf8);
        szBuffUtf8 = NULL;
    }
}
