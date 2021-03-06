// ToolStirpItemsStatusContainer.h : 记录一系列ToolStripItem，根据需要将它们的Enable属性设置为false(Push)，或回复被Push之前的Enable的状态
// Copyright: ⓒ INF Technologies Ltd., 2011. All Rights Reserved
// Revision:
// 2011.07.04 XiaoMing Zheng  
//
#pragma once

namespace ITLStdLib
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Globalization;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;
	using namespace System::IO;
	using namespace System::Windows::Forms;
	using namespace System::Runtime::InteropServices;

	//************************************************************************
	//Name: 	ITLToolStirpItemsStatusContainer
	//Author: 	Zheng XM (2011/7/4)
	//Modify: 	
	//Return:  	
	//Description: 记录一系列ToolStripItem，根据需要将它们的Enable属性设置为false(Push)，或回复被Push之前的Enable的状态
	//				用于在操作一个ToolStripItem时禁止使用其他相关的ToolStripItem
	//			
	public ref class ITLToolStirpItemsStatusContainer
	{
	#pragma region 内部类
	public:
		ref class ToolStripItemStatus
		{
		public:
			ToolStripItemStatus(ToolStripItem^ oToolStripItem)
			{
			#ifdef _DEBUG
				Debug::Assert(oToolStripItem!=nullptr, L"oToolStripItem不得为null");
			#endif	//_DEBUG
				this->m_oToolStripItem = oToolStripItem;
				this->m_bStatusBackup = oToolStripItem->Enabled;
			}
			~ToolStripItemStatus()
			{
			}
		#pragma region 公开函数
		public:
			//************************************************************************
			//Name: 	Disable
			//Author: 	Zheng XM (2011/7/4)
			//Modify: 	
			//Return:  	
			//Description: 把所管理的ToolStripItem的Enable设为false;
			//
			void Disable()
			{
				this->m_oToolStripItem->Enabled = false;
			}

			//************************************************************************
			//Name: 	Reset
			//Author: 	Zheng XM (2011/7/4)
			//Modify: 	
			//Return:  	
			//Description: 把所管理的ToolStripItem的Enable设为Disable之前的状态;
			//
			void Reset()
			{
				this->m_oToolStripItem->Enabled = this->m_bStatusBackup;
			}
		#pragma endregion 公开函数
		#pragma region 内部变量
		private:
			ToolStripItem^	m_oToolStripItem;
			bool			m_bStatusBackup;
		#pragma endregion 内部变量
		};
	#pragma endregion 内部类

	#pragma region 此类的初始化和善后处理
	public:
		ITLToolStirpItemsStatusContainer(array<ToolStripItem^>^ oToolStripItems)
		{
			if((oToolStripItems!=nullptr)&&(oToolStripItems->Length>0))
			{
				this->m_oToolStripItemStatus = gcnew array<ToolStripItemStatus^>(oToolStripItems->Length);
				for(int i=0; i<oToolStripItems->Length; i++)
				{
					this->m_oToolStripItemStatus[i] = gcnew ToolStripItemStatus(oToolStripItems[i]);
				}
			}
			else
			{
				this->m_oToolStripItemStatus = nullptr;
			}
		}
		~ITLToolStirpItemsStatusContainer()
		{
		}
	#pragma endregion 此类的初始化和善后处理
	#pragma region 公开函数
	public:
		//************************************************************************
		//Name: 	Disable
		//Author: 	Zheng XM (2011/7/4)
		//Modify: 	
		//Return:  	
		//Description: 把所管理的ToolStripItem的Enable设为false;
		//
		void Disable()
		{
			if(this->m_oToolStripItemStatus != nullptr)
			{
				for each(ToolStripItemStatus^ oItem in this->m_oToolStripItemStatus)
				{
					oItem->Disable();
				}
			}
		}

		//************************************************************************
		//Name: 	Reset
		//Author: 	Zheng XM (2011/7/4)
		//Modify: 	
		//Return:  	
		//Description: 把所管理的ToolStripItem的Enable设为Disable之前的状态;
		//
		void Reset()
		{
			if(this->m_oToolStripItemStatus != nullptr)
			{
				for each(ToolStripItemStatus^ oItem in this->m_oToolStripItemStatus)
				{
					oItem->Reset();
				}
			}
		}
	#pragma endregion 公开函数

	#pragma region Properties
	public:
	#pragma endregion Properties

	#pragma region 内部变量
	private:
		array<ToolStripItemStatus^>^	m_oToolStripItemStatus;	//所有ToolStripItem极其状态
	#pragma endregion 内部变量
	};
}