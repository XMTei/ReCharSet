// ITLShell32.h : 包装Windows的Shell32的API的类
// Copyright: ⓒ INF Technologies Ltd., 2011. All Rights Reserved
// Revision:
// 2011.09.7 XiaoMing Zheng  
//

#pragma once

#include	<Winuser.h>				//Winuser API
#include	<Shellapi.h>			//Shell API

#pragma comment(lib, "User32.lib")	//Winuser API
#pragma comment(lib, "Shell32.lib")	//Shell API

namespace ITLStdLib
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Globalization;
	using namespace System::IO;
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
	public ref class Shell32 abstract sealed
	{
	public:
		//************************************************************************
		//Name: 	GetFileIcon
		//Author: 	Zheng XM (2011/9/7)
		//Modify:
		//Return:  	
		//Description: 获取给定文件的系统显示用Icon
		//
		static System::Drawing::Icon^ GetFileIcon(String^ strFileName,	//文件名。如果是".扩充名"的时候，获取此扩充名的Icon
													bool bSmall,		//true:获取小Icon（16X16）。否则获取大Icon（32X32）
													bool bLinkOverlay)	//true:获取Shortcut式Icon
		{
			System::Drawing::Icon^ oRcd = nullptr;

			if(strFileName!=nullptr)//&&(File::Exists(strFileName)))
			{//文件存在.利用SHGetFileInfo()获取Icon
				//把文件名转换为wchar_t*
				wchar_t*	lpstrTemp = static_cast<wchar_t*>(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(strFileName).ToPointer());
				DWORD		dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
				SHFILEINFO	sSHFileInfo;
				UINT		nFlag = SHGFI_ICON | SHGFI_USEFILEATTRIBUTES;
				if (bSmall)
				{//获取小Icon
					nFlag |= SHGFI_SMALLICON;
				}
				else
				{//获取大Icon
					nFlag |= SHGFI_LARGEICON;
				}
				if(bLinkOverlay)
				{
					nFlag |= SHGFI_LINKOVERLAY;
				}
				DWORD	dwRcd;

				dwRcd = ::SHGetFileInfo(lpstrTemp,dwFileAttributes, &sSHFileInfo, sizeof(SHFILEINFO),nFlag);

				//复制一个.NET的Icon
				oRcd = cli::safe_cast<System::Drawing::Icon^>(System::Drawing::Icon::FromHandle(IntPtr(sSHFileInfo.hIcon))->Clone());

				//Destroy所获取的Icon
				::DestroyIcon(sSHFileInfo.hIcon);

				System::Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr(lpstrTemp));
			}

			return oRcd;
		}

		//************************************************************************
		//Name: 	GetFolderIcon
		//Author: 	Zheng XM (2011/9/7)
		//Modify:
		//Return:  	
		//Description: 获取给定文件夹的系统显示用Icon
		//
		static System::Drawing::Icon^ GetFolderIcon(String^ strFilePath,//Folder的Path。随便一个值(例如：L"dic")，可以获取默认的Folder的Icon
													bool bSmall,	//true:获取小Icon（16X16）。否则获取大Icon（32X32）
													bool bOpen)		//true:打开的Icon
		{
			System::Drawing::Icon^ oRcd = nullptr;

			if(strFilePath!=nullptr)//&&(Directory::Exists(strFilePath)))
			{//文件存在.利用SHGetFileInfo()获取Icon
				//把文件名转换为wchar_t*
				wchar_t*	lpstrTemp = static_cast<wchar_t*>(System::Runtime::InteropServices::Marshal::StringToHGlobalUni(strFilePath).ToPointer());
				DWORD		dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
				SHFILEINFO	sSHFileInfo;
				UINT		nFlag = SHGFI_ICON | SHGFI_USEFILEATTRIBUTES;
				if (bSmall)
				{//获取小Icon
					nFlag |= SHGFI_SMALLICON;
				}
				else
				{//获取大Icon
					nFlag |= SHGFI_LARGEICON;
				}
				if(bOpen)
				{
					nFlag |= SHGFI_OPENICON;
				}
				DWORD	dwRcd;

				dwRcd = ::SHGetFileInfo(lpstrTemp,dwFileAttributes, &sSHFileInfo, sizeof(SHFILEINFO),nFlag);

				//复制一个.NET的Icon
				oRcd = cli::safe_cast<System::Drawing::Icon^>(System::Drawing::Icon::FromHandle(IntPtr(sSHFileInfo.hIcon))->Clone());

				//Destroy所获取的Icon
				::DestroyIcon(sSHFileInfo.hIcon);

				System::Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr(lpstrTemp));
			}

			return oRcd;
		}
	};
}
