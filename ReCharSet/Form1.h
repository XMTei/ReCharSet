#pragma once

#include "ITLAbout.h"

namespace ReCharSet
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
	using namespace ITLStdLib;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();

			//this Software Title(使用AssemblyInfo.cpp中的AssemblyDescriptionAttribute)
			//array<System::Object^>^ oAttributes = this->GetType()->Assembly->GetCustomAttributes(System::Reflection::AssemblyDescriptionAttribute::typeid,true);
			//if(oAttributes->Length>0)
			//{
			//	this->Text = cli::safe_cast<System::Reflection::AssemblyDescriptionAttribute^>(oAttributes[0])->Description;
			//}



			//设置Toolbar上的Iamge
			this->toolStripButtonSetSelectedTextFileCharSet->Image = gcnew System::Drawing::Bitmap(this->GetType()->Assembly->GetManifestResourceStream(L"character_picker.png"));
			this->toolStripButtonDo->Image = gcnew System::Drawing::Bitmap(this->GetType()->Assembly->GetManifestResourceStream(L"apply.png"));
			this->toolStripButtonExpandSelectedAndItsSubNode->Image = gcnew System::Drawing::Bitmap(this->GetType()->Assembly->GetManifestResourceStream(L"toggle_expand.png"));
			this->toolStripButtonCollapseSelectedAndItsSubNode->Image = gcnew System::Drawing::Bitmap(this->GetType()->Assembly->GetManifestResourceStream(L"toggle_collapse.png"));
			this->toolStripButtonAbout->Image = gcnew Bitmap(this->GetType()->Assembly->GetManifestResourceStream(L"get_info.png"));

			//设置TreeListView的ImageList
			this->treeListViewFiles->SmallImageList = gcnew ImageList();
			for each (ITLStdLib::TextFileEncordingInfo^ oFileInfo in this->SupportedFiles)
			{
				//获取Icon
				System::Drawing::Icon^ oFileIcon = ITLStdLib::Shell32::GetFileIcon(oFileInfo->FileExtention,true,false);
				this->treeListViewFiles->SmallImageList->Images->Add(oFileIcon);
			}
			//this->treelistViewTest->SmallImageList的最后两个Image是关闭的FolderIcon和打开的FolderIcon
			System::Drawing::Icon^ oFolderIcon = ITLStdLib::Shell32::GetFolderIcon(L"dic",true, true);	//Open type
			this->treeListViewFiles->SmallImageList->Images->Add(oFolderIcon);
			oFolderIcon = ITLStdLib::Shell32::GetFolderIcon(L"dic",true, false);	//Close type
			this->treeListViewFiles->SmallImageList->Images->Add(oFolderIcon);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	#pragma region Properties
	protected:
		//可处理的文件类型集
		[Browsable(false), Localizable(false)]
		property array<ITLStdLib::TextFileEncordingInfo^>^ SupportedFiles
		{
			array<ITLStdLib::TextFileEncordingInfo^>^ get()
			{
				array<ITLStdLib::TextFileEncordingInfo^>^ oFlies = gcnew array<ITLStdLib::TextFileEncordingInfo^>(6)
												{
													gcnew ITLStdLib::TextFileEncordingInfo(L"C Source Code",L".c"),
													gcnew ITLStdLib::TextFileEncordingInfo(L"C++ Source Code",L".cpp"),
													gcnew ITLStdLib::TextFileEncordingInfo(L"C/C++ Header File",L".h"),
													gcnew ITLStdLib::TextFileEncordingInfo(L"In Line File",L".inl"),
													gcnew ITLStdLib::TextFileEncordingInfo(L"Resource File",L".rc"),
													gcnew ITLStdLib::TextFileEncordingInfo(L"Text File",L".txt")
												};
				return oFlies;
			}
		}

		//当前系统所支持的Encordings的MenuItems（array<System::Windows::Forms::ToolStripItem^>^）
		//每个Menu的Tag中记录着相应的Encroding的object
		[Browsable(false), Localizable(false)]
		property array<System::Windows::Forms::ToolStripItem^>^ SystemSupportedEncordingMenu
		{
			array<System::Windows::Forms::ToolStripItem^>^ get()
			{
				array<System::Windows::Forms::ToolStripItem^>^	oRcd = nullptr;

				array<System::Text::EncodingInfo^>^	oaEncordingInfo = System::Text::Encoding::GetEncodings();
				if(oaEncordingInfo->Length>0)
				{
					oRcd = gcnew array<System::Windows::Forms::ToolStripItem^>(oaEncordingInfo->Length);
					int i =0;
					for each(System::Text::EncodingInfo^ oEncordingInfo in System::Text::Encoding::GetEncodings())
					{
						oRcd[i] = gcnew System::Windows::Forms::ToolStripMenuItem();
						oRcd[i]->Text = oEncordingInfo->DisplayName;
						oRcd[i]->Tag =  oEncordingInfo->GetEncoding();
						i++;
					}
				}
				return oRcd;
			}
		}
	#pragma endregion Properties

	protected: 

	#pragma region UI应答函数
	private:
		//************************************************************************
		//Name: 	OnSelectDirectory
		//Author: 	Zheng XM (2011/9/12)
		//Modify:
		//Return:  	
		//Description: 选择需要转换的文件夹
		//
		System::Void OnSelectDirectory(System::Object^  /*sender*/, System::EventArgs^  /*e*/)
		{
			FolderBrowserDialog^ oSelectForlder = gcnew FolderBrowserDialog();
			oSelectForlder->ShowNewFolderButton = false;

			if (this->textBoxDirectory->Text->Equals(""))
			{	//如果还没有设定过Dir的话就使用当前的Dir
				oSelectForlder->SelectedPath = Environment::CurrentDirectory;
			}
			else
			{	//如果已经设定过，就使用其
				oSelectForlder->SelectedPath = this->textBoxDirectory->Text;
			}

			// Show the FolderBrowserDialog.
			System::Windows::Forms::DialogResult result = oSelectForlder->ShowDialog();
			if (result == System::Windows::Forms::DialogResult::OK)
			{	//选择好DIR了
				this->textBoxDirectory->Text = oSelectForlder->SelectedPath;
				this->treeListViewFiles->VirtualListSize = 0;
				MakeTreeListViewItemFromDirectoryAndFile(this->textBoxDirectory->Text, this->treeListViewFiles->Nodes);
				this->treeListViewFiles->VirtualListSize = this->treeListViewFiles->Nodes->TotalCount[true];
			}
		}

		//************************************************************************
		//Name: 	OnSelectedFileChanged
		//Author: 	Zheng XM (2011/9/12)
		//Modify:
		//Return:  	
		//Description: 变换选择文件
		//
		System::Void OnSelectedFileChanged(System::Object^  /*sender*/, System::EventArgs^  /*e*/)
		{
			Cursor->Current = Cursors::WaitCursor;	//显示等待
			//清空Preview
			this->textBoxPreview->Text = String::Empty;
			if((this->treeListViewFiles->SelectedIndices->Count>0)&&
				(this->treeListViewFiles->SelectedIndices[0]>=0))
			{//有选项
				ITLStdLib::TreeListViewNode::Position^ oPos = this->treeListViewFiles->Nodes->At[this->treeListViewFiles->SelectedIndices[0],true];
				if((oPos!=nullptr)&&
					(oPos->TreeNode!=nullptr)&&
					(oPos->TreeNode->GetType()==ITLStdLib::TreeListViewTextFileNode::typeid))
				{//所选的是文件Node
					ITLStdLib::TreeListViewTextFileNode^ oFileNode = cli::safe_cast<ITLStdLib::TreeListViewTextFileNode^>(oPos->TreeNode);
					this->textBoxPreview->Text = oFileNode->TextFromFile;
					this->textBoxFailedDecodedData->Text = oFileNode->FailedDacodedData;
				}
			}
		}

		//************************************************************************
		//Name: 	OnDoChangeCharacterSet
		//Author: 	Zheng XM (2011/9/13)
		//Modify:
		//Return:  	
		//Description: 更改所有文件的Character Set
		//
		System::Void OnDoChangeCharacterSet(System::Object^  /*sender*/, System::EventArgs^  /*e*/)
		{
			String^ strMessageTile = Application::ProductName;
			String^ strMessage = L"All the text files will be save as Unicode, and orignal text files will be lost.\nIf you want to keep them, please have a copy.\n Contine?";

			if(MessageBox::Show(this,strMessage,strMessageTile,MessageBoxButtons::YesNo,MessageBoxIcon::Asterisk)==System::Windows::Forms::DialogResult::Yes)
			{
				Cursor->Current = Cursors::WaitCursor;	//显示等待

				//获取第一个Node
				ITLStdLib::TreeListViewNode::Position^ oPos = this->treeListViewFiles->Nodes->FirstTreeListViewNode;
				//将所有文本文件保存为Unicode的文本数据
				while(oPos!=nullptr)
				{
					ITLStdLib::TreeListViewNode^ oNode = oPos->TreeNode;
					if(oNode->GetType()==ITLStdLib::TreeListViewTextFileNode::typeid)
					{//此Node属于文本文件的Node
						ITLStdLib::TreeListViewTextFileNode^ oTextFileNode = cli::safe_cast<ITLStdLib::TreeListViewTextFileNode^>(oNode);
						//保存
						bool bRcd = oTextFileNode->SaveAsUnicodeFile();
						if(!bRcd)
						{//转换被中断
							break;
						}
					}
					oPos = oPos->Next[false];	//取下一个Node（不可视的也取）
				}
				//重新取一次文件，以便更新显示内容
				this->treeListViewFiles->VirtualListSize = 0;
				MakeTreeListViewItemFromDirectoryAndFile(this->textBoxDirectory->Text, this->treeListViewFiles->Nodes);
				this->treeListViewFiles->VirtualListSize = this->treeListViewFiles->Nodes->TotalCount[true];
			}
		}

		//************************************************************************
		//Name: 	OnSetTextFileCharacterSet
		//Author: 	Zheng XM (2011/9/12)
		//Modify:
		//Return:  	
		//Description: 设置这顶文件的CharacterSet（Encording）
		//
		System::Void OnSetTextFileCharacterSet(System::Object^  /*sender*/, System::Windows::Forms::ToolStripItemClickedEventArgs^  e)
		{
			Cursor->Current = Cursors::WaitCursor;	//显示等待
		#ifdef _DEBUG
			Debug::Assert(e->ClickedItem!=nullptr,L"点击的menu必须存在");
			Debug::Assert(e->ClickedItem->Tag!=nullptr,L"点击的menu的Tag必须存在");
			Debug::Assert(e->ClickedItem->Tag->GetType()!=System::Text::Encoding::typeid,L"点击的menu的Tag必须是Encoding");
		#endif	//_DEBUG
			if((e->ClickedItem!=nullptr)&&
				(e->ClickedItem->Tag!=nullptr)&&
				(e->ClickedItem->Tag->GetType()!=System::Text::Encoding::typeid)&&
				(this->treeListViewFiles->SelectedIndices->Count>0)&&
				(this->treeListViewFiles->SelectedIndices[0]>=0))
			{//有选项
				ITLStdLib::TreeListViewNode::Position^ oPos = this->treeListViewFiles->Nodes->At[this->treeListViewFiles->SelectedIndices[0],true];
				if((oPos!=nullptr)&&
					(oPos->TreeNode!=nullptr)&&
					(oPos->TreeNode->GetType()==ITLStdLib::TreeListViewTextFileNode::typeid))
				{//所选的是文件Node
					ITLStdLib::TreeListViewTextFileNode^ oFileNode = cli::safe_cast<ITLStdLib::TreeListViewTextFileNode^>(oPos->TreeNode);
					if((oFileNode->SubItems!=nullptr)&&
						(oFileNode->SubItems->Count>1)&&
						(oFileNode->SubItems[1]->GetType()==ITLStdLib::TreeListViewTextFileNode::TextFileEncodingSubItem::typeid))
					{//Encording sub Item存在
						ITLStdLib::TreeListViewTextFileNode::TextFileEncodingSubItem^ oEncordingSubItem = cli::safe_cast<ITLStdLib::TreeListViewTextFileNode::TextFileEncodingSubItem^>(oFileNode->SubItems[1]);
						oEncordingSubItem->EncodingInfo = cli::safe_cast<System::Text::Encoding^>(e->ClickedItem->Tag);
						this->treeListViewFiles->Invalidate();	//重新显示
						this->textBoxPreview->Text = oFileNode->TextFromFile;
						this->textBoxFailedDecodedData->Text = oFileNode->FailedDacodedData;
					}
				}
			}
		}

		//************************************************************************
		//Name: 	OnSetTextFileCharacterSetButtonOpening
		//Author: 	Zheng XM (2011/9/12)
		//Modify:
		//Return:  	
		//Description: 设置文件的CharacterSet（Encording）的按钮被按动，需要准备可用的Encording Menu的时候
		//
		System::Void OnSetTextFileCharacterSetButtonOpening(System::Object^  /*sender*/, System::EventArgs^  /*e*/)
		{
			Cursor->Current = Cursors::WaitCursor;	//显示等待
			array<System::Windows::Forms::ToolStripItem^>^ oToolStripItems = this->SystemSupportedEncordingMenu;
			if((oToolStripItems!=nullptr)&&
				(oToolStripItems->Length>0))
			{
				this->toolStripButtonSetSelectedTextFileCharSet->DropDownItems->AddRange(oToolStripItems);
			}
			else
			{
				this->toolStripButtonSetSelectedTextFileCharSet->DropDownItems->Clear();
			}
		}

		//************************************************************************
		//Name: 	OnSetTextFileCharacterSetContextMenuOpening
		//Author: 	Zheng XM (2011/9/12)
		//Modify:
		//Return:  	
		//Description: 设置文件的CharacterSet（Encording）的ContextMenu打开时，需要准备可用的Encording Menu的时候
		//
		System::Void OnSetTextFileCharacterSetContextMenuOpening(System::Object^  /*sender*/, System::ComponentModel::CancelEventArgs^  e)
		{
			Cursor->Current = Cursors::WaitCursor;	//显示等待
			array<System::Windows::Forms::ToolStripItem^>^ oToolStripItems = this->SystemSupportedEncordingMenu;
			if((oToolStripItems!=nullptr)&&
				(oToolStripItems->Length>0))
			{
				this->contextMenuStripTreeListViewItem->Items->AddRange(oToolStripItems);
				e->Cancel = false;
			}
			else
			{
				this->contextMenuStripTreeListViewItem->Items->Clear();
				e->Cancel = true;
			}
		}

		//************************************************************************
		//Name: 	OnExpandSelectAndItsSubNode
		//Author: 	Zheng XM (2011/9/13)
		//Modify:
		//Return:  	
		//Description: 展开当前所选Node(如果是Directory Node)极其其下所有DirectoryNode
		//
		System::Void OnExpandSelectAndItsSubNode(System::Object^  /*sender*/, System::EventArgs^  /*e*/)
		{
			Cursor->Current = Cursors::WaitCursor;	//显示等待
			this->treeListViewFiles->ExpandSelectedAll();
		}

		//************************************************************************
		//Name: 	OnCollapseSelectAndItsSubNode
		//Author: 	Zheng XM (2011/9/13)
		//Modify:
		//Return:  	
		//Description: 关闭当前所选Node(如果是Directory Node)极其其下所有DirectoryNode
		//
		System::Void OnCollapseSelectAndItsSubNode(System::Object^  /*sender*/, System::EventArgs^  /*e*/)
		{
			Cursor->Current = Cursors::WaitCursor;	//显示等待
			this->treeListViewFiles->CollapseSelectedAll();
		}

		//************************************************************************
		//Name: 	OnAboutThisApplication
		//Author: 	Zheng XM (2011/9/13)
		//Modify:
		//Return:  	
		//Description: about this app
		//
		System::Void OnAboutThisApplication(System::Object^  /*sender*/, System::EventArgs^  /*e*/)
		{
			ITLAbout^ oAboutDlg = gcnew ITLAbout();

			oAboutDlg->ShowDialog();
		}
	#pragma endregion UI应答函数

	#pragma region 内部函数
	private:
		//************************************************************************
		//Name: 	ConvertDirectory
		//Author: 	Zheng XM (2010/7/3)
		//Modify: 	
		//Return:  
		//Description: 转换一个DIR之下的相关文件，并递归调用此，转换其下的DIR
		//		
		void MakeTreeListViewItemFromDirectoryAndFile(String^ strParentDirectory,ITLStdLib::TreeListViewNode::TreeListViewNodeCollection^ oNodes)
		{
			oNodes->Clear();
			Cursor->Current = Cursors::WaitCursor;	//显示等待
			if (Directory::Exists(strParentDirectory))
			{
				//继续处理此Dir之下的Files
				array<String^>^ astrFileNames = Directory::GetFiles(strParentDirectory);
				for each (String^ strFileName in astrFileNames)
				{
					if (File::Exists(strFileName))
					{	//此文件存在
						FileInfo^ oFileInfo = gcnew FileInfo(strFileName);
						int nFileInfoIndex = this->GetFileInfoIndex(oFileInfo->Extension);
						if (nFileInfoIndex>=0)
						{//此文件属于处理对象
							ITLStdLib::TreeListViewTextFileNode^ oNode = gcnew ITLStdLib::TreeListViewTextFileNode(oFileInfo);
							oNode->ImageIndex = nFileInfoIndex;
							oNodes->AddLast(oNode);
						}
					}
				}

				//继续处理此Dir之下的Directories
				array<String^>^ astrDirectories = Directory::GetDirectories(strParentDirectory);
				for each (String^ strDirectory in astrDirectories)
				{
					//预备文件夹类
					ITLStdLib::TreeListViewDirectoryNode^	oTreeListViewDirectoryNode = gcnew ITLStdLib::TreeListViewDirectoryNode(gcnew System::IO::DirectoryInfo(strDirectory));
					//oTreeListViewDirectoryNode->ShowSubTreeListViewNodes = false;
					//添加文件夹
					oNodes->AddLast(oTreeListViewDirectoryNode);
				#ifdef _DEBUG
					Debug::Assert(oTreeListViewDirectoryNode->SubTreeListViewNodes!=nullptr,L"ITLStdLib::TreeListViewDirectoryNode的SubTreeListViewNodes值不可为nulltr");
				#endif	//_DEBUG
					//递归调用此函数
					MakeTreeListViewItemFromDirectoryAndFile(strDirectory,oTreeListViewDirectoryNode->SubTreeListViewNodes);
				}
			}
		}

		//************************************************************************
		//Name: 	GetFileInfoIndex
		//Author: 	Zheng XM (2011/9/3)
		//Modify: 	
		//Return:  -1:不支持此文件。〉0：this->SupprotedFiles中的Index
		//Description: 检查给出的文件扩充名是否是处理对象文件,
		//				如果是返回其在this->SupprotedFiles中的Index。
		//				如果不是返回-1
		//		
		int GetFileInfoIndex(String^ strFileExtension)
		{
			int nRcd = -1;

			strFileExtension->ToLower();	//一定要以小写字来比较
			int nTemp = 0;
			for each (ITLStdLib::TextFileEncordingInfo^ oFileInfo in this->SupportedFiles)
			{
				if (oFileInfo->FileExtention->Equals(strFileExtension))
				{
					nRcd = nTemp;
					break;
				}
				nTemp++;
			}

			return nRcd;
		}
#pragma endregion 内部函数


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		System::ComponentModel::IContainer^  components;
		System::Windows::Forms::SplitContainer^  splitContainer1;
		System::Windows::Forms::SplitContainer^  splitContainer2;
		System::Windows::Forms::GroupBox^  groupBoxStep1;
		System::Windows::Forms::Button^  buttonSelectDirectory;
		System::Windows::Forms::TextBox^  textBoxDirectory;
		System::Windows::Forms::ToolStrip^  toolStripTreeListView;

		System::Windows::Forms::ToolStripButton^  toolStripButtonDo;
		System::Windows::Forms::GroupBox^  groupBoxStep2;
		System::Windows::Forms::GroupBox^  groupBoxPreview;
		System::Windows::Forms::TextBox^  textBoxPreview;
		System::Windows::Forms::ColumnHeader^  columnHeaderName;
		System::Windows::Forms::ColumnHeader^  columnHeaderCharaterSet;
		System::Windows::Forms::SplitContainer^  splitContainer3;
		System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
		System::Windows::Forms::ToolStripDropDownButton^  toolStripButtonSetSelectedTextFileCharSet;
		System::Windows::Forms::ToolStripButton^  toolStripButtonExpandSelectedAndItsSubNode;
		System::Windows::Forms::ToolStripButton^  toolStripButtonCollapseSelectedAndItsSubNode;
		System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
		System::Windows::Forms::ContextMenuStrip^  contextMenuStripTreeListViewItem;
		System::Windows::Forms::SplitContainer^  splitContainer4;
		System::Windows::Forms::GroupBox^  groupBoxFailedtoDecode;
		System::Windows::Forms::TextBox^  textBoxFailedDecodedData;
		System::Windows::Forms::ToolStripButton^  toolStripButtonAbout;

		ITLStdLib::TreeListView^  treeListViewFiles;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->groupBoxStep1 = (gcnew System::Windows::Forms::GroupBox());
			this->splitContainer3 = (gcnew System::Windows::Forms::SplitContainer());
			this->textBoxDirectory = (gcnew System::Windows::Forms::TextBox());
			this->buttonSelectDirectory = (gcnew System::Windows::Forms::Button());
			this->groupBoxStep2 = (gcnew System::Windows::Forms::GroupBox());
			this->treeListViewFiles = (gcnew ITLStdLib::TreeListView());
			this->columnHeaderName = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeaderCharaterSet = (gcnew System::Windows::Forms::ColumnHeader());
			this->contextMenuStripTreeListViewItem = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->toolStripTreeListView = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripButtonDo = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripButtonSetSelectedTextFileCharSet = (gcnew System::Windows::Forms::ToolStripDropDownButton());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripButtonExpandSelectedAndItsSubNode = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButtonCollapseSelectedAndItsSubNode = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButtonAbout = (gcnew System::Windows::Forms::ToolStripButton());
			this->splitContainer4 = (gcnew System::Windows::Forms::SplitContainer());
			this->groupBoxPreview = (gcnew System::Windows::Forms::GroupBox());
			this->textBoxPreview = (gcnew System::Windows::Forms::TextBox());
			this->groupBoxFailedtoDecode = (gcnew System::Windows::Forms::GroupBox());
			this->textBoxFailedDecodedData = (gcnew System::Windows::Forms::TextBox());
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->groupBoxStep1->SuspendLayout();
			this->splitContainer3->Panel1->SuspendLayout();
			this->splitContainer3->Panel2->SuspendLayout();
			this->splitContainer3->SuspendLayout();
			this->groupBoxStep2->SuspendLayout();
			this->toolStripTreeListView->SuspendLayout();
			this->splitContainer4->Panel1->SuspendLayout();
			this->splitContainer4->Panel2->SuspendLayout();
			this->splitContainer4->SuspendLayout();
			this->groupBoxPreview->SuspendLayout();
			this->groupBoxFailedtoDecode->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			resources->ApplyResources(this->splitContainer1, L"splitContainer1");
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->splitContainer2);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer4);
			// 
			// splitContainer2
			// 
			resources->ApplyResources(this->splitContainer2, L"splitContainer2");
			this->splitContainer2->Name = L"splitContainer2";
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->groupBoxStep1);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->groupBoxStep2);
			// 
			// groupBoxStep1
			// 
			this->groupBoxStep1->Controls->Add(this->splitContainer3);
			resources->ApplyResources(this->groupBoxStep1, L"groupBoxStep1");
			this->groupBoxStep1->Name = L"groupBoxStep1";
			this->groupBoxStep1->TabStop = false;
			// 
			// splitContainer3
			// 
			resources->ApplyResources(this->splitContainer3, L"splitContainer3");
			this->splitContainer3->Name = L"splitContainer3";
			// 
			// splitContainer3.Panel1
			// 
			this->splitContainer3->Panel1->Controls->Add(this->textBoxDirectory);
			// 
			// splitContainer3.Panel2
			// 
			this->splitContainer3->Panel2->Controls->Add(this->buttonSelectDirectory);
			// 
			// textBoxDirectory
			// 
			resources->ApplyResources(this->textBoxDirectory, L"textBoxDirectory");
			this->textBoxDirectory->Name = L"textBoxDirectory";
			this->textBoxDirectory->ReadOnly = true;
			// 
			// buttonSelectDirectory
			// 
			resources->ApplyResources(this->buttonSelectDirectory, L"buttonSelectDirectory");
			this->buttonSelectDirectory->Name = L"buttonSelectDirectory";
			this->buttonSelectDirectory->UseVisualStyleBackColor = true;
			this->buttonSelectDirectory->Click += gcnew System::EventHandler(this, &Form1::OnSelectDirectory);
			// 
			// groupBoxStep2
			// 
			this->groupBoxStep2->Controls->Add(this->treeListViewFiles);
			this->groupBoxStep2->Controls->Add(this->toolStripTreeListView);
			resources->ApplyResources(this->groupBoxStep2, L"groupBoxStep2");
			this->groupBoxStep2->Name = L"groupBoxStep2";
			this->groupBoxStep2->TabStop = false;
			// 
			// treeListViewFiles
			// 
			this->treeListViewFiles->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeaderName, 
				this->columnHeaderCharaterSet});
			this->treeListViewFiles->ContextMenuStrip = this->contextMenuStripTreeListViewItem;
			resources->ApplyResources(this->treeListViewFiles, L"treeListViewFiles");
			this->treeListViewFiles->FullRowSelect = true;
			this->treeListViewFiles->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->treeListViewFiles->MultiSelect = false;
			this->treeListViewFiles->Name = L"treeListViewFiles";
			this->treeListViewFiles->OwnerDraw = true;
			this->treeListViewFiles->ShowItemToolTips = true;
			this->treeListViewFiles->Sorting = System::Windows::Forms::SortOrder::None;
			this->treeListViewFiles->UseCompatibleStateImageBehavior = false;
			this->treeListViewFiles->View = System::Windows::Forms::View::Details;
			this->treeListViewFiles->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::OnSelectedFileChanged);
			// 
			// columnHeaderName
			// 
			resources->ApplyResources(this->columnHeaderName, L"columnHeaderName");
			// 
			// columnHeaderCharaterSet
			// 
			resources->ApplyResources(this->columnHeaderCharaterSet, L"columnHeaderCharaterSet");
			// 
			// contextMenuStripTreeListViewItem
			// 
			this->contextMenuStripTreeListViewItem->Name = L"contextMenuStripTreeListViewItem";
			resources->ApplyResources(this->contextMenuStripTreeListViewItem, L"contextMenuStripTreeListViewItem");
			this->contextMenuStripTreeListViewItem->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &Form1::OnSetTextFileCharacterSet);
			this->contextMenuStripTreeListViewItem->Opening += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::OnSetTextFileCharacterSetContextMenuOpening);
			// 
			// toolStripTreeListView
			// 
			this->toolStripTreeListView->ImageScalingSize = System::Drawing::Size(24, 24);
			this->toolStripTreeListView->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {this->toolStripButtonDo, 
				this->toolStripSeparator2, this->toolStripButtonSetSelectedTextFileCharSet, this->toolStripSeparator1, this->toolStripButtonExpandSelectedAndItsSubNode, 
				this->toolStripButtonCollapseSelectedAndItsSubNode, this->toolStripButtonAbout});
			resources->ApplyResources(this->toolStripTreeListView, L"toolStripTreeListView");
			this->toolStripTreeListView->Name = L"toolStripTreeListView";
			// 
			// toolStripButtonDo
			// 
			this->toolStripButtonDo->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->toolStripButtonDo, L"toolStripButtonDo");
			this->toolStripButtonDo->Name = L"toolStripButtonDo";
			this->toolStripButtonDo->Click += gcnew System::EventHandler(this, &Form1::OnDoChangeCharacterSet);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			resources->ApplyResources(this->toolStripSeparator2, L"toolStripSeparator2");
			// 
			// toolStripButtonSetSelectedTextFileCharSet
			// 
			this->toolStripButtonSetSelectedTextFileCharSet->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->toolStripButtonSetSelectedTextFileCharSet, L"toolStripButtonSetSelectedTextFileCharSet");
			this->toolStripButtonSetSelectedTextFileCharSet->Name = L"toolStripButtonSetSelectedTextFileCharSet";
			this->toolStripButtonSetSelectedTextFileCharSet->DropDownItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &Form1::OnSetTextFileCharacterSet);
			this->toolStripButtonSetSelectedTextFileCharSet->DropDownOpening += gcnew System::EventHandler(this, &Form1::OnSetTextFileCharacterSetButtonOpening);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			resources->ApplyResources(this->toolStripSeparator1, L"toolStripSeparator1");
			// 
			// toolStripButtonExpandSelectedAndItsSubNode
			// 
			this->toolStripButtonExpandSelectedAndItsSubNode->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->toolStripButtonExpandSelectedAndItsSubNode, L"toolStripButtonExpandSelectedAndItsSubNode");
			this->toolStripButtonExpandSelectedAndItsSubNode->Name = L"toolStripButtonExpandSelectedAndItsSubNode";
			this->toolStripButtonExpandSelectedAndItsSubNode->Click += gcnew System::EventHandler(this, &Form1::OnExpandSelectAndItsSubNode);
			// 
			// toolStripButtonCollapseSelectedAndItsSubNode
			// 
			this->toolStripButtonCollapseSelectedAndItsSubNode->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->toolStripButtonCollapseSelectedAndItsSubNode, L"toolStripButtonCollapseSelectedAndItsSubNode");
			this->toolStripButtonCollapseSelectedAndItsSubNode->Name = L"toolStripButtonCollapseSelectedAndItsSubNode";
			this->toolStripButtonCollapseSelectedAndItsSubNode->Click += gcnew System::EventHandler(this, &Form1::OnCollapseSelectAndItsSubNode);
			// 
			// toolStripButtonAbout
			// 
			this->toolStripButtonAbout->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->toolStripButtonAbout->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			resources->ApplyResources(this->toolStripButtonAbout, L"toolStripButtonAbout");
			this->toolStripButtonAbout->Name = L"toolStripButtonAbout";
			this->toolStripButtonAbout->Click += gcnew System::EventHandler(this, &Form1::OnAboutThisApplication);
			// 
			// splitContainer4
			// 
			resources->ApplyResources(this->splitContainer4, L"splitContainer4");
			this->splitContainer4->Name = L"splitContainer4";
			// 
			// splitContainer4.Panel1
			// 
			this->splitContainer4->Panel1->Controls->Add(this->groupBoxPreview);
			// 
			// splitContainer4.Panel2
			// 
			this->splitContainer4->Panel2->Controls->Add(this->groupBoxFailedtoDecode);
			// 
			// groupBoxPreview
			// 
			this->groupBoxPreview->Controls->Add(this->textBoxPreview);
			resources->ApplyResources(this->groupBoxPreview, L"groupBoxPreview");
			this->groupBoxPreview->Name = L"groupBoxPreview";
			this->groupBoxPreview->TabStop = false;
			// 
			// textBoxPreview
			// 
			resources->ApplyResources(this->textBoxPreview, L"textBoxPreview");
			this->textBoxPreview->Name = L"textBoxPreview";
			this->textBoxPreview->ReadOnly = true;
			// 
			// groupBoxFailedtoDecode
			// 
			this->groupBoxFailedtoDecode->Controls->Add(this->textBoxFailedDecodedData);
			resources->ApplyResources(this->groupBoxFailedtoDecode, L"groupBoxFailedtoDecode");
			this->groupBoxFailedtoDecode->Name = L"groupBoxFailedtoDecode";
			this->groupBoxFailedtoDecode->TabStop = false;
			// 
			// textBoxFailedDecodedData
			// 
			resources->ApplyResources(this->textBoxFailedDecodedData, L"textBoxFailedDecodedData");
			this->textBoxFailedDecodedData->Name = L"textBoxFailedDecodedData";
			this->textBoxFailedDecodedData->ReadOnly = true;
			// 
			// Form1
			// 
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer1);
			this->Name = L"Form1";
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->ResumeLayout(false);
			this->groupBoxStep1->ResumeLayout(false);
			this->splitContainer3->Panel1->ResumeLayout(false);
			this->splitContainer3->Panel1->PerformLayout();
			this->splitContainer3->Panel2->ResumeLayout(false);
			this->splitContainer3->ResumeLayout(false);
			this->groupBoxStep2->ResumeLayout(false);
			this->groupBoxStep2->PerformLayout();
			this->toolStripTreeListView->ResumeLayout(false);
			this->toolStripTreeListView->PerformLayout();
			this->splitContainer4->Panel1->ResumeLayout(false);
			this->splitContainer4->Panel2->ResumeLayout(false);
			this->splitContainer4->ResumeLayout(false);
			this->groupBoxPreview->ResumeLayout(false);
			this->groupBoxPreview->PerformLayout();
			this->groupBoxFailedtoDecode->ResumeLayout(false);
			this->groupBoxFailedtoDecode->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
};
}

