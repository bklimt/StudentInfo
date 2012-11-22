# Microsoft Developer Studio Project File - Name="StudentInfo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=StudentInfo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "StudentInfo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StudentInfo.mak" CFG="StudentInfo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StudentInfo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "StudentInfo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "StudentInfo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "StudentInfo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "StudentInfo - Win32 Release"
# Name "StudentInfo - Win32 Debug"
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Group "GridControl"

# PROP Default_Filter ""
# Begin Group "Helpers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DateEditHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\DateEditHelper.h
# End Source File
# Begin Source File

SOURCE=.\GridControlHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\GridControlHelper.h
# End Source File
# Begin Source File

SOURCE=.\IndexedListBoxHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexedListBoxHelper.h
# End Source File
# Begin Source File

SOURCE=.\StringEditHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\StringEditHelper.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\GridControl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridControl.h
# End Source File
# Begin Source File

SOURCE=.\GridLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\GridLocation.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AssessmentModsButton.cpp
# End Source File
# Begin Source File

SOURCE=.\AssessmentModsButton.h
# End Source File
# Begin Source File

SOURCE=.\CheckIndexedListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckIndexedListBox.h
# End Source File
# Begin Source File

SOURCE=.\DateEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DateEdit.h
# End Source File
# Begin Source File

SOURCE=.\FancyCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\FancyCheckBox.h
# End Source File
# Begin Source File

SOURCE=.\IndexBox.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexBox.h
# End Source File
# Begin Source File

SOURCE=.\IndexedListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexedListBox.h
# End Source File
# Begin Source File

SOURCE=.\MemoryDC.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoryDC.h
# End Source File
# Begin Source File

SOURCE=.\MultiLineEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiLineEdit.h
# End Source File
# Begin Source File

SOURCE=.\PhoneEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\PhoneEdit.h
# End Source File
# Begin Source File

SOURCE=.\SSNEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SSNEdit.h
# End Source File
# Begin Source File

SOURCE=.\StringEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\StringEdit.h
# End Source File
# Begin Source File

SOURCE=.\TabButton.cpp
# End Source File
# Begin Source File

SOURCE=.\TabButton.h
# End Source File
# End Group
# Begin Group "Dialogs and Windows"

# PROP Default_Filter ""
# Begin Group "Student Editor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AssessmentModsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AssessmentModsDlg.h
# End Source File
# Begin Source File

SOURCE=.\AssessmentsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\AssessmentsPage.h
# End Source File
# Begin Source File

SOURCE=.\ContactsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ContactsPage.h
# End Source File
# Begin Source File

SOURCE=.\DemographicsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DemographicsPage.h
# End Source File
# Begin Source File

SOURCE=.\DistributionsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DistributionsPage.h
# End Source File
# Begin Source File

SOURCE=.\EvaluationsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EvaluationsPage.h
# End Source File
# Begin Source File

SOURCE=.\ReferralsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ReferralsPage.h
# End Source File
# Begin Source File

SOURCE=.\ServicesPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ServicesPage.h
# End Source File
# Begin Source File

SOURCE=.\StudentEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StudentEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\StudentEditWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StudentEditWnd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AddListItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddListItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\BackupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BackupDlg.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ListEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ListEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\MainWindow.h
# End Source File
# Begin Source File

SOURCE=.\NewStudentOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewStudentOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\RestoreDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RestoreDlg.h
# End Source File
# Begin Source File

SOURCE=.\StudentChooserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StudentChooserDlg.h
# End Source File
# Begin Source File

SOURCE=.\StudentChooserWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StudentChooserWnd.h
# End Source File
# Begin Source File

SOURCE=.\UserEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserListDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\UserListWnd.h
# End Source File
# End Group
# Begin Group "Storage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BackupHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\BackupHandler.h
# End Source File
# Begin Source File

SOURCE=.\Connection.cpp
# End Source File
# Begin Source File

SOURCE=.\Connection.h
# End Source File
# Begin Source File

SOURCE=.\Preferences.cpp
# End Source File
# Begin Source File

SOURCE=.\Preferences.h
# End Source File
# Begin Source File

SOURCE=.\RestoreHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\RestoreHandler.h
# End Source File
# Begin Source File

SOURCE=.\StorageController.cpp
# End Source File
# Begin Source File

SOURCE=.\StorageController.h
# End Source File
# Begin Source File

SOURCE=.\Table.cpp
# End Source File
# Begin Source File

SOURCE=.\Table.h
# End Source File
# Begin Source File

SOURCE=.\Variant.cpp
# End Source File
# Begin Source File

SOURCE=.\Variant.h
# End Source File
# Begin Source File

SOURCE=.\XMLObject.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLObject.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\check.ico
# End Source File
# Begin Source File

SOURCE=.\mainwindow.ico
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\studentinfo.ico
# End Source File
# Begin Source File

SOURCE=.\toolbar.bmp
# End Source File
# End Group
# Begin Group "Logic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AssessmentRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\AssessmentRecord.h
# End Source File
# Begin Source File

SOURCE=.\Crypto.cpp
# End Source File
# Begin Source File

SOURCE=.\Crypto.h
# End Source File
# Begin Source File

SOURCE=.\Date.cpp
# End Source File
# Begin Source File

SOURCE=.\Date.h
# End Source File
# Begin Source File

SOURCE=.\Distribution.cpp
# End Source File
# Begin Source File

SOURCE=.\Distribution.h
# End Source File
# Begin Source File

SOURCE=.\Evaluation.cpp
# End Source File
# Begin Source File

SOURCE=.\Evaluation.h
# End Source File
# Begin Source File

SOURCE=.\IndexedList.cpp
# End Source File
# Begin Source File

SOURCE=.\IndexedList.h
# End Source File
# Begin Source File

SOURCE=.\Person.cpp
# End Source File
# Begin Source File

SOURCE=.\Person.h
# End Source File
# Begin Source File

SOURCE=.\Service.cpp
# End Source File
# Begin Source File

SOURCE=.\Service.h
# End Source File
# Begin Source File

SOURCE=.\Student.cpp
# End Source File
# Begin Source File

SOURCE=.\Student.h
# End Source File
# Begin Source File

SOURCE=.\StudentYear.cpp
# End Source File
# Begin Source File

SOURCE=.\StudentYear.h
# End Source File
# Begin Source File

SOURCE=.\User.cpp
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\nocheck.ico
# End Source File
# Begin Source File

SOURCE=.\StudentInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StudentInfo.h
# End Source File
# End Target
# End Project
