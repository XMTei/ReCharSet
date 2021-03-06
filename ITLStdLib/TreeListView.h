// TreeListView.h : 一个基于ListView的TreeListView控件
// Copyright: ⓒ INF Technologies Ltd., 2011. All Rights Reserved
// Revision:
// 2011.08.29 XiaoMing Zheng  
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
	using namespace System::Windows::Forms;
	using namespace System::Windows::Forms::Design;
	using namespace System::Windows::Forms::VisualStyles;
	
	//************************************************************************
	//Name: 	ITLTreeListView
	//Author: 	Zheng XM (2011/8/29)
	//Modify: 	
	//Return:  	
	//Description:  一个基于ListView的TreeListView控件
	//
	//[ToolboxItem(System::Windows::Forms::TreeView::typeid)]
	public ref class TreeListView : public System::Windows::Forms::ListView
	{
	public:
		TreeListView():System::Windows::Forms::ListView()
		{
			__super::DoubleBuffered = true;
			__super::OwnerDraw = true;

			__super::ShowGroups = false;	//不允许使用Group
			__super::Sorting =  System::Windows::Forms::SortOrder::None;	//不排序
			__super::View = System::Windows::Forms::View::Details;	//必须使用Details
			__super::VirtualMode = true;	//使用VirtualMode

			//this->HideSelection = false;	//有问题

			this->m_oTreeListViewNodes = gcnew TreeListViewNode::TreeListViewNodeCollection();	//此TreeListView之下的所以TreeListViewNode
			this->m_nExpandMarkSize = 20;	//给一个初始值，将在OnDrawSubItem()中再度使用Item的高度(e->Bounds.Height)初始化。
		}

	protected:
		~TreeListView(void)
		{
		}

	#pragma region Properties
	public:
	#pragma region 禁止显示的基类的的属性
		//ShowGroups
		[Browsable(false), DefaultValue(false), Localizable(false)]
		property ListViewGroupCollection^ Groups
		{
			ListViewGroupCollection^ get () new
			{
				return __super::Groups;
			}
		}
		//ShowGroups
		[Browsable(false), DefaultValue(false), Localizable(false)]
		property bool OwnerDraw
		{
			bool get () new
			{
				return __super::OwnerDraw;
			}
			void set (bool /*bValue*/) new
			{
				__super::OwnerDraw = true;	//强行使用OwnerDraw
			}
		}
		//ShowGroups
		[Browsable(false), DefaultValue(false), Localizable(false)]
		property bool ShowGroups
		{
			bool get () new
			{
				return __super::ShowGroups;
			}
			void set (bool /*value*/) new
			{
				__super::ShowGroups = false;	//不允许使用Group
			}
		}

		//Sorting
		[Browsable(false), DefaultValue(false), Localizable(false)]
		property System::Windows::Forms::SortOrder Sorting
		{
			System::Windows::Forms::SortOrder get () new
			{
				return __super::Sorting;
			}
			void set (System::Windows::Forms::SortOrder /*value*/) new
			{
				__super::Sorting = System::Windows::Forms::SortOrder::None;	//不排序
			}
		}

		//View
		[Browsable(false), Localizable(false)]
		property System::Windows::Forms::View View
		{
			System::Windows::Forms::View get () new
			{
				return __super::View;
			}
			void set (System::Windows::Forms::View /*value*/) new
			{
				__super::View = System::Windows::Forms::View::Details;	//必须使用Details
			}
		}
		//VirtualMode
		[Browsable(false), DefaultValue(true), Localizable(false)]
		property bool VirtualMode
		{
			bool get () new
			{
				return __super::VirtualMode;
			}
			void set (bool /*value*/) new
			{
				__super::VirtualMode = true;	//强行使用Virtaul模式
			}
		}
	#pragma endregion 禁止显示的基类的的属性

		//此TreeListView之下的所以TreeListViewNode
		[Browsable(false), DefaultValue(true), Localizable(false)]
		property TreeListViewNode::TreeListViewNodeCollection^ Nodes
		{
			TreeListViewNode::TreeListViewNodeCollection^ get ()
			{
				return this->m_oTreeListViewNodes;
			}
		}

	protected:
		//获取指定TreeListViewNode开关Tree的图标的位置
		[Browsable(false), DefaultValue(true), Localizable(false)]
		property System::Drawing::Rectangle PositionOfExpandMark[TreeListViewNode^]
		{
			System::Drawing::Rectangle get (TreeListViewNode^ oNode)
			{
				System::Drawing::Rectangle rcRcd = System::Drawing::Rectangle(0,0,0,0);
				TreeListViewNode::Position^	oPos = cli::safe_cast<TreeListViewNode::Position^>(oNode->Tag);
				if(oPos!=nullptr)
				{
					int	nLevel = oPos->Level;
					int	nXShift = this->m_nExpandMarkSize*nLevel;

					//开关Tree的图标的位置
					System::Drawing::Point	sPoint = System::Drawing::Point(oNode->Bounds.X+nXShift,oNode->Bounds.Y);
					//开关Tree的图标的Size
					System::Drawing::Size	sSize = System::Drawing::Size(this->m_nExpandMarkSize,this->m_nExpandMarkSize);
					rcRcd = System::Drawing::Rectangle(sPoint,sSize);
				}
				return rcRcd;
			}
		}
	#pragma endregion Properties

	#pragma region 内部变量
	private:
		TreeListViewNode::TreeListViewNodeCollection^	m_oTreeListViewNodes;	//此TreeListView之下的所以TreeListViewNode
		int													m_nExpandMarkSize;		//Tree打开/关闭图标的大小 
	#pragma endregion 内部变量

	#pragma region 公开函数
	public:
		//************************************************************************
		//Name: 	ExpandAll
		//Author: 	Zheng XM (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description: 展开所有项及其下属项
		//
		void ExpandAll()
		{
			this->m_oTreeListViewNodes->ExpandAll();
		}
		//************************************************************************
		//Name: 	ExpandSelectedAll
		//Author: 	Zheng XM (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description: 展开当前所选项及其下属项
		//
		void ExpandSelectedAll()
		{
			if((this->SelectedIndices->Count>0)&&
				(this->SelectedIndices[0]>=0))
			{//当前存在选择的项(Node)
				ITLStdLib::TreeListViewNode::Position^ oPos = this->m_oTreeListViewNodes->At[this->SelectedIndices[0],true];
				if((oPos!=nullptr)&&	//Pos存在
					(oPos->TreeNode!=nullptr))	//其Node也存在
				{//所选择的是文件夹Node
					ITLStdLib::TreeListViewNode^ oNode = oPos->TreeNode;
					oNode->ExpandAll();
					this->VirtualListSize = this->m_oTreeListViewNodes->TotalCount[true];
				}
			}
		}

		//************************************************************************
		//Name: 	CollapseAll
		//Author: 	Zheng XM (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description: 关闭所有项及其下属项
		//
		void CollapseAll()
		{
			this->m_oTreeListViewNodes->CollapseAll();
		}

		//************************************************************************
		//Name: 	CollapseSelectedAll
		//Author: 	Zheng XM (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description: 关闭当前所选项及其下属项
		//
		void CollapseSelectedAll()
		{
			if((this->SelectedIndices->Count>0)&&
				(this->SelectedIndices[0]>=0))
			{//当前存在选择的项(Node)
				ITLStdLib::TreeListViewNode::Position^ oPos = this->m_oTreeListViewNodes->At[this->SelectedIndices[0],true];
				if((oPos!=nullptr)&&	//Pos存在
					(oPos->TreeNode!=nullptr))	//其Node也存在
				{//所选择的是文件夹Node
					ITLStdLib::TreeListViewNode^ oNode = oPos->TreeNode;
					oNode->CollapseAll();
					this->VirtualListSize = this->m_oTreeListViewNodes->TotalCount[true];
				}
			}
		}
	#pragma endregion 公开函数

	#pragma region Virtual Functions
	public:
	protected:
		//************************************************************************
		//Name: 	OnExpandNode
		//Author: 	Zheng XM (2011/9/11)
		//Modify: 	
		//Return:  	
		//Description: 展开TreeNode
		//		
		virtual void OnExpandNode(TreeListViewNode^ e)
		{
			e->ShowSubTreeListViewNodes = true;
			this->VirtualListSize = this->Nodes->TotalCount[true];
			//this->Invalidate();
		}
		//************************************************************************
		//Name: 	OnCollapseNode
		//Author: 	Zheng XM (2011/9/11)
		//Modify: 	
		//Return:  	
		//Description: 关闭TreeNode
		//		
		virtual void OnCollapseNode(TreeListViewNode^ e)
		{
			e->ShowSubTreeListViewNodes = false;
			this->VirtualListSize = this->Nodes->TotalCount[true];
			//this->Invalidate();
		}
	#pragma endregion Virtual Functions

	#pragma region Override Functions
	public:
	protected:
		//************************************************************************
		//Name: 	OnMouseClick
		//Author: 	Zheng XM (2011/9/11)
		//Modify: 	
		//Return:  	
		//Description: 处理鼠标点击
		//		
		virtual void OnMouseClick(MouseEventArgs^ e) override
		{
			__super::OnMouseClick(e);

			//获取被点中的ListViewItem
			System::Windows::Forms::ListViewHitTestInfo^	oLVHitTestInfo = this->HitTest(e->X, e->Y);
			if(oLVHitTestInfo!=nullptr)
			{//被点中了，此时的oLVHitTestInfo中的Item正确，Location不可信(按照一般ListView的位置获取的，所以不正确)
				if((TreeListViewNode::typeid)->IsAssignableFrom(oLVHitTestInfo->Item->GetType()))
				{//oLVHitTestInfo->Item是源自TreeListViewNode的Object
					TreeListViewNode^		oNode = cli::safe_cast<TreeListViewNode^>(oLVHitTestInfo->Item);
					if(oNode->SubTreeListViewNodes!=nullptr)
					{//属于可开关的Node
						System::Drawing::Rectangle	rcRect = this->PositionOfExpandMark[oNode];	//此Node的Expand/Collapse图标的位置
						if(rcRect.Contains(e->X, e->Y))
						{//开关图标被点击
							if(!oNode->ShowSubTreeListViewNodes)
							{//目前是打开(Expand)状态
								OnExpandNode(oNode);
							}
							else
							{//目前是关闭(Collapse)状态
								OnCollapseNode(oNode);
							}
						}
					}
				}
			}
		}

		//************************************************************************
		//Name: 	OnDrawColumnHeader
		//Author: 	Zheng XM (2011/9/4)
		//Modify: 	
		//Return:  	
		//Description: 画ColumnHeader
		//		
		virtual void OnDrawColumnHeader(DrawListViewColumnHeaderEventArgs^ e) override
		{
			e->DrawDefault = true;
			__super::OnDrawColumnHeader(e);
		}

		//************************************************************************
		//Name: 	OnDrawItem
		//Author: 	Zheng XM (2011/9/2)
		//Modify: 	
		//Return:  	
		//Description: 画指定项
		//		
		//virtual void OnDrawItem(DrawListViewItemEventArgs^ e) override
		//{
		//	//e->DrawDefault = true;
		//	__super::OnDrawItem(e);
		//}

		//************************************************************************
		//Name: 	OnDrawSubItem
		//Author: 	Zheng XM (2011/9/2)
		//Modify: 	
		//Return:  	
		//Description: 画指定项
		//		
		virtual void OnDrawSubItem(DrawListViewSubItemEventArgs^ e) override
		{
			if(e->ColumnIndex==0)
			{//处理最左边的主项目
				if(e->Item!=nullptr)
				{
					e->DrawDefault = true;	//如果出现非正常情况话，交给系统处理
					if((TreeListViewNode::typeid)->IsAssignableFrom(e->Item->GetType()))
					{//e->Item是源自TreeListViewNode的Object
						TreeListViewNode^			oNode = cli::safe_cast<TreeListViewNode^>(e->Item);
						TreeListViewNode::Position^	oPos = cli::safe_cast<TreeListViewNode::Position^>(oNode->Tag);
						if(oPos!=nullptr)
						{
						#ifdef _DEBUG
							String^	strMsg = String::Format(L"({0},{1}),{2}\n",e->ItemIndex,e->ColumnIndex,e->ItemState);
							Debug::WriteLine(strMsg);
						#endif	//_DEBUG
							e->DrawDefault = false;	//在此处理
							//清除背景
							e->DrawBackground();
							this->m_nExpandMarkSize = e->Bounds.Height;	//再度初始化开关Tree的图标

							int	nLevel = oPos->Level;
							int	nXShift = this->m_nExpandMarkSize*nLevel;
							int	nWidth = e->Bounds.Width - nXShift;

							//Node的位置
							System::Drawing::Point	sPoint = System::Drawing::Point(e->Bounds.X+nXShift,e->Bounds.Y);
							if(oNode->SubTreeListViewNodes!=nullptr)
							{//其下包含有Node--〉需要画"+"或"-"
								VisualStyleRenderer^	oVisualStyleRenderer = nullptr;
								if(oNode->ShowSubTreeListViewNodes)
								{//显示其下属的Node--〉画"-"
									oVisualStyleRenderer = gcnew VisualStyleRenderer(VisualStyleElement::TreeView::Glyph::Opened);
									oNode->ImageIndex = this->SmallImageList->Images->Count-2;	//使用打开的ForlderIcon
								}
								else
								{
									oVisualStyleRenderer = gcnew VisualStyleRenderer(VisualStyleElement::TreeView::Glyph::Closed);
									oNode->ImageIndex = this->SmallImageList->Images->Count-1;	//使用关闭的ForlderIcon
								}
								oVisualStyleRenderer->DrawBackground(e->Graphics, System::Drawing::Rectangle(sPoint, System::Drawing::Size(this->m_nExpandMarkSize,this->m_nExpandMarkSize)));

								//以下代码是获取相关VisualStyle的画面Size的方法
								//System::Drawing::Size sSize = oVisualStyleRenderer->GetPartSize(e->Graphics,System::Windows::Forms::VisualStyles::ThemeSizeType::Draw);
							}
							sPoint.X += this->m_nExpandMarkSize;
							nWidth -= this->m_nExpandMarkSize;

							if(nLevel>0)
							{//画连线，在此需要画本行从左至右的各个Level的竖线
								//预备Pen
								System::Drawing::Pen^	oPen = gcnew System::Drawing::Pen(SystemBrushes::ControlDark, 1.0f);
								oPen->DashStyle = System::Drawing::Drawing2D::DashStyle::Dot;
								//预备Y坐标
								int	nTop = e->Bounds.Y;	//本行的顶部Y坐标
								int	nCenter = e->Bounds.Y + e->Bounds.Height/2;	//本行的上下中间位置的Y坐标
								int	nBottom = e->Bounds.Y + e->Bounds.Height;	//本行的底部Y坐标

								TreeListViewNode::Position^ oTempPos = oPos->SubTreeListViewNodePosition;
								int	nTempLevel = 1;
								while(oTempPos!=nullptr)
								{//循环到此Node的最里层的Position
									int	nRight = e->Bounds.X + nTempLevel*this->m_nExpandMarkSize;
									int	nLeft = nRight - this->m_nExpandMarkSize/2;
									if(oNode->SubTreeListViewNodes==nullptr)
									{//没有加减号时，横线需要长一点
										nRight += 16;	//ForderIcon的宽度
									}
									if(oTempPos->SubTreeListViewNodePosition==nullptr)
									{//到了Position的最低层了
										int	nTempBottom = nBottom;
										if(oTempPos->TreeListViewNodeCollectionNode->Next==nullptr)
										{//此Node属于同一级别中的最后一个Node--〉竖线只画在中央
											nTempBottom = nCenter;
										}
										//画竖线
										e->Graphics->DrawLine(oPen, nLeft,nTop,nLeft,nTempBottom);
										//画横线
										e->Graphics->DrawLine(oPen, nLeft,nCenter,nRight,nCenter);
									}
									else
									{//没到Position的最低层了
										if(oTempPos->TreeListViewNodeCollectionNode->Next!=nullptr)
										{//此Node不是此级别中的最后一个Node--〉竖线只画在中央（从顶部话到底部）
											e->Graphics->DrawLine(oPen, nLeft,nTop,nLeft,nBottom);
										}
										//如果是最后此级别中的最后一个Node就不画竖线
									}
									oTempPos = oTempPos->SubTreeListViewNodePosition;
									nTempLevel++;
								}
							}

							//画Icon
							e->Graphics->DrawImage(this->SmallImageList->Images[oNode->ImageIndex],sPoint);
							sPoint.X += 16;
							nWidth -= 16;

							//画Item的Text
							//Node的Size
							System::Drawing::Size	sSize = System::Drawing::Size(nWidth,e->Bounds.Height);
							System::Drawing::Rectangle sRect = System::Drawing::Rectangle(sPoint,sSize);
							//Text的颜色
							System::Drawing::Brush^ oTextBrush = nullptr;
							if(((e->ItemState&ListViewItemStates::Selected)==ListViewItemStates::Selected)||
								((this->SelectedIndices->Contains(e->ItemIndex))&&(this->Focused)))	//可能是ListView的But，Ex. 在ShowToolTips为true的情况下，当此Item处于被选择状态时，在其上方移动鼠标后，在此接到的不是ListViewItemStates::Selected
							{//选择状态
								System::Drawing::Brush^ oBackBrush = System::Drawing::SystemBrushes::Highlight;
								e->Graphics->FillRectangle(oBackBrush, sRect);

								//决定Text的颜色
								oTextBrush = System::Drawing::SystemBrushes::HighlightText;
							}
							else
							{
								//决定Text的颜色
								oTextBrush = System::Drawing::SystemBrushes::WindowText;
							}
							//画此项目的Text
							e->Graphics->DrawString(e->SubItem->Text,e->SubItem->Font,oTextBrush,sRect);
						}
					}
				}
			}
			else
			{//其他项
				e->DrawDefault = true;	//交给系统处理
			}
			__super::OnDrawSubItem(e);
		}

		//************************************************************************
		//Name: 	OnRetrieveVirtualItem
		//Author: 	Zheng XM (2011/8/30)
		//Modify: 	
		//Return:  	
		//Description: 提供各项的内容
		//		
		virtual void OnRetrieveVirtualItem(RetrieveVirtualItemEventArgs^ e) override
		{
			//获取所需位置的Item的Position
			TreeListViewNode::Position^ oPos = this->m_oTreeListViewNodes->At[e->ItemIndex,true];
			oPos->TreeNode->Tag = oPos;	//通过Tag传递Position
			e->Item = oPos->TreeNode;

			__super::OnRetrieveVirtualItem(e);
		}

		//************************************************************************
		//Name: 	OnCacheVirtualItems
		//Author: 	Zheng XM (2011/8/30)
		//Modify: 	
		//Return:  	
		//Description: 
		//		
		//virtual void OnCacheVirtualItems(CacheVirtualItemsEventArgs^ e) override
		//{
		//	//__super::OnCacheVirtualItems(e);
		//}

		//************************************************************************
		//Name: 	OnSearchForVirtualItem
		//Author: 	Zheng XM (2011/8/30)
		//Modify: 	
		//Return:  	
		//Description: 
		//		
		//virtual void OnSearchForVirtualItem(SearchForVirtualItemEventArgs^ e) override
		//{
		//	//__super::OnSearchForVirtualItem(e);
		//}

		//************************************************************************
		//Name: 	OnVirtualItemsSelectionRangeChanged
		//Author: 	Zheng XM (2011/8/30)
		//Modify: 	
		//Return:  	
		//Description: 
		//		
		//virtual void OnVirtualItemsSelectionRangeChanged(ListViewVirtualItemsSelectionRangeChangedEventArgs^ e) override
		//{
		//	//__super::OnVirtualItemsSelectionRangeChanged(e);
		//}
	#pragma endregion Override Functions

	};
}