﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "StdAfx.h"
#include "MainApp.h"
#include "Strings.h"
#include "language\LanguageHelper.h"
#include "utilities\Utilities.h"
#include "utilities\UnicodeUtf8.h"
#include "utilities\Utf8String.h"
#include "xml\XmlTools.h"
#include "ToolsDlg.h"

DWORD WINAPI ToolsDlgDropThread(LPVOID lpParam)
{
    ToolsDlgDropContext* pDD = (ToolsDlgDropContext*)lpParam;
    pDD->pDlg->HandleDropFiles(pDD->hDrop);
    pDD->bHandled = true;
    return ::CloseHandle(pDD->hThread);
}

IMPLEMENT_DYNAMIC(CToolsDlg, CDialog)
CToolsDlg::CToolsDlg(CWnd* pParent /*=NULL*/)
    : CMyDialogEx(CToolsDlg::IDD, pParent)
{
    this->m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);
    this->szToolsDialogResize = _T("");
    this->szToolsListColumns = _T("");
    this->bUpdate = false;
    this->bDownload = false;
    this->nSelectedTool = 0;
}

CToolsDlg::~CToolsDlg()
{

}

void CToolsDlg::DoDataExchange(CDataExchange* pDX)
{
    CMyDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_TOOL_NAME, m_StcName);
    DDX_Control(pDX, IDC_STATIC_TOOL_PLATFORM, m_StcPlatform);
    DDX_Control(pDX, IDC_STATIC_TOOL_FORMATS, m_StcFormats);
    DDX_Control(pDX, IDC_STATIC_TOOL_URL, m_StcUrl);
    DDX_Control(pDX, IDC_STATIC_TOOL_FILE, m_StcFile);
    DDX_Control(pDX, IDC_STATIC_TOOL_EXTRACT, m_StcExtract);
    DDX_Control(pDX, IDC_STATIC_TOOL_PATH, m_StcPath);
    DDX_Control(pDX, IDC_LIST_TOOLS, m_LstTools);
    DDX_Control(pDX, IDC_EDIT_TOOL_NAME, m_EdtName);
    DDX_Control(pDX, IDC_EDIT_TOOL_PLATFORM, m_EdtPlatform);
    DDX_Control(pDX, IDC_EDIT_TOOL_FORMATS, m_EdtFormats);
    DDX_Control(pDX, IDC_EDIT_TOOL_URL, m_EdtUrl);
    DDX_Control(pDX, IDC_EDIT_TOOL_FILE, m_EdtFile);
    DDX_Control(pDX, IDC_EDIT_TOOL_EXTRACT, m_EdtExtract);
    DDX_Control(pDX, IDC_EDIT_TOOL_PATH, m_EdtPath);
    DDX_Control(pDX, IDOK, m_BtnOK);
    DDX_Control(pDX, IDCANCEL, m_BtnCancel);
    DDX_Control(pDX, IDC_BUTTON_TOOL_IMPORT, m_BtnImport);
    DDX_Control(pDX, IDC_BUTTON_TOOL_EXPORT, m_BtnExport);
    DDX_Control(pDX, IDC_BUTTON_TOOL_DUPLICATE, m_BtnDuplicate);
    DDX_Control(pDX, IDC_BUTTON_TOOL_REMOVE_ALL, m_BtnRemoveAll);
    DDX_Control(pDX, IDC_BUTTON_TOOL_REMOVE, m_BtnRemove);
    DDX_Control(pDX, IDC_BUTTON_TOOL_ADD, m_BtnAdd);
    DDX_Control(pDX, IDC_BUTTON_TOOL_UP, m_BtnMoveUp);
    DDX_Control(pDX, IDC_BUTTON_TOOL_DOWN, m_BtnMoveDown);
    DDX_Control(pDX, IDC_BUTTON_TOOL_UPDATE, m_BtnUpdate);
    DDX_Control(pDX, IDC_BUTTON_TOOL_LOAD, m_BtnLoad);
    DDX_Control(pDX, IDC_BUTTON_TOOL_SAVE, m_BtnSave);
    DDX_Control(pDX, IDC_BUTTON_TOOL_DOWNLOAD, m_BtnDownload);
}

BEGIN_MESSAGE_MAP(CToolsDlg, CMyDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DROPFILES()
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TOOLS, OnLvnItemchangedListTools)
    ON_EN_CHANGE(IDC_EDIT_TOOL_NAME, OnEnChangeEditToolName)
    ON_EN_CHANGE(IDC_EDIT_TOOL_PLATFORM, OnEnChangeEditToolPlatform)
    ON_EN_CHANGE(IDC_EDIT_TOOL_FORMATS, OnEnChangeEditToolFormats)
    ON_EN_CHANGE(IDC_EDIT_TOOL_URL, OnEnChangeEditToolUrl)
    ON_EN_CHANGE(IDC_EDIT_TOOL_FILE, OnEnChangeEditToolFile)
    ON_EN_CHANGE(IDC_EDIT_TOOL_EXTRACT, OnEnChangeEditToolExtract)
    ON_EN_CHANGE(IDC_EDIT_TOOL_PATH, OnEnChangeEditToolPath)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_IMPORT, OnBnClickedButtonImport)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_EXPORT, OnBnClickedButtonExport)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_DUPLICATE, OnBnClickedButtonDuplicate)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_REMOVE_ALL, OnBnClickedButtonRemoveAllTools)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_REMOVE, OnBnClickedButtonRemoveTool)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_ADD, OnBnClickedButtonAddTool)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_UP, OnBnClickedButtonToolUp)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_DOWN, OnBnClickedButtonToolDown)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_UPDATE, OnBnClickedButtonUpdateTool)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_LOAD, OnBnClickedButtonLoadTools)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_SAVE, OnBnClickedButtonSaveTools)
    ON_BN_CLICKED(IDC_BUTTON_TOOL_DOWNLOAD, OnBnClickedButtonDownloadSelected)
    ON_WM_CLOSE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CToolsDlg::OnInitDialog()
{
    CMyDialogEx::OnInitDialog();

    InitCommonControls();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // update list style
    DWORD dwExStyle = m_LstTools.GetExtendedStyle();
    dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES;
    m_LstTools.SetExtendedStyle(dwExStyle);

    // insert all ListCtrl columns
    m_LstTools.InsertColumn(TOOL_COLUMN_NAME, _T("Name"), LVCFMT_LEFT, 195);
    m_LstTools.InsertColumn(TOOL_COLUMN_URL, _T("Url"), LVCFMT_LEFT, 195);
    m_LstTools.InsertColumn(TOOL_COLUMN_STATUS, _T("Status"), LVCFMT_LEFT, 95);

    // insert all ListCtrl items and sub items
    this->InsertToolsToListCtrl();

    m_LstTools.SetItemState(nSelectedTool, LVIS_SELECTED, LVIS_SELECTED);
    m_LstTools.EnsureVisible(nSelectedTool, FALSE);

    // enable drag & drop
    this->DragAcceptFiles(TRUE);

    this->LoadWindowSettings();
    this->SetLanguage();

    return TRUE;
}

void CToolsDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CMyDialogEx::OnPaint();
    }
}

HCURSOR CToolsDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CToolsDlg::OnDropFiles(HDROP hDropInfo)
{
    if (bDownload == true)
        return;

    if (this->m_DD.bHandled == true)
    {
        this->m_DD.bHandled = false;
        this->m_DD.pDlg = this;
        this->m_DD.hDrop = hDropInfo;
        this->m_DD.hThread = ::CreateThread(NULL, 0, ToolsDlgDropThread, (LPVOID)&this->m_DD, 0, &this->m_DD.dwThreadID);
        if (this->m_DD.hThread == NULL)
            this->m_DD.bHandled = true;
    }
    CMyDialogEx::OnDropFiles(hDropInfo);
}

void CToolsDlg::OnBnClickedOk()
{
    if (bDownload == true)
        return;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
        nSelectedTool = m_LstTools.GetNextSelectedItem(pos);
    else
        nSelectedTool = -1;

    this->SaveWindowSettings();

    OnOK();
}

void CToolsDlg::OnBnClickedCancel()
{
    if (bDownload == true)
        return;

    this->SaveWindowSettings();

    OnCancel();
}

void CToolsDlg::OnLvnItemchangedListTools(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    if (bDownload == false)
    {
        this->ListSelectionChange();
    }

    *pResult = 0;
}

void CToolsDlg::OnBnClickedButtonImport()
{
    if (bDownload == true)
        return;

    CString szFilter;
    szFilter.Format(_T("%s (*.tool)|*.tool|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
        pConfig->GetString(0x00310010, pszFileDialogs[9]),
        pConfig->GetString(0x00310002, pszFileDialogs[1]),
        pConfig->GetString(0x00310001, pszFileDialogs[0]));

    CFileDialog fd(TRUE, _T("tool"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        szFilter, this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->LoadTool(szFileXml);
    }
}

void CToolsDlg::OnBnClickedButtonExport()
{
    if (bDownload == true)
        return;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nSelected = m_LstTools.GetNextSelectedItem(pos);
        if (nSelected >= 0)
        {
            CTool& tool = m_Tools.Get(nSelected);

            CString szFilter;
            szFilter.Format(_T("%s (*.tool)|*.tool|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
                pConfig->GetString(0x00310010, pszFileDialogs[9]),
                pConfig->GetString(0x00310002, pszFileDialogs[1]),
                pConfig->GetString(0x00310001, pszFileDialogs[0]));

            CFileDialog fd(FALSE, _T("tool"), tool.szName,
                OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
                szFilter, this);

            if (fd.DoModal() == IDOK)
            {
                CString szFileXml = fd.GetPathName();
                this->SaveTool(szFileXml, tool);
            }
        }
    }
}

void CToolsDlg::OnBnClickedButtonDuplicate()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nSelected = m_LstTools.GetNextSelectedItem(pos);
        if (nSelected >= 0)
        {
            CTool& tool = m_Tools.Get(nSelected);
            CTool copy = tool;

            m_Tools.Insert(copy);

            int nItem = m_LstTools.GetItemCount();
            AddToList(copy, nItem);

            m_LstTools.SetItemState(-1, 0, LVIS_SELECTED);
            m_LstTools.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
            m_LstTools.EnsureVisible(nItem, FALSE);
        }
    }

    bUpdate = false;

    this->ListSelectionChange();
}

void CToolsDlg::OnBnClickedButtonRemoveAllTools()
{
    if (bDownload == true)
        return;

    if (m_Tools.Count() > 0)
    {
        CTool& tool = m_Tools.Get(nSelectedTool);
        m_Tools.RemoveAll();

        m_LstTools.DeleteAllItems();

        this->ListSelectionChange();
    }
}

void CToolsDlg::OnBnClickedButtonRemoveTool()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    bUpdate = true;

    int nItem = -1;
    int nItemLastRemoved = -1;
    do
    {
        nItem = m_LstTools.GetNextItem(-1, LVIS_SELECTED);
        if (nItem != -1)
        {
            m_Tools.Remove(nItem);
            m_LstTools.DeleteItem(nItem);

            nItemLastRemoved = nItem;
        }
    } while (nItem != -1);

    m_LstTools.SetItemState(-1, 0, LVIS_SELECTED);

    int nItems = m_LstTools.GetItemCount();
    if (nItemLastRemoved != -1)
    {
        if (nItemLastRemoved < nItems && nItems >= 0)
        {
            m_LstTools.SetItemState(nItemLastRemoved, LVIS_SELECTED, LVIS_SELECTED);
            m_LstTools.EnsureVisible(nItemLastRemoved, FALSE);
        }
        else if (nItemLastRemoved >= nItems && nItems >= 0)
        {
            m_LstTools.SetItemState(nItemLastRemoved - 1, LVIS_SELECTED, LVIS_SELECTED);
            m_LstTools.EnsureVisible(nItemLastRemoved, FALSE);
        }
    }

    bUpdate = false;

    this->ListSelectionChange();
}

void CToolsDlg::OnBnClickedButtonAddTool()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    bUpdate = true;

    int nItem = m_LstTools.GetItemCount();

    CTool tool;
    tool.szName = pConfig->GetString(0x00240004, pszToolsDialog[3]);
    tool.szPlatform = _T("");
    tool.szFormats = _T("");
    tool.szUrl = _T("");
    tool.szFile = _T("");
    tool.szExtract = _T("");
    tool.szPath = _T("");
    tool.szStatus = _T("");

    m_Tools.Insert(tool);

    AddToList(tool, nItem);

    m_LstTools.SetItemState(-1, 0, LVIS_SELECTED);
    m_LstTools.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
    m_LstTools.EnsureVisible(nItem, FALSE);

    bUpdate = false;

    this->ListSelectionChange();
}

void CToolsDlg::OnBnClickedButtonToolUp()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstTools.GetNextSelectedItem(pos);
        if (nItem > 0)
        {
            CTool& tool1 = m_Tools.Get(nItem);
            CTool& tool2 = m_Tools.Get(nItem - 1);

            m_LstTools.SetItemText(nItem, TOOL_COLUMN_NAME, tool2.szName);
            m_LstTools.SetItemText(nItem, TOOL_COLUMN_URL, tool2.szUrl);
            m_LstTools.SetItemText(nItem, TOOL_COLUMN_STATUS, tool2.szStatus);
            m_LstTools.SetItemText(nItem - 1, TOOL_COLUMN_NAME, tool1.szName);
            m_LstTools.SetItemText(nItem - 1, TOOL_COLUMN_URL, tool1.szUrl);
            m_LstTools.SetItemText(nItem - 1, TOOL_COLUMN_STATUS, tool1.szStatus);

            m_Tools.Swap(nItem, nItem - 1);

            m_LstTools.SetItemState(-1, 0, LVIS_SELECTED);
            m_LstTools.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);
            m_LstTools.EnsureVisible(nItem - 1, FALSE);
        }
    }

    bUpdate = false;
}

void CToolsDlg::OnBnClickedButtonToolDown()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstTools.GetNextSelectedItem(pos);
        int nItems = m_LstTools.GetItemCount();
        if (nItem != (nItems - 1) && nItem >= 0)
        {
            CTool& tool1 = m_Tools.Get(nItem);
            CTool& tool2 = m_Tools.Get(nItem + 1);

            m_LstTools.SetItemText(nItem, TOOL_COLUMN_NAME, tool2.szName);
            m_LstTools.SetItemText(nItem, TOOL_COLUMN_URL, tool2.szUrl);
            m_LstTools.SetItemText(nItem, TOOL_COLUMN_STATUS, tool2.szStatus);
            m_LstTools.SetItemText(nItem + 1, TOOL_COLUMN_NAME, tool1.szName);
            m_LstTools.SetItemText(nItem + 1, TOOL_COLUMN_URL, tool1.szUrl);
            m_LstTools.SetItemText(nItem + 1, TOOL_COLUMN_STATUS, tool1.szStatus);

            m_Tools.Swap(nItem, nItem + 1);

            m_LstTools.SetItemState(-1, 0, LVIS_SELECTED);
            m_LstTools.SetItemState(nItem + 1, LVIS_SELECTED, LVIS_SELECTED);
            m_LstTools.EnsureVisible(nItem + 1, FALSE);
        }
    }

    bUpdate = false;
}

void CToolsDlg::OnBnClickedButtonUpdateTool()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstTools.GetNextSelectedItem(pos);

        CString szName = _T("");
        CString szPlatform = _T("");
        CString szFormats = _T("");
        CString szUrl = _T("");
        CString szFile = _T("");
        CString szExtract = _T("");
        CString szPath = _T("");

        this->m_EdtName.GetWindowText(szName);
        this->m_EdtPlatform.GetWindowText(szPlatform);
        this->m_EdtFormats.GetWindowText(szFormats);
        this->m_EdtUrl.GetWindowText(szUrl);
        this->m_EdtFile.GetWindowText(szFile);
        this->m_EdtExtract.GetWindowText(szExtract);
        this->m_EdtPath.GetWindowText(szPath);

        CTool& tool = m_Tools.Get(nItem);
        tool.szName = szName;
        tool.szPlatform = szPlatform;
        tool.szFormats = szFormats;
        tool.szUrl = szUrl;
        tool.szFile = szFile;
        tool.szExtract = szExtract;
        tool.szPath = szPath;

        m_LstTools.SetItemText(nItem, TOOL_COLUMN_NAME, szName);
        m_LstTools.SetItemText(nItem, TOOL_COLUMN_URL, szUrl);
        m_LstTools.SetItemText(nItem, TOOL_COLUMN_STATUS, tool.szStatus);

        m_LstTools.SetItemState(-1, 0, LVIS_SELECTED);
        m_LstTools.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
        m_LstTools.EnsureVisible(nItem, FALSE);
    }

    bUpdate = false;
}

void CToolsDlg::OnEnChangeEditToolName()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolPlatform()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolFormats()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolUrl()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolFile()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolExtract()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnEnChangeEditToolPath()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    OnBnClickedButtonUpdateTool();
}

void CToolsDlg::OnBnClickedButtonLoadTools()
{
    if (bDownload == true)
        return;

    CString szFilter;
    szFilter.Format(_T("%s (*.tools)|*.tools|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
        pConfig->GetString(0x00310009, pszFileDialogs[8]),
        pConfig->GetString(0x00310002, pszFileDialogs[1]),
        pConfig->GetString(0x00310001, pszFileDialogs[0]));

    CFileDialog fd(TRUE, _T("tools"), _T(""),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER,
        szFilter, this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->LoadTools(szFileXml);
    }
}

void CToolsDlg::OnBnClickedButtonSaveTools()
{
    if (bDownload == true)
        return;

    CString szFilter;
    szFilter.Format(_T("%s (*.tools)|*.tools|%s (*.xml)|*.xml|%s (*.*)|*.*||"),
        pConfig->GetString(0x00310009, pszFileDialogs[8]),
        pConfig->GetString(0x00310002, pszFileDialogs[1]),
        pConfig->GetString(0x00310001, pszFileDialogs[0]));

    CFileDialog fd(FALSE, _T("tools"), _T("tools"),
        OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT,
        szFilter, this);

    if (fd.DoModal() == IDOK)
    {
        CString szFileXml = fd.GetPathName();
        this->SaveTools(szFileXml);
    }
}

void CToolsDlg::OnBnClickedButtonDownloadSelected()
{
    if (bDownload == true)
    {
        m_Worker.Terminate();

        CLanguageHelper helper(pConfig);
        helper.SetWndText(&m_BtnDownload, 0x000E0023);

        EnableUserInterface(TRUE);

        bDownload = false;
        return;
    }

    bDownload = true;

    int nCount = m_LstTools.GetItemCount();
    if (nCount > 0)
    {
        for (int i = 0; i < nCount; i++)
        {
            if (m_LstTools.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                m_LstTools.SetItemText(i, TOOL_COLUMN_STATUS, _T(""));
            }
        }

        m_Worker.Start(
            [](void *param)->int
            {
                CToolsDlg* pToolsDlg = (CToolsDlg*)param;
                pToolsDlg->DownloadTools();
                return pToolsDlg->m_Worker.Close();
            },
            this, false);
    }
}

void CToolsDlg::OnClose()
{
    this->SaveWindowSettings();

    CMyDialogEx::OnClose();
}

void CToolsDlg::OnDestroy()
{
    CMyDialogEx::OnDestroy();

    if (bDownload == true)
    {
        m_Worker.Terminate();
        bDownload = false;
    }
}

void CToolsDlg::LoadWindowSettings()
{
    // set window rectangle and position
    if (szToolsDialogResize.CompareNoCase(_T("")) != 0)
        this->SetWindowRectStr(szToolsDialogResize);

    // load columns width for ToolsList
    if (szToolsListColumns.CompareNoCase(_T("")) != 0)
    {
        int nColWidth[3];
        if (_stscanf(szToolsListColumns, _T("%d %d %d"),
            &nColWidth[0],
            &nColWidth[1],
            &nColWidth[2]) == 3)
        {
            for (int i = 0; i < 3; i++)
                m_LstTools.SetColumnWidth(i, nColWidth[i]);
        }
    }
}

void CToolsDlg::SaveWindowSettings()
{
    // save window rectangle and position
    this->szToolsDialogResize = this->GetWindowRectStr();

    // save columns width from ToolsList
    int nColWidth[3];
    for (int i = 0; i < 3; i++)
        nColWidth[i] = m_LstTools.GetColumnWidth(i);
    szToolsListColumns.Format(_T("%d %d %d"),
        nColWidth[0],
        nColWidth[1],
        nColWidth[2]);
}

void CToolsDlg::SetLanguage()
{
    CLanguageHelper helper(pConfig);

    helper.SetColumnText(m_LstTools, TOOL_COLUMN_NAME, 0x000E0001);
    helper.SetColumnText(m_LstTools, TOOL_COLUMN_URL, 0x000E0002);
    helper.SetColumnText(m_LstTools, TOOL_COLUMN_STATUS, 0x000E0003);

    helper.SetWndText(this, 0x000E0010);
    helper.SetWndText(&m_BtnCancel, 0x000E0011);
    helper.SetWndText(&m_StcName, 0x000E0012);
    helper.SetWndText(&m_StcPlatform, 0x000E0013);
    helper.SetWndText(&m_StcFormats, 0x000E0014);
    helper.SetWndText(&m_StcUrl, 0x000E0015);
    helper.SetWndText(&m_StcFile, 0x000E0016);
    helper.SetWndText(&m_StcExtract, 0x000E0017);
    helper.SetWndText(&m_StcPath, 0x000E0018);
    helper.SetWndText(&m_BtnMoveUp, 0x000E0019);
    helper.SetWndText(&m_BtnMoveDown, 0x000E001A);
    helper.SetWndText(&m_BtnImport, 0x000E001B);
    helper.SetWndText(&m_BtnExport, 0x000E001C);
    helper.SetWndText(&m_BtnDuplicate, 0x000E001D);
    helper.SetWndText(&m_BtnRemoveAll, 0x000E001E);
    helper.SetWndText(&m_BtnRemove, 0x000E001F);
    helper.SetWndText(&m_BtnAdd, 0x000E0020);
    helper.SetWndText(&m_BtnLoad, 0x000E0021);
    helper.SetWndText(&m_BtnSave, 0x000E0022);
    helper.SetWndText(&m_BtnDownload, 0x000E0023);
    helper.SetWndText(&m_BtnUpdate, 0x000E0025);
    helper.SetWndText(&m_BtnOK, 0x000E0026);
}

void CToolsDlg::AddToList(CTool &tool, int nItem)
{
    LVITEM lvi;

    ZeroMemory(&lvi, sizeof(LVITEM));

    lvi.mask = LVIF_TEXT | LVIF_STATE;
    lvi.state = 0;
    lvi.stateMask = 0;
    lvi.iItem = nItem;

    lvi.iSubItem = TOOL_COLUMN_NAME;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tool.szName);
    m_LstTools.InsertItem(&lvi);

    lvi.iSubItem = TOOL_COLUMN_URL;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tool.szUrl);
    m_LstTools.SetItemText(lvi.iItem, TOOL_COLUMN_URL, lvi.pszText);

    lvi.iSubItem = TOOL_COLUMN_STATUS;
    lvi.pszText = (LPTSTR)(LPCTSTR)(tool.szStatus);
    m_LstTools.SetItemText(lvi.iItem, TOOL_COLUMN_STATUS, lvi.pszText);
}

void CToolsDlg::InsertToolsToListCtrl()
{
    int nTools = m_Tools.Count();
    for (int i = 0; i < nTools; i++)
    {
        CTool& tool = m_Tools.Get(i);
        this->AddToList(tool, i);
    }
}

void CToolsDlg::HandleDropFiles(HDROP hDropInfo)
{
    int nCount = ::DragQueryFile(hDropInfo, (UINT)0xFFFFFFFF, NULL, 0);
    if (nCount > 0)
    {
        for (int i = 0; i < nCount; i++)
        {
            int nReqChars = ::DragQueryFile(hDropInfo, i, NULL, 0);

            CString szFile;
            ::DragQueryFile(hDropInfo, i, szFile.GetBuffer(nReqChars * 2 + 8), nReqChars * 2 + 8);
            if (!(::GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY))
            {
                CString szPath = szFile;
                CString szExt = ::GetFileExtension(szPath);

                if (szExt.CompareNoCase(_T("tools")) == 0)
                {
                    this->LoadTools(szPath);
                }
                else if (szExt.CompareNoCase(_T("tool")) == 0)
                {
                    // Add tool to tools list.
                    XmlTools doc;
                    if (doc.Open(szPath) == true)
                    {
                        CTool tool;
                        doc.GetTool(tool);
                        m_Tools.Insert(tool);

                        int nItem = m_Tools.Count() - 1;
                        this->AddToList(tool, nItem);
                    }
                }
            }

            szFile.ReleaseBuffer();
        }
    }

    ::DragFinish(hDropInfo);
}

void CToolsDlg::UpdateFields(CTool &tool)
{
    this->m_EdtName.SetWindowText(tool.szName);
    this->m_EdtPlatform.SetWindowText(tool.szPlatform);
    this->m_EdtFormats.SetWindowText(tool.szFormats);
    this->m_EdtUrl.SetWindowText(tool.szUrl);
    this->m_EdtFile.SetWindowText(tool.szFile);
    this->m_EdtExtract.SetWindowText(tool.szExtract);
    this->m_EdtPath.SetWindowText(tool.szPath);
}

void CToolsDlg::ListSelectionChange()
{
    if (bDownload == true)
        return;

    if (bUpdate == true)
        return;

    bUpdate = true;

    POSITION pos = m_LstTools.GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int nItem = m_LstTools.GetNextSelectedItem(pos);

        CTool& tool = this->m_Tools.Get(nItem);

        this->UpdateFields(tool);
    }
    else
    {
        this->m_EdtName.SetWindowText(_T(""));
        this->m_EdtPlatform.SetWindowText(_T(""));
        this->m_EdtFormats.SetWindowText(_T(""));
        this->m_EdtUrl.SetWindowText(_T(""));
        this->m_EdtFile.SetWindowText(_T(""));
        this->m_EdtExtract.SetWindowText(_T(""));
        this->m_EdtPath.SetWindowText(_T(""));
    }

    bUpdate = false;
}

void CToolsDlg::LoadTool(CString szFileXml)
{
    XmlTools doc;
    if (doc.Open(szFileXml) == true)
    {
        CTool tool;
        doc.GetTool(tool);
        m_Tools.Insert(tool);

        int nItem = m_Tools.Count() - 1;
        this->AddToList(tool, nItem);
    }
    else
    {
        MessageBox(
            pConfig->GetString(0x00240002, pszToolsDialog[1]),
            pConfig->GetString(0x00240001, pszToolsDialog[0]),
            MB_OK | MB_ICONERROR);
    }
}

void CToolsDlg::SaveTool(CString szFileXml, CTool &tool)
{
    XmlTools doc;
    doc.SetTool(tool);
    if (doc.Save(szFileXml) != true)
    {
        MessageBox(
            pConfig->GetString(0x00240003, pszToolsDialog[2]),
            pConfig->GetString(0x00240001, pszToolsDialog[0]),
            MB_OK | MB_ICONERROR);
    }
}

void CToolsDlg::LoadTools(CString szFileXml)
{
    XmlTools doc;
    if (doc.Open(szFileXml) == true)
    {
        this->m_Tools.RemoveAll();
        this->m_LstTools.DeleteAllItems();

        doc.GetTools(this->m_Tools);

        if (this->m_Tools.Count() > 0)
            nSelectedTool = 0;

        this->InsertToolsToListCtrl();
        this->ListSelectionChange();
    }
    else
    {
        MessageBox(
            pConfig->GetString(0x00240002, pszToolsDialog[1]),
            pConfig->GetString(0x00240001, pszToolsDialog[0]),
            MB_OK | MB_ICONERROR);
    }
}

void CToolsDlg::SaveTools(CString szFileXml)
{
    XmlTools doc;
    doc.SetTools(this->m_Tools);
    if (doc.Save(szFileXml) != true)
    {
        MessageBox(
            pConfig->GetString(0x00240003, pszToolsDialog[2]),
            pConfig->GetString(0x00240001, pszToolsDialog[0]),
            MB_OK | MB_ICONERROR);
    }
}

void CToolsDlg::DownloadTools()
{
    bDownload = true;
    EnableUserInterface(FALSE);

    CLanguageHelper helper(pConfig);
    helper.SetWndText(&m_BtnDownload, 0x000E0024);

    int nCount = m_LstTools.GetItemCount();
    if (nCount > 0)
    {
        for (int i = 0; i < nCount; i++)
        {
            if (m_LstTools.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
            {
                m_LstTools.EnsureVisible(i, FALSE);

                CTool& tool = this->m_Tools.Get(i);

                m_Download.Download(tool.szUrl, tool.szFile,
                    [this, i](int nProgress, CString szStatus)
                    {
                        for (int s = 0; s < 8; s++)
                        {
                            if (szStatus.Find(pszDownloadStatus[s]) >= 0)
                            {
                                CString szTranslation = pConfig->GetString(0x00400001 + s, pszDownloadStatus[s]);
                                szStatus.Replace(pszDownloadStatus[s], szTranslation);
                            }
                        }

                        m_LstTools.SetItemText(i, TOOL_COLUMN_STATUS, szStatus);
                    });
            }
        }
    }

    helper.SetWndText(&m_BtnDownload, 0x000E0023);
    EnableUserInterface(TRUE);

    bDownload = false;
}

void CToolsDlg::EnableUserInterface(BOOL bEnable)
{
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (bEnable == FALSE)
        pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);
    else
        pSysMenu->EnableMenuItem(SC_CLOSE, MF_ENABLED);

    this->m_EdtName.EnableWindow(bEnable);
    this->m_EdtPlatform.EnableWindow(bEnable);
    this->m_EdtFormats.EnableWindow(bEnable);
    this->m_EdtUrl.EnableWindow(bEnable);
    this->m_EdtFile.EnableWindow(bEnable);
    this->m_EdtExtract.EnableWindow(bEnable);
    this->m_EdtPath.EnableWindow(bEnable);
    this->m_BtnOK.EnableWindow(bEnable);
    this->m_BtnCancel.EnableWindow(bEnable);
    this->m_BtnImport.EnableWindow(bEnable);
    this->m_BtnExport.EnableWindow(bEnable);
    this->m_BtnDuplicate.EnableWindow(bEnable);
    this->m_BtnRemoveAll.EnableWindow(bEnable);
    this->m_BtnRemove.EnableWindow(bEnable);
    this->m_BtnAdd.EnableWindow(bEnable);
    this->m_BtnMoveUp.EnableWindow(bEnable);
    this->m_BtnMoveDown.EnableWindow(bEnable);
    this->m_BtnUpdate.EnableWindow(bEnable);
    this->m_BtnLoad.EnableWindow(bEnable);
    this->m_BtnSave.EnableWindow(bEnable);
}
