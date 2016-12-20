//#include "RegistryWork.h"
//функции работы с реестром
//V 1.0
//LAST - 14 декабря 2008, 14:14

#include "stdafx.h"

#include "RegistryWork.h"
#include "Y:\DINBUFFER\DinBuffer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CRegWork2::CRegWork2()
{
	m_k=0;
}

CRegWork2::~CRegWork2()
{
	CloseKey();
}

void CRegWork2::CloseKey()
{
	if(m_k)
	{
		RegCloseKey(m_k);
	}
	m_k=0;
}

const HKEY CRegWork2::GetHKEY()
{
	return m_k;
}

bool CRegWork2::OpenKeyIn_HKEY_ROOT(
	const HKEY HKEY_ROOT,
	//HKEY& k_out,
	const char* pchKeyPath,
	bool bCreateIfNotExists,
	const DWORD dwdAccess
	)
{
	CloseKey();
	DWORD dwdRes=-1;

	if(bCreateIfNotExists)
	{
		//пытаемся создать новый ключ "HKEY_CURRENT_USER\KeyName"
		DWORD dwdDisposition=REG_CREATED_NEW_KEY|REG_OPENED_EXISTING_KEY;
		dwdRes=::RegCreateKeyEx(HKEY_ROOT,pchKeyPath,0,0,REG_OPTION_NON_VOLATILE,dwdAccess,0,&m_k,&dwdDisposition);
	}
	else
	{
		//просто открыть ключ
		dwdRes=::RegOpenKeyEx(HKEY_ROOT,pchKeyPath,0,dwdAccess,&m_k);
	}

	if(dwdRes!=ERROR_SUCCESS)
	{
		m_k=0;
	}

	return (dwdRes==ERROR_SUCCESS);
}

bool CRegWork2::DeleteKeyIn_HKEY_CURRENT_USER(const char* KeyPath,bool bKillSubKeysToo)
{

	HKEY HK=HKEY_CURRENT_USER;
	DWORD dwdRes=-1;


	CRegWork2 rw2;

	if(!rw2.OpenKeyIn_HKEY_CURRENT_USER(KeyPath,false))return false;

	if(bKillSubKeysToo)
	{
		//return ERROR_SUCCESS==::SHDeleteKey(rw2.GetHKEY(),KeyPath);
		return false;
	}
	else
	{
		return ERROR_SUCCESS==::RegDeleteKey(rw2.GetHKEY(),KeyPath);
	}
}

//удаление параметра ключа , открытого в данный момент
//с доступом KEY_SET_VALUE (или KEY_ALL_ACCESS)
bool CRegWork2::DeleteValueFromKeyOpenedWithRight__KEY_SET_VALUE(const char* pchValueName)
{
	return ERROR_SUCCESS==::RegDeleteValue(m_k,pchValueName);
}

//открываем ключ "HKEY_LOCAL_MACHINE\KeyName" в реестре
bool CRegWork2::OpenKeyIn_HKEY_LOCAL_MACHINE(const char* pchKeyPath,bool bCreateIfNotExists,const DWORD dwdAccess)
{
	CloseKey();
	return OpenKeyIn_HKEY_ROOT(HKEY_LOCAL_MACHINE,pchKeyPath,bCreateIfNotExists,dwdAccess);
}

//открываем ключ "HKEY_CURRENT_USER\KeyName" в реестре
bool CRegWork2::OpenKeyIn_HKEY_CURRENT_USER(const char* pchKeyPath,bool bCreateIfNotExists,const DWORD dwdAccess)
{
	CloseKey();
	return OpenKeyIn_HKEY_ROOT(HKEY_CURRENT_USER,pchKeyPath,bCreateIfNotExists,dwdAccess);
}


//определить размер буфера, необходимого для чтения данных из реестра
//(для строк также посчитается и ноль на конце, то есть для "abc" длина == 4)
bool CRegWork2::GetParameterValueLength(const char* pchValueName,DWORD& datalen_out)
{
	//достаём длину данных
	datalen_out=0;
	return (ERROR_SUCCESS==::RegQueryValueEx(m_k,pchValueName,0,REG_NONE,0,&datalen_out));
}



//создать список параметров ключа
void CRegWork2::EnumParamsInKey(CStringArray& csaParamsList)
{
	csaParamsList.RemoveAll();

	LONG res;
	char currParName[260+1];
	memset(currParName,0,sizeof(currParName));
	DWORD dwdNameLenToUse=0;

	for(DWORD dwd=0;;dwd++)
	{
		dwdNameLenToUse=sizeof(currParName)-1;
		res=RegEnumValue(m_k,dwd,currParName,&dwdNameLenToUse,0,0,0,0);

		switch(res)
		{
			default:
			case ERROR_MORE_DATA:
			case ERROR_NO_MORE_ITEMS:
			{
				return;
			}
			break;

			case ERROR_SUCCESS:
			{
				csaParamsList.Add(currParName);
			}
			break;
		}
	}
}

//получить список COM-портов в системе
void CRegWork2::st_EnumCOMports(
			CStringArray& csaCOMzList,
			bool bClearListBeforeWork,
			CComboBox* const pcbx,
			const CStringArray* const p_csaCOMzList_dontTouchComboIfNewListEqualToThis,
			bool* pbWasCha
			)
{
	if(pbWasCha)*pbWasCha=false;
	CRegWork2 rw2;
	if(rw2.OpenKeyIn_HKEY_LOCAL_MACHINE("HARDWARE\\DEVICEMAP\\SERIALCOMM"))
	{
		if(bClearListBeforeWork)csaCOMzList.RemoveAll();

		LONG res;
		char currParName[MAX_PATH+1];
		memset(currParName,0,sizeof(currParName));
		DWORD dwdNameLenToUse=0;

		BYTE currCOMName[100+1];
		memset(currCOMName,0,sizeof(currCOMName));
		DWORD dwdcurrCOMnameLen=0;

		DWORD dwdType=REG_SZ;

		CString csTemp;
		for(DWORD dwd=0;;dwd++)
		{
			dwdNameLenToUse=sizeof(currParName)-1;
			dwdcurrCOMnameLen=sizeof(currCOMName)-1;
			res=RegEnumValue(rw2.GetHKEY(),dwd,currParName,&dwdNameLenToUse,0,&dwdType,currCOMName,&dwdcurrCOMnameLen);

			if(res!=ERROR_SUCCESS)break;

			if(dwdcurrCOMnameLen && dwdcurrCOMnameLen<=sizeof(currCOMName)-1)
			{
				currCOMName[dwdcurrCOMnameLen]=0;
				csaCOMzList.Add(currCOMName);

				//сортируем по возрастанию
				if(csaCOMzList.GetSize()>1)
				{
					int i;
					for(i=csaCOMzList.GetSize()-1; i>=1; i--)
					{
						if(csaCOMzList[i]>=csaCOMzList[i-1])break;

						csTemp=csaCOMzList[i];
						csaCOMzList[i]=csaCOMzList[i-1];
						csaCOMzList[i-1]=csTemp;
					}
				}
			}
		}
	}

	//определяем похожесть на предыдущий список
	bool bIsLikeTo=true;
	if(!p_csaCOMzList_dontTouchComboIfNewListEqualToThis)
	{
		bIsLikeTo=false;
	}
	else
	{
		if(p_csaCOMzList_dontTouchComboIfNewListEqualToThis->GetSize()!=csaCOMzList.GetSize())
		{
			bIsLikeTo=false;
		}
		else
		{
			//размеры списков равны
			int i;
			for(i=0; i<csaCOMzList.GetSize(); i++)
			{
				if(csaCOMzList[i]!=(*p_csaCOMzList_dontTouchComboIfNewListEqualToThis)[i])
				{
					bIsLikeTo=false;
					break;
				}
			}
		}
	}

//p_csaCOMzList_dontTouchComboIfNewListEqualToThis->GetSize()

	//csaCOMzList - отсортирован по возрастанию

	if(pcbx && !bIsLikeTo)
	{
		if(pbWasCha)*pbWasCha=true;

		pcbx->ResetContent();

		//USB<->COM порты
		pcbx->AddString("USB");

		//компорты
		int i;
		for(i=0;i<csaCOMzList.GetSize();i++)
		{
			pcbx->AddString(csaCOMzList[i]);
		}
	}
/*
	//USB<->COM порты
	m_vec_PortsNamesList.Add("USB");

	//компорты
	for(w=0;w<18;w++)
	{
		txt.Format("COM%d",w+1);
		m_vec_PortsNamesList.Add(txt);
	}

	//суём в комбу
	pcbx=& ((CMainFrame*)theApp.m_pMainWnd)->m_IDR_COM_PORT_MENU;
	pcbx->ResetContent();
	for(int i=0;i<m_vec_PortsNamesList.GetSize();i++)
	{
		pcbx->InsertString(-1,m_vec_PortsNamesList[i]);
	}

	//подсвечиваем нужную надпись
	//(если не найдена - будет -1, то есть пустое окошко)
	pcbx->SetCurSel(pcbx->FindStringExact(0,m_Portz.m_csNameLastLoadedFromRegistry));

*/

}

//сохранение двоичных данных
//передаётся указатель на массив и размер массива байтах
bool CRegWork2::Data_BINARY_Set(const char* pchValueName,const BYTE* const bindata,const DWORD datalen)
{
	//пишем
	return (ERROR_SUCCESS==::RegSetValueEx(m_k,pchValueName,0,REG_BINARY,bindata,datalen));
}

bool CRegWork2::Data_BINARY_Get(const char* pchValueName,CDinBuffer& DinBuffer,DWORD dwdMaxSize)
{
	DinBuffer.SetDataLenToZero();

	s_RegBinaryBuffer regBuf;
	if(!Data_BINARY_Get(pchValueName,&regBuf,dwdMaxSize))return false;

	if(!DinBuffer.SetParams(regBuf.GetLen(),regBuf.GetLen(),regBuf.GetLen()))return false;

	return DinBuffer.AddDataToEnd(regBuf.GetBuff(),regBuf.GetLen());
}

/*	bool Data_BINARY_Set(const char* pchValueName,const CDinBuffer DinBuffer)
{
	s_RegBinaryBuffer regBuf;
	if(!regBuf.Initialize(DinBuffer.GetDataLen()))return false;

	memmove(regBuf.GetBuff(),DinBuffer.GetBuffer(),min(regBuf.GetLen(),DinBuffer.GetDataLen()));

	return Data_BINARY_Set(pchValueName,&regBuf);

}
*/

bool CRegWork2::Data_BINARY_Set(const char* pchValueName,const CDinBuffer& DinBuffer)
{
	s_RegBinaryBuffer regBuf;
	if(!regBuf.Initialize(DinBuffer.GetDataLen()))return false;

	memmove(regBuf.GetBuff(),DinBuffer.GetBuffer(),min(regBuf.GetLen(),DinBuffer.GetDataLen()));

	return Data_BINARY_Set(pchValueName,&regBuf);

}

bool CRegWork2::Data_BINARY_Set(const char* pchValueName,const s_RegBinaryBuffer* const pBuffer)
{
	if(!pBuffer->GetBuff())return false;
	//пишем
	return (ERROR_SUCCESS==::RegSetValueEx(m_k,pchValueName,0,REG_BINARY,pBuffer->GetBuff(),pBuffer->GetLen()));
}


bool CRegWork2::Data_BINARY_Get(const char* pchValueName,s_RegBinaryBuffer* pBuffer,DWORD dwdMaxSize)
{
	DWORD type=REG_BINARY;//тип параметра - bin

	pBuffer->Release();


	DWORD dwdLen=0;
	if(!GetParameterValueLength(pchValueName,dwdLen))return false;

	if(!dwdLen)
	{
		pBuffer->Initialize(0);
		return false;
	}

	if(dwdMaxSize)
	{
		dwdLen=min(dwdMaxSize,dwdLen);
	}

	if(!pBuffer->Initialize(dwdLen,true))return false;

	return ERROR_SUCCESS==::RegQueryValueEx(m_k,pchValueName,0,&type,pBuffer->GetBuff(),&dwdLen);
}

bool CRegWork2::Data_BINARY_Get(const char* pchValueName, BYTE* const bindata,const DWORD datalen)
{
	//читаем
	DWORD type=REG_BINARY;//тип параметра - bin
	DWORD dwdLen=datalen;
	return (ERROR_SUCCESS==::RegQueryValueEx(m_k,pchValueName,0,&type,bindata,&dwdLen));
}

//создать текстовый параметр в ключе реестра
bool CRegWork2::Data_TEXT_Set(const char* pchValueName,const char* chText,bool bIs_REG_EXPAND_SZ)
{
	DWORD type=(bIs_REG_EXPAND_SZ?REG_EXPAND_SZ:REG_SZ);//тип параметра - текст
	DWORD datalen=::strlen(chText);
	return ERROR_SUCCESS==::RegSetValueEx(m_k,pchValueName,0,type,(BYTE*)chText,datalen);
}

bool CRegWork2::Data_TEXT_Get(const char* pchValueName, CString& value,const char* pchDefaultVal,DWORD dwdMaxSize,bool bIs_REG_EXPAND_SZ)
{
	DWORD type=(bIs_REG_EXPAND_SZ?REG_EXPAND_SZ:REG_SZ);//тип параметра - текст

	value=pchDefaultVal;

	DWORD dwdLen=0;
	if(!GetParameterValueLength(pchValueName,dwdLen))return false;

	if(!dwdLen)
	{
		value="";
		return true;
	}

	if(dwdMaxSize)
	{
		dwdLen=min(dwdMaxSize,dwdLen);
	}

	//выделяем буфер для строки
	s_RegBinaryBuffer Buffer;
	if(!Buffer.Initialize(dwdLen,false))return false;


	if(ERROR_SUCCESS!=::RegQueryValueEx(m_k,pchValueName,0,&type,Buffer.GetBuff(),&dwdLen))
	{
		return false;
	}

	//на всякий случай ставим 0 в конце (хотя он там должен уже быть)
	Buffer.GetBuff()[Buffer.GetLen()-1]=0;

	value=(const char*)Buffer.GetBuff();

	return true;
}


//создать DWORD параметр в ключе реестра
bool CRegWork2::Data_DWORD_Set(const char* pchValueName,DWORD dwd)
{
	return ERROR_SUCCESS==::RegSetValueEx(m_k,pchValueName,0,REG_DWORD,(BYTE*)&dwd,sizeof(dwd));
}

//прочитать DWORD параметр из ключа реестра
bool CRegWork2::Data_DWORD_Get(const char* pchValueName,DWORD* pdwd,const DWORD dwdDefault)
{
	*pdwd=dwdDefault;
	DWORD type=REG_DWORD;//тип параметра - DWORD
	DWORD datalen=sizeof(*pdwd);
	return ERROR_SUCCESS==::RegQueryValueEx(m_k,pchValueName,0,&type,(BYTE*)(pdwd),&datalen);
}


//создать WORD параметр в ключе реестра
bool CRegWork2::Data_WORD_Set(const char* pchValueName,WORD w)
{
	DWORD dwd=(DWORD)w;
	return Data_DWORD_Set(pchValueName,dwd);
}

//прочитать WORD параметр из ключа реестра
bool CRegWork2::Data_WORD_Get(const char* pchValueName,WORD* pw,WORD wDefault)
{
	*pw=wDefault;
	DWORD dwd=0;
	bool bRes=Data_DWORD_Get(pchValueName,&dwd,(DWORD)wDefault);
	*pw=(WORD)dwd;
	return bRes;
}

//создать long параметр в ключе реестра
bool CRegWork2::Data_long_Set(const char* pchValueName,long l)
{
	DWORD dwd=(DWORD)l;
	return Data_DWORD_Set(pchValueName,dwd);
}

//прочитать long параметр из ключа реестра
bool CRegWork2::Data_long_Get(const char* pchValueName,long* pl,long lDefault)
{
	*pl=lDefault;
	DWORD dwd=0;
	bool bRes=Data_DWORD_Get(pchValueName,&dwd,(DWORD)lDefault);
	*pl=(long)dwd;
	return bRes;
}

//создать int параметр в ключе реестра
bool CRegWork2::Data_int_Set(const char* pchValueName,int n)
{
	DWORD dwd=(DWORD)n;
	return Data_DWORD_Set(pchValueName,dwd);
}

//прочитать int параметр из ключа реестра
bool CRegWork2::Data_int_Get(const char* pchValueName,int* pn,int nDefault)
{
	*pn=nDefault;
	DWORD dwd=0;
	bool bRes=Data_DWORD_Get(pchValueName,&dwd,(DWORD)nDefault);
	*pn=(int)dwd;
	return bRes;
}

//создать BYTE параметр в ключе реестра
bool CRegWork2::Data_BYTE_Set(const char* pchValueName,BYTE by)
{
	return Data_BINARY_Set(pchValueName,&by,1);
}

//прочитать BYTE параметр из ключа реестра
bool CRegWork2::Data_BYTE_Get(const char* pchValueName,BYTE* pby,BYTE byDefault)
{
	*pby=byDefault;

	DWORD dwdLenToCheck=0;
	if(!GetParameterValueLength(pchValueName,dwdLenToCheck))return false;


	if(dwdLenToCheck==1 || dwdLenToCheck==4) // 4 - если где то успел записать 4 байта. Потом убрать
	{
		*pby=byDefault;

		s_RegBinaryBuffer Buffer;
		bool bRes=Data_BINARY_Get(pchValueName,&Buffer,1);
		if(bRes)
		{
			*pby=Buffer.GetBuff()[0];
		}

		return bRes;
	}

	return false;
}

//создать bool параметр в ключе реестра
bool CRegWork2::Data_bool_Set(const char* pchValueName,bool b)
{
	BYTE by=(b?1:0);
	return Data_BYTE_Set(pchValueName,by);
}

//прочитать bool параметр из ключа реестра
bool CRegWork2::Data_bool_Get(const char* pchValueName,bool* pb,bool bDefault)
{
	*pb=bDefault;
	BYTE by=0;
	bool bRes=Data_BYTE_Get(pchValueName,&by,bDefault?1:0);
	*pb=(by?true:false);
	return bRes;
}

//	CHARFORMAT cf;
//создать CHARFORMAT параметр в ключе реестра
bool CRegWork2::Data_CHARFORMAT_Set(const char* pchValueName,const CHARFORMAT* pCF)
{
	return Data_BINARY_Set(pchValueName,(BYTE*)pCF, sizeof(*pCF));
}

//прочитать CHARFORMAT параметр из ключа реестра
bool CRegWork2::Data_CHARFORMAT_Get(const char* pchValueName,CHARFORMAT* pCF,const CHARFORMAT* pCF_Default)
{
	if(pCF_Default)
	{
		*pCF=*pCF_Default;
	}
	else
	{
		memset(pCF,0,sizeof(*pCF));
	}

	DWORD dwdLen=0;
	if(!GetParameterValueLength(pchValueName,dwdLen))return false;
	if(dwdLen!=sizeof(*pCF))return false;
	
	s_RegBinaryBuffer buf;
	if(!buf.Initialize(sizeof(*pCF),true))return false;

	bool bRes=Data_BINARY_Get(pchValueName,&buf,sizeof(*pCF));

	if(bRes)
	{
		memmove(pCF,buf.GetBuff(),min(sizeof(*pCF),buf.GetLen()));
	}

	return bRes;
}

//создать LOGFONT параметр в ключе реестра
bool CRegWork2::Data_LOGFONT_Set(const char* pchValueName,const LOGFONT* plogfont)
{
	return Data_BINARY_Set(pchValueName,(BYTE*)plogfont, sizeof(*plogfont));
}

//прочитать LOGFONT параметр из ключа реестра
bool CRegWork2::Data_LOGFONT_Get(const char* pchValueName,LOGFONT* plogfont,const LOGFONT* plogfont_Default)
{
	if(plogfont_Default)
	{
		*plogfont=*plogfont_Default;
	}
	else
	{
		memset(plogfont,0,sizeof(*plogfont));
	}

	DWORD dwdLen=0;
	if(!GetParameterValueLength(pchValueName,dwdLen))return false;
	if(dwdLen!=sizeof(*plogfont))return false;
	
	s_RegBinaryBuffer buf;
	if(!buf.Initialize(sizeof(*plogfont),true))return false;

	bool bRes=Data_BINARY_Get(pchValueName,&buf,sizeof(*plogfont));

	if(bRes)
	{
		memmove(plogfont,buf.GetBuff(),min(sizeof(*plogfont),buf.GetLen()));
	}

	return bRes;
}

//создать WINDOWPLACEMENT параметр в ключе реестра
bool CRegWork2::Data_WINDOWPLACEMENT_Set(const char* pchValueName,const WINDOWPLACEMENT* pWINDOWPLACEMENT)
{
	return Data_BINARY_Set(pchValueName,(BYTE*)pWINDOWPLACEMENT, sizeof(*pWINDOWPLACEMENT));
}

//прочитать WINDOWPLACEMENT параметр из ключа реестра
bool CRegWork2::Data_WINDOWPLACEMENT_Get(const char* pchValueName,WINDOWPLACEMENT* pWINDOWPLACEMENT)
{
	memset(pWINDOWPLACEMENT,0,sizeof(*pWINDOWPLACEMENT));

	DWORD dwdLen=0;
	if(!GetParameterValueLength(pchValueName,dwdLen))return false;
	if(dwdLen!=sizeof(*pWINDOWPLACEMENT))return false;
	
	s_RegBinaryBuffer buf;
	if(!buf.Initialize(sizeof(*pWINDOWPLACEMENT),true))return false;

	bool bRes=Data_BINARY_Get(pchValueName,&buf,sizeof(*pWINDOWPLACEMENT));
/*
//корректироовка

			if(dwdWPMSize==sizeof(wpm) && _R_ReadRegKeyBINParam(k,_cs "wpm",dwdWPMSize,(BYTE*)(&wpm)))
			{
				long wsW=::GetSystemMetrics(SM_CXSCREEN);
				long wsH=::GetSystemMetrics(SM_CYSCREEN);
				long W=_WPMwid(wpm);
				long H=_WPMhig(wpm);
				//проверка на выход за границы экрана
				if(_WPMlef(wpm)<0){_WPMlef(wpm)=0;_WPMrig(wpm)=W-1;}
				if(_WPMrig(wpm)>wsW){_WPMrig(wpm)=wsW-1;_WPMlef(wpm)=wsW-1-W+1;}
				if(_WPMtop(wpm)<0){_WPMtop(wpm)=0;_WPMbot(wpm)=H-1;}
				if(_WPMbot(wpm)>wsH){_WPMbot(wpm)=wsH-1;_WPMtop(wpm)=wsH-1-H+1;}
				wpm.showCmd=loadshowcmd;
				pwnd->SetWindowPlacement(&wpm);
			}

*/
	if(bRes)
	{
		memmove(pWINDOWPLACEMENT,buf.GetBuff(),min(sizeof(*pWINDOWPLACEMENT),buf.GetLen()));
	}

	return bRes;
}


