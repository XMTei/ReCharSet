// TreeListViewFileNode.h : ITLTreeListView的项目数据类(文件名专用)
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
	//Name: 	TreeListViewFileNode
	//Author: 	Zheng XM (2011/8/31)
	//Modify: 	
	//Return:  	
	//Description: TreeListViewFileNode的项目数据类
	//
	public ref class TreeListViewFileNode : public TreeListViewNode
	{
	#pragma region 内部用类
	public:
		//可以在此定义此文件名相关的子项目。Ex. Size，Date等等，具体可参考TreeListViewTextFileNode::TextFileEncodingSubItem
	#pragma endregion 内部用类

	#pragma region 此类的初始化和善后处理
	public:
		TreeListViewFileNode()
		{
			this->FileInformation = nullptr;
		}
		TreeListViewFileNode(System::IO::FileInfo^ oFileInfo)
		{
			this->FileInformation = oFileInfo;
		}
		~TreeListViewFileNode()
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
	#pragma region Virtual Properties
	public:
		//FileInfo
		[Browsable(false), Localizable(false)]
		virtual property System::IO::FileInfo^ FileInformation
		{
			System::IO::FileInfo^ get()
			{
				return this->m_oFileInformation;
			}
			void set(System::IO::FileInfo^ oFileInfo)
			{
				this->m_oFileInformation = oFileInfo;
			}
		}
	#pragma endregion Virtual Properties
	protected:
	#pragma endregion Properties

	#pragma region 内部变量
	protected:
		System::IO::FileInfo^		m_oFileInformation;	//FileInfo
	#pragma endregion 内部变量
	};
}