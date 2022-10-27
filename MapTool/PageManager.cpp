#include "pch.h"
#include "PageManager.h"

PageManager::PageManager(UINT nIDTemplate, CTreeCtrl* tree, CStatic* selected, CStatic* imageID, CStatic* path)
	: CPropertyPage(nIDTemplate)
	, mTextFormat(nullptr)
	, mBlackBrush(nullptr)
	, mTree(tree)
	, mSelectedItem(selected)
	, mSelectedImageID(imageID)
	, mSelectedPath(path)
	, mSelectedData{}
{

}

PageManager::~PageManager()
{

}

void PageManager::CreateD2D()
{
	try
	{
		mSelectedItem->EnableD2DSupport();

		// Initialize D2D resources:

		// Text Format
		mTextFormat = new CD2DTextFormat(mSelectedItem->GetRenderTarget(), _T("Verdana"), 50);
		mTextFormat->Get()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		mTextFormat->Get()->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		// Brush
		mBlackBrush = new CD2DSolidColorBrush(mSelectedItem->GetRenderTarget(), 0x000000);
	}
	catch (std::bad_alloc& ba)
	{
		CString errMessage = _T("Bad Alloc : ");
		errMessage = ba.what();

		OutputDebugString(errMessage);
	}
	catch (...)
	{
		OutputDebugString(_T("PageManager���� D2D �����ϴ� �߿� ������ �߻��߽��ϴ�.\n"));
	}
}

HRESULT PageManager::AddImage(UINT imageID, CString path)
{
	// �̹��� �ߺ� üũ
	map<UINT, CD2DBitmap*>::iterator temp = mImages.find(imageID);

	if (temp != mImages.end())
		return S_OK;

	// �̹��� ����
	mImages[imageID] = new CD2DBitmap(mSelectedItem->GetRenderTarget(), path);

	if (mImages[imageID] != nullptr)
		return S_OK;

	return E_FAIL;
}

HTREEITEM PageManager::AddItem(CString itemName, HTREEITEM root, ItemData itemData)
{
	static UINT itemID = 0;

	HTREEITEM item = mTree->InsertItem(itemName, root, TVI_LAST);

	if (root != TVI_ROOT)
	{
		mResourcesData[itemID] = itemData;
		mTree->SetItemData(item, itemID++);
	}

	return item;
}

/// <summary>
/// Picture control�� ���õ� Item�� ����Ѵ�.
/// </summary>
void PageManager::PictureRender(ItemData iData)
{
	CHwndRenderTarget* pRT = mSelectedItem->GetRenderTarget();

	CRect rect;

	mSelectedItem->GetClientRect(&rect);

	pRT->BeginDraw();
	pRT->Clear(D2D1::ColorF(D2D1::ColorF::Gray));

	pRT->DrawBitmap(mImages[iData.imageID], rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &iData.imageRect);

	pRT->EndDraw();
}

void PageManager::SelectedItem(LPNMTREEVIEW pNMTreeView)
{
	UINT itemID = mTree->GetItemData(pNMTreeView->itemNew.hItem);
	auto findResult = mResourcesData.find(itemID);
	
	if (findResult != mResourcesData.end())
	{
		mSelectedData = mResourcesData[itemID];

		TCHAR buf[32];

		_itot_s(mSelectedData.ID, buf, 10);

		CString temp = _T("ID : ");
		temp += buf;

		mSelectedImageID->SetWindowText(temp);

		_itot_s(mSelectedData.imageID, buf, 10);

		temp = _T("ImageID : ");
		temp += buf;

		mSelectedPath->SetWindowText(temp);

		PictureRender(mSelectedData);
	}
}

BEGIN_MESSAGE_MAP(PageManager, CPropertyPage)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void PageManager::OnDestroy()
{
	CPropertyPage::OnDestroy();
}

BOOL PageManager::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL PageManager::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// Font
	mSelectedImageID->SetWindowText(_T("ID : sample"));

	mStaticFont.CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("����ü"));

	mSelectedImageID->SetFont(&mStaticFont);
	mSelectedPath->SetFont(&mStaticFont);

	return CPropertyPage::PreCreateWindow(cs);
}