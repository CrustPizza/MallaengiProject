
#include "pch.h"
#include "framework.h"
#include "MainFrm.h"
#include "SpriteView.h"
#include "Resource.h"
#include "SpriteTool.h"

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CSpriteView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = nullptr) noexcept : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// 积己/家戈
//////////////////////////////////////////////////////////////////////

CSpriteView::CSpriteView() noexcept
	: mCount(1)
{
	mCurrSort = ID_SORTING_GROUPBYTYPE;
}

CSpriteView::~CSpriteView()
{
}

BEGIN_MESSAGE_MAP(CSpriteView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 皋矫瘤 贸府扁

int CSpriteView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 轰甫 父奠聪促.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!mWndSpriteView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("胶橇扼捞飘 轰甫 父甸瘤 给沁嚼聪促.\n");
		return -1;      // 父甸瘤 给沁嚼聪促.
	}

	// 捞固瘤甫 肺靛钦聪促.
	mSpriteViewImages.Create(IDB_CLASS_VIEW, 16, 0, RGB(255, 0, 255));
	mWndSpriteView.SetImageList(&mSpriteViewImages, TVSIL_NORMAL);

	//mWndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	//mWndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* 泪陛 */);

	OnChangeVisualStyle();

	//mWndToolBar.SetPaneStyle(mWndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	//mWndToolBar.SetPaneStyle(mWndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	//mWndToolBar.SetOwner(this);

	//// 葛电 疙飞篮 何葛 橇饭烙捞 酒囱 捞 牧飘费阑 烹秦 扼快泼邓聪促.
	//mWndToolBar.SetRouteCommandsViaFrame(FALSE);

	//CMenu menuSort;
	//menuSort.LoadMenu(IDR_POPUP_SORT);

	//mWndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	//CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, mWndToolBar.GetButton(0));

	//if (pButton != nullptr)
	//{
	//	pButton->m_bText = FALSE;
	//	pButton->m_bImage = TRUE;
	//	pButton->SetImage(GetCmdMgr()->GetCmdImage(mCurrSort));
	//	pButton->SetMessageWnd(this);
	//}

	// 沥利 飘府 轰 单捞磐甫 歹固 内靛肺 盲矿聪促.
	FillSpriteView();

	return 0;
}

void CSpriteView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CSpriteView::FillSpriteView()
{
	mWndSpriteView.hRoot = mWndSpriteView.InsertItem(_T("胶橇扼捞飘 捞固瘤"), 0, 0);
	mWndSpriteView.SetItemState(mWndSpriteView.hRoot, TVIS_BOLD, TVIS_BOLD);
}

void CSpriteView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&mWndSpriteView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 努腐茄 亲格阑 急琶钦聪促.
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != nullptr)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CSpriteView::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	//int cyTlb = mWndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	//mWndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	mWndSpriteView.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CSpriteView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CSpriteView::OnSort(UINT id)
{
	if (mCurrSort == id)
	{
		return;
	}

	mCurrSort = id;

	//CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, mWndToolBar.GetButton(0));

	//if (pButton != nullptr)
	//{
	//	pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
	//	mWndToolBar.Invalidate();
	//	mWndToolBar.UpdateWindow();
	//}
}

void CSpriteView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == mCurrSort);
}

void CSpriteView::Init()
{
	mCount = 1;
	mSpriteList.clear();
	mWndSpriteView.DeleteAllItems();
	FillSpriteView();
}

void CSpriteView::Insert(UINT imageID, const CD2DRectF& imageRect, double duration, CPoint pivot, const ColliderInfo& collider)
{
	char buf[16];
	ZeroMemory(buf, 16);
	_itoa_s<16>(mCount, buf, 10);

	CString str;
	str = buf;

	mWndSpriteView.InsertItem(str, 1, 1, mWndSpriteView.hRoot);
	mWndSpriteView.Expand(mWndSpriteView.hRoot, TVE_EXPAND);
	mSpriteList[mCount] = { imageID, imageRect, duration, pivot, collider };

	mCount++;
}

void CSpriteView::SaveFile(const string& path)
{
	SpriteJson spriteJson;

	for (auto iter = mSpriteList.begin(); iter != mSpriteList.end(); iter++)
		spriteJson.mSpriteList.push_back(iter->second);

	spriteJson.SerializeToFile(path);
}

string CSpriteView::SaveFile()
{
	SpriteJson spriteJson;

	for (auto iter = mSpriteList.begin(); iter != mSpriteList.end(); iter++)
		spriteJson.mSpriteList.push_back(iter->second);

	return spriteJson.SerializeToMFC();
}

void CSpriteView::LoadFile(const string& path)
{
	SpriteJson spriteJson;
	spriteJson.DeserializeFromFile(path);

	for (auto iter = spriteJson.mSpriteList.begin(); iter != spriteJson.mSpriteList.end(); iter++)
		Insert(iter->imageID, iter->imageRect, iter->duration, iter->pivot, iter->collider);
}

void CSpriteView::LoadFileForMFC(const string& data)
{
	SpriteJson spriteJson;
	spriteJson.DeserializeFromData(data);

	for (auto iter = spriteJson.mSpriteList.begin(); iter != spriteJson.mSpriteList.end(); iter++)
		Insert(iter->imageID, iter->imageRect, iter->duration, iter->pivot, iter->collider);
}

void CSpriteView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("糕滚 窃荐 眠啊..."));
}

void CSpriteView::OnClassAddMemberVariable()
{
	// TODO: 咯扁俊 疙飞 贸府扁 内靛甫 眠啊钦聪促.
}

void CSpriteView::OnClassDefinition()
{
	// TODO: 咯扁俊 疙飞 贸府扁 内靛甫 眠啊钦聪促.
}

void CSpriteView::OnClassProperties()
{
	// TODO: 咯扁俊 疙飞 贸府扁 内靛甫 眠啊钦聪促.
}

void CSpriteView::OnNewFolder()
{
	AfxMessageBox(_T("货 弃歹..."));
}

void CSpriteView::OnPaint()
{
	CPaintDC dc(this); // 弊府扁甫 困茄 叼官捞胶 牧咆胶飘涝聪促.

	CRect rectTree;
	mWndSpriteView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CSpriteView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	mWndSpriteView.SetFocus();
}

LRESULT CSpriteView::OnChangeActiveTab(WPARAM, LPARAM)
{
	return LRESULT();
}

void CSpriteView::OnChangeVisualStyle()
{
	//mWndToolBar.CleanUpLockedImages();
	//mWndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 泪陛 */);

	mSpriteViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("厚飘甘阑 肺靛且 荐 绝嚼聪促. %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	mSpriteViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	mSpriteViewImages.Add(&bmp, RGB(255, 0, 255));

	mWndSpriteView.SetImageList(&mSpriteViewImages, TVSIL_NORMAL);
}

BEGIN_MESSAGE_MAP(CSpriteViewTree, CViewTree)
ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CSpriteViewTree::OnTvnSelchanged)
END_MESSAGE_MAP()

void CSpriteViewTree::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 咯扁俊 牧飘费 舅覆 贸府扁 内靛甫 眠啊钦聪促.

	if (hRoot != pNMTreeView->itemNew.hItem)
	{
		CMainFrame* mainFrame = dynamic_cast<CMainFrame*>(theApp.m_pMainWnd);

		if (mainFrame != nullptr)
		{
			int index = atoi(CT2CA(GetItemText(pNMTreeView->itemNew.hItem)));
			mainFrame->mWndAnimationView.SetTargetSprite(index);
			mainFrame->mWndProperties.SetSpriteProperty(&mainFrame->mWndSpriteView.mSpriteList[index]);
		}
	}

	*pResult = 0;
}