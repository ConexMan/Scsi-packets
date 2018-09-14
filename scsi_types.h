
#include <WINIOCTL.H>
#define SCSI_IOCTL_DATA_OUT						0
#define SCSI_IOCTL_DATA_IN						1
#define SCSI_IOCTL_DATA_UNSPECIFIED				2
#define IOCTL_SCSI_BASE							FILE_DEVICE_CONTROLLER


#define IOCTL_SCSI_PASS_THROUGH					CTL_CODE(IOCTL_SCSI_BASE, 0x0401, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SCSI_MINIPORT						CTL_CODE(IOCTL_SCSI_BASE, 0x0402, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SCSI_GET_INQUIRY_DATA				CTL_CODE(IOCTL_SCSI_BASE, 0x0403, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_GET_CAPABILITIES				CTL_CODE(IOCTL_SCSI_BASE, 0x0404, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_PASS_THROUGH_DIRECT			CTL_CODE(IOCTL_SCSI_BASE, 0x0405, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_SCSI_GET_ADDRESS					CTL_CODE(IOCTL_SCSI_BASE, 0x0406, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_RESCAN_BUS					CTL_CODE(IOCTL_SCSI_BASE, 0x0407, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_GET_DUMP_POINTERS			CTL_CODE(IOCTL_SCSI_BASE, 0x0408, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SCSI_FREE_DUMP_POINTERS			CTL_CODE(IOCTL_SCSI_BASE, 0x0409, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IDE_PASS_THROUGH					CTL_CODE(IOCTL_SCSI_BASE, 0x040a, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_ATA_PASS_THROUGH					CTL_CODE(IOCTL_SCSI_BASE, 0x040b, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_ATA_PASS_THROUGH_DIRECT			CTL_CODE(IOCTL_SCSI_BASE, 0x040c, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)		

#define WRITE_DVD_DATA							1//write types requested by user
#define WRITE_DVD_VIDEO							2
#define WRITE_CD_AUDIO							3
#define WRITE_CD_DATA							4
#define WRITE_CD_XA								5
#define WRITE_CD_I								6
#define READ_CD_DATA							7

#define ROM_XA									0
#define UN_KNOWN								1
#define NO_MEDIA								2
#define AUDIO_DATA								3
#define MODE_1									4
#define ROM_I									5
#define NONE									2

#define DATADIRECTIONWRITE						0
#define DATADIRECTIONREAD						1

#define AUDIO_BLOCK_SIZE						16
#define DATA_BLOCK_SIZE							16
#define AUDIO_SECTOR_SIZE						2352									
#define MINIUM_AUDIO_SECTORS					150
#define MINIUM_FILE_SECTORS						16
#define AUDIO_BLOCK_SIZE_DVD					16
#define MAX_TRACKS								99
#define MAX_CDROM_DRIVES						4
#define MAX_CDROMS_OPENED						1
#define NO_CDROMS_FOUND							2
#define CDROMS_OK								3
#define MAX_CDDRIVES							4
#define DRIVE1									0
#define DRIVE2									1
#define DRIVE3									2
#define DRIVE4									3

#define PROFILE_CD_ROM							8
#define PROFILE_CD_R							9
#define PROFILE_CD_RW							0XA
#define PROFILE_DVD_ROM							0X10
#define PROFILE_DVD_R							0X11
#define PROFILE_DVD_RAM							0X12
#define PROFILE_DVD_RW_RES						0X13
#define PROFILE_DVD_RW_SEQ						0X14
#define PROFILE_DVD_R_DL_SEQ					0X15
#define PROFILE_DVD_R_DL_JUMP					0X16
#define PROFILE_DVD_RW_DL						0X17
#define PROFILE_DVD_DOWNLOAD					0X18
#define PROFILE_DVD_PLUS_RW						0X1A
#define PROFILE_DVD_PLUS_R						0X1B
#define PROFILE_BD_ROM							0X40
#define PROFILE_BD_R_SEQ						0X41
#define PROFILE_BD_R_RRM						0X42
#define PROFILE_BD__RE							0X43
#define PROFILE_HD_DVD_ROM						0X50
#define PROFILE_HD_DVD_R						0X51
#define PROFILE_HD_DVD_RAM						0X52
#define PROFILE_HD_DVD_RW						0X53
#define PROFILE_HD_DVD_R_DL						0X58
#define PROFILE_HD_DVD_RW_DL					0X5A

#define STATUS_EMPTY							0
#define STATUS_INCOMPLETE						1
#define STATUS_COMPLETE							2
#define STATUS_OTHER							3

#define FULL_FORMAT								0
#define SPARE_AREA_EXPANSION					1
#define RW_FULL_FORMAT							0X10
#define GROW_SESSION							0X11
#define QUICK_GROW_BORDER						0X13
#define QUICK_FORMAT							0X15


#define NO_PAUSE								0
#define ONE_SECOND_PAUSE						75
#define TWO_SECOND_PAUSE						150

#define SESSION_AT_ONCE							2
#define DISC_AT_ONCE							2
#define TRACK_AT_ONCE							1
#define WRITE_DONE_OK							3

#define RESERVED_MEDIA							0
#define UNFORMATTED_MEDIA						1
#define FORMATTED_MEDIA							2
#define RAW_DATA								3
#define NO_MEDIA_PRESENT						4

#define CDROM_HANDLE_FAILED						0
#define CDROM_HANDLE_OK							1
#define CDROM_FOUND_OK							2

#define CD_DA_ROM								0
#define CD_ROM_I								0X10
#define CD_ROM_XA								0X20
#define CD_UNDEFINED							0XFF

#define DEVICEIOCONTROL_OK						0
#define CUE_FILE_ERROR							1
#define FIND_TRACK_OK							2
#define FAILED_NO_TRACKINFO						3
#define GET_TRACK_INFO_OK						4
#define FAILED_SOURCE_DATA						5
#define FAILED_CUESHEET							6
#define FAILED_TRACKEND							7
#define DEVICEIOCONTROL_FAILED					0X80
#define DEVICEIOCONTROL_SENSE					0X81
#define MAX_DRIVES_FOUND						0X82  

#define NO_MEDIA_IN_DRIVE						0
#define MEDIA_DOOR_OPEN							1
#define DVD_MEDIA_IN_DRIVE						2
#define CD_MEDIA_IN_DRIVE_UNKNOWN				3
#define CD_MEDIA_IN_DRIVE_AUDIO_RW				10
#define CD_MEDIA_IN_DRIVE_AUDIO_ROM				11
#define CD_MEDIA_IN_DRIVE_DATA_RW				12
#define CD_MEDIA_IN_DRIVE_DATA_ROM				13
#define MEDIA_IN_DRIVE_UNKNOWN					4
#define DVD_MEDIA								15
#define CD_MEDIA								16
#define NO_TARGET_HANDLE						17
#define CD_MEDIA_IN_DRIVE_XA					18
#define CD_MEDIA_IN_DRIVE_I						19
#define CD_MEDIA_IN_DRIVE_DA_ROM				20
#define CD_MEDIA_IN_DRIVE_UNDEFINED				21
#define MEDIA_IN_DRIVE_BLANK_CD					22
#define MEDIA_NOT_READY							23
#define CD_AUDIO_MEDIA_RW						24
#define CD_AUDIO_MEDIA_ROM						25
#define CD_MEDIA_INCOMPLETE						26


#define DVD_MEDIA_ROM							4
#define DVD_MEDIA_RAM							5
#define DVD_MEDIA_R								6
#define DVD_MEDIA_MINUS_RW						7
#define DVD_MEDIA_PLUS_RW						8
#define DVD_MEDIA_UNKNOWN						9
#define CD_RW_MEDIA								10
#define DVD_MEDIA_PLUS_R						11

#define CD_MEDIA_XA								12
#define CD_MEDIA_DA_ROM 						13
#define CD_MEDIA_UNDEFINED						14
#define CD_MEDIA_DATA_ROM						15
#define CD_RW_MEDIA_BLANK						16

#define BOOK_DVD_ROM							0
#define BOOK_DVD_RAM							1
#define BOOK_DVD_MINUS_R						2
#define BOOK_DVD_MINUS_RW						3
#define BOOK_DVD_PLUS_RW						9
#define BOOK_DVD_PLUS_R							10

#define DVD										1
#define CD										3
#define UNKNOWN									255
#define RESERVED_UNIT							15

#define CD_DATA_ROM								0
#define CD_AUDIO_ROM							1
#define CD_XA_ROM								2
#define CD_XA_I									3
#define DVD_DATA								4
#define DVD_VIDEO								5
#define CD_MEDIA_IN_DRIVE						6
#define CD_DATA_RW								7


#define	REMOVABLE_DISK							2
#define	CD_ROM									8
#define	CD_R									9
#define	CD_RW									0XA
#define	DVD_ROM									0X10
#define	DVD_R_SEQ								0X11
#define	DVD_RAM									0X12
#define	DVD_RW_RES								0X13
#define	DVD_RW_SEQ								0X14
#define	DVD_R_DL_SEQ							0X15
#define	DVD_R_DL_JUMP							0X16
#define	DVD_RW_DL								0X17
#define	DVD_DOWNLOAD							0X18
#define	DVD_PLUS_RW								0X19
#define	DVD_PLUS_R								0X1B
#define	BD_ROM									0X40
#define	BD_R_SEQ								0X41
#define	BD_R_RAN								0X42
#define	BD_RE									0X43
#define	HD_DVD_ROM								0X50
#define	HD_DVD_R								0X51
#define	HD_DVD_RAM								0X52
#define	HD_DVD_RW								0X53
#define	HD_DVD_R_DL								0X58
#define HD_DVD_RW_DL							0X5A

#define BLANK_QUICK								0X11
#define BLANK_FULL								0X10

#define EMPTY_SESSION							0
#define INCOMPLETE_SESSION						1
#define DAMAGED_BORDER							2
#define COMPLETE_SESSION						3

#define EMPTY_DISC								0
#define INCOMPLETE_DISC							1
#define COMPLETE_DISC							2
#define FINALIZATION_SUSPENDED					3


#define BLANK									0
#define CLOSE_TRACK								1
#define COMPARE									2
#define ERASE									3
#define FORMAT									4
#define CONFIGURATION							5
#define EVENT_STATUS							6
#define PERFORMANCE								7
#define INQUIRY									8
#define LOAD_UNLOAD								9
#define LOCK_UNLOCK_CACHE						10
#define LOG_SELECT								11
#define LOG_SENSE								12
#define MECHANISM_STATUS						13
#define MODE_SELECT								14
#define MODE_SENSE								15
#define NO_COMMAND								16
#define PAUSE_RESUME							17
#define PLAY_AUDIO_MSF							18
#define PLAY_AUDIO								19
#define PLAY_CD									20
#define PRE_FETCH								21
#define PREVENT_ALLOW							22
#define READ_BUFFER								23
#define READ_BUFFER_CAPACITY					24
#define READ_CAPACITY							25
#define READ_CD									26
#define READ_CD_MSF								27
#define READ_DISC_INFO							28
#define READ_DVD_STRUCTURE						29
#define READ_FORMAT_CAPACITIES					30
#define READ_HEADER								31
#define READ_SUBCHANNEL							32
#define READ_TOC_PMA_ATIP						33
#define READ_TRACK_INFO							34
#define READ_10									35
#define READ_12									36
#define RECEIVE_DIAGNOSTIC						37
#define RELEASE_10								38
#define RELEASE_6								39
#define REPAIR_ZONE								40
#define REPORT_KEY								41
#define REQUEST_SENSE							42
#define RESERVE_TRACK							43
#define RESERVE_10								44
#define RESERVE_6								45
#define SCAN									46
#define SEEK									47
#define SEND_CUE_SHEET							48
#define SEND_DIAGNOSTIC							49
#define SEND_DVD_STRUCTURE						50
#define SEND_EVENT								51
#define SEND_KEY								52
#define SEND_OPC_INFO							53
#define SET_CD_SPEED							54
#define SET_READ_AHEAD							55
#define SET_STREAMING							56
#define START_STOP								57
#define STOP_PLAY_SCAN							58
#define SYNCHRONIZE_CACHE						59
#define TEST_UNIT_READY							60
#define VERIFY_10								61
#define WRITE_VERIFY_10							62
#define WRITE_BUFFER							63
#define WRITE_10								64
#define WRITE_12								65

#define ZRESET_PRIMARY							66
#define ZRESET_SECONDARY						67

#define READ_NO_SUBDATA							0
#define READ_RAW_SUBDATA						1
#define READ_Q_SUBDATA							2
#define READ_RESERVED_SUBDATA					3
#define READ_RW_SUBDATA							4

#define CD_SPEED_1X								176
#define CD_SPEED_2X								353
#define CD_SPEED_4X								706
#define CD_SPEED_6X								1059
#define CD_SPEED_8X								1400
#define CD_SPEED_10X							1760
#define CD_SPEED_12X							2112
#define CD_SPEED_16X							2800
#define CD_SPEED_20X							3520
#define CD_SPEED_24X							4226
#define CD_SPEED_32X							5600
#define CD_SPEED_40X							7040
#define CD_SPEED_64X							11200
//*************** Current jobs *******************
#define READ_DVD								1
#define WRITE_DVD								2
#define BLANKQUICK_DVD							3
#define BLANKFULL_DVD							4
#define CLOSE_DOOR								5
#define OPEN_DOOR								6

//********************************************
typedef struct _STREAMING_DESCRIPTOR
{
	BYTE		Mrw			: 1;
	BYTE       Exact		: 1;
	BYTE		Rdd			: 1;
	BYTE		Wrc			: 2;
	BYTE		Hie			: 1;
	BYTE		Res_1		: 2;

	BYTE		Res_2;
	BYTE		Res_3;
	BYTE		Res_4;

	DWORD StartLba;
	DWORD EndLba;
	DWORD ReadSize;
	DWORD ReadTime;
	DWORD WriteSize;
	DWORD WriteTime;
}STREAMING_DESCRIPTOR,*LPSTREAMING_DESCRIPTOR;

//********************************************
typedef struct _MSF
{
	BYTE Unsed_byte;
	BYTE Minute;
	BYTE Second;
	BYTE Frame;
}MSF,*LPMSF;

//************************************************
typedef struct _DEVICE_NAMES_FOUND
{
	char Vendor_id[16];
	char Product_id[24];
	char Revision_id[8];
	char CompleteID[64];
	char ProductPlusLetter[64];

	WCHAR wVendor_id[32];
	WCHAR wProduct_id[64];
	WCHAR wRevision_id[32];
	WCHAR wCompleteID[128];
	WCHAR wProductPlusLetter[128];
}DEVICE_NAMES_FOUND,*LPDEVICE_NAMES_FOUND;
//*********************************************

#pragma pack(1)
struct _TRACK_DESCRIPTOR
{
	BYTE  Resv_Data1;

	BYTE  Control		:4;
	BYTE  Adr			:4;

	BYTE  Track_Number;
	BYTE  Resv_Data2;
	BYTE  Msf_Unsed;
	BYTE  Msf_Minute ;
	BYTE  Msf_Second;
	BYTE  Msf_Frame;
}TRACK_DESCRIPTOR,*LPTRACK_DESCRIPTOR;

//******************************************
// Command 0X43 Funtion 0 Toc Msf 

typedef struct _TOCMSF_INFORMATION
{
	BYTE  TocDataLengthMsb;
	BYTE  TocDataLengthLsb;
	BYTE  First_Track;
	BYTE  Last_Track;
struct _TRACK_DESCRIPTOR Tracks[MAX_TRACKS] ;	
}TOCMSF_INFORMATION,*LPTOCMSF_INFORMATION;

//*********************************************
#pragma pack()
typedef struct _CDDRIVES_DETAILS
{
	HANDLE ScsiHandle;
	bool DriveValidFlag;
	char DriveLetter[2];
	DWORD ErrorCode;
	struct _DEVICE_NAMES_FOUND DriveName;
}CDDRIVES_DETAILS,*LPCDDRIVES_DETAILS;
//************************************************
typedef struct _MODE_HEADER//For mode sense page code 0X2A
{
	BYTE DataLenMsb;
	BYTE DataLenLsb;
	BYTE TypeCode;
	BYTE Reserved1;
	BYTE Reserved2;
	BYTE Reserved3;
	WORD BlockWord;

}MODE_HEADER, *LPMODE_HEADER;
//*********************************************

struct _SPEED_DESCRIPTOR
{
	BYTE  Resv_Data1;
	BYTE  Rotation_Control	: 2;
	BYTE  Resv_Data_2		: 6;
	WORD  Write_Speed;
}SPEED_DESCRIPTOR;


//*********************************************
#pragma pack(1)
 typedef struct _FEATURE_HEADER//for GetConfiguration
{
	DWORD DataLength;//must be bswapped
	BYTE Reserved1;
	BYTE Reserved2;
	WORD CurrentProfile;//must be byte swapped
}FEATURE_HEADER,*LPFEATURE_HEADER;
 //****************************************************
 typedef struct  _FEATURE_MRW// MRW 0X28
 {
	 _FEATURE_HEADER Header;
	BYTE FeatureCodeMsb;
	BYTE FeatureCodeLsb;

	BYTE Current		: 1;
	BYTE Persistent		: 1;
	BYTE Version		: 4;
	BYTE Reserved_1		: 2;

	BYTE AdditionalLenght;

   BYTE Write			:1;
   BYTE Res_1			:7;

  BYTE Res_2[3];
 }FEATURE_MRW, *LPFEATURE_MRW;
//******************************************************
typedef struct  _FEATURE_CD_MASTERING// CD MASTERING 0X2E
{
_FEATURE_HEADER Header;
BYTE FeatureCodeMsb;
BYTE FeatureCodeLsb;

BYTE Current			:1;
BYTE Persistent			:1;
BYTE Version			:4;
BYTE Reserved_1			:2;

BYTE AdditionalLenght;

BYTE R_W				:1;//indicate that the logical unit can record the R-W subchannels with user supplied information
BYTE Cd_Rw				:1;//indicate that the logical unit can overwrite previously recorded data.
BYTE TestWrite			:1;//indicate that the logical unit can perform test writes.
BYTE Raw				:1;//indicate that the logical unit can record using the raw write type
BYTE RawMs				:1;//indicate that the logical unit can record Multi-Session in raw mode.
BYTE Sao				:1;//indicate that the logical unit can record using the Session-at-Once write type.
BYTE Buf				:1;//shall indicate that the logical unit supports Buffer Underrun Free
BYTE Reserved_2			:1;

BYTE MaxCueLenghtMsb;
BYTE MaxCueLenghtMid;
BYTE MaxCueLenghtLsb;
}FEATURE_CD_MASTERING,*LPFEATURE_CD_MASTERING;
//**************************************************************
typedef struct  _FEATURE_DVD_READ// 0X1F DVD READ
{
	_FEATURE_HEADER Header;
	BYTE FeatureCodeMsb;
	BYTE FeatureCodeLsb;

	BYTE Current		: 1;
	BYTE Persistent		: 1;
	BYTE Version		: 4;
	BYTE Reserved_1		: 2;

	BYTE AdditionalLenght;

	BYTE Multi110		: 1;
	BYTE Reserved_2		: 7;

	BYTE Reserved_3;

	BYTE Dual_Minus_r	: 1;
	BYTE Dual_Minus_rw	: 1;
	BYTE Reserved_4		: 6;

	BYTE Reserved_5;

}FEATURE_DVD_READ, *LPFEATURE_DVD_READ;
//********************************************
typedef struct _FEATURE_DVD_PLUS//0X2A DVD+RW
{
	_FEATURE_HEADER Header;

	BYTE FeatureCodeMsb;
	BYTE FeatureCodeLsb;

	BYTE Current		: 1;
	BYTE Persistent		: 1;
	BYTE Version		: 4;
	BYTE Reserved1		: 2;

	BYTE AdditionalLength;

	BYTE Write		: 1;
	BYTE Reserved2	: 7;

	BYTE CloseOnly	: 1;
	BYTE QuickStart	: 1;
	BYTE Reserved3	: 6;

	BYTE Reserved4;
	BYTE Reserved5;
}FEATURE_DVD_PLUS, *LPFEATURE_DVD_PLUS;
//********************************************
typedef struct  _FEATURE_CD_TAO// CD TRACK AT ONCE 0X2D
{
	_FEATURE_HEADER Header;
	BYTE FeatureCodeMsb;
	BYTE FeatureCodeLsb;

	BYTE Current			: 1;
	BYTE Persistent			: 1;
	BYTE Version			: 4;
	BYTE Reserved_1			: 2;

	BYTE AdditionalLenght;

	BYTE Write_RW_SubCode	: 1;
	BYTE CD_RW_OverWrite	: 1;
	BYTE TestWrite			: 1;
	BYTE Write_RW_Pack		: 1;
	BYTE Write_RW_Raw		: 1;
	BYTE Reserved_2			: 1;
	BYTE Buffer_Overrun		: 1;
	BYTE Reserved_3			: 1;

	BYTE Reserved_4;
	WORD Data_Block_Support;

}FEATURE_CD_TAO, *LPFEATURE_CD_TAO;
//*******************************************
typedef struct  _FEATURE_DVD_MINUS// CD TRACK AT ONCE 0X2D
{
	_FEATURE_HEADER Header;
	BYTE FeatureCodeMsb;
	BYTE FeatureCodeLsb;

	BYTE Current		: 1;
	BYTE Persistent		: 1;
	BYTE Version		: 4;
	BYTE Reserved_1		: 2;

	BYTE AdditionalLenght;

	BYTE Reserved_2		: 1;
	BYTE DvdRw			: 1;
	BYTE TestWrite		: 1;
	BYTE ReadDL_R		: 1;
	BYTE Reserved2		: 2;
	BYTE Buffer_Underrun: 1;
	BYTE Reserved_3		: 1;

	BYTE Reserved_4;
	BYTE Reserved_5;
	BYTE Reserved_6;

}FEATURE_DVD_MINUS, *LPFEATURE_DVD_MINUS;
//*//*******************************************
typedef struct  _FEATURE_DVD_PLUS_R// CD TRACK AT ONCE 0X3B
{
	_FEATURE_HEADER Header;
	BYTE FeatureCodeMsb;
	BYTE FeatureCodeLsb;

	BYTE Current		: 1;
	BYTE Persistent		: 1;
	BYTE Version		: 4;
	BYTE Reserved_1		: 2;

	BYTE AdditionalLenght;

	BYTE DvdPlusRWrite	: 1;
	BYTE Reserved_2		: 7;

	BYTE Reserved_3;
	BYTE Reserved_4;
	BYTE Reserved_5;

}FEATURE_DVD_PLUS_R, *LPFEATURE_DVD_PLUS_R;
//*********************************************
typedef struct _FEATURE_REAL_STREAM
{
	_FEATURE_HEADER Header;
	BYTE FeatureCodeMsb;
	BYTE FeatureCodeLsb;

	BYTE Current			: 1;
	BYTE Persistent			: 1;
	BYTE Version			: 4;
	BYTE Reserved_1			: 2;
	BYTE AdditionalLenght;

	BYTE StreamWrite		: 1;
	BYTE WriteDes			: 1;
	BYTE MecCaps			: 1;
	BYTE SetCdSpeed			: 1;
	BYTE ReadBuffer			: 1;
	BYTE SetHie				: 1;
	BYTE Reserved_2			: 2;

	BYTE Reserved_3;
	BYTE Reserved_4;
	BYTE Reserved_5;
}FEATURE_REAL_STREAM,*LPFEATURE_REAL_STREAM;
//********************************************

#pragma pack()
typedef struct _CDDRIVES_INFO
{
	BYTE AmountCdDrives;
	HANDLE CurrentScsiDrive;
	char CurrentLetter[2];
	struct _CDDRIVES_DETAILS Details[MAX_CDDRIVES];
}CDDRIVES_INFO,*LPCDDRIVES_INFO;


//*********************************************
#pragma pack(1)	
typedef	struct _READ_CAPACITY_ZERO
{
	WORD  DataLength;
	BYTE  Reserved1;
	BYTE  Reserved2;
	DWORD BufferSizeBytes;		//must be bswapped
	DWORD BufferFreeBytes;	//must be bswapped
}READ_CAPACITY_ZERO,*LPREAD_CAPACITY_ZERO;
//*********************************************
#pragma pack(1)	
typedef	struct _READ_CAPACITY_ONE
{
	WORD  DataLength;
	BYTE  Reserved1;
	BYTE  Reserved2;
	DWORD Reserved3;
	DWORD BufferFreeBlocks;	//must be bswapped
}READ_CAPACITY_ONE, *LPREAD_CAPACITY_ONE;

//*******************************************

typedef struct _FEATURE_DESCRIPTOR
{
	WORD ProfileNumber;//must be byte swapped

	BYTE Current           :1;
	BYTE Peristent         :1;
	BYTE Version           :4;
	BYTE Reserved1			:2;
	BYTE AddLenght;
}FEATURE_DESCRIPTOR,*LPFEATURE_DESCRIPTOR;
//*********************************************
typedef struct _PROFILE_DESCRIPTOR
{
	WORD ProfileNumber;//must be byte swapped
	BYTE CurrentP			:1;
	BYTE Reserved1			:7;
	BYTE Reserved2;
}PROFILE_DESCRIPTOR,*LPPROFILE_DESCRIPTOR;

//*********************************************
typedef struct _PROFILE_LIST
{
	FEATURE_HEADER Header;
	FEATURE_DESCRIPTOR ListDiscriptor;
	PROFILE_DESCRIPTOR Profile[64];
}PROFILE_LIST,*LPPROFILE_LIST;


//*********************************************
typedef struct _WAVE_HEADER//44Bytes
{
	DWORD Riff;					//"RIFF" 0X52,0X49,0X46,0X46
	DWORD ChunkSize;			//Size of This Block - 8(Block = 44 -8 = 36) + size of SubChunk2Size
	DWORD Format;				//"WAVE" 0X57,0X41,0X56,0X45
	DWORD SubChunk1ID;			//"fmt " 0X66,0X6D,0X74,0X20
	DWORD SubChunk1Size;		// 16 for PCM
	WORD  AudioFormat;			//PCM = 1
	WORD  NumChannels;			//Streo = 2
	DWORD SampleRate;			//Cds use 44100 = 0XAC44
	DWORD ByteRate;				//cds use 176400 = 0X2B110
	WORD  BlockAlign;			//Number of Channels * BitsPerSample / 8
	WORD  BitsPerSample;		//16 = 16 bits etc
	DWORD SubChunk2ID;			//"data" 0X64,0X61,0X74,0X61
	DWORD SubChunk2Size;		//Size of Data thats follows this

}WAVE_HEADER,*LPWAVE_HEADER; 
#pragma pack()	
//***************************************	
typedef struct _SCSI_PASS_THROUGH 
{
  USHORT   Length;
  UCHAR     ScsiStatus;
  UCHAR     PathId;
  UCHAR     TargetId;
  UCHAR     Lun;
  UCHAR     CdbLength;
  UCHAR     SenseInfoLength;
  UCHAR     DataIn;
  ULONG     DataTransferLength;
  ULONG     TimeOutValue;
  ULONGLONG 	DataBufferOffset;
  ULONG		SenseInfoOffset;
  UCHAR     Cdb[16];
} SCSI_PASS_THROUGH, *PSCSI_PASS_THROUGH;
//*********************************************
typedef struct _SCSI_BLOCK_USER
{
	SCSI_PASS_THROUGH Block;
	ULONG64 filler;
	UCHAR Sense_Data[64];
	UCHAR Data_Returned[2448 * AUDIO_BLOCK_SIZE];
}SCSI_BLOCK_USER, *LPSCSI_BLOCK_USER;

//******************************************
#pragma pack()	
typedef struct _DRIVE_LIST
{
	UINT DriveTypes[26];
}DRIVE_LIST,*LPDRIVE_LIST;
//************************************************************* Mode Sence *******************************
//*******************************************
typedef struct _ALL_MODES  // Page Code 0
{
	MODE_HEADER Header;
	BYTE Buffer[512];
}ALL_MODES, *LPALL_MODES;
//*****************************************
typedef struct _MOUNT_RAINIER  // Page Code 3
{
	MODE_HEADER Header;
	BYTE  Page_Code			: 6;//0
	BYTE  Resv_Data2		: 1;
	BYTE  PS				: 1;

	BYTE  Page_Lenght;//1
	BYTE  Reserved_1;

	BYTE  LbaSpace			: 1;
	BYTE  Reserved_2		: 7;
	BYTE  Reserved_3[4];
}MOUNT_RAINIER, *LPMOUNT_RAINIER;
//********************************************
typedef struct _ERROR_RECOVERY  // Page Code1
{
	MODE_HEADER Header;
	BYTE  Page_Code		: 6;//0
	BYTE  Resv_Data2	: 1;
	BYTE	PS			: 1;

	BYTE  Page_Lenght;//1

	BYTE DCR			: 1;
	BYTE DTE			: 1;
	BYTE PER			: 1;
	BYTE Res1			: 1;
	BYTE RC				: 1;
	BYTE TB				: 1;
	BYTE ARRE			: 1;
	BYTE AWRE			: 1;

	BYTE ReadCount;
	BYTE CorrectionSpan;
	BYTE HeadOffsetCount;
	BYTE DataStobeOffsetCount;

	BYTE Emcdr			: 2;
	BYTE Res2			: 6;

	BYTE WriteCount;
	BYTE ThresholdMsb;
	BYTE ThresholdMid;
	BYTE ThresholdLsb;
}ERROR_RECOVERY, *LPERROR_RECOVERY;
//*******************************************
#pragma pack(1)	
typedef struct _WRITE_PAGE  // Page Code 05
{
	MODE_HEADER Header;
//Now mode page
	BYTE  Page_Code			:6;
	BYTE  Resv_Data1		:1;
	BYTE  PS				:1;

	BYTE  Page_Lenght;

	BYTE  Write_Type		:4;
	BYTE  Test_Write		:1;
	BYTE  LS_V				:1;
	BYTE  BUFE				:1;
	BYTE  Resv_Data2		:1;

	BYTE  Track_Mode		:4;
	BYTE  Copy				:1;
	BYTE  FP				:1;
	BYTE  Multi_Session		:2;  

	BYTE  Data_Block_Type	:4;
	BYTE  Resv_Data3		:4;

	BYTE  Link_Size;
	BYTE  Resv_Data4;

	BYTE  Host_Code 		:6;
	BYTE  Resv_Data5		:2;

	BYTE  Session_Format;
	BYTE  Resv_Data6;

	BYTE  Packet_Size_MSB;
	BYTE  Packet_Size_1;
	BYTE  Packet_Size_2;
	BYTE  Packet_Size_LSB;

	BYTE  Audio_Pause_Lenght_MSB;
	BYTE  Audio_Pause_Lenght_LSB;

	BYTE  Media_Cat[16];
    BYTE  ISRC[16];
	BYTE  Sub_Bytes[4];

}WRITE_PAGE,*LPWRITE_PAGE;
//**********************************************
typedef struct _CACH_MODE  // Mode Sence 8
{
	MODE_HEADER Header;
	BYTE  Page_Code			: 6;
	BYTE  Resv_Data2		: 1;
	BYTE  PS				: 1;

	BYTE  Page_Lenght;//1

	BYTE Rcd			: 1;
	BYTE Res_1			: 1;
	BYTE Wc				: 1;
	BYTE Res_2			: 5;

	BYTE Res_3[9];
}CACH_MODE, *LPCACH_MODE;


//******************************************************
typedef struct _EXCEPTION_CONTROL  // Mode Sence 0X1C
{
	MODE_HEADER Header;
	BYTE  Page_Code			: 6;
	BYTE  Resv_Data2		: 1;
	BYTE  PS				: 1;

	BYTE  Page_Lenght;//1
	BYTE	LogErr			: 1;
	BYTE	Reserved_1		: 1;
	BYTE	Test			: 1;
	BYTE	DExept			: 1;
	BYTE	EWasc			: 1;
	BYTE	Reserved_2		: 2;
	BYTE	Perf			: 1;

	BYTE	Mrie			: 4;
	BYTE	Reserved_3		: 4;

	DWORD InternalTimer;//needs Bswap
	DWORD ReportCount;//needs Bswap
}EXCEPTION_CONTROL, *LPEXCEPTION_CONTROL;
//*******************************************************
typedef struct _CAPABILITIES  // Mode Sence 0X2A
{
	MODE_HEADER Header;

	BYTE  Page_Code			: 6;
	BYTE  Resv_Data2		: 1;
	BYTE  PS				: 1;

	BYTE  Page_Lenght;//1

	BYTE  CD_R_Read			: 1;//2
	BYTE  CD_RW_Read		: 1;
	BYTE  Method2			: 1;
	BYTE  DVD_ROM_Read		: 1;
	BYTE  DVD_R_Read		: 1;
	BYTE  DVD_RAM_Read		: 1;
	BYTE  Resv_Data3		: 2;

	BYTE  CD_R_Write		: 1;//3
	BYTE  CD_RW_Write		: 1;
	BYTE  Test_Write		: 1;
	BYTE  Resv_Data4		: 1;
	BYTE  DVD_R_Write		: 1;
	BYTE  DVD_RAM_Write		: 1;
	BYTE  Resv_Data5		: 2;

	BYTE  Audio_Play		: 1;//4
	BYTE  Composite			: 1;
	BYTE  Digital_Port1		: 1;
	BYTE  Digital_Port2		: 1;
	BYTE  Mode2_Form1		: 1;
	BYTE  Mode2_Form2		: 1;
	BYTE  Multi_Session		: 1;
	BYTE  Resv_Data6		: 1;

	BYTE  CD_DA				: 1;//5
	BYTE  CDDA_Accurate		: 1;
	BYTE  R_W_Supported		: 1;
	BYTE  R_W_DC			: 1;
	BYTE  C2_Pointers		: 1;
	BYTE  ISRC				: 1;
	BYTE  UPC				: 1;
	BYTE  Read_Bar_Capable	: 1;

	BYTE  Lock				: 1;//6
	BYTE  Lock_State		: 1;
	BYTE  Prevent_Jumper	: 1;
	BYTE  Eject				: 1;
	BYTE  Resv_Data7		: 1;
	BYTE  LMT				: 3;

	BYTE  Sep_Vol			: 1;//7
	BYTE  Sep_Channel_Mute	: 1;
	BYTE  Disc_Present		: 1;
	BYTE  Slot_Selection	: 1;
	BYTE  Side_Capable		: 1;
	BYTE  R_W_Leadin_Read	: 1;
	BYTE  Resv_Data8		: 2;

	WORD  Obsolete_1;//8/9

	WORD  Vol_Levels;//10/11
	WORD  Buffer_Size;//12/13
	WORD  Obsolete_2;//14/15

	BYTE  Obsolete_3;//16

	BYTE  Resv_Data9		: 1;//17
	BYTE  BCKF				: 1;
	BYTE  RCK				: 1;
	BYTE  LSBF				: 1;
	BYTE  Lenght_BCKs: 2;
	BYTE  Resv_Data10		: 2;

	WORD  Obsolete_4;//18/19
	WORD  Obsolete_5;//20/21              
	WORD  Copy_Manger_Rev;//22/23
	BYTE  Resv_Data11[3];//24/25/26

	BYTE  Rotation_Control : 2;//27  
	BYTE  Resv_Data12		: 6;

	WORD  Current_Write_Speed;//28/29
	WORD  Write_Speed_Tables;//30/31
	struct _SPEED_DESCRIPTOR  Speed_Write_Data[16];//32 / 128

}CAPABILITIES, *LPCAPABILITIES;
//************************************************************************** Mode Sense END ************************
//*****************************************************
typedef struct _FEATURE_LIST
{
	CAPABILITIES Capabilities;				//uses Modesense 0X2A

	FEATURE_MRW MountRainier;				//uses Feature 0x28
	FEATURE_DVD_PLUS DvdPlusRWDetails;		//uses Feature 0X2A
	FEATURE_CD_TAO TrackAtOnce;				//uses Feature 0X2D
	FEATURE_CD_MASTERING CdMastering;		//uses feature 0X2E
	FEATURE_DVD_MINUS DvdMinus;				//uses feature 0X2F
	FEATURE_DVD_PLUS_R DvdPlusR;			//uses feature 0X3B
	FEATURE_REAL_STREAM RealStreaming;//uses feature 0x107
}FEATURE_LIST, *LPFEATURE_LIST;
//***************************************
typedef struct _MODE_LIST
{
	
	ALL_MODES AllModes;						//uses ModePage 0 (Get all)
	ERROR_RECOVERY ErrorRecovery;			//uses ModePage 1 (get Recovery)
	MOUNT_RAINIER MountRainier;				//uses ModePage 3 (get MRW):MMC (maybe Feature  0x28)
	WRITE_PAGE  ModeWrite;					//uses ModePage 5 (get write)
	CACH_MODE CachingMode;					//uses ModePage 8 (get caching):MMC
	CAPABILITIES Capabilities;				//uses Modesense 0X2A
}MODE_LIST, *LPMODE_LIST;
//**********************************************
typedef struct _DVD_HEADER
{
	BYTE  Data_Lenght_Msb;
	BYTE  Data_Lenght_LSB;
	BYTE  Resv_Data_2;
	BYTE  Resv_Data_2a;	
}DVD_HEADER;
//***********************************************
#pragma pack(1)	
typedef struct _ADIP_INFO
{
	struct _DVD_HEADER DvdHeader ;
	BYTE DiskVersion		: 4;//0 should be 2
	BYTE DiskCatagory		: 4;//bit 7 = 1 disk to +R/+RW:bit6 = 0 single layer:bit5-4 = 01 = +RW

	BYTE DiskMaxRead		: 4;//1 max Read transfer = 1111 = no max
	BYTE DiskSize				: 4;//disksize = 0000 = 120mm

	BYTE DiskStructure	;//2 Bit7-4 = 0000 :bits3- 0 =0100 indicating a rewritable recording layer
	BYTE RecordDensity;//3 track pitch of 0,74 μm and Channel bit length indicating 0,133 μm
	BYTE PhysicalSector;//4 not used
	BYTE  PhysicalSectorMsb;//5 msb start of PhysicalSector(030000)
	BYTE  PhysicalSectorMid;//6 mid start of PhysicalSector(030000)
	BYTE  PhysicalSectorLsb;//7 lsb start of PhysicalSector(030000)

	BYTE LastSector;//8 not used
	BYTE  LastSectorMsb;//9 msb start of PhysicalSector(026053f)
	BYTE  LastSectorMid;//10 mid start of PhysicalSector(030000)
	BYTE  LastSectorLsb;//11 lsb start of PhysicalSector(030000)
	DWORD Res_1;//12 13 14 15

	BYTE Flagsb7		: 1;//16 set to 0
	BYTE Flagsb6		: 1;//= 0 no Extended format information : =1 Data Zone contains Extended format
	BYTE Flagsb5		: 1;//Res
	BYTE Flagsb4_0	: 5;//Set to zero

	BYTE DiskAppCode;//17 (00) identifies a disk for General Purpose use

	BYTE ExtenInfob7_6 : 2;//18 set to 00
	BYTE ExtenInfob5_0 : 6;//block. Bit bi shall be set to 1 if Extended Information block
	char  DiskManufacturerID[8];//19,20,21,22,23,24,25,26
	char  MediaTypeID[3];//27,28,29
	BYTE ProductRevision;//30
	BYTE rem[220];
}ADIP_INFO,*LPADIP_INFO;
//***********************************************
typedef struct _PRE_PIT_INFO
{
	BYTE  Field_id_1;
	BYTE  Application_Code;
	BYTE  Disc_Physical_Code;
	BYTE  Last_Record_Address[3];//MSB _ LSB
	BYTE  ExtensionCode				:4;
	BYTE  LppPartVersion			:4;
	BYTE  Reserved_1;

	BYTE  Field_id_2;
	BYTE  Opc_Record_Power;
	BYTE  Opc_Erase_Power;
	BYTE  Write_Code_1[4];
	BYTE  Reserved_2;

	BYTE  Field_id_3;
	char  Manufacture_id_1[6];
	BYTE  Reserved_3;

	BYTE  Field_id_4;
	char  Manufacture_id_2[6];
	BYTE  Reserved_4;

	BYTE  Field_id_5;
	BYTE Write_Code_2[6];
	BYTE Reserved_5[25];
}PRE_PIT_INFO,*LPPRE_PIT_INFO;
//*************************************************
typedef struct _UNIQUE_DISC
{
	WORD  Reserved_1;
	WORD  Random_Date;
	DWORD Year;
	WORD  Month;
	WORD  Day;
	WORD  Hour;
	WORD  Minute;
	WORD  Second;
}UNIQUE_DISC,*LPUNIQUE_DISC;
//*************************************************
typedef struct _RMD_FIELD_ZERO
{
	struct _DVD_HEADER DvdHeader;
	BYTE LastRmaMsb;
	BYTE LastRmaHigh;
	BYTE LastRmaMid;
	BYTE LastRmaLsb;

	BYTE RmdFormatMsb;
	BYTE RmdFormatLsb;
	BYTE DiscStatus;
	BYTE Reserved_1;
	UNIQUE_DISC UniqueDiscID;
	PRE_PIT_INFO PrePitInfo;
	BYTE Reserved_2[42];
	BYTE RbgInfo;
	BYTE Reserved_3[1919];
}RMD_FIELD_ZERO,*LPRMD_FIELD_ZERO;
//*************************************************
//For _DVD Read Structure
typedef struct _DISKCONTROL_BLOCK_FFFFFFFF
{
struct _DVD_HEADER    Physical_Info;	
DWORD CurrentDescriptor;//Msb _ Lsb needs bswap
DWORD Reserved_1;
char Vendor_ID[32];
BYTE Reserved_2;
BYTE NumberOfReadableDcds;
BYTE Reserved_3;
BYTE NumberOfRecordedDcds;
DWORD ReadableDcbs[8];                  
}DISKCONTROL_BLOCK_FFFFFFFF	;


//For _DVD Read Structure**********************
typedef struct _FORMAT_1_FIELD_0
{
struct _DVD_HEADER    Physical_Info;	
DWORD Last_Rmd_set;	//Msb _ Lsb needs bswap

WORD Rmd_Format;	//Msb - Lsb needs bswap
BYTE Disk_Status;
BYTE Reserved_1;
struct _UNIQUE_DISC Unique_Disc;
struct _PRE_PIT_INFO Pre_Pit;
BYTE Reserved_2[1920];         
}FORMAT_1_FIELD_0;
//*******************************************

//For _DVD Read Structure
typedef struct _FORMAT_1_FIELD_DATA
{
char Drive_Id[32];                       
char Serial_Number[16];
char Model_Number[16];
BYTE FirstWrite_code_1[4];	//Msb - lsb needs bswap
BYTE Recording_Power[4];
BYTE Time_Stamp[8];
BYTE Power_Calibration_Address[4];
BYTE Running_Opc_Information[24];                   
BYTE SecondWrite_code[6];
BYTE Dsv[2];
BYTE Reserved1[12];
}FORMAT_1_FIELD_DATA;
//*******************************************
//For _DVD Read Structure
typedef struct _FORMAT_1_FIELD_1
{
struct _DVD_HEADER Physical_Info;	
DWORD Last_Rmd_set;	//Msb _ Lsb needs bswap
struct _FORMAT_1_FIELD_DATA FieldData[4];
BYTE Reseved2[1536];
}FORMAT_1_FIELD_1;
//******************************************
//For _DVD Read Structure

//*******************************************
typedef struct _FORMAT_2_FIELD_0
{
struct _DVD_HEADER    DvdHeader;	
DWORD Last_Rmd_set;	//Msb _ Lsb needs bswap
WORD Rmd_Format;	//Msb - Lsb needs bswap
BYTE Disk_Status;
BYTE Reserved_1;
struct _UNIQUE_DISC Unique_Disc;
struct _PRE_PIT_INFO Pre_Pit;
BYTE Reserved_2[42];
BYTE RBG_Information;
BYTE Reserved_3[1919];                  
}FORMAT_2_FIELD_0;
//*******************************************
//*******************************************
//For _DVD Read Structure
typedef struct _FORMAT_2_FIELD_1
{
struct _DVD_HEADER    Physical_Info	;	
DWORD Last_Rmd_set;	//Msb _ Lsb needs bswap
DWORD Update_Counter;                       
DWORD Format_3_PTR;
DWORD Reserved_1;
WORD  Erase_Counter;//Msb - lsb needs bswap                   
BYTE  Reserved_2[2048];                  
}FORMAT_2_FIELD_1;
//*******************************************
//For _DVD Read Structure
typedef struct _FORMAT_2_FIELD_2
{
struct _DVD_HEADER    Physical_Info;	
DWORD Last_Rmd_set;	//Msb _ Lsb needs bswap
BYTE  Erase_Code;                       
BYTE  Reserved_1;
DWORD Erase_Info_1;
DWORD Erase_Info_2;	//Msb - lsb needs bswap                   
BYTE  Reserved_3[2048];                  
}FORMAT_2_FIELD_2;
//*******************************************
//For _DVD Read Structure
typedef struct _FORMAT_3_FIELD_3
{
struct _DVD_HEADER    Physical_Info;	
DWORD Last_Rmd_set;		//Msb _ Lsb needs bswap
BYTE  Format_Code;                       
BYTE  Reserved_1;
DWORD Format_Info_1;
DWORD Format_Info_2;	//Msb - lsb needs bswap  
DWORD Reserved_2;
DWORD Start_Border_Out;

BYTE  Bam_1					:1;
BYTE  Defect_1				:1;
BYTE  Reserved_3			:6;

BYTE Reserved_4;                
BYTE  Reserved_5[2048];                  
}FORMAT_3_FIELD_3;
//*******************************************
//For _DVD Read Structure
typedef struct _DVD_STRUCTURE_0
{
struct _DVD_HEADER    Physical_Info;	
BYTE Part_Version			:4;
BYTE Book_Type				:4;//0=dvd_rom:1=dvd_ram:2=dvd_r:3=dvd-rw:9=dvd+rw

BYTE Max_Rate				:4;
BYTE Disc_Size				:4;

BYTE Layer_Type				:4;
BYTE Track_Path             :1;
BYTE Amount_Layers			:2;
BYTE Reserved_1				:1;

BYTE Track_Density			:4;
BYTE Linear_Density			:4;

BYTE Reserved_2;                            

BYTE Start_Physical_Msb;
BYTE Start_Physical_1;
BYTE Start_Physical_Lsb;

BYTE Reserved_3;                             

BYTE End_Physical_Msb;
BYTE End_Physical_1;
BYTE End_Physical_Lsb;

BYTE Reserved_4;
BYTE End_Sector_Msb;
BYTE End_Sector_1;
BYTE End_Sector_Lsb;

BYTE Reserved_5				:7;
BYTE BCA_Flag				:1;

BYTE Media_Specific[2031]             ;                  
}DVD_STRUCTURE_0,*LPDVD_STRUCTURE_0;
//*******************************************
struct _ERASE_FORMAT
{
	WORD DVD_Valid				:1;
	WORD CD_Valid				:1;
	WORD Format_Valid			:1;
	WORD Erase_Valid			:1;
	WORD DVD_Format_Quick		:1;
	WORD DVD_Format_Full		:1;
	WORD DVD_Erase_Full			:1;
	WORD DVD_Erase_Quick		:1;
	WORD Spare_1				:1;
	WORD Spare_2				:1;
	WORD CD_Erase_Full			:1;
	WORD CD_Erase_Quick			:1;
	WORD CD_Format_Zone			:1;
	WORD CD_Format_Spare		:1;
	WORD CD_Format_Zone_Reformat:1;
	WORD CD_Format_Full			:1;
}ERASE_FORMAT;
#pragma pack()	
//*********************************************
typedef struct _PACKET_DRIVER
{ 
	WCHAR wBuffer[128];
	DWORD AccessCreate;
	HWND hDlg;
	UINT OutPutWindow;
    HANDLE hcd_device;
	DWORD Amount_Transfer;
	DWORD Amount_Sectors;
	DWORD Size_Sectors;
	int	SizeOfWriteData;
	DWORD BufferSize;
	DWORD UsedSectors;
	DWORD LastError;
	DWORD FileCount;
	ULARGE_INTEGER LFileCount;
	DWORD TypeOfWrite;
	DWORD CurrentJob;

	BYTE BlankingType;						//blanking			data
	BOOLEAN BlankingFlag;					//TRUE Blanking allowed
	DWORD BufferAvailableTotalBlocks;		//used in Writing data to cd/dvd
	DWORD BufferAvilableCurrentBlocks;		//used in Writing data to cd/dvd
	DWORD BufferTotalSizeBytes;				//used in Writing data to cd/dvd
	BOOL WriteAndVerityFlag;				//used if we are going to verity
	BYTE CloseType;							//close track/sess data
	BYTE CloseTrackMsb;						//close track/sess data
	BYTE CloseTrackLsb;						//close track/sess data
	BYTE ClassType;							//getevent data
	BYTE Cdb[12];							//Used in Test Lab data
	BOOL DataDirectionFlag;					//true = read :false = write
	BYTE FormatType;						//format data
	BYTE FeatureMSB;						//configuration	data
	BYTE FeatureLSB;						//configuration	data
	BOOLEAN WaveHeaderFlag;					//true = add header fale = do not
    BYTE PerformDataType;					//performance		data
	BYTE PerformStartMSB;					//performance		data
	BYTE PerformStartHIGH;					//performance		data
	BYTE PerformStartLOW;					//performance		data
	BYTE PerformStartLSB;					//performance		data
	BYTE PerformType;						//performance		data
	BYTE PerformMaxMsb;						//performance		data
	BYTE PerformMaxLsb;						//performance		data
	WORD Progress_Indicator;				//used in WaitUntillDone(class ScsiClass *scsi)//(23)
	BYTE LoadUload;							//Load/Unload		data
	BYTE ModeSelectMSB;						//ModeSelect(10)	data
	BYTE ModeSelectLSB;						//ModeSelect(10)	data
	BYTE ModeSelectWriteType;				//ModeSelect(10)	data
	BYTE ModeSenseLenMsb;					//ModeSense(10)    data
	BYTE ModeSenseLenLsb;					//ModeSense(10)    data
	BYTE ModeSenseCode;						//ModeSense(10)    data
	BYTE PageCode;							//ModeSense(10)    data
	BYTE StartM;							//Play Audio(MSF)  data
	BYTE StartS;							//Play Audio(MSF)  data
	BYTE StartF;							//Play Audio(MSF)  data
	BYTE EndingM;							//Play Audio(MSF)  data
	BYTE EndingS;							//Play Audio(MSF)  data
	BYTE EndingF;							//Play Audio(MSF)  data
	DWORD StartPlay;						//Used in PlayCd (LBA)
	DWORD PlayLenght;						//Used in PlayCd (LBA)
	BYTE Resume;							//used in Pause/Resume data 0=hold/1=resume
	DWORD ReadCDLba;						//Used in ReadCD   data
	BYTE Readcd_Expected;					//Used in ReadCD   data
	BYTE Readcd_Flags;						//Used in ReadCD   data
	BYTE Readcd_Sub;						//Used in ReadCD   data
	BYTE ReadByte;							//used in Read(10)/(12) data
	DWORD Read10Lba;						//used in Read10 data
	BYTE ReadBufferMode;					//Used in ReadBuffer data
	BYTE ReadBufferId;						//Used in ReadBuffer data
	BYTE BufferOffsetMsb;					//Used in ReadBuffer data
	BYTE BufferOffsetHigh;					//Used in ReadBuffer data
	BYTE BufferOffsetLsb;					//Used in ReadBuffer data
	BYTE BufferAllocateMsb;					//Used in ReadBuffer data
	BYTE BufferAllocateHigh;				//Used in ReadBuffer data
	BYTE BufferAllocateLsb;					//Used in ReadBuffer data
	BYTE ReadBufferCapacityBlock;			//Used in ReadBufferCapacity
	DWORD Read12TransferLength;				//used in Read(12)
	DWORD Read12Lba;						//used in Read(12)
	BYTE ReadStreaming;						//used in Read(12)
	BYTE PreventAllow;						//used in PreventAllow data bits 0,1 only
	DWORD ReadDvdAddress;					//Used in ReadDvdStructure data
	BYTE ReadDvdLayer;						//Used in ReadDvdStructure data
	BYTE ReadDvdFormat;						//Used in ReadDvdStructure data
	BYTE ReadHeaderMsf;						//Used in ReadHeader data
	DWORD ReadHeaderAddress;				//Used in ReadHeader data
	DWORD OldReadHeaderAddress;				//Used in ReadHeader data to reinstate old address
	BYTE ReadSubMsf;						//Used in ReadSubChannel data
	BYTE ReadSubFormat;						//Used in ReadSubChannel data
	bool ReadSubCodeStatus;					//Used in ReadSubChannel data
	BYTE ReadSubCodeType;					//Used in ReadSubChannel data 0 = none:1=raw:2=q:3=res:4=rw
	BYTE ReadSubTrack;						//Used in ReadSubChannel data
	BYTE ReadTocMsf;						//Used in ReadTocPmaAtipChannel data
	BYTE ReadTocFormat;						//Used in ReadTocPmaAtipChannel data
	BYTE ReadTocTrack;						//Used in ReadTocPmaAtipChannel data
    BYTE ReadTrackInfoType;					//Used in ReadTrackInfo data
	DWORD ReadTrackInfoAddress;				//Used in ReadTrackInfo data
	BYTE ReadTrackAllocateMsb;				//Used in ReadTrackInfo data
	BYTE ReadTrackAllocateLsb;				//Used in ReadTrackInfo data
	BYTE RepairZoneMsb;						//Used in RepairRzone data
	BYTE RepairZoneLsb;						//Used in RepairRzone data
	DWORD ReportKeyAddress;					//Used in ReportKey data
	BYTE ReportKeyClass;					//Used in ReportKey data
	BYTE ReportKeyFormatAgid;				//Used in ReportKey data
	DWORD ReservationSize;					//Used in ReserveTrackRzone data
	BYTE  ReservationType;					//Used in ReserveTrackRzone data
	DWORD ScanStart;						//Used in Scan data
	BYTE ScanType;							//Used in Scan data
	BYTE ScanDirection;						//Used in Scan data (0X10 = Fast Reversed 0 = Fast Forward)
	DWORD SeekAddress;						//Used in SeeK data
	BYTE SendCueMsb;						//Used in SendCueSheet data
	BYTE SendCueData;						//Used in SendCueSheet data
	BYTE SendCueHigh;						//Used in SendCueSheet data
	BYTE SendCueLsb;						//Used in SendCueSheet data
	BYTE SendDvdStructureFormat;			//Used in SendDvdStructure data
	BYTE SendDvdStructureMsb;				//Used in SendDvdStructure data
	BYTE SendDvdStructureLsb;				//Used in SendDvdStructure data
	BYTE SendEventListMsb;					//Used in SendEvent data
	BYTE SendEventListLsb;					//Used in SendEvent data
	BYTE SendKeyClass;						//Used in SendKey data
	BYTE SendKeyMsb;						//Used in SendKey data
	BYTE SendEKeyLsb;						//Used in SendKey data
	BYTE SendKeyAgidFormat;					//Used in SendKey data
	BYTE SendOpcFlag;						//Used In SendOpcInfo data
	BYTE SendOpcEx;							//Used In SendOpcInfo data
	BYTE SendOpcListMsb;					//Used In SendOpcInfo data
	BYTE SendOpcListLsb;					//Used In SendOpcInfo data

	BYTE SetCdSpeedControl;					//Used in SetCdSpeed data (0 = CLV 1 = CAV)
	BYTE SetCdSpeedReadMsb;					//Used in SetCdSpeed data
	BYTE SetCdSpeedReadLsb;					//Used in SetCdSpeed data
	BYTE SetCdSpeedWriteMsb;				//Used in SetCdSpeed data
	BYTE SetCdSpeedWriteLsb;				//Used in SetCdSpeed data
	bool SetCdWriteAutoFlag;				//Used in SetCdSpeed data
	bool SetCdReadAutoFlag;					//Used in SetCdspeed data
	bool SetCdResetFlag;					//Used in SetCdSpeed data
	DWORD SetCdWriteSpeed;					//Used in SetCdSpeed data
	DWORD SetCdReadSpeed;					//Used in SetCdSpeed data

	DWORD SetReadAheadTrigger;				//Used in SetReadAhead data
	DWORD SetReadAheadBlock;				//Used in SetReadAhead data

	BYTE SetStreamingListMsb;				//Used in SetStreaming data
	BYTE SetStreamingListLsb;				//Used in SetStreaming data
	bool SetStreamingWriteAutoFlag;			//Used in SetStreaming data
    bool SetStreamingReadAutoFlag;			//Used in SetStreaming data
	bool SetStreamingResetFlag;				//Used in SetStreaming data
	DWORD SetStreamingWriteSpeed;			//Used in SetStreaming data
	DWORD SetStreamingReadSpeed;			//Used in SetStreaming data
	STREAMING_DESCRIPTOR SetStreamingDes;	//Used in SetStreaming data

	BOOLEAN ShowSense;						//Used in SendScsi in show sense data if request failed
	BYTE StartStop;							//Used in StartStop data bits 7-4 = power bits 3-2 = res bits 1-0 = loej/start
	BOOLEAN StopExe;						//Used to stop current Active sub Function
	BYTE SynImmediate;						//used in Syncache command
	BOOLEAN ScsiTimeOutFlag;				//Used to signal new Time Out value Needed
	DWORD ScsiTimeOutValue;					//Used to set new TimeOut Value (Value = Seconds)
	BOOLEAN RestartExe;						//Used to Restart Header at last position
	BYTE Verify10Flags;						//Used in Verify10 data
	DWORD Verify10Lba;						//Used in Verify10 data
	BYTE Verify10LengthMsb;					//Used in Verify10 data
	BYTE Verify10LengthLsb;					//Used in Verify10 data
	BYTE Verify12Flags;						//Used in Verify12 data
	DWORD Verify12Lba;						//Used in Verify12 data
	BYTE Verify12LengthMsb;					//Used in Verify12 data
	BYTE Verify12LengthLsb;					//Used in Verify12 data
	DWORD Write10Lba;						//Used in Write10 data
	BYTE Write10Byte1;						//Used in Write10 data
    BYTE Write12Streaming;					//Used in Write12 data bit 7 = streaming 
	DWORD Write12Lba;						//Used in Write12 data
	BYTE Write12Byte1;						//Used in Write12 data
	BYTE WriteBufferMode;					//Used in WriteBuffer data
	BYTE WriteBufferId;						//Used in WriteBuffer data
	BOOLEAN WriteType;						//true = Data : False = Audio
	LARGE_INTEGER FileSize;					//used in Writing to CD/DVD size of File to be written
}PACKET_DRIVER,*LPPACKET_DRIVER;
//*******************************************
#pragma pack(1)	
typedef struct _MECHSTATUS
{
	BYTE Current_Slot			:5;//slot selected
	BYTE Charger_State			:2;//0=ready:1=loading:2=unloading:3=initializing
	BYTE Fault					:1;//charger Failed

	BYTE Reserved1				:4;//reserved
	BYTE Door_Open				:1;//1=door is open
	BYTE Mechstate              :3;//0=idle:1=active:2=scaning:3=active host:4-6=res:7=no info

	DWORD CurrentLba;				//last used position for read/play//MSB_LSB needs byteswap
	BYTE Amount_slots_Available	:6;//number of slots supported
	BYTE Reserved2							:2;//reserved

	BYTE Len_slot_msb;				//Msb of lenght of slot info
	BYTE Len_slot_lsb;				//Lsb of lenght of slot info
}MECHSTATUS,*LPMECHSTATUS;

//******************************************

typedef struct _DISKINFO
{
	BYTE DiskLenMsb;//diskinfo lenght Msb
	BYTE DiskLenLsb;//diskinfo lenght Lsb

	BYTE DiscStatus					:2;//0=empty:1=incomplete:2=complete:3=others
	BYTE SessionStatus				:2;//0=empty:1=incomplete:2=res/damaged border:3=complete
	BYTE Erasable					:1;//0=not erasable:1=erasable
	BYTE Reserved1					:3;//resevered

	BYTE FirstTrack;					//first Track On Disc
	BYTE AmountSessionsLsb;				//amount of sessions/borders First Lsb
	BYTE FirstTrackLsb;					//First Track in Last Session Lsb
	BYTE LastTrackLsb;					//Last Track in Last Session Lsb

	BYTE Reserved2					:5;//resvered
	BYTE Urc								:1;//Unrestricted Use 1=any use:0=restricted use only
	BYTE Dbc_V							:1;//Disc_Bar_Valid 1=valid:0=invalid
	BYTE Did_V							:1;//Disc Id Valid 1=valid:0=invalid

	BYTE DiscType;//0=CD_DA or CD_ROM:0X10=CD_I:0X20=CD_ROM XA:0XFF= Undefined all others Reserved
	BYTE AmountSessionsMsb;//amount of sessions/borders First Msb
	BYTE FirstTrackMsb;//First Track in Last Session Msb
	BYTE LastTrackMsb;//Last Track in Last Session Msb

	DWORD DiscID						;//Disc Identification Msb - Lsb (needs Byteswap)
	struct _MSF LeadInLastSessMsf       ;//Lead In start Time Last Session (MSF format)(needs Byteswap)
	struct _MSF LastPosStartTimeMsf     ;//Last Possible Strat Time for Lead Out (MSF format)(needs Byteswap)

	BYTE DiscBarCode[8];//Disc_Bar_Code
	BYTE Reserved3;//reserved
	BYTE AmountOfOpcTables;//Amount Of OPC Table Entries
	BYTE OpcTables[32];//OPC Tables
}DISKINFO,*LPDISKINFO;
//***************************************
struct _TRACK_CUE_INFO 
{
	BYTE Ctl_Adr     ;
	BYTE Track_Number;
	BYTE Index_Number;
	BYTE Data_Form   ;
	BYTE Scms_Byte   ;
	BYTE Abs_Minute  ;
	BYTE Abs_Second  ;
	BYTE Abs_Frame   ;
}TRACK_CUE_INFO,*LPTRACK_CUE_INFO;
//************************************************
typedef struct _CUE_SHEET
{
	struct _TRACK_CUE_INFO Track_Cue_Info[MAX_TRACKS];
}CUE_SHEET,*LPCUE_SHEET;
//*******************************************
struct _SUB_CHANNEL_HEADER
{
	BYTE Reserved1;
	BYTE Audio_Status;
	BYTE Sub_LengthMsb;
	BYTE Sub_LengthLsb;
}SUB_CHANNEL_HEADER,*LPSUB_CHANNEL_HEADER;
//*******************************************
struct _CDTEXT_PACK
{
	BYTE PackTypeIndicator;

	BYTE TrackIndicator			:7;
	BYTE ExtensionFlag			:1;

	BYTE SequenceIndicator ;

	BYTE CharacterPosition		:4;
	BYTE BlockNumber			:3;
	BYTE DoubleCharCode			:1;
	
	BYTE TextOrDataField[12];
	BYTE CrcField0;
	BYTE CrcField1;

}CDTEXT_PACK,*LPCDTEXT_PACK;
//****************************************
typedef struct _CDTEXT_INFO
{
	BYTE CdTextLenMsb;
	BYTE CdTextLenLsb;
	BYTE Res1;
	BYTE Res2;
	struct _CDTEXT_PACK PackLists[1024];
}CDTEXT_INFO,*LPCDTEXT_INFO;
//*******************************************
struct _TRACK_HEADER_BLOCK
{
	BYTE Sub_Channel_Format;
	BYTE Control			:4;
	BYTE Adr				:4;
	BYTE Track_Number;
	BYTE Reserved1;
}TRACK_HEADER_BLOCK,*LPTRACK_HEADER_BLOCK;
//*******************************************

typedef struct _MEDIA_CATALOG
{
	BYTE Reserved1				:7;
	BYTE Mc_Valid				:1;

	char Cat_Asc[13];
	BYTE Zero;
	BYTE Aframe;
}MEDIA_CATALOG,*LPMEDIA_CATALOG;
//******************************************
typedef struct _ISRC_CODE
{
	struct _SUB_CHANNEL_HEADER Header;
	struct _TRACK_HEADER_BLOCK Header_Block;
	BYTE Reserved1					:7;
	BYTE Tc_Valid					:1;

	char Country_Code[2];
    char Owner_Code[3];
	char Year_Code[2];
    char Serial_Number[5];
	BYTE Zero;
	BYTE Aframe;
	BYTE Reserved2;
}ISRC_CODE,*LPISRC_CODE;

//******************************************
struct _SUB_CODE_POSITION//Current Position
{
	struct _SUB_CHANNEL_HEADER Header;
	BYTE Sub_Code;

	BYTE Control				:4;
	BYTE Adr					:4;

	BYTE Track						;
	BYTE Index						;

	struct _MSF Absolute_Adr		;
	struct _MSF Relative_Adr        ;
}SUB_CODE_POSITION,*LPSUB_CODE_POSITION;
//******************************************
typedef struct _SUB_CODE_CATALOG//Media Catalog
{
	struct _SUB_CHANNEL_HEADER Header;
	BYTE Sub_Code;
	BYTE Reserved1					;
	BYTE Reserved2					;
	BYTE Reserved3					;
	struct _MEDIA_CATALOG Catalog	;
}SUB_CODE_CATALOG,*LPSUB_CODE_CATALOG;
//******************************************
 typedef struct _SUB_CODE_ISRC//ISRC
{
	struct _ISRC_CODE Isrc_Code[MAX_TRACKS] ;
}SUB_CODE_ISRC,*LPSUB_CODE_ISRC;

 //**************For _SOURCE_DATA **********
//******************************************
struct _TRACK_DETAILS
{
	BYTE Tracks[2048];
}TRACK_DETAILS,*LPTRACK_DETAILS;
//*******************************************
struct _ISRC_DETAILS
{
	BYTE Isrc[2048];
}ISRC_DETAILS,*LPISRC_DETAILS;
//*******************************************
struct _CATALOG_DETAILS
{
	BYTE Catalog[24];
}CATALOG_DETAILS,*LPCATALOG_DETAILS;

//*******************************************
typedef struct _SOURCE_DATA//data from source when reading
{
	_TRACK_DETAILS Track_Data;		//2048 bytes
	_ISRC_DETAILS Isrc_Data;		//2048 bytes
	_CATALOG_DETAILS Catalog_Data;	//24 bytes
	DWORD AmountTracksLoaded;
	MSF LastTrackEnd;
	MSF LastTrackStart;
	MSF LastTrackSize;
	MSF LastLeadOut;
}SOURCE_DATA,*LPSOURCE_DATA;
//********************************************
typedef struct _TRACK_INFO
{
	MSF		TrackStartMsf[MAX_TRACKS];
	MSF		TrackLenghtMsf[MAX_TRACKS];
	MSF     TrackEndMsf[MAX_TRACKS];
	DWORD	TrackStartLba[MAX_TRACKS];
	DWORD	TrackLenghtLba[MAX_TRACKS];
	DWORD   TrackEndLba[MAX_TRACKS];
	BOOL	TrackLoadedFlag;
}TRACK_INFO,*LPTRACK_INFO;
//******************************************
typedef struct _TRACK_ISRC
{
struct _SUB_CODE_ISRC Isrc_Data;
}TRACK_ISRC,*LPTRACK_ISRC;
//******************************************
typedef struct _FORMATCAPACITY_HEADER
{
	BYTE Reserved1;
	BYTE Reserved2;
	BYTE Reserved3;
	BYTE ListLenght;
}FORMATCAPACITIY_HEADER;
//*******************************************
typedef struct _FORMATCAPACITY_DESCRIPTOR
{
	DWORD NumberOfBlocks;
	BYTE DescriptorType		:2	;
	BYTE Reserved1			:6	;
	BYTE BlockLenghtMsb;
	WORD BlockLenghtLow;
}FORMATCAPACITY_DESCRIPTOR;
//*******************************************
typedef struct _FORMAT_DESCRIPTOR
{
	DWORD NumberOfBlocks;
	BYTE Reserved1			:2	;
	BYTE DescriptorType		:6	;
	BYTE TypeLenghtMsb;
	WORD BlockLenghtLow;
}FORMAT_DESCRIPTOR;

//******************************************
typedef struct _FORMATCAPACITY
{
	struct _FORMATCAPACITY_HEADER Header;
	struct _FORMATCAPACITY_DESCRIPTOR CurrentCapacity;
	struct _FORMAT_DESCRIPTOR Capacity[3];

}FORMATCAPACITY,*LPFORMATCAPACITY;
//*******************************************
typedef struct _TOCFORMAT_1
{
	BYTE MsbDataLength;
	BYTE LsbDataLength;
	BYTE FirstCompleteSession;
	BYTE LastCompleteSession;
	BYTE Reserved_1;
	BYTE Control		:4;
	BYTE ADR			:4;
	BYTE FirstTrackLastSession;
	BYTE Reserved_2;
	BYTE MsfUnused;
	BYTE MsfMinute;
	BYTE MsfSecond;
	BYTE MsfFrame;
}TOCFORMAT_1,*LPTOCFORMAT_1;
//*******************************************
typedef struct _PERFORMANCE_HEADER
{
	DWORD PerformanceLenght;
	BYTE  Execpt 				:1	;
	BYTE  Write     			:1	;
	BYTE  Reserved_1			:6	;
	BYTE  Reserved_2			;
	BYTE  Reserved_3			;
	BYTE  Reserved_4			;
}PERFORMANCE_HEADER;

//******************************************
typedef struct _PERFORMANCE_INFO
{
	struct _PERFORMANCE_HEADER Header;
	DWORD StartLBA;
	DWORD PerformanceStartData;
	DWORD EndLBA	;
	DWORD PerformanceEndData	;
	
}PERFORMANCE_INFO,*LPPERFORMANCE_INFO;

//******************************************
typedef struct _PERFORMANCE_INFO_3
{
	//struct _PERFORMANCE_HEADER Header	;
	BYTE  MRW						:1 ;
	BYTE  Exact						:1	;
	BYTE  Rdd						:1 ;
	BYTE  Wrc						:2 ;
	BYTE Hie						:1;
	BYTE  Reserved_1				:2	;

	BYTE  Reserved_2;
	BYTE  Reserved_3;
	BYTE  Reserved_4;
	DWORD StartLba;
	DWORD EndLba	;
	DWORD ReadSize	;
	DWORD ReadTime;
	DWORD WriteSize;
	DWORD WriteTime;
}PERFORMANCE_INFO_3,*LPPERFORMANCE_INFO_3;

//******************************************
#pragma pack(1)
typedef struct _ROOT_DIRECTORY
{
	BYTE SizeRecord;							//0 size of this Record Must be Even
	BYTE NumBer_Sectors_Extended;				//1 amount of sectors in Ex attribute
	DWORD StartSectorLsb;						//2 start sector Little endian zero = empty
	DWORD StartSectorMsb;						//6 start sector Big endian zero = empty
	DWORD SizeFileLsb;							//10 Size of file/directory Little endian
	DWORD SizeFilemsb;							//14 Size of file/directory Big endian
	BYTE Years;									//18 Years from 1900
	BYTE Month;									//19 Month Jan = 1
	BYTE DayMonth;								//20 Day Of the Month 1-31
	BYTE Hour;									//21 Hour 0-23
	BYTE Minute;								//22 Minute 0-59
	BYTE Second;								//23 Second 0-59
	BYTE GreenWichOffset;                      //24 15 Minute Intervals Positive east/Negative west

	BYTE FileType								:1;//25 0=normal file: 1 = Hidden file
	BYTE File_Directory							:1;//1=Directory
	BYTE AssociatedFile							:1;//1=Associcated File 
	BYTE RecordFormatSpecified					:1;//1=Record Format specifield
	BYTE Permissons								:1;//1 = Permissions specified
	BYTE Unsed1									:1;//Unsed
	BYTE Unsed2									:1;//Unsed 
	BYTE FinalRecord							:1;//1=Not The Final Record for This File

	BYTE FileUnitSizeInterleaved;				//26 File Unit Size For Interleaved
	BYTE FileGapSizeInterleaved;				//27 interleaved gap size for file
	WORD VolumeSeqNumberLsb;					//28 Volume Sequence Number Little Endian
	WORD VolumeSeqNumberMsb;					//30 Volume Sequence Number Big Endian
	BYTE IdentifierLength;                      //32 Identifier Lenght
	BYTE Identifier;                            //33 file identifier
}ROOT_DIRECTORY,*LPROOT_DIRECTORY;

//*******************************************
typedef struct _DATE_TIME
{
	char  Year[4];
	char  Month[2];
	char  DayMonth[2];
	char  Hour[2];
	char  Minute[2];
	char  Second[2];
	char  MsSeconds[2];
	BYTE  OffsetGreenwich;
}DATE_TIME,*LPDATA_TIME;
//*********************************

typedef struct _DIRECTORY_RECORD
{
	BYTE SizeRecord;						//0 size of this Record Must be Even
	BYTE NumBer_Sectors_Extended;			//1 amount of sectors in Ex attribute
	DWORD StartSectorLsb;					//2 start sector Little endian zero = empty
	DWORD StartSectorMsb;					//6 start sector Big endian zero = empty
	DWORD SizeFileLsb;						//10 Size of file/directory Little endian
	DWORD SizeFilemsb;						//14 Size of file/directory Big endian
	BYTE Years;								//18 Years from 1900
	BYTE Month;								//19 Month Jan = 1
	BYTE DayMonth;							//20 Day Of the Month 1-31
	BYTE Hour;								//21 Hour 0-23
	BYTE Minute;							//22 Minute 0-59
	BYTE Second;							//23 Second 0-59
	BYTE GreenWichOffset;                   //24 15 Minute Intervals Positive east/Negative west

	BYTE FileType						:1;//25 0=normal file: 1 = Hidden file
	BYTE File_Directory					:1;//1=Directory
	BYTE AssociatedFile					:1;//1=Associcated File 
	BYTE RecordFormatSpecified			:1;//1=Record Format specifield
	BYTE Permissons						:1;//1 = Permissions specified
	BYTE Unsed1							:1;//Unsed
	BYTE Unsed2							:1;//Unsed 
	BYTE FinalRecord					:1;//1=Not The Final Record for This File

	BYTE FileUnitSizeInterleaved;            //26 File Unit Size For Interleaved
	BYTE FileGapSizeInterleaved;            //27 interleaved gap size for file
	WORD VolumeSeqNumberLsb;				//28 Volume Sequence Number Little Endian
	WORD VolumeSeqNumberMsb;				//30 Volume Sequence Number Big Endian
	BYTE IdentifierLength;					//32 Identifier Lenght
	BYTE Identifier;
}DIRECTORY_RECORD,*LPDIRECTORY_RECORD;
//******************************************
typedef struct _DIRECTORY_LIST
{
	struct _DIRECTORY_RECORD Directory[8];
} DIRECTORY_LIST, *LPDIRECTORY_LIST;
//*******************************************
typedef struct _VOLUME_DESCRIPTOR
{
	BYTE	Volume_Type;				// = 1
	BYTE	Identifier[5];				// = "CD001"
	BYTE	Volume_Version;				// = 1
	BYTE	Unsed1;						// = 0
	BYTE	System_Identifier[32];		// = ?
	BYTE	Volume_Identifier[32];		// = ?
	BYTE	Unsed2[8];					// = 0

	DWORD   Volume_Space_LowHigh;		//Low/High Format
	DWORD	Volume_Space_HighLow  ;		//High/Low Format

	BYTE		Unsed3[32];				// = 0
	
	WORD    VolumeSet_SizeLsb;			// LSB Format = 1
	WORD    VolumeSet_SizeMsb;			// MSB Format = 1

	WORD    VolumeSeq_NumberLsb;		// LSB Format = 1
	WORD    VolumeSeq_NumberMsb;		// MSB Format = 1
	
	WORD    LogicalAUDIO_BLOCK_SIZELsb;	// LSB Format = 2048
	WORD    LogicalAUDIO_BLOCK_SIZEMsb;	// MSB Format = 2048


	DWORD   PathTable_SizeLsb;	    	// LSB Format
	DWORD   PathTable_SizeMsb;	    	// MSB Format

	DWORD   Location_PathLTable;		// LSB Fomat
	DWORD   Location_OptPathLTable;		// LSB Fomat
	DWORD   Location_PathMTable;		// MSB Format
	DWORD   Location_OptPathMTable;		// MSB Format


	//****************** Root ***************************************************
							// See A.17
	struct _ROOT_DIRECTORY RootDir;             //34 bytes       

//****************************************************************************************
	BYTE    Volume_Set_Indentifier[128];		// d Characters
	BYTE    Publisher_Indentifier[128];			// a Characters
	BYTE    Data_Prepare_Indentifier[128];		// a Characters
	BYTE    Application_Indentifier[128];		// a Characters

	BYTE    Copy_File_Identifier[38];			// d Characters SEPARATOR 1,SEPARATOR 2
	BYTE    Abstract_File_Identifier[36];		// d Characters SEPARATOR 1,SEPARATOR 2
	BYTE    Bibliographic_File_Indentifier[37];	// a Characters SEPARATOR 1,SEPARATOR 2
	struct _DATE_TIME Volume_Creation_DateTime;	// Digit(s)
	struct _DATE_TIME Volume_Mod_DataTime;		// Digit(s)
	struct _DATE_TIME Volume_Expiration_DateTime;// Digit(s)
	struct _DATE_TIME Volume_Effective_DataTime;// Digit(s)
	BYTE    File_Structure_Version;				// = 1
	BYTE    Reserved1;							// = 0
	BYTE    Application_Use[512];				// not specified
	BYTE    Reserved2[653];						// = 0
}VOLUME_DESCRIPTOR,*LPVOLUME_DESCRIPTOR;

//*******************************************
typedef struct _ISO_DATA
{
	char	System_Identifier[32];				// = ?
	char	Volume_Identifier[32];				// = ?
	DWORD   Volume_Space_SizeLsb;				// LSB Format	
	WORD    LogicalAUDIO_BLOCK_SIZELsb;			// LSB Format = 2048
	DWORD   PathTable_SizeLsb;	    			// LSB Format
	DWORD   Location_PathLTable;				// LSB Fomat
	struct _ROOT_DIRECTORY Root_Directory;
	char    Volume_Set_Indentifier[128];		// d Characters
	char    Publisher_Indentifier[128];			// a Characters
    char    Data_Prepare_Indentifier[128];		// a Characters
	char    Application_Indentifier[128];		// a Characters

	char    Copy_File_Identifier[37];			// d Characters SEPARATOR 1,SEPARATOR 2
	char    Abstract_File_Identifier[37];		// d Characters SEPARATOR 1,SEPARATOR 2
	char    Bibliographic_File_Indentifier[37]; // a Characters SEPARATOR 1,SEPARATOR 2

	struct _DATE_TIME Volume_Creation_DateTime;	// Digit(s)
	struct _DATE_TIME Volume_Mod_DataTime;		// Digit(s)
	struct _DATE_TIME Volume_Expiration_DateTime;// Digit(s)
	struct _DATE_TIME Volume_Effective_DataTime;// Digit(s)
}ISO_DATA,*LPISO_DATA;
//*********************************************
typedef struct _JOILET_DATA
{
	char	System_Identifier[32];				// = ?
	char	Volume_Identifier[32];				// = ?
	DWORD   Volume_Space_SizeLsb;				// LSB Format	
	WORD    LogicalAUDIO_BLOCK_SIZELsb;			// LSB Format = 2048
	DWORD   PathTable_SizeLsb;	    			// LSB Format
	DWORD   Location_PathLTable;				// LSB Fomat
	struct _ROOT_DIRECTORY Root_Directory;
	char    Volume_Set_Indentifier[128];		// d Characters
	char    Publisher_Indentifier[128];			// a Characters
    char    Data_Prepare_Indentifier[128];		// a Characters
	char    Application_Indentifier[128];		// a Characters

	char    Copy_File_Identifier[37];			// d Characters SEPARATOR 1,SEPARATOR 2
	char    Abstract_File_Identifier[37];		// d Characters SEPARATOR 1,SEPARATOR 2
	char    Bibliographic_File_Indentifier[37]; // a Characters SEPARATOR 1,SEPARATOR 2

	struct _DATE_TIME Volume_Creation_DateTime;			// Digit(s)
	struct _DATE_TIME Volume_Mod_DataTime;				// Digit(s)
	struct _DATE_TIME Volume_Expiration_DateTime;		// Digit(s)
	struct _DATE_TIME Volume_Effective_DataTime;		// Digit(s)

}JOILET_DATA,*LPJOILET_DATA;
//********************************************
typedef struct _PATHLTABLINFO
{
	BYTE LengthOfEntry;
	BYTE ExAttribRecLen;
	DWORD LocationLsb;
	WORD DirNumOfParent;
	char   DirName[64];
}PATHLABLEINFO,*LPPATHLTABLEINFO;
//********************************************
typedef struct _PATHMTABLINFO
{
	BYTE LengthOfEntry;
	BYTE ExAttribRecLen;
	DWORD LocationMsb;
	WORD DirNumOfParent;
	char   DirName[64];
}PATHMTABLEINFO, *LPPATHMTABLEINFO;
#pragma pack()
//***************************************
typedef struct _TRACKNAMES
{
	char TrackNames[64];
	BYTE TrackNumber;
}TRACKNAMES,*LPTRACKNAMES;
//*********************************************
struct _AUDIODISC_INFO
{
struct _SUB_CODE_CATALOG AudioCatalog;
struct _ISRC_CODE AudioIsrc[MAX_TRACKS];
struct _TRACK_INFO AudioTrackInfo;
struct _CDTEXT_INFO CdTextRawInfo;
BOOLEAN CdTextFlag;
char  AlbumTile[64];
struct _TRACKNAMES TrackTile[MAX_TRACKS];
struct _TRACKNAMES Performers[MAX_TRACKS];
struct _TRACKNAMES SongWriters[MAX_TRACKS];
struct _TRACKNAMES TrackComposers[MAX_TRACKS];
}AUDIODISC_INFO,*LPAUDISC_INFO;

//*********************************************
 typedef struct _UNKNOWN_MEDIA
 {
	 WCHAR UnKnown[128];
 }UNKNOWN_MEDIA,*LPUNKNOWN_MEDIA;
 //*********************************************
 typedef struct _NOMEDIA
 {
	 WCHAR NoMedia[128];
 }NOMEDIA,*LPNOMEDIA;
//*********************************************
 typedef struct _DOOROPEN
 {
	 WCHAR DoorOpen[128];
 }DOOROPEN,*LPDOOROPEN;
 //*********************************************
 typedef struct _DVDCLASS
 {
	 WCHAR DvdMedia[128];
	 WCHAR DvdMediaType[128];//possible types rom/r/-rw/+rw
	 WCHAR DvdMediaData[128];//possible Data Formatted/unFormatted/Blank/
	 BYTE PartVersion			:4;
	 BYTE DvdBookType			:4;

	 BYTE MaxRate				:4;
	 BYTE DiscSize				:4;

	 BYTE LayerType				:4;
	 BYTE TrackPath				:1;
	 BYTE AmountOfLayers		:2;
	 BYTE Reserved1				:1;

	 BYTE TrackDensity			:4;
	 BYTE LinearDensity			:4;

	 WORD AmountErase;					//amount of times this dvd has been erased

	 ULARGE_INTEGER DvdMaxBytes;		//max Bytes after blanking/reformatting
	 ULARGE_INTEGER DvdCurrentBytes;	//Current Bytes after blanking/reformatting
	 DWORD DvdMaxSectors;				//max sector when blanking/reformatting
	 DWORD DvdCurrentSectors;			//Current sector when blanking/reformatting
	 
	 DWORD DvdReadSpeedStart;			//Media Read speed
	 DWORD DvdReadSpeedStartLba;
	 DWORD DvdWriteSpeedStart;			//Media Write speed
	 DWORD DvdWriteSpeedStartLba;

	 DWORD DvdReadSpeedEnd;				//Media Read speed
	 DWORD DvdReadSpeedEndLba;
	 DWORD DvdWriteSpeedEnd;			//Media Write speed
	 DWORD DvdWriteSpeedEndLba;

	 WORD  DvdSectorSize;				//2048
	 WCHAR DriveSerialNumber[32];
	 char DriveModelNumber[32];
	 char DriveManufacturerID[64];
	 char MediaManufacturer[32];
	 BYTE MediaStatus;
	 BYTE LastRecordedAddress[4];	//only 3 bytes used 0-2
	 DVD_STRUCTURE_0 DvdStructure_Zero;
 }DVDCLASS,*LPDVDCLASS;
//**********************************************
 typedef struct _CURRENT_MEDIA
 {
	 BYTE CurrentDataType;
	 BYTE CurrentMediaType;
	 DWORD CurrentSectorSize;
	 ULARGE_INTEGER CurrentAmounfOfSectors;
	 ULARGE_INTEGER CurrentAmountOfBytes;
 }CURRENT_MEDIA,*LPCURRENT_MEDIA;
 //*********************************************
 typedef struct _CDCLASS
 {
	 char CdMedia[64];
	 char CdMediaType[64];			//possible types rom/r/rw
	 char CdMediaData[64];			//possible Data Data(mode1)/Audio(mode0)/Blank
	 BOOL AudioFlag;
	 BOOL DataFlag;
	 DWORD CdMaxBytes;				//max Bytes after blanking

	 DWORD CdReadSpeedStart;		//Media Read speed
	 DWORD CdReadSpeedStartLba;
	 DWORD CdWriteSpeedStart;		//Media Write speed
	 DWORD CdWriteSpeedStartLba;

	 DWORD CdReadSpeedEnd;			//Media Read speed
	 DWORD CdReadSpeedEndLba;
	 DWORD CdWriteSpeedEnd;			//Media Write speed
	 DWORD CdWriteSpeedEndLba;

	 DWORD CdMaxSectors;			//max sector when blanking
	 WORD  CdSectorSize;			//2048//2352//possible more types
	 struct _TOCMSF_INFORMATION TrackInfo;
	 struct _AUDIODISC_INFO AudioDetails;
 }CDCLASS,*LPCDCLASS;
//********************************************************
typedef struct _MEDIA_DESCRIBTOR
	{
	bool MediaEraseableFlag;
	bool DoorOpenFlag;
	bool NoMediaFlag;
	bool DvdFlag;
	bool CdFlag;
	bool UnknownFlag;
	struct _CURRENT_MEDIA Current;
	struct _DISKINFO DiskInfo;
	struct _UNKNOWN_MEDIA UnKnownMedia;
	struct _DOOROPEN DoorOpen;
	struct _NOMEDIA  NoMediaClass;
	struct _DVDCLASS DvdMediaClass;
	struct _CDCLASS  CdMediaClass;
}MEDIA_DESCRIBTOR,*LPMEDIA_DESCRIBTOR;
//*********************************************************
typedef struct _CUE_SHEET_INFO
{
	BOOL CueSheetFlag;
	WORD CueSheetSize;
	struct _CUE_SHEET CueSheet;
}CUE_SHEET_INFO,*LPCUE_SHEET_INFO;

//*******************************************************
typedef struct _TRACKDATA
{
	DWORD TotalSectorPos;
	DWORD TrackNumber;
	DWORD StepSizeTrack;
	DWORD TrackPercent;
	MSF	  TrackPosMsf;
}TRACKDATA,*LPTRACKDATA;
//********************************
typedef struct _DISPLAY
{
	ULARGE_INTEGER Number;
	BYTE AmountSet;
	WCHAR Bufferset[128];
	WCHAR formatbuffer[64];
	BYTE numberformat;
	BYTE Digits[16];
}DISPLAY, *LPDISPLAY;

