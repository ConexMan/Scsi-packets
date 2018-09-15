ScsiClass::ScsiClass()
{
	ScsiClass::SetCdDrives();
	ScsiClass::OpenCdUnit();
	ScsiClass::OpenUnit				= FALSE;
	ScsiClass::SetUnit				= FALSE;
	ScsiClass::UserRequestPacket			= FALSE;
	ScsiClass::CatalogEntryAuto			= true;
	GetSystemInfo(&this->ScsiClass::SystemInfo);
	ScsiClass::GetFeatures(ScsiClass::packet);
	QueryPerformanceFrequency((LARGE_INTEGER *)&Frequency.QuadPart);
}
//******************************************
ScsiClass::~ScsiClass()
{
	if (fDataFileFlag)
		CloseHandle(hDataFile);
}
//**********************************************************************************************************************
UCHAR ScsiClass::SetCdDrives()
	{//open code
    char drive[8];
	char nocd[8];
	UINT drive_type;
	int count;
	char string[32 ]    	= {0};				
	char letter[4]		= "A:";	
	UINT Type;

	strcpy_s(drive,"A:\\");
	strcpy_s(nocd,"NO CD");
	for(count = 0 ; count < 26; count++)
	{			
	drive_type = GetDriveTypeA(drive);	
	ScsiClass::DriveList.DriveTypes[count] = drive_type;//to be removed later
	drive[0] = drive[0] + 1;
	drive_type = DRIVE_NO_ROOT_DIR;
	}		
	ScsiClass::DriveCount = 0;
	ScsiClass::CdInfo.AmountCdDrives = 0;
		for(count = 0; count < 26 ;count++)
	{//open for 1
			Type = ScsiClass::DriveList.DriveTypes[count];//to be removed later
		if(Type == DRIVE_CDROM)
		{//open if DRIVE_CDROM
	 
	ScsiClass::CdInfo.Details[ScsiClass::DriveCount].DriveLetter[0] = letter[0];
	ScsiClass::CdInfo.Details[ScsiClass::DriveCount].DriveValidFlag  = true;
	ScsiClass::DriveCount++;
	ScsiClass::CdInfo.AmountCdDrives++;
	if(ScsiClass::DriveCount == MAX_CDROM_DRIVES)
	return(MAX_DRIVES_FOUND);
		}
	letter[0]++;
	}
	if(ScsiClass::DriveCount == 0)
	return(DEVICEIOCONTROL_FAILED);
	ScsiClass::SetUnit = TRUE;
	return(DEVICEIOCONTROL_OK);
	}
//**************************************
	BOOLEAN ScsiClass::OpenDoor(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
packet.Amount_Sectors			= 0;
packet.Size_Sectors			= 0X800;
packet.Amount_Transfer			= 0;
packet.hcd_device			= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;
ScsiClass::spwb.Block.Cdb[0]	= 0X1B;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 2;//2=open:3=close
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;
return(ScsiClass::SendScsi(packet));
	}
//**************************************
	BOOLEAN ScsiClass::CloseDoor(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors			= 0X800;
packet.Amount_Transfer			= 0;
packet.hcd_device			= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;
ScsiClass::spwb.Block.Cdb[0]	= 0X1B;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 3;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;
return(ScsiClass::SendScsi(packet));
	}	
//*************************************
	UCHAR ScsiClass::OpenCdUnit()
	{//code open	
		UCHAR retcode;
		int count;
		int Index;
		char Vendor_id[16]  = {0};
		char Product_id[24] = {0};
		char Revision_id[8] = {0};
		DWORD count1;
		char letter[4] = "A:";
		char device_name[12] = "\\\\.\\";
		char drive[4] = {0};

		for (count1=0;count1 < ScsiClass::DriveCount;count1++)
		{//open1
		drive[0] = ScsiClass::CdInfo.Details[count1].DriveLetter[0];
		drive[1] = ':';
		strcat_s(device_name,sizeof(device_name),drive);
		ScsiClass::CdInfo.Details[count1].ScsiHandle = CreateFileA(device_name,GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_FLAG_WRITE_THROUGH,0);
		if(ScsiClass::CdInfo.Details[count1].ScsiHandle == INVALID_HANDLE_VALUE)
		{
			ScsiClass::CdInfo.Details[count1].ErrorCode			 = GetLastError();
			ScsiClass::CdInfo.Details[count1].DriveValidFlag	 = false;
		return(DEVICEIOCONTROL_FAILED) ;
		}
		ScsiClass::CdInfo.Details[count1].DriveValidFlag	     = true;
		strcpy_s(device_name,"\\\\.\\");
		}//close 1
	ScsiClass::CdInfo.CurrentScsiDrive		= ScsiClass::CdInfo.Details[0].ScsiHandle;
	ScsiClass::CdInfo.CurrentLetter[0]		= ScsiClass::CdInfo.Details[0].DriveLetter[0];
	
	
	
	for(count = 0 ; count < MAX_CDDRIVES ; count++)
		{//open1
	Index = 0;
	ScsiClass::CurrentScsiHandle			= ScsiClass::CdInfo.Details[count].ScsiHandle;
	retcode = ScsiClass::GetInquiry(ScsiClass::packet);
	if(retcode == DEVICEIOCONTROL_OK)
	{//open2
	for(count1 = 0; count1 < 8 ;count1++)
	{
		
		ScsiClass::CdInfo.Details[count].DriveName.Vendor_id[count1]		= ScsiClass::spwb.Data_Returned[count1 + 8];
		ScsiClass::CdInfo.Details[count].DriveName.CompleteID[Index]		= ScsiClass::spwb.Data_Returned[count1 + 8];
		Index++;
	}

	MultiByteToWideChar(CP_ACP, 0, ScsiClass::CdInfo.Details[count].DriveName.Vendor_id, sizeof(ScsiClass::CdInfo.Details[count].DriveName.Vendor_id),
		ScsiClass::CdInfo.Details[count].DriveName.wVendor_id, _countof(ScsiClass::CdInfo.Details[count].DriveName.wVendor_id));

	MultiByteToWideChar(CP_ACP, 0, ScsiClass::CdInfo.Details[count].DriveName.Vendor_id, sizeof(ScsiClass::CdInfo.Details[count].DriveName.Vendor_id),
		ScsiClass::CdInfo.Details[count].DriveName.wCompleteID, _countof(ScsiClass::CdInfo.Details[count].DriveName.wCompleteID));

	ScsiClass::CdInfo.Details[count].DriveName.CompleteID[Index] = 32;
	Index++;
	for(count1 = 0; count1 < 16 ;count1++)
	{
	
		ScsiClass::CdInfo.Details[count].DriveName.Product_id[count1]					= ScsiClass::spwb.Data_Returned[count1 + 16];
		ScsiClass::CdInfo.Details[count].DriveName.ProductPlusLetter[count1]	= ScsiClass::spwb.Data_Returned[count1 + 16];
		ScsiClass::CdInfo.Details[count].DriveName.CompleteID[Index]					= ScsiClass::spwb.Data_Returned[count1 + 16];
		Index++;
	}
	ScsiClass::CdInfo.Details[count].DriveName.CompleteID[Index] = 32;
	Index++;

	MultiByteToWideChar(CP_ACP, 0, ScsiClass::CdInfo.Details[count].DriveName.Product_id, sizeof(ScsiClass::CdInfo.Details[count].DriveName.Product_id),
		ScsiClass::CdInfo.Details[count].DriveName.wProduct_id, _countof(ScsiClass::CdInfo.Details[count].DriveName.wProduct_id));

	MultiByteToWideChar(CP_ACP, 0, ScsiClass::CdInfo.Details[count].DriveName.ProductPlusLetter, sizeof(ScsiClass::CdInfo.Details[count].DriveName.ProductPlusLetter),
		ScsiClass::CdInfo.Details[count1].DriveName.wProductPlusLetter, _countof(ScsiClass::CdInfo.Details[count1].DriveName.wProductPlusLetter));

	MultiByteToWideChar(CP_ACP, 0, ScsiClass::CdInfo.Details[count].DriveName.CompleteID, sizeof(ScsiClass::CdInfo.Details[count].DriveName.CompleteID),
		ScsiClass::CdInfo.Details[count].DriveName.wCompleteID, _countof(ScsiClass::CdInfo.Details[count].DriveName.wCompleteID));

	for(count1 = 0; count1 < 4 ;count1++)
	{

		ScsiClass::CdInfo.Details[count].DriveName.Revision_id[count1] = ScsiClass::spwb.Data_Returned[count1 + 32];
		ScsiClass::CdInfo.Details[count].DriveName.CompleteID[Index]	  = ScsiClass::spwb.Data_Returned[count1 + 32];
		Index++;
	}
	MultiByteToWideChar(CP_ACP, 0, ScsiClass::CdInfo.Details[count].DriveName.Revision_id, sizeof(ScsiClass::CdInfo.Details[count].DriveName.Revision_id),
		ScsiClass::CdInfo.Details[count].DriveName.wRevision_id, _countof(ScsiClass::CdInfo.Details[count].DriveName.wRevision_id));

	MultiByteToWideChar(CP_ACP, 0, ScsiClass::CdInfo.Details[count].DriveName.CompleteID, sizeof(ScsiClass::CdInfo.Details[count].DriveName.CompleteID),
		ScsiClass::CdInfo.Details[count].DriveName.wCompleteID, _countof(ScsiClass::CdInfo.Details[count].DriveName.wCompleteID));

	}//close2
	
	}//close1

	for (count = 0; count < ScsiClass::CdInfo.AmountCdDrives; count++)
	{
		strcat_s(ScsiClass::CdInfo.Details[count].DriveName.ProductPlusLetter, "( ");
		strcat_s(ScsiClass::CdInfo.Details[count].DriveName.ProductPlusLetter, ScsiClass::CdInfo.Details[count].DriveLetter);
		strcat_s(ScsiClass::CdInfo.Details[count].DriveName.ProductPlusLetter, ":");
		strcat_s(ScsiClass::CdInfo.Details[count].DriveName.ProductPlusLetter, " ) ");

		MultiByteToWideChar(CP_ACP, 0, ScsiClass::CdInfo.Details[count].DriveName.ProductPlusLetter, sizeof(ScsiClass::CdInfo.Details[count].DriveName.ProductPlusLetter),
			ScsiClass::CdInfo.Details[count].DriveName.wProductPlusLetter, _countof(ScsiClass::CdInfo.Details[count].DriveName.wProductPlusLetter));
	}
	ScsiClass::OpenUnit = TRUE;
	ScsiClass::CurrentScsiHandle	= ScsiClass::CdInfo.Details[0].ScsiHandle;
	return(DEVICEIOCONTROL_OK);
		}//close code
	
//**************************************
	UCHAR ScsiClass::BlankDisc(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;
ScsiClass::spwb.Block.Cdb[0]	= 0XA1;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.BlankingType;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
//**************************************
	UCHAR ScsiClass::CloseTrack(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer	    	= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X5B;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= packet.CloseType;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= packet.CloseTrackMsb;
ScsiClass::spwb.Block.Cdb[5]	= packet.CloseTrackLsb;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
//**************************************
	UCHAR ScsiClass::FormatDisc(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0X40;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU
ScsiClass::w_spwb.Block.Cdb[0]	= 4;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]	= packet.FormatType;
ScsiClass::w_spwb.Block.Cdb[2]	= 0;
ScsiClass::w_spwb.Block.Cdb[3]	= 0;
ScsiClass::w_spwb.Block.Cdb[4]	= 0;
ScsiClass::w_spwb.Block.Cdb[5]	= 0;
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= 0;
ScsiClass::w_spwb.Block.Cdb[8]	= 0;
ScsiClass::w_spwb.Block.Cdb[9]	= 0;
ScsiClass::w_spwb.Block.Cdb[10]	= 0;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
//**************************************
    UCHAR ScsiClass::GetConfiguration(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0X800;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X46;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 2;//only one to be returned
ScsiClass::spwb.Block.Cdb[2]	= packet.FeatureMSB;
ScsiClass::spwb.Block.Cdb[3]	= packet.FeatureLSB;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 8;//msb of lenght
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
//**************************************
	UCHAR ScsiClass::GetEvent(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0X40;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X4A;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= packet.ClassType;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;//msb of lenght
ScsiClass::spwb.Block.Cdb[8]	= 0X40;//lsb of lenght
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
//**************************************
	UCHAR ScsiClass::GetPerformance(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0X20;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XAC;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.PerformDataType;// Data Type field;
ScsiClass::spwb.Block.Cdb[2]	= packet.PerformStartMSB;
ScsiClass::spwb.Block.Cdb[3]	= packet.PerformStartHIGH;
ScsiClass::spwb.Block.Cdb[4]	= packet.PerformStartLOW;
ScsiClass::spwb.Block.Cdb[5]	= packet.PerformStartLSB;
ScsiClass::spwb.Block.Cdb[6]	= 0;//res
ScsiClass::spwb.Block.Cdb[7]	= 0;//res
ScsiClass::spwb.Block.Cdb[8]	= packet.PerformMaxMsb;// max descriptors to get
ScsiClass::spwb.Block.Cdb[9]	= packet.PerformMaxLsb;// descriptors to get
ScsiClass::spwb.Block.Cdb[10]	= packet.PerformType; // this the type field 0 = performance etc;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
//**************************************
	UCHAR ScsiClass::GetInquiry(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0X80;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;
ScsiClass::spwb.Block.Cdb[0]	= 0X12;				
ScsiClass::spwb.Block.Cdb[1]	= 0;	
ScsiClass::spwb.Block.Cdb[2]	= 0;			
ScsiClass::spwb.Block.Cdb[3]	= 0;		
ScsiClass::spwb.Block.Cdb[4]	= 0X80;			
ScsiClass::spwb.Block.Cdb[5]	= 0;	
ScsiClass::spwb.Block.Cdb[6]	= 0;			
ScsiClass::spwb.Block.Cdb[7]	= 0;	
ScsiClass::spwb.Block.Cdb[8]	= 0;		
ScsiClass::spwb.Block.Cdb[9]	= 0;		
ScsiClass::spwb.Block.Cdb[10]	= 0;			
ScsiClass::spwb.Block.Cdb[11]	= 0;			
return(ScsiClass::SendScsi(packet));
	}
//**************************************
	UCHAR ScsiClass::LoadUnLoad(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;
ScsiClass::spwb.Block.Cdb[0]	= 0XA6;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= packet.LoadUload;//data allocation size
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
//**************************************
	UCHAR ScsiClass::MechStatus(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0x40;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XBD;				
ScsiClass::spwb.Block.Cdb[1]	= 0;	
ScsiClass::spwb.Block.Cdb[2]	= 0;			
ScsiClass::spwb.Block.Cdb[3]	= 0;		
ScsiClass::spwb.Block.Cdb[4]	= 0;			
ScsiClass::spwb.Block.Cdb[5]	= 0;	
ScsiClass::spwb.Block.Cdb[6]	= 0;			
ScsiClass::spwb.Block.Cdb[7]	= 0;	
ScsiClass::spwb.Block.Cdb[8]	= 0;		
ScsiClass::spwb.Block.Cdb[9]	= 0X40;		
ScsiClass::spwb.Block.Cdb[10]	= 0;			
ScsiClass::spwb.Block.Cdb[11]	= 0;		
return(ScsiClass::SendScsi(packet));
	}//just set up packet and call SendScsi
//**************************************
	UCHAR ScsiClass::ModeSelect(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0X80;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU
ScsiClass::w_spwb.Block.Cdb[0]	= 0X55;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]	= 0X10;
ScsiClass::w_spwb.Block.Cdb[2]	= 0;
ScsiClass::w_spwb.Block.Cdb[3]	= 0;
ScsiClass::w_spwb.Block.Cdb[4]	= 0;
ScsiClass::w_spwb.Block.Cdb[5]	= 0;
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= packet.ModeSelectMSB;//WAS 0
ScsiClass::w_spwb.Block.Cdb[8]	= packet.ModeSelectLSB;//WAS 0X80
ScsiClass::w_spwb.Block.Cdb[9]	= 0;
ScsiClass::w_spwb.Block.Cdb[10]	= 0;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}

////**************************************
	UCHAR ScsiClass::ModeSense(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 2;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= packet.Amount_Sectors * packet.Size_Sectors;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X05A;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 8;
ScsiClass::spwb.Block.Cdb[2]	= packet.ModeSenseCode;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;//data allocation size
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0x10;// packet.ModeSenseLenMsb;
ScsiClass::spwb.Block.Cdb[8]	= 0;// packet.ModeSenseLenLsb;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
////**************************************
	UCHAR ScsiClass::PauseResume(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X4B;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= packet.Resume;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
////**************************************
	UCHAR ScsiClass::PlayAudio(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X45;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0XFF;
ScsiClass::spwb.Block.Cdb[8]	= 0XFF;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::PlayAudioMsf(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X47;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= packet.StartM;
ScsiClass::spwb.Block.Cdb[4]	= packet.StartS;
ScsiClass::spwb.Block.Cdb[5]	= packet.StartF;
ScsiClass::spwb.Block.Cdb[6]	= packet.EndingM;
ScsiClass::spwb.Block.Cdb[7]	= packet.EndingS;
ScsiClass::spwb.Block.Cdb[8]	= packet.EndingF;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
////**************************************
	UCHAR ScsiClass::PlayLba(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XBC;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
*pointer                 		= ScsiClass::BswapDword(&packet.StartPlay);
pointer++;
*pointer						= ScsiClass::BswapDword(&packet.PlayLenght);
ScsiClass::spwb.Block.Cdb[10]	= 1;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::PlayMsf(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XBC;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= packet.StartM;
ScsiClass::spwb.Block.Cdb[4]	= packet.StartS;
ScsiClass::spwb.Block.Cdb[5]	= packet.StartF;
ScsiClass::spwb.Block.Cdb[6]	= packet.EndingM;
ScsiClass::spwb.Block.Cdb[7]	= packet.EndingS;
ScsiClass::spwb.Block.Cdb[8]	= packet.EndingF;
ScsiClass::spwb.Block.Cdb[10]	= 1;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::PreventAllow(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			= 0;
packet.Size_Sectors				= 0X800;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU= //SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X1E;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= packet.PreventAllow;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}

////**************************************
	UCHAR ScsiClass::Read10Full(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= AUDIO_BLOCK_SIZE;
packet.Amount_Transfer			= packet.Size_Sectors * packet.Amount_Sectors;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X28;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.ReadByte;
*pointer                 		= BswapDword(&packet.Read10Lba);
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= (BYTE) AUDIO_BLOCK_SIZE;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
////**************************************
	UCHAR ScsiClass::Read10Part(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= packet.Amount_Sectors;
packet.Amount_Transfer			= packet.Size_Sectors * packet.Amount_Sectors;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X28;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.ReadByte;
*pointer                 		= BswapDword(&packet.Read10Lba);
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= (BYTE)packet.Amount_Sectors;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}	
////**************************************
  UCHAR ScsiClass::Read12(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;
void *dwordptr2;
DWORD *pointer2;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

dwordptr2 = &ScsiClass::spwb.Block.Cdb[6];
pointer2  = (DWORD *) dwordptr2;

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= AUDIO_BLOCK_SIZE;
packet.Amount_Transfer			= packet.Size_Sectors  * packet.Amount_Sectors;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XA8;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.ReadByte;;
*pointer                 		= BswapDword(&packet.Read12Lba);
*pointer2						= BswapDword(&packet.Read12TransferLength);
ScsiClass::spwb.Block.Cdb[10]	= packet.ReadStreaming;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::ReadBuffer(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= sizeof(ScsiClass::spwb.Data_Returned);
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X3C;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.ReadBufferMode;
ScsiClass::spwb.Block.Cdb[2]	= packet.ReadBufferId;
ScsiClass::spwb.Block.Cdb[3]	= packet.BufferOffsetMsb;
ScsiClass::spwb.Block.Cdb[4]	= packet.BufferOffsetHigh;
ScsiClass::spwb.Block.Cdb[5]	= packet.BufferOffsetLsb;
ScsiClass::spwb.Block.Cdb[6]	= packet.BufferAllocateMsb;
ScsiClass::spwb.Block.Cdb[7]	= packet.BufferAllocateHigh;
ScsiClass::spwb.Block.Cdb[8]	= packet.BufferAllocateLsb;
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::ReadBufferCapacity(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0XC;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X5C;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.ReadBufferCapacityBlock;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0X40;
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::ReadCapacity(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0X8;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X25;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
   UCHAR ScsiClass::ReadCDFull(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;
dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;
//packet->Size_Sectors set by user	

packet.Amount_Sectors			= AUDIO_BLOCK_SIZE;
packet.Amount_Transfer			= packet.Size_Sectors * packet.Amount_Sectors;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XBE;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.Readcd_Expected;
*pointer						= BswapDword(&packet.ReadCDLba);
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;

ScsiClass::spwb.Block.Cdb[8]	= (BYTE)AUDIO_BLOCK_SIZE;
ScsiClass::spwb.Block.Cdb[9]	= packet.Readcd_Flags;
ScsiClass::spwb.Block.Cdb[10]	= packet.Readcd_Sub;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::ReadCDPart(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;
//packet->Size_Sectors set by user		
//packet->Amount_Sectors set by user
packet.Amount_Transfer			= packet.Size_Sectors * packet.Amount_Sectors;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XBE;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.Readcd_Expected;
*pointer                 						= BswapDword(&packet.ReadCDLba);
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= (BYTE)packet.Amount_Sectors;
ScsiClass::spwb.Block.Cdb[9]	= packet.Readcd_Flags;
ScsiClass::spwb.Block.Cdb[10]	= packet.Readcd_Sub;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::ReadCDMsf(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Amount_Sectors			=  AUDIO_BLOCK_SIZE;
packet.Amount_Transfer			= packet.Size_Sectors * packet.Amount_Sectors;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XB9;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.Readcd_Expected;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= packet.StartM;
ScsiClass::spwb.Block.Cdb[4]	= packet.StartS;
ScsiClass::spwb.Block.Cdb[5]	= packet.StartF;
ScsiClass::spwb.Block.Cdb[6]	= packet.EndingM;
ScsiClass::spwb.Block.Cdb[7]	= packet.EndingS;
ScsiClass::spwb.Block.Cdb[8]	= packet.EndingF;
ScsiClass::spwb.Block.Cdb[9]	= packet.Readcd_Flags;
ScsiClass::spwb.Block.Cdb[10]	= packet.Readcd_Sub;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::ReadDiscInfo(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0x40;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X51;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 8;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::ReadDvdStructure(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= AUDIO_BLOCK_SIZE_DVD;
packet.Amount_Transfer			= packet.Size_Sectors * packet.Amount_Sectors;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XAD;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
*pointer                 		= BswapDword(&packet.ReadDvdAddress);

ScsiClass::spwb.Block.Cdb[6]	= packet.ReadDvdLayer;
ScsiClass::spwb.Block.Cdb[7]	= packet.ReadDvdFormat;
ScsiClass::spwb.Block.Cdb[8]	= 0X80;//returns 16 sectors (1 ECC Block)
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::ReadFormatCapacity(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0X40;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X23;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0x40;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::ReadHeader(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 8;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X44;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.ReadHeaderMsf;
*pointer						= BswapDword(&packet.ReadHeaderAddress);
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0X40;
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}

////**************************************
	UCHAR ScsiClass::ReadSubChannel(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 2;
packet.Amount_Transfer			= 0X1000;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X42;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.ReadSubMsf;
ScsiClass::spwb.Block.Cdb[2]	= 0X40;
ScsiClass::spwb.Block.Cdb[3]	= packet.ReadSubFormat;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= packet.ReadSubTrack;
ScsiClass::spwb.Block.Cdb[7]	= 0x10;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::ReadTocPmaAtip(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0X800;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X43;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.ReadTocMsf;
ScsiClass::spwb.Block.Cdb[2]	= packet.ReadTocFormat;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= packet.ReadTocTrack;
ScsiClass::spwb.Block.Cdb[7]	= 8;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
////**************************************
	UCHAR ScsiClass::ReadTrackInfo(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0X80;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X52;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.ReadTrackInfoType;
*pointer                 		= BswapDword(&packet.ReadTrackInfoAddress);
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= packet.ReadTrackAllocateMsb;
ScsiClass::spwb.Block.Cdb[8]	= packet.ReadTrackAllocateLsb;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::RepairRzone(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X58;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= packet.RepairZoneMsb;
ScsiClass::spwb.Block.Cdb[5]	= packet.RepairZoneLsb;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::ReportKey(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0X80;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XA4;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
*pointer						= BswapDword(&packet.ReportKeyAddress);
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= packet.ReportKeyClass;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]    = 0X80;
ScsiClass::spwb.Block.Cdb[10]	= packet.ReportKeyFormatAgid;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::RequestSense(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0X40;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_IN;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 3;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0X40;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}

////**************************************
	UCHAR ScsiClass::ReserveTrackRzone(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[5];
pointer  = (DWORD *) dwordptr;

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X53;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.ReservationType;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
*pointer						= BswapDword(&packet.ReservationSize);
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::Scan(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XBA;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.ScanDirection;
*pointer						= BswapDword(&packet.ScanStart);
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]    = packet.ScanType;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::Seek(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X2B;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
*pointer						= BswapDword(&packet.SeekAddress);
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::SendCue(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= (WORD)ScsiClass::CueSheetInfo.CueSheetSize;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU
ScsiClass::w_spwb.Block.Cdb[0]	= 0X5D;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]	= 0;
ScsiClass::w_spwb.Block.Cdb[2]	= 0;
ScsiClass::w_spwb.Block.Cdb[3]	= 0;
ScsiClass::w_spwb.Block.Cdb[4]	= 0;
ScsiClass::w_spwb.Block.Cdb[5]	= 0;
ScsiClass::w_spwb.Block.Cdb[6]	= packet.SendCueMsb;
ScsiClass::w_spwb.Block.Cdb[7]	= packet.SendCueHigh;
ScsiClass::w_spwb.Block.Cdb[8]	= packet.SendCueLsb;
ScsiClass::w_spwb.Block.Cdb[9]	= 0;
ScsiClass::w_spwb.Block.Cdb[10]	= 0;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::SendDvdStructure(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0X800;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU
ScsiClass::w_spwb.Block.Cdb[0]	= 0XBF;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]	= 0;
ScsiClass::w_spwb.Block.Cdb[2]	= 0;
ScsiClass::w_spwb.Block.Cdb[3]	= 0;
ScsiClass::w_spwb.Block.Cdb[4]	= 0;
ScsiClass::w_spwb.Block.Cdb[5]	= 0;
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= packet.SendDvdStructureFormat;
ScsiClass::w_spwb.Block.Cdb[8]	= packet.SendDvdStructureMsb;
ScsiClass::w_spwb.Block.Cdb[9]  = packet.SendDvdStructureLsb;
ScsiClass::w_spwb.Block.Cdb[10]	= 0;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::SendEvent(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0X800;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU
ScsiClass::w_spwb.Block.Cdb[0]	= 0XA2;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]	= 0;
ScsiClass::w_spwb.Block.Cdb[2]	= 0;
ScsiClass::w_spwb.Block.Cdb[3]	= 0;
ScsiClass::w_spwb.Block.Cdb[4]	= 0;
ScsiClass::w_spwb.Block.Cdb[5]	= 0;
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= 0;
ScsiClass::w_spwb.Block.Cdb[8]	= packet.SendEventListMsb;
ScsiClass::w_spwb.Block.Cdb[9]	= packet.SendEventListLsb;
ScsiClass::w_spwb.Block.Cdb[10]	= 0;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::SendKey(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0X800;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU
ScsiClass::w_spwb.Block.Cdb[0]	= 0XA3;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]	= 0;
ScsiClass::w_spwb.Block.Cdb[2]	= 0;
ScsiClass::w_spwb.Block.Cdb[3]	= 0;
ScsiClass::w_spwb.Block.Cdb[4]	= 0;
ScsiClass::w_spwb.Block.Cdb[5]	= 0;
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= packet.SendKeyClass;
ScsiClass::w_spwb.Block.Cdb[8]	= packet.SendKeyMsb;
ScsiClass::w_spwb.Block.Cdb[9]	= packet.SendEKeyLsb;
ScsiClass::w_spwb.Block.Cdb[10]	= packet.SendKeyAgidFormat;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::SendOpcInfo(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0X800;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU
ScsiClass::w_spwb.Block.Cdb[0]	= 0X54;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]	= packet.SendOpcFlag;
ScsiClass::w_spwb.Block.Cdb[2]	= 0;
ScsiClass::w_spwb.Block.Cdb[3]	= 0;
ScsiClass::w_spwb.Block.Cdb[4]	= 0;
ScsiClass::w_spwb.Block.Cdb[5]	= 0;
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= packet.SendOpcListMsb;
ScsiClass::w_spwb.Block.Cdb[8]	= packet.SendOpcListLsb;
ScsiClass::w_spwb.Block.Cdb[9]	= 0;
ScsiClass::w_spwb.Block.Cdb[10]	= 0;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
//****************************************
	UCHAR ScsiClass::SendTestCommand(PACKET_DRIVER packet)
	{
ScsiClass::spwb.Block.Cdb[0]	= ScsiClass::packet.Cdb[0];
ScsiClass::spwb.Block.Cdb[1]	= ScsiClass::packet.Cdb[1];
ScsiClass::spwb.Block.Cdb[2]	= ScsiClass::packet.Cdb[2];
ScsiClass::spwb.Block.Cdb[3]	= ScsiClass::packet.Cdb[3];
ScsiClass::spwb.Block.Cdb[4]	= ScsiClass::packet.Cdb[4];
ScsiClass::spwb.Block.Cdb[5]	= ScsiClass::packet.Cdb[5];
ScsiClass::spwb.Block.Cdb[6]	= ScsiClass::packet.Cdb[6];
ScsiClass::spwb.Block.Cdb[7]	= ScsiClass::packet.Cdb[7];
ScsiClass::spwb.Block.Cdb[8]	= ScsiClass::packet.Cdb[8];
ScsiClass::spwb.Block.Cdb[9]    = ScsiClass::packet.Cdb[9];
ScsiClass::spwb.Block.Cdb[10]	= ScsiClass::packet.Cdb[10];
ScsiClass::spwb.Block.Cdb[11]	= ScsiClass::packet.Cdb[11];
return(ScsiClass::SendScsi(packet));
	}
//**************************************
	UCHAR ScsiClass::SetCdSpeed(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XBB;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.SetCdSpeedControl;
ScsiClass::spwb.Block.Cdb[2]	= packet.SetCdSpeedReadMsb;
ScsiClass::spwb.Block.Cdb[3]	= packet.SetCdSpeedReadLsb;
ScsiClass::spwb.Block.Cdb[4]	= packet.SetCdSpeedWriteMsb;
ScsiClass::spwb.Block.Cdb[5]	= packet.SetCdSpeedWriteLsb;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::SetReadAhead(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;
void *dwordptr2;
DWORD *pointer2;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

dwordptr2 = &ScsiClass::spwb.Block.Cdb[6];
pointer2  = (DWORD *) dwordptr2;

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0XA7;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
*pointer						= BswapDword(&packet.SetReadAheadTrigger);
*pointer2						= BswapDword(&packet.SetReadAheadBlock);
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::SetStreaming(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0X80;//was 0x40
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU
ScsiClass::w_spwb.Block.Cdb[0]	= 0XB6;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]	= 0;//needs work
ScsiClass::w_spwb.Block.Cdb[2]	= 0;
ScsiClass::w_spwb.Block.Cdb[3]	= 0;
ScsiClass::w_spwb.Block.Cdb[4]	= 0;
ScsiClass::w_spwb.Block.Cdb[5]	= 0;
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= 0;
ScsiClass::w_spwb.Block.Cdb[8]	= 0;
ScsiClass::w_spwb.Block.Cdb[9]	= packet.SetStreamingListMsb;
ScsiClass::w_spwb.Block.Cdb[10]	= packet.SetStreamingListLsb;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::StartStopUnit(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X1B;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= packet.StartStop;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::StopPlayScan(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X4E;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::SynCache(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X35;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= ScsiClass::packet.SynImmediate;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::TestUnitReady(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0;
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= 0;
ScsiClass::spwb.Block.Cdb[2]	= 0;
ScsiClass::spwb.Block.Cdb[3]	= 0;
ScsiClass::spwb.Block.Cdb[4]	= 0;
ScsiClass::spwb.Block.Cdb[5]	= 0;
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= 0;
ScsiClass::spwb.Block.Cdb[8]	= 0;
ScsiClass::spwb.Block.Cdb[9]	= 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
//**************************************
	UCHAR ScsiClass::Verify10(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;
packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= 0;
packet.hcd_device						= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock				= FALSE;
ScsiClass::spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU
ScsiClass::spwb.Block.Cdb[0]	= 0X2F;//command scsi
ScsiClass::spwb.Block.Cdb[1]	= packet.Verify10Flags;
*pointer										= BswapDword(&packet.Verify10Lba);
ScsiClass::spwb.Block.Cdb[6]	= 0;
ScsiClass::spwb.Block.Cdb[7]	= packet.Verify10LengthMsb;
ScsiClass::spwb.Block.Cdb[8]	= packet.Verify10LengthLsb;
ScsiClass::spwb.Block.Cdb[9]    = 0;
ScsiClass::spwb.Block.Cdb[10]	= 0;
ScsiClass::spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
//**************************************
	UCHAR ScsiClass::Write10(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::w_spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;

packet.Amount_Sectors					= AUDIO_BLOCK_SIZE;      
packet.Amount_Transfer				= packet.Size_Sectors * packet.Amount_Sectors;
packet.hcd_device							= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock					= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU
ScsiClass::w_spwb.Block.Cdb[0]	= 0X2A;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]		= packet.Write10Byte1;
*pointer                 							= BswapDword(&packet.Write10Lba);
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= 0;

ScsiClass::w_spwb.Block.Cdb[8]	= (BYTE) AUDIO_BLOCK_SIZE;
ScsiClass::w_spwb.Block.Cdb[9]	= 0;
ScsiClass::w_spwb.Block.Cdb[10]	= 0;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::WriteCDFull(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::w_spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;
//packet->Size_Sectors set by user
   		
packet.Amount_Transfer				= packet.Size_Sectors * packet.Amount_Sectors;
packet.hcd_device							= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock					= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU
ScsiClass::w_spwb.Block.Cdb[0]	= 0X2A;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]	= packet.Write10Byte1;
*pointer                 							= ScsiClass::BswapDword(&packet.Write10Lba);
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= 0;

ScsiClass::w_spwb.Block.Cdb[8]	= (BYTE)ScsiClass::packet.Amount_Sectors;
ScsiClass::w_spwb.Block.Cdb[9]	= 0;
ScsiClass::w_spwb.Block.Cdb[10]	= 0;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::WriteCDPart(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::w_spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;
//packet->Size_Sectors set by user		
//packet->Amount_Sectors set by user
packet.Amount_Transfer				= packet.Size_Sectors * packet.Amount_Sectors;
packet.hcd_device							= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock					= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU
ScsiClass::w_spwb.Block.Cdb[0]	= 0X2A;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]		= packet.Write10Byte1;
*pointer                 							= BswapDword(&packet.Write10Lba);
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= 0;
ScsiClass::w_spwb.Block.Cdb[8]	= (BYTE)ScsiClass::packet.Amount_Sectors;
ScsiClass::w_spwb.Block.Cdb[9]	= 0;
ScsiClass::w_spwb.Block.Cdb[10]	= 0;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
////**************************************
	UCHAR ScsiClass::Write12(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::w_spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;
//packet->Size_Sectors set by user  

packet.Amount_Sectors					= AUDIO_BLOCK_SIZE;           
packet.Amount_Transfer				= packet.Size_Sectors * packet.Amount_Sectors;
packet.hcd_device							= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock					= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU  
ScsiClass::w_spwb.Block.Cdb[0]	= 0XAA;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]		= packet.Write12Byte1;
*pointer                 							= BswapDword(&packet.Write12Lba);
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= 0;
ScsiClass::w_spwb.Block.Cdb[8]	= 0;

ScsiClass::w_spwb.Block.Cdb[9]	= (BYTE) AUDIO_BLOCK_SIZE;

ScsiClass::w_spwb.Block.Cdb[10]	= packet.Write12Streaming;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
//**************************************
	UCHAR ScsiClass::WriteVerify10(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{
void *dwordptr;
DWORD *pointer;

dwordptr = &ScsiClass::spwb.Block.Cdb[2];
pointer  = (DWORD *) dwordptr;
//packet->Size_Sectors set by user   

packet.Amount_Sectors					= AUDIO_BLOCK_SIZE;               
packet.Amount_Transfer				= packet.Size_Sectors * packet.Amount_Sectors;
packet.hcd_device							= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock					= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_UNSPECIFIED;//data direction to/from CPU  
ScsiClass::w_spwb.Block.Cdb[0]	= 0X2E;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]		= packet.Verify12Flags;
*pointer											= BswapDword(&packet.Verify12Lba);
ScsiClass::w_spwb.Block.Cdb[6]	= 0;
ScsiClass::w_spwb.Block.Cdb[7]	= packet.Verify12LengthMsb;
ScsiClass::w_spwb.Block.Cdb[8]	= packet.Verify12LengthLsb;
ScsiClass::w_spwb.Block.Cdb[9]	= 0;
ScsiClass::w_spwb.Block.Cdb[10]	= 0;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
////**************************************
	UCHAR ScsiClass::WriteBuffer(PACKET_DRIVER packet)//just set up packet and call SendScsi
	{

packet.Size_Sectors				= 0X800;
packet.Amount_Sectors			= 0;
packet.Amount_Transfer			= sizeof(ScsiClass::spwb.Data_Returned);
packet.hcd_device				= ScsiClass::CurrentScsiHandle;
ScsiClass::CurrentBlock			= TRUE;
ScsiClass::w_spwb.Block.DataIn	= SCSI_IOCTL_DATA_OUT;//data direction to/from CPU  
ScsiClass::w_spwb.Block.Cdb[0]	= 0X3B;//command scsi
ScsiClass::w_spwb.Block.Cdb[1]	= packet.WriteBufferMode;
ScsiClass::w_spwb.Block.Cdb[2]	= packet.WriteBufferId;
ScsiClass::w_spwb.Block.Cdb[3]	= packet.BufferOffsetMsb;
ScsiClass::w_spwb.Block.Cdb[4]	= packet.BufferOffsetHigh;
ScsiClass::w_spwb.Block.Cdb[5]	= packet.BufferOffsetLsb;
ScsiClass::w_spwb.Block.Cdb[6]	= packet.BufferAllocateMsb;
ScsiClass::w_spwb.Block.Cdb[7]	= packet.BufferAllocateHigh;
ScsiClass::w_spwb.Block.Cdb[8]	= packet.BufferAllocateLsb;
ScsiClass::w_spwb.Block.Cdb[9]	= 0;
ScsiClass::w_spwb.Block.Cdb[10]	= 0;
ScsiClass::w_spwb.Block.Cdb[11]	= 0;//end of command scsi
return(ScsiClass::SendScsi(packet));
	}
	
//****************************************** Main Function To Call Scsi Device 
//*****************************************************************************************************************
	UCHAR ScsiClass::SendScsi(PACKET_DRIVER packet)
	{//open code 1

		DWORD  written;
		DWORD dwLength;
		int count;
		char Buffer[32] = { 0 };
		BOOLEAN Device_Code;

		

		if (!ScsiClass::CurrentBlock)//only zero memory if not writing data to the device
		{//open 2
			ZeroMemory(ScsiClass::spwb.Data_Returned, sizeof(ScsiClass::spwb.Data_Returned));
			ScsiClass::spwb.Block.Length				= sizeof(SCSI_PASS_THROUGH);
			ScsiClass::spwb.Block.PathId				= 0;
			ScsiClass::spwb.Block.TargetId				= 0;
			ScsiClass::spwb.Block.Lun					= 0;
			ScsiClass::spwb.Block.CdbLength				= 12;
			ScsiClass::spwb.Block.SenseInfoLength		= sizeof(ScsiClass::spwb.Sense_Data);
			ScsiClass::spwb.Block.DataTransferLength	= packet.Amount_Transfer;

			if (ScsiClass::packet.ScsiTimeOutFlag)
				ScsiClass::spwb.Block.TimeOutValue = (ULONG)ScsiClass::packet.ScsiTimeOutValue;
			else
				ScsiClass::spwb.Block.TimeOutValue = 30;

			ScsiClass::spwb.Block.DataBufferOffset = offsetof(SCSI_BLOCK_USER, Data_Returned);
			ScsiClass::spwb.Block.SenseInfoOffset = offsetof(SCSI_BLOCK_USER, Sense_Data);
			ScsiClass::spwb.Block.ScsiStatus = 0;
			dwLength = offsetof(SCSI_BLOCK_USER, Data_Returned) + ScsiClass::spwb.Block.DataTransferLength;

			QueryPerformanceFrequency((LARGE_INTEGER *)&ScsiClass::Frequency.QuadPart);
			QueryPerformanceCounter((LARGE_INTEGER *)&ScsiClass::StartingTime.QuadPart);

			Device_Code = DeviceIoControl(packet.hcd_device, IOCTL_SCSI_PASS_THROUGH, (LONG64 *)&spwb, sizeof(SCSI_BLOCK_USER),
				(LONG64 *)&spwb, dwLength, &written, NULL);

			QueryPerformanceCounter((LARGE_INTEGER *)&ScsiClass::EndingTime.QuadPart);
			
			//SendDlgItemMessageA(packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(char *)LastPacketTime);
			if ((Device_Code) && (ScsiClass::spwb.Block.ScsiStatus == 0))
			{//open 3
				ScsiClass::ScsiDataReturnLen = ScsiClass::spwb.Block.DataTransferLength;
			
				for (count = 0; count < 12; count++)
				{//open 4
					wsprintfA(Buffer, " %2.2X", ScsiClass::spwb.Block.Cdb[count]);
					strcat_s(Buffer, sizeof(Buffer), "::");
					
				}//close 4
				return DEVICEIOCONTROL_OK;
			}//close 3

			if (Device_Code == 0)
			{//open 3
				ScsiClass::packet.LastError = GetLastError();
				return DEVICEIOCONTROL_FAILED;
			}//close 3

			if (ScsiClass::spwb.Block.ScsiStatus != 0)
			{//open 3
				//ScsiClass::SenseFlag		= TRUE;
				ScsiClass::LastCommandError = ScsiClass::spwb.Block.Cdb[0];

		
				for (count = 0; count < 12; count++)
				{//open 4
					wsprintfA(Buffer, " %2.2X", ScsiClass::spwb.Block.Cdb[count]);
					strcat_s(Buffer, sizeof(Buffer), "::");
				}//close 4
				DisplaySense(0);
				return DEVICEIOCONTROL_SENSE;
			}//close 3
			return DEVICEIOCONTROL_FAILED;
		}//close 2


		 //**********************************************************************************************************


		 //************************************* Write Data *********************************************************
		else if (ScsiClass::CurrentBlock)//only zero memory if not writing data to the device
		{//open 2		
			ScsiClass::w_spwb.Block.Length				= sizeof(SCSI_PASS_THROUGH);
			ScsiClass::w_spwb.Block.PathId				= 0;
			ScsiClass::w_spwb.Block.TargetId			= 0;
			ScsiClass::w_spwb.Block.Lun					= 0;
			ScsiClass::w_spwb.Block.CdbLength			= 12;
			ScsiClass::w_spwb.Block.SenseInfoLength		= sizeof(ScsiClass::w_spwb.Sense_Data);
			ScsiClass::w_spwb.Block.DataTransferLength	= packet.Amount_Transfer;
			ScsiClass::w_spwb.Block.TimeOutValue		= 30;
			ScsiClass::w_spwb.Block.DataBufferOffset	= offsetof(SCSI_BLOCK_USER, Data_Returned);
			ScsiClass::w_spwb.Block.SenseInfoOffset		= offsetof(SCSI_BLOCK_USER, Sense_Data);
			ScsiClass::w_spwb.Block.ScsiStatus			= 0;

			dwLength = offsetof(SCSI_BLOCK_USER, Data_Returned) + ScsiClass::w_spwb.Block.DataTransferLength;

			QueryPerformanceFrequency((LARGE_INTEGER *)&ScsiClass::Frequency.QuadPart);
			QueryPerformanceCounter((LARGE_INTEGER *)&ScsiClass::StartingTime.QuadPart);

			Device_Code = DeviceIoControl(packet.hcd_device, IOCTL_SCSI_PASS_THROUGH, (LONG64 *)&w_spwb, sizeof(SCSI_BLOCK_USER),
				(LONG64 *)&w_spwb, dwLength, &written, NULL);

			QueryPerformanceCounter((LARGE_INTEGER *)&ScsiClass::EndingTime.QuadPart);


			if ((Device_Code) && (ScsiClass::w_spwb.Block.ScsiStatus == 0))
				return DEVICEIOCONTROL_OK;

			if (Device_Code == 0)
				return DEVICEIOCONTROL_FAILED;

			if (ScsiClass::w_spwb.Block.ScsiStatus != 0)
			{//open 3
				//ScsiClass::SenseFlag = TRUE;
				ScsiClass::LastCommandError = ScsiClass::w_spwb.Block.Cdb[0];

				for (count = 0; count < 12; count++)
				{//open 4
					wsprintfA(Buffer, " %2.2X", ScsiClass::w_spwb.Block.Cdb[count]);

				}//close 4
				DisplaySense(1);
				return DEVICEIOCONTROL_SENSE;
			}// close 3
			return DEVICEIOCONTROL_FAILED;
		} // close 2
		return DEVICEIOCONTROL_FAILED;
		}//close 1
		 //***********************************************************************************************************

//***************************************** Functions Used By Class ScsiClass *****************************************************
	BOOLEAN ScsiClass::DisplaySense(BYTE SenseBuffer)
	{//open code 1
		char database[256];
		char string[32];
		char sense1[8];
		char sense2[8];
		char sense3[8];

		//char Command[32];
		WORD len;

		strcpy_s(string, sizeof(string), "string");
		strcat_s(string, sizeof(string), "_");
		if (SenseBuffer == 0)
		{//open 2
			wsprintfA(sense1, "%2.2X", ScsiClass::spwb.Sense_Data[2]);//sense key
			strcat_s(string, sizeof(string), sense1);
			strcat_s(string, sizeof(string), "_");
			wsprintfA(sense2, "%2.2X", ScsiClass::spwb.Sense_Data[12]);//additional sense code
			strcat_s(string, sizeof(string), sense2);
			strcat_s(string, sizeof(string), "_");
			wsprintfA(sense3, "%2.2X", ScsiClass::spwb.Sense_Data[13]);//additional sense code qualifier
			strcat_s(string, sizeof(string), sense3);
		}//close 2
		else if (SenseBuffer == 1)
		{//open 2
			wsprintfA(sense1, "%2.2X", ScsiClass::w_spwb.Sense_Data[2]);//sense key
			strcat_s(string, sizeof(string), sense1);
			strcat_s(string, sizeof(string), "_");
			wsprintfA(sense2, "%2.2X", ScsiClass::w_spwb.Sense_Data[12]);//additional sense code
			strcat_s(string, sizeof(string), sense2);
			strcat_s(string, sizeof(string), "_");
			wsprintfA(sense3, "%2.2X", ScsiClass::w_spwb.Sense_Data[13]);//additional sense code qualifier
			strcat_s(string, sizeof(string), sense3);
		}//close 2


		if ((ScsiClass::spwb.Block.ScsiStatus == 0) && (SenseBuffer != 1))
			return FALSE;
		if ((ScsiClass::w_spwb.Block.ScsiStatus == 0) && (SenseBuffer == 1))
			return FALSE;

		if (ScsiClass::CheckString(string, database, &len))
		{//open 2
			strcpy_s(ScsiClass::SenseError, database);
			ScsiClass::SenseLenght = len;

		}//close 2


		if ((ScsiClass::spwb.Sense_Data[15] & 0X80) == 0X80)
		{//open 2
			ScsiClass::SenseExtra = TRUE;
			return TRUE;
		}//close 2
		if ((ScsiClass::w_spwb.Sense_Data[15] & 0X80) == 0X80)
		{//open 2
			ScsiClass::SenseExtra = TRUE;
			return TRUE;
		}//close 2

		ScsiClass::SenseExtra = FALSE;
		return TRUE;
	
}//close code

//****************** BswapWord ***********************************************************************
	WORD  ScsiClass::BswapWord(WORD *swap_bytes)
	{
		WORD swap = *swap_bytes;
		BYTE Byte1, Byte2;

		Byte1 = LOBYTE(swap);
		Byte2 = HIBYTE(swap);
		swap = 0;
		swap = (BYTE)Byte1;
		swap = swap << 8;
		swap = swap + Byte2;
	
		return swap;
	}
	//****************** BswapDword *************************************************************************
	DWORD ScsiClass::BswapDword(DWORD *swap_bytes)//(10)
	{
		DWORD RetTurn;
		DWORD swapValue = *swap_bytes ;
		BYTE bytes[4];
		WORD words[2];

		words[0] = HIWORD(swapValue);
		words[1] = LOWORD(swapValue);
		bytes[0] = HIBYTE(words[0]);
		bytes[1] = LOBYTE(words[0]);
		bytes[2] = HIBYTE(words[1]);
		bytes[3] = LOBYTE(words[1]);
		RetTurn = bytes[3];
		RetTurn = RetTurn << 8;
		RetTurn = RetTurn + bytes[2];
		RetTurn = RetTurn << 8;
		RetTurn = RetTurn + bytes[1];
		RetTurn = RetTurn << 8;
		RetTurn = RetTurn + bytes[0];
		return RetTurn;
	}

	//****************** SetStringChars ***********************************************************************
	WORD  ScsiClass::SetStringChars(char *buffer, int amount, char* chars)
	{
		WORD amountset = 0;
		int count;

		for (count = 0; count < amount; count++)
		{
			*buffer = *chars;
			buffer++;
			chars++;
		}
		
		return amountset;
	}



//**************************************************
	UCHAR ScsiClass::CheckMedia()
	{
		BYTE						*dvd_0;
		char						Buffer[64];
		ULARGE_INTEGER				Sectors, SectorSize;
		LPPRE_PIT_INFO				PrePitInfo;
		ADIP_INFO					*Adip;
		LPFORMATCAPACITY			FormatDes;
		LPPERFORMANCE_INFO			PerformInfo;
		LPERROR_RECOVERY			Recovery;
		LPDISKINFO					disk;
		MSF							TempMsf1;
		MSF							TempMsf2;
		MSF							TempMsf3;
		FORMAT_2_FIELD_0			*Field_0;
		FORMAT_1_FIELD_1			*Field_1_1;
		FORMAT_1_FIELD_0			*Field_1_0;
		RMD_FIELD_ZERO				*RmdZero;
		BYTE						disc_status = 0;
		BYTE						disc_type = 0;
		BYTE						disc_sess = 0;
		BYTE						*bytecopy;
		BYTE						*bytecopycat;
		BYTE						*bytecopyisrc;
		BYTE						*volume;
		DWORD						*ptrdw;
		DWORD						*Capacity;
		DWORD						TempLba1;
		WORD						Erase_Amount;
		BOOLEAN						Dvd_Flag = FALSE;
		BOOLEAN						disc_erase = FALSE;
		BOOLEAN						Audio = FALSE;
		BOOLEAN						Data = FALSE;
		int							maxwait;
		void						*any, *any1;
		BYTE						*bytemaster, *bytedata;
		BYTE						*bytecaps;
		int							count, count1;
		UCHAR						retcode;
		BYTE						*bytecopydisc;


		volume = (BYTE*)&this->ScsiClass::VolumeDes;
		bytecopycat = (BYTE *)&this->ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog;
		bytecopy = (BYTE *)&this->ScsiClass::MediaInfo.CdMediaClass.TrackInfo;
		bytecopydisc = (BYTE *)&this->ScsiClass::MediaInfo.DiskInfo;
		bytecopyisrc = (BYTE *)&this->ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[0];
		bytemaster = (BYTE *)&this->ScsiClass::FeatureList.CdMastering;
		bytecaps = (BYTE *)&this->ScsiClass::FeatureList.Capabilities;

		any1 = &ScsiClass::MediaInfo.DvdMediaClass.DvdStructure_Zero;
		any = &ScsiClass::spwb.Data_Returned;

		dvd_0 = (BYTE *)any1;
		FormatDes = (FORMATCAPACITY *)any;
		disk = (LPDISKINFO)any;
		Capacity = (LPDWORD)any;
		ptrdw = (DWORD *)any;
		bytedata = (BYTE *)any;
		PerformInfo = (LPPERFORMANCE_INFO)any;
		Field_0 = (FORMAT_2_FIELD_0 *)any;
		Field_1_1 = (FORMAT_1_FIELD_1 *)any;
		Field_1_0 = (FORMAT_1_FIELD_0 *)any;
		Recovery = (LPERROR_RECOVERY)any;
		RmdZero = (LPRMD_FIELD_ZERO)any;
		PrePitInfo = (LPPRE_PIT_INFO)any;
		Adip = (ADIP_INFO *)any;
		CorrectMediaFlag = false;
		ScsiClass::MediaInfo.MediaEraseableFlag = false;
		ScsiClass::packet.ShowSense = TRUE;
		ScsiClass::spwb.Block.ScsiStatus = 0;
		ZeroMemory(&this->ScsiClass::MediaInfo, sizeof(MEDIA_DESCRIBTOR));//all media info zeroed before any new data added

		//ScsiClass::packet.ModeSenseCode					= 1;
		//retcode																	= ScsiClass::ModeSense(ScsiClass::packet);
		ScsiClass::GetFeatures(ScsiClass::packet);
		//***********************
		//test for door Open ***************************************************************************
		maxwait = 50;
		ScsiClass::WaitUntillReady(&maxwait);//make sure device is ready

		retcode = ScsiClass::MechStatus(ScsiClass::packet);
		if ((retcode == DEVICEIOCONTROL_OK) && ((ScsiClass::spwb.Data_Returned[1] & 0x10) == 0x10))//bit4 = 1 if door open
		{
			//	Reply = MessageBoxA(NULL, "Door is Open Do Which To Proceed?", "Info", MB_YESNO);
			//	if (Reply == IDYES)
			//	{
			//		ScsiClass::CloseDoor(ScsiClass::packet);
			//		maxwait = 150;
			//		ScsiClass::WaitUntillReady(&maxwait);//make sure device is ready
			//		retcode = ScsiClass::MechStatus(ScsiClass::packet);
			//		if ((retcode == DEVICEIOCONTROL_OK) && ((ScsiClass::spwb.Data_Returned[1] & 0x10) == 0x10))//bit4 = 1 if door open
			//		{	
			//		ScsiClass::MediaInfo.DoorOpenFlag	= true;
			//		ScsiClass::MediaInfo.NoMediaFlag		= true;
			//		ScsiClass::MediaInfo.DvdFlag				= false;
			//		ScsiClass::MediaInfo.CdFlag					= false;
			//		ScsiClass::LastMedia								= NONE;
			//		StringCchPrintf(ScsiClass::MediaInfo.DoorOpen.DoorOpen, _countof(ScsiClass::MediaInfo.DoorOpen.DoorOpen), L"Media Door Is Open");
			//		StringCchPrintf(ScsiClass::MediaInfo.NoMediaClass.NoMedia, _countof(ScsiClass::MediaInfo.NoMediaClass.NoMedia), L"Unable To Test Media Door Is open!"); 
			//		return(MEDIA_DOOR_OPEN);
			//		}
			//	}
			//	if (Reply == IDNO)
			//	{
			ScsiClass::MediaInfo.DoorOpenFlag = true;
			ScsiClass::MediaInfo.NoMediaFlag = true;
			ScsiClass::MediaInfo.DvdFlag = false;
			ScsiClass::MediaInfo.CdFlag = false;
			ScsiClass::LastMedia = NONE;
			StringCchPrintf(ScsiClass::MediaInfo.DoorOpen.DoorOpen, _countof(ScsiClass::MediaInfo.DoorOpen.DoorOpen), L"Media Door Is Open");
			StringCchPrintf(ScsiClass::MediaInfo.NoMediaClass.NoMedia, _countof(ScsiClass::MediaInfo.NoMediaClass.NoMedia), L"Unable To Test Media Door Is open!");
			return(MEDIA_DOOR_OPEN);
			//}
		}
		//Door is already closed OR has been closed

		//testing for start/stop (enable/disenable media access operations ****************************
		ScsiClass::packet.StartStop = 1;//start disc and read the TOC
		retcode = ScsiClass::StartStopUnit(ScsiClass::packet);
		if (retcode == DEVICEIOCONTROL_FAILED)//failed so no media in drive
		{
			StringCchPrintf(MediaInfo.NoMediaClass.NoMedia, _countof(MediaInfo.NoMediaClass.NoMedia), L"Failed To Read TOC");
			return retcode;
		}
		if (retcode == DEVICEIOCONTROL_SENSE)
		{
			ScsiClass::MediaInfo.UnknownFlag = true;
			ScsiClass::MediaInfo.NoMediaFlag = true;
			ScsiClass::MediaInfo.DvdFlag = false;
			ScsiClass::MediaInfo.CdFlag = false;
			ScsiClass::MediaInfo.DoorOpenFlag = false;
			StringCchPrintf(MediaInfo.UnKnownMedia.UnKnown, _countof(MediaInfo.UnKnownMedia.UnKnown), L"Unknown Media In Drive");
			StringCchPrintf(ScsiClass::MediaInfo.NoMediaClass.NoMedia, _countof(ScsiClass::MediaInfo.NoMediaClass.NoMedia), L"No Media In Drive");
			ScsiClass::MediaInfo.Current.CurrentDataType = NO_MEDIA;
			ScsiClass::MediaInfo.Current.CurrentMediaType = NONE;
			ScsiClass::LastMedia = NONE;
			ScsiClass::MediaInfo.Current.CurrentSectorSize = 0;
			ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = 0;
			ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = 0;
			return(NO_MEDIA_IN_DRIVE);
		}
		//end testing start/stop **********************************************************************
		//Delay = 400;
		//ScsiClass::WaitUntillReady(&Delay);
		//testing Disk Infomation and setting disk status in ScsiClass ********************************

		ScsiClass::ReadDiscInfo(ScsiClass::packet);
		ScsiClass::CurrentDiskStatus = ScsiClass::spwb.Data_Returned[2] & 3;
		ScsiClass::CurrentSessionStatus = (ScsiClass::spwb.Data_Returned[2] & 0xC) >> 2;

		//more data to class Brian Later 21/11/15 now used for dvd's for eraseable check (no dependence on book type)
		for (count = 0; count < 2048; count++)
		{
			*bytecopydisc = ScsiClass::spwb.Data_Returned[count];
			bytecopydisc++;
		}
		bytecopydisc -= 2048;//reset pointer back to spwt.Data_Return[0]
		if (ScsiClass::MediaInfo.DiskInfo.Erasable)
			ScsiClass::MediaInfo.MediaEraseableFlag = true;
		//end of Disk Information *********************************************************************
		//if here then door is closed and media has been detected so carry on checking media
		//next test to see if CD or a DVD Media is in the drive	

		//testing for DVD ******************************************************************************

		for (count = 0; count < 2048; count++) //zero memory used in class ScsiClass first
		{
			ScsiClass::spwb.Data_Returned[count] = 0;
		}
		//***********************

		ScsiClass::packet.ReadDvdAddress = 0;//dword in packet
		ScsiClass::packet.ReadDvdFormat = 0;//byte in packet
		ScsiClass::packet.ReadDvdLayer = 0;//byte in packet

		retcode = ScsiClass::ReadDvdStructure(ScsiClass::packet);
		//switch on retcode if ok must be a dvd if sense returned its a cd
		//Delay = 400;
		//ScsiClass::WaitUntillReady(&Delay);
		switch (retcode)
		{//open 1
		case DEVICEIOCONTROL_OK:
			ScsiClass::MediaInfo.DvdFlag = true;
			ScsiClass::MediaInfo.CdFlag = false;
			ScsiClass::MediaInfo.DoorOpenFlag = false;
			ScsiClass::MediaInfo.UnknownFlag = false;
			ScsiClass::MediaInfo.NoMediaFlag = false;
			ScsiClass::MediaInfo.UnknownFlag = false;
			ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize = 2048;
			ScsiClass::MediaInfo.Current.CurrentSectorSize = 2048;
			ScsiClass::MediaInfo.Current.CurrentMediaType = DVD;
			ScsiClass::LastMedia = DVD;
			ScsiClass::MediaInfo.Current.CurrentDataType = MODE_1;
			Dvd_Flag = TRUE;
			StringCchPrintf(MediaInfo.DvdMediaClass.DvdMedia, _countof(MediaInfo.DvdMediaClass.DvdMedia), L"The Current Media Is DVD");
			for (count = 0; count < 2048; count++)//if returned ok from ReadDvdStructure copy info to class will contain book/layer etc data 
			{
				*dvd_0 = ScsiClass::spwb.Data_Returned[count];
				dvd_0++;
			}
			//dvd_0 -= 2048;

			ScsiClass::MediaInfo.DvdMediaClass.PartVersion = MediaInfo.DvdMediaClass.DvdStructure_Zero.Part_Version;
			ScsiClass::MediaInfo.DvdMediaClass.DvdBookType = MediaInfo.DvdMediaClass.DvdStructure_Zero.Book_Type;
			ScsiClass::MediaInfo.DvdMediaClass.MaxRate = MediaInfo.DvdMediaClass.DvdStructure_Zero.Max_Rate;
			ScsiClass::MediaInfo.DvdMediaClass.DiscSize = MediaInfo.DvdMediaClass.DvdStructure_Zero.Disc_Size;
			ScsiClass::MediaInfo.DvdMediaClass.LayerType = MediaInfo.DvdMediaClass.DvdStructure_Zero.Layer_Type;
			ScsiClass::MediaInfo.DvdMediaClass.TrackPath = MediaInfo.DvdMediaClass.DvdStructure_Zero.Track_Path;
			ScsiClass::MediaInfo.DvdMediaClass.AmountOfLayers = MediaInfo.DvdMediaClass.DvdStructure_Zero.Amount_Layers;
			ScsiClass::MediaInfo.DvdMediaClass.TrackDensity = MediaInfo.DvdMediaClass.DvdStructure_Zero.Track_Density;
			ScsiClass::MediaInfo.DvdMediaClass.LinearDensity = MediaInfo.DvdMediaClass.DvdStructure_Zero.Linear_Density;
			ScsiClass::packet.PerformDataType = 16;//Tolerance = bits4-3 = 10 = 10%:bit2 = write:Except = bits1-0
			ScsiClass::packet.PerformStartMSB = 0;
			ScsiClass::packet.PerformStartHIGH = 0;
			ScsiClass::packet.PerformStartLOW = 0;
			ScsiClass::packet.PerformStartLSB = 0;
			ScsiClass::packet.PerformMaxMsb = 0;
			ScsiClass::packet.PerformMaxLsb = 8;
			ScsiClass::packet.PerformType = 0;

			retcode = ScsiClass::GetPerformance(ScsiClass::packet);
			if (retcode == DEVICEIOCONTROL_OK)
			{
				ScsiClass::MediaInfo.DvdMediaClass.DvdReadSpeedStart = ScsiClass::BswapDword(&PerformInfo->PerformanceStartData);
				ScsiClass::MediaInfo.DvdMediaClass.DvdReadSpeedStartLba = ScsiClass::BswapDword(&PerformInfo->StartLBA);
				ScsiClass::MediaInfo.DvdMediaClass.DvdReadSpeedEnd = ScsiClass::BswapDword(&PerformInfo->PerformanceEndData);
				ScsiClass::MediaInfo.DvdMediaClass.DvdReadSpeedEndLba = ScsiClass::BswapDword(&PerformInfo->EndLBA);
			}

			ScsiClass::packet.PerformDataType = 20;//Tolerance = bits4-3 = 10 = 10%:bit2 = write:Except = bits1-0
			ScsiClass::packet.PerformStartMSB = 0;
			ScsiClass::packet.PerformStartHIGH = 0;
			ScsiClass::packet.PerformStartLOW = 0;
			ScsiClass::packet.PerformStartLSB = 0;
			ScsiClass::packet.PerformMaxMsb = 0;
			ScsiClass::packet.PerformMaxLsb = 8;
			ScsiClass::packet.PerformType = 0;
			retcode = ScsiClass::GetPerformance(ScsiClass::packet);
			if (retcode == DEVICEIOCONTROL_OK)
			{

				ScsiClass::MediaInfo.DvdMediaClass.DvdWriteSpeedStart = ScsiClass::BswapDword(&PerformInfo->PerformanceStartData);
				ScsiClass::MediaInfo.DvdMediaClass.DvdWriteSpeedStartLba = ScsiClass::BswapDword(&PerformInfo->StartLBA);
				ScsiClass::MediaInfo.DvdMediaClass.DvdWriteSpeedEnd = ScsiClass::BswapDword(&PerformInfo->PerformanceEndData);
				ScsiClass::MediaInfo.DvdMediaClass.DvdWriteSpeedEndLba = ScsiClass::BswapDword(&PerformInfo->EndLBA);
			}
			//Media manufacturer info
			for (count = 0; count < 11; count++)
			{
				ScsiClass::MediaInfo.DvdMediaClass.MediaManufacturer[count] = MediaInfo.DvdMediaClass.DvdStructure_Zero.Media_Specific[count + 2];
				if (ScsiClass::MediaInfo.DvdMediaClass.MediaManufacturer[count] == 0)
					ScsiClass::MediaInfo.DvdMediaClass.MediaManufacturer[count] = 32;
			}
			switch (MediaInfo.DvdMediaClass.DvdStructure_Zero.Book_Type)//check the book type
			{//open 2
			case BOOK_DVD_ROM:
				ScsiClass::MediaInfo.MediaEraseableFlag = false;

				ScsiClass::packet.Read10Lba = 256;
				ScsiClass::packet.ReadByte = 8;
				ScsiClass::packet.Amount_Sectors = 4;
				ScsiClass::packet.Size_Sectors = 2048;
				ScsiClass::Read10Part(ScsiClass::packet);

				for (count = 0; count < 2048; count++)
				{
					*volume = ScsiClass::spwb.Data_Returned[count];
					volume++;
				}
				//*******************need to find if dvd is using udf format and adjust for it**************************
		//current is set from Volume descriptor Volume size because its a DVD_ROM 
				ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors = ScsiClass::VolumeDes.Volume_Space_LowHigh;//note fomat intel low/high
				Sectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors;
				ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize = 2048;
				ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart = Sectors.QuadPart * 2048;
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = Sectors.QuadPart * 2048;
				ScsiClass::packet.ReadDvdAddress = 2;//address 1 = field 0 : 2 = field 1 etc
				ScsiClass::packet.ReadDvdFormat = 0XD;
				ScsiClass::packet.ReadDvdLayer = 0;
				retcode = ScsiClass::ReadDvdStructure(ScsiClass::packet);//find format 1 field 1 logical and opc info data
				if (retcode == DEVICEIOCONTROL_OK)
				{
					for (count = 0; count < 16; count++)
						ScsiClass::MediaInfo.DvdMediaClass.DriveSerialNumber[count] = Field_1_1->FieldData[0].Serial_Number[count];
					ScsiClass::MediaInfo.DvdMediaClass.DriveSerialNumber[count] = 0;

					for (count = 0; count < 16; count++)
						ScsiClass::MediaInfo.DvdMediaClass.DriveModelNumber[count] = Field_1_1->FieldData[0].Model_Number[count];
					ScsiClass::MediaInfo.DvdMediaClass.DriveModelNumber[count] = 0;

				}


				return(DVD_MEDIA_ROM);//0=dvd_rom:1=dvd_ram:2=dvd_r:3=dvd-rw:9=dvd+rw	
				break;

			case BOOK_DVD_RAM:
				StringCchPrintf(MediaInfo.DvdMediaClass.DvdMediaType, _countof(MediaInfo.DvdMediaClass.DvdMediaType), L"There Is A DVD Ram Disc In The Drive");
				ScsiClass::MediaInfo.DvdMediaClass.DvdBookType = BOOK_DVD_RAM;
				ScsiClass::MediaInfo.MediaEraseableFlag = true;
				ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = 0;
				return(DVD_MEDIA_RAM);//0=dvd_rom:1=dvd_ram:2=dvd_r:3=dvd-rw:9=dvd+rw
				break;

			case BOOK_DVD_MINUS_R://more testing for DVD Minus R needed
				ScsiClass::MediaInfo.MediaEraseableFlag = false;
				ScsiClass::MediaInfo.DvdMediaClass.DvdBookType = BOOK_DVD_MINUS_R;
				StringCchPrintf(MediaInfo.DvdMediaClass.DvdMediaType, _countof(MediaInfo.DvdMediaClass.DvdMediaType), L"There Is A DVD-R Disc In The Drive");
				if (ScsiClass::MediaInfo.DiskInfo.DiscStatus == STATUS_COMPLETE)
				{
					ScsiClass::packet.Read10Lba = 16;
					ScsiClass::packet.ReadByte = 8;
					ScsiClass::packet.Amount_Sectors = 1;
					ScsiClass::packet.Size_Sectors = 2048;
					ScsiClass::Read10Part(ScsiClass::packet);

					for (count = 0; count < 2048; count++)
					{
						*volume = ScsiClass::spwb.Data_Returned[count];
						volume++;
					}
					ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors = ScsiClass::VolumeDes.Volume_Space_LowHigh;//note fomat intel low/high
					ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors;
					Sectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors;

					ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize = 2048;
					ScsiClass::MediaInfo.Current.CurrentSectorSize = 2048;

					ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart = Sectors.QuadPart * 2048;
					ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = Sectors.QuadPart * 2048;
				}

				else
				{
					retcode = ScsiClass::ReadFormatCapacity(ScsiClass::packet);
					if (retcode == DEVICEIOCONTROL_OK)
					{
						//current
						ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors = ScsiClass::BswapDword(&FormatDes->CurrentCapacity.NumberOfBlocks);
						ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors;
						Sectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors;

						ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize = ScsiClass::BswapWord(&FormatDes->CurrentCapacity.BlockLenghtLow);
						SectorSize.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize;
						ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart = Sectors.QuadPart * SectorSize.QuadPart;
						ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart;
					}

				}

				ScsiClass::packet.ReadDvdAddress = 1;//address 1 = field 0 : 2 = field 1 etc
				ScsiClass::packet.ReadDvdFormat = 0XD;
				ScsiClass::packet.ReadDvdLayer = 0;
				retcode = ScsiClass::ReadDvdStructure(ScsiClass::packet);//find format 1 field 1 logical and opc info data
				if (retcode == DEVICEIOCONTROL_OK)
				{
					for (count = 0; count < 6; count++)
						ScsiClass::MediaInfo.DvdMediaClass.MediaManufacturer[count] = Field_0->Pre_Pit.Manufacture_id_1[count];
					ScsiClass::MediaInfo.DvdMediaClass.MediaManufacturer[6] = '-';
					ScsiClass::MediaInfo.DvdMediaClass.MediaManufacturer[7] = 0;
					for (count = 0; count < 6; count++)
						Buffer[count] = Field_0->Pre_Pit.Manufacture_id_2[count];
					Buffer[6] = 0;
					strcat_s(MediaInfo.DvdMediaClass.MediaManufacturer, sizeof(MediaInfo.DvdMediaClass.MediaManufacturer), Buffer);
				}

				ScsiClass::packet.ReadDvdAddress = 2;//address 1 = field 0 : 2 = field 1 etc
				ScsiClass::packet.ReadDvdFormat = 0XD;
				ScsiClass::packet.ReadDvdLayer = 0;
				retcode = ScsiClass::ReadDvdStructure(ScsiClass::packet);//find format 1 field 1 logical and opc info data
				if (retcode == DEVICEIOCONTROL_OK)
				{
					for (count = 0; count < 32; count++)
						ScsiClass::MediaInfo.DvdMediaClass.DriveManufacturerID[count] = Field_1_1->FieldData[0].Drive_Id[count];
					ScsiClass::MediaInfo.DvdMediaClass.DriveManufacturerID[count] = 0;

					for (count = 0; count < 16; count++)
						ScsiClass::MediaInfo.DvdMediaClass.DriveSerialNumber[count] = Field_1_1->FieldData[0].Serial_Number[count];
					ScsiClass::MediaInfo.DvdMediaClass.DriveSerialNumber[count] = 0;

					for (count = 0; count < 16; count++)
						ScsiClass::MediaInfo.DvdMediaClass.DriveModelNumber[count] = Field_1_1->FieldData[0].Model_Number[count];
					ScsiClass::MediaInfo.DvdMediaClass.DriveModelNumber[count] = 0;

				}
				return(DVD_MEDIA_R);//0=dvd_rom:1=dvd_ram:2=dvd_r:3=dvd-rw:9=dvd+rw
				break;

			case BOOK_DVD_MINUS_RW:
				ScsiClass::MediaInfo.MediaEraseableFlag = true;
				ScsiClass::MediaInfo.DvdMediaClass.DvdBookType = BOOK_DVD_MINUS_RW;
				StringCchPrintf(MediaInfo.DvdMediaClass.DvdMediaType, _countof(MediaInfo.DvdMediaClass.DvdMediaType), L"There Is A DVD MINUS RW Disc In The Drive");
				retcode = ScsiClass::ReadFormatCapacity(ScsiClass::packet);
				if (retcode == DEVICEIOCONTROL_OK)
				{
					//current
					ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors = ScsiClass::BswapDword(&FormatDes->CurrentCapacity.NumberOfBlocks);
					ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors;
					Sectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors;

					ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize = ScsiClass::BswapWord(&FormatDes->CurrentCapacity.BlockLenghtLow);
					SectorSize.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize;
					ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart = Sectors.QuadPart * SectorSize.QuadPart;
					ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart;
				}
				//max
				ScsiClass::MediaInfo.DvdMediaClass.DvdMaxSectors = ScsiClass::BswapDword(&FormatDes->Capacity[0].NumberOfBlocks);
				Sectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdMaxSectors;
				SectorSize.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize;
				ScsiClass::MediaInfo.DvdMediaClass.DvdMaxBytes.QuadPart = Sectors.QuadPart * SectorSize.QuadPart;
				//info
				ScsiClass::packet.ReadDvdAddress = 1;//
				ScsiClass::packet.ReadDvdFormat = 0XD;
				ScsiClass::packet.ReadDvdLayer = 0;
				retcode = ScsiClass::ReadDvdStructure(ScsiClass::packet);
				if (retcode == DEVICEIOCONTROL_OK)
				{
					CopyMemory(&FieldZero, RmdZero, sizeof(RMD_FIELD_ZERO));
				}

				ScsiClass::packet.ReadDvdAddress = 2;//address 1 = field 0 : 2 = field 1 etc
				ScsiClass::packet.ReadDvdFormat = 0XD;//return rmd start in address field
				ScsiClass::packet.ReadDvdLayer = 0;
				retcode = ScsiClass::ReadDvdStructure(ScsiClass::packet);//find amount of times this media has been erased

				if (retcode == DEVICEIOCONTROL_OK)
				{
					Erase_Amount = ScsiClass::spwb.Data_Returned[20];
					Erase_Amount = Erase_Amount << 8;
					Erase_Amount = Erase_Amount + ScsiClass::spwb.Data_Returned[21];
					ScsiClass::MediaInfo.DvdMediaClass.AmountErase = Erase_Amount;
				}

				ScsiClass::packet.ReadDvdAddress = 1;//address 1 = field 0 : 2 = field 1 etc
				ScsiClass::packet.ReadDvdFormat = 0XD;
				ScsiClass::packet.ReadDvdLayer = 0;
				retcode = ScsiClass::ReadDvdStructure(ScsiClass::packet);//find format 1 field 1 logical and opc info data
				if (retcode == DEVICEIOCONTROL_OK)
				{
					for (count = 0; count < 6; count++)
						ScsiClass::MediaInfo.DvdMediaClass.MediaManufacturer[count] = Field_0->Pre_Pit.Manufacture_id_1[count];
					ScsiClass::MediaInfo.DvdMediaClass.MediaManufacturer[6] = '-';
					ScsiClass::MediaInfo.DvdMediaClass.MediaManufacturer[7] = 0;
					for (count = 0; count < 6; count++)
						Buffer[count] = Field_0->Pre_Pit.Manufacture_id_2[count];
					Buffer[6] = 0;
					strcat_s(MediaInfo.DvdMediaClass.MediaManufacturer, sizeof(MediaInfo.DvdMediaClass.MediaManufacturer), Buffer);
				}
				return(DVD_MEDIA_MINUS_RW);//0=dvd_rom:1=dvd_ram:2=dvd_r:3=dvd-rw:9=dvd+rw
				break;

			case BOOK_DVD_PLUS_RW:
				ScsiClass::MediaInfo.MediaEraseableFlag = true;
				ScsiClass::MediaInfo.DvdMediaClass.DvdBookType = BOOK_DVD_PLUS_RW;
				StringCchPrintf(MediaInfo.DvdMediaClass.DvdMediaType, _countof(MediaInfo.DvdMediaClass.DvdMediaType), L"There Is A DVD PLUS RW Disc In The Drive");
				retcode = ScsiClass::ReadFormatCapacity(ScsiClass::packet);
				if (retcode == DEVICEIOCONTROL_OK)
				{
					ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors = ScsiClass::BswapDword(&FormatDes->CurrentCapacity.NumberOfBlocks);
					ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors;
					Sectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors;
					ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize = ScsiClass::BswapWord(&FormatDes->CurrentCapacity.BlockLenghtLow);
					SectorSize.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize;
					ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart = Sectors.QuadPart * SectorSize.QuadPart;
					ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart;
					//max
					ScsiClass::MediaInfo.DvdMediaClass.DvdMaxSectors = ScsiClass::BswapDword(&FormatDes->Capacity[0].NumberOfBlocks);
					Sectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdMaxSectors;
					SectorSize.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize;
					ScsiClass::MediaInfo.DvdMediaClass.DvdMaxBytes.QuadPart = Sectors.QuadPart * SectorSize.QuadPart;
				}
				return(DVD_MEDIA_PLUS_RW);//0=dvd_rom:1=dvd_ram:2=dvd_r:3=dvd-rw:9=dvd+rw
				break;
				//**********************
			case BOOK_DVD_PLUS_R:
				ScsiClass::MediaInfo.MediaEraseableFlag = false;
				StringCchPrintf(MediaInfo.DvdMediaClass.DvdMediaType, _countof(MediaInfo.DvdMediaClass.DvdMediaType), L"There Is A DVD +R Disc In The Drive");
				ScsiClass::MediaInfo.DvdMediaClass.DvdBookType = BOOK_DVD_PLUS_R;

				ScsiClass::packet.Read10Lba = 16;
				ScsiClass::packet.ReadByte = 8;
				ScsiClass::packet.Amount_Sectors = 1;
				ScsiClass::packet.Size_Sectors = 2048;
				ScsiClass::Read10Part(ScsiClass::packet);

				for (count = 0; count < 2048; count++)
				{
					*volume = ScsiClass::spwb.Data_Returned[count];
					volume++;
				}
				ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors = ScsiClass::VolumeDes.Volume_Space_LowHigh;//note fomat intel low/high
				ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors;
				Sectors.QuadPart = ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentSectors;

				ScsiClass::MediaInfo.DvdMediaClass.DvdSectorSize = 2048;
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 2048;

				ScsiClass::MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart = Sectors.QuadPart * 2048;
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = Sectors.QuadPart * 2048;
				return(DVD_MEDIA_PLUS_R);//0=dvd_rom:1=dvd_ram:2=dvd_r:3=dvd-rw:9=dvd+rw
				break;
				//*************************
			default:
				ScsiClass::MediaInfo.UnknownFlag = true;
				StringCchPrintf(ScsiClass::MediaInfo.UnKnownMedia.UnKnown, _countof(ScsiClass::MediaInfo.UnKnownMedia.UnKnown), L"There Is A Unknown DVD Disc In The Drive");
				ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentMediaType = DVD;
				ScsiClass::LastMedia = DVD;
				return(DVD_MEDIA_UNKNOWN);
				break;

			}//close dvd book type
		//*************************************** Close If  ReadDvdStructure was OK ***********************************************


		case DEVICEIOCONTROL_SENSE:
			ScsiClass::MediaInfo.DvdFlag = false;
			ScsiClass::MediaInfo.CdFlag = true;
			//***************** Testing for cd media manufacturer ********
		   /*
		   packet.ReadTocFormat = 4;
		   retcode = ScsiClass::ReadTocPmaAtip(packet);
		   //**********************************************************
		   //******** Now Test Reading from media
		   //now check for a Audio/Data media
		   packet.ModeSenseCode = 0X0E;
		   retcode = ScsiClass::ModeSense(packet);
		   //***************** end of testing ****************************
		   */

			retcode = ScsiClass::ReadDiscInfo(packet);
			if ((retcode == DEVICEIOCONTROL_FAILED) || (retcode == DEVICEIOCONTROL_SENSE))//if failed to get discinfo then its unknown
			{
				ScsiClass::MediaInfo.UnknownFlag = true;
				StringCchPrintf(ScsiClass::MediaInfo.UnKnownMedia.UnKnown, _countof(ScsiClass::MediaInfo.UnKnownMedia.UnKnown), L"There Is A Unknown CD In The Dive");
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 0;
				ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentDataType = UNKNOWN;
				ScsiClass::MediaInfo.Current.CurrentMediaType = NONE;
				ScsiClass::LastMedia = NONE;
				return(CD_MEDIA_IN_DRIVE_UNKNOWN);
			}

			if (disk->Erasable)//disk is a pointer to diskinfo structure
			{
				disc_erase = TRUE;
				ScsiClass::MediaInfo.MediaEraseableFlag = true;
			}
			else
			{
				disc_erase = FALSE;
				ScsiClass::MediaInfo.MediaEraseableFlag = false;
			}
			disc_status = disk->DiscStatus;
			disc_type = disk->DiscType;
			ScsiClass::CurrentDiskStatus = disk->DiscStatus;//read current diskstatus into the class
			if ((ScsiClass::CurrentDiskStatus == 0) && (!ScsiClass::MediaInfo.MediaEraseableFlag))//if 0 then disc is empty
			{
				ScsiClass::packet.PerformDataType = 16;//Tolerance = bits4-3 = 10 = 10%:bit2 = write:Except = bits1-0
				ScsiClass::packet.PerformStartMSB = 0;
				ScsiClass::packet.PerformStartHIGH = 0;
				ScsiClass::packet.PerformStartLOW = 0;
				ScsiClass::packet.PerformStartLSB = 0;
				ScsiClass::packet.PerformMaxMsb = 0;
				ScsiClass::packet.PerformMaxLsb = 8;
				ScsiClass::packet.PerformType = 0;
				retcode = ScsiClass::GetPerformance(ScsiClass::packet);
				if (retcode == DEVICEIOCONTROL_OK)
				{
					ScsiClass::MediaInfo.CdMediaClass.CdReadSpeedStart = ScsiClass::BswapDword(&PerformInfo->PerformanceStartData);
					ScsiClass::MediaInfo.CdMediaClass.CdReadSpeedStartLba = ScsiClass::BswapDword(&PerformInfo->StartLBA);
					ScsiClass::MediaInfo.CdMediaClass.CdReadSpeedEnd = ScsiClass::BswapDword(&PerformInfo->PerformanceEndData);
					ScsiClass::MediaInfo.CdMediaClass.CdReadSpeedEndLba = ScsiClass::BswapDword(&PerformInfo->EndLBA);
				}

				ScsiClass::packet.PerformDataType = 20;//Tolerance = bits4-3 = 10 = 10%:bit2 = write:Except = bits1-0
				ScsiClass::packet.PerformStartMSB = 0;
				ScsiClass::packet.PerformStartHIGH = 0;
				ScsiClass::packet.PerformStartLOW = 0;
				ScsiClass::packet.PerformStartLSB = 0;
				ScsiClass::packet.PerformMaxMsb = 0;
				ScsiClass::packet.PerformMaxLsb = 8;
				ScsiClass::packet.PerformType = 0;
				retcode = ScsiClass::GetPerformance(ScsiClass::packet);
				if (retcode == DEVICEIOCONTROL_OK)
				{
					ScsiClass::MediaInfo.CdMediaClass.CdWriteSpeedStart = ScsiClass::BswapDword(&PerformInfo->PerformanceStartData);
					ScsiClass::MediaInfo.CdMediaClass.CdWriteSpeedStartLba = ScsiClass::BswapDword(&PerformInfo->StartLBA);
					ScsiClass::MediaInfo.CdMediaClass.CdWriteSpeedEnd = ScsiClass::BswapDword(&PerformInfo->PerformanceEndData);
					ScsiClass::MediaInfo.CdMediaClass.CdWriteSpeedEndLba = ScsiClass::BswapDword(&PerformInfo->EndLBA);
				}
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "There Is a CD R Empty Disc In The Drive");
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 0;
				ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentDataType = UNKNOWN;
				ScsiClass::MediaInfo.Current.CurrentMediaType = CD_R;
				ScsiClass::LastMedia = CD_R;
				return(MEDIA_IN_DRIVE_BLANK_CD);
			}
			ScsiClass::packet.PerformDataType = 16;//Tolerance = bits4-3 = 10 = 10%:bit2 = write:Except = bits1-0
			ScsiClass::packet.PerformStartMSB = 0;
			ScsiClass::packet.PerformStartHIGH = 0;
			ScsiClass::packet.PerformStartLOW = 0;
			ScsiClass::packet.PerformStartLSB = 0;
			ScsiClass::packet.PerformMaxMsb = 0;
			ScsiClass::packet.PerformMaxLsb = 8;
			ScsiClass::packet.PerformType = 0;
			retcode = ScsiClass::GetPerformance(ScsiClass::packet);
			if (retcode == DEVICEIOCONTROL_OK)
			{
				ScsiClass::MediaInfo.CdMediaClass.CdReadSpeedStart = ScsiClass::BswapDword(&PerformInfo->PerformanceStartData);
				ScsiClass::MediaInfo.CdMediaClass.CdReadSpeedStartLba = ScsiClass::BswapDword(&PerformInfo->StartLBA);
				ScsiClass::MediaInfo.CdMediaClass.CdReadSpeedEnd = ScsiClass::BswapDword(&PerformInfo->PerformanceEndData);
				ScsiClass::MediaInfo.CdMediaClass.CdReadSpeedEndLba = ScsiClass::BswapDword(&PerformInfo->EndLBA);
			}
			ScsiClass::packet.PerformDataType = 20;//Tolerance = bits4-3 = 10 = 10%:bit2 = write:Except = bits1-0
			ScsiClass::packet.PerformStartMSB = 0;
			ScsiClass::packet.PerformStartHIGH = 0;
			ScsiClass::packet.PerformStartLOW = 0;
			ScsiClass::packet.PerformStartLSB = 0;
			ScsiClass::packet.PerformMaxMsb = 0;
			ScsiClass::packet.PerformMaxLsb = 8;
			ScsiClass::packet.PerformType = 0;
			retcode = ScsiClass::GetPerformance(ScsiClass::packet);
			if (retcode == DEVICEIOCONTROL_OK)
			{
				ScsiClass::MediaInfo.CdMediaClass.CdWriteSpeedStart = ScsiClass::BswapDword(&PerformInfo->PerformanceStartData);
				ScsiClass::MediaInfo.CdMediaClass.CdWriteSpeedStartLba = ScsiClass::BswapDword(&PerformInfo->StartLBA);
				ScsiClass::MediaInfo.CdMediaClass.CdWriteSpeedEnd = ScsiClass::BswapDword(&PerformInfo->PerformanceEndData);
				ScsiClass::MediaInfo.CdMediaClass.CdWriteSpeedEndLba = ScsiClass::BswapDword(&PerformInfo->EndLBA);
			}
			//disc_status = disk->DiscStatus;
			//disc_type = disk->DiscType;
		//****************************************** start of switch(disc_type)
			switch (disc_type)//returning only if CD_ROM_XA Or CD_ROM_I Media Found
			{
			case CD_ROM_XA:
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "There Is A CD XA Disc In The Drive");
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaData, "The Data Is Rom Ex Architecture");
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 0;
				ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentDataType = ROM_XA;
				ScsiClass::MediaInfo.Current.CurrentMediaType = CD;
				ScsiClass::LastMedia = CD;
				return(CD_MEDIA_IN_DRIVE_XA);
				break;

			case CD_ROM_I:
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "There Is A CD I Disc In The Drive");
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaData, "The Data Must be Played on CD Players");
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 0;
				ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentDataType = ROM_I;
				ScsiClass::MediaInfo.Current.CurrentMediaType = CD;
				ScsiClass::LastMedia = CD;
				return(CD_MEDIA_IN_DRIVE_I);
				break;

			case CD_DA_ROM://posible CD_DA OR CD_ROM So Contine Checking this data will be good for CD_DA And CD_RW AUDIO Media's
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 2352;
				ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentDataType = AUDIO_DATA;
				ScsiClass::MediaInfo.Current.CurrentMediaType = CD;
				ScsiClass::LastMedia = CD;
				break;


			case CD_UNDEFINED:
				ScsiClass::MediaInfo.UnknownFlag = true;
				StringCchPrintf(ScsiClass::MediaInfo.UnKnownMedia.UnKnown, _countof(ScsiClass::MediaInfo.UnKnownMedia.UnKnown), L"There Is A CD Undefined Disc (1) In The Drive");
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 0;
				ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentDataType = UNKNOWN;
				ScsiClass::MediaInfo.Current.CurrentMediaType = CD;
				ScsiClass::LastMedia = CD;
				break;
			}
			//******************************************* end of switch(disc_type)
			disc_sess = disk->SessionStatus;
			//now check for a Audio/Data media	
			ScsiClass::packet.ReadTocFormat = 1;
			ScsiClass::packet.ReadTocMsf = 2;
			ScsiClass::packet.ReadTocTrack = 0;
			retcode = ScsiClass::ReadTocPmaAtip(ScsiClass::packet);
			if (retcode == DEVICEIOCONTROL_OK)
			{
				if ((ScsiClass::spwb.Data_Returned[5] & 0XF) >= 4 && (ScsiClass::spwb.Data_Returned[5] & 0XF) <= 5)//was 7
					//first test = control if 4 == data track recorded uninterrupted second test = control if 5 == data track recorded incremental 
				{
					Data = TRUE;
					Audio = FALSE;
				}
				else
				{
					Data = FALSE;
					Audio = TRUE;
				}
			}
			if ((Audio && disc_erase) && (disc_status == 2) && (disc_type == 0))
				//disc_status == 2 complected disc not appendable disc_type == 0 CD_DA or CD_ROM
			{
				ScsiClass::MediaInfo.CdMediaClass.AudioFlag = true;
				ScsiClass::MediaInfo.CdMediaClass.DataFlag = false;
				ScsiClass::packet.ReadTocFormat = 0;//read all tracks including leadout For Start Times
				ScsiClass::packet.ReadTocMsf = 2;//select MSF
				ScsiClass::packet.ReadTocTrack = 0;
				ScsiClass::packet.ShowSense = TRUE;

				ScsiClass::MediaInfo.CdMediaClass.CdSectorSize = 2352;
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 2352;
				ScsiClass::MediaInfo.Current.CurrentMediaType = CD;
				ScsiClass::LastMedia = CD;
				//***********************************************************
				//get tocpmatip info 
				retcode = ScsiClass::ReadTocPmaAtip(ScsiClass::packet);//get the first/last tracks entry and details of all tracks and copy this to media.trackinfo

				if (retcode == DEVICEIOCONTROL_OK)//and place into MediaInfo.CdMediaaClass.TrackInfo
				{
					for (count = 0; count < sizeof(TOCMSF_INFORMATION); count++)
					{
						*bytecopy = (BYTE)ScsiClass::spwb.Data_Returned[count];//bytecopy set to Media>CdMediaClass.TrackInfo[0] at start
						bytecopy++;
					}
					strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaType, "There Is A CD Audio/Rom Disc In The Drive");
				}
				//**********************************************************
				//***************Get Media Catalog and copy to the class
				ScsiClass::packet.ReadSubFormat = 2;//read media catalog
				ScsiClass::packet.ReadSubTrack = 0;
				ScsiClass::packet.ReadSubMsf = 0;
				//get Read_Subcode info 
				retcode = ScsiClass::ReadSubChannel(ScsiClass::packet);

				if (retcode == DEVICEIOCONTROL_OK)
				{
					for (count = 0; count < sizeof(ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog); count++)
					{
						*bytecopycat = (BYTE)ScsiClass::spwb.Data_Returned[count];
						bytecopycat++;
					}
				}


				//***************Get ISRC and copy to the class
				ScsiClass::packet.ReadSubFormat = 3;//read isrc info
				ScsiClass::packet.ReadSubTrack = 1;//get first track
				ScsiClass::packet.ReadSubMsf = 0;

				for (count1 = 0; count1 <= ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Last_Track; count1++)//amount of tracks
				{//open 2	
					retcode = ScsiClass::ReadSubChannel(ScsiClass::packet);
					if (retcode == DEVICEIOCONTROL_SENSE)
					{//open 3
						ScsiClass::SenseFlag = TRUE;
						ScsiClass::packet.ShowSense = TRUE;
						break;
					}//close 3
					if (retcode == DEVICEIOCONTROL_OK)
					{//open 3
						if ((ScsiClass::spwb.Data_Returned[8] & 0x80) != 0x80)
							break;
						for (count = 0; count < sizeof(ISRC_CODE); count++)
						{//open 4
							*bytecopyisrc = (BYTE)ScsiClass::spwb.Data_Returned[count];
							bytecopyisrc++;
						}//close 4
						ScsiClass::packet.ReadSubTrack = ScsiClass::packet.ReadSubTrack + 1;
						ScsiClass::packet.ReadSubMsf = 0;
						ScsiClass::packet.ReadSubFormat = 3;//read isrc info
					}//close 3
				}//close 2

				ScsiClass::packet.ScsiTimeOutFlag = FALSE;



				//************************************************

				//get details of track start/end/lenght in msf and lba format for all valid tracks
				if ((ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Last_Track != 0) && (ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Last_Track <= MAX_TRACKS))
				{
					ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLoadedFlag = false;
					for (count1 = 0; count1 < ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Last_Track + 1; count1++)//amount of tracks
					{
						//getting start msf from traclinfo and copying ito TrackInfo also finding start lba and copying into TrackInfo
						TempMsf1 = ScsiClass::ConvertBytesToMsf(&ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Tracks[count1].Msf_Unsed);
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[count1] = TempMsf1;
						TempLba1 = ScsiClass::ConvertMsfLba(TempMsf1);
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartLba[count1] = TempLba1;
						//************************ End Of TrackStart MSF/LBA
						//Getting TrackLenght MSF/LBA And Setting TrackInfo *****************************************************
						TempMsf2 = ScsiClass::ConvertBytesToMsf(&ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Tracks[count1 + 1].Msf_Unsed);
						//ScsiClass::SubMsfMsf(TempMsf2,MsfTwoSecond,&TempMsf2);
						if (!ScsiClass::SubMsfMsf(TempMsf2, TempMsf1, &TempMsf3))
							break;
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLenghtMsf[count1] = TempMsf3;
						TempLba1 = ScsiClass::ConvertMsfLba(TempMsf3);
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLenghtLba[count1] = TempLba1;
						//************************************************
						//Getting TrackEnd
						TempMsf1 = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[count1];
						TempMsf2 = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLenghtMsf[count1];
						if (!ScsiClass::AddMsfMsf(TempMsf2, TempMsf1, &TempMsf3))
							break;
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackEndMsf[count1] = TempMsf3;
						TempLba1 = ScsiClass::ConvertMsfLba(TempMsf3);
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackEndLba[count1] = TempLba1;
					}
					ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLoadedFlag = true;
				}

				//**********************************************************

				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "There Is A CD Audio R/W In The Drive");
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaData, "The Media Is CD_RW");
				ScsiClass::MediaInfo.CdFlag = true;
				ScsiClass::MediaInfo.CdMediaClass.DataFlag = false;
				ScsiClass::MediaInfo.CdMediaClass.AudioFlag = true;
				ScsiClass::MediaInfo.Current.CurrentDataType = AUDIO_DATA;
				ScsiClass::MediaInfo.Current.CurrentMediaType = CD_RW;
				ScsiClass::LastMedia = CD_RW;
				ScsiClass::MediaInfo.CdMediaClass.CdSectorSize = 2352;
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 2352;
				retcode = ScsiClass::ReadCapacity(ScsiClass::packet);
				if (retcode == DEVICEIOCONTROL_OK)
				{
					ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors = ScsiClass::BswapDword(Capacity);
					ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors;
					ScsiClass::MediaInfo.CdMediaClass.CdMaxBytes = ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors * 2352;
					ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = ScsiClass::MediaInfo.CdMediaClass.CdMaxBytes;
					ScsiClass::MediaInfo.Current.CurrentDataType = AUDIO_DATA;
				}
				return(CD_MEDIA_IN_DRIVE_AUDIO_RW);
			}
			else if ((Audio && !disc_erase) && (disc_status == 2) && (disc_type == 0))//if audio and NOT eraseable and is a complete disc and is a CD_DA OR CD_ROM
			{
				ScsiClass::MediaInfo.CdMediaClass.AudioFlag = true;
				ScsiClass::MediaInfo.CdMediaClass.DataFlag = false;
				ScsiClass::packet.ReadTocFormat = 0;//read all tracks including leadout For Start Times
				ScsiClass::packet.ReadTocMsf = 2;//select MSF
				ScsiClass::packet.ReadTocTrack = 0;
				ScsiClass::packet.ShowSense = TRUE;
				ScsiClass::MediaInfo.CdMediaClass.CdSectorSize = 2352;
				//***********************************************************
				//get tocpmatip info 
				retcode = ScsiClass::ReadTocPmaAtip(ScsiClass::packet);//get the first/last tracks entry and details of all tracks and copy this to media.trackinfo

				if (retcode == DEVICEIOCONTROL_OK)//and place into MediaInfo.CdMediaaClass.TrackInfo
				{
					for (count = 0; count < sizeof(TOCMSF_INFORMATION); count++)
					{
						*bytecopy = (BYTE)ScsiClass::spwb.Data_Returned[count];//bytecopy set to Media>CdMediaClass.TrackInfo[0] at start
						bytecopy++;
					}
					strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaType, "There Is A CD Audio/Rom Disc In The Drive");
				}

				//***************Get Media Catalog and copy to the class
				ScsiClass::packet.ReadSubFormat = 2;//read media catalog
				ScsiClass::packet.ReadSubTrack = 0;
				ScsiClass::packet.ReadSubMsf = 0;
				//get Read_Subcode info 
				retcode = ScsiClass::ReadSubChannel(ScsiClass::packet);

				if (retcode == DEVICEIOCONTROL_OK)
				{
					for (count = 0; count < sizeof(ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog); count++)
					{
						*bytecopycat = (BYTE)ScsiClass::spwb.Data_Returned[count];
						bytecopycat++;
					}
				}

				//***************Get ISRC and copy to the class
				ScsiClass::packet.ReadSubFormat = 3;//read isrc info
				ScsiClass::packet.ReadSubTrack = 1;//get first track
				ScsiClass::packet.ReadSubMsf = 0;
				for (count1 = 0; count1 < ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Last_Track; count1++)//amount of tracks
				{//open 2	
					retcode = ScsiClass::ReadSubChannel(ScsiClass::packet);
					if ((retcode == DEVICEIOCONTROL_SENSE) || (retcode == DEVICEIOCONTROL_FAILED) || \
						(!ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[count].Tc_Valid))
					{//open 3
						ScsiClass::SenseFlag = FALSE;
						ScsiClass::packet.ShowSense = FALSE;
						break;
					}//close 3
					if (retcode == DEVICEIOCONTROL_OK)
					{//open 3
						//DataReturnedLen = ScsiClass::ScsiDataReturnLen;
						for (count = 0; count < sizeof(ISRC_CODE); count++)
						{//open 4
							*bytecopyisrc = (BYTE)ScsiClass::spwb.Data_Returned[count];
							bytecopyisrc++;
						}//close 4
						ScsiClass::packet.ReadSubTrack = ScsiClass::packet.ReadSubTrack + 1;
						ScsiClass::packet.ReadSubMsf = 0;
						ScsiClass::packet.ReadSubFormat = 3;//read isrc info
						maxwait = 50;
						ScsiClass::WaitUntillReady(&maxwait);
					}//close 3
				}//close 2

					//get details of track start/end/lenght in msf and lba format for all valid tracks
				if ((ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Last_Track != 0) && (ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Last_Track <= MAX_TRACKS))
				{
					ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLoadedFlag = false;
					for (count1 = 0; count1 < ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Last_Track + 1; count1++)//amount of tracks
					{
						//getting start msf from traclinfo and copying ito TrackInfo also finding start lba and copying into TrackInfo
						TempMsf1 = ScsiClass::ConvertBytesToMsf(&ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Tracks[count1].Msf_Unsed);
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[count1] = TempMsf1;
						TempLba1 = ScsiClass::ConvertMsfLba(TempMsf1);
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartLba[count1] = TempLba1;
						//************************ End Of TrackStart MSF/LBA
						//Getting TrackLenght MSF/LBA And Setting TrackInfo *****************************************************
						TempMsf2 = ScsiClass::ConvertBytesToMsf(&ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Tracks[count1 + 1].Msf_Unsed);
						//ScsiClass::SubMsfMsf(TempMsf2,MsfTwoSecond,&TempMsf2);
						if (!ScsiClass::SubMsfMsf(TempMsf2, TempMsf1, &TempMsf3))
							break;
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLenghtMsf[count1] = TempMsf3;
						TempLba1 = ScsiClass::ConvertMsfLba(TempMsf3);
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLenghtLba[count1] = TempLba1;
						//************************************************
						//Getting TrackEnd
						TempMsf1 = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[count1];
						TempMsf2 = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLenghtMsf[count1];
						if (!ScsiClass::AddMsfMsf(TempMsf2, TempMsf1, &TempMsf3))
							break;
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackEndMsf[count1] = TempMsf3;
						TempLba1 = ScsiClass::ConvertMsfLba(TempMsf3);
						ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackEndLba[count1] = TempLba1;
					}
					ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLoadedFlag = true;
				}


				ScsiClass::MediaInfo.CdMediaClass.AudioDetails.CdTextFlag = FALSE;//no cd Text needs more work on pack indicators before using

				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "There Is A CD Audio Rom In The Drive");
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaData, "The Media Is Audio Rom");
				ScsiClass::MediaInfo.CdFlag = true;
				ScsiClass::MediaInfo.CdMediaClass.AudioFlag = true;
				ScsiClass::MediaInfo.CdMediaClass.DataFlag = false;
				ScsiClass::MediaInfo.Current.CurrentDataType = AUDIO_DATA;
				ScsiClass::MediaInfo.Current.CurrentMediaType = CD;
				ScsiClass::LastMedia = CD;
				ScsiClass::MediaInfo.CdMediaClass.CdSectorSize = 2352;
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 2352;
				retcode = ScsiClass::ReadCapacity(ScsiClass::packet);

				if (retcode == DEVICEIOCONTROL_OK)
				{
					ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors = ScsiClass::BswapDword(Capacity);
					ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors;
					ScsiClass::MediaInfo.CdMediaClass.CdMaxBytes = ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors * 2352;
					ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = ScsiClass::MediaInfo.CdMediaClass.CdMaxBytes;
				}
				return(CD_MEDIA_IN_DRIVE_AUDIO_ROM);
			}


			if (!Audio)//check for Data Media read 32 sectors of data sector size 2048
			{//open 1 not audio
				ScsiClass::packet.Size_Sectors = 2048;
				ScsiClass::packet.Amount_Sectors = 32;
				ScsiClass::packet.Readcd_Expected = 8;//Mode1 yellow book only data 0f 2048 bytes
				ScsiClass::packet.Readcd_Flags = 0X10;//get user data data only
				ScsiClass::packet.Readcd_Sub = 0;//no sub channel info
				ScsiClass::packet.ReadCDLba = 0X10;

				retcode = ScsiClass::ReadCDPart(ScsiClass::packet);
				if (retcode == DEVICEIOCONTROL_OK)//if ok then media must be a data disc
				{//open 2 not audio
					Data = TRUE;

					if ((Data && disc_erase) && (disc_status == 2) && (disc_type == 0))
					{//open 3 not audio
						strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "There Is A CD RW (Data) In The Drive");
						strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaData, "Current Data Mode1 (RW)");
						ScsiClass::MediaInfo.CdFlag = true;
						ScsiClass::MediaInfo.CdMediaClass.DataFlag = true;
						ScsiClass::MediaInfo.CdMediaClass.AudioFlag = false;
						ScsiClass::MediaInfo.CdMediaClass.CdSectorSize = 2048;
						ScsiClass::MediaInfo.Current.CurrentSectorSize = 2048;
						ScsiClass::MediaInfo.Current.CurrentMediaType = CD_RW;
						ScsiClass::LastMedia = CD_RW;
						ScsiClass::MediaInfo.Current.CurrentDataType = MODE_1;
						retcode = ScsiClass::ReadCapacity(ScsiClass::packet);
						if (retcode == DEVICEIOCONTROL_OK)
						{//open 4 not audio
							ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors = ScsiClass::BswapDword(Capacity);
							ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors;
							ScsiClass::MediaInfo.CdMediaClass.CdMaxBytes = ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors * 2048;
							ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = ScsiClass::MediaInfo.CdMediaClass.CdMaxBytes;
						}//close 4 not audio
						return(CD_MEDIA_IN_DRIVE_DATA_RW);
					}//close 3 not audio
					else if ((Data && !disc_erase) && (disc_status == 2) && (disc_type == 0))
					{//open 3 not audio
						strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "There Is A CD Data Rom In The Drive");
						strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaData, "The Media Data Is Rom mode1");
						ScsiClass::MediaInfo.CdFlag = true;
						ScsiClass::MediaInfo.CdMediaClass.DataFlag = true;
						ScsiClass::MediaInfo.CdMediaClass.AudioFlag = false;
						ScsiClass::MediaInfo.CdMediaClass.CdSectorSize = 2048;
						ScsiClass::MediaInfo.Current.CurrentSectorSize = 2048;
						ScsiClass::MediaInfo.Current.CurrentMediaType = CD;
						ScsiClass::LastMedia = CD;
						ScsiClass::MediaInfo.Current.CurrentDataType = MODE_1;
						retcode = ScsiClass::ReadCapacity(ScsiClass::packet);
						if (retcode == DEVICEIOCONTROL_OK)
						{//open 4 not audio
							ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors = ScsiClass::BswapDword(Capacity);
							ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors;
							ScsiClass::MediaInfo.CdMediaClass.CdMaxBytes = ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors * 2048;
							ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = ScsiClass::MediaInfo.CdMediaClass.CdMaxBytes;
						}//close 4 not audio
						return(CD_MEDIA_IN_DRIVE_DATA_ROM);
					}//close 3 not audio	
				}//close 2 not audio

			}//close 1 not audio
		}


		retcode = ScsiClass::ReadDiscInfo(ScsiClass::packet);
		if (retcode == DEVICEIOCONTROL_OK)
		{//open 1a
			if ((ScsiClass::spwb.Data_Returned[2] & 3) == 0)//checking for empty disc
			{//open 2a
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "The Media Is CD-RW");
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaData, "The Media Data Is Blank");
				ScsiClass::MediaInfo.CdFlag = true;
				ScsiClass::MediaInfo.Current.CurrentMediaType = CD_RW;
				ScsiClass::LastMedia = CD_RW;
				return(MEDIA_IN_DRIVE_BLANK_CD);
			}//close 2a
			if ((ScsiClass::spwb.Data_Returned[2] & 3) == 1)//checking for Incomplete disc
			{//open 2a
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "The Media Is Possible Blank Or Incomplete CD");
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaData, "The Data Is Is Undefined");
				ScsiClass::MediaInfo.CdFlag = true;
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 0;
				ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentDataType = UN_KNOWN;
				ScsiClass::MediaInfo.Current.CurrentMediaType = CD;
				ScsiClass::LastMedia = CD;
				return(MEDIA_IN_DRIVE_BLANK_CD);
			}//close 2a
			if ((ScsiClass::spwb.Data_Returned[2] & 3) == 2)//checking for a complete disc (not appendable)
			{//open 2a
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "The Media is A CD Data Rom");
				strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaData, "The Data Is Mode 1");
				ScsiClass::MediaInfo.CdFlag = true;
				ScsiClass::MediaInfo.CdMediaClass.DataFlag = true;
				ScsiClass::MediaInfo.CdMediaClass.AudioFlag = false;
				ScsiClass::MediaInfo.CdMediaClass.CdSectorSize = 2048;
				ScsiClass::MediaInfo.Current.CurrentSectorSize = 2048;
				retcode = ScsiClass::ReadCapacity(ScsiClass::packet);
				if (retcode == DEVICEIOCONTROL_OK)
				{//open 3a
					ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors = ScsiClass::BswapDword(Capacity);
					ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart = ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors;
					ScsiClass::MediaInfo.CdMediaClass.CdMaxBytes = ScsiClass::MediaInfo.CdMediaClass.CdMaxSectors * 2048;
					ScsiClass::MediaInfo.Current.CurrentSectorSize = 2048;
				}//close 3a
				ScsiClass::MediaInfo.Current.CurrentAmountOfBytes.QuadPart = 0;
				ScsiClass::MediaInfo.Current.CurrentDataType = UN_KNOWN;
				if (ScsiClass::MediaInfo.MediaEraseableFlag)
				{
					ScsiClass::MediaInfo.Current.CurrentMediaType = CD_RW;
					ScsiClass::LastMedia = CD_RW;
					strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "The Media is A CD_RW Data Rom");
					return(CD_MEDIA_IN_DRIVE_DATA_ROM);
				}
				else
				{
					ScsiClass::MediaInfo.Current.CurrentMediaType = CD;
					ScsiClass::LastMedia = CD;
					strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "The Media is A CD Data Rom");
					return(CD_MEDIA_IN_DRIVE_DATA_ROM);
				}
			}//close 2a
		}//close 1a
		if (disc_erase)
		{
			strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "The Media Is CD-RW");
			strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaData, "The Data Is Undefined Or Blank CD-RW");
			ScsiClass::MediaInfo.CdFlag = true;
			ScsiClass::MediaInfo.Current.CurrentSectorSize = 0;
			ScsiClass::MediaInfo.Current.CurrentMediaType = CD_RW;
			ScsiClass::LastMedia = CD_RW;
			//	ScsiClass::MediaInfo.
			return(MEDIA_IN_DRIVE_BLANK_CD);
		}
		strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMedia, "The Media Is Undefined");
		strcpy_s(ScsiClass::MediaInfo.CdMediaClass.CdMediaData, "The Data Is Undefined or Unknown");
		ScsiClass::MediaInfo.CdFlag = true;
		ScsiClass::MediaInfo.Current.CurrentSectorSize = 0;
		ScsiClass::MediaInfo.Current.CurrentMediaType = CD;
		ScsiClass::LastMedia = CD;
		return(MEDIA_IN_DRIVE_UNKNOWN);
	}


	//*******************************************************************************
	UCHAR ScsiClass::WaitUntillReady(int *count)
	{
		int counter = 0;
		BYTE result = 0;
		UCHAR retcode;
		//***********************************************
		do
		{
			counter++;
			Sleep(50);
			retcode = ScsiClass::TestUnitReady(ScsiClass::packet);
			//ScsiClass::packet.ReadTrackInfoType = 1;
			//retcode = ScsiClass::ReadTrackInfo(ScsiClass::packet);
			Sleep(50);
			if (ScsiClass::spwb.Block.ScsiStatus != 0)//ScsiStatus is set to the Sense Error from unit
			{
				result = ScsiClass::spwb.Sense_Data[2];
				result = result & 0XF;
			}
			else
				result = 0;
		} while ((result == 2) && (counter <= *count));
		//**************************************************
		if (counter > *count)//counter = *count then we waited for the max time and gave up returns 0xff
		{
			*count = counter;
			return 0XFF;
		}
		else//we got ready before max so return the amount of times tried while waiting for ready
		{
			*count = counter;
			return 0;
		}
	}

	//******************  ConvertLbaMsf **********************************************************************
	MSF ScsiClass::ConvertLbaMsf(DWORD Lba)
	{
		MSF msf;
		DWORD Min, Sec, Frame;

		Min = Lba / 75 / 60;//added "(BYTE)" 5/12/15
		Sec = Lba / 75 % 60;//added "(BYTE)" 5/12/15
		Frame = Lba % 75;		//added "(BYTE)" 5/12/15

		msf.Unsed_byte = 0;
		msf.Minute = (BYTE)Min;
		msf.Second = (BYTE)Sec;
		msf.Frame = (BYTE)Frame;
		return msf;
	}
	//*************** Convert a byte pointer to a msf ********************************************************
	MSF ScsiClass::ConvertBytesToMsf(BYTE * byteptr)
	{
		MSF msf;
		msf.Unsed_byte = *byteptr;
		byteptr++;
		msf.Minute = *byteptr;
		byteptr++;
		msf.Second = *byteptr;
		byteptr++;
		msf.Frame = *byteptr;
		return msf;
	}

	//******************  End Of ConvertLbaMsf ***************************************************************
	//******************  ConvertMsfLba **********************************************************************
	DWORD ScsiClass::ConvertMsfLba(MSF msf)
	{
		DWORD temp = 0;

		temp = temp + msf.Minute * 60 * 75;
		temp = temp + msf.Second * 75;
		temp = temp + msf.Frame;
		return temp;
	}
	//******************  End Of ConvertMsfLba ***************************************************************
	//******************  AddMsfMsf     **********************************************************************
	BOOLEAN ScsiClass::AddMsfMsf(MSF one, MSF two, MSF *answer)
	{
		BYTE Frame = 0;
		BYTE Second = 0;
		BYTE Minute = 0;

		answer->Unsed_byte = 0;
		answer->Minute = 0;
		answer->Second = 0;
		answer->Frame = 0;

		if ((one.Frame > 74) || (two.Frame > 74))
			return FALSE;
		if ((one.Second > 59) || (two.Second > 59))
			return FALSE;
		if ((one.Minute > 127) || (two.Minute > 127))
			return FALSE;

		Frame = one.Frame + two.Frame;
		if (Frame > 74)
		{
			answer->Second++;
			Frame = Frame - 75;
		}
		Second = one.Second + two.Second;
		if (Second > 59)
		{
			answer->Minute++;
			Second = Second - 60;
		}
		Minute = one.Minute + two.Minute;
		answer->Minute = answer->Minute + Minute;
		answer->Second = answer->Second + Second;
		answer->Frame = answer->Frame + Frame;
		return TRUE;
	}
	//******************  End Of AddMsfMsf ***************************************************************
	//******************  SubMsfMsf **********************************************************************
	BOOLEAN ScsiClass::SubMsfMsf(MSF biggest, MSF smallest, MSF *answer)
	{
		BYTE Frame = 0;
		BYTE Second = 0;
		BYTE Minute = 0;
		DWORD Lba_one = 0;
		DWORD Lba_two = 0;

		answer->Unsed_byte = 0;
		answer->Minute = 0;
		answer->Second = 0;
		answer->Frame = 0;

		Lba_one = ConvertMsfLba(biggest);
		Lba_two = ConvertMsfLba(smallest);

		if (Lba_two > Lba_one)
			return FALSE;
		if ((biggest.Frame > 74) || (smallest.Frame > 74))
			return FALSE;

		if ((biggest.Second > 59) || (smallest.Second > 59))
			return FALSE;

		if ((biggest.Minute > 127) || (smallest.Minute > 127))
			return FALSE;

		if (smallest.Frame > biggest.Frame)
		{
			biggest.Frame = biggest.Frame + 75;
			biggest.Second = biggest.Second - 1;
			if (biggest.Second == 0XFF)
			{
				biggest.Minute = biggest.Minute - 1;
				biggest.Second = 0;
			}
			Frame = biggest.Frame - smallest.Frame;
		}
		else
			Frame = biggest.Frame - smallest.Frame;
		if (smallest.Second > biggest.Second)
		{
			biggest.Second = biggest.Second + 60;
			biggest.Minute = biggest.Minute - 1;
			if (biggest.Minute == 0XFF)
			{
				biggest.Minute = 0;
			}
			Second = biggest.Second - smallest.Second;
		}
		else
			Second = biggest.Second - smallest.Second;
		answer->Minute = biggest.Minute - smallest.Minute;
		answer->Second = Second;
		answer->Frame = Frame;
		return TRUE;
	}
	//******************  End Of SubMsfMsfMsf ***************************************************************

	//******************************** Set up Write Mode ********************************
	UCHAR ScsiClass::SetWriteMode()
	{//open code
		void *any;
		DWORD count;
		WRITE_PAGE *write_page;
		UCHAR retcode;

		ScsiClass::packet.ShowSense = TRUE;
		ScsiClass::packet.ModeSenseCode = 5;      //Write Mode Pars
		any = &ScsiClass::w_spwb.Data_Returned[0];
		write_page = (WRITE_PAGE *)any;
		retcode = ScsiClass::ModeSense(ScsiClass::packet);
		if (retcode != DEVICEIOCONTROL_OK)//if not ok return retcode
			return(retcode);
		//**************************************writes new ModeSense values to ModeSelect
		for (count = 0; count < 128; count++)
		{
			ScsiClass::w_spwb.Data_Returned[count] = ScsiClass::spwb.Data_Returned[count];
			ScsiClass::CopyModeSense[count] = ScsiClass::spwb.Data_Returned[count];
		}

		write_page->Audio_Pause_Lenght_LSB = 0;//;
		write_page->Audio_Pause_Lenght_MSB = 0;
		write_page->Page_Code = 5;
		write_page->Write_Type = ScsiClass::packet.ModeSelectWriteType;//1 = track : 2 = session/disc at once
		write_page->Session_Format = 0;
		write_page->Host_Code = 0;

		switch (ScsiClass::TypeWriteInProgress)
		{
		case WRITE_CD_DATA:
			write_page->Data_Block_Type = 8;//data 2048 mode 1
			write_page->Track_Mode = 4;//4 for cd 5 for dvd
			break;
			//****
		case WRITE_DVD_DATA:
			write_page->Data_Block_Type = 8;//data 2048 mode 1
			write_page->Track_Mode = 5;//4 for cd 5 for dvd
			break;
			//****
		case WRITE_DVD_VIDEO:
			write_page->Data_Block_Type = 8;//data 2048 mode 1
			write_page->Track_Mode = 5;//4 for cd 5 for dvd
			break;
			//*****
		case WRITE_CD_AUDIO:
			if (ScsiClass::packet.ModeSelectWriteType == DISC_AT_ONCE)
				write_page->Data_Block_Type = 0;//Raw data 2352 
			else if (ScsiClass::packet.ModeSelectWriteType == RAW_DATA)
				write_page->Data_Block_Type = 1;//Raw data 2368 
			else if (ScsiClass::packet.ModeSelectWriteType == TRACK_AT_ONCE)
				write_page->Data_Block_Type = 0;//Raw data 2352 
			write_page->Track_Mode = 0;//4 for cd 5 for dvd
			break;
			//*****
		default://default settings
			write_page->Data_Block_Type = 8;//data 2048 mode 1
			write_page->Track_Mode = 4;//4 for cd 5 for dvd
			break;
		}

		if (ScsiClass::TestBurn)
			write_page->Test_Write = TRUE;//testing on for testing only
		else
			write_page->Test_Write = FALSE;//testing off for Burning
		write_page->BUFE = TRUE;
		write_page->LS_V = FALSE;
		write_page->Link_Size = 0;
		write_page->Packet_Size_MSB = 0;
		write_page->Packet_Size_1 = 0;
		write_page->Packet_Size_2 = 0;
		write_page->Packet_Size_LSB = 0;
		write_page->FP = 0;
		write_page->Copy = 0;
		write_page->Multi_Session = 0;
		write_page->Page_Lenght = 0X32;
		for (count = 0; count < 16; count++)
			write_page->Media_Cat[count] = 0;

		for (count = 0; count < 16; count++)
			write_page->ISRC[count] = 0;

		ScsiClass::packet.ModeSelectLSB = write_page->Header.DataLenLsb + 2;
		if (ScsiClass::packet.ModeSelectLSB == 0)
			write_page->Header.DataLenMsb++;
		ScsiClass::packet.ModeSelectMSB = write_page->Header.DataLenMsb;

		retcode = ScsiClass::ModeSelect(ScsiClass::packet);
		return(retcode);
	}
	//****************************************************************

	UCHAR ScsiClass::ReSetWriteMode()
	{
		void *any;
		DWORD count;
		WRITE_PAGE *write_page;
		UCHAR retcode;

		any = &ScsiClass::w_spwb.Data_Returned[0];
		write_page = (WRITE_PAGE *)any;

		for (count = 0; count < sizeof(ScsiClass::CopyModeSense); count++)
		{
			ScsiClass::w_spwb.Data_Returned[count] = ScsiClass::CopyModeSense[count];
		}
		ScsiClass::packet.ModeSelectLSB = write_page->Header.DataLenLsb + 2;
		if (ScsiClass::packet.ModeSelectLSB == 0)
			write_page->Header.DataLenMsb++;
		ScsiClass::packet.ModeSelectMSB = write_page->Header.DataLenMsb;
		write_page->Test_Write = FALSE;//testing off for Burning
		retcode = ScsiClass::ModeSelect(ScsiClass::packet);
		return(retcode);
	}

	//******************  BuildCueSheet ********************************************************
	UCHAR ScsiClass::BuildCueSheet()
	{
		WORD count_1;

		int data_count = 0;
		BYTE index_1 = 0;
		BYTE data_form;
		BYTE Ctl_Adr;
		BYTE Isrc_Index;
		CUE_SHEET cue_sheet;
		DWORD AmountSectors;
		MSF TempMsf;
		data_form = 0;

		if (ScsiClass::MediaInfo.Current.CurrentSectorSize == 2352)
			Ctl_Adr = 1;
		else
			Ctl_Adr = 0x41;

		if (Ctl_Adr == 1)//Audio = 1
		{//************ for audio 
			//******************* Build catalog if Valid *********************************
			if (ScsiClass::CatalogEntryAuto)
			{
				if (ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Mc_Valid)
				{
					cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 2;//catalog
					cue_sheet.Track_Cue_Info[index_1].Track_Number = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[0];
					cue_sheet.Track_Cue_Info[index_1].Index_Number = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[1];
					cue_sheet.Track_Cue_Info[index_1].Data_Form = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[2];
					cue_sheet.Track_Cue_Info[index_1].Scms_Byte = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[3];
					cue_sheet.Track_Cue_Info[index_1].Abs_Minute = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[4];
					cue_sheet.Track_Cue_Info[index_1].Abs_Second = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[5];
					cue_sheet.Track_Cue_Info[index_1].Abs_Frame = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[6];
					index_1++;

					cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 2;//catalog
					cue_sheet.Track_Cue_Info[index_1].Track_Number = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[7];
					cue_sheet.Track_Cue_Info[index_1].Index_Number = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[8];
					cue_sheet.Track_Cue_Info[index_1].Data_Form = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[9];
					cue_sheet.Track_Cue_Info[index_1].Scms_Byte = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[10];
					cue_sheet.Track_Cue_Info[index_1].Abs_Minute = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[11];
					cue_sheet.Track_Cue_Info[index_1].Abs_Second = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioCatalog.Catalog.Cat_Asc[12];
					cue_sheet.Track_Cue_Info[index_1].Abs_Frame = 0;
					index_1++;
					data_count += 16;
				}
			}

			else
			{
				cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 2;//catalog
				cue_sheet.Track_Cue_Info[index_1].Track_Number = ScsiClass::AlternateCatalog[0];
				cue_sheet.Track_Cue_Info[index_1].Index_Number = ScsiClass::AlternateCatalog[1];
				cue_sheet.Track_Cue_Info[index_1].Data_Form = ScsiClass::AlternateCatalog[2];
				cue_sheet.Track_Cue_Info[index_1].Scms_Byte = ScsiClass::AlternateCatalog[3];
				cue_sheet.Track_Cue_Info[index_1].Abs_Minute = ScsiClass::AlternateCatalog[4];
				cue_sheet.Track_Cue_Info[index_1].Abs_Second = ScsiClass::AlternateCatalog[5];
				cue_sheet.Track_Cue_Info[index_1].Abs_Frame = ScsiClass::AlternateCatalog[6];
				index_1++;

				cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 2;//catalog
				cue_sheet.Track_Cue_Info[index_1].Track_Number = ScsiClass::AlternateCatalog[7];
				cue_sheet.Track_Cue_Info[index_1].Index_Number = ScsiClass::AlternateCatalog[8];
				cue_sheet.Track_Cue_Info[index_1].Data_Form = ScsiClass::AlternateCatalog[9];
				cue_sheet.Track_Cue_Info[index_1].Scms_Byte = ScsiClass::AlternateCatalog[10];
				cue_sheet.Track_Cue_Info[index_1].Abs_Minute = ScsiClass::AlternateCatalog[11];
				cue_sheet.Track_Cue_Info[index_1].Abs_Second = ScsiClass::AlternateCatalog[12];
				cue_sheet.Track_Cue_Info[index_1].Abs_Frame = 0;
				index_1++;
				data_count += 16;
			}

			//*****************************************************************************


			cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 1;//Lead in
			cue_sheet.Track_Cue_Info[index_1].Track_Number = 0;
			cue_sheet.Track_Cue_Info[index_1].Index_Number = 0;
			cue_sheet.Track_Cue_Info[index_1].Data_Form = 1;
			cue_sheet.Track_Cue_Info[index_1].Scms_Byte = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Minute = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Second = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Frame = 0;//Lead in
			index_1++;
			data_count += 8;

			cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 1;//Track 1 Start
			cue_sheet.Track_Cue_Info[index_1].Track_Number = 1;
			cue_sheet.Track_Cue_Info[index_1].Index_Number = 0;
			cue_sheet.Track_Cue_Info[index_1].Data_Form = 1;
			cue_sheet.Track_Cue_Info[index_1].Scms_Byte = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Minute = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Second = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Frame = 0;
			index_1++;
			data_count += 8;

			Isrc_Index = 0;

			for (count_1 = 1; count_1 <= ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Last_Track; count_1++)
			{
				if (ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Tc_Valid)
				{
					cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 3;//Isrc
					cue_sheet.Track_Cue_Info[index_1].Track_Number = 3;
					cue_sheet.Track_Cue_Info[index_1].Index_Number = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Country_Code[0];
					cue_sheet.Track_Cue_Info[index_1].Data_Form = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Country_Code[1];
					cue_sheet.Track_Cue_Info[index_1].Scms_Byte = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Owner_Code[0];
					cue_sheet.Track_Cue_Info[index_1].Abs_Minute = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Owner_Code[1];
					cue_sheet.Track_Cue_Info[index_1].Abs_Second = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Owner_Code[2];
					cue_sheet.Track_Cue_Info[index_1].Abs_Frame = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Year_Code[0];
					index_1++;

					cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 3;//Isrc
					cue_sheet.Track_Cue_Info[index_1].Track_Number = 3;
					cue_sheet.Track_Cue_Info[index_1].Index_Number = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Year_Code[1];
					cue_sheet.Track_Cue_Info[index_1].Data_Form = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Serial_Number[0];
					cue_sheet.Track_Cue_Info[index_1].Scms_Byte = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Serial_Number[1];
					cue_sheet.Track_Cue_Info[index_1].Abs_Minute = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Serial_Number[2];
					cue_sheet.Track_Cue_Info[index_1].Abs_Second = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Serial_Number[3];
					cue_sheet.Track_Cue_Info[index_1].Abs_Frame = ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioIsrc[Isrc_Index].Serial_Number[4];
					index_1++;
					data_count += 16;
					Isrc_Index++;
				}

				cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 1;
				cue_sheet.Track_Cue_Info[index_1].Track_Number = (BYTE)count_1;//was +1
				cue_sheet.Track_Cue_Info[index_1].Index_Number = 1;
				cue_sheet.Track_Cue_Info[index_1].Data_Form = 0;
				cue_sheet.Track_Cue_Info[index_1].Scms_Byte = 0;
				cue_sheet.Track_Cue_Info[index_1].Abs_Minute = ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Tracks[count_1 - 1].Msf_Minute;//was count_1
				if (count_1 == 1)
					cue_sheet.Track_Cue_Info[index_1].Abs_Second = 2;//track 1 must start at 00:02:00 so ignore seconds if on first track 19/10/15
				else
					cue_sheet.Track_Cue_Info[index_1].Abs_Second = ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Tracks[count_1 - 1].Msf_Second;
				if (count_1 == 1)
					cue_sheet.Track_Cue_Info[index_1].Abs_Frame = 0;
				else
					cue_sheet.Track_Cue_Info[index_1].Abs_Frame = ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Tracks[count_1 - 1].Msf_Frame;
				index_1++;
				data_count += 8;
			}

			cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 1;//Track 0XAA Lead out
			cue_sheet.Track_Cue_Info[index_1].Track_Number = 0XAA;
			cue_sheet.Track_Cue_Info[index_1].Index_Number = 1;
			cue_sheet.Track_Cue_Info[index_1].Data_Form = 1;
			cue_sheet.Track_Cue_Info[index_1].Scms_Byte = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Minute = ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Tracks[count_1 - 1].Msf_Minute;;
			cue_sheet.Track_Cue_Info[index_1].Abs_Second = ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Tracks[count_1 - 1].Msf_Second;
			cue_sheet.Track_Cue_Info[index_1].Abs_Frame = ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Tracks[count_1 - 1].Msf_Frame;
			index_1 = 0;
			data_count += 8;
		}

		//******************** for data 
		else
		{
			cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 1;//Lead in
			cue_sheet.Track_Cue_Info[index_1].Track_Number = 0;
			cue_sheet.Track_Cue_Info[index_1].Index_Number = 0;
			cue_sheet.Track_Cue_Info[index_1].Data_Form = 1;
			cue_sheet.Track_Cue_Info[index_1].Scms_Byte = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Minute = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Second = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Frame = 0;//Lead in
			index_1++;
			data_count += 8;

			cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 1;//Track 1 Start
			cue_sheet.Track_Cue_Info[index_1].Track_Number = 1;
			cue_sheet.Track_Cue_Info[index_1].Index_Number = 0;
			cue_sheet.Track_Cue_Info[index_1].Data_Form = 1;
			cue_sheet.Track_Cue_Info[index_1].Scms_Byte = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Minute = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Second = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Frame = 0;
			index_1++;
			data_count += 8;

			cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = Ctl_Adr;
			cue_sheet.Track_Cue_Info[index_1].Track_Number = 1;
			cue_sheet.Track_Cue_Info[index_1].Index_Number = 1;
			cue_sheet.Track_Cue_Info[index_1].Data_Form = 0;
			cue_sheet.Track_Cue_Info[index_1].Scms_Byte = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Minute = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Second = 0;
			cue_sheet.Track_Cue_Info[index_1].Abs_Frame = 0;
			index_1++;
			data_count += 8;

			cue_sheet.Track_Cue_Info[index_1].Ctl_Adr = 1;//Track 0XAA Lead out
			cue_sheet.Track_Cue_Info[index_1].Track_Number = 0XAA;
			cue_sheet.Track_Cue_Info[index_1].Index_Number = 1;
			cue_sheet.Track_Cue_Info[index_1].Data_Form = 1;
			cue_sheet.Track_Cue_Info[index_1].Scms_Byte = 0;
			AmountSectors = (DWORD)ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart;
			TempMsf = ScsiClass::ConvertLbaMsf(AmountSectors);
			cue_sheet.Track_Cue_Info[index_1].Abs_Minute = TempMsf.Minute;
			cue_sheet.Track_Cue_Info[index_1].Abs_Second = TempMsf.Second;
			cue_sheet.Track_Cue_Info[index_1].Abs_Frame = TempMsf.Frame;
			index_1 = 0;
			data_count += 8;
		}

		ScsiClass::CueSheetInfo.CueSheetSize = data_count;
		ScsiClass::CueSheetInfo.CueSheetFlag = true;
		CopyMemory(&this->ScsiClass::CueSheetInfo.CueSheet.Track_Cue_Info, &cue_sheet, ScsiClass::CueSheetInfo.CueSheetSize);

		return TRUE;

	}
	//******************  End Of BuildCusSheet ********************************************************
	//******************  SendCueSheet **********************************************************
	UCHAR ScsiClass::SendCueSheet()
	{

		void *any;
		BYTE *ptrcue;
		UCHAR retcode = 0;
		WORD count;

		any = &this->ScsiClass::CueSheetInfo.CueSheet.Track_Cue_Info;
		ptrcue = (BYTE *)any;

		if (!ScsiClass::CueSheetInfo.CueSheetFlag)
			return DEVICEIOCONTROL_FAILED;

		//************************ get Data for cue sheet from Class

		ScsiClass::packet.SendCueMsb = 0;
		ScsiClass::packet.SendCueHigh = HIBYTE(ScsiClass::CueSheetInfo.CueSheetSize);
		ScsiClass::packet.SendCueLsb = LOBYTE(ScsiClass::CueSheetInfo.CueSheetSize);
		for (count = 0; count < ScsiClass::CueSheetInfo.CueSheetSize; count++)
		{
			ScsiClass::w_spwb.Data_Returned[count] = *ptrcue;
			ptrcue++;
		}
		retcode = ScsiClass::SendCue(ScsiClass::packet);
		return retcode;
	}
	//****************** End Of SendCueSheet ****************************************************
	//****************** ShowDigits(ULARGE_INTEGER Number) ***********************************
	UCHAR ScsiClass::ShowDigits(DISPLAY *Display)
	{
		int count;
		int digitszero;

		for (count = 0; count < Display->AmountSet; count++)
		{
			Display->Digits[count] = Display->Number.QuadPart % 10;
			Display->Number.QuadPart = Display->Number.QuadPart / 10;
		}
		Display->AmountSet = count;
		digitszero = 0;
		for (count = Display->AmountSet; count > 0; count--)
		{
			if (Display->Digits[count - 1] == 0)
				digitszero++;
			else
				break;
		}

		switch (digitszero)
		{
		case 0:
			StringCchPrintf(Display->Bufferset, _countof(Display->Bufferset), L"%d,%d%d%d,%d%d%d,%d%d%d",
				Display->Digits[9], Display->Digits[8], Display->Digits[7], Display->Digits[6],
				Display->Digits[5], Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
			break;
		case 1:
			StringCchPrintf(Display->Bufferset, _countof(Display->Bufferset), L"%d%d%d,%d%d%d,%d%d%d",
				Display->Digits[8], Display->Digits[7], Display->Digits[6],
				Display->Digits[5], Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
			break;
		case 2:
			StringCchPrintf(Display->Bufferset, _countof(Display->Bufferset), L"%d%d,%d%d%d,%d%d%d",
				Display->Digits[7], Display->Digits[6], Display->Digits[5],
				Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
			break;
		case 3:
			StringCchPrintf(Display->Bufferset, _countof(Display->Bufferset), L"%d,%d%d%d,%d%d%d",
				Display->Digits[6], Display->Digits[5], Display->Digits[4],
				Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
			break;
		case 4:
			StringCchPrintf(Display->Bufferset, _countof(Display->Bufferset), L"%d%d%d,%d%d%d",
				Display->Digits[5], Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
			break;
		case 5:
			StringCchPrintf(Display->Bufferset, _countof(Display->Bufferset), L"%d%d,%d%d%d",
				Display->Digits[4], Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
			break;
		case 6:
			StringCchPrintf(Display->Bufferset, _countof(Display->Bufferset), L"%d,%d%d%d",
				Display->Digits[3], Display->Digits[2], Display->Digits[1], Display->Digits[0]);
			break;
		case 7:
			StringCchPrintf(Display->Bufferset, _countof(Display->Bufferset), L"%d%d%d", Display->Digits[2], Display->Digits[1], Display->Digits[0]);
			break;
		case 8:
			StringCchPrintf(Display->Bufferset, _countof(Display->Bufferset), L"%d%d", Display->Digits[1], Display->Digits[0]);
			break;
		case 9:
			StringCchPrintf(Display->Bufferset, _countof(Display->Bufferset), L"%d", Display->Digits[0]);
			break;
		case 10:
			StringCchPrintf(Display->Bufferset, _countof(Display->Bufferset), L"%d", 0);
			break;
		}
		wcscat_s(Display->Bufferset, _countof(Display->Bufferset), Display->formatbuffer);

		return 1;
	}


	//**************************** GetFeatures ****************
	UCHAR ScsiClass::GetFeatures(PACKET_DRIVER packet)
	{
		UCHAR *capsptr, *cdmasterptr, *tracktaoptr, *dvdptrplus, *dvdptrminus, *dvdptrplusr;
		UCHAR *mrw, *realstream;
		UCHAR RetStatus;
		void *any, *any1, *any2, *any3, *any4, *any5, *any6, *any7;
		int count;

		any = &ScsiClass::FeatureList.Capabilities;
		any1 = &ScsiClass::FeatureList.CdMastering;
		any2 = &ScsiClass::FeatureList.TrackAtOnce;
		any3 = &ScsiClass::FeatureList.DvdPlusRWDetails;
		any4 = &ScsiClass::FeatureList.DvdMinus;
		any5 = &ScsiClass::FeatureList.DvdPlusR;
		any6 = &ScsiClass::FeatureList.MountRainier;
		any7 = &ScsiClass::FeatureList.RealStreaming;

		capsptr = (UCHAR *)any;
		cdmasterptr = (UCHAR *)any1;
		tracktaoptr = (UCHAR *)any2;
		dvdptrplus = (UCHAR *)any3;
		dvdptrminus = (UCHAR *)any4;
		dvdptrplusr = (UCHAR *)any5;
		mrw = (UCHAR *)any6;
		realstream = (UCHAR *)any7;

		//******************** mode sense caps ***********
		ScsiClass::packet.ModeSenseCode = 0x2a;
		ScsiClass::ModeSense(ScsiClass::packet);
		for (count = 0; count < sizeof(CAPABILITIES); count++)
		{
			*capsptr = (UCHAR)ScsiClass::spwb.Data_Returned[count];
			capsptr++;
		}
		//**************************************

		//******			Features	//*****

		//************* Features 0x28 MRW ***************
		ScsiClass::packet.FeatureLSB = 0x28;
		ScsiClass::packet.FeatureMSB = 0;
		RetStatus = ScsiClass::GetConfiguration(ScsiClass::packet);

		for (count = 0; count < sizeof(FEATURE_MRW); count++)
		{
			*mrw = (UCHAR)ScsiClass::spwb.Data_Returned[count];
			mrw++;
		}

		ScsiClass::FeatureList.MountRainier.Header.DataLength = ScsiClass::BswapDword(&this->ScsiClass::FeatureList.MountRainier.Header.DataLength);
		ScsiClass::FeatureList.MountRainier.Header.CurrentProfile = ScsiClass::BswapWord(&this->ScsiClass::FeatureList.MountRainier.Header.CurrentProfile);
		//************** Feature 0x2E Cd Mastering ********
		ScsiClass::packet.FeatureLSB = 0x2e;
		ScsiClass::packet.FeatureMSB = 0;
		ScsiClass::GetConfiguration(ScsiClass::packet);

		for (count = 0; count < sizeof(FEATURE_CD_MASTERING); count++)
		{
			*cdmasterptr = (UCHAR)ScsiClass::spwb.Data_Returned[count];
			cdmasterptr++;
		}
		//*********** Feature 0x2D Track At Once ************
		ScsiClass::packet.FeatureLSB = 0x2d;
		ScsiClass::packet.FeatureMSB = 0;
		ScsiClass::GetConfiguration(ScsiClass::packet);

		for (count = 0; count < sizeof(FEATURE_CD_TAO); count++)
		{
			*tracktaoptr = (UCHAR)ScsiClass::spwb.Data_Returned[count];
			tracktaoptr++;
		}
		//************ Feature 0x2A DVD+RW ****************
		ScsiClass::packet.FeatureLSB = 0x2a;
		ScsiClass::packet.FeatureMSB = 0;
		ScsiClass::GetConfiguration(ScsiClass::packet);

		for (count = 0; count < sizeof(FEATURE_DVD_PLUS); count++)
		{
			*dvdptrplus = (UCHAR)ScsiClass::spwb.Data_Returned[count];
			dvdptrplus++;
		}
		//**************** Feature 0x2F DVD -R/RW *************
		ScsiClass::packet.FeatureLSB = 0x2f;
		ScsiClass::packet.FeatureMSB = 0;
		ScsiClass::GetConfiguration(ScsiClass::packet);

		for (count = 0; count < sizeof(FEATURE_DVD_MINUS); count++)
		{
			*dvdptrminus = (UCHAR)ScsiClass::spwb.Data_Returned[count];
			dvdptrminus++;
		}
		//**************** Feature 0x3B  DVD+R Dual Layer **********
		ScsiClass::packet.FeatureLSB = 0x3b;
		ScsiClass::packet.FeatureMSB = 0;
		ScsiClass::GetConfiguration(ScsiClass::packet);

		for (count = 0; count < sizeof(FEATURE_DVD_PLUS_R); count++)
		{
			*dvdptrplusr = (UCHAR)ScsiClass::spwb.Data_Returned[count];
			dvdptrplusr++;
		}
		//**************** Feature 0x107 Real Time Streaming ********
		ScsiClass::packet.FeatureLSB = 7;
		ScsiClass::packet.FeatureMSB = 1;
		ScsiClass::GetConfiguration(ScsiClass::packet);

		for (count = 0; count < sizeof(FEATURE_REAL_STREAM); count++)
		{
			*realstream = (UCHAR)ScsiClass::spwb.Data_Returned[count];
			realstream++;
		}

		return (DEVICEIOCONTROL_OK);
	}
	//***************** Delay Timer ********************
	/*
	BOOL ScsiClass::DelayNow(LARGE_INTEGER Amount)
	{

		LARGE_INTEGER TargetTime;
		BOOL Result;

		Result = QueryPerformanceFrequency(&this->ScsiClass::Frequency);
		if (!Result)
		{
		ScsiClass::HighPreTimerOk = FALSE;
		return FALSE;
		}
		TargetTime.QuadPart = Amount.QuadPart;
		ScsiClass::LastDelay.QuadPart = TargetTime.QuadPart;
		QueryPerformanceCounter(&this->ScsiClass::StartingTime);
		do
		{
			QueryPerformanceCounter(&this->ScsiClass::EndingTime);
			ScsiClass::ElapsedMicroseconds.QuadPart = ScsiClass::EndingTime.QuadPart - ScsiClass::StartingTime.QuadPart;
			if (ScsiClass::ElapsedMicroseconds.QuadPart != 0)
			{
				ScsiClass::ElapsedMicroseconds.QuadPart *= 1000000;
				ScsiClass::ElapsedMicroseconds.QuadPart /= ScsiClass::Frequency.QuadPart;
			}
		} while (ScsiClass::ElapsedMicroseconds.QuadPart < TargetTime.QuadPart);
		ScsiClass::HighPreTimerOk = TRUE;
		return TRUE;
	}
	*/
	//****************** GetTracksFromCd  *********************
	bool ScsiClass::GetTracksFromCd(PACKET_DRIVER packet)
	{
		/*
		DWORD StartLba;
		DWORD EndLba;
		DWORD TrackLenLba;
		MSF StartMsf;
		MSF EndMsf;
		MSF TrackLenMsf;
		DWORD AmountOfBytesToRead;
		DWORD AmountOfSectorsToRead;
		DWORD FullLoads;
		DWORD PartLoads;
		DWORD count1;
		DWORD BytesWrite;
		DWORD Written;
		WCHAR wBuffer[256];
		UCHAR RetCode;
		LARGE_INTEGER Filesize;

		ScsiClass::CheckMedia();
		//************** Check Audio in Drive *****************
		if (!ScsiClass::MediaInfo.CdMediaClass.AudioFlag )
			return false;
		if ((ScsiClass::MediaInfo.CdMediaClass.TrackInfo.First_Track != 1) || (ScsiClass::MediaInfo.CdMediaClass.TrackInfo.Last_Track <= 1))
			return false;
		if (ScsiClass::MediaInfo.Current.CurrentAmounfOfSectors.QuadPart <= 150)
			return false;
		//********************************************************
		StartLba		= ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartLba[5];
		StartMsf		= ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[5];
		if (StartLba == 0)
			return false;
		EndLba		= ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackEndLba[5];
		EndMsf		= ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackEndMsf[5];
		if (EndLba == 0)
			return false;
		TrackLenLba	= ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLenghtLba[5];
		TrackLenMsf	= ScsiClass::MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackLenghtMsf[5];
		if (TrackLenLba == 0)
			return false;
		StringCchPrintf(wBuffer, _countof(wBuffer), L"*****   TRACK  %d  ******", 5);
		SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

		StringCchPrintf(wBuffer, _countof(wBuffer), L"Start LBA		%d Sector",StartLba);
		SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
		StringCchPrintf(wBuffer, _countof(wBuffer), L"Start Msf		%d:%d:%d Msf", StartMsf.Minute,StartMsf.Second,StartMsf.Frame);
		SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

		StringCchPrintf(wBuffer, _countof(wBuffer), L"End LBA		%d Sector",EndLba);
		SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
		StringCchPrintf(wBuffer, _countof(wBuffer), L"End Msf		%d:%d:%d Msf", EndMsf.Minute, EndMsf.Second, EndMsf.Frame);
		SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

		StringCchPrintf(wBuffer, _countof(wBuffer), L"Lenght LBA	%d Sectors",TrackLenLba);
		SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
		StringCchPrintf(wBuffer, _countof(wBuffer), L"Lenght Msf	%d:%d:%d Msf", TrackLenMsf.Minute, TrackLenMsf.Second, TrackLenMsf.Frame);
		SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

		StringCchPrintf(wBuffer, _countof(wBuffer), L"*****   TRACK  %d  ******", 5);
		SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

		if (!ScsiClass::GetTrackFile())
			return false;
		packet.FileCount					= 0;
		packet.Readcd_Sub				= 0;
		packet.Readcd_Expected		= 4;//check this as partloads below using 4 instead of 0
		packet.Readcd_Flags				= 0x10;
		packet.Amount_Sectors			= AUDIO_BLOCK_SIZE;
		packet.ReadCDLba					= StartLba - 150;
		packet.Size_Sectors				= AUDIO_SECTOR_SIZE;

		AmountOfBytesToRead		= TrackLenLba * AUDIO_SECTOR_SIZE;
		AmountOfSectorsToRead		= TrackLenLba;
		FullLoads									= AmountOfSectorsToRead / AUDIO_BLOCK_SIZE;
		PartLoads								= AmountOfSectorsToRead % AUDIO_BLOCK_SIZE ;
		BytesWrite								= 0;

		BytesWrite = BytesWrite + (AUDIO_BLOCK_SIZE * AUDIO_SECTOR_SIZE);
		for (count1 = 0; count1 < FullLoads; count1++)
		{
			RetCode = ReadCDPart(packet);
			if (RetCode == DEVICEIOCONTROL_SENSE)
			{
				StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR		Failed ReadCD (Full)  Error  %u!!", RetCode);
				SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
				CloseHandle(hTrackFile);
				fTrackFileFlag = false;
				return false;
			}

			WriteFile(hTrackFile,spwb.Data_Returned, BytesWrite, &Written, NULL),
			packet.ReadCDLba = packet.ReadCDLba + AUDIO_BLOCK_SIZE;
		}
		//************** End of Full Loads ***************
		packet.Amount_Sectors = PartLoads;
		//********** Now start Part loads ************
		if (PartLoads != 0)
		{
			RetCode = ReadCDPart(packet);
			if (RetCode != DEVICEIOCONTROL_OK)
			{
				StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR		Failed ReadCD (Part)  Error  %u!!", RetCode);
				SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
				CloseHandle(hTrackFile);
				fTrackFileFlag = false;
				return false;
			}
			BytesWrite = packet.Amount_Sectors * packet.Size_Sectors;
			WriteFile(hTrackFile, spwb.Data_Returned, BytesWrite, &Written, NULL);
			packet.FileCount = packet.FileCount + BytesWrite;
			SetFilePointer(hTrackFile, 0, NULL, FILE_BEGIN);
			if (!GetFileSizeEx(hTrackFile, &Filesize))
				return false;

			file_header.ChunkSize			= Filesize.LowPart + 36;
			file_header.SubChunk2Size	= Filesize.LowPart;
			WriteFile(hTrackFile, &file_header, sizeof(file_header), &Written, NULL);
		}
		if (fTrackFileFlag)
		{
			CloseHandle(hTrackFile);
			fTrackFileFlag = false;
		}
		*/
		return true;
	}
	//**************** GetTrackFile ***********************************
	bool ScsiClass::GetTrackFile()
	{
		/*
		DWORD ErrorCode;
		WCHAR wBuffer[256];

		ZeroMemory(&TrackDataFile, sizeof(OPENFILENAMEW));
		TrackDataFile.lStructSize			= sizeof(OPENFILENAMEW);
		TrackDataFile.hwndOwner		= ScsiClass::packet.hDlg;
		TrackDataFile.lpstrFile				= scsi.packet.wBuffer;
		TrackDataFile.nMaxFile				= sizeof(scsi.packet.wBuffer);
		TrackDataFile.lpstrFilter			= L"All Files\0*.*\0\0";
		TrackDataFile.nFilterIndex		= 1;
		TrackDataFile.lpstrFileTitle		= NULL;
		TrackDataFile.nMaxFileTitle		= 0;
		TrackDataFile.lpstrInitialDir		= NULL;
		TrackDataFile.Flags					= OFN_CREATEPROMPT | OFN_EXPLORER;
		TrackDataFileFlag						= GetOpenFileNameW(&TrackDataFile);
		if (!TrackDataFileFlag)
			return false;

		MessageBeep(MB_OK);
		packet.AccessCreate		= GENERIC_READ | GENERIC_WRITE;
		CurrentJob						= WORKSHOP_JOB1;
		hTrackFile = CreateFile(packet.wBuffer, packet.AccessCreate, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hTrackFile == INVALID_HANDLE_VALUE)
		{
			fTrackFileFlag = false;
			ErrorCode = GetLastError();
			SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_RESETCONTENT, 0, 0);
			StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR		Failed To Create File(Data) Error Code  %u!!", ErrorCode);
			SendDlgItemMessage(packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
			CloseHandle(hTrackFile);
			fTrackFileFlag = false;
			return false;
		}
		fTrackFileFlag = true;
		*/
		return true;
	}
	//*****************************************************************
	bool ScsiClass::GetStartTime()
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&StartingTime.QuadPart);
		return true;
	}
	//*****************************************************************
	bool ScsiClass::GetEndTime()
	{
		LARGE_INTEGER  TempBaseCounter;
		LARGE_INTEGER  TempCounter;

		QueryPerformanceCounter((LARGE_INTEGER *)&EndingTime.QuadPart);
		TempBaseCounter.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
		TempCounter.QuadPart = TempBaseCounter.QuadPart;
		ElapsedNanoseconds = (double)(TempCounter.QuadPart * 1000000000) / Frequency.QuadPart;
		TempCounter.QuadPart = TempBaseCounter.QuadPart;
		ElapsedMicroseconds = (double)(TempCounter.QuadPart *= 1000000) / Frequency.QuadPart;
		TempCounter.QuadPart = TempBaseCounter.QuadPart;
		ElapsedMilliseconds = (double)(TempCounter.QuadPart *= 1000) / Frequency.QuadPart;
		TempCounter.QuadPart = TempBaseCounter.QuadPart;
		ElapsedSeconds = (double)(TempCounter.QuadPart) / Frequency.QuadPart;
		if ((TempCounter.QuadPart != 0) && (Frequency.QuadPart != 0))
			return false;

		return true;
	}
	
	//****************************** ReportMedia(ScsiClass *scsi) ************************
	bool ScsiClass::ReportMedia()
	{
		char Buffer[128];
		WCHAR wBuffer[128];
		ULARGE_INTEGER  TotalBytes;
		UINT OutPut;
		HWND hDlg;
		BYTE FirstTrack, LastTrack;
		UCHAR RetStatus;
		hDlg = packet.hDlg;
		OutPut = packet.OutPutWindow;

		SendDlgItemMessageA(hDlg, OutPut, LB_RESETCONTENT, 0, 0);
		//***************************************
		switch (CurrentDiskStatus)
		{
		case EMPTY_DISC:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"Disc Is Empty");
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			break;
			//***************
		case INCOMPLETE_DISC:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"Disc Is InComplete");
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			break;
			//***************
		case COMPLETE_DISC:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"Disc Is Complete");
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			break;
			//***************
		case FINALIZATION_SUSPENDED:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"Disc Is Neither Seq Recorded Or Restricted OverWrite");
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			break;
			//********
		default:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"Disc Is In a UnKnown State!");
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			break;
		}
		// Now Session Status *************
		switch (CurrentSessionStatus)
		{
		case EMPTY_SESSION:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"Session Is Empty!");
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			break;
			//***************
		case INCOMPLETE_SESSION:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"Session Is InComplete");
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			break;
			//***************
		case DAMAGED_BORDER:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"Session Has a Damaged Border Or/And Finalization Suspended");
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			break;
			//***************
		case COMPLETE_SESSION:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"Session Is Complete");
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			break;
			//********
		default:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"Session Is In a UnKnown State");
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			break;
		}
		//**********************************
		//DVD Found
		if (MediaInfo.DvdFlag)
		{//open dvd 1
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)MediaInfo.DvdMediaClass.DvdMedia);
			if (FeatureList.DvdPlusRWDetails.Current)
			{//open dvd 2 Note Dvd+R/RW is Current
				RetStatus = ReadFormatCapacity(packet);
				if (RetStatus != 0)
				{
					StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Failed to ReadFormatCapacity ");
					SendDlgItemMessage(packet.hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
				}
				if (spwb.Data_Returned[8] == 1)//unformatted media = 1:2 = formatted media:3 = no media present
				{//open 3
					StringCchPrintf(wTemp, _countof(wTemp), L"DVD+RW Media Is Not Formatted");
					SendDlgItemMessage(packet.hDlg, OutPut, LB_ADDSTRING,0, (LPARAM)(WCHAR *)wTemp);
				}
				if (spwb.Data_Returned[8] == 2)//unformatted media = 1:2 = formatted media:3 = no media present
				{//open 3
					StringCchPrintf(wTemp, _countof(wTemp), L"DVD+RW Media Is Formatted");
					SendDlgItemMessage(packet.hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
				}
				if (spwb.Data_Returned[8] == 3)//unformatted media = 1:2 = formatted media:3 = no media present
				{//open 3
					StringCchPrintf(wTemp, _countof(wTemp), L"DVD+RW Media Is Not Present");
					SendDlgItemMessage(packet.hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
				}
				switch (FeatureList.DvdPlusRWDetails.Write)
				{//open dvd 3
				case 1:
					StringCchPrintf(wBuffer, _countof(wBuffer), L"Media Is DVD+RW");
					SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
					//StringCchPrintf(wBuffer, _countof(wBuffer), L"Book Is set To DVD ROM");
					//SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
					break;
				case 0:
					StringCchPrintf(wBuffer, _countof(wBuffer), L"Media Is DVD+R");
					SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
					//StringCchPrintf(wBuffer, _countof(wBuffer), L"Book Is set To Is DVD ROM");
					//SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
					break;
				default:
					break;
				}//close dvd 3

			}//close dvd 2
			//if(!FeatureList.DvdPlusRWDetails.Current)
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)MediaInfo.DvdMediaClass.DvdMediaType);

			StringCchPrintf(wBuffer, _countof(wBuffer),L"Part Version Is %u", MediaInfo.DvdMediaClass.PartVersion);
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			StringCchPrintf(wBuffer, _countof(wBuffer),L"DVD Sector Size Is %u Bytes", MediaInfo.DvdMediaClass.DvdSectorSize);
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);

			if (MediaInfo.MediaEraseableFlag)
				StringCchPrintf(wBuffer,_countof(wBuffer), L"The Current Media Is Eraseable");
			else
			StringCchPrintf(wBuffer, _countof(wBuffer),L"The Current Media In Not Eraseable");
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			ZeroMemory(&this->ScsiClass::Display, sizeof(ScsiClass::Display));
			TotalBytes.QuadPart = MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart;
			Display.Number.QuadPart = TotalBytes.QuadPart;
			Display.AmountSet = 10;
			wcscpy_s(Display.formatbuffer, _countof(Display.formatbuffer), L" Bytes");
			ShowDigits(&Display);
			wcscpy_s(wBuffer, _countof(wBuffer), L"Current Size Is ");
			wcscat_s(wBuffer, _countof(wBuffer), Display.Bufferset);
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);

			if (MediaInfo.DvdMediaClass.DvdBookType == BOOK_DVD_MINUS_RW)
			{//open dvd 2
				ZeroMemory(&this->ScsiClass::Display, sizeof(ScsiClass::Display));
				TotalBytes.QuadPart = MediaInfo.DvdMediaClass.DvdMaxBytes.QuadPart;
				Display.Number = TotalBytes;
				Display.AmountSet = 10;
				wcscpy_s(Display.formatbuffer, _countof(Display.formatbuffer), L" Bytes");
				ShowDigits(&Display);
				wcscpy_s(wBuffer, _countof(wBuffer), L"The Max Formatted Size Is  ");
				wcscat_s(wBuffer, _countof(wBuffer), Display.Bufferset);
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);

				ZeroMemory(&this->ScsiClass::Display, sizeof(ScsiClass::Display));
				TotalBytes.QuadPart = MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart / 2048;
				Display.Number = TotalBytes;
				Display.AmountSet = 10;
				wcscpy_s(Display.formatbuffer, _countof(Display.formatbuffer), L" Sectors");
				ShowDigits(&Display);
				wcscpy_s(wBuffer, _countof(wBuffer), L"Amount of Sectors  Is ");
				wcscat_s(wBuffer, _countof(wBuffer), Display.Bufferset);
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);

				StringCchPrintf(wBuffer, _countof(wBuffer),L"This DVD Has Been Erased %u Times", MediaInfo.DvdMediaClass.AmountErase);
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			}//close dvd 2

			StringCchPrintf(wBuffer, _countof(wBuffer),L"Amount Of Layers Is %u", MediaInfo.DvdMediaClass.AmountOfLayers + 1);
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			if (MediaInfo.DvdMediaClass.DiscSize == 0)
			{
				StringCchPrintf(wBuffer, _countof(wBuffer),L"Disc Size Is 120mm");
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			}
			if (MediaInfo.DvdMediaClass.DiscSize == 1)
			{
				StringCchPrintf(wBuffer, _countof(wBuffer),L"Disc Size Is 80mm");
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			}
			//******* Max Rate ************
			if (MediaInfo.DvdMediaClass.MaxRate == 0)
			{
				StringCchPrintf(wBuffer, _countof(wBuffer),L"Max Rate Is 2.52 Mbps");
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			}
			if (MediaInfo.DvdMediaClass.MaxRate == 1)
			{
				StringCchPrintf(wBuffer, _countof(wBuffer), L"Max Rate Is 5.04 Mbps");
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			}
			if (MediaInfo.DvdMediaClass.MaxRate == 2)
			{
				StringCchPrintf(wBuffer, _countof(wBuffer), L"Max Rate Is 10.08 Mbps");
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			}
			if (MediaInfo.DvdMediaClass.MaxRate == 15)
			{
				StringCchPrintf(wBuffer,_countof(wBuffer), L"No Max Rate Is Specified");
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			}
			//********************************
			//***** Layer Type ***************
			if (MediaInfo.DvdMediaClass.LayerType == 0)
			{
				wsprintfA(Buffer, "Layer Contains Embossed Area");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.LayerType == 1)
			{
				wsprintfA(Buffer, "Layer Contains Recordable Area");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.LayerType == 2)
			{
				wsprintfA(Buffer, "Layer Contains Re-Writeable Area");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.LayerType == 3)
			{
				wsprintfA(Buffer, "Layer Is Reserved");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			//***********************************
			//**** Track Path *******************
			if (MediaInfo.DvdMediaClass.TrackPath == 0)
			{
				wsprintfA(Buffer, "Track Path Is Parallel");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.TrackPath == 1)
			{
				wsprintfA(Buffer, "Track Path Is Opposite");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			//***********************************
			//********* Track Density ***********
			if (MediaInfo.DvdMediaClass.TrackDensity == 0)
			{
				wsprintfA(Buffer, "Track Density Is 0.74 um/track");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.TrackDensity == 1)
			{
				wsprintfA(Buffer, "Track Density Is 0.80 um/track");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.TrackDensity == 2)
			{
				wsprintfA(Buffer, "Track Density Is 0.615 um/track");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.TrackDensity >2)
			{
				wsprintfA(Buffer, "Track Density Is Reserved");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			//***********************************
			//********* Linear Density ***********
			if (MediaInfo.DvdMediaClass.LinearDensity == 0)
			{
				wsprintfA(Buffer, "Linear Density Is 0.267 um/bit");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.LinearDensity == 1)
			{
				wsprintfA(Buffer, "Linear Density Is 0.293 um/bit");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.LinearDensity == 2)
			{
				wsprintfA(Buffer, "Linear Density Is 0.409-0.435 um/bit");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.LinearDensity == 4)
			{
				wsprintfA(Buffer, "Linear Density Is 0.280-0.291 um/bit");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.LinearDensity == 8)
			{
				wsprintfA(Buffer, "Linear Density Is 0.353 um/bit");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			if (MediaInfo.DvdMediaClass.LinearDensity > 8)
			{
				wsprintfA(Buffer, "Linear Density Is Resvered");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			//Manufacturer Details

			wsprintfA(Buffer, "Drive Manufacturer Is  %s", CdInfo.Details[0].DriveName.CompleteID);
			SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);


			wsprintfA(Buffer, "Media Manufacturer Is  %s", MediaInfo.DvdMediaClass.MediaManufacturer);
			SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			if (MediaInfo.DvdMediaClass.MediaStatus == 4)
			{
				wsprintfA(Buffer, "The Last Erase Was Minimally Blanked");
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			//Performance Details
			wsprintfA(Buffer, "The ReadSpeed Start Is %u Kb/s At Sector %u", MediaInfo.DvdMediaClass.DvdReadSpeedStart,
				MediaInfo.DvdMediaClass.DvdReadSpeedStartLba);
			SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);

			wsprintfA(Buffer, "The ReadSpeed End Is %u Kb/s At Sector %u", MediaInfo.DvdMediaClass.DvdReadSpeedEnd,
				MediaInfo.DvdMediaClass.DvdReadSpeedEndLba);
			SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);


			if ((MediaInfo.MediaEraseableFlag) || (FeatureList.DvdPlusRWDetails.Write))//only report write if disc is eraseable
			{
				wsprintfA(Buffer, "The WriteSpeed Start Is %u Kb/s At Sector %u", MediaInfo.DvdMediaClass.DvdWriteSpeedStart,
					MediaInfo.DvdMediaClass.DvdWriteSpeedStartLba);
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
				wsprintfA(Buffer, "The WriteSpeed End Is %u Kb/s At Sector %u", MediaInfo.DvdMediaClass.DvdWriteSpeedEnd,
					MediaInfo.DvdMediaClass.DvdWriteSpeedEndLba);
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
			}
			ZeroMemory(&this->ScsiClass::Display, sizeof(ScsiClass::Display));
			Display.Number.QuadPart = MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart / 2048;
			Display.AmountSet = 10;
			wcscpy_s(Display.formatbuffer, _countof(Display.formatbuffer), L" Sectors");
			ShowDigits(&Display);
			wcscpy_s(wBuffer, _countof(wBuffer), L"The Amount Sectors are  ");
			wcscat_s(wBuffer, _countof(wBuffer), Display.Bufferset);
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);

			ZeroMemory(&this->ScsiClass::Display, sizeof(ScsiClass::Display));
			TotalBytes.QuadPart = MediaInfo.DvdMediaClass.DvdCurrentBytes.QuadPart;
			Display.Number.QuadPart = TotalBytes.QuadPart;
			Display.AmountSet = 10;
			wcscpy_s(Display.formatbuffer, _countof(Display.formatbuffer), L" Bytes");
			ShowDigits(&Display);
			wcscpy_s(wBuffer, _countof(wBuffer), L"The Amount Of Bytes are  ");
			wcscat_s(wBuffer, _countof(wBuffer), Display.Bufferset);
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			return true;
		}//close dvd 1
		//*********************End DVD *******************************************

		//CD Found
		if (MediaInfo.CdMediaClass.AudioFlag)
		{//open audio 1
			StringCchPrintf(wBuffer, _countof(wBuffer), L"CD_R Read Speed Start	= %dKb/s", MediaInfo.CdMediaClass.CdReadSpeedStart);
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);

			StringCchPrintf(wBuffer, _countof(wBuffer), L"CD_R Read Speed End	= %dKb/s", MediaInfo.CdMediaClass.CdReadSpeedEnd);
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);

			StringCchPrintf(wBuffer, _countof(wBuffer), L"CD_R Write Speed Start	= %dKb/s", MediaInfo.CdMediaClass.CdWriteSpeedStart);
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);

			StringCchPrintf(wBuffer, _countof(wBuffer), L"CD_R Write Speed End	= %dKb/s", MediaInfo.CdMediaClass.CdWriteSpeedEnd);
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);

			SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)MediaInfo.CdMediaClass.CdMedia);
			SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)MediaInfo.CdMediaClass.CdMediaData);
			if (MediaInfo.CdMediaClass.AudioFlag)
			{//open audio 2
				FirstTrack = MediaInfo.CdMediaClass.TrackInfo.First_Track;
				LastTrack = MediaInfo.CdMediaClass.TrackInfo.Last_Track;

				wsprintfA(Buffer, "This Audio CD First Track Is %u", FirstTrack);
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);
				wsprintfA(Buffer, "This Audio CD Last Track Is %u", LastTrack);
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);

				wsprintfA(Buffer, "First Track Starts At   %u Miniutes : %u Seconds : %u Frames", MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[0].Minute,
					MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[0].Second, MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[0].Frame);
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);

				wsprintfA(Buffer, "Last Track Starts At   %u Miniutes : %u Seconds : %u Frames", MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[LastTrack - 1].Minute,
					MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[LastTrack - 1].Second,
					MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[LastTrack - 1].Frame);
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);

				wsprintfA(Buffer, "Leadout Starts At   %u Miniutes : %u Seconds : %u Frames", MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[LastTrack].Minute,
					MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[LastTrack].Second,
					MediaInfo.CdMediaClass.AudioDetails.AudioTrackInfo.TrackStartMsf[LastTrack].Frame);
				SendDlgItemMessageA(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)Buffer);

				Display.Number.QuadPart = MediaInfo.Current.CurrentAmountOfBytes.QuadPart;
				Display.AmountSet = 10;
				wcscpy_s(Display.formatbuffer, _countof(Display.formatbuffer), L" Bytes");
				ShowDigits(&Display);
				wcscpy_s(wBuffer, _countof(wBuffer), L"The Current Capacity Size Is  ");
				wcscat_s(wBuffer, _countof(wBuffer), Display.Bufferset);
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
			}//close audio 2
				return true;
			}//close audio 1

			if (MediaInfo.CdMediaClass.DataFlag)
			{//open cd data 1
				//********************
				StringCchPrintf(wBuffer, _countof(wBuffer),L"Current Sectors = %u", MediaInfo.CdMediaClass.CdMaxSectors);
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
				//**********************
				Display.Number.QuadPart = (DWORD)MediaInfo.CdMediaClass.CdMaxBytes;
				Display.AmountSet = 10;
				wcscpy_s(Display.formatbuffer, _countof(Display.formatbuffer), L" Bytes");
				ShowDigits(&Display);
				wcscpy_s(wBuffer, _countof(wBuffer), L"Current Size Is ");
				wcscat_s(wBuffer, _countof(wBuffer), Display.Bufferset);
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);
				//*******************
				StringCchPrintf(wBuffer, _countof(wBuffer),L"CD Sector Size Is %u Bytes", MediaInfo.CdMediaClass.CdSectorSize);
				SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)wBuffer);

				return true;
			}//close cd data 1
		//**********************End CD Data

		//Open Door Found
		if (MediaInfo.DoorOpenFlag)
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)MediaInfo.DoorOpen.DoorOpen);
		if (MediaInfo.NoMediaFlag)
			SendDlgItemMessage(hDlg, OutPut, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)MediaInfo.NoMediaClass.NoMedia);

		//End Open Door
		return false;
	}
	
	//**************************** GetFileForDvdRead(packet) *************

	bool ScsiClass::GetFileForDvdRead(PACKET_DRIVER packet,int ReportLog)
	{
		DWORD ErrorCode;
		WCHAR wTempBuffer[128];

		CheckMedia();
		ReportMedia();
		
		if (LastMedia != DVD)
		{
			StringCchPrintf(wTempBuffer, _countof(wTempBuffer), L"ERROR	No Dvd In Drive");
			SendDlgItemMessage(ScsiClass::packet.hDlg, ReportLog, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTempBuffer);
			return false;
		}
		else
		{
			StringCchPrintf(wTempBuffer, _countof(wTempBuffer), L"INFO	Dvd In Drive");
			SendDlgItemMessage(ScsiClass::packet.hDlg, ReportLog, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTempBuffer);
		}


		ZeroMemory(&ReadDataFile, sizeof(OPENFILENAMEW));
		ReadDataFile.lStructSize		= sizeof(OPENFILENAMEW);
		ReadDataFile.hwndOwner			= packet.hDlg;
		ReadDataFile.lpstrFile			= ScsiClass::packet.wBuffer;
		ReadDataFile.nMaxFile			= sizeof(ScsiClass::packet.wBuffer);
		ReadDataFile.lpstrFilter		= L"All Files\0*.*\0\0";
		ReadDataFile.nFilterIndex		= 1;
		ReadDataFile.lpstrFileTitle		= NULL;
		ReadDataFile.nMaxFileTitle		= 0;
		ReadDataFile.lpstrInitialDir	= NULL;
		ReadDataFile.Flags				= OFN_CREATEPROMPT | OFN_EXPLORER;
		ReadDataFileFlag				= GetOpenFileNameW(&ReadDataFile);
		if (!ReadDataFileFlag)
			return false;
		ScsiClass::packet.AccessCreate	= GENERIC_READ | GENERIC_WRITE;
		hDataFile										= CreateFile(ScsiClass::packet.wBuffer, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		if (hDataFile == INVALID_HANDLE_VALUE)
		{
			fDataFileFlag = false;
			ErrorCode = GetLastError();
			StringCchPrintf(wTempBuffer, _countof(wTempBuffer), L"ERROR	Failed To Create file for Writing To Error Code  %u!!", ErrorCode);
			SendDlgItemMessage(ScsiClass::packet.hDlg, ReportLog, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTempBuffer);
			return false;
		}
		fDataFileFlag = true;
		StringCchPrintf(wTempBuffer, _countof(wTempBuffer), L"INFO	Created Target Data File OK");
		SendDlgItemMessage(ScsiClass::packet.hDlg, ReportLog, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTempBuffer);
		return true;
	}
	//************************** GetFileForDvdWrite ***********************************************

	//**************************** GetFileForDvdRead(packet) *************
	bool ScsiClass::GetFileForDvdWrite(PACKET_DRIVER packet,int ReportLog)
	{
		DWORD ErrorCode;
		WCHAR wTempBuffer[128];

		CheckMedia();
		ReportMedia();
		if (LastMedia != DVD)
		{
			StringCchPrintf(wTempBuffer, _countof(wTempBuffer), L"ERROR	No Dvd In Drive");
			SendDlgItemMessage(ScsiClass::packet.hDlg, ReportLog, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTempBuffer);
			return false;
		}
		else
		{
			StringCchPrintf(wTempBuffer, _countof(wTempBuffer), L"INFO	Dvd In Drive");
			SendDlgItemMessage(ScsiClass::packet.hDlg, ReportLog, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTempBuffer);
		}
		ZeroMemory(&WriteDataFile, sizeof(OPENFILENAMEW));
		WriteDataFile.lStructSize		= sizeof(OPENFILENAMEW);
		WriteDataFile.hwndOwner			= packet.hDlg;
		WriteDataFile.lpstrFile			= ScsiClass::packet.wBuffer;
		WriteDataFile.nMaxFile			= sizeof(ScsiClass::packet.wBuffer);
		WriteDataFile.lpstrFilter		= L"All Files\0*.*\0\0";
		WriteDataFile.nFilterIndex		= 1;
		WriteDataFile.lpstrFileTitle	= NULL;
		WriteDataFile.nMaxFileTitle		= 0;
		WriteDataFile.lpstrInitialDir	= NULL;
		WriteDataFile.Flags				= OFN_CREATEPROMPT | OFN_EXPLORER;
		WriteDataFileFlag				= GetOpenFileNameW(&WriteDataFile);
		if (!WriteDataFileFlag)
			return false;
		ScsiClass::packet.AccessCreate = GENERIC_READ | GENERIC_WRITE;
		hDataFile = CreateFile(ScsiClass::packet.wBuffer, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		if (hDataFile == INVALID_HANDLE_VALUE)
		{
			fDataFileFlag = false;
			ErrorCode = GetLastError();
			StringCchPrintf(wTempBuffer, _countof(wTempBuffer), L"ERROR	Failed To Create file for reading from Error Code  %u!!", ErrorCode);
			SendDlgItemMessage(ScsiClass::packet.hDlg, ReportLog, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTempBuffer);
			return false;
		}
		fDataFileFlag = true;
		StringCchPrintf(wTempBuffer, _countof(wTempBuffer), L"INFO	Created Target Data File OK");
		SendDlgItemMessage(ScsiClass::packet.hDlg, ReportLog, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTempBuffer);
		return true;

	}
	
	//***************************************************************
//******************** CheckString ***************************************************************

BOOLEAN ScsiClass::CheckString(char * string, char * database, WORD * len)
{
	if (strcmp(string, "string_80_00_00") == 0)
	{
		strcpy_s(database, 256, string_08_00_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_00") == 0)
	{
		strcpy_s(database, 256, string_00_00_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_01") == 0)
	{
		strcpy_s(database, 256, string_00_00_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_02") == 0)
	{
		strcpy_s(database, 256, string_00_00_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_03") == 0)
	{
		strcpy_s(database, 256, string_00_00_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_04") == 0)
	{
		strcpy_s(database, 256, string_00_00_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_05") == 0)
	{
		strcpy_s(database, 256, string_00_00_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_0B_00_06") == 0)
	{
		strcpy_s(database, 256, string_0B_00_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	//*************************************************************


	if (strcmp(string, "string_00_00_11") == 0)
	{
		strcpy_s(database, 256, string_00_00_11);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_12") == 0)
	{
		strcpy_s(database, 256, string_00_00_12);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_13") == 0)
	{
		strcpy_s(database, 256, string_00_00_13);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_14") == 0)
	{
		strcpy_s(database, 256, string_00_00_14);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_15") == 0)
	{
		strcpy_s(database, 256, string_00_00_15);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_16") == 0)
	{
		strcpy_s(database, 256, string_00_00_16);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_00_17") == 0)
	{
		strcpy_s(database, 256, string_04_00_17);
		*len = (WORD)strlen(database);
		return TRUE;
	}

	//*************************************************************
	if (strcmp(string, "string_00_00_18") == 0)
	{
		strcpy_s(database, 256, string_00_00_18);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_19") == 0)
	{
		strcpy_s(database, 256, string_00_00_19);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_1A") == 0)
	{
		strcpy_s(database, 256, string_00_00_1A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_1B") == 0)
	{
		strcpy_s(database, 256, string_00_00_1B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_1C") == 0)
	{
		strcpy_s(database, 256, string_00_00_1C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_00_1D") == 0)
	{
		strcpy_s(database, 256, string_00_00_1D);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_01_00") == 0)
	{
		strcpy_s(database, 256, string_04_01_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_02_00") == 0)
	{
		strcpy_s(database, 256, string_03_02_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_03_00") == 0)
	{
		strcpy_s(database, 256, string_03_03_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_03_01") == 0)
	{
		strcpy_s(database, 256, string_03_03_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_03_03") == 0)
	{
		strcpy_s(database, 256, string_03_03_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_04_00") == 0)
	{
		strcpy_s(database, 256, string_02_04_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_04_01") == 0)
	{
		strcpy_s(database, 256, string_02_04_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_04_02") == 0)
	{
		strcpy_s(database, 256, string_02_04_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_04_03") == 0)
	{
		strcpy_s(database, 256, string_02_04_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_04_04") == 0)
	{
		strcpy_s(database, 256, string_02_04_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_04_05") == 0)
	{
		strcpy_s(database, 256, string_02_04_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_04_06") == 0)
	{
		strcpy_s(database, 256, string_02_04_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_04_07") == 0)
	{
		strcpy_s(database, 256, string_02_04_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_04_08") == 0)
	{
		strcpy_s(database, 256, string_02_04_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	//&&
	if (strcmp(string, "string_00_04_09") == 0)
	{
		strcpy_s(database, 256, string_00_04_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_04_0A") == 0)
	{
		strcpy_s(database, 256, string_00_04_0A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_04_0B") == 0)
	{
		strcpy_s(database, 256, string_00_04_0B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_04_0C") == 0)
	{
		strcpy_s(database, 256, string_00_04_0C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_04_10") == 0)
	{
		strcpy_s(database, 256, string_00_04_10);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_04_11") == 0)
	{
		strcpy_s(database, 256, string_00_04_11);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_04_12") == 0)
	{
		strcpy_s(database, 256, string_00_04_12);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_05_00") == 0)
	{
		strcpy_s(database, 256, string_04_05_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_06_00") == 0)
	{
		strcpy_s(database, 256, string_02_06_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_07_00") == 0)
	{
		strcpy_s(database, 256, string_05_07_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_08_00") == 0)
	{
		strcpy_s(database, 256, string_04_08_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_08_01") == 0)
	{
		strcpy_s(database, 256, string_04_08_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string40802") == 0)
	{
		strcpy_s(database, 256, string_04_08_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_08_03") == 0)
	{
		strcpy_s(database, 256, string_04_08_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_08_04") == 0)
	{
		strcpy_s(database, 256, string_00_08_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_09_00") == 0)
	{
		strcpy_s(database, 256, string_04_09_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_09_01") == 0)
	{
		strcpy_s(database, 256, string_04_09_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_09_02") == 0)
	{
		strcpy_s(database, 256, string_04_09_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_09_03") == 0)
	{
		strcpy_s(database, 256, string_04_09_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_09_04") == 0)
	{
		strcpy_s(database, 256, string_04_09_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_0A_00") == 0)
	{
		strcpy_s(database, 256, string_06_0A_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_0B_00") == 0)
	{
		strcpy_s(database, 256, string_01_0B_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_0B_01") == 0)
	{
		strcpy_s(database, 256, string_01_0B_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_0B_02") == 0)
	{
		strcpy_s(database, 256, string_01_0B_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0B_03") == 0)
	{
		strcpy_s(database, 256, string_00_0B_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0B_04") == 0)
	{
		strcpy_s(database, 256, string_00_0B_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0B_05") == 0)
	{
		strcpy_s(database, 256, string_00_0B_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_0C_00") == 0)
	{
		strcpy_s(database, 256, string_03_0C_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_0C_01") == 0)
	{
		strcpy_s(database, 256, string_03_0C_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_0C_02") == 0)
	{
		strcpy_s(database, 256, string_03_0C_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_0C_03") == 0)
	{
		strcpy_s(database, 256, string_03_0C_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_0C_04") == 0)
	{
		strcpy_s(database, 256, string_03_0C_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_0C_05") == 0)
	{
		strcpy_s(database, 256, string_03_0C_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_0C_06") == 0)
	{
		strcpy_s(database, 256, string_03_0C_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_0C_07") == 0)
	{
		strcpy_s(database, 256, string_03_0C_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_0C_08") == 0)
	{
		strcpy_s(database, 256, string_03_0C_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_0C_09") == 0)
	{
		strcpy_s(database, 256, string_03_0C_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_0C_0A") == 0)
	{
		strcpy_s(database, 256, string_01_0C_0A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0C_0B") == 0)
	{
		strcpy_s(database, 256, string_00_0C_0B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0C_0C") == 0)
	{
		strcpy_s(database, 256, string_00_0C_0C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0C_0D") == 0)
	{
		strcpy_s(database, 256, string_00_0C_0D);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0D_00") == 0)
	{
		strcpy_s(database, 256, string_00_0D_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0D_01") == 0)
	{
		strcpy_s(database, 256, string_00_0D_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0D_02") == 0)
	{
		strcpy_s(database, 256, string_00_0D_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0D_03") == 0)
	{
		strcpy_s(database, 256, string_00_0D_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0D_04") == 0)
	{
		strcpy_s(database, 256, string_00_0D_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0D_05") == 0)
	{
		strcpy_s(database, 256, string_00_0D_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0E_00") == 0)
	{
		strcpy_s(database, 256, string_00_0E_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0E_01") == 0)
	{
		strcpy_s(database, 256, string_00_0E_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0E_02") == 0)
	{
		strcpy_s(database, 256, string_00_0E_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0E_03") == 0)
	{
		strcpy_s(database, 256, string_00_0E_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_0F_00") == 0)
	{
		strcpy_s(database, 256, string_00_0F_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_10_00") == 0)
	{
		strcpy_s(database, 256, string_03_10_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_10_01") == 0)
	{
		strcpy_s(database, 256, string_00_10_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_10_02") == 0)
	{
		strcpy_s(database, 256, string_00_10_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_10_03") == 0)
	{
		strcpy_s(database, 256, string_00_10_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_00") == 0)
	{
		strcpy_s(database, 256, string_03_11_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_01") == 0)
	{
		strcpy_s(database, 256, string_03_11_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_02") == 0)
	{
		strcpy_s(database, 256, string_03_11_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_03") == 0)
	{
		strcpy_s(database, 256, string_03_11_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_04") == 0)
	{
		strcpy_s(database, 256, string_03_11_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_05") == 0)
	{
		strcpy_s(database, 256, string_03_11_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_06") == 0)
	{
		strcpy_s(database, 256, string_03_11_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_07") == 0)
	{
		strcpy_s(database, 256, string_03_11_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_08") == 0)
	{
		strcpy_s(database, 256, string_03_11_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_09") == 0)
	{
		strcpy_s(database, 256, string_03_11_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_0A") == 0)
	{
		strcpy_s(database, 256, string_03_11_0A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_0B") == 0)
	{
		strcpy_s(database, 256, string_03_11_0B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_0C") == 0)
	{
		strcpy_s(database, 256, string_03_11_0C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_0D") == 0)
	{
		strcpy_s(database, 256, string_03_11_0D);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_0E") == 0)
	{
		strcpy_s(database, 256, string_03_11_0E);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_0F") == 0)
	{
		strcpy_s(database, 256, string_03_11_0F);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_11_10") == 0)
	{
		strcpy_s(database, 256, string_03_11_10);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_0B_11_11") == 0)
	{
		strcpy_s(database, 256, string_0B_11_11);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_11_12") == 0)
	{
		strcpy_s(database, 256, string_00_11_12);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_11_13") == 0)
	{
		strcpy_s(database, 256, string_00_11_13);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_12_00") == 0)
	{
		strcpy_s(database, 256, string_03_12_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_13_00") == 0)
	{
		strcpy_s(database, 256, string_03_13_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_14_00") == 0)
	{
		strcpy_s(database, 256, string_00_14_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_14_01") == 0)
	{
		strcpy_s(database, 256, string_03_14_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_14_02") == 0)
	{
		strcpy_s(database, 256, string_03_14_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_14_03") == 0)
	{
		strcpy_s(database, 256, string_03_14_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_14_04") == 0)
	{
		strcpy_s(database, 256, string_03_14_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_14_05") == 0)
	{
		strcpy_s(database, 256, string_03_14_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_14_06") == 0)
	{
		strcpy_s(database, 256, string_03_14_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_14_07") == 0)
	{
		strcpy_s(database, 256, string_00_14_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_15_00") == 0)
	{
		strcpy_s(database, 256, string_03_15_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_15_01") == 0)
	{
		strcpy_s(database, 256, string_03_15_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_15_02") == 0)
	{
		strcpy_s(database, 256, string_03_15_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_16_00") == 0)
	{
		strcpy_s(database, 256, string_03_16_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_16_01") == 0)
	{
		strcpy_s(database, 256, string_03_16_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_16_02") == 0)
	{
		strcpy_s(database, 256, string_03_16_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_16_03") == 0)
	{
		strcpy_s(database, 256, string_03_16_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_16_04") == 0)
	{
		strcpy_s(database, 256, string_03_16_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_17_00") == 0)
	{
		strcpy_s(database, 256, string_01_17_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_17_01") == 0)
	{
		strcpy_s(database, 256, string_01_17_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_17_02") == 0)
	{
		strcpy_s(database, 256, string_01_17_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_17_03") == 0)
	{
		strcpy_s(database, 256, string_01_17_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_17_04") == 0)
	{
		strcpy_s(database, 256, string_01_17_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_17_05") == 0)
	{
		strcpy_s(database, 256, string_01_17_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_17_06") == 0)
	{
		strcpy_s(database, 256, string_01_17_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string11707") == 0)
	{
		strcpy_s(database, 256, string_01_17_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_17_08") == 0)
	{
		strcpy_s(database, 256, string_01_17_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_17_09") == 0)
	{
		strcpy_s(database, 256, string_01_17_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_18_00") == 0)
	{
		strcpy_s(database, 256, string_01_18_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_18_01") == 0)
	{
		strcpy_s(database, 256, string_01_18_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_18_02") == 0)
	{
		strcpy_s(database, 256, string_01_18_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_18_03") == 0)
	{
		strcpy_s(database, 256, string_01_18_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_18_04") == 0)
	{
		strcpy_s(database, 256, string_01_18_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_18_05") == 0)
	{
		strcpy_s(database, 256, string_01_18_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_18_06") == 0)
	{
		strcpy_s(database, 256, string_01_18_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_18_07") == 0)
	{
		strcpy_s(database, 256, string_01_18_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_18_08") == 0)
	{
		strcpy_s(database, 256, string_01_18_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_19_00") == 0)
	{
		strcpy_s(database, 256, string_03_19_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_19_01") == 0)
	{
		strcpy_s(database, 256, string_03_19_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_19_02") == 0)
	{
		strcpy_s(database, 256, string_03_19_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_19_03") == 0)
	{
		strcpy_s(database, 256, string_03_19_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_1A_00") == 0)
	{
		strcpy_s(database, 256, string_05_1A_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_1B_00") == 0)
	{
		strcpy_s(database, 256, string_04_1B_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_1C_00") == 0)
	{
		strcpy_s(database, 256, string_04_1C_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_1C_01") == 0)
	{
		strcpy_s(database, 256, string_04_1C_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_1C_02") == 0)
	{
		strcpy_s(database, 256, string_04_1C_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_0E_1D_00") == 0)
	{
		strcpy_s(database, 256, string_0E_1D_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_1E_00") == 0)
	{
		strcpy_s(database, 256, string_01_1E_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_1F_00") == 0)
	{
		strcpy_s(database, 256, string_03_1F_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_20_00") == 0)
	{
		strcpy_s(database, 256, string_05_20_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_20_01") == 0)
	{
		strcpy_s(database, 256, string_00_20_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_20_02") == 0)
	{
		strcpy_s(database, 256, string_00_20_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_20_03") == 0)
	{
		strcpy_s(database, 256, string_00_20_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_20_04") == 0)
	{
		strcpy_s(database, 256, string_00_20_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_20_05") == 0)
	{
		strcpy_s(database, 256, string_00_20_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_20_06") == 0)
	{
		strcpy_s(database, 256, string_00_20_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_20_07") == 0)
	{
		strcpy_s(database, 256, string_00_20_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_20_08") == 0)
	{
		strcpy_s(database, 256, string_00_20_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_20_09") == 0)
	{
		strcpy_s(database, 256, string_00_20_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_20_0A") == 0)
	{
		strcpy_s(database, 256, string_00_20_0A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_20_0B") == 0)
	{
		strcpy_s(database, 256, string_00_20_0B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_21_00") == 0)
	{
		strcpy_s(database, 256, string_05_21_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_21_01") == 0)
	{
		strcpy_s(database, 256, string_05_21_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_21_02") == 0)
	{
		strcpy_s(database, 256, string_05_21_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_21_03") == 0)
	{
		strcpy_s(database, 256, string_05_21_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_22_00") == 0)
	{
		strcpy_s(database, 256, string_05_22_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_23_00") == 0)
	{
		strcpy_s(database, 256, string_00_23_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_24_00") == 0)
	{
		strcpy_s(database, 256, string_05_24_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_24_01") == 0)
	{
		strcpy_s(database, 256, string_00_24_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_24_02") == 0)
	{
		strcpy_s(database, 256, string_00_24_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_24_03") == 0)
	{
		strcpy_s(database, 256, string_00_24_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_24_04") == 0)
	{
		strcpy_s(database, 256, string_00_24_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_24_05") == 0)
	{
		strcpy_s(database, 256, string_00_24_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_24_06") == 0)
	{
		strcpy_s(database, 256, string_00_24_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_24_07") == 0)
	{
		strcpy_s(database, 256, string_00_24_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_25_00") == 0)
	{
		strcpy_s(database, 256, string_05_25_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_26_00") == 0)
	{
		strcpy_s(database, 256, string_05_26_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_26_01") == 0)
	{
		strcpy_s(database, 256, string_05_26_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_26_02") == 0)
	{
		strcpy_s(database, 256, string_05_26_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_26_03") == 0)
	{
		strcpy_s(database, 256, string_05_26_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_26_04") == 0)
	{
		strcpy_s(database, 256, string_05_26_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_26_05") == 0)
	{
		strcpy_s(database, 256, string_00_26_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_26_06") == 0)
	{
		strcpy_s(database, 256, string_00_26_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_26_07") == 0)
	{
		strcpy_s(database, 256, string_00_26_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_26_08") == 0)
	{
		strcpy_s(database, 256, string_00_26_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_26_09") == 0)
	{
		strcpy_s(database, 256, string_00_26_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_26_0A") == 0)
	{
		strcpy_s(database, 256, string_00_26_0A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_26_0B") == 0)
	{
		strcpy_s(database, 256, string_00_26_0B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_26_0C") == 0)
	{
		strcpy_s(database, 256, string_00_26_0C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_26_0D") == 0)
	{
		strcpy_s(database, 256, string_00_26_0D);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_26_0E") == 0)
	{
		strcpy_s(database, 256, string_00_26_0E);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_26_0F") == 0)
	{
		strcpy_s(database, 256, string_00_26_0F);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_07_27_00") == 0)
	{
		strcpy_s(database, 256, string_07_27_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_07_27_01") == 0)
	{
		strcpy_s(database, 256, string_07_27_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_07_27_02") == 0)
	{
		strcpy_s(database, 256, string_07_27_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_07_27_03") == 0)
	{
		strcpy_s(database, 256, string_07_27_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_07_27_04") == 0)
	{
		strcpy_s(database, 256, string_07_27_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_07_27_05") == 0)
	{
		strcpy_s(database, 256, string_07_27_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_07_27_06") == 0)
	{
		strcpy_s(database, 256, string_07_27_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_28_00") == 0)
	{
		strcpy_s(database, 256, string_06_28_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_28_01") == 0)
	{
		strcpy_s(database, 256, string_06_28_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_28_02") == 0)
	{
		strcpy_s(database, 256, string_06_28_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_29_00") == 0)
	{
		strcpy_s(database, 256, string_06_29_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_29_01") == 0)
	{
		strcpy_s(database, 256, string_06_29_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_29_02") == 0)
	{
		strcpy_s(database, 256, string_06_29_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_29_03") == 0)
	{
		strcpy_s(database, 256, string_06_29_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string62904") == 0)
	{
		strcpy_s(database, 256, string_06_29_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_29_05") == 0)
	{
		strcpy_s(database, 256, string_00_29_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_29_06") == 0)
	{
		strcpy_s(database, 256, string_00_29_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_29_07") == 0)
	{
		strcpy_s(database, 256, string_00_29_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_2A_00") == 0)
	{
		strcpy_s(database, 256, string_06_2A_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_2A_01") == 0)
	{
		strcpy_s(database, 256, string_06_2A_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_2A_02") == 0)
	{
		strcpy_s(database, 256, string_06_2A_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_2A_03") == 0)
	{
		strcpy_s(database, 256, string_06_2A_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2A_04") == 0)
	{
		strcpy_s(database, 256, string_00_2A_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2A_05") == 0)
	{
		strcpy_s(database, 256, string_00_2A_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2A_06") == 0)
	{
		strcpy_s(database, 256, string_00_2A_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2A_07") == 0)
	{
		strcpy_s(database, 256, string_00_2A_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2A_08") == 0)
	{
		strcpy_s(database, 256, string_00_2A_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2A_09") == 0)
	{
		strcpy_s(database, 256, string_00_2A_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2A_10") == 0)
	{
		strcpy_s(database, 256, string_00_2A_10);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_2B_00") == 0)
	{
		strcpy_s(database, 256, string_05_2B_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_2C_00") == 0)
	{
		strcpy_s(database, 256, string_05_2C_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_2C_01") == 0)
	{
		strcpy_s(database, 256, string_05_2C_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_2C_02") == 0)
	{
		strcpy_s(database, 256, string_05_2C_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_2C_03") == 0)
	{
		strcpy_s(database, 256, string_05_2C_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_2C_04") == 0)
	{
		strcpy_s(database, 256, string_05_2C_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_2C_05") == 0)
	{
		strcpy_s(database, 256, string_05_2C_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2C_06") == 0)
	{
		strcpy_s(database, 256, string_00_2C_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2C_07") == 0)
	{
		strcpy_s(database, 256, string_00_2C_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2C_08") == 0)
	{
		strcpy_s(database, 256, string_00_2C_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2C_09") == 0)
	{
		strcpy_s(database, 256, string_00_2C_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2C_0A") == 0)
	{
		strcpy_s(database, 256, string_00_2C_0A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_2C_0B") == 0)
	{
		strcpy_s(database, 256, string_00_2C_0B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_2D_00") == 0)
	{
		strcpy_s(database, 256, string_03_2D_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_2E_00") == 0)
	{
		strcpy_s(database, 256, string_06_2E_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_2F_00") == 0)
	{
		strcpy_s(database, 256, string_06_2F_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_30_00") == 0)
	{
		strcpy_s(database, 256, string_02_30_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_30_00") == 0)
	{
		strcpy_s(database, 256, string_05_30_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_30_01") == 0)
	{
		strcpy_s(database, 256, string_02_30_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_30_02") == 0)
	{
		strcpy_s(database, 256, string_02_30_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_30_02") == 0)
	{
		strcpy_s(database, 256, string_05_30_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_30_03") == 0)
	{
		strcpy_s(database, 256, string_02_30_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_30_04") == 0)
	{
		strcpy_s(database, 256, string_05_30_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_30_05") == 0)
	{
		strcpy_s(database, 256, string_05_30_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_30_06") == 0)
	{
		strcpy_s(database, 256, string_05_30_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string23007") == 0)
	{
		strcpy_s(database, 256, string_02_30_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_30_08") == 0)
	{
		strcpy_s(database, 256, string_05_30_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_30_09") == 0)
	{
		strcpy_s(database, 256, string_05_30_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_30_0A") == 0)
	{
		strcpy_s(database, 256, string_00_30_0A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_08_30_0C") == 0)
	{
		strcpy_s(database, 256, string_08_30_0C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_30_10") == 0)
	{
		strcpy_s(database, 256, string_00_30_10);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_30_1B") == 0)
	{
		strcpy_s(database, 256, string_02_30_1B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_31_00") == 0)
	{
		strcpy_s(database, 256, string_03_31_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_31_00") == 0)
	{
		strcpy_s(database, 256, string_05_31_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_31_01") == 0)
	{
		strcpy_s(database, 256, string_03_31_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_31_02") == 0)
	{
		strcpy_s(database, 256, string_03_31_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_31_08") == 0)
	{
		strcpy_s(database, 256, string_05_31_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_32_00") == 0)
	{
		strcpy_s(database, 256, string_03_32_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	/*
		if(strcmp(string,"string_03_32_01") == 0)		//Error
		{
			strcpy_s(database,256,string_03_32_01);
			*len = (WORD)strlen(database);
		return TRUE;
		}
		*/
	if (strcmp(string, "string_03_33_00") == 0)
	{
		strcpy_s(database, 256, string_03_33_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_34_00") == 0)
	{
		strcpy_s(database, 256, string_04_34_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_35_00") == 0)
	{
		strcpy_s(database, 256, string_04_35_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_35_01") == 0)
	{
		strcpy_s(database, 256, string_05_35_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_35_02") == 0)
	{
		strcpy_s(database, 256, string_02_35_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_35_03") == 0)
	{
		strcpy_s(database, 256, string_04_35_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_35_04") == 0)
	{
		strcpy_s(database, 256, string_05_35_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_35_05") == 0)
	{
		strcpy_s(database, 256, string_00_35_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_36_00") == 0)
	{
		strcpy_s(database, 256, string_03_36_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_37_00") == 0)
	{
		strcpy_s(database, 256, string_01_37_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_38_00") == 0)
	{
		strcpy_s(database, 256, string_05_38_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_38_02") == 0)
	{
		strcpy_s(database, 256, string_00_38_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_38_04") == 0)
	{
		strcpy_s(database, 256, string_00_38_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_38_06") == 0)
	{
		strcpy_s(database, 256, string_00_38_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_39_00") == 0)
	{
		strcpy_s(database, 256, string_05_39_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_3A_00") == 0)
	{
		strcpy_s(database, 256, string_02_3A_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_3A_01") == 0)
	{
		strcpy_s(database, 256, string_02_3A_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_3A_02") == 0)
	{
		strcpy_s(database, 256, string_02_3A_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3A_03") == 0)
	{
		strcpy_s(database, 256, string_00_3A_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3A_04") == 0)
	{
		strcpy_s(database, 256, string_00_3A_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_3B_00") == 0)
	{
		strcpy_s(database, 256, string_03_3B_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_3B_01") == 0)
	{
		strcpy_s(database, 256, string_03_3B_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_3B_02") == 0)
	{
		strcpy_s(database, 256, string_03_3B_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_3B_03") == 0)
	{
		strcpy_s(database, 256, string_03_3B_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_3B_04") == 0)
	{
		strcpy_s(database, 256, string_04_3B_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_3B_05") == 0)
	{
		strcpy_s(database, 256, string_04_3B_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_3B_06") == 0)
	{
		strcpy_s(database, 256, string_03_3B_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_3B_07") == 0)
	{
		strcpy_s(database, 256, string_03_3B_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_3B_08") == 0)
	{
		strcpy_s(database, 256, string_03_3B_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_3B_09") == 0)
	{
		strcpy_s(database, 256, string_03_3B_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_3B_0A") == 0)
	{
		strcpy_s(database, 256, string_03_3B_0A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_3B_0B") == 0)
	{
		strcpy_s(database, 256, string_03_3B_0B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_3B_0C") == 0)
	{
		strcpy_s(database, 256, string_03_3B_0C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_3B_0D") == 0)
	{
		strcpy_s(database, 256, string_05_3B_0D);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_3B_0E") == 0)
	{
		strcpy_s(database, 256, string_05_3B_0E);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_3B_0F") == 0)
	{
		strcpy_s(database, 256, string_06_3B_0F);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_3B_11") == 0)
	{
		strcpy_s(database, 256, string_02_3B_11);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_3B_12") == 0)
	{
		strcpy_s(database, 256, string_06_3B_12);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_3B_13") == 0)
	{
		strcpy_s(database, 256, string_06_3B_13);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_3B_14") == 0)
	{
		strcpy_s(database, 256, string_06_3B_14);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_3B_15") == 0)
	{
		strcpy_s(database, 256, string_06_3B_15);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_3B_16") == 0)
	{
		strcpy_s(database, 256, string_04_3B_16);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3B_17") == 0)
	{
		strcpy_s(database, 256, string_00_3B_17);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3C_00") == 0)
	{
		strcpy_s(database, 256, string_00_3C_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_3D_00") == 0)
	{
		strcpy_s(database, 256, string_05_3D_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_3E_00") == 0)
	{
		strcpy_s(database, 256, string_02_3E_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_3E_01") == 0)
	{
		strcpy_s(database, 256, string_04_3E_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_3E_02") == 0)
	{
		strcpy_s(database, 256, string_04_3E_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3E_03") == 0)
	{
		strcpy_s(database, 256, string_00_3E_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3E_04") == 0)
	{
		strcpy_s(database, 256, string_00_3E_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_3F_00") == 0)
	{
		strcpy_s(database, 256, string_06_3F_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_3F_01") == 0)
	{
		strcpy_s(database, 256, string_06_3F_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_3F_02") == 0)
	{
		strcpy_s(database, 256, string_06_3F_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_3F_03") == 0)
	{
		strcpy_s(database, 256, string_06_3F_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_04") == 0)
	{
		strcpy_s(database, 256, string_00_3F_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_05") == 0)
	{
		strcpy_s(database, 256, string_00_3F_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_06") == 0)
	{
		strcpy_s(database, 256, string_00_3F_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_07") == 0)
	{
		strcpy_s(database, 256, string_00_3F_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_08") == 0)
	{
		strcpy_s(database, 256, string_00_3F_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_09") == 0)
	{
		strcpy_s(database, 256, string_00_3F_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_0A") == 0)
	{
		strcpy_s(database, 256, string_00_3F_0A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_0B") == 0)
	{
		strcpy_s(database, 256, string_00_3F_0B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_0C") == 0)
	{
		strcpy_s(database, 256, string_00_3F_0C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_0D") == 0)
	{
		strcpy_s(database, 256, string_00_3F_0D);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_0E") == 0)
	{
		strcpy_s(database, 256, string_00_3F_0E);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_0F") == 0)
	{
		strcpy_s(database, 256, string_00_3F_0F);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_10") == 0)
	{
		strcpy_s(database, 256, string_00_3F_10);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_3F_11") == 0)
	{
		strcpy_s(database, 256, string_00_3F_11);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_40_00") == 0)
	{
		strcpy_s(database, 256, string_04_40_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_40_80") == 0)
	{
		strcpy_s(database, 256, string_04_40_80);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_41_00") == 0)
	{
		strcpy_s(database, 256, string_04_41_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string04_42-00") == 0)
	{
		strcpy_s(database, 256, string_04_42_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_43_00") == 0)
	{
		strcpy_s(database, 256, string_05_43_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_44_00") == 0)
	{
		strcpy_s(database, 256, string_04_44_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_44_71") == 0)
	{
		strcpy_s(database, 256, string_00_44_71);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_0B_45_00") == 0)
	{
		strcpy_s(database, 256, string_0B_45_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_46_00") == 0)
	{
		strcpy_s(database, 256, string_04_46_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_47_00") == 0)
	{
		strcpy_s(database, 256, string_04_47_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_47_01") == 0)
	{
		strcpy_s(database, 256, string_00_47_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_47_02") == 0)
	{
		strcpy_s(database, 256, string_00_47_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_47_03") == 0)
	{
		strcpy_s(database, 256, string_00_47_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_47_04") == 0)
	{
		strcpy_s(database, 256, string_00_47_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_47_05") == 0)
	{
		strcpy_s(database, 256, string_00_47_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_47_06") == 0)
	{
		strcpy_s(database, 256, string_00_47_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_47_7F") == 0)
	{
		strcpy_s(database, 256, string_00_47_7F);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_0B_48_00") == 0)
	{
		strcpy_s(database, 256, string_0B_48_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_0B_49_00") == 0)
	{
		strcpy_s(database, 256, string_0B_49_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_4A_00") == 0)
	{
		strcpy_s(database, 256, string_04_4A_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_4B_00") == 0)
	{
		strcpy_s(database, 256, string_04_4B_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_4B_01") == 0)
	{
		strcpy_s(database, 256, string_00_4B_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_4B_02") == 0)
	{
		strcpy_s(database, 256, string_00_4B_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_4B_03") == 0)
	{
		strcpy_s(database, 256, string_00_4B_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_4B_04") == 0)
	{
		strcpy_s(database, 256, string_00_4B_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_4B_05") == 0)
	{
		strcpy_s(database, 256, string_00_4B_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_4B_06") == 0)
	{
		strcpy_s(database, 256, string_00_4B_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_4C_00") == 0)
	{
		strcpy_s(database, 256, string_04_4C_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_0B_4D_00") == 0)
	{
		strcpy_s(database, 256, string_0B_4D_80);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_0B_4E_00") == 0)
	{
		strcpy_s(database, 256, string_0B_4E_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_4F_00") == 0)
	{
		strcpy_s(database, 256, string_00_4F_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_50_00") == 0)
	{
		strcpy_s(database, 256, string_00_50_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_50_01") == 0)
	{
		strcpy_s(database, 256, string_00_50_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_50_02") == 0)
	{
		strcpy_s(database, 256, string_00_50_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_51_00") == 0)
	{
		strcpy_s(database, 256, string_03_51_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_51_01") == 0)
	{
		strcpy_s(database, 256, string_03_51_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_52_00") == 0)
	{
		strcpy_s(database, 256, string_03_52_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_53_00") == 0)
	{
		strcpy_s(database, 256, string_04_53_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_53_01") == 0)
	{
		strcpy_s(database, 256, string_00_53_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_53_02") == 0)
	{
		strcpy_s(database, 256, string_02_53_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_53_02") == 0)
	{
		strcpy_s(database, 256, string_05_53_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_54_00") == 0)
	{
		strcpy_s(database, 256, string_00_54_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_55_00") == 0)
	{
		strcpy_s(database, 256, string_05_55_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_55_01") == 0)
	{
		strcpy_s(database, 256, string_00_55_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_55_02") == 0)
	{
		strcpy_s(database, 256, string_00_55_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_55_03") == 0)
	{
		strcpy_s(database, 256, string_05_55_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_55_04") == 0)
	{
		strcpy_s(database, 256, string_00_55_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_55_05") == 0)
	{
		strcpy_s(database, 256, string_00_55_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_55_06") == 0)
	{
		strcpy_s(database, 256, string_00_55_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_55_07") == 0)
	{
		strcpy_s(database, 256, string_00_55_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_56_00") == 0)
	{
		strcpy_s(database, 256, string_00_56_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_57_00") == 0)
	{
		strcpy_s(database, 256, string_03_57_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_58_00") == 0)
	{
		strcpy_s(database, 256, string_00_58_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_59_00") == 0)
	{
		strcpy_s(database, 256, string_00_59_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5A_00") == 0)
	{
		strcpy_s(database, 256, string_06_5A_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5A_01") == 0)
	{
		strcpy_s(database, 256, string_06_5A_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5A_02") == 0)
	{
		strcpy_s(database, 256, string_06_5A_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5A_03") == 0)
	{
		strcpy_s(database, 256, string_06_5A_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5B_00") == 0)
	{
		strcpy_s(database, 256, string_06_5B_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5B_01") == 0)
	{
		strcpy_s(database, 256, string_06_5B_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5B_02") == 0)
	{
		strcpy_s(database, 256, string_06_5B_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5B_03") == 0)
	{
		strcpy_s(database, 256, string_06_5B_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5C_00") == 0)
	{
		strcpy_s(database, 256, string_06_5C_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5C_01") == 0)
	{
		strcpy_s(database, 256, string_06_5C_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_5C_02") == 0)
	{
		strcpy_s(database, 256, string_03_5C_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_5D_00") == 0)
	{
		strcpy_s(database, 256, string_01_5D_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_5D_01") == 0)
	{
		strcpy_s(database, 256, string_01_5D_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_02") == 0)
	{
		strcpy_s(database, 256, string_00_5D_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_5D_03") == 0)
	{
		strcpy_s(database, 256, string_01_5D_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_10") == 0)
	{
		strcpy_s(database, 256, string_00_5D_10);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_11") == 0)
	{
		strcpy_s(database, 256, string_00_5D_11);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_12") == 0)
	{
		strcpy_s(database, 256, string_00_5D_12);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_13") == 0)
	{
		strcpy_s(database, 256, string_00_5D_13);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_14") == 0)
	{
		strcpy_s(database, 256, string_00_5D_14);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_15") == 0)
	{
		strcpy_s(database, 256, string_00_5D_15);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_16") == 0)
	{
		strcpy_s(database, 256, string_00_5D_16);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_17") == 0)
	{
		strcpy_s(database, 256, string_00_5D_17);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_18") == 0)
	{
		strcpy_s(database, 256, string_00_5D_18);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_19") == 0)
	{
		strcpy_s(database, 256, string_00_5D_19);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_1A") == 0)
	{
		strcpy_s(database, 256, string_00_5D_1A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_1B") == 0)
	{
		strcpy_s(database, 256, string_00_5D_1B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_1C") == 0)
	{
		strcpy_s(database, 256, string_00_5D_1C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_20") == 0)
	{
		strcpy_s(database, 256, string_00_5D_20);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_21") == 0)
	{
		strcpy_s(database, 256, string_00_5D_21);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_22") == 0)
	{
		strcpy_s(database, 256, string_00_5D_22);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_23") == 0)
	{
		strcpy_s(database, 256, string_00_5D_23);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_24") == 0)
	{
		strcpy_s(database, 256, string_00_5D_24);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_25") == 0)
	{
		strcpy_s(database, 256, string_00_5D_25);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_26") == 0)
	{
		strcpy_s(database, 256, string_00_5D_26);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_27") == 0)
	{
		strcpy_s(database, 256, string_00_5D_27);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_28") == 0)
	{
		strcpy_s(database, 256, string_00_5D_28);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_29") == 0)
	{
		strcpy_s(database, 256, string_00_5D_29);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_2A") == 0)
	{
		strcpy_s(database, 256, string_00_5D_2A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_2B") == 0)
	{
		strcpy_s(database, 256, string_00_5D_2B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_2C") == 0)
	{
		strcpy_s(database, 256, string_00_5D_2C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_30") == 0)
	{
		strcpy_s(database, 256, string_00_5D_30);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_31") == 0)
	{
		strcpy_s(database, 256, string_00_5D_31);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_32") == 0)
	{
		strcpy_s(database, 256, string_00_5D_32);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_33") == 0)
	{
		strcpy_s(database, 256, string_00_5D_33);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_34") == 0)
	{
		strcpy_s(database, 256, string_00_5D_34);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_35") == 0)
	{
		strcpy_s(database, 256, string_00_5D_35);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_36") == 0)
	{
		strcpy_s(database, 256, string_00_5D_36);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_37") == 0)
	{
		strcpy_s(database, 256, string_00_5D_37);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_38") == 0)
	{
		strcpy_s(database, 256, string_00_5D_38);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_39") == 0)
	{
		strcpy_s(database, 256, string_00_5D_39);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_3A") == 0)
	{
		strcpy_s(database, 256, string_00_5D_3A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_3B") == 0)
	{
		strcpy_s(database, 256, string_00_5D_3B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_3C") == 0)
	{
		strcpy_s(database, 256, string_00_5D_3C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_40") == 0)
	{
		strcpy_s(database, 256, string_00_5D_40);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_41") == 0)
	{
		strcpy_s(database, 256, string_00_5D_41);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_42") == 0)
	{
		strcpy_s(database, 256, string_00_5D_42);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_43") == 0)
	{
		strcpy_s(database, 256, string_00_5D_43);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_44") == 0)
	{
		strcpy_s(database, 256, string_00_5D_44);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_45") == 0)
	{
		strcpy_s(database, 256, string_00_5D_45);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_46") == 0)
	{
		strcpy_s(database, 256, string_00_5D_46);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_47") == 0)
	{
		strcpy_s(database, 256, string_00_5D_47);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_48") == 0)
	{
		strcpy_s(database, 256, string_00_5D_48);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_49") == 0)
	{
		strcpy_s(database, 256, string_00_5D_49);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_4A") == 0)
	{
		strcpy_s(database, 256, string_00_5D_4A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_4B") == 0)
	{
		strcpy_s(database, 256, string_00_5D_4B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_4C") == 0)
	{
		strcpy_s(database, 256, string_00_5D_4C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_50") == 0)
	{
		strcpy_s(database, 256, string_00_5D_50);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_51") == 0)
	{
		strcpy_s(database, 256, string_00_5D_51);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_52") == 0)
	{
		strcpy_s(database, 256, string_00_5D_52);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_53") == 0)
	{
		strcpy_s(database, 256, string_00_5D_53);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_54") == 0)
	{
		strcpy_s(database, 256, string_00_5D_54);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_55") == 0)
	{
		strcpy_s(database, 256, string_00_5D_55);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_56") == 0)
	{
		strcpy_s(database, 256, string_00_5D_56);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_57") == 0)
	{
		strcpy_s(database, 256, string_00_5D_57);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_58") == 0)
	{
		strcpy_s(database, 256, string_00_5D_58);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_59") == 0)
	{
		strcpy_s(database, 256, string_00_5D_59);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_5A") == 0)
	{
		strcpy_s(database, 256, string_00_5D_5A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_5B") == 0)
	{
		strcpy_s(database, 256, string_00_5D_5B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_5C") == 0)
	{
		strcpy_s(database, 256, string_00_5D_5C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_60") == 0)
	{
		strcpy_s(database, 256, string_00_5D_60);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_61") == 0)
	{
		strcpy_s(database, 256, string_00_5D_61);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_62") == 0)
	{
		strcpy_s(database, 256, string_00_5D_62);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_63") == 0)
	{
		strcpy_s(database, 256, string_00_5D_63);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_64") == 0)
	{
		strcpy_s(database, 256, string_00_5D_64);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_65") == 0)
	{
		strcpy_s(database, 256, string_00_5D_65);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_66") == 0)
	{
		strcpy_s(database, 256, string_00_5D_66);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_67") == 0)
	{
		strcpy_s(database, 256, string_00_5D_67);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_68") == 0)
	{
		strcpy_s(database, 256, string_00_5D_68);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_69") == 0)
	{
		strcpy_s(database, 256, string_00_5D_69);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_6A") == 0)
	{
		strcpy_s(database, 256, string_00_5D_6A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_6B") == 0)
	{
		strcpy_s(database, 256, string_00_5D_6B);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5D_6C") == 0)
	{
		strcpy_s(database, 256, string_00_5D_6C);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_5D_FF") == 0)
	{
		strcpy_s(database, 256, string_01_5D_FF);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5E_00") == 0)
	{
		strcpy_s(database, 256, string_06_5E_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5E_01") == 0)
	{
		strcpy_s(database, 256, string_06_5E_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5E_02") == 0)
	{
		strcpy_s(database, 256, string_06_5E_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5E_03") == 0)
	{
		strcpy_s(database, 256, string_06_5E_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_5E_04") == 0)
	{
		strcpy_s(database, 256, string_06_5E_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5E_41") == 0)
	{
		strcpy_s(database, 256, string_00_5E_41);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5E_42") == 0)
	{
		strcpy_s(database, 256, string_00_5E_42);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5E_43") == 0)
	{
		strcpy_s(database, 256, string_00_5E_43);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5E_45") == 0)
	{
		strcpy_s(database, 256, string_00_5E_45);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5E_47") == 0)
	{
		strcpy_s(database, 256, string_00_5E_47);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_5F_00") == 0)
	{
		strcpy_s(database, 256, string_00_5F_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_60_00") == 0)
	{
		strcpy_s(database, 256, string_04_60_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_61_00") == 0)
	{
		strcpy_s(database, 256, string_03_61_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_61_01") == 0)
	{
		strcpy_s(database, 256, string_03_61_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_61_02") == 0)
	{
		strcpy_s(database, 256, string_03_61_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_62_00") == 0)
	{
		strcpy_s(database, 256, string_04_62_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_63_00") == 0)
	{
		strcpy_s(database, 256, string_05_63_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_63_01") == 0)
	{
		strcpy_s(database, 256, string_05_63_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_64_00") == 0)
	{
		strcpy_s(database, 256, string_05_64_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_64_01") == 0)
	{
		strcpy_s(database, 256, string_05_64_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_65_00") == 0)
	{
		strcpy_s(database, 256, string_04_65_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_66_00") == 0)
	{
		strcpy_s(database, 256, string_04_66_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_66_01") == 0)
	{
		strcpy_s(database, 256, string_04_66_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_66_02") == 0)
	{
		strcpy_s(database, 256, string_04_66_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_66_03") == 0)
	{
		strcpy_s(database, 256, string_04_66_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_67_00") == 0)
	{
		strcpy_s(database, 256, string_04_67_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_67_01") == 0)
	{
		strcpy_s(database, 256, string_04_67_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_67_02") == 0)
	{
		strcpy_s(database, 256, string_04_67_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_67_03") == 0)
	{
		strcpy_s(database, 256, string_04_67_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_67_04") == 0)
	{
		strcpy_s(database, 256, string_04_67_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_67_05") == 0)
	{
		strcpy_s(database, 256, string_04_67_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_67_06") == 0)
	{
		strcpy_s(database, 256, string_04_67_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_67_07") == 0)
	{
		strcpy_s(database, 256, string_04_67_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_67_08") == 0)
	{
		strcpy_s(database, 256, string_00_67_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_67_09") == 0)
	{
		strcpy_s(database, 256, string_00_67_09);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_00_67_0A") == 0)
	{
		strcpy_s(database, 256, string_00_67_0A);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_02_68_00") == 0)
	{
		strcpy_s(database, 256, string_02_68_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_69_00") == 0)
	{
		strcpy_s(database, 256, string_04_69_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_69_01") == 0)
	{
		strcpy_s(database, 256, string_04_69_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_69_02") == 0)
	{
		strcpy_s(database, 256, string_04_69_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_6A_00") == 0)
	{
		strcpy_s(database, 256, string_01_6A_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_6B_00") == 0)
	{
		strcpy_s(database, 256, string_06_6B_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_6B_01") == 0)
	{
		strcpy_s(database, 256, string_06_6B_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_06_6B_02") == 0)
	{
		strcpy_s(database, 256, string_06_6B_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_6C_00") == 0)
	{
		strcpy_s(database, 256, string_03_6C_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_6D_00") == 0)
	{
		strcpy_s(database, 256, string_03_6D_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_04_6E_00") == 0)
	{
		strcpy_s(database, 256, string_04_6E_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_6F_00") == 0)
	{
		strcpy_s(database, 256, string_05_6F_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_6F_01") == 0)
	{
		strcpy_s(database, 256, string_05_6F_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_6F_02") == 0)
	{
		strcpy_s(database, 256, string_05_6F_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_6F_03") == 0)
	{
		strcpy_s(database, 256, string_05_6F_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_6F_04") == 0)
	{
		strcpy_s(database, 256, string_05_6F_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_6F_05") == 0)
	{
		strcpy_s(database, 256, string_05_6F_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_6F_06") == 0)
	{
		strcpy_s(database, 256, string_05_6F_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_6F_07") == 0)
	{
		strcpy_s(database, 256, string_05_6F_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_6F_08") == 0)
	{
		strcpy_s(database, 256, string_05_6F_08);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_70_80") == 0)
	{
		strcpy_s(database, 256, string_03_70_80);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_71_00") == 0)
	{
		strcpy_s(database, 256, string_03_71_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_72_00") == 0)
	{
		strcpy_s(database, 256, string_03_72_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_72_01") == 0)
	{
		strcpy_s(database, 256, string_03_72_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_72_02") == 0)
	{
		strcpy_s(database, 256, string_03_72_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_72_03") == 0)
	{
		strcpy_s(database, 256, string_05_72_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_72_04") == 0)
	{
		strcpy_s(database, 256, string_05_72_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_72_05") == 0)
	{
		strcpy_s(database, 256, string_05_72_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_72_06") == 0)
	{
		strcpy_s(database, 256, string_05_72_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_72_07") == 0)
	{
		strcpy_s(database, 256, string_05_72_07);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_73_00") == 0)
	{
		strcpy_s(database, 256, string_03_73_00);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_73_01") == 0)
	{
		strcpy_s(database, 256, string_01_73_01);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_73_02") == 0)
	{
		strcpy_s(database, 256, string_03_73_02);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_73_03") == 0)
	{
		strcpy_s(database, 256, string_03_73_03);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_73_04") == 0)
	{
		strcpy_s(database, 256, string_03_73_04);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_03_73_05") == 0)
	{
		strcpy_s(database, 256, string_03_73_05);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_73_06") == 0)
	{
		strcpy_s(database, 256, string_01_73_06);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_73_10") == 0)
	{
		strcpy_s(database, 256, string_01_73_10);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_73_11") == 0)
	{
		strcpy_s(database, 256, string_05_73_11);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_73_15") == 0)
	{
		strcpy_s(database, 256, string_05_73_15);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_01_73_16") == 0)
	{
		strcpy_s(database, 256, string_01_73_16);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	if (strcmp(string, "string_05_73_17") == 0)
	{
		strcpy_s(database, 256, string_05_73_17);
		*len = (WORD)strlen(database);
		return TRUE;
	}
	//****************
	return FALSE;
}

//***************************************************************
