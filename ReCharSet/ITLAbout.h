#pragma once

//#include "StdAfx.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Diagnostics;
using namespace System::Drawing;

namespace ReCharSet
{
	/// <summary>
	/// Summary for ITLAbout
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ITLAbout : public System::Windows::Forms::Form
	{
	public:
		ITLAbout(void)
		{
			InitializeComponent();

			//获取此App的Assembly(AssemblyInfo.cpp中的内容)
			System::Reflection::Assembly^ oAssembly = System::Reflection::Assembly::GetExecutingAssembly();
			System::Reflection::AssemblyTitleAttribute^ oTiltleAttribute = cli::safe_cast<System::Reflection::AssemblyTitleAttribute^>(System::Reflection::AssemblyTitleAttribute::GetCustomAttribute(oAssembly,System::Reflection::AssemblyTitleAttribute::typeid));
			System::Reflection::AssemblyCopyrightAttribute^ oCopyRightAttribute = cli::safe_cast<System::Reflection::AssemblyCopyrightAttribute^>(System::Reflection::AssemblyCopyrightAttribute::GetCustomAttribute(oAssembly,System::Reflection::AssemblyCopyrightAttribute::typeid));
			//System::Reflection::AssemblyVersionAttribute^ oVerisionRightAttribute = cli::safe_cast<System::Reflection::AssemblyVersionAttribute^>(System::Reflection::AssemblyVersionAttribute::GetCustomAttribute(oAssembly,System::Reflection::AssemblyVersionAttribute::typeid));

			this->labelProductName->Text = oTiltleAttribute->Title;
			//this->labelCopyRights->Text = oCopyRightAttribute->Copyright;
			System::Version^ oVer = oAssembly->GetName()->Version;
			this->labelVersion->Text = String::Format(L"Ver. {0}.{1} (Build: {2})",oVer->Major,oVer->Minor,oVer->Build);

			//Hyper link --> link ITL to www.infbj.com
			linkLabelCopyrights->Text =  oCopyRightAttribute->Copyright;
			String^	strITL = "INF Technologies Ltd.";
			int	nLength = strITL->Length;
			if(linkLabelCopyrights->Text->Length>=nLength)
			{
				int	nPos = linkLabelCopyrights->Text->IndexOf(strITL,System::StringComparison::OrdinalIgnoreCase);
				if(nPos>=0)
				{
					linkLabelCopyrights->Links->Add(nPos,nLength,"www.infbj.com");
				}
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ITLAbout()
		{
			if (components)
			{
				delete components;
			}
		}


	private:
		//************************************************************************
		//Name: 	OnLinkClicked
		//Author: 	Zheng XM (2012/4/18)
		//Modify:
		//Return:  	
		//Description: show web when clicked the link
		//
		System::Void OnLinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e)
		{
			if(sender->GetType()==System::Windows::Forms::LinkLabel::typeid)
			{
				System::Windows::Forms::LinkLabel^	oLinkLabel = cli::safe_cast<System::Windows::Forms::LinkLabel^>(sender);
				//此Link被点击过
				oLinkLabel->Links[oLinkLabel->Links->IndexOf(e->Link)]->Visited = true;
				if(e->Link->LinkData!=nullptr)
				{
					String^	strTarget = cli::safe_cast<String^>(e->Link->LinkData);
					if(!String::IsNullOrEmpty(strTarget))
					{
						System::Diagnostics::Process::Start(strTarget);
					}
				}
			}
		}

	private:

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		System::Windows::Forms::Button^  buttonOK;
		System::Windows::Forms::Label^  labelProductName;

		System::Windows::Forms::Label^  labelVersion;
		System::Windows::Forms::LinkLabel^  linkLabelCopyrights;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->buttonOK = (gcnew System::Windows::Forms::Button());
			this->labelProductName = (gcnew System::Windows::Forms::Label());
			this->labelVersion = (gcnew System::Windows::Forms::Label());
			this->linkLabelCopyrights = (gcnew System::Windows::Forms::LinkLabel());
			this->SuspendLayout();
			// 
			// buttonOK
			// 
			this->buttonOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->buttonOK->Location = System::Drawing::Point(147, 110);
			this->buttonOK->Margin = System::Windows::Forms::Padding(0);
			this->buttonOK->Name = L"buttonOK";
			this->buttonOK->Size = System::Drawing::Size(75, 21);
			this->buttonOK->TabIndex = 0;
			this->buttonOK->Text = L"OK";
			this->buttonOK->UseVisualStyleBackColor = true;
			// 
			// labelProductName
			// 
			this->labelProductName->AutoSize = true;
			this->labelProductName->Font = (gcnew System::Drawing::Font(L"Times New Roman", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->labelProductName->Location = System::Drawing::Point(17, 15);
			this->labelProductName->Name = L"labelProductName";
			this->labelProductName->Size = System::Drawing::Size(40, 16);
			this->labelProductName->TabIndex = 1;
			this->labelProductName->Text = L"label1";
			// 
			// labelVersion
			// 
			this->labelVersion->AutoSize = true;
			this->labelVersion->Font = (gcnew System::Drawing::Font(L"Times New Roman", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->labelVersion->Location = System::Drawing::Point(17, 38);
			this->labelVersion->Name = L"labelVersion";
			this->labelVersion->Size = System::Drawing::Size(40, 16);
			this->labelVersion->TabIndex = 1;
			this->labelVersion->Text = L"label1";
			// 
			// linkLabelCopyrights
			// 
			this->linkLabelCopyrights->AutoSize = true;
			this->linkLabelCopyrights->Font = (gcnew System::Drawing::Font(L"Times New Roman", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->linkLabelCopyrights->Location = System::Drawing::Point(18, 66);
			this->linkLabelCopyrights->Name = L"linkLabelCopyrights";
			this->linkLabelCopyrights->Size = System::Drawing::Size(115, 15);
			this->linkLabelCopyrights->TabIndex = 2;
			this->linkLabelCopyrights->TabStop = true;
			this->linkLabelCopyrights->Text = L"INF Technologies Ltd.";
			this->linkLabelCopyrights->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &ITLAbout::OnLinkClicked);
			// 
			// ITLAbout
			// 
			this->AcceptButton = this->buttonOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 11);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(369, 140);
			this->Controls->Add(this->linkLabelCopyrights);
			this->Controls->Add(this->labelVersion);
			this->Controls->Add(this->labelProductName);
			this->Controls->Add(this->buttonOK);
			this->Font = (gcnew System::Drawing::Font(L"Times New Roman", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"ITLAbout";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"About this application";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
};
}
