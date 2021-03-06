// ToolStripMultiProgressBarButton.h : 在按钮被按下的时候，在ToolStripButton上显示多个进度条(最大4个)
// Copyright: ⓒ INF Technologies Ltd., 2010. All Rights Reserved
// Revision:
// 2011.06.29 XiaoMing Zheng  
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
	using namespace System::Windows::Forms::VisualStyles;
	
	//************************************************************************
	//Name: 	ToolStripMultiProgressBarButton
	//Author: 	Zheng XM (2011/6/29)
	//Modify: 	
	//Return:  	
	//Description: 在按钮被按下的时候，在ToolStripButton上显示进度(最大4个)
	//			注意：
	//				此按钮的CheckOnClick属性被设为true,外部不得修改此属性；
	//			使用方法
	//				1）从Form的Designer上的ToolStrip中的可添加控件选项中选择ToolStripCheckBox即可直接添加
	//					或，在ToolBar上添加ToolStripButton后，将其从ToolStripButton改为[YourNameSpace]::ITLToolStripMultiProgressBarButton
	//				2）设置此按钮的基础Image
	//					this->m_toolStripMultiProgressBarButtonTest->Image = gcnew Bitmap(....);
	//				3）在此按钮被按下的Event函数中
	//					在大循环前设置第一个ProgressBar的最大最小数值
	//						this->m_toolStripMultiProgressBarButtonTest->MinimumProgressValue[0] = 0;
	//						this->m_toolStripMultiProgressBarButtonTest->MaximumProgressValue[0] = YYYYY;	//如果不知道的话可设个适当的数值
	//					在各个小循环处理前设置下属ProgressBar最大最小值
	//						this->m_toolStripMultiProgressBarButtonTest->MinimumProgressValue[1] = 0;
	//						this->m_toolStripMultiProgressBarButtonTest->MaximumProgressValue[1] = YYYYY;	//如果不知道的话可设个适当的数值
	//					根据情况变更ProgressBar的个数
	//						this->m_toolStripMultiProgressBarButtonTest->NumberOfProgressBar = 2;
	//					根据情况分别设置各个ProgressBar的进度值
	//						this->m_toolStripMultiProgressBarButtonTest->CurrentProgressValue[0] = XXXXX;	//如果超出最大值MainMaximumProgressValue，会自动显示XXXXX/MaximumProgressValue的余
	//						this->m_toolStripMultiProgressBarButtonTest->CurrentProgressValue[1] = XXXXX;	//如果超出最大值SubMaximumProgressValue，会自动显示XXXXX/MaximumProgressValue的余
	//							或
	//						this->m_toolStripMultiProgressBarButtonTest->Increament(0);		//递增显示，Progress到头后再度从0开始显示
	//						this->m_toolStripMultiProgressBarButtonTest->Increament(1);		//递增显示，Progress到头后再度从0开始显示
	//						Application::DoEvents();	//处理界面Event
	//						if(!oToolStripMultiProgressBarButton->Checked)
	//						{//再次被按起来时视为Cancel处理
	//							bCancel = true;
	//							break;
	//						}
	//					处理结束后
	//						this->m_toolStripMultiProgressBarButtonTest->Checked = false;	/弹出按钮
	//
	[DisplayName(L"ToolStripMultiProgressBarButton")]
	[ToolStripItemDesignerAvailabilityAttribute(ToolStripItemDesignerAvailability::ToolStrip)]
	public ref class ToolStripMultiProgressBarButton : public System::Windows::Forms::ToolStripButton
	{
	public:
		ToolStripMultiProgressBarButton()
		{
			Init(1);
		}

		ToolStripMultiProgressBarButton(UInt16 nNumberOfProgressBar)	//ProgressBar的个数,必须>0。否则无意义（与ToolStripButton的动作完全一样）
		{
			Init(nNumberOfProgressBar);
		}

	protected:
		~ToolStripMultiProgressBarButton(void)
		{
		}

#pragma region 公开函数
	public:
		//************************************************************************
		//Name: 	Increament
		//Author: 	Zheng XM (2011/6/15)
		//Modify: 	
		//Return:  	
		//Description: 递增CurrentProgressValue
		//				当CurrentProgressValue要要超越最大值时，自动回到最小值，
		//				利用此可以将此ProgressBar设置为循环显示进度。适用用预先得不到最大值的处理时
		//		
		void Increament(UInt16 nNo)
		{
			this->CurrentProgressValue[nNo]++;
		}

		//************************************************************************
		//Name: 	Increament
		//Author: 	Zheng XM (2011/6/15)
		//Modify: 	
		//Return:  	true:继续处理，false:Cancel
		//Description: 处理界面操作，包括接受Cancel操作
		//		
		bool DoEvents()
		{
			bool	bRcd = true;
			Application::DoEvents();	//处理界面
			if(!this->Checked)
			{//当此控件不是Checked的时候，被视为Cancel
				bRcd = false;
			}
			return bRcd;
		}
#pragma endregion 公开函数

#pragma region Properties
	public:
#pragma region Override Properties
		//此Control的Image，用于外部调用，此代码中必须使用__super::Image来设置
		virtual property System::Drawing::Image^	Image
		{
			System::Drawing::Image^ get() override
			{
				return __super::Image;	//返回当前的Image
			}
			void set(System::Drawing::Image^ oValue) override
			{
				this->m_oOriginalImage = oValue;
				__super::Image = oValue;
			}
		}
#pragma endregion Override Properties
		//ProgressBar的个数
		[Browsable(true), Localizable(false)]
		property UInt16 NumberOfProgressBar
		{
			UInt16 get()
			{
				return this->m_nNumberOfProgressBar;
			}
			void set(UInt16 nValue)	//允许途中修改ProgressBar的个数
			{
				if(nValue<=0)
				{
					nValue = 1;	//至少要有1个ProgressBar
				}
				if(nValue>this->MaxNumberOfProgressBar)
				{
					nValue = this->MaxNumberOfProgressBar;	//最多为内部规定最大ProgressBar个数
				}
				this->m_nNumberOfProgressBar = nValue;
				this->SetDataSize(this->m_anMinimumProgressValue, nValue, 0);	//各个ProgressBar的进度最小值
				this->SetDataSize(this->m_anMaximumProgressValue, nValue, 100);//各个ProgressBar的进度最大值
				this->SetDataSize(this->m_anCurrentProgressValue, nValue, 0);	//各个ProgressBar的当前的进度值
				this->SetDataSize(this->m_anLastDrawnProgressWidth, nValue, 0);	//记录最近一次各个ProgressBar的绘制宽度,用于规避重复绘制一个宽度的//当前的进度值
				this->ShowProgressBar();
			}
		}

		//ProgressBar的最大数量
		[Browsable(false), Localizable(false)]
		property UInt16 MaxNumberOfProgressBar
		{
			UInt16 get()
			{
				return ToolStripMultiProgressBarButton::m_nMaxNumberOfProgressBar;
			}
		}

		//各个ProgressBar的最小值
		[Browsable(false), Localizable(false)]
		property Int32 MinimumProgressValue[UInt16]	//指定ProgressBar的号码
		{
			Int32 get(UInt16 nNo)
			{
				Int32 nRcd = 0;
				if(this->IsRightProgressBarNo(nNo))
				{//指定号码正常
					if((this->m_anMinimumProgressValue!=nullptr)&&(nNo<UInt16(this->m_anMinimumProgressValue->Length)))
					{
						nRcd = this->m_anMinimumProgressValue[nNo];
					}
				}
				return nRcd;
			}
			void set(UInt16 nNo, Int32 nValue)
			{
				if(this->IsRightProgressBarNo(nNo))
				{//指定号码正常
					if((this->m_anMinimumProgressValue!=nullptr)&&(nNo<UInt16(this->m_anMinimumProgressValue->Length)))
					{
						this->m_anMinimumProgressValue[nNo] = nValue;
						this->LastDrawnProgressWidth[nNo] = 0;	//清除绘制记录，以便重新绘制
					}
				}
			}
		}

		//各个ProgressBar的最大值
		[Browsable(false), Localizable(false)]
		property Int32 MaximumProgressValue[UInt16]	//指定ProgressBar的号码
		{
			Int32 get(UInt16 nNo)
			{
				Int32 nRcd = 0;
				if(this->IsRightProgressBarNo(nNo))
				{//指定号码正常
					if((this->m_anMaximumProgressValue!=nullptr)&&(nNo<UInt16(this->m_anMaximumProgressValue->Length)))
					{
						nRcd = this->m_anMaximumProgressValue[nNo];
					}
				}
				return nRcd;
			}
			void set(UInt16 nNo, Int32 nValue)
			{
				if(this->IsRightProgressBarNo(nNo))
				{//指定号码正常
					if((this->m_anMaximumProgressValue!=nullptr)&&(nNo<UInt16(this->m_anMaximumProgressValue->Length)))
					{
						this->m_anMaximumProgressValue[nNo] = nValue;
						this->LastDrawnProgressWidth[nNo] = 0;	//清除绘制记录，以便重新绘制
					}
				}
			}
		}

		//各个ProgressBar的当前的进度值
		[Browsable(false), Localizable(false)]
		property Int32 CurrentProgressValue[UInt16]	//指定ProgressBar的号码
		{
			Int32 get(UInt16 nNo)
			{
				Int32 nRcd = 0;
				if(this->IsRightProgressBarNo(nNo))
				{//指定号码正常
					if((this->m_anCurrentProgressValue!=nullptr)&&(nNo<UInt16(this->m_anCurrentProgressValue->Length)))
					{
						nRcd = this->m_anCurrentProgressValue[nNo];
					}
				}
				return nRcd;
			}
			void set(UInt16 nNo, Int32 nValue)
			{
				if(this->IsRightProgressBarNo(nNo)&&
					(this->m_anCurrentProgressValue!=nullptr)&&
					(this->m_nNumberOfProgressBar==UInt16(this->m_anCurrentProgressValue->Length)))
				{//必要的数据都正常
					//检查最大最下值
					if(this->MinimumProgressValue[nNo]>=this->MaximumProgressValue[nNo])
					{//恢复到0--100
						this->MinimumProgressValue[nNo] = 0;
						this->MaximumProgressValue[nNo] = 100;
					}
					//调整此值,不得超出最大最小值
					if(nValue<this->MinimumProgressValue[nNo])
					{
						nValue = this->MinimumProgressValue[nNo];
					}
					if(nValue>this->MaximumProgressValue[nNo])
					{
						nValue = nValue%this->MaximumProgressValue[nNo];	//取其余
					}

					Int32	nLastProgressValue = this->m_anCurrentProgressValue[nNo];
					this->m_anCurrentProgressValue[nNo] = nValue;
					if(this->Checked)
					{//只在按下的时候处理Progress
						if(this->m_anCurrentProgressValue[nNo]!=nLastProgressValue)
						{//为了减少不必要的画面操作
							Int32	nWidth = this->ContentRectangle.Width*this->m_anCurrentProgressValue[nNo]/this->MaximumProgressValue[nNo];
							if(nWidth!=this->LastDrawnProgressWidth[nNo])
							{//为了减少不必要的画面操作
								this->LastDrawnProgressWidth[nNo] = nWidth;	//记录下此次绘制的宽度
								this->ShowProgressBar();
							}
						}
					}
				}
			}
		}
	protected:
		//各个ProgressBar的最近一次各个ProgressBar的绘制宽度(内部专用，不对外公开此属性)
		[Browsable(false), Localizable(false)]
		property Int32 LastDrawnProgressWidth[UInt16]	//指定ProgressBar的号码
		{
			Int32 get(UInt16 nNo)
			{
				Int32 nRcd = 0;
				if((this->m_anLastDrawnProgressWidth!=nullptr)&&(nNo<UInt16(this->m_anLastDrawnProgressWidth->Length)))
				{
					nRcd = this->m_anLastDrawnProgressWidth[nNo];
				}
				return nRcd;
			}
			void set(UInt16 nNo, Int32 nValue)
			{
				if((this->m_anLastDrawnProgressWidth!=nullptr)&&(nNo<UInt16(this->m_anLastDrawnProgressWidth->Length)))
				{
					this->m_anLastDrawnProgressWidth[nNo] = nValue;
				}
			}
		}
#pragma endregion Properties

#pragma region 内部变量
	private:
		Int16			m_nNumberOfProgressBar;		//ProgressBar的个数
		array<Int32>^	m_anMinimumProgressValue;	//各个ProgressBar的进度最小值
		array<Int32>^	m_anMaximumProgressValue;	//各个ProgressBar的进度最大值
		array<Int32>^	m_anCurrentProgressValue;	//各个ProgressBar的当前的进度值
		array<Int32>^	m_anLastDrawnProgressWidth;	//记录最近一次各个ProgressBar的绘制宽度,用于规避重复绘制一个宽度的

		System::Drawing::Image^	m_oOriginalImage;			//按钮上的原始Image
		System::Drawing::Imaging::ImageAttributes^ m_oImageAttributes;	//透明用属性

		static UInt16	m_nMaxNumberOfProgressBar = 3;	//ProgressBar的最大数量
#pragma endregion 内部变量

#pragma region Override Functions
	protected:
		//************************************************************************
		//Name: 	OnCheckedChanged
		//Author: 	Zheng XM (2011/6/27)
		//Modify: 	
		//Return:  	
		//Description: 
		//		
		virtual void OnCheckedChanged(EventArgs^ e) override
		{
			__super::OnCheckedChanged(e);
			if(!this->Checked)
			{
				this->Image = gcnew Bitmap(this->m_oOriginalImage);	//回复原始状态下的ButtonImage
				//重新显示
				this->Invalidate();
			}
		}
#pragma endregion Override Functions

#pragma region 内部函数
	private:
		//************************************************************************
		//Name: 	Init
		//Author: 	Zheng XM (2011/6/30)
		//Modify: 	
		//Return:  	
		//Description: 内部数据初始化
		//		
		void Init(UInt16 nNo)
		{
			this->NumberOfProgressBar = nNo;

			this->m_oOriginalImage = nullptr;	//按钮上的原始Image

			//相关属性的设置
			this->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->CheckOnClick = true;	//按动后，保持其状态

			//半透明用参数的设置
			System::Drawing::Imaging::ColorMatrix^ oColorMatrix = gcnew System::Drawing::Imaging::ColorMatrix();
			oColorMatrix->Matrix00 = 1.0F;
			oColorMatrix->Matrix11 = 1.0F;
			oColorMatrix->Matrix22 = 1.0F;
			oColorMatrix->Matrix33 = 0.7F;
			oColorMatrix->Matrix44 = 1.0F;
			this->m_oImageAttributes = gcnew System::Drawing::Imaging::ImageAttributes();
			this->m_oImageAttributes->SetColorMatrix(oColorMatrix);
		}

		//************************************************************************
		//Name: 	SetDataSize
		//Author: 	Zheng XM (2011/6/30)
		//Modify: 	
		//Return:  	
		//Description: 设置指定数据的Size，如果有现有数据存在，不初始化其值，如果是扩大Size值初始化被扩充的部分
		//		
		void SetDataSize(array<Int32>^% oData, UInt16 nNo, Int32 nInitValue)
		{
			if(nNo>0)
			{
				int	nInitStart = 0;
				int	nInitEnd = nNo;
				if(oData==nullptr)
				{
					oData = gcnew array<Int32>(nNo);		//各个ProgressBar的进度最小值//进度最小值
				}
				else
				{
					nInitStart = oData->Length;
					Array::Resize(oData,nNo);	//保留原有数据
				}
				//初始化被扩充的部分
				for(int i=nInitStart; i<nInitEnd; i++)
				{
					oData[i] = nInitValue;
				}
			}
		}

		//************************************************************************
		//Name: 	IsRightProgressBarNo
		//Author: 	Zheng XM (2011/6/29)
		//Modify: 	
		//Return:  	false:所给的ProgressBar的号码不合格
		//Description: 
		//		
		bool IsRightProgressBarNo(UInt16 nNo)
		{
			bool	bRcd = false;

			if((nNo>=0)&&(nNo<this->NumberOfProgressBar))
			{//指定号码正常
				bRcd = true;
			}

			return bRcd;
		}

		//************************************************************************
		//Name: 	ShowProgressBar
		//Author: 	Zheng XM (2011/6/30)
		//Modify: 	
		//Return:  	
		//Description: 显示ProgressBar
		//		
		void ShowProgressBar()
		{
			if(this->m_oOriginalImage!=nullptr)
			{
				//以下在this->m_oOriginalImage上绘制this->m_nNumberOfProgressBar个ProgressBar
				Int32 nOrignalHeight = this->m_oOriginalImage->Height;
				Int32 nOrignalWidth = this->m_oOriginalImage->Width;
				Int32 nAverageHeight =nOrignalHeight / this->m_nNumberOfProgressBar;	//平均最大高度
				Int32 nRestHeight = nOrignalHeight % this->m_nNumberOfProgressBar;		//剩余高度
				if(nAverageHeight>=4)
				{//否则，无法绘制
					//计算各个ProgressBar的高度
					array<Int32>^ anHeight = gcnew array<Int32>(this->m_nNumberOfProgressBar);
					for(int i=0; i<this->m_nNumberOfProgressBar; i++)
					{
						anHeight[i] = nAverageHeight + ((i<nRestHeight)?1:0);	//需要把富余的部分分配给前几个ProgressBar
					}
					//预备当前所有的ProgressBar
					array<System::Drawing::Image^>^	oProgressBarImages = gcnew array<System::Drawing::Image^>(this->m_nNumberOfProgressBar);
					for(UInt16 i=0; i<this->m_nNumberOfProgressBar; i++)
					{
						oProgressBarImages[i] = this->MakeProgressBarImage(this->LastDrawnProgressWidth[i],anHeight[i]);
					}

					//将oProgressBarImages以透明的形式画在此按钮的Image之上
					__super::Image = gcnew Bitmap(this->m_oOriginalImage);
					Graphics^ oGraphics = Graphics::FromImage(this->Image);
					int	nY = 0;
					for(int i=0; i<this->m_nNumberOfProgressBar; i++)
					{
						oGraphics->DrawImage(oProgressBarImages[i],System::Drawing::Rectangle(0,nY,nOrignalWidth,anHeight[i]),0,0,this->Image->Width,anHeight[i],System::Drawing::GraphicsUnit::Pixel,this->m_oImageAttributes);
						nY += anHeight[i];
					}
					//重新显示
					this->Invalidate();
				}
			}
		}

		//************************************************************************
		//Name: 	MakeProgressBarImage
		//Author: 	Zheng XM (2011/6/28)
		//Modify: 	
		//Return:  	
		//Description: 按照指定的高宽，画一ProgressBarImage
		//		
		System::Drawing::Image^ MakeProgressBarImage(Int32 nWidth,		//ProgressBar进度的宽度
													Int32 nHeight)		//ProgressBar高度
		{
			System::Drawing::Image^	oRcd = gcnew Bitmap(this->ContentRectangle.Width,nHeight);
			Graphics^ oGraphics =  Graphics::FromImage(oRcd);
			VisualStyleRenderer^ oVisualStyleRenderer = gcnew VisualStyleRenderer(System::Windows::Forms::VisualStyles::VisualStyleElement::ProgressBar::Chunk::Normal);
			System::Drawing::Rectangle oRect = System::Drawing::Rectangle(0,0,nWidth,nHeight);
			oVisualStyleRenderer->DrawBackground(oGraphics,oRect);
			return oRcd;
		}
#pragma endregion 内部函数

	};
}