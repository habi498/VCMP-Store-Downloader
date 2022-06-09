#include FetchName.ahk
#include OpenUrl.ahk
VERSION:=1001
; for GetServerName(szServerIP, iServerPort)
; Allow the user to maximize or drag-resize the window:
OnError("_OnError")
;%cause% := error
Gui +Resize
Gui, Font, s14
Menu, FileMenu, Add, Refresh/Update List, RefreshMasterList
Menu, FileMenu, Add, Exit, GuiClose
Menu, HelpMenu, Add, About, About
Menu, HelpMenu, Add, What is new, Whatisnew
Menu, HelpMenu, Add, Upgrade All Stores, StoreUpgrade
Menu, HelpMenu, Add, Check for Updates, SoftwareUpdate
Menu, StoreMenu, Add, Add/Update storeto masterlist, AddStore
Menu, StoreMenu, Add, Delete a store from masterlist, DeleteStore
Menu, SearchMenu, Add, Search a server, SearchServer
Menu, MyMenuBar, Add, File, :FileMenu
Menu, MyMenuBar, Add, Store, :StoreMenu
Menu, MyMenuBar, Add, Search, :SearchMenu
Menu, MyMenuBar, Add, Help, :HelpMenu

Menu, ContextMenu, Add, Download Store, DownloadAndInstallStore
Menu, ContextMenu, Add, Copy Server IP, CopyServerIP
Menu, ContextMenu, Add, Copy Store URL, CopyStoreURL
Menu, ContextMenu, Add, Copy Server Name, CopyServerName
Gui, Menu, MyMenuBar ; Attach MyMenuBar to the GUI

Gui, Add, ListView, xm r20 w700 vMyListView gMyListView -Multi AltSubmit, Server|Store URL
Gui, Add, Text,vIP w250, 
SplashTextOn , 300 ,100 , store-gui , Loading...

CreateListView:
Loading:=true
szCmd:="store.exe update"
SplashTextOn , 300 ,100 , store-gui , Loading...`nContacting Master List
RunWait, %szCmd%, ,"Hide"
SplashTextOn , 300 ,100 , store-gui , Loading...`nReading Information
Array := []
szDataFile:="data"
szfileParam1:="Server="
szfileParam2:=", ZipFile="
if FileExist(szDataFile)
{
	Loop
	{
		FileReadLine, line, %szDataFile%, %A_Index%
		if ErrorLevel
			break
		pos:=InStr(line, szfileParam2) 
		if(pos)
		{
			iParam1Len:=StrLen(szfileParam1)
			szServer:=SubStr(line, iParam1Len+1 , pos-iParam1Len-1)
			iParam2Len:=StrLen(szfileParam2)
			szStoreURL:=SubStr(line, pos+iParam2Len)
			iPosColon:=InStr(szServer, ":")
			if(iPosColon)
			{
				szIP:=SubStr(szServer, 1, iPosColon-1)
				iPort:=SubStr(szServer, iPosColon+1)
				;SplashTextOn , 300 ,100 , store-gui , Loading...`n%szServer%
				szServerName:=GetServerName(szIP, iPort)
				if(szServerName)
					LV_Add("", szServerName, szStoreURL)
				else 
					LV_Add("", szServer, szStoreURL)
				Array.Push(Array(szServer,szStoreURL))
				LV_ModifyCol(1, "Auto" )
			}
		}else continue
		;MsgBox, %line%
		
	}
}
SplashTextOn , 300 ,100 , store-gui , Loading...`nCompleted
LV_ModifyCol(1, "Auto" )
SplashTextOff
Loading:=false
Gui, Show
return

MyListView:
if (A_GuiEvent = "DoubleClick")  ; There are many other possible values the script can check.
{
    RowNumber := LV_GetNext(0)  
	global Array
	if RowNumber
	{
		szTargetServer:=Array[RowNumber][1]
		szCmd:="store.exe install " . szTargetServer . " w"
		Run, %szCmd%
	}
}else if(A_GuiEvent = "Normal")
{
	global Array
	szIP:=Array[A_EventInfo][1]
	GuiControl, , IP , %szIP%
	LV_GetText(szServerName, A_EventInfo, 1)
	if(szServerName=szIP)
	{
		iPosColon:=InStr(szIP, ":")
		global Loading
		if(iPosColon && !Loading)
		{
			szIPAddr:=SubStr(szIP, 1, iPosColon-1)
			iPort:=SubStr(szIP, iPosColon+1)
			szServerName:=GetServerName(szIPAddr, iPort)
			if(szServerName)
				LV_Modify(A_EventInfo,"", szServerName)
		}
	}
	
}else if(A_GuiEvent = "I")
{
	If(InStr(ErrorLevel, "S", true))
	{
		global Array
		szIP:=Array[A_EventInfo][1]
		GuiControl, , IP , %szIP%
	}
}
return

GuiEscape:
GuiClose:
ExitApp



About:
Gui +OwnDialogs
MsgBox, Store-Downloader developed for VC-MP by habi
return

AddStore:
OpenUrl("vc-mp-store-downloader.sourceforge.io/index.html")
return

DeleteStore:
Gui +OwnDialogs
MsgBox, To delete any store URL, type "delete" without quotes in Add Store Website.
return

RefreshMasterList:
global Array
LV_Delete()
while(Array.Count()>0)
Array.RemoveAt(0)
Goto, CreateListView
return

Whatisnew:
OpenUrl("https://forum.vc-mp.org/?topic=8833.0")
return

StoreUpgrade:
szCmd:="store.exe upgrade w" ;w- wait for user to close window
RunWait, %szCmd%
return

CopyServerIP:
 RowNumber := LV_GetNext(0)  ;
if RowNumber
{
	global Array
	szTargetServerIP:=Array[RowNumber][1]
	clipboard:=szTargetServerIP
} 
return
CopyStoreURL:
RowNumber := LV_GetNext(0)  ;
if RowNumber
{
	LV_GetText(szTargetStoreURL, RowNumber, 2)
	clipboard:=szTargetStoreURL
} 
return
CopyServerName:
RowNumber := LV_GetNext(0)  ;
if RowNumber
{
	LV_GetText(szTargetServerName, RowNumber, 1)
	clipboard:=szTargetServerName
}
return
DownloadAndInstallStore:
RowNumber := LV_GetNext(0)  ;
if RowNumber
{
	; Copy of event "Double-Click" in LV
	szTargetServer:=Array[RowNumber][1]
	szCmd:="store.exe install " . szTargetServer . " w"
	Run, %szCmd%
} 
return

GuiContextMenu:  ; Launched in response to a right-click or press of the Apps key.
if (A_GuiControl != "MyListView")  ; This check is optional. It displays the menu only for clicks inside the ListView.
    return
; Show the menu at the provided coordinates, A_GuiX and A_GuiY. These should be used
; because they provide correct coordinates even if the user pressed the Apps key:
global Array
	szIP:=Array[A_EventInfo][1]
	GuiControl, , IP , %szIP%
	
Menu, ContextMenu, Show, %A_GuiX%, %A_GuiY%
return

SearchServer:

Gui, +OwnDialogs
InputBox, UserInput, Search Server , Please enter server IP:PORT,
if not ErrorLevel
{
	if not UserInput
	return
	pos:=InStr(UserInput, ":") 
	if pos
	{
		global Array
		Loop % Array.Count()
		{
			szIP:=Array[A_Index][1]
			IfEqual, UserInput, %szIP%
			{
				LV_Modify(A_Index, "Select")
				GuiControl, , IP , %szIP%
				break
			}
			
		}
	}else 
	{
		MsgBox, Error: Port not specified
		goto, SearchServer
	}
}
    
return

SoftwareUpdate:
szCmd1:="store.exe appupdate w"
Run, %szCmd1%
global VERSION
szCmd:="update.exe store-gui " . VERSION . " w" ; wait for user to close window
Run, %szCmd%
return




_OnError(exception) {
	SplashTextOff
	msg:="Error on line " . exception.Line . ": " . exception.Message . "`nClick OK to Exit"
    MsgBox, %msg%
	ExitApp
}