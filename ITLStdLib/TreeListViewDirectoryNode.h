// TreeListViewDirectoryNode.h : ITLTreeListView的项目数据类(文件夹专用)
// Copyright: ⓒ INF Technologies Ltd., 2011. All Rights Reserved
// Revision:
// 2011.09.02 XiaoMing Zheng  
//
#pragma once

#include "TreeListViewNode.h"

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
	//Name: 	TreeListViewDirectoryNode
	//Author: 	Zheng XM (2011/9/3)
	//Modify: 	
	//Return:  	
	//Description: TreeListViewDirectoryNode的项目数据类
	//
	public ref class TreeListViewDirectoryNode : public TreeListViewNode
	{
	#pragma region 内部用类
	public:
	#pragma endregion 内部用类

	#pragma region 此类的初始化和善后处理
	public:
		TreeListViewDirectoryNode()
		{
			this->m_oSubTreeListViewNodes = gcnew TreeListViewNodeCollection();//此TreeListViewNode之下的TreeListViewNode
			this->DirectoryInformation = nullptr;
		}
		TreeListViewDirectoryNode(System::IO::DirectoryInfo^ oDirectoryInfo)
		{
			this->m_oSubTreeListViewNodes = gcnew TreeListViewNodeCollection();//此TreeListViewNode之下的TreeListViewNode
			this->DirectoryInformation = oDirectoryInfo;
		}
		~TreeListViewDirectoryNode()
		{
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region 公开函数
	public:
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
	public:
		//DirectoryInfo
		[Browsable(false), Localizable(false)]
		property System::IO::DirectoryInfo^ DirectoryInformation
		{
			System::IO::DirectoryInfo^ get()
			{
				return this->m_oDirectoryInformation;
			}
			void set(System::IO::DirectoryInfo^ oValue)
			{
				this->m_oDirectoryInformation = oValue;
				if(this->m_oDirectoryInformation!=nullptr)
				{
					this->Text = this->m_oDirectoryInformation->Name;	//显示内容
					this->BackColor = System::Drawing::Color::White;
				}
				else
				{
					this->Text = L"Non Directory";	//显示内容
					this->BackColor = System::Drawing::Color::Gray;
				}
				//空的SubItem
				ListViewItem::ListViewSubItem^	oSubItem = gcnew ListViewItem::ListViewSubItem(this, L"");
				this->SubItems->Add(oSubItem);
			}
		}
	protected:
	#pragma endregion Properties

	#pragma region 内部变量
	protected:
		System::IO::DirectoryInfo^	m_oDirectoryInformation;	//DirectoryInfo
	#pragma endregion 内部变量
	};
}