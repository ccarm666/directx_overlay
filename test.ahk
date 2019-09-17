ConInit()
ConW("`nThis is text from AHK Script!!! ")
Loop, 10
	ConW("`n# " . A_Index . ": This is More from AHK Script!!! ")

ConUnInit()
Return

ConInit(){
	ATTACH_PARENT_PROCESS = 0x0ffffffff ;
	DllCall("AttachConsole", "UInt", ATTACH_PARENT_PROCESS)
	}
ConUnInit(){
	DllCall("FreeConsole")
}
ConW(txt){
	FileAppend, %txt%, con
	}