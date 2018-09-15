
	//****************
	class ScsiClass
	{
	public:
		
		BOOLEAN TestBurn;
		BOOLEAN SenseFlag;
		BOOLEAN SenseExtra;
		BOOLEAN CurrentScsiFlag;
		BOOLEAN OpenUnit;//used to get drive list
		BOOLEAN SetUnit;//used to get cddrives
		BOOLEAN CurrentBlock;//used in scsi transfers
		BOOLEAN MediaCalledFlag;//false if checkmedia() has not been called
		BOOLEAN UserRequestPacket;

		BOOL TargetFileFlag;
		BOOL TargetCreated;
		BOOL CueCreated;
		BOOL ReadAllFlag;
		BOOL CatalogEntryAuto;
		BOOL CheckCueFlag;
		BOOL CueFileFlag;
		BOOL CorrectMediaFlag;
		BOOL MediaChangeFlag;
		BYTE Read_Secs;
		BYTE Read_Mins;
		BYTE Read_Hrs;
		BYTE LastCommandError;
		BYTE DriveCount;
		BYTE CopyModeSense[128];
		BYTE CurrentDiskStatus;
		BYTE CurrentTrack;
		BYTE CurrentSessionStatus;
		BYTE TypeWrite;
		BYTE TypeWriteInProgress;
		BYTE SectorBytes[2048];


		char AlternateCatalog[16];
		char CueFileName[128];
		char SenseError[256];
		WCHAR LastCommandString[128];
		WCHAR LastPacketTime[128];
		char TargetFileName[128];
		char CurrentScsiLetter[2];

		int Time_Hrs;
		int Time_Mins;
		int Time_Secs;
		int Total_Secs;

		int CommandIndex;

		WORD SenseLenght;

		DWORD ScsiDataReturnLen;
		DWORD Read_TimeSecs;
		DWORD CurrentJob;
		BOOL HighPreTimerOk;
		LARGE_INTEGER OffsetIntoFile;
		LARGE_INTEGER TargetFileSize;

		LARGE_INTEGER StartingTime;
		LARGE_INTEGER EndingTime;
		double ElapsedNanoseconds;
		double ElapsedMicroseconds;
		double ElapsedMilliseconds;
		double ElapsedSeconds;
		LARGE_INTEGER Frequency;
		LARGE_INTEGER LastDelay;

		CDDRIVES_INFO CdInfo;

		CUE_SHEET_INFO CueSheetInfo;

		DRIVE_LIST DriveList;
		FEATURE_LIST FeatureList;
		MODE_LIST ModeList;
		HANDLE CurrentScsiHandle;
		
		RMD_FIELD_ZERO FieldZero;
		
		HANDLE hCueFile;//for read / write
		HANDLE hDataFile;//for read / write
		HANDLE hTrackFile;
		HANDLE hReadthread;//thread for read
		HANDLE hWritethread;//thread for write
		HANDLE hBlankthread;//thread for blanking
		bool fDataFileFlag;//for read / write
		bool fCueFileFlag;//for read / write
		bool fTrackFileFlag;

		int LastMedia;
		HWND Current_Dialog;

		PACKET_DRIVER packet;

		MEDIA_DESCRIBTOR MediaInfo;

		SCSI_BLOCK_USER spwb;
		SCSI_BLOCK_USER w_spwb;
		SYSTEM_INFO SystemInfo;

		OPENFILENAMEW ReadDataFile;
		bool ReadDataFileFlag;

		OPENFILENAMEW WriteDataFile;
		bool WriteDataFileFlag;

		WCHAR wTemp[256] = { 0 };

		VOLUME_DESCRIPTOR PrimaryVolume;

		WAVE_HEADER file_header = { 0X46464952,0,0X45564157,0X20746D66,0X10,1,2,0XAC44,0X2B110,4,0X10,0X61746164,0 };

		VOLUME_DESCRIPTOR VolumeDes;
		DIRECTORY_LIST DirList;
		DISPLAY Display;
		//****************** Subs **************************
		ScsiClass();
		~ScsiClass();
		UCHAR	BlankDisc(PACKET_DRIVER packet);
		UCHAR	CloseDoor(PACKET_DRIVER packet);
		UCHAR	CloseTrack(PACKET_DRIVER packet);
		BOOLEAN CheckString(char * string, char * database, WORD * len);
		//BOOL	DelayNow(LARGE_INTEGER Amount);
		UCHAR	FormatDisc(PACKET_DRIVER packet);
		UCHAR	GetConfiguration(PACKET_DRIVER packet);
		UCHAR	GetEvent(PACKET_DRIVER packet);
		UCHAR	GetFeatures(PACKET_DRIVER packet);
		bool	GetFileForDvdRead(PACKET_DRIVER packet,int ReportLog);
		bool	GetFileForDvdWrite(PACKET_DRIVER packet,int ReportLog);
		UCHAR	GetPerformance(PACKET_DRIVER packet);
		UCHAR	GetInquiry(PACKET_DRIVER packet);
		bool	GetStartTime();
		bool	GetEndTime();
	    	UCHAR	LoadUnLoad(PACKET_DRIVER packet);
		UCHAR	MechStatus(PACKET_DRIVER packet);
		UCHAR	ModeSelect(PACKET_DRIVER packet);
		UCHAR	ModeSense(PACKET_DRIVER packet);
		UCHAR	OpenCdUnit();
		UCHAR	OpenDoor(PACKET_DRIVER packet);
		UCHAR	PauseResume(PACKET_DRIVER packet);
		UCHAR	PlayAudio(PACKET_DRIVER packet);
		UCHAR	PlayAudioMsf(PACKET_DRIVER packet);
		UCHAR	PlayLba(PACKET_DRIVER packet);
		UCHAR	SendScsi(PACKET_DRIVER packet);
		UCHAR	PlayMsf(PACKET_DRIVER packet);
		UCHAR	PreventAllow(PACKET_DRIVER packet);
		UCHAR	Read10Full(PACKET_DRIVER packet);
		UCHAR	Read10Part(PACKET_DRIVER packet);
		UCHAR	Read12(PACKET_DRIVER packet);
		UCHAR	ReadBuffer(PACKET_DRIVER packet);
		UCHAR	ReadBufferCapacity(PACKET_DRIVER packet);
		UCHAR	ReadCapacity(PACKET_DRIVER packet);
		UCHAR	ReadCDFull(PACKET_DRIVER packet);
		UCHAR	ReadCDPart(PACKET_DRIVER packet);
		UCHAR	ReadCDMsf(PACKET_DRIVER packet);
		UCHAR	ReadDiscInfo(PACKET_DRIVER packet);
		UCHAR	ReadDvdStructure(PACKET_DRIVER packet);
		UCHAR	ReadFormatCapacity(PACKET_DRIVER packet);
		UCHAR	ReadHeader(PACKET_DRIVER packet);
		UCHAR	ReadSubChannel(PACKET_DRIVER packet);
		UCHAR	ReadTocPmaAtip(PACKET_DRIVER packet);
		UCHAR	ReadTrackInfo(PACKET_DRIVER packet);
		UCHAR	RepairRzone(PACKET_DRIVER packet);
		UCHAR	ReportKey(PACKET_DRIVER packet);
		bool	ReportMedia();
		UCHAR	RequestSense(PACKET_DRIVER packet);
		UCHAR	ReserveTrackRzone(PACKET_DRIVER packet);
		UCHAR	Scan(PACKET_DRIVER packet);
		UCHAR	Seek(PACKET_DRIVER packet);
		UCHAR	SendCue(PACKET_DRIVER packet);
		UCHAR	SendDvdStructure(PACKET_DRIVER packet);
		UCHAR	SendEvent(PACKET_DRIVER packet);
		UCHAR	SendKey(PACKET_DRIVER packet);
		UCHAR	SendOpcInfo(PACKET_DRIVER packet);
		UCHAR	SendTestCommand(PACKET_DRIVER packet);
		UCHAR	SetCdSpeed(PACKET_DRIVER packet);
		UCHAR	SetCdDrives();
		UCHAR	SetReadAhead(PACKET_DRIVER packet);
		WORD	SetStringChars(char *buffer, int amount, char* chars);
		UCHAR	SetStreaming(PACKET_DRIVER packet);
		UCHAR	ShowDigits(DISPLAY *Display);
		UCHAR	StartStopUnit(PACKET_DRIVER packet);
		UCHAR	StopPlayScan(PACKET_DRIVER packet);
		UCHAR	SynCache(PACKET_DRIVER packet);
		UCHAR	TestUnitReady(PACKET_DRIVER packet);
		UCHAR	Verify10(PACKET_DRIVER packet);
		UCHAR	Write10(PACKET_DRIVER packet);
		UCHAR	WriteCDFull(PACKET_DRIVER packet);
		UCHAR	WriteCDPart(PACKET_DRIVER packet);
		UCHAR	Write12(PACKET_DRIVER packet);
		UCHAR	WriteVerify10(PACKET_DRIVER packet);
		UCHAR	WriteBuffer(PACKET_DRIVER packet);
		BOOLEAN DisplaySense(BYTE SenseBuffer);
		DWORD	BswapDword(DWORD *swap_bytes);
		WORD	BswapWord(WORD *swap_bytes);
		UCHAR	BuildCueSheet();
		UCHAR	CheckMedia();
		UCHAR	WaitUntillReady(int *count);
		UCHAR	SendCueSheet();
		UCHAR	SetWriteMode();
		UCHAR	ReSetWriteMode();

		BOOLEAN AddMsfMsf(MSF one, MSF two, MSF *answer);
		MSF		ConvertLbaMsf(DWORD Lba);
		MSF		ConvertBytesToMsf(BYTE *);
		DWORD	ConvertMsfLba(MSF msf);
		bool	GetTrackFile();
		bool	GetTracksFromCd(PACKET_DRIVER packet);
		BOOLEAN SubMsfMsf(MSF biggest, MSF smallest, MSF *answer);
	};






