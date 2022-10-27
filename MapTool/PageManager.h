#pragma once

#include "JsonManager.h"
#include <map>

using std::map;

/// <summary>
/// ������ ������ ���� PageManager
/// Page���� �������� ����� �������� ���� ���
/// 
/// 2022. 02. 07 JK
/// </summary>
class PageManager : public CPropertyPage
{
protected:
	// Ÿ�� Resource ����
	map<UINT, ItemData> mResourcesData;
	map<UINT, CD2DBitmap*> mImages;

	// Text Format
	CD2DTextFormat* mTextFormat;	// Tile Index ����� ���� TextFormat

	// Brush
	CD2DSolidColorBrush* mBlackBrush;	// Grid, Text

	// Font
	CFont mStaticFont;

	// Tree Control
	CTreeCtrl* mTree;

	// ���õ� ���
	CStatic* mSelectedItem;
	CStatic* mSelectedImageID;
	CStatic* mSelectedPath;

public:
	// Type
	string mType;

	// ���õ� ����� ItemData
	ItemData	mSelectedData;

protected:
	PageManager() = delete;
	PageManager(UINT nIDTemplate, CTreeCtrl* tree, CStatic* selected, CStatic* imageID, CStatic* path);
	virtual ~PageManager();

protected:
	void CreateD2D();
	HRESULT AddImage(UINT imageID, CString path);
	HTREEITEM AddItem(CString itemName, HTREEITEM root = TVI_ROOT, ItemData itemData = {});
	void PictureRender(ItemData iData);

	void SelectedItem(LPNMTREEVIEW pNMTreeView);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};