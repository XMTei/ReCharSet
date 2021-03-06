// ITLStdLib.h : 各种公用类的包
// Copyright: ⓒ INF Technologies Ltd., 2011. All Rights Reserved
// Revision:
// 2011.09.7 XiaoMing Zheng  
//	有一个ITLStdLib35的Project是基于.NET3.5。此Project中的头文件都是和3.5Project
//	共用的，共用通过SourceSafe的Share实现
//

#pragma once

//基类为Object的类（自定义数据类）
#include "TextFileEncordingInfo.h"	//描述Text文件的Encording（CharSet）的类
#include "ToolStirpItemsStatusContainer.h"	//记录一系列ToolStripItem，根据需要将它们的Enable属性设置为false(Push)，或回复被Push之前的Enable的状态

//直接调用API或COM的相关类
#include "ITLMLang.h"	//使用IE的MLang
#include "ITLShell32.h"	//使用API的Shell32

#include "TreeListView.h"			//以ListView为基类的TreeListView，包含其使用的ITLTreeListViewNode
#include "TreeListViewDirectoryNode.h"	//以ITLTreeListViewNode为基类的显示Dirctory用的TreeListView的Node
#include "TreeListViewFileNode.h"		//以ITLTreeListViewNode为基类的显示File用的TreeListView的Node
#include "TreeListViewTextFileNode.h"	//以ITLTreeListViewFileNode为基类的显示Text File用的TreeListView的Node
#include "ToolStripCheckBox.h"			//ToolStrip上用的CheckBox
#include "ToolStripTrackBar.h"			//ToolStrip上用的TrackBar
#include "ToolStripMultiProgressBarButton.h"	//在ToolStripButton上显示多个进度条(最大4个)

//其他源自.NET非控件类的类
#include "TextEncoding.h"			//System.Text.Encoding相关类

using namespace System;

namespace ITLStdLib
{

}
