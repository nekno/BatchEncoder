@echo off
rem
rem BatchEncoder (Audio Conversion GUI)
rem Copyright (C) 2005-2008 Wieslaw Soltes <wisodev@users.sourceforge.net>
rem
rem This program is free software; you can redistribute it and/or modify
rem it under the terms of the GNU General Public License as published by
rem the Free Software Foundation; version 2 of the License.
rem
rem This program is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem GNU General Public License for more details.
rem
rem You should have received a copy of the GNU General Public License
rem along with this program; if not, write to the Free Software
rem Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
rem
rem $Id: GetProgress_BuildDebug.cmd 437 2008-03-24 12:15:45Z wisodev $
setlocal

rem Load environment variables for Visual Studio
rem Default path to MSVC++ 2008:
@call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86

set BasePath=..

devenv /Build "Debug|Win32" "%BasePath%\GetProgress.sln"
devenv /Build "Debug|x64" "%BasePath%\GetProgress.sln"

endlocal