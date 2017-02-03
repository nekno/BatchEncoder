﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

class CFileContext
{
public:
    CWorkerContext* pWorkerContext;
    CString szInputFile;
    CString szOutputFile;
    TCHAR *szCommandLine;
    int nItemId;
    CString szFunction;
    bool bUseReadPipes;
    bool bUseWritePipes;
public:
    CFileContext(CWorkerContext *pWorkerContext, CFormat &format, int nPreset, int nItemId, CString szInputFile, CString szOutputFile)
    {
        CPreset& preset = format.m_Presets.GetData(nPreset);

        this->pWorkerContext = pWorkerContext;
        this->szInputFile = szInputFile;
        this->szOutputFile = szOutputFile;

        CString szExecute;

        szExecute = format.szTemplate;
        szExecute.Replace(_T("$EXE"), _T("\"$EXE\""));
        szExecute.Replace(_T("$EXE"), format.szPath);
        szExecute.Replace(_T("$OPTIONS"), preset.szOptions);

        szExecute.Replace(_T("$INFILE"), _T("\"$INFILE\""));
        if (format.bPipeInput == true)
            szExecute.Replace(_T("$INFILE"), _T("-"));
        else
            szExecute.Replace(_T("$INFILE"), szInputFile);

        szExecute.Replace(_T("$OUTFILE"), _T("\"$OUTFILE\""));
        if (format.bPipeOutput == true)
            szExecute.Replace(_T("$OUTFILE"), _T("-"));
        else
            szExecute.Replace(_T("$OUTFILE"), szOutputFile);

        TCHAR szCommandLine[(64 * 1024)];
        ZeroMemory(szCommandLine, sizeof(szCommandLine));
        lstrcpy(szCommandLine, szExecute.GetBuffer(szExecute.GetLength()));
        this->szCommandLine = szCommandLine;

        this->nItemId = nItemId;
        this->szFunction = format.szFunction;
        this->bUseReadPipes = format.bPipeInput;
        this->bUseWritePipes = format.bPipeOutput;
    }
};