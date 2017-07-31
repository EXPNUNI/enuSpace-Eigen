#pragma once

#ifndef _GLOBAL_HEADER_
#define _GLOBAL_HEADER_


#include <string>
#include <map>
#include <Eigen/Eigen>
using namespace Eigen;

#define SYMBOL_NONE					-1
#define SYMBOL_MATRIX				0

#define MATRIX_un					-1
#define MATRIX_f					0
#define MATRIX_d					1
#define MATRIX_i					2
#define MATRIX_cf					3
#define MATRIX_cd					4

struct ItemMatrix
{
	int type;
	void* pMatrix;
	
	public:ItemMatrix()
	{
		type = MATRIX_f;
		pMatrix = nullptr;
	}
};

struct ObjectInfo
{
	std::string id;				

	int type;					
	void* pItem;

	public:ObjectInfo()
	{
		type = SYMBOL_NONE;
		pItem = nullptr;
	}
};

extern std::string g_strDllPath;
extern std::map<std::string, ObjectInfo* > m_ObjectMapList;

#endif