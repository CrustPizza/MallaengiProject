#pragma once
#include <vector>

using std::vector;

// Json
#include "..\\Library\\Products.h"
#pragma comment(lib, "..\\..\\Output\\Library.lib")

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
/// ���� �̹����� ���̽� Ŭ����
/// 
/// 2022. 02. 16 JK 
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
/// �ݶ��̴� ������ ���� ����ü
/// 
/// 2022. 02. 17 JK
/// </summary>
struct ColliderInfo
{
	D2D1_RECT_F rect;
	INT type;
};

/// <summary>
/// ��������Ʈ ������ �����ϴ� ����ü
/// 
/// 2022. 02. 15 JK
/// </summary>
struct SpriteInfo
{
	UINT imageID;
	D2D1_RECT_F imageRect;
	double duration;
	CPoint pivot;
	ColliderInfo collider;
};

/// <summary>
/// ��������Ʈ ������ ���̽�
/// 
/// 2022. 02. 16 JK
/// </summary>
class SpriteJson : public Products
{
public:
	vector<SpriteInfo> mSpriteList;

public:
	void DataSetup(const rapidjson::Value& obj);
	bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
	string SerializeToMFC();
};