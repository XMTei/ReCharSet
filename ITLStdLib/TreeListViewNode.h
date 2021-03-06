// TreeListViewNode.h : ITLTreeListView的项目数据类
// Copyright: ⓒ INF Technologies Ltd., 2011. All Rights Reserved
// Revision:
// 2011.08.29 XiaoMing Zheng  
//
#pragma once

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
	
	//************************************************************************
	//Name: 	TreeListViewNode
	//Author: 	Zheng XM (2011/8/31)
	//Modify: 	
	//Return:  	
	//Description: ITLTreeListView的项目数据类
	//
	public ref class TreeListViewNode : public System::Windows::Forms::ListViewItem
	{
	#pragma region 内部用类
	public:
		//************************************************************************
		//Name: 	Position
		//Author: 	Zheng XM (2011/8/31)
		//Modify: 	
		//Return:  	
		//Description: 对NodeCollection进行操作时的位置信息
		//				
		ref class Position
		{
		#pragma region 此类的初始化和善后处理
		public:
			Position()
			{
				this->m_oTreeListViewNodeCollectionNode = nullptr;
				this->m_oSubTreeListViewNodePosition = nullptr;	//如果此值不是nullptr，要优先将此Position作为当前位置来使用
			}
			~Position()
			{
			}
		#pragma endregion 此类的初始化和善后处理

		#pragma region 公开函数
		public:
			//************************************************************************
			//Name: 	Clone
			//Author: 	Zheng XM (2011/8/31)
			//Modify: 	
			//Return:  	
			//Description: 复制此类，所有值变量都可被复制，而参照变量也被复制
			//
			Position^ Clone()
			{
				Position^ oPos = nullptr;
				if(this->m_oTreeListViewNodeCollectionNode != nullptr)
				{
					//复制当前的数据
					oPos = cli::safe_cast<Position^>(this->MemberwiseClone());
					if(this->m_oSubTreeListViewNodePosition!=nullptr)
					{//复制此下的Position
						oPos->SubTreeListViewNodePosition = this->m_oSubTreeListViewNodePosition->Clone();
					}
					else
					{
						oPos->SubTreeListViewNodePosition = nullptr;
					}
				}
				return oPos;
			}
		#pragma endregion 公开函数

		#pragma region Override函数
			//************************************************************************
			//Name: 	Equals
			//Author: 	Zheng XM (2010/11/18)
			//Modify: 	
			//Return:  	
			//Description: 判断与同类的内容是否相等（同）
			//			所谓相同是指此两Position指向同一位置
			//			
			virtual bool Equals( Object^ obj ) override
			{
				bool bRcd = false;

				if (( obj != nullptr) && (this->GetType() == obj->GetType()) )
				{	//给定类存在，且是同类
					Position^ oPos = cli::safe_cast<Position^>(obj);
					if((oPos!=nullptr)&&
						(oPos->TreeListViewNodeCollectionNode==this->m_oTreeListViewNodeCollectionNode))
					{//TreeListViewNodeCollectionNode指向同一数据（TreeListViewNode）
						bRcd = true;
						if(this->m_oSubTreeListViewNodePosition!=nullptr)
						{//比较此下的Position
							if(oPos->SubTreeListViewNodePosition!=nullptr)
							{
								bRcd = this->m_oSubTreeListViewNodePosition->Equals(oPos->SubTreeListViewNodePosition);
							}
							else
							{
								bRcd = false;
							}
						}
					}
				}
				return bRcd;
			}
		#pragma endregion Override函数

		#pragma region Properties
		public:
			//所在NodeCollection列表中的一个要素的LinkedListNode<TreeListViewNode^>
			[Browsable(false), Localizable(false)]
			property LinkedListNode<TreeListViewNode^>^ TreeListViewNodeCollectionNode
			{
				LinkedListNode<TreeListViewNode^>^ get()
				{
					return this->m_oTreeListViewNodeCollectionNode;
				}
				void set(LinkedListNode<TreeListViewNode^>^ oValue)
				{
					this->m_oTreeListViewNodeCollectionNode = oValue;
				}
			}

			//m_oTreeListViewNodeCollectionNode之下的TreeListViewNode的位置,如果此值不是nullptr，要优先将此Position作为当前位置来使用
			[Browsable(false), Localizable(false)]
			property Position^ SubTreeListViewNodePosition
			{
				Position^ get()
				{
					return this->m_oSubTreeListViewNodePosition;
				}
				void set(Position^ oValue)
				{
					this->m_oSubTreeListViewNodePosition = oValue;
				}
			}

			//此位置所指向的TreeListViewNode
			//注意：Position中包含有其下的ePosition，此Property获取的是最底层的TreeListViewNode
			[Browsable(false), Localizable(false)]
			property TreeListViewNode^ TreeNode
			{
				TreeListViewNode^ get()
				{
					TreeListViewNode^ oRcd = nullptr;
					if(this->m_oTreeListViewNodeCollectionNode!=nullptr)
					{
						if(this->m_oSubTreeListViewNodePosition!=nullptr)
						{//返回m_oSubNodePosition所指向的TreeListViewNode
							oRcd = this->m_oSubTreeListViewNodePosition->TreeNode;
						}
						else
						{//返回m_oTreeListViewNodeCollectionNode所指向的TreeListViewNode
							oRcd = cli::safe_cast<TreeListViewNode^>(this->m_oTreeListViewNodeCollectionNode->Value);
						}
					}
					return oRcd;
				}
			}

			//此位置所指向的TreeListViewNode的Root TreeListViewNode
			//注意：Position中包含有其下的ePosition，此Property获取的是最顶层的TreeListViewNode
			[Browsable(false), Localizable(false)]
			property TreeListViewNode^ TreeListViewRootNode
			{
				TreeListViewNode^ get()
				{
					TreeListViewNode^ oRcd = nullptr;
					if(this->m_oTreeListViewNodeCollectionNode!=nullptr)
					{
						oRcd = cli::safe_cast<TreeListViewNode^>(this->m_oTreeListViewNodeCollectionNode->Value);
					}
					return oRcd;
				}
			}

			//此位置(LinkedListNode<TreeListViewNode^>)的下一个位置(LinkedListNode<TreeListViewNode^>)
			[Browsable(false), Localizable(false)]
			property Position^ Next[bool]	//true:获取下一可视的TreeListViewNode的Postion
			{
				Position^ get(bool bVisibleOnly)
				{
					Position^ oPos = nullptr;
					if(this->m_oTreeListViewNodeCollectionNode!=nullptr)
					{
						//获取当前的位置的TreeListViewNode
						TreeListViewNode^ oCurrentTreeListViewRootNode = this->TreeListViewRootNode;
					#ifdef _DEBUG
						Debug::Assert(oCurrentTreeListViewRootNode!=nullptr,L"当前的位置的TreeListViewNode的值不可为nulltr");
					#endif	//_DEBUG
						if(oCurrentTreeListViewRootNode!=nullptr)
						{
							//获取同一级的下一TreeListViewNodeCollectionNode
							LinkedListNode<TreeListViewNode^>^ oNextTreeListViewNodeCollectionNodeInThisLevel = this->m_oTreeListViewNodeCollectionNode->Next;
							//获取此下一级的TreeListViewNodePosition
							Position^	oNextSubTreeListViewNodePosition = nullptr;	//初始值为nullptr
							if(!(bVisibleOnly&&(!oCurrentTreeListViewRootNode->ShowSubTreeListViewNodes)))
							{//在非（只获取可视TreeListViewNode且当前TreeListViewNode之下Node为可视状态）的时候，需要获取当前TreeListViewNode之下的TreeListViewNode
								if(this->m_oSubTreeListViewNodePosition!=nullptr)
								{//目前掌握有指向下属的TreeListViewNode的Position，使用此子级Position获取其下的下一个TreeListViewNodePosition
									oNextSubTreeListViewNodePosition = this->m_oSubTreeListViewNodePosition->Next[bVisibleOnly];
								}
								else
								{//目前没有指向下属的TreeListViewNode的Position，检查是否需要指向下属的TreeListViewNode
									if(oCurrentTreeListViewRootNode->SubTreeListViewNodes!=nullptr)
									{//拿到下属第一个TreeListViewNode的Position
										oNextSubTreeListViewNodePosition = oCurrentTreeListViewRootNode->SubTreeListViewNodes->FirstTreeListViewNode;
									}
								}
							}
							if(oNextSubTreeListViewNodePosition==nullptr)
							{//目前指向下属的下一个TreeListViewNode不存在，需要指向此级别的下一个
								if(oNextTreeListViewNodeCollectionNodeInThisLevel!=nullptr)
								{
									oPos = gcnew Position();
									oPos->TreeListViewNodeCollectionNode = oNextTreeListViewNodeCollectionNodeInThisLevel;
									oPos->SubTreeListViewNodePosition = nullptr;
								}
								//如果oNextTreeListViewNodeCollectionNodeInThisLevel也是nullptr的话就返回nullptr
							}
							else
							{//下属的下一个TreeListViewNode存在
								oPos = gcnew Position();
								oPos->TreeListViewNodeCollectionNode = this->m_oTreeListViewNodeCollectionNode;	//使用当前此级别的TreeListViewNodeCollectionNode
								oPos->SubTreeListViewNodePosition = oNextSubTreeListViewNodePosition;
							}
						}
					}
					return oPos;
				}
			}

			//此位置(LinkedListNode<TreeListViewNode^>)的前一个位置(LinkedListNode<TreeListViewNode^>)
			//[Browsable(false), Localizable(false)]
			//property Position^ Previous
			//{
			//	Position^ get()
			//	{
			//		Position^ oPos = nullptr;
			//		return oPos;
			//	}
			//}

			//此位置(LinkedListNode<TreeListViewNode^>)的级别深度
			[Browsable(false), Localizable(false)]
			property int Level
			{
				int get()
				{
					int	nRcd = 0;
					Position^ oTempPos = this->m_oSubTreeListViewNodePosition;
					while(oTempPos!=nullptr)
					{
						oTempPos = oTempPos->SubTreeListViewNodePosition;
						nRcd++;
					}
					return nRcd;
				}
			}

			//此位置的TreeListViewNode是否是该Node所在级别中的最后一个Node
			//[Browsable(false), Localizable(false)]
			//property bool IsLastTreeListViewNodeInItsLevel
			//{
			//	bool get()
			//	{
			//		bool	bRcd = true;
			//		if(this->m_oTreeListViewNodeCollectionNode!=nullptr)
			//		{
			//			if(this->m_oSubTreeListViewNodePosition!=nullptr)
			//			{
			//				bRcd = this->m_oSubTreeListViewNodePosition->IsLastTreeListViewNodeInItsLevel;
			//			}
			//			else
			//			{
			//				if(this->m_oTreeListViewNodeCollectionNode->Next!=nullptr)
			//				{
			//					bRcd = false;
			//				}
			//			}
			//		}
			//		return bRcd;
			//	}
			//}
		#pragma endregion Properties

		#pragma region 内部变量
		private:
			LinkedListNode<TreeListViewNode^>^	m_oTreeListViewNodeCollectionNode;	//所在NodeCollection列表中的一个要素的LinkedListNode<TreeListViewNode^>
			Position^								m_oSubTreeListViewNodePosition;					//m_oTreeListViewNodeCollectionNode之下的TreeListViewNode的位置
		#pragma endregion 内部变量
		};

		//************************************************************************
		//Name: 	TreeListViewNodeCollection
		//Author: 	Zheng XM (2011/8/31)
		//Modify: 	
		//Return:  	
		//Description: 一组TreeListViewItem
		//
		ref class TreeListViewNodeCollection : public LinkedList<TreeListViewNode^>
		{
		#pragma region 内部用类
		public:
		#pragma endregion 内部用类

		#pragma region 此类的初始化和善后处理
		public:
			TreeListViewNodeCollection()
			{
			}
			~TreeListViewNodeCollection()
			{
			}
		#pragma endregion 此类的初始化和善后处理

		#pragma region 公开函数
		public:
			//************************************************************************
			//Name: 	Clone
			//Author: 	Zheng XM (2011/8/31)
			//Modify: 	
			//Return:  	
			//Description: 复制此类，所有值变量都可被复制，而参照变量也被复制
			//
			TreeListViewNodeCollection^ Clone()
			{
				return cli::safe_cast<TreeListViewNodeCollection^>(this->MemberwiseClone());
			}

			//************************************************************************
			//Name: 	ExpandAll
			//Author: 	Zheng XM (2011/9/13)
			//Modify: 	
			//Return:  	
			//Description: 展开此项下所有的Node
			//
			void ExpandAll()
			{
				for each(TreeListViewNode^ oNode in this)
				{
					oNode->ExpandAll();
				}
			}

			//************************************************************************
			//Name: 	CollapseAll
			//Author: 	Zheng XM (2011/9/13)
			//Modify: 	
			//Return:  	
			//Description: 关闭此项下所有的Node
			//
			void CollapseAll()
			{
				for each(TreeListViewNode^ oNode in this)
				{
					oNode->CollapseAll();
				}
			}
		#pragma endregion 公开函数

		#pragma region Properties
		public:
			//第一个TreeListViewNode
			[Browsable(false), Localizable(false)]
			property Position^ FirstTreeListViewNode
			{
				Position^ get()
				{
					Position^ oPos = nullptr;
					if(this->First!=nullptr)
					{
						oPos = gcnew Position();
						oPos->TreeListViewNodeCollectionNode = this->First;
						oPos->SubTreeListViewNodePosition = nullptr;
					}
					return oPos;
				}
			}

			//最后一个TreeListViewNode
			[Browsable(false), Localizable(false)]
			property Position^ LastTreeListViewNode
			{
				Position^ get()
				{
					Position^ oPos = nullptr;
					if(this->Last!=nullptr)
					{
						oPos = gcnew Position();
						oPos->TreeListViewNodeCollectionNode = this->Last;
						oPos->SubTreeListViewNodePosition = nullptr;
					}
					return oPos;
				}
			}

			//获取第N个TreeListViewNode.
			[Browsable(false), Localizable(false)]
			property Position^ At[int,bool]
			{
				//nNo:所要获得的TreeListViewNode的序列号(>=0), bVisibleOnly:true时意味着序列号为可视TreeListViewNode的序列号
				Position^ get(int nNo, bool bVisibleOnly)
				{
					Position^ oPos = nullptr;
					if(nNo>=0)
					{
						int	nTempCount = 0;	//从零开始计数
						oPos = this->FirstTreeListViewNode;
						while((nTempCount<nNo)&&(oPos!=nullptr))
						{
							nTempCount++;
							oPos = oPos->Next[bVisibleOnly];
						}
					}
					return oPos;
				}
			}

			//获取其下TreeListViewNode的总个数.
			[Browsable(false), Localizable(false)]
			property int TotalCount[bool]
			{
				//bVisibleOnly:true时返回可视的TreeListViewNode的总个数，false时返回所有TreeListViewNode的总个数
				int get(bool bVisibleOnly)
				{
					int nRcd = 0;
					for each(TreeListViewNode^ oNode in this)
					{
						nRcd += oNode->TotalCount[bVisibleOnly];
					}
					return nRcd;
				}
			}
		#pragma endregion Properties

		};
	#pragma endregion 内部用类

	#pragma region 此类的初始化和善后处理
	public:
		TreeListViewNode()
		{
			this->m_bShowSubTreeListViewNodes = true;	//true:展开此TreeListViewNode之下的TreeListViewNode
			this->m_oSubTreeListViewNodes = nullptr;//此TreeListViewNode之下的TreeListViewNode,nullptr:此下不可能有TreeNode
		}
		~TreeListViewNode()
		{
		}
	#pragma endregion 此类的初始化和善后处理

	#pragma region 公开函数
	public:
		//************************************************************************
		//Name: 	Clone
		//Author: 	Zheng XM (2011/8/31)
		//Modify: 	
		//Return:  	
		//Description: 复制此类，所有值变量都可被复制，而参照变量也被复制
		//
		//virtual Object^ Clone() override
		//{
		//	TreeListViewNode^ oRcd = cli::safe_cast<TreeListViewNode^>(this->MemberwiseClone());
		//	return oRcd;
		//}

		//************************************************************************
		//Name: 	ExpandAll
		//Author: 	Zheng XM (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description: 展开此项下所有的Node
		//
		void ExpandAll()
		{
			if(this->m_oSubTreeListViewNodes!=nullptr)
			{
				this->m_bShowSubTreeListViewNodes = true;
				this->m_oSubTreeListViewNodes->ExpandAll();
			}
		}

		//************************************************************************
		//Name: 	CollapseAll
		//Author: 	Zheng XM (2011/9/13)
		//Modify: 	
		//Return:  	
		//Description: 关闭此项下所有的Node
		//
		void CollapseAll()
		{
			if(this->m_oSubTreeListViewNodes!=nullptr)
			{
				this->m_bShowSubTreeListViewNodes = false;
				this->m_oSubTreeListViewNodes->CollapseAll();
			}
		}
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
		//		TreeListViewNode^ oTreeListViewNode = cli::safe_cast<TreeListViewNode^>(obj);
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
		//此下（包括此）的TreeListViewNode的总数量
		[Browsable(false), Localizable(false)]
		property int TotalCount[bool]	//true:获取可视的TreeListViewNode的总数量,false:获取所有TreeListViewNode的总数量
		{
			int get(bool bVisibleOnly)
			{
				int	nRcd = 1;	//此TreeListViewNode
				bool	bCountAll = true;

				if(bVisibleOnly)
				{//上方要求累加可视Node
					if(!this->m_bShowSubTreeListViewNodes)
					{//目前此Node下属Node处于非可视状态
						bCountAll = false;	//不累计其下的Node数量
					}
				}
				if((bCountAll)&&(this->m_oSubTreeListViewNodes!=nullptr))
				{//累计其下的Node的数量
					nRcd += this->m_oSubTreeListViewNodes->TotalCount[bVisibleOnly];	//累加
				}
				return nRcd;
			}
		}

		//显示此下的TreeListViewNode
		[Browsable(false), Localizable(false)]
		property bool ShowSubTreeListViewNodes
		{
			bool get()
			{
				return this->m_bShowSubTreeListViewNodes;
			}
			void set(bool bValue)
			{
				this->m_bShowSubTreeListViewNodes = bValue;	//只改变此级的展开和关闭的属性，保留其下SubNodes的ShowSubNodes的属性不变
			}
		}

		//此TreeListViewNode之下的TreeListViewNode,nullptr:此下不可能有TreeNode
		[Browsable(false), Localizable(false)]
		property TreeListViewNodeCollection^ SubTreeListViewNodes
		{
			TreeListViewNodeCollection^ get()
			{
				return this->m_oSubTreeListViewNodes;
			}
		}
	protected:
	#pragma endregion Properties

	#pragma region 内部变量
	protected:
		bool							m_bShowSubTreeListViewNodes;		//是否显示此下的TreeListViewNode
		TreeListViewNodeCollection^		m_oSubTreeListViewNodes;			//此TreeListViewNode之下的TreeListViewNode
	#pragma endregion 内部变量
	};
}