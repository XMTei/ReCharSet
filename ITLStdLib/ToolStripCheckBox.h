// ToolStripCheckBox.h : ToolStrip上用的TrackBar控件
// Copyright: ⓒ INF Technologies Ltd., 2010. All Rights Reserved
// Revision:
// 2011.08.26 XiaoMing Zheng  
//
#pragma once

namespace ITLStdLib
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace System::Windows::Forms::Design;
	
	//************************************************************************
	//Name: 	ToolStripCheckBox
	//Author: 	Zheng XM (2011/8/26)
	//Modify: 	
	//Return:  	
	//Description: 把Form中的CheckBox控件用到ToolStrip中。
	//			备注：using ITLStdLib的程序中，从Form的Designer上的ToolStrip中的可添加控件选项中选择ToolStripCheckBox即可直接添加
	//
	[DisplayName(L"ToolStripCheckBox")]
	[ToolStripItemDesignerAvailabilityAttribute(ToolStripItemDesignerAvailability::ToolStrip|ToolStripItemDesignerAvailability::MenuStrip|ToolStripItemDesignerAvailability::ContextMenuStrip)]
	public ref class ToolStripCheckBox : public System::Windows::Forms::ToolStripControlHost
	{
	public:
		ToolStripCheckBox(void):System::Windows::Forms::ToolStripControlHost(gcnew System::Windows::Forms::CheckBox())
		{
			this->AutoSize = true;
			this->CheckBox->AutoSize = true;
			this->CheckBox->AutoEllipsis = true;
			this->CheckBox->UseVisualStyleBackColor = true;
			this->CheckBox->Text = L"CheckBox1";
		}

	protected:
		~ToolStripCheckBox(void)
		{
		}

	public:
	#pragma region Properties
		//CheckBox的控件
		[DesignerSerializationVisibility(DesignerSerializationVisibility::Content)]
		property System::Windows::Forms::CheckBox^ CheckBox
		{
			System::Windows::Forms::CheckBox^ get()
			{
				return static_cast<System::Windows::Forms::CheckBox^>(this->Control);
			}
		}
	#pragma endregion Properties

	#pragma region Override Functions
	public:
		//************************************************************************
		//Name: 	GetPreferredSize
		//Author: 	Zheng XM (2011/8/26)
		//Modify: 	
		//Return:  	
		//Description: Retrieves the size of a rectangular area into which a control can be fitted
		//		
		virtual System::Drawing::Size GetPreferredSize(System::Drawing::Size /*constrainingSize*/) override
		{
			return this->CheckBox->Size;
		}
	protected:
		//************************************************************************
		//Name: 	OnSubscribeControlEvents
		//Author: 	Zheng XM (2011/8/26)
		//Modify: 	
		//Return:  	
		//Description: ホストしているCheckBoxのイベントをサブスクライブする
		//		
		virtual void OnSubscribeControlEvents(System::Windows::Forms::Control^ control) override
		{
			__super::OnSubscribeControlEvents(control);
			this->CheckBox->CheckedChanged += gcnew EventHandler(this, &ToolStripCheckBox::OnCheckedChanged);
		}

		//************************************************************************
		//Name: 	OnUnsubscribeControlEvents
		//Author: 	Zheng XM (2011/8/26)
		//Modify: 	
		//Return:  	
		//Description: ホストしているTrackBarのイベントをアンサブスクライブする
		//		
		virtual void OnUnsubscribeControlEvents(System::Windows::Forms::Control^ control) override
		{
			__super::OnUnsubscribeControlEvents(control);
			this->CheckBox->CheckedChanged -= gcnew EventHandler(this, &ToolStripCheckBox::OnCheckedChanged);
		}
	#pragma endregion Override Functions

	#pragma region Events
	public:
		EventHandler^ CheckedChanged;
	#pragma endregion Events

	#pragma region Event的处理
	private:
		//************************************************************************
		//Name: 	OnChecedChanged
		//Author: 	Zheng XM (2011/8/26)
		//Modify: 	
		//Return:  	
		//Description: ChecedChangedイベントの処理
		//
		void OnCheckedChanged(System::Object^ sender, EventArgs^ e)
		{
			if(sender->GetType()==System::Windows::Forms::CheckBox::typeid)
			{
				if (this->CheckedChanged != nullptr)
				{
					this->CheckedChanged(this, e);
				}
			}
		}
	#pragma endregion Event的处理

	};
}