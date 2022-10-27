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

class ImageJson : public Products
{
public:
	vector<ImageData> mImageList;

public:
	void DataSetup(const rapidjson::Value& obj);
	bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
};