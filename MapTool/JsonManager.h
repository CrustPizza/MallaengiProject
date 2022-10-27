#pragma once
#include <vector>
#include <d2d1.h>
using std::vector;

// Json
#include "..\\Library\\Products.h"
#pragma comment(lib, "..\\..\\Output\\Library.lib")
#pragma comment(lib, "d2d1.lib")

/// <summary>
/// ������ Item�� �����͸� �����Ͽ�
/// ��� �� Ÿ�� �Է½ÿ� �����ϱ� ���� ����ü
/// 
/// 2022. 02. 07 JK
/// </summary>
struct ItemData
{
	UINT ID;		// Item ID
	string name;
	vector<INT> group;
	UINT imageID;	// Image ID
	CD2DRectF imageRect;
	UINT layer;		// ���̾� - Sort Order
};

/// <summary>
/// Tile ������ �о���� Ŭ����
/// 
/// 2022. 02. 09 JK
/// </summary>
class ItemJson : public Products
{
public:
	vector<ItemData> mItemList;

public:
	void DataSetup(const rapidjson::Value& obj);
	bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
};

/// <summary>
/// �̹��� �����͸� �ٷ�� ���� ����ü
/// 
/// 2022. 02. 09 JK
/// </summary>
struct ImageData
{
	UINT imageID;
	CString name;
	CString path;
};

/// <summary>
/// �̹��� ������ �о���� Ŭ����
/// 
/// 2022. 02. 18 JK
/// </summary>
class ImageJson : public Products
{
public:
	vector<ImageData> mImageList;

public:
	void DataSetup(const rapidjson::Value& obj);
	bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
};

/// <summary>
/// �� ������ ����ü
/// 
/// 2022. 02. 07 JK
/// </summary>
struct TileData
{
	UINT ID;				// Item ID
	string type;			// Ÿ�� Ÿ��
	UINT index;				// Ÿ�� �ּ�
	vector<INT> group;		// ����� ��ü
	UINT imageID;			// Image ID
	CD2DRectF imageRect;	// �̹��� �������� ����
	UINT layer;				// ���̾� - Sort Order
};

/// <summary>
/// �� ������ �а� �������� Ŭ����
/// 
/// 2022. 02. 18 JK
/// </summary>
class MapJson : public Products
{
public:
	vector<TileData> mTileList;

public:
	void DataSetup(const rapidjson::Value& obj);
	void SaveData(const string& filePath, vector<vector<TileData>> tiles, vector<vector<vector<TileData>>> objects);
	bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
	string MapJson::SerializeToMFC();
};