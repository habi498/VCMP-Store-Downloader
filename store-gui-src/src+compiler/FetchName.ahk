AutoTrim, Off
#include Socket.ahk
;s:=GetServerName("5.2.79.73",8192)
;MsgBox, The server name is %s%
GetServerName(szServerIP, iServerPort)
{

	Client := new SocketUDP()	;creating instance of Socket.ahk class
	Client.Connect([szServerIP, iServerPort]) ; connecting to server

	VarSetCapacity(Buffer,8,0)
	NumPut(0x56,&Buffer)
	NumPut(0x43,&Buffer,1)
	NumPut(0x4d,&Buffer,2)
	NumPut(0x50,&Buffer,3)

	VarSetCapacity(szText, 11)
	iCount:=0
	for index, iByte in StrSplit(szServerIP, ".")
	{
		if iByte is integer
		{
			
			NumPut(iByte, &Buffer, 4+iCount)
			iCount++
		}
	}
	if(iCount==4)
	{
		iPortLessSignificant:=Mod(iServerPort, 256)
		iPortMostSignificatn:=(iServerPort-iPortLessSignificant)/256
		NumPut(iPortLessSignificant, &Buffer, 4+iCount)
		NumPut(iPortMostSignificatn, &Buffer, 4+iCount+1)
		NumPut(0x69,&Buffer, 4+iCount+2)
		bFlag:=0
		sendpacket:
		Client.Send(&Buffer,11)
		try  ; Attempts to execute code.
		{
			r:=Client.Recv(Reply)
			if(r=-1)
			{
				if(!bFlag)
				{
					bFlag:=1
					goto sendpacket
				}
				else return
			}
			iNameB0:=NumGet(Reply, 28, "Char")
			iNameB1:=NumGet(Reply, 29, "Char")
			iNameB2:=NumGet(Reply, 30, "Char")
			iNameB3:=NumGet(Reply, 31, "Char")
			iNameLen:=iNameB0+iNameB1*256+iNameB2*256*256+iNameB3*256*256*256
			;MsgBox, The lowest significant byte of length of server name is %iNameB0%
			;MsgBox, %iNameB0% %iNameB1% %iNameB2% %iNameB3%
			;MsgBox, The length of server name is %iNameLen%
			String:=StrGet(&Reply+32 , iNameLen , Encoding := None)
			return String
		}
		catch e  ; Handles the first error/exception raised by the block above.
		{
			;MsgBox, An exception was thrown!`nSpecifically: %e%
			;Exit
		}
		
	}
}