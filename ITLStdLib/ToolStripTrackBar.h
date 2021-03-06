// ToolStripTrackBar.h : ToolStrip上用的TrackBar控件
// Copyright: ⓒ INF Technologies Ltd., 2010. All Rights Reserved
// Revision:
// 2010.07.15 XiaoMing Zheng  
// 2011.09.19 XiaoMing Zheng  改为ITLStdLib的泪
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
	//Name: 	ToolStripTrackBar
	//Author: 	Zheng XM (2010/7/15)
	//Modify: 	
	//Return:  	
	//Description: 把Form中的TrackBar控件用到ToolStrip中。
	//		使用方法如下：
	//			1）
	//				this->toolStripTrackBarVolume = (gcnew ToolStripTrackBar());	//在Form中宣布此控件
	//				this->toolStripPlayMIDI->Items->Insert(3,this->toolStripTrackBar);	//添加到Form中的ToolStrip（toolStripNightView）控件中
	//				this->toolStripTrackBarVolume->TrackBar->Enabled = false;	//直接调用NumericUpDown的例子
	//				this->toolStripTrackBarVolume->ValueChanged += gcnew EventHandler(this, &Form1::OnTrackBarChanged);	//添加Event
	//			2）Designer的ToolStrip中的可添加控件选项中选择ToolStripTrackBar即可直接添加，之后通过Property来实施设置（包括Evnet）
	//
	[DisplayName(L"ToolStripTrackBar")]
	[ToolStripItemDesignerAvailabilityAttribute(ToolStripItemDesignerAvailability::ToolStrip)]
	public ref class ToolStripTrackBar : public System::Windows::Forms::ToolStripControlHost
	{
	public:
		ToolStripTrackBar(void):System::Windows::Forms::ToolStripControlHost(gcnew System::Windows::Forms::TrackBar())
		{
			//this->TrackBar->MinimumSize = System::Drawing::Size(80,20);
		}

	protected:
		~ToolStripTrackBar(void)
		{
		}

	public:
#pragma region Properties
		//TrackBar的控件
		[BindableAttribute(true)] 
		property System::Windows::Forms::TrackBar^ TrackBar
		{
			System::Windows::Forms::TrackBar^ get()
			{
				return static_cast<System::Windows::Forms::TrackBar^>(Control);
				//return cli::safe_cast<System::Windows::Forms::TrackBar^>(Control);
			}
		}
#pragma endregion Properties

#pragma region Override Functions
	protected:
		//************************************************************************
		//Name: 	OnSubscribeControlEvents
		//Author: 	Zheng XM (2010/4/21)
		//Modify: 	
		//Return:  	
		//Description: ホストしているTrackBarのイベントをサブスクライブする
		//		
		virtual void OnSubscribeControlEvents(System::Windows::Forms::Control^ control) override
		{
			__super::OnSubscribeControlEvents(control);
			this->TrackBar->ValueChanged += gcnew EventHandler(this, &ToolStripTrackBar::OnValueChanged);
		}

		//************************************************************************
		//Name: 	OnUnsubscribeControlEvents
		//Author: 	Zheng XM (2010/4/21)
		//Modify: 	
		//Return:  	
		//Description: ホストしているTrackBarのイベントをアンサブスクライブする
		//		
		virtual void OnUnsubscribeControlEvents(System::Windows::Forms::Control^ control) override
		{
			__super::OnUnsubscribeControlEvents(control);
			this->TrackBar->ValueChanged -= gcnew EventHandler(this, &ToolStripTrackBar::OnValueChanged);
		}
#pragma endregion Override Functions

#pragma region Events
	public:
		EventHandler^ ValueChanged;
#pragma endregion Events

#pragma region Event的处理
	private:
		//************************************************************************
		//Name: 	OnValueChanged
		//Author: 	Zheng XM (2010/7/14)
		//Modify: 	
		//Return:  	
		//Description: ValueChangedイベントの処理
		//
		void OnValueChanged(System::Object^ sender, EventArgs^ e)
		{
			if(sender->GetType()==System::Windows::Forms::TrackBar::typeid)
			{
				if (this->ValueChanged != nullptr)
				{
					this->ValueChanged(this, e);
				}
			}
		}
#pragma endregion Event的处理

	};
}