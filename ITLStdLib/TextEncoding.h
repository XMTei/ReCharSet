// TextEncoding.h : 文本文件编码(Encode:String-->Bytes)/解码(Decode:Bytes-->String)所需类
// Copyright: ⓒ INF Technologies Ltd., 2011. All Rights Reserved
// Revision:
// 2011.09.13 XiaoMing Zheng  
//
//	使用方法：
//		ITLStdLib::EncoderReportFallback^	oEncoderReplacementFallback = gcnew ITLStdLib::EncoderReportFallback();
//		ITLStdLib::DecoderReportFallback^	oDecoderReplacementFallback = gcnew ITLStdLib::DecoderReportFallback();
//		System::Text::Encoding^			oEncoding = System::Text::Encoding::GetEncoding(oEncordingSubItem->EncodingInfo->CodePage,oEncoderReplacementFallback,oDecoderReplacementFallback);
//		String^ strTemp = oEncoding->GetString(oTextData);	//此时oDecoderReplacementFallback->FallbakBuffer->UnknownDataList会有数据。 当String--〉Bytes时，oEncoderReplacementFallback中会有数据
//		String^ strReport = oDecoderReplacementFallback->FallbakBuffer->UnknownDataList->Report;
//

#pragma once

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
	//Name: 	EncodingUnknownDataBase
	//Author: 	Zheng XiaoMing (2011/9/14)
	//Modify: 	
	//Return:  	
	//Description:编码(Encode:String-->Bytes)/解码(Decode:Bytes-->String)时遇到的不认识的数据的基类
	//			
	public ref class EncodingUnknownDataBase abstract
	{
	#pragma region 此类的初始化和善后处理
	public:
		EncodingUnknownDataBase(int nIndex)
		{
			this->m_nIndex = nIndex;
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region Properties
	#pragma region Virtual Properties
	public:
		//数据报告（字符串方式），需要在派生类中完善
		[Browsable(false), Localizable(false)]
		virtual property String^ Report
		{
			String^ get()
			{
				return String::Empty;
			}
		}
	#pragma endregion Virtual Properties
	public:
		//该数据的位置
		[Browsable(false), Localizable(false)]
		property int Index
		{
			int get()
			{
				return this->m_nIndex;
			}
		}
	#pragma endregion Properties

	#pragma region 内部变量
	private:
		int					m_nIndex;	//该数据的位置
	#pragma endregion 内部变量
	};

	//************************************************************************
	//Name: 	EncoderUnknownCharacter
	//Author: 	Zheng XiaoMing (2011/9/14)
	//Modify: 	
	//Return:  	
	//Description:记录编码(Encode:String-->Bytes)时遇到的当前Encoding下不认识的字符
	//			
	public ref class EncoderUnknownCharacter : public EncodingUnknownDataBase
	{
	#pragma region 此类的初始化和善后处理
	public:
		EncoderUnknownCharacter(wchar_t cChar, int nIndex) : EncodingUnknownDataBase(nIndex)
		{
			this->m_cCharacterHigh = cChar;
			this->m_cCharacterLow = 0;
			this->m_bUseCharacterLow = false;
		}
		EncoderUnknownCharacter(wchar_t cCharHigh, wchar_t cCharLow, int nIndex) : EncodingUnknownDataBase(nIndex)
		{
			this->m_cCharacterHigh = cCharHigh;
			this->m_cCharacterLow = cCharLow;
			this->m_bUseCharacterLow = true;
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region Properties
	#pragma region Override Properties
	public:
		//数据报告（字符串方式），需要在派生类中完善
		[Browsable(false), Localizable(false)]
		virtual property String^ Report
		{
			String^ get() override
			{
				String^	strRcd = String::Empty;
				if(this->m_bUseCharacterLow)
				{
					strRcd = String::Format(L"{0:X02}: {1:X02} {2:X02}\r\n",this->Index, this->m_cCharacterHigh,this->m_cCharacterLow);
				}
				else
				{
					strRcd = String::Format(L"{0:X02}: {1:X02}\r\n",this->Index, this->m_cCharacterHigh);
				}
				return strRcd;
			}
		}
	#pragma endregion Override Properties
	public:
		//编码(Encode:String-->Bytes)时，无法编码的字符.(Heigh,Low同时存在时为Heigh)
		[Browsable(false), Localizable(false)]
		property wchar_t CharacterHigh
		{
			wchar_t get()
			{
				return this->m_cCharacterHigh;
			}
		}

		//编码(Encode:String-->Bytes)时，无法编码的字符的低位字节.Low不存在时返回0
		[Browsable(false), Localizable(false)]
		property wchar_t CharacterLow
		{
			wchar_t get()
			{
				return this->m_cCharacterLow;
			}
		}
	#pragma endregion Properties

	#pragma region 内部变量
	private:
		wchar_t		m_cCharacterHigh;	//编码(Encode:String-->Bytes)时，无法编码的字符。(Heigh,Low同时存在时为Heigh)
		wchar_t		m_cCharacterLow;	//编码(Encode:String-->Bytes)时，无法编码的字符的低位字节
		bool		m_bUseCharacterLow;	//是否使用了m_cCharacterLow
	#pragma endregion 内部变量
	};

	//************************************************************************
	//Name: 	DecoderUnknownBytes
	//Author: 	Zheng XiaoMing (2011/9/14)
	//Modify: 	
	//Return:  	
	//Description:记录解码(Decode:Bytes-->String)时遇到的当前Encoding下不认识的字节数据
	//			
	public ref class DecoderUnknownBytes : public EncodingUnknownDataBase
	{
	#pragma region 此类的初始化和善后处理
	public:
		DecoderUnknownBytes(array<unsigned char>^ oBytes, int nIndex) : EncodingUnknownDataBase(nIndex)
		{
			this->m_oByte = oBytes;
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region Properties
	#pragma region Override Properties
	public:
		//数据报告（字符串方式），需要在派生类中完善
		[Browsable(false), Localizable(false)]
		virtual property String^ Report
		{
			String^ get() override
			{
				String^ strRcd = String::Empty;
				if(this->m_oByte!=nullptr)
				{
					strRcd = String::Format(L"{0:X02}: ",this->Index);
					for each (unsigned char cByte in this->m_oByte)
					{
						strRcd += String::Format(L"{0:X02} ",cByte);
					}
					strRcd += L"\r\n";
				}

				return strRcd;
			}
		}
	#pragma endregion Override Properties
	public:
		//解码(Decode:Bytes-->String)时，无法解码的数据
		[Browsable(false), Localizable(false)]
		property array<unsigned char>^ Bytes
		{
			array<unsigned char>^ get()
			{
				return this->m_oByte;
			}
		}
	#pragma endregion Properties

	#pragma region 内部变量
	private:
		array<unsigned char>^	m_oByte;	//解码(Decode:Bytes-->String)时，无法解码的数据
	#pragma endregion 内部变量
	};

	//************************************************************************
	//Name: 	EncodingFallbackData
	//Author: 	Zheng XiaoMing (2011/9/14)
	//Modify: 	
	//Return:  	
	//Description:
	//			
	public ref class EncodingFallbackData : LinkedList<EncodingUnknownDataBase^>
	{
	#pragma region 此类的初始化和善后处理
	public:
		EncodingFallbackData()
		{
		}
	#pragma endregion 此类的初始化和善后处理
	#pragma region Properties
	public:
		//数据报告（字符串方式），需要在派生类中完善
		[Browsable(false), Localizable(false)]
		property String^ Report
		{
			String^ get()
			{
				String^ strRcd = String::Empty;

				for each (EncodingUnknownDataBase^ oData in this)
				{
				#ifdef _DEBUG
					Debug::Assert(oData!=nullptr,L"不可为nullptr");
				#endif	//_DEBUG
					if(oData!=nullptr)
					{
						strRcd += oData->Report;
					}
				}

				return strRcd;
			}
		}
	#pragma endregion Properties
	};

	//************************************************************************
	//Name: 	EncoderReplacementReportFallbackBuffer
	//Author: 	Zheng XiaoMing (2011/9/14)
	//Modify: 	
	//Return:  	
	//Description:与EncoderReplacementReportFallback共同使用，没有测试过，可作为Fallback()等函数的编制方法的参考
	//				在记录无法编码的的数据的同时，支持实施置换（函数：Fallback(),GetNextChar()等,  属性：Remaining）
	//			
	public ref class EncoderReplacementReportFallbackBuffer sealed : public System::Text::EncoderFallbackBuffer
	{
	#pragma region 内部用类
	public:
	#pragma endregion 内部用类

	#pragma region 此类的初始化和善后处理
	public:
		EncoderReplacementReportFallbackBuffer()
		{
			this->m_oFallbackData = gcnew EncodingFallbackData();	//解码(Decode:Bytes-->String)时，无法解码的数据
			this->m_strDefault = L"?";	//置换无法解码的数据用的字符串(固定为"?")

			this->m_nFallbackCount = 0;
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region Properties
	public:
		//解码(Decode:Bytes-->String)时，无法解码的数据
		[Browsable(false), Localizable(false)]
		property EncodingFallbackData^ UnknownDataList
		{
			EncodingFallbackData^ get()
			{
				return this->m_oFallbackData;
			}
		}
		//置换无法解码的数据用的字符串(固定为"?")
		[Browsable(false), Localizable(false)]
		property String^ DefaultString
		{
			String^ get()
			{
				return this->m_strDefault;
			}
		}
	#pragma region Override Properties
		//現在の EncoderFallbackBuffer オブジェクト内に処理されずに残っている文字数を取得します。
		virtual property int Remaining
		{
			int get() override
			{
				return this->m_nFallbackCount<=0?0:this->m_nFallbackCount;
			}
		}
	#pragma endregion Override Properties
	#pragma endregion Properties

	#pragma region 内部变量
	private:
		EncodingFallbackData^	m_oFallbackData;	//解码(Decode:Bytes-->String)时，无法解码的数据
		String^					m_strDefault;		//置换无法解码的数据用的字符串.(固定为"?")
		int						m_nFallbackCount;	//处理过程中被重复调用时的计数器
	#pragma endregion 内部变量
		
	#pragma region Override founctions
	public:
		//************************************************************************
		//Name: 	Fallback
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:利用此纪录不可encorder的字符
		//			此类必须Override的函数
		//			
		virtual bool Fallback(wchar_t charUnknown, int index)	override
		{
		#ifdef _DEBUG
			Debug::Assert(this->m_nFallbackCount<=0,L"正在处理过程中，不应该再次调用此函数");
		#endif	//_DEBUG
			bool	bRcd = false;
			if(this->m_nFallbackCount<=0)
			{//>0的时候属于正在处理中，不应该再次进入此函数
				this->m_oFallbackData->AddLast(gcnew EncoderUnknownCharacter(charUnknown,index));
				this->m_nFallbackCount = 1;	//为了传给上方一个置换符号"?"
				bRcd = true;	//置换处理，GetNextChar()将被调用
			}
			return bRcd;
		}

		//************************************************************************
		//Name: 	Fallback
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:此类必须Override的函数
		//
		virtual bool Fallback(wchar_t charUnknownHigh, wchar_t charUnknownLow, int index) override
		{
		#ifdef _DEBUG
			Debug::Assert(this->m_nFallbackCount<=0,L"正在处理过程中，不应该再次调用此函数");
		#endif	//_DEBUG
			bool	bRcd = false;
			if(this->m_nFallbackCount<=0)
			{//>0的时候属于正在处理中，不应该再次进入此函数
				this->m_oFallbackData->AddLast(gcnew EncoderUnknownCharacter(charUnknownHigh,charUnknownLow,index));
				this->m_nFallbackCount = 1;	//为了传给上方一个置换符号"?"
				bRcd = true;	//置换处理，GetNextChar()将被调用
			}
			return bRcd;
		}

		//************************************************************************
		//Name: 	GetNextChar
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:获取下一个置换用字符
		//
		virtual wchar_t GetNextChar() override
		{
			wchar_t	cRcd = L'\0';

			if (this->m_nFallbackCount > 0)
			{
			#ifdef _DEBUG
				Debug::Assert(this->m_strDefault!=nullptr,L"置换用字符必须存在");
				Debug::Assert(this->m_strDefault->Length==1,L"置换用字符一定是1个字");
			#endif	//_DEBUG
				this->m_nFallbackCount = this->m_strDefault[0];
			}
			this->m_nFallbackCount = 0;

			return cRcd;
		}

		//************************************************************************
		//Name: 	MovePrevious
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:指向下一个置换用字符
		//
		virtual bool MovePrevious() override
		{
			bool	bRcd = false;
			if(this->m_nFallbackCount<=0)
			{
				this->m_nFallbackCount = 1;
				bRcd = true;
			}
			return bRcd;
		}

		//************************************************************************
		//Name: 	Reset 
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:复位
		//
		virtual void Reset () override
		{
			this->m_nFallbackCount = 0;
		}
	#pragma endregion Override founctions
	};

	//************************************************************************
	//Name: 	EncoderReplacementReportFallback
	//Author: 	Zheng XiaoMing (2011/9/13)
	//Modify: 	
	//Return:  	
	//Description:编码(Encode:String-->Bytes)时,处理当前Encoding不能处理的文字。
	//				与EncoderReplacementReportFallbackBuffer共同使用
	//			注意：	
	//				1）	与System::Text::EncoderReplacementFallback类的功能有所不同
	//					可获取无法被编码的数据
	//				2）	代替符号固定为"?"(无论是单一字节还是，双字节都被置换为1个"?")
	//				3)	没有测试过
	//
	[SerializableAttribute]
	public ref class EncoderReplacementReportFallback sealed: public System::Text::EncoderFallback
	{
	#pragma region 内部用类
	public:
	#pragma endregion 内部用类

	#pragma region 此类的初始化和善后处理
	public:
		EncoderReplacementReportFallback()
		{
			this->m_oFallbackBuffer = gcnew EncoderReplacementReportFallbackBuffer();
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region 内部变量
	private:
		EncoderReplacementReportFallbackBuffer^	m_oFallbackBuffer;	//FallbackBuffer
	#pragma endregion 内部变量

	#pragma endregion Properties
	public:
		[Browsable(false), Localizable(false)]
		property EncoderReplacementReportFallbackBuffer^ FallbakBuffer
		{
			EncoderReplacementReportFallbackBuffer^ get()
			{
				return m_oFallbackBuffer;
			}
		}

	#pragma region Override Properties
		//現在の EncoderFallback オブジェクトが返すことができる最大文字数を取得します。
		virtual property int MaxCharCount
		{
			int get() override
			{
				return this->m_oFallbackBuffer->DefaultString->Length;
			}
		}
	#pragma endregion Override Properties
	#pragma endregion Properties

	#pragma region Override founctions
	public:
		//************************************************************************
		//Name: 	CreateFallbackBuffer
		//Author: 	Zheng XiaoMing (2009/6/29)
		//Modify: 	
		//Return:  	
		//Description:此类必须Override的函数，提供一个EncoderFallbackBuffer
		virtual EncoderFallbackBuffer^ CreateFallbackBuffer() override
		{
			return this->m_oFallbackBuffer;
		}
	#pragma endregion Override founctions
	};

	//************************************************************************
	//Name: 	EncoderReportFallbackBuffer
	//Author: 	Zheng XiaoMing (2011/9/14)
	//Modify: 	
	//Return:  	
	//Description:与EncoderReportFallback共同使用，在记录无法编码的的数据
	//			
	public ref class EncoderReportFallbackBuffer sealed : public System::Text::EncoderFallbackBuffer
	{
	#pragma region 内部用类
	public:
	#pragma endregion 内部用类

	#pragma region 此类的初始化和善后处理
	public:
		EncoderReportFallbackBuffer()
		{
			this->m_oFallbackData = gcnew EncodingFallbackData();	//解码(Decode:Bytes-->String)时，无法解码的数据
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region Properties
	public:
		//解码(Decode:Bytes-->String)时，无法解码的数据
		[Browsable(false), Localizable(false)]
		property EncodingFallbackData^ UnknownDataList
		{
			EncodingFallbackData^ get()
			{
				return this->m_oFallbackData;
			}
		}
	#pragma region Override Properties
		//現在の EncoderFallbackBuffer オブジェクト内に処理されずに残っている文字数を取得します。
		virtual property int Remaining
		{
			int get() override
			{
				return 0;
			}
		}
	#pragma endregion Override Properties
	#pragma endregion Properties

	#pragma region 内部变量
	private:
		EncodingFallbackData^	m_oFallbackData;	//解码(Decode:Bytes-->String)时，无法解码的数据
	#pragma endregion 内部变量
		
	#pragma region Override founctions
	public:
		//************************************************************************
		//Name: 	Fallback
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:利用此纪录不可encorder的字符
		//			此类必须Override的函数
		//			
		virtual bool Fallback(wchar_t charUnknown, int index)	override
		{
			this->m_oFallbackData->AddLast(gcnew EncoderUnknownCharacter(charUnknown,index));
			return false;
		}

		//************************************************************************
		//Name: 	Fallback
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:此类必须Override的函数
		//
		virtual bool Fallback(wchar_t charUnknownHigh, wchar_t charUnknownLow, int index) override
		{
			this->m_oFallbackData->AddLast(gcnew EncoderUnknownCharacter(charUnknownHigh,charUnknownLow,index));
			return false;
		}

		//************************************************************************
		//Name: 	GetNextChar
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:获取下一个置换用字符
		//
		virtual wchar_t GetNextChar() override
		{
			return L'\0';
		}

		//************************************************************************
		//Name: 	MovePrevious
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:指向下一个置换用字符
		//
		virtual bool MovePrevious() override
		{
			return false;
		}

		//************************************************************************
		//Name: 	Reset 
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:复位
		//
		virtual void Reset () override
		{
		}
	#pragma endregion Override founctions
	};

	//************************************************************************
	//Name: 	EncoderReportFallback
	//Author: 	Zheng XiaoMing (2011/9/13)
	//Modify: 	
	//Return:  	
	//Description:编码(Encode:String-->Bytes)时,记录当前Encoding不能处理的文字。
	//				与EncoderReportFallbackBuffer共同使用
	//
	[SerializableAttribute]
	public ref class EncoderReportFallback sealed: public System::Text::EncoderFallback
	{
	#pragma region 内部用类
	public:
	#pragma endregion 内部用类

	#pragma region 此类的初始化和善后处理
	public:
		EncoderReportFallback()
		{
			this->m_oFallbackBuffer = gcnew EncoderReportFallbackBuffer();
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region 内部变量
	private:
		EncoderReportFallbackBuffer^	m_oFallbackBuffer;	//FallbackBuffer
	#pragma endregion 内部变量

	#pragma endregion Properties
	public:
		[Browsable(false), Localizable(false)]
		property EncoderReportFallbackBuffer^ FallbakBuffer
		{
			EncoderReportFallbackBuffer^ get()
			{
				return m_oFallbackBuffer;
			}
		}

	#pragma region Override Properties
		//現在の EncoderFallback オブジェクトが返すことができる最大文字数を取得します。
		virtual property int MaxCharCount
		{
			int get() override
			{
				return 0;
			}
		}
	#pragma endregion Override Properties
	#pragma endregion Properties

	#pragma region Override founctions
	public:
		//************************************************************************
		//Name: 	CreateFallbackBuffer
		//Author: 	Zheng XiaoMing (2009/6/29)
		//Modify: 	
		//Return:  	
		//Description:此类必须Override的函数，提供一个EncoderFallbackBuffer
		virtual EncoderFallbackBuffer^ CreateFallbackBuffer() override
		{
			return this->m_oFallbackBuffer;
		}
	#pragma endregion Override founctions
	};

	//************************************************************************
	//Name: 	DecoderReportFallbackBuffer
	//Author: 	Zheng XiaoMing (2011/9/14)
	//Modify: 	
	//Return:  	
	//Description:与DecoderReportFallback共同使用，在记录无法解码的数据
	//			
	public ref class DecoderReportFallbackBuffer sealed : public System::Text::DecoderFallbackBuffer
	{
	#pragma region 内部用类
	public:
	#pragma endregion 内部用类

	#pragma region 此类的初始化和善后处理
	public:
		DecoderReportFallbackBuffer()
		{
			this->m_oFallbackData = gcnew EncodingFallbackData();	//解码(Decode:Bytes-->String)时，无法解码的数据
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region Properties
	public:
		//解码(Decode:Bytes-->String)时，无法解码的数据
		[Browsable(false), Localizable(false)]
		property EncodingFallbackData^ UnknownDataList
		{
			EncodingFallbackData^ get()
			{
				return this->m_oFallbackData;
			}
		}
	#pragma region Override Properties
		//現在の EncoderFallbackBuffer オブジェクト内に処理されずに残っている文字数を取得します。
		virtual property int Remaining
		{
			int get() override
			{
				return 0;
			}
		}
	#pragma endregion Override Properties
	#pragma endregion Properties

	#pragma region 内部变量
	private:
		EncodingFallbackData^	m_oFallbackData;	//解码(Decode:Bytes-->String)时，无法解码的数据
	#pragma endregion 内部变量
		
	#pragma region Override founctions
	public:
		//************************************************************************
		//Name: 	Fallback
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:利用此纪录不可encorder的字符
		//			此类必须Override的函数
		//			
		virtual bool Fallback(array<unsigned char>^ bytesUnknown, int index)	override
		{
			this->m_oFallbackData->AddLast(gcnew DecoderUnknownBytes(bytesUnknown,index));
			return false;
		}

		//************************************************************************
		//Name: 	GetNextChar
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:获取下一个置换用字符
		//
		virtual wchar_t GetNextChar() override
		{
			return L'\0';
		}

		//************************************************************************
		//Name: 	MovePrevious
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:指向下一个置换用字符
		//
		virtual bool MovePrevious() override
		{
			return false;
		}

		//************************************************************************
		//Name: 	Reset 
		//Author: 	Zheng XiaoMing (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description:复位
		//
		//virtual void Reset () override
		//{
		//}
	#pragma endregion Override founctions
	};

	//************************************************************************
	//Name: 	DecoderReportFallback
	//Author: 	Zheng XiaoMing (2011/9/13)
	//Modify: 	
	//Return:  	
	//Description:编码(Encode:String-->Bytes)时,记录当前Encoding不能处理的文字。
	//				与EncoderReportFallbackBuffer共同使用
	//
	[SerializableAttribute]
	public ref class DecoderReportFallback sealed: public System::Text::DecoderFallback
	{
	#pragma region 内部用类
	public:
	#pragma endregion 内部用类

	#pragma region 此类的初始化和善后处理
	public:
		DecoderReportFallback()
		{
			this->m_oFallbackBuffer = gcnew DecoderReportFallbackBuffer();
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region 内部变量
	private:
		DecoderReportFallbackBuffer^	m_oFallbackBuffer;	//FallbackBuffer
	#pragma endregion 内部变量

	#pragma endregion Properties
	public:
		[Browsable(false), Localizable(false)]
		property DecoderReportFallbackBuffer^ FallbakBuffer
		{
			DecoderReportFallbackBuffer^ get()
			{
				return m_oFallbackBuffer;
			}
		}

	#pragma region Override Properties
		//現在の EncoderFallback オブジェクトが返すことができる最大文字数を取得します。
		virtual property int MaxCharCount
		{
			int get() override
			{
				return 0;
			}
		}
	#pragma endregion Override Properties
	#pragma endregion Properties

	#pragma region Override founctions
	public:
		//************************************************************************
		//Name: 	CreateFallbackBuffer
		//Author: 	Zheng XiaoMing (2009/6/29)
		//Modify: 	
		//Return:  	
		//Description:此类必须Override的函数，提供一个EncoderFallbackBuffer
		virtual DecoderFallbackBuffer^ CreateFallbackBuffer() override
		{
			return this->m_oFallbackBuffer;
		}
	#pragma endregion Override founctions
	};
}
