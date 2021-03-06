// ITLMLang.h : 包装Windows的MLang的API的类
// Copyright: ⓒ INF Technologies Ltd., 2011. All Rights Reserved
// Revision:
// 2011.08.2 XiaoMing Zheng  
//

#pragma once

#include	<MLang.h>			//IE中的MLang
#pragma comment(lib, "Ole32.lib")	//COM or OLE， CoCreateInstance()等等)

namespace ITLStdLib
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Globalization;
	using namespace System::Runtime::InteropServices;
	using namespace System::Text;
	using namespace System::Windows::Forms;

	//************************************************************************
	//Name: 	ITLMLang
	//Author: 	Zheng XM (2011/8/2)
	//Modify: 	
	//Return:  
	//Description: 包装MLang.DLL的类
	//	
	public ref class ITLMLang abstract sealed
	{
	public:
		//************************************************************************
		//Name: 	DetectInputCodepage
		//Author: 	Zheng XM (2011/8/4)
		//Modify:
		//Return:  	
		//Description: 获取给定TextData(array<Btye>)的文字码信息
		//				内部调用IMultiLanguage2::DetectInputCodepage
		//
		//		注意：MS提供的DetectInputCodepage()并不精准，目前已知的问题有
		//		1）	区分不开Japanese (JIS)(50220),Japanese (JIS-Allow 1 byte Kana)(50221),Japanese (JIS-Allow 1 byte Kana - SO/SI)(50222)，全部被认为是50220
		//			但是，不知为何，50221和50222的Text都可以用50220的Encoding来解码。这样的话就不成问题了。
		//		2）	区分不开Japanese (Shift-JIS)(932)，Japanese (Mac)(10001)，全部被认为是932
		//			应该不是问题。
		//		3）	Japanese (JIS 0208-1990 and 0212-1990)（20932）被认为是Japanese (EUC)（51932）
		//			应该不是问题。
		//		4）	IBM EBCDIC (Japanese katakana)（20290）别误认为是GB2312（936）
		//
		static Encoding^ DetectInputCodepage(array<Byte>^ oTextData)
		{
			Encoding^ oRcd = nullptr;

			if((oTextData!=nullptr)&&(oTextData->Length>0))
			{//有数据存在
				//利用IMultiLanguage2::DetectInputCodepage找的该Text的最适当的CodePage
				IMultiLanguage2*	lpMlang;
				DetectEncodingInfo	sInfo = {0};
				int					nLength = oTextData->Length;
				int					nCount = 1;
				//MIMECPINFO		sCodepageinfo={0};

				HRESULT hr = S_OK;
				hr =CoInitialize(NULL); // init COM
				if(SUCCEEDED(hr))
				{
					hr = CoCreateInstance(CLSID_CMultiLanguage, NULL, CLSCTX_INPROC_SERVER, IID_IMultiLanguage2,(void **)&lpMlang);
					if(SUCCEEDED(hr))
					{
						char*	lpstrTemp = new char[nLength];
						System::Runtime::InteropServices::Marshal::Copy(oTextData,0,IntPtr(lpstrTemp),nLength);

						hr = lpMlang->DetectInputCodepage(0,0, lpstrTemp, &nLength, &sInfo, &nCount);
						if(SUCCEEDED(hr))
						{
							oRcd = Encoding::GetEncoding(sInfo.nCodePage);	//转换为
							//hr = mlang->GetCodePageInfo(sInfo.nCodePage, sInfo.nLangID, &sCodepageinfo);
							//if(SUCCEEDED(hr))
							//{
							//	sCodepageinfo.SetAs(codepageinfo.wszDescription);
							//}
						}
						delete lpstrTemp;
					}

					lpMlang->Release();
				}
				CoUninitialize();
			}

			return oRcd;
		}
	};
}
