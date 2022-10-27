#pragma once

// FMOD
#pragma comment(lib, "..\\lib\\fmod_vc.lib")
#include "..\\inc\\fmod.hpp"

// namespace 
using namespace FMOD;
using namespace std;

// C++ / STL
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <array>

//rapidjson
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"		// for stringify JSON
#include "rapidjson/filereadstream.h"	// fopen������� ����..
#include "rapidjson/filewritestream.h"
#include "rapidjson/error/error.h"		// �����Ǵ��� ���ؼ�
#include "rapidjson/error/en.h"
#include "rapidjson/encodedstream.h"    // AutoUTFInputStream Json���� ���ڵ� �ؼ��� ���ؼ�..

#include "Product.h"
#include "Products.h"

#include <Windows.h>
#include <tchar.h>