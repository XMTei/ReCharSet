// TextFileEncordingInfo.h : 描述Text文件的Encording（CharSet）的类
// Copyright: ⓒ INF Technologies Ltd., 2011. All Rights Reserved
// Revision:
// 2011.08.03 XiaoMing Zheng  
//

#pragma once

namespace ITLStdLib
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Data;
	using namespace System::Diagnostics;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Windows::Forms;
	using namespace System::Windows::Forms::Design;

	//************************************************************************
	//Name: 	OnTestGetTextCodec
	//Author: 	Zheng XM (2011/8/20)
	//Modify:
	//Return:  	
	//Description: 测试获取指定文件的CodePage（Encoding）
	//
	public ref class TextFileEncordingInfo
	{
	public:
		TextFileEncordingInfo()
		{
			this->m_strFileDiscription = String::Empty;
			this->m_strFileExtention = String::Empty;
		}
		TextFileEncordingInfo(String^ strFileDiscription, String^ strFileExtention)
		{
			this->m_strFileDiscription = strFileDiscription;
			this->FileExtention = strFileExtention;
		}

	#pragma region Properties
	public:
		//文件说明
		[Browsable(false), Localizable(false)]
		property String^ FileDiscription
		{
			String^ get()
			{
				return this->m_strFileDiscription;
			}
			void set(String^ strValue)
			{
				if(strValue!=nullptr)
				{
					this->m_strFileDiscription = strValue;
				}
				else
				{
					this->m_strFileDiscription = String::Empty;
				}
			}
		}
		//文件扩充名
		[Browsable(false), Localizable(false)]
		property String^ FileExtention
		{
			String^ get()
			{
				return this->m_strFileExtention;
			}
			void set(String^ strValue)
			{
				if(strValue!=nullptr)
				{
					this->m_strFileExtention = strValue;
				}
				else
				{
					this->m_strFileExtention = String::Empty;
				}
			}
		}
		//FileDialog的Filter（Ex. L"txt files (*.txt)|*.txt"）
		[Browsable(false),Localizable(false)]
		property String^ FileDialogFilter
		{
			String^ get()
			{
				return String::Format(L"{0} (*{1})|*{1}",this->m_strFileDiscription,this->m_strFileExtention);
			}
		}
	#pragma endregion Properties

	#pragma region 内部变量
	private:
		String^					m_strFileDiscription;
		String^					m_strFileExtention;
	#pragma endregion 内部变量
	};
}

