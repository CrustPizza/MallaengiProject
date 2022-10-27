#pragma once

#include "Terrain.h"
#include "Object.h"

// ControlProperty

enum class eItemType
{
	NONE,
	Terrain,
	Object
};

/// <summary>
/// Page �����ϴ� Sheet Ŭ����
/// 
/// 2022. 02. 07 JK
/// </summary>
class ControlProperty : public CPropertySheet
{
	DECLARE_DYNAMIC(ControlProperty)

#pragma region MyVariable
private:
	Terrain mTerrain;
	Object	mObject;

#pragma endregion

public:
	ControlProperty(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	ControlProperty(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	virtual ~ControlProperty();

#pragma region MyFunction
	const ItemData& GetSelectedItemData();
	eItemType GetItemType();
	const string& GetItemTypeString();
#pragma endregion

protected:
	DECLARE_MESSAGE_MAP()
public:
};