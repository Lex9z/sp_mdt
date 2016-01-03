#include "PCMemoryChecksumProgress.h"

#include "Form\form_MultiLoad.h"
#include "MultiPortFlashDownloadTool.h"

#define DLCALL(a) {if((a)) {return -1;}}

PCMemoryChecksumProgress::PCMemoryChecksumProgress(bool CreateSuspended) : TThread(CreateSuspended),mHint("")
{

}

void __fastcall PCMemoryChecksumProgress::Execute()
{
    SyncStart();
    SyncEnd();

}

//---------------------------------------------------------------------------
void _fastcall PCMemoryChecksumProgress::Start()
{
    mPrefix = "";
    m_ucProgressPercentage = 0;
}

//---------------------------------------------------------------------------
void _fastcall PCMemoryChecksumProgress::SyncStart()
{
    Synchronize(Start);
    if (!TformPCMemoryChecksum::mChecksumFlag)
    {
       TformPCMemoryChecksum::mChecksumFlag = VerifyHostBufferBeforeStart();
    }

}


//---------------------------------------------------------------------------
void _fastcall PCMemoryChecksumProgress::End()
{
    formPCMemoryChecksum->Close();
}

//---------------------------------------------------------------------------
void _fastcall PCMemoryChecksumProgress::SyncEnd()
{
    Synchronize(End);
    
}


//---------------------------------------------------------------------------
void _fastcall PCMemoryChecksumProgress::InitProgress()
{

    formPCMemoryChecksum->PCMemoryChecksumProgressBar->Progress = 0;
    formPCMemoryChecksum->checksumImageNameLabel->Caption = mPrefix;
}

//---------------------------------------------------------------------------
void _fastcall PCMemoryChecksumProgress::SyncInitProgress()
{
    Synchronize(InitProgress);
}

//---------------------------------------------------------------------------
void _fastcall PCMemoryChecksumProgress::UpdateProgressPercentage()
{
    formPCMemoryChecksum->PCMemoryChecksumProgressBar->Progress = m_ucProgressPercentage;
}

//---------------------------------------------------------------------------
void _fastcall PCMemoryChecksumProgress::SyncUpdateProgressPercentage(int precentage)
{
   m_ucProgressPercentage = precentage;
   Synchronize(UpdateProgressPercentage);
}

//---------------------------------------------------------------------------
void _fastcall PCMemoryChecksumProgress::UpdateProgressColor()
{
   formPCMemoryChecksum->PCMemoryChecksumProgressBar->BackColor = _bk_col;
   formPCMemoryChecksum->PCMemoryChecksumProgressBar->ForeColor = _ft_col;
   formPCMemoryChecksum->PCMemoryChecksumProgressBar->Hint = mHint;
}

//---------------------------------------------------------------------------
void _fastcall PCMemoryChecksumProgress::SyncUpdateProgressColor(TColor bk, TColor ft)
{
    _bk_col =  bk;
    _ft_col =  ft;
    Synchronize(UpdateProgressColor);
}

//---------------------------------------------------------------------------
static int __stdcall cb_checksum_init(void *usr_arg, const char*image_name)
{

    PCMemoryChecksumProgress *t=(PCMemoryChecksumProgress *)usr_arg;
    AnsiString imageName = (AnsiString)image_name;
    int iLD = imageName.LastDelimiter("\\");
    imageName = imageName.SubString(iLD + 1, imageName.Length());
    GC_TRACE("<TBOOT1> cb_checksum_init(): imageName = %s", imageName);  

    t->mPrefix = "ChkSum(" + (AnsiString)imageName + ")";
    t->mHint = (AnsiString) image_name;
    t->SyncInitProgress();
    t->SyncUpdateProgressColor(clWhite, clFuchsia);

    return 0;
}
//---------------------------------------------------------------------------
static int __stdcall cb_checksum_progress(unsigned char finished_percentage, unsigned int finished_bytes, unsigned int total_bytes, void *usr_arg)
{

    PCMemoryChecksumProgress *t=(PCMemoryChecksumProgress *)usr_arg;
    t->SyncUpdateProgressPercentage(finished_percentage);

    return 0;
}

//---------------------------------------------------------------------------
void PCMemoryChecksumProgress::InitChecksumArgs(RomMemChecksumArg &checksumArg, RomMemChecksumResult &checksumResult)
{
	memset(&checksumArg, 0, sizeof(checksumArg));
	memset(&checksumResult, 0, sizeof(checksumResult));

	checksumArg.m_cb_rom_mem_checksum_init = cb_checksum_init;
	checksumArg.m_cb_rom_mem_checksum = cb_checksum_progress;   
	checksumArg.m_cb_rom_mem_checksum_arg = this;
	checksumArg.m_cb_rom_mem_checksum_init_arg = this;
	checksumArg.p_stopflag = NULL;
}


int PCMemoryChecksumProgress::VerifyHostBufferCksm(RomMemChecksumArg &checksumArg, RomMemChecksumResult &checksumResult)
{

    DL_HANDLE_T &dl_handle = *MainForm->Get_DlHandle(0);
    CksmVector::iterator it;
    unsigned short count;
    ROM_INFO rom[MAX_LOAD_SECTIONS];
    int ret = S_DONE;
    bool failFlag = false;
    

    GC_TRACE("[PCMemoryChecksumProgress] VerifyHostBufferCksm(): begin VerifyHostBufferCksm. index is %d", checksumArg.index);  
    if(frmMultiLoad->m_cksmVt.empty())
    {
        GC_TRACE("<MDLF> VerifyHostBufferCksm(): Cksm vector is empty, no golden value!");        
        return -1;
    }

    DLCALL(DL_GetCount(dl_handle, &count));
    DLCALL(DL_Rom_GetInfoAll(dl_handle, rom, MAX_LOAD_SECTIONS));

    for( it = frmMultiLoad->m_cksmVt.begin(); (it != frmMultiLoad->m_cksmVt.end()) /*&& (0xFFFFFFFF != (*it).second)*/; it++ )
    {
        int k = 0;
        for(; k < count; k++)
        {
           if(0 == (*it).first.compare(rom[k].name))
           {
               //ret = DL_VerifyROMMemBuf(dl_handle, rom[k].index, (*it).second);
			   checksumArg.index =  rom[k].index;
			   checksumArg.chksum = (*it).second;
			   ret = DL_VerifyROMMemBuf(dl_handle, &checksumArg, &checksumResult);
               if(S_DONE != ret)
               {
                   GC_TRACE("<MDLF> VerifyHostBufferCksm(): %s(0x%x) checksum verify fail!", (*it).first.c_str(), (*it).second);
                   failFlag = true;
                   break;
               }
               else
               {
                   GC_TRACE("<MDLF> VerifyHostBufferCksm(): %s(0x%x) checksum verify pass.", (*it).first.c_str(), (*it).second);
                   break;
               }
           }
        }

        /*if(k >= count)
        {
            GC_TRACE("<MDLF> VerifyHostBufferCksm(): Cannot find ROM info(%s)!", (*it).first.c_str());
            return -1;
        }*/
    }

    return (failFlag)?(-1):(0);
}

bool PCMemoryChecksumProgress::VerifyHostBufferBeforeStart()
{
	if(frmMultiLoad->IsHostBufferCksmFileExisted())
    {
        frmMultiLoad->LoadHostBufferStdCksm();
        RomMemChecksumArg checksumArg;
		RomMemChecksumResult checksumResult;
		InitChecksumArgs(checksumArg, checksumResult);
		 //md5 check sum
		if( frmMultiLoad->GetCheckSumVer())
		{
	       
	        formPCMemoryChecksum->MD5ChecksumLabel1->Caption ="Md5 check sum running";
		    if(frmMultiLoad->MD5Checksum())
		    {
		       formPCMemoryChecksum->MD5ChecksumLabel1->Caption ="Md5 check sum Pass";
		    }
			else 
		    {
		       formPCMemoryChecksum->MD5ChecksumLabel1->Caption ="Md5 check sum fail";
			   return false;
		    }
	    }
 
	    if(VerifyHostBufferCksm( checksumArg, checksumResult ))
        {
            Application->MessageBox( "The memory buffer for loading image verification fail!", "FAIL", MB_OK );
            return false;
        }
		return true;
    } 
	else
	{
		Application->MessageBox( "Can not find Checksum.ini file! Please use ChecksumGenerateTool to generate the Checksum.ini .", "FAIL", MB_OK );
	}
    
	
    return false;
}
 