//********************** ReadDvdMedia() ************************ (1)
unsigned __stdcall ReadDvdMedia(void *par)
{
	BOOL StatusWrite;
	UCHAR RetStatus;
	WCHAR wTemp[128];
	WCHAR wTemp1[128];
	LARGE_INTEGER FullLoads, PartLoads;
	LARGE_INTEGER count;
	DWORD Written;
	DWORD ErrorCode;
	ScsiClass *scsi;
	scsi = (ScsiClass *)par;
	//***************** Testing for error conditions ***************
	if ((!scsi->ReadDataFileFlag) || (!scsi->fDataFileFlag))
	{
		StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	No File/No DVD!");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
		_endthreadex(0);
		return 0xff;
	}
	if (scsi->MediaInfo.Current.CurrentAmounfOfSectors.QuadPart == 0)
	{
		StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	No Sector Size!");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
		_endthreadex(0);
		return 0xff;
	}
	//*****************************************************************
	//*********** Set up FullLoads Main loop And PartLoads ********
	FullLoads.QuadPart = scsi->MediaInfo.Current.CurrentAmounfOfSectors.QuadPart / DATA_BLOCK_SIZE;
	PartLoads.QuadPart = scsi->MediaInfo.Current.CurrentAmounfOfSectors.QuadPart % DATA_BLOCK_SIZE;
	//*****************************************************************
	scsi->packet.Read10Lba = 0;//reset the LBA to Zero
	scsi->packet.ReadByte	= 0;//writing to buffer will be ok
	//*********************** Start The Timer For Read **************
	SetTimer(scsi->packet.hDlg, IDT_TIMER_READ, 1000, NULL);
	//****************************************************************
	//***************** Main Loop FullLoad Entered ****************
	for (count.QuadPart = 0; count.QuadPart < FullLoads.QuadPart; count.QuadPart++)
	{

		RetStatus = scsi->Read10Full(scsi->packet);
		if (RetStatus == DEVICEIOCONTROL_SENSE)
		{
			StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Failed To Read Dvd");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
			StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Sense Code ");
			MultiByteToWideChar(CP_ACP, 0, scsi->SenseError, sizeof(scsi->SenseError), wTemp1, _countof(wTemp1));
			wcscat_s(wTemp, _countof(wTemp), wTemp1);
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
			KillTimer(scsi->packet.hDlg, IDT_TIMER_READ);//Stop Timer
			_endthreadex(0);
			return 0xff;
		}
		StatusWrite = WriteFile(scsi->hDataFile, scsi->spwb.Data_Returned, (DWORD)DATA_BLOCK_SIZE * scsi->MediaInfo.Current.CurrentSectorSize, &Written, NULL);
		if (!StatusWrite)
		{
			ErrorCode = GetLastError();
			StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Failed To Write To File Code %d",ErrorCode);
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
			KillTimer(scsi->packet.hDlg, IDT_TIMER_READ);//Stop Timer
			return 0xff;
		}
		scsi->packet.Read10Lba = scsi->packet.Read10Lba + DATA_BLOCK_SIZE;
		scsi->packet.LFileCount.QuadPart = scsi->packet.LFileCount.QuadPart + (DWORD)(DATA_BLOCK_SIZE * scsi->MediaInfo.Current.CurrentSectorSize);
	}
	//****************************************************************
	//******************** If PartLoads Not Zero *******************
	if (PartLoads.QuadPart != 0)
	{
		scsi->packet.Amount_Sectors = (DWORD)PartLoads.QuadPart;
		scsi->Read10Part(scsi->packet);
		WriteFile(scsi->hDataFile, scsi->spwb.Data_Returned, (DWORD)PartLoads.QuadPart * scsi->MediaInfo.Current.CurrentSectorSize, &Written, NULL);
		scsi->packet.LFileCount.QuadPart = scsi->packet.LFileCount.QuadPart + (PartLoads.QuadPart * scsi->MediaInfo.Current.CurrentSectorSize);
	}
	//**************************************************************
	Sleep(1000);//wait for Timer to send last info
	KillTimer(scsi->packet.hDlg, IDT_TIMER_READ);//Stop Timer
	//****************** Close File  if Opened *********************
	if (scsi->fDataFileFlag)
	{
		CloseHandle(scsi->hDataFile);
		StringCchPrintf(wTemp, _countof(wTemp), L"INFO	File ");
		wcscat_s(wTemp, _countof(wTemp), scsi->packet.wBuffer);
		wcscat_s(wTemp, _countof(wTemp), L"Has Been Closed");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
	}
	//*************************************************************
	
	_endthreadex(0);
	return 0;
}
//***************************************************************

//********************* Blank Quick ************************** (2)
unsigned __stdcall BlankDvdMedia(void *par)
{
	UCHAR RetStatus;
	WCHAR wTemp[128];
	BYTE ProgressMsb = 0;
	BYTE ProgressLsb = 0;
	BYTE format_msb;
	BYTE format_high;
	BYTE format_low;
	BYTE format_lsb;
	BYTE Sense_Key;
	BYTE Sense_Specific;
	int format;

	ScsiClass *scsi;
	scsi = (ScsiClass *)par;
	
	MessageBeep(MB_ICONWARNING);
	//****************** Error If Not a DVD-RW OR a DVD+RW ****************
	if ((scsi->MediaInfo.DvdMediaClass.DvdBookType == BOOK_DVD_MINUS_RW) || (scsi->MediaInfo.DvdMediaClass.DvdBookType == BOOK_DVD_PLUS_RW))
	{//open 1
		//***************************************************************************
		//********************* See if A DVD+RW **********************************
		if (scsi->MediaInfo.DvdMediaClass.DvdBookType == BOOK_DVD_PLUS_RW)
		{//open 2
			if ((scsi->packet.BlankingType == BLANK_QUICK) || (scsi->packet.BlankingType == BLANK_FULL))//Full format is called for inform user if not needed
			{//open 3
				StringCchPrintf(wTemp, _countof(wTemp), L"INFO	FULL Format DVD + RW ");
				SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
				scsi->packet.Progress_Indicator = 0;
				RetStatus = scsi->ReadFormatCapacity(scsi->packet);
				if (RetStatus != 0)
				{//open 4
					StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Failed to ReadFormatCapacity ");
					SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
					_endthreadex(0);
					return 0;
				}//close 4
				if (scsi->spwb.Data_Returned[8] == 1)//unformatted media = 1:2 = formatted media:3 = no media present
				{//open 4
					SetTimer(scsi->packet.hDlg, IDT_TIMER_BLANKING, 500, (TIMERPROC)NULL);
					StringCchPrintf(wTemp, _countof(wTemp), L"INFO	DVD+RW Media Is Not Formatted");
					SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

					format = MessageBox(scsi->packet.hDlg, wTemp, L"Info", MB_OK);
					format_msb = scsi->spwb.Data_Returned[4];
					format_high = scsi->spwb.Data_Returned[5];
					format_low = scsi->spwb.Data_Returned[6];
					format_lsb = scsi->spwb.Data_Returned[7];
					if (format == IDOK)
					{//open 5

						StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Formatting DVD+RW Please Wait");
						SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

						scsi->packet.FormatType	= 0X11;//sets bit4 and bit0 (must be set)
						scsi->w_spwb.Data_Returned[0] = 0;
						scsi->w_spwb.Data_Returned[1] = 1;//sets the IMMED bit and vendor specific
						scsi->w_spwb.Data_Returned[2] = 0;//
						scsi->w_spwb.Data_Returned[3] = 8;
						scsi->w_spwb.Data_Returned[4] = format_msb;
						scsi->w_spwb.Data_Returned[5] = format_high;
						scsi->w_spwb.Data_Returned[6] = format_low;
						scsi->w_spwb.Data_Returned[7] = format_lsb;
						scsi->w_spwb.Data_Returned[8] = 0X98;//0X26 << 2//basic dvd+rw basic format
						scsi->w_spwb.Data_Returned[9] = 0;
						scsi->w_spwb.Data_Returned[10] = 0;
						scsi->w_spwb.Data_Returned[11] = 0;
						RetStatus =  scsi->FormatDisc(scsi->packet);
						if (RetStatus != 0)
						{//open 6
							StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Failed to FormatDisc ");
							SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
							KillTimer(scsi->packet.hDlg, IDT_TIMER_BLANKING);//turn off timer	
							_endthreadex(0);
							return 0;
						}//close 6

						do
						{//open 6
							scsi->TestUnitReady(scsi->packet);
							Sleep(100);
							scsi->RequestSense(scsi->packet);
							Sleep(100);
							Sense_Key = scsi->spwb.Data_Returned[2];
							Sense_Key = Sense_Key & 0XF;
							Sense_Specific = scsi->spwb.Data_Returned[15];//Bit7 = SKSV Bits[6]-[0] is reserved
							Sense_Specific = Sense_Specific & 0X80;

							ProgressMsb = scsi->spwb.Data_Returned[16];//FieldPointerMsb
							ProgressLsb = scsi->spwb.Data_Returned[17];//FieldPointerLsb
							scsi->packet.Progress_Indicator = ProgressLsb << 8;
							scsi->packet.Progress_Indicator = scsi->packet.Progress_Indicator + ProgressMsb;
						}//close 6
						while ((Sense_Key == 2) || (Sense_Specific == 0X80));//close 2

						StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Formatting DVD+RW Please Wait");
						SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

					

						SendDlgItemMessage(scsi->packet.hDlg, IDC_BLANKING_PROGRESS, PBM_SETPOS, 100, 0);
						SendDlgItemMessageA(scsi->packet.hDlg, IDC_BLANK_PERCENT, LB_RESETCONTENT, 0, 0);
						StringCchPrintf(wTemp, _countof(wTemp), L" %d   ", 100);
						SendDlgItemMessage(scsi->packet.hDlg, IDC_BLANK_PERCENT, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
						StringCchPrintf(wTemp, _countof(wTemp), L" Blanking %d%%", 100);
						SendMessage(hStatusMedia, SB_SETTEXT, 1, (LPARAM)(WCHAR *)wTemp);


						KillTimer(scsi->packet.hDlg, IDT_TIMER_BLANKING);//turn off timer	
						_endthreadex(0);
						return 0;
					}//close 5 if(IDOK)
				}//close 4 if unformatted Media dectected
				else
				{
					StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Already Formatted Just Burn!");
					SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
					_endthreadex(0);
					return 0;

				}
			}////close 3 if(BLANK_FULL)
		}//close 2 (if(BOOK_DVD_PLUS_RW)

		if (scsi->MediaInfo.DvdMediaClass.DvdBookType == BOOK_DVD_MINUS_RW)
		{//open 2
			//**************************************************************************
			//********************* Start Blanking DVD-RW ***************************
			if (scsi->packet.BlankingType == BLANK_FULL)
			{//open 3
				StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Blanking DVD - RW Full ");
				SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
				scsi->BlankDisc(scsi->packet);

			}//close 3
			else
			{//open 3
				StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Blanking DVD - RW Quick ");
				SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
				scsi->BlankDisc(scsi->packet);
			}//close 3

			SetTimer(scsi->packet.hDlg, IDT_TIMER_BLANKING, 50, (TIMERPROC)NULL);
			scsi->packet.Progress_Indicator = 0;
			do
			{//open 3
				scsi->TestUnitReady(scsi->packet);
				Sleep(100);
				scsi->RequestSense(scsi->packet);
				Sleep(100);
				Sense_Key = scsi->spwb.Data_Returned[2];
				Sense_Key = Sense_Key & 0XF;
				Sense_Specific = scsi->spwb.Data_Returned[15];//Bit7 = SKSV Bits[6]-[0] is reserved
				Sense_Specific = Sense_Specific & 0X80;

				ProgressMsb = scsi->spwb.Data_Returned[16];//FieldPointerMsb
				ProgressLsb = scsi->spwb.Data_Returned[17];//FieldPointerLsb
				scsi->packet.Progress_Indicator = ProgressMsb << 8;
				scsi->packet.Progress_Indicator = scsi->packet.Progress_Indicator + ProgressLsb;
			} while ((Sense_Key == 2) || (Sense_Specific == 0X80));//close 3

			SendDlgItemMessage(scsi->packet.hDlg, IDC_BLANKING_PROGRESS, PBM_SETPOS, 100, 0);
			SendDlgItemMessageA(scsi->packet.hDlg, IDC_BLANK_PERCENT, LB_RESETCONTENT, 0, 0);
			StringCchPrintf(wTemp, _countof(wTemp), L" %d   ", 100);
			SendDlgItemMessage(scsi->packet.hDlg, IDC_BLANK_PERCENT, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
			StringCchPrintf(wTemp, _countof(wTemp), L" Blanking %d%%", 100);
			SendMessage(hStatusMedia, SB_SETTEXT, 1, (LPARAM)(WCHAR *)wTemp);
			KillTimer(scsi->packet.hDlg, IDT_TIMER_BLANKING);//turn off timer	
			//*************************************************************
			_endthreadex(0);
			return 0;
		}//close 2
	}//close 1
	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Blanking Aborted No Eraseable Media ");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
		_endthreadex(0);
		return 0;
}
//****************************************************************



//********************* WritePlusDvd ***********************(4)
unsigned __stdcall WritePlusDvd(void *par)
{
	BOOL StatusWrite;
	BOOL bStatus;
	bool FirstWait = true;
	UCHAR retcode;
	WCHAR wTemp[128];
	WCHAR wBuffer[128];
	BOOLEAN FirstLoad = TRUE;
	BOOLEAN FirstMain = TRUE;
	DWORD BytesWrite;
	int WaitCount;
	LARGE_INTEGER FullLoads, PartLoads;
	LARGE_INTEGER count;
	DWORD Written;
	DWORD ErrorCode;
	BYTE format_msb;
	BYTE format_high;
	BYTE format_low;
	BYTE format_lsb;
	BYTE Sense_Key;
	BYTE Sense_Specific;
	int format;
	BYTE ProgressMsb = 0;
	BYTE ProgressLsb = 0;

	ScsiClass *scsi;
	scsi = (ScsiClass *)par;
	//***************** Testing for error conditions ***************
	if ((!scsi->WriteDataFileFlag) || (!scsi->fDataFileFlag) || (scsi->LastMedia != DVD))
	{
		StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	No File/No DVD!");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
		_endthreadex(0);
		return 0xff;
	}
	
	//**************** Get File size on Disk ****************
	bStatus = GetFileSizeEx(scsi->hDataFile, &scsi->TargetFileSize);
	if (!bStatus)
	{
		ErrorCode = GetLastError();
		StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Failed To Get TargetFile Size Error Code  %u!!", ErrorCode);
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
		_endthreadex(0);
		return 0xff;
	}
	//*********************************** Checking for unformatted Media **************************************
	
	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	FULL Format DVD + RW ");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
	scsi->packet.Progress_Indicator = 0;
	retcode = scsi->ReadFormatCapacity(scsi->packet);
	if (retcode != 0)
	{//open 1
		StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Failed to ReadFormatCapacity ");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
		_endthreadex(0);
		return 0;
	}//close 1
	if (scsi->spwb.Data_Returned[8] == 1)//unformatted media = 1:2 = formatted media:3 = no media present
	{//open 1
		SetTimer(scsi->packet.hDlg, IDT_TIMER_BLANKING, 500, (TIMERPROC)NULL);
		StringCchPrintf(wTemp, _countof(wTemp), L"INFO	DVD+RW Media Is Not Formatted");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

		format = MessageBox(scsi->packet.hDlg, wTemp, L"Info", MB_OK);
		format_msb = scsi->spwb.Data_Returned[4];//
		format_high = scsi->spwb.Data_Returned[5];
		format_low = scsi->spwb.Data_Returned[6];
		format_lsb = scsi->spwb.Data_Returned[7];
		if (format == IDOK)
		{//open 2

			StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Formatting DVD+RW Please Wait");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

			scsi->packet.FormatType = 0X11;//sets bit4 and bit0 (must be set)
			scsi->w_spwb.Data_Returned[0] = 0;
			scsi->w_spwb.Data_Returned[1] = 1;//sets the IMMED bit and vendor specific
			scsi->w_spwb.Data_Returned[2] = 0;//
			scsi->w_spwb.Data_Returned[3] = 8;
			scsi->w_spwb.Data_Returned[4] = 0xff;// format_msb;
			scsi->w_spwb.Data_Returned[5] = 0xff;// format_high;
			scsi->w_spwb.Data_Returned[6] = 0xff;// format_low;
			scsi->w_spwb.Data_Returned[7] = 0xff;// format_lsb;
			scsi->w_spwb.Data_Returned[8] = 0X98;//0X26 << 2//basic dvd+rw basic format
			scsi->w_spwb.Data_Returned[9] = 0;
			scsi->w_spwb.Data_Returned[10] = 0;
			scsi->w_spwb.Data_Returned[11] = 0;
			retcode = scsi->FormatDisc(scsi->packet);
			if (retcode != 0)
			{//open 3
				StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Failed to FormatDisc ");
				SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
				KillTimer(scsi->packet.hDlg, IDT_TIMER_BLANKING);//turn off timer	
				_endthreadex(0);
				return 0;
			}//close 3

			do
			{//open 3
				scsi->TestUnitReady(scsi->packet);
				Sleep(100);
				scsi->RequestSense(scsi->packet);
				Sleep(100);
				Sense_Key = scsi->spwb.Data_Returned[2];
				Sense_Key = Sense_Key & 0XF;
				Sense_Specific = scsi->spwb.Data_Returned[15];//Bit7 = SKSV Bits[6]-[0] is reserved
				Sense_Specific = Sense_Specific & 0X80;

				ProgressMsb = scsi->spwb.Data_Returned[16];//FieldPointerMsb
				ProgressLsb = scsi->spwb.Data_Returned[17];//FieldPointerLsb
				scsi->packet.Progress_Indicator = ProgressLsb << 8;
				scsi->packet.Progress_Indicator = scsi->packet.Progress_Indicator + ProgressMsb;
				//scsi->ReadDiscInfo(scsi->packet);
				//scsi->spwb.Data_Returned[7] = scsi->spwb.Data_Returned[7] & 3;

					
			}//close 3
			while ((Sense_Key == 2) || (Sense_Specific == 0X80));//close 2

			SendDlgItemMessage(scsi->packet.hDlg, IDC_BLANKING_PROGRESS, PBM_SETPOS, 100, 0);
			SendDlgItemMessageA(scsi->packet.hDlg, IDC_BLANK_PERCENT, LB_RESETCONTENT, 0, 0);
			StringCchPrintf(wTemp, _countof(wTemp), L" %d   ", 100);
			SendDlgItemMessage(scsi->packet.hDlg, IDC_BLANK_PERCENT, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
			StringCchPrintf(wTemp, _countof(wTemp), L" Blanking %d%%", 100);
			SendMessage(hStatusMedia, SB_SETTEXT, 1, (LPARAM)(WCHAR *)wTemp);
			KillTimer(scsi->packet.hDlg, IDT_TIMER_BLANKING);//turn off timer	
		}//close 2
	}//close 1
	//Sleep(30000);
	
	//******************* Using scsi rotine to add commas to a large number

	scsi->Display.Number.QuadPart = scsi->TargetFileSize.QuadPart;
	scsi->Display.AmountSet = 10;
	wcscpy_s(scsi->Display.formatbuffer, _countof(scsi->Display.formatbuffer), L" Bytes");
	scsi->ShowDigits(&scsi->Display);
	wcscpy_s(wBuffer, _countof(wBuffer), L"INFO	The TargetFile Size Is  ");
	wcscat_s(wBuffer, _countof(wBuffer), scsi->Display.Bufferset);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)wBuffer);

	scsi->Display.Number.QuadPart = scsi->TargetFileSize.QuadPart / 2048;
	scsi->Display.AmountSet = 10;
	wcscpy_s(scsi->Display.formatbuffer, _countof(scsi->Display.formatbuffer), L" Sectors");
	scsi->ShowDigits(&scsi->Display);
	wcscpy_s(wBuffer, _countof(wBuffer), L"INFO	The TargetFile Size Sectors  ");
	wcscat_s(wBuffer, _countof(wBuffer), scsi->Display.Bufferset);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)wBuffer);

	//**************************************************************

	scsi->packet.FileSize.QuadPart = scsi->TargetFileSize.QuadPart;

	FullLoads.QuadPart	= (DWORD)((scsi->TargetFileSize.QuadPart / 2048) / AUDIO_BLOCK_SIZE);//calulate for data
	PartLoads.QuadPart	= (DWORD)((scsi->TargetFileSize.QuadPart / 2048) % AUDIO_BLOCK_SIZE);

	if ((FullLoads.QuadPart <= MINIUM_FILE_SECTORS) || (FullLoads.QuadPart == 0))
	{
		if (scsi->fDataFileFlag)
		{
			StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Closing File");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
			CloseHandle(scsi->hDataFile);
			scsi->fDataFileFlag = false;
		}
		StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	File size Too Small");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
		_endthreadex(0);
		return 0xff;
	}
	//******************* if here all ok ****************

	
	scsi->packet.Write10Byte1		= 0;
	scsi->packet.Write10Lba			= 0;
	scsi->packet.Size_Sectors			= 2048;
	scsi->packet.Amount_Sectors	= (DWORD)DATA_BLOCK_SIZE;

	QueryPerformanceFrequency(&scsi->Frequency);
	QueryPerformanceCounter(&scsi->StartingTime);

	scsi->packet.PreventAllow = 1;
	scsi->PreventAllow(scsi->packet);
	WaitCount = 4000;
	scsi->WaitUntillReady(&WaitCount);

	QueryPerformanceCounter(&scsi->EndingTime);
	scsi->ElapsedMicroseconds = (double)scsi->EndingTime.QuadPart - scsi->StartingTime.QuadPart;
	scsi->ElapsedMicroseconds *= 1000000;
	scsi->ElapsedMicroseconds /= scsi->Frequency.QuadPart;

	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Locking Drive Took %6.2f ms ", scsi->ElapsedMicroseconds / 1000);//show in ms 
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

	scsi->packet.FileCount = 0;
	scsi->packet.LFileCount.QuadPart = 0;
	scsi->Time_Secs = 0;//reset timer in class of scsi
	SetTimer(scsi->packet.hDlg, IDT_TIMER_WRITE, 1000, (TIMERPROC)NULL);
	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Timer 1 Now On");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Locking Device");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
	//*********************************************************************************************************************
	//****************************** For Loop ********************************
	for (count.QuadPart = 0; count.QuadPart < FullLoads.QuadPart; count.QuadPart++)
	{//open 1
		StatusWrite = ReadFile(scsi->hDataFile, scsi->w_spwb.Data_Returned, AUDIO_BLOCK_SIZE * scsi->packet.Size_Sectors, &Written, NULL);
		if (StatusWrite == false)
		{//open 2
			StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Failed To Read File!!");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
			scsi->packet.PreventAllow = 0;
			scsi->PreventAllow(scsi->packet);
			if (scsi->fDataFileFlag)
				CloseHandle(scsi->hDataFile);
			KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);
			_endthreadex(0);
			return 0;
		}//close 2
		if ((FirstMain) && (!FirstLoad))
		{//open 2
			StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Writing Main Data");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
			FirstMain = FALSE;
		}//close 2
		retcode = scsi->WriteCDFull(scsi->packet);

		switch (retcode)
		{//open 3
			break;
		case DEVICEIOCONTROL_FAILED:
			StringCchPrintf(wTemp, _countof(wTemp), L"ERROR	Failed To Write To CD!!");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
			scsi->packet.PreventAllow = 0;
			scsi->PreventAllow(scsi->packet);
			KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);
			if (scsi->fDataFileFlag)
				CloseHandle(scsi->hDataFile);
			_endthreadex(0);
			return 0;
			break;

		case DEVICEIOCONTROL_OK:

			break;

		case DEVICEIOCONTROL_SENSE:
			//if (FirstLoad)
			//{//open 2a
			do
			{
				if (FirstLoad)
				{
					StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Writing Leadin");
					SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
					FirstLoad = FALSE;
				}
				//WaitCount = 400;
				//scsi->WaitUntillReady(&WaitCount);
				scsi->WriteCDFull(scsi->packet);
			} while (scsi->w_spwb.Block.ScsiStatus == 2);

			break;
		}
		scsi->packet.Write10Lba = scsi->packet.Write10Lba + DATA_BLOCK_SIZE;
		scsi->CurrentTrack = 0;
		BytesWrite = scsi->packet.Amount_Sectors * scsi->packet.Size_Sectors;
		scsi->packet.LFileCount.QuadPart = scsi->packet.LFileCount.QuadPart + BytesWrite;

	}//close open 1

	scsi->packet.Amount_Sectors = (DWORD)PartLoads.QuadPart;

	if (PartLoads.QuadPart != 0)
	{//open 2
		
		retcode = ReadFile(scsi->hDataFile, scsi->w_spwb.Data_Returned, (DWORD)scsi->packet.Size_Sectors * (DWORD)PartLoads.QuadPart,
			&Written, NULL);
		QueryPerformanceFrequency(&scsi->Frequency);
		QueryPerformanceCounter(&scsi->StartingTime);
		scsi->WriteCDPart(scsi->packet);
		WaitCount = 4000;//wait max  = 400secs
		scsi->WaitUntillReady(&WaitCount);
		BytesWrite = scsi->packet.Amount_Sectors * scsi->packet.Size_Sectors;
		scsi->packet.LFileCount.QuadPart = scsi->packet.LFileCount.QuadPart + BytesWrite;
		QueryPerformanceCounter(&scsi->EndingTime);
		scsi->ElapsedMicroseconds = (double)scsi->EndingTime.QuadPart - scsi->StartingTime.QuadPart;
		scsi->ElapsedMicroseconds *= 1000000;
		scsi->ElapsedMicroseconds /= scsi->Frequency.QuadPart;
		StringCchPrintf(wTemp, _countof(wTemp), L"INFO	WriteCdPart Took %6.3f ms", scsi->ElapsedMicroseconds / 1000);
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

	}//close 2
	scsi->packet.Write10Lba = (DWORD)scsi->packet.Write10Lba +(DWORD) PartLoads.QuadPart;
	scsi->packet.LFileCount.QuadPart = scsi->TargetFileSize.QuadPart;
	Sleep(4000);
	KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);//turn off timer
	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Timer 1 Now Off");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

	//************** Ending Stage ************************************************************************
	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Syncache Now Please Wait");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

	scsi->packet.SynImmediate = 0;
	QueryPerformanceFrequency(&scsi->Frequency);
	QueryPerformanceCounter(&scsi->StartingTime);

	WaitCount = 4000;//wait max  = 400secs
	scsi->WaitUntillReady(&WaitCount);

	scsi->SynCache(scsi->packet);
	WaitCount = 4000;//wait max  = 400secs
	scsi->WaitUntillReady(&WaitCount);

	QueryPerformanceCounter(&scsi->EndingTime);
	scsi->ElapsedMicroseconds = (double)scsi->EndingTime.QuadPart - scsi->StartingTime.QuadPart;
	scsi->ElapsedMicroseconds *= 1000000;
	scsi->ElapsedMicroseconds /= scsi->Frequency.QuadPart;

	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	SynCache Took %6.3f ms", scsi->ElapsedMicroseconds / 1000);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

	//*****************  if CD Media  close Track *******************
	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Closing Track Now Please Wait");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

	scsi->packet.CloseType = 0;//close track
	scsi->packet.CloseTrackLsb = 0;
	scsi->packet.CloseTrackMsb = 0;
	WaitCount = 4000;//wait max  = 400secs
	scsi->WaitUntillReady(&WaitCount);
	QueryPerformanceFrequency(&scsi->Frequency);
	QueryPerformanceCounter(&scsi->StartingTime);

	retcode = scsi->CloseTrack(scsi->packet);
	WaitCount = 4000;//wait max  = 400secs
	scsi->WaitUntillReady(&WaitCount);

	QueryPerformanceCounter(&scsi->EndingTime);
	scsi->ElapsedMicroseconds = (double)scsi->EndingTime.QuadPart - scsi->StartingTime.QuadPart;
	scsi->ElapsedMicroseconds *= 1000000;
	scsi->ElapsedMicroseconds /= scsi->Frequency.QuadPart;

	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	CloseTrack Took %6.3f ms",scsi-> ElapsedMicroseconds / 1000);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
	//*****************  if CD Media  close session *******************
	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Closing Session Now Please Wait");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

	scsi->packet.CloseType				= 2;//close session
	scsi->packet.CloseTrackLsb		= 0;
	scsi->packet.CloseTrackMsb	= 0;

	QueryPerformanceFrequency(&scsi->Frequency);
	QueryPerformanceCounter(&scsi->StartingTime);

	retcode = scsi->CloseTrack(scsi->packet);
	WaitCount = 4000;//wait max  = 400secs
	scsi->WaitUntillReady(&WaitCount);

	QueryPerformanceCounter(&scsi->EndingTime);
	scsi->ElapsedMicroseconds = (double)scsi->EndingTime.QuadPart - scsi->StartingTime.QuadPart;
	scsi->ElapsedMicroseconds *= 1000000;
	scsi->ElapsedMicroseconds /= scsi->Frequency.QuadPart;

	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	CloseSession Took %6.3f ms", scsi->ElapsedMicroseconds / 1000);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
	//***********************************************************
	scsi->packet.PreventAllow = 0;

	QueryPerformanceFrequency(&scsi->Frequency);
	QueryPerformanceCounter(&scsi->StartingTime);

	scsi->PreventAllow(scsi->packet);
	WaitCount = 4000;//wax wait = 400secs
	scsi->WaitUntillReady(&WaitCount);

	QueryPerformanceCounter(&scsi->EndingTime);
	scsi->ElapsedMicroseconds = (double)scsi->EndingTime.QuadPart - scsi->StartingTime.QuadPart;
	scsi->ElapsedMicroseconds *= 1000000;
	scsi->ElapsedMicroseconds /= scsi->Frequency.QuadPart;

	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	UnLocking Drive Took %6.3f ms", scsi->ElapsedMicroseconds / 1000);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Timer 1 Now Off");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
	scsi->OpenDoor(scsi->packet);
	Sleep(1000);
	scsi->CloseDoor(scsi->packet);
	scsi->TypeWriteInProgress = WRITE_DONE_OK;
	
	//***************************************************************************************
	StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Burn Has Completed");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);

	if (scsi->fDataFileFlag)
	{
		StringCchPrintf(wTemp, _countof(wTemp), L"INFO	Closing File");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wTemp);
		CloseHandle(scsi->hDataFile);
		scsi->fDataFileFlag = false;
	}
	 //****************************** For Loop ********************************
	_endthreadex(0);
	return 0;
}

//********************** WriteMinusDvd(*scsi) ****************************************
unsigned __stdcall WriteMinusDvd(void *par) //(5)
{
	ScsiClass *scsi;
	scsi = (ScsiClass *)par;

	void *any;
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	LARGE_INTEGER FullLoads, PartLoads;
	DWORD BytesWrite;
	DWORD FullLevel;
	DWORD EmptyLevel = 128;
	LPREAD_CAPACITY_ZERO BufferCapacity;
	DWORD BufferTotalSizeBytes;
	DWORD BufferTotalSizeSectors;
	DWORD BufferFreeSizeBytes;
	DWORD BufferFreeSizeSectors;
	DWORD BufferUsedSizeSectors;

	DWORD count;
	DWORD Written;
	BOOL RetStatus;
	BOOL bStatus;
	BOOLEAN FirstLoad = TRUE;
	BOOLEAN FirstMain = TRUE;
	UCHAR retcode;
	UCHAR RetScsi;
	UCHAR ret;
	BYTE SenseKey;
	BYTE SenseSpecific;
	BYTE ProgressMsb = 0;
	BYTE ProgressLsb = 0;
	DWORD ErrorCode;
	int MessageReply;
	int WaitCount;
	WCHAR wBuffer[256];
	
	any = &scsi->spwb.Data_Returned;
	BufferCapacity = (LPREAD_CAPACITY_ZERO)any;
	//***************** Testing for error conditions ***************
	if ((!scsi->WriteDataFileFlag) || (!scsi->fDataFileFlag) || (scsi->LastMedia != DVD))
	{
		StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	No File/No DVD!");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
		_endthreadex(0);
		return 0xff;
	}

	//**************** Get File size on Disk ****************
	bStatus = GetFileSizeEx(scsi->hDataFile, &scsi->TargetFileSize);
	if (!bStatus)
	{
		ErrorCode = GetLastError();
		StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	Failed To Get TargetFile Size Error Code  %u!!", ErrorCode);
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
		_endthreadex(0);
		return 0xff;
	}
	

	//******************* Using scsi rotine to add commas to a large number

	scsi->Display.Number.QuadPart = scsi->TargetFileSize.QuadPart;
	scsi->Display.AmountSet = 10;
	wcscpy_s(scsi->Display.formatbuffer, _countof(scsi->Display.formatbuffer), L" Bytes");
	scsi->ShowDigits(&scsi->Display);
	wcscpy_s(wBuffer, _countof(wBuffer), L"INFO	The TargetFile Size Is  ");
	wcscat_s(wBuffer, _countof(wBuffer), scsi->Display.Bufferset);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)wBuffer);

	scsi->Display.Number.QuadPart = scsi->MediaInfo.Current.CurrentAmounfOfSectors.QuadPart;
	scsi->Display.AmountSet = 10;
	wcscpy_s(scsi->Display.formatbuffer, _countof(scsi->Display.formatbuffer), L" Sectors");
	scsi->ShowDigits(&scsi->Display);
	wcscpy_s(wBuffer, _countof(wBuffer), L"INFO	The TargetFile Sectors are  ");
	wcscat_s(wBuffer, _countof(wBuffer), scsi->Display.Bufferset);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)wBuffer);
	//**************************************************************

	scsi->packet.FileSize.QuadPart = scsi->TargetFileSize.QuadPart;

	FullLoads.QuadPart	= (DWORD)((scsi->TargetFileSize.QuadPart / 2048) / AUDIO_BLOCK_SIZE);//calulate for data
	PartLoads.QuadPart	= (DWORD)((scsi->TargetFileSize.QuadPart / 2048) % AUDIO_BLOCK_SIZE);

	if ((FullLoads.QuadPart <= MINIUM_FILE_SECTORS) || (FullLoads.QuadPart == 0))
	{
		if (scsi->fDataFileFlag)
		{
			StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	Closing File");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
			CloseHandle(scsi->hDataFile);
			scsi->fDataFileFlag = false;
		}
		StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	File size Too Small");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
		_endthreadex(0);
		return 0xff;
	}
	//******************* if here all ok ****************

	
	if (scsi->LastMedia == DVD)
	{
		StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Correct Media In Drive");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
	}
	else
	{
		StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	InCorrect Media In Drive");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
	}

	if ((scsi->MediaInfo.MediaEraseableFlag) && (scsi->MediaInfo.DiskInfo.DiscStatus == 0))
	{
		StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Media Is Already Blank");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
	}
	//****************************************************************
	//************** Needs Blanking **********************************
	else
	{
		StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Media Needs Blanking");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
		//*********Testing **********************
		MessageReply = MessageBoxExW(scsi->packet.hDlg, L"This Media Requires Blanking\n"
			L"Do you Which to Quick Blank \n",
			L"Blanking", MB_YESNO | MB_ICONQUESTION, 0);
		switch (MessageReply)
		{//open switch on message relp
		case IDYES:
			//******
			scsi->packet.BlankingType = BLANK_QUICK;
			if ((scsi->packet.BlankingType == 0) || (scsi->packet.BlankingType == 0X10))
				StringCchPrintf(wBuffer, _countof(wBuffer), L"Blanking Disc (Full) ");
			else
				StringCchPrintf(wBuffer, _countof(wBuffer), L"Blanking Disc (Quick) ");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_INFORMATION_DETAILS, WM_SETTEXT, 0, (LPARAM)(WCHAR *)wBuffer);
			scsi->BlankDisc(scsi->packet);
			SetTimer(scsi->packet.hDlg, IDT_TIMER_BLANKING, 100, (TIMERPROC)NULL);
			scsi->packet.Progress_Indicator = 0;

			do
			{//open 2
				scsi->TestUnitReady(scsi->packet);
				Sleep(500);
				scsi->RequestSense(scsi->packet);
				Sleep(500);
				SenseKey = scsi->spwb.Data_Returned[2];
				SenseSpecific = scsi->spwb.Data_Returned[15];//Bit7 = SKSV Bits[6]-[0] is reserved
				SenseKey = SenseKey & 0XF;
				ProgressMsb = scsi->spwb.Data_Returned[16];
				ProgressLsb = scsi->spwb.Data_Returned[17];
				scsi->packet.Progress_Indicator = ProgressMsb << 8;
				scsi->packet.Progress_Indicator = scsi->packet.Progress_Indicator + ProgressLsb;
			} while ((SenseKey == 2) || (SenseSpecific == 0X80));//close 2

			SendDlgItemMessage(scsi->packet.hDlg, IDC_BLANKING_PROGRESS, PBM_SETPOS, 100, 0);
			StringCchPrintf(wBuffer, _countof(wBuffer), L" Blanking %d%%", 100);
			SendMessage(hStatusMedia, SB_SETTEXT, 1, (LPARAM)(WCHAR *)wBuffer);
			KillTimer(scsi->packet.hDlg, IDT_TIMER_BLANKING);//turn off timer	
			break;
			//***********
		case IDNO:
			break;

			break;
		}//close switch on message relpy

	}//close else if media needs blanking
	scsi->packet.ModeSelectWriteType	= DISC_AT_ONCE;//DISC works here for DVD+RW
	scsi->TypeWriteInProgress			= WRITE_DVD_DATA;
	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	DISC_AT_ONCE");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Writing DVD(Minus) Data");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
	//*****************************************************
	scsi->TestBurn = FALSE;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);
	retcode = scsi->SetWriteMode();
	if (retcode != DEVICEIOCONTROL_OK)
	{
		scsi->packet.PreventAllow = 0;
		scsi->PreventAllow(scsi->packet);
		if (scsi->fDataFileFlag)
		{
			CloseHandle(scsi->hDataFile);
			scsi->fDataFileFlag = false;
		}
		_endthreadex(0);
		return false;
	}

	WaitCount = 4000;
	scsi->WaitUntillReady(&WaitCount);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	SetWriteMode Took %I64u ms ", ElapsedMicroseconds.QuadPart / 1000);//show in ms 
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

	//*************************************************************************************
	scsi->packet.Write10Byte1 = 0;
	scsi->packet.Write10Lba = 0;
	scsi->packet.Size_Sectors = 2048;
	scsi->packet.Amount_Sectors = (DWORD)AUDIO_BLOCK_SIZE;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	scsi->packet.PreventAllow = 1;
	scsi->PreventAllow(scsi->packet);
	WaitCount = 4000;
	scsi->WaitUntillReady(&WaitCount);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Locking Drive Took %I64u ms ", ElapsedMicroseconds.QuadPart / 1000);//show in ms 
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

	scsi->packet.FileCount = 0;
	scsi->packet.LFileCount.QuadPart = 0;
	scsi->Time_Secs = 0;//reset timer in class of scsi
	SetTimer(scsi->packet.hDlg, IDT_TIMER_WRITE, 1000, (TIMERPROC)NULL);
	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Timer 1 Now On");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Locking Device");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

	//******************************* reserved Track ********************************
	if (scsi->MediaInfo.DvdMediaClass.DvdBookType == BOOK_DVD_MINUS_RW) //do this only if current media selected is DVD_MINUS_RW
	{//open reserve track if DVD
		scsi->packet.ReservationSize = (DWORD)scsi->packet.FileSize.QuadPart / 2048;
		scsi->packet.ReservationType = 0;
		retcode = scsi->ReserveTrackRzone(scsi->packet);
		StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Reserve Track Rzone Started");
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
		switch (retcode)//test retcode
		{//open 3
		case DEVICEIOCONTROL_FAILED:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	1 Reserve Track Rzone Failed");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
			scsi->packet.PreventAllow = 0;
			scsi->PreventAllow(scsi->packet);
			if (scsi->fDataFileFlag)
			{
				CloseHandle(scsi->hDataFile);
				scsi->fDataFileFlag = false;
			}
			//_endthreadex(0);
			KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);
			break;
			//return false;

		case DEVICEIOCONTROL_SENSE:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	2 Reserve Track Rzone Failed");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
			scsi->packet.PreventAllow = 0;
			scsi->PreventAllow(scsi->packet);
			if (scsi->fDataFileFlag)
			{
				CloseHandle(scsi->hDataFile);
				scsi->fDataFileFlag = false;
			}
			KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);//turn off timer
			break;
			//return false;

		case DEVICEIOCONTROL_OK:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Reserve Track Rzone OK ");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
			break;
		}//close 3
	}//close if current media = DVD_RW
	 //***************************************************************************

	 //****************************** For Loop ********************************
	for (count = 0; count < FullLoads.QuadPart; count++)
	{
		RetStatus = ReadFile(scsi->hDataFile, scsi->w_spwb.Data_Returned, AUDIO_BLOCK_SIZE * scsi->packet.Size_Sectors, &Written, NULL);
		if (RetStatus == false)
		{
			StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	Failed To Read File!!");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
			scsi->packet.PreventAllow = 0;
			scsi->PreventAllow(scsi->packet);
			if (scsi->fDataFileFlag)
			{
				CloseHandle(scsi->hDataFile);
				scsi->fDataFileFlag = false;
			}
			_endthreadex(0);
			KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);
			return false;
		}
		if ((FirstMain) && (!FirstLoad))
		{
			StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Writing Main Data");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
			FirstMain = FALSE;
		}
		RetScsi = scsi->WriteCDFull(scsi->packet);

		switch (RetScsi)
		{//open switch
			break;
		case DEVICEIOCONTROL_FAILED:
			StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	Failed To Write To CD!!");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
			scsi->packet.PreventAllow = 0;
			scsi->PreventAllow(scsi->packet);
			KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);
			if (scsi->fDataFileFlag)
			{
				CloseHandle(scsi->hDataFile);
				scsi->fDataFileFlag = false;
			}
			_endthreadex(0);
			return false;
			break;

		case DEVICEIOCONTROL_OK:

			break;

		case DEVICEIOCONTROL_SENSE:
			//if (FirstLoad)
			//{//open 2a
			do
			{
				if (FirstLoad)
				{
					StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Writing Leadin");
					SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
					FirstLoad = FALSE;
				}
				//WaitCount = 400;
				//scsi->WaitUntillReady(&WaitCount);
				ret = scsi->WriteCDFull(scsi->packet);
			} while (scsi->w_spwb.Block.ScsiStatus == 2);

			break;
			//}

			/*
			if (!FirstLoad)
			{

			StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	Failed To Write To CD Sense!!");
			SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
			SendDlgItemMessageA(scsi->packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(char *)scsi->SenseError);
			SendDlgItemMessageA(scsi->packet.hDlg, IDC_INFORMATION_DETAILS, LB_ADDSTRING, 0, (LPARAM)(char *)scsi->LastCommandString);
			KillTimer(scsi->packet.hDlg, IDT_TIMER_1);
			scsi->packet.PreventAllow = 0;
			scsi->PreventAllow(scsi->packet);
			if (scsi->fDataFileFlag)
			CloseHandle(scsi->hDataFile);
			return false;
			break;
			}

			*/
			break;
		}//close switch

		 //************* Find The Dvd/cd unit's Buffer Capacity ONLY FOR DVD_BOOK_RWs
			scsi->packet.ReadBufferCapacityBlock = 0;
			retcode = scsi->ReadBufferCapacity(scsi->packet);
			//********************
			switch (retcode)//test retcode
			{//open 3
			case DEVICEIOCONTROL_FAILED:
				StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	Failed To Read BufferCapacity");
				SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
				KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);//turn off timer
				if (scsi->fDataFileFlag)
				{
					CloseHandle(scsi->hDataFile);
					scsi->fDataFileFlag = false;
				}
				_endthreadex(0);
				return false;

			case DEVICEIOCONTROL_SENSE:
				StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	Failed To Read BufferCapacity ");
				SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
				KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);//turn off timer
				if (scsi->fDataFileFlag)
				{
					CloseHandle(scsi->hDataFile);
					scsi->fDataFileFlag = false;
				}
				_endthreadex(0);
				return false;
			}//close 3
			 //*******************

			BufferTotalSizeBytes = scsi->BswapDword(&BufferCapacity->BufferSizeBytes);
			BufferTotalSizeSectors = BufferTotalSizeBytes / 2048;
			BufferFreeSizeBytes = scsi->BswapDword(&BufferCapacity->BufferFreeBytes);
			BufferFreeSizeSectors = BufferFreeSizeBytes / 2048;
			BufferUsedSizeSectors = BufferTotalSizeSectors - BufferFreeSizeSectors;
			scsi->packet.BufferAvilableCurrentBlocks = BufferUsedSizeSectors;
			FullLevel = BufferTotalSizeSectors - 128;//320 sectors below max device buffer

			if (BufferUsedSizeSectors >= FullLevel)
			{//open 4
				do
				{//open 5
					scsi->packet.ReadBufferCapacityBlock = 0;
					retcode = scsi->ReadBufferCapacity(scsi->packet);
					switch (retcode)//test retcode
					{//open 6
					case DEVICEIOCONTROL_FAILED:
						StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	Failed To Read BufferCapacity");
						SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
						KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);//turn off timer
						if (scsi->fDataFileFlag)
						{
							CloseHandle(scsi->hDataFile);
							scsi->fDataFileFlag = false;
						}
						_endthreadex(0);
						return false;

					case DEVICEIOCONTROL_SENSE:
						StringCchPrintf(wBuffer, _countof(wBuffer), L"ERROR	Failed To Read BufferCapacity");
						SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
						KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);//turn off timer
						if (scsi->fDataFileFlag)
						{
							CloseHandle(scsi->hDataFile);
							scsi->fDataFileFlag = false;
						}
						_endthreadex(0);
						return false;


					case DEVICEIOCONTROL_OK:
						BufferTotalSizeBytes = scsi->BswapDword(&BufferCapacity->BufferSizeBytes);
						BufferTotalSizeSectors = BufferTotalSizeBytes / 2048;
						BufferFreeSizeBytes = scsi->BswapDword(&BufferCapacity->BufferFreeBytes);
						BufferFreeSizeSectors = BufferFreeSizeBytes / 2048;
						BufferUsedSizeSectors = BufferTotalSizeSectors - BufferFreeSizeSectors;
						scsi->packet.BufferAvilableCurrentBlocks = BufferUsedSizeSectors;

						//StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO Read BufferCapacity Reports %d Used out of %d Buffers", BufferUsedSizeSectors, BufferTotalSizeSectors);
						//SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
						break;
					}//close 6
					 //*********************************************


				}//close 5

				while (BufferUsedSizeSectors > EmptyLevel);//close 4


			}//close 4

	

		scsi->packet.Write10Lba = scsi->packet.Write10Lba + AUDIO_BLOCK_SIZE;
		scsi->CurrentTrack = 0;
		BytesWrite = scsi->packet.Amount_Sectors * scsi->packet.Size_Sectors;
		scsi->packet.LFileCount.QuadPart = scsi->packet.LFileCount.QuadPart + BytesWrite;

	}

	scsi->packet.Amount_Sectors = (DWORD)PartLoads.QuadPart;

	if (PartLoads.QuadPart != 0)
	{//open 2
		StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Writing Cd PartLoads %d",(int) PartLoads.QuadPart);
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
		retcode = ReadFile(scsi->hDataFile, scsi->w_spwb.Data_Returned, (DWORD)scsi->packet.Size_Sectors * (DWORD)PartLoads.QuadPart,
			&Written, NULL);
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartingTime);
		scsi->WriteCDPart(scsi->packet);
		//WaitCount = 4000;//wait max  = 400secs
		//scsi->WaitUntillReady(&WaitCount);
		BytesWrite = scsi->packet.Amount_Sectors * scsi->packet.Size_Sectors;
		scsi->packet.LFileCount.QuadPart = scsi->packet.LFileCount.QuadPart + BytesWrite;
		QueryPerformanceCounter(&EndingTime);
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
		ElapsedMicroseconds.QuadPart *= 1000000;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
		StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Closing/Syn Took %I64u ms", ElapsedMicroseconds.QuadPart / 1000);
		SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

	}//close 2
	scsi->packet.Write10Lba = scsi->packet.Write10Lba + (DWORD)PartLoads.QuadPart;
	scsi->CurrentTrack = scsi->MediaInfo.CdMediaClass.TrackInfo.Last_Track;

	scsi->packet.FileCount = (DWORD)scsi->TargetFileSize.LowPart;
	Sleep(4000);
	KillTimer(scsi->packet.hDlg, IDT_TIMER_WRITE);//turn off timer
	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Timer 1 Now Off");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

	//************** Ending Stage ************************************************************************
	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Syncache Now Please Wait");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

	scsi->packet.SynImmediate = 0;
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	WaitCount = 4000;//wait max  = 400secs
	scsi->WaitUntillReady(&WaitCount);

	scsi->SynCache(scsi->packet);
	WaitCount = 4000;//wait max  = 400secs
	scsi->WaitUntillReady(&WaitCount);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	SynCache Took %I64u ms", ElapsedMicroseconds.QuadPart / 1000);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

	//*****************  if CD Media  close Track *******************
	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Closing Track Now Please Wait");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

	scsi->packet.CloseType = 0;//close track
	scsi->packet.CloseTrackLsb = 0;
	scsi->packet.CloseTrackMsb = 0;
	WaitCount = 4000;//wait max  = 400secs
	scsi->WaitUntillReady(&WaitCount);
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	retcode = scsi->CloseTrack(scsi->packet);
	WaitCount = 4000;//wait max  = 400secs
	scsi->WaitUntillReady(&WaitCount);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	CloseTrack Took %I64u ms", ElapsedMicroseconds.QuadPart / 1000);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
	//*****************  if CD Media  close session *******************
	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Closing Session Now Please Wait");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);

	scsi->packet.CloseType = 2;//close session
	scsi->packet.CloseTrackLsb = 0;
	scsi->packet.CloseTrackMsb = 0;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	retcode = scsi->CloseTrack(scsi->packet);
	WaitCount = 4000;//wait max  = 400secs
	scsi->WaitUntillReady(&WaitCount);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	CloseSession Took %I64u ms", ElapsedMicroseconds.QuadPart / 1000);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
	//***********************************************************
	scsi->packet.PreventAllow = 0;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	scsi->PreventAllow(scsi->packet);
	WaitCount = 4000;//wax wait = 400secs
	scsi->WaitUntillReady(&WaitCount);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	UnLocking Drive Took %I64u ms", ElapsedMicroseconds.QuadPart / 1000);
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Timer 1 Now Off");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
	scsi->OpenDoor(scsi->packet);
	Sleep(1000);
	scsi->CloseDoor(scsi->packet);
	scsi->TypeWriteInProgress = WRITE_DONE_OK;
	if (scsi->fDataFileFlag)
		CloseHandle(scsi->hDataFile);
	//***************************************************************************************
	StringCchPrintf(wBuffer, _countof(wBuffer), L"INFO	Burn Has Completed");
	SendDlgItemMessage(scsi->packet.hDlg, IDC_REPORT_LOG, LB_ADDSTRING, 0, (LPARAM)(WCHAR *)wBuffer);
	_endthreadex(0);
	return true;
}