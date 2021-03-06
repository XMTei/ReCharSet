// TreeListViewTextFileNode.h : ITLTreeListView的项目数据类(文本文件名专用)
// Copyright: ⓒ INF Technologies Ltd., 2011. All Rights Reserved
// Revision:
// 2011.09.02 XiaoMing Zheng  
//
#pragma once

#include "TreeListViewFileNode.h"
#include "TextEncoding.h"

namespace ITLStdLib
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Data;
	using namespace System::Diagnostics;
	using namespace System::Drawing;
	using namespace System::Globalization;
	using namespace System::IO;
	using namespace System::Text;
	using namespace System::Windows::Forms;
	using namespace System::Windows::Forms::Design;
	
	//************************************************************************
	//Name: 	TreeListViewTextFileNode
	//Author: 	Zheng XM (2011/8/31)
	//Modify: 	
	//Return:  	
	//Description: TreeListView的项目数据类(文本文件专用)
	//
	public ref class TreeListViewTextFileNode : public TreeListViewFileNode
	{
	#pragma region 内部用类
	public:
		//************************************************************************
		//Name: 	TextFileEncodingSubItem
		//Author: 	Zheng XM (2011/9/3)
		//Modify: 	
		//Return:  	
		//Description: TreeListViewFileNode的附属项目，记述Text文件的解码信息
		//				
		ref class TextFileEncodingSubItem : public ListViewItem::ListViewSubItem
		{
		#pragma region 此类的初始化和善后处理
		public:
			TextFileEncodingSubItem(String^ strFileName)
			{
				this->m_oEncoding = nullptr;
				if (File::Exists(strFileName))
				{//此文件存在
					array<Byte>^ oTextData = System::IO::File::ReadAllBytes(strFileName);
					if((oTextData!=nullptr)&&(oTextData->Length>0))
					{
						this->EncodingInfo = ITLStdLib::ITLMLang::DetectInputCodepage(oTextData);
					}
					else
					{//数据不存在（文件为空）
						this->Text = L"Empty File";
						this->BackColor = System::Drawing::Color::RosyBrown;
					}
				}
				else
				{//文件不存在
					this->Text = L"File Not Found";
					this->BackColor = System::Drawing::Color::LightCyan;
				}
			}
			~TextFileEncodingSubItem()
			{
			}
		#pragma endregion 此类的初始化和善后处理

		#pragma region 公开函数
		public:
			//************************************************************************
			//Name: 	Clone
			//Author: 	Zheng XM (2011/8/31)
			//Modify: 	
			//Return:  	
			//Description: 复制此类，所有值变量都可被复制，而参照变量也被复制
			//
			TextFileEncodingSubItem^ Clone()
			{
				TextFileEncodingSubItem^ oRcd = cli::safe_cast<TextFileEncodingSubItem^>(this->MemberwiseClone());
				return oRcd;
			}
		#pragma endregion 公开函数

		#pragma region Override函数
			//************************************************************************
			//Name: 	Equals
			//Author: 	Zheng XM (2010/11/18)
			//Modify: 	
			//Return:  	
			//Description: 判断与同类的内容是否相等（同）
			//			
			virtual bool Equals( Object^ obj ) override
			{
				bool bRcd = false;

				if (( obj != nullptr) && (this->GetType() == obj->GetType()) )
				{	//给定类存在，且是同类
					TextFileEncodingSubItem^ oSubItem = cli::safe_cast<TextFileEncodingSubItem^>(obj);
					if((oSubItem!=nullptr)&&
						(oSubItem->EncodingInfo==this->m_oEncoding))
					{//相同数据
						bRcd = true;
						if((this->m_oEncoding!=nullptr)&&(!this->m_oEncoding->Equals(oSubItem->EncodingInfo)))
						{
							bRcd = false;
						}
					}
				}
				return bRcd;
			}
		#pragma endregion Override函数

		#pragma region Properties
		public:
			//相关Text文件的解码信息
			[Browsable(false), Localizable(false)]
			property Encoding^ EncodingInfo
			{
				Encoding^ get()
				{
					return this->m_oEncoding;
				}
				void set(Encoding^ oValue)
				{
					if(oValue==nullptr)
					{//使用系统Default
						oValue = Encoding::Default;
						//用背景颜色表明被迫使用Default
						this->BackColor = System::Drawing::Color::Pink;
					}
					else
					{
						this->BackColor = System::Drawing::Color::White;
					}
					this->m_oEncoding = oValue;
					this->Text = String::Format(L"{0}({1})",this->m_oEncoding->EncodingName,this->m_oEncoding->CodePage);
				}
			}
		#pragma endregion Properties

		#pragma region 内部变量
		private:
			Encoding^	m_oEncoding;	//相关Text文件的解码信息
		#pragma endregion 内部变量
		};
	#pragma endregion 内部用类

	#pragma region 此类的初始化和善后处理
	public:
		TreeListViewTextFileNode()
		{
			this->FileInformation = nullptr;
		}
		TreeListViewTextFileNode(System::IO::FileInfo^ oFileInfo) : TreeListViewFileNode(oFileInfo)
		{
		}
		~TreeListViewTextFileNode()
		{
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region 公开函数
	public:
		//************************************************************************
		//Name: 	SaveAsUnicodeFile
		//Author: 	Zheng XM (2011/9/15)
		//Modify: 	
		//Return:  	false:cancel,不继续转换
		//Description: 以Unicode形式保存此文件
		//			
		bool SaveAsUnicodeFile()
		{
			bool	bRcd = true;	//没被取消转换
			String^	strAllText = this->TextFromFile;

			if((strAllText!=nullptr)&&(!strAllText->Equals(String::Empty)))
			{//Encording sub Item存在
				try
				{
					//保存成Unicode文件
					File::WriteAllText(this->m_oFileInformation->FullName, strAllText, Encoding::Unicode);
				}
				catch(System::Exception^ e)
				{
					String^ strMessageTile = Application::ProductName;
					String^ strMessage = String::Format(L"{0}\nContinue?",e->Message);

					if(System::Windows::Forms::MessageBox::Show(strMessage,strMessageTile,MessageBoxButtons::YesNo,MessageBoxIcon::Asterisk)!=System::Windows::Forms::DialogResult::Yes)
					{
						bRcd = false;
					}
				}
			}
			return bRcd;
		}
	#pragma endregion 公开函数

	#pragma region Override函数
	public:
		//************************************************************************
		//Name: 	Equals
		//Author: 	Zheng XM (2011/8/31)
		//Modify: 	
		//Return:  	
		//Description: 判断与同类的内容是否相等（同）
		//			所谓相同是指此两Position指向同一位置
		//			
		//virtual bool Equals( Object^ obj ) override
		//{
		//	bool bRcd = false;

		//	if (( obj != nullptr) && (this->GetType() == obj->GetType()) )
		//	{	//给定类存在，且是同类
		//		ITLTreeListViewNode^ oTreeListViewNode = cli::safe_cast<ITLTreeListViewNode^>(obj);
		//		if((oTreeListViewNode!=nullptr)&&
		//			(oTreeListViewNode->CharacterBaseNode==this->m_oCharacterBaseNode))
		//		{
		//			bRcd = true;
		//		}
		//	}
		//	return bRcd;
		//}
	#pragma endregion Override函数

	#pragma region Properties
	#pragma region Override Properties
	public:
		//FileInfo
		[Browsable(false), Localizable(false)]
		virtual property System::IO::FileInfo^ FileInformation
		{
			System::IO::FileInfo^ get() override
			{
				return this->m_oFileInformation;
			}
			void set(System::IO::FileInfo^ oFileInfo) override
			{
				this->m_oFileInformation = oFileInfo;
				//添加EncodingInfo的Sub Item
				if(this->m_oFileInformation!=nullptr)
				{
					this->Text = this->m_oFileInformation->Name;	//显示内容
					this->BackColor = System::Drawing::Color::White;
					this->ForeColor = System::Drawing::SystemColors::WindowText;
					ListViewItem::ListViewSubItem^	oSubItem = gcnew TextFileEncodingSubItem(this->m_oFileInformation->FullName);	//附属Encording项目
					this->SubItems->Add(oSubItem);
				}
				else
				{
					this->Text = L"Non File";	//显示内容
					this->BackColor = System::Drawing::Color::Gray;
					//空的SubItem
					ListViewItem::ListViewSubItem^	oSubItem = gcnew ListViewItem::ListViewSubItem(this, L"");
					this->SubItems->Add(oSubItem);
				}
			}
		}
	#pragma endregion Override Properties
	public:
		//此文件中的Text(用于Preview以及输出Unicode文本文件)
		property String^ TextFromFile
		{
			String^ get()
			{
				String^	strRcd = String::Empty;
				if((this->m_oFileInformation->Exists)&&
					(this->SubItems!=nullptr)&&
					(this->SubItems->Count>1)&&
					(this->SubItems[1]->GetType()==TreeListViewTextFileNode::TextFileEncodingSubItem::typeid))
				{//Encording sub Item存在
					TreeListViewTextFileNode::TextFileEncodingSubItem^ oEncordingSubItem = cli::safe_cast<TreeListViewTextFileNode::TextFileEncodingSubItem^>(this->SubItems[1]);
					//用当前的Encoding读文件
					strRcd = File::ReadAllText(this->m_oFileInformation->FullName, oEncordingSubItem->EncodingInfo);
				}
				return strRcd;
			}
		}
		//解码此文本文件时发现的无法解码的数据
		property String^ FailedDacodedData
		{
			String^ get()
			{
				String^	strRcd = String::Empty;
				if((this->FileInformation->Exists)&&//此文件存在
					(this->SubItems!=nullptr)&&
					(this->SubItems->Count>1)&&
					(this->SubItems[1]->GetType()==ITLStdLib::TreeListViewTextFileNode::TextFileEncodingSubItem::typeid))
				{//Encording sub Item存在
					TreeListViewTextFileNode::TextFileEncodingSubItem^ oEncordingSubItem = cli::safe_cast<ITLStdLib::TreeListViewTextFileNode::TextFileEncodingSubItem^>(this->SubItems[1]);
					array<Byte>^ oTextData = System::IO::File::ReadAllBytes(this->FileInformation->FullName);
					if((oTextData!=nullptr)&&(oTextData->Length>0))
					{
						//System::Text::DecoderReplacementFallback^	oDecoderReplacementFallback = gcnew System::Text::DecoderReplacementFallback(L"?");
						//System::Text::DecoderReplacementFallbackBuffer^	oDecoderReplacementFallbackBuffer = gcnew System::Text::DecoderReplacementFallbackBuffer(oDecoderReplacementFallback);
						//System::Text::Encoding^			oEncoding = System::Text::Encoding::GetEncoding(oEncordingSubItem->EncodingInfo->CodePage,gcnew System::Text::EncoderExceptionFallback(),oDecoderReplacementFallback);
						//String^ strTemp = oEncoding->GetString(oTextData);
						//System::Text::DecoderFallback^ oDecodeFallback = oEncoding->DecoderFallback;
						EncoderReportFallback^	oEncoderReplacementFallback = gcnew EncoderReportFallback();
						DecoderReportFallback^	oDecoderReplacementFallback = gcnew DecoderReportFallback();
						System::Text::Encoding^			oEncoding = System::Text::Encoding::GetEncoding(oEncordingSubItem->EncodingInfo->CodePage,oEncoderReplacementFallback,oDecoderReplacementFallback);
						strRcd = oEncoding->GetString(oTextData);
						strRcd = oDecoderReplacementFallback->FallbakBuffer->UnknownDataList->Report;
					}
				}
				return strRcd;
			}
		}
	protected:
	#pragma endregion Properties

	#pragma region 内部变量
	#pragma endregion 内部变量
	};
}