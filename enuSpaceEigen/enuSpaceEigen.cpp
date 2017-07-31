// enuSpaceEigen.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "enuSpaceEigen.h"

#include "GlobalHeader.h"
#include "utility_functions.h"
#include <string>


#include <Eigen/Eigen>
using namespace Eigen;


//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CenuSpaceEigenApp

BEGIN_MESSAGE_MAP(CenuSpaceEigenApp, CWinApp)
END_MESSAGE_MAP()


// CenuSpaceEigenApp construction

CenuSpaceEigenApp::CenuSpaceEigenApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CenuSpaceEigenApp object

CenuSpaceEigenApp theApp;


// CenuSpaceEigenApp initialization

BOOL CenuSpaceEigenApp::InitInstance()
{
	CWinApp::InitInstance();

	HINSTANCE hInstance = AfxGetInstanceHandle();
	wchar_t szPath[MAX_PATH];
	GetModuleFileName(hInstance, szPath, MAX_PATH);

	wchar_t drive[MAX_PATH];               // ����̺� ��
	wchar_t dir[MAX_PATH];                 // ���丮 ���
	wchar_t fname[MAX_PATH];			   // ���ϸ�
	wchar_t ext[MAX_PATH];                 // Ȯ���� ��

	_wsplitpath_s(szPath, drive, dir, fname, ext);

	std::string strMessage;
	std::string strTenMessage = string_format("Eigen -> %s", strMessage.c_str());

	CString strDllPath;
	strDllPath.Format(L"%s%s", drive, dir);
	g_strDllPath = CStringToString(strDllPath);

	return TRUE;
}


void(*g_fcbSetValue)(wchar_t*, double) = NULL;
VariableStruct(*g_fcbGetValue)(wchar_t*) = NULL;
void(*g_fcbSetArrayValue)(wchar_t*, void*, int, int) = NULL;
void(*g_fcbSetReShapeArrayValue)(wchar_t*, void*, int, int) = NULL;
VariableStruct(*g_fcbGetArrayValue)(wchar_t*) = NULL;
void(*g_fcbPrintMessage)(wchar_t*) = NULL;

CMap<CString, LPCWSTR, VariableStruct*, VariableStruct*> g_DBMapList;

//////////////////////////////////////////////////////////////////////////////////////////////
// enuSpace defualt interface functions.
extern "C" __declspec(dllexport) void SetCallBack_SetValue(void fcbSetValue(wchar_t*, double));
extern "C" __declspec(dllexport) void SetCallBack_GetValue(VariableStruct fcbGetValue(wchar_t*));
extern "C" __declspec(dllexport) void SetCallBack_SetArrayValue(void fcbSetArrayValue(wchar_t*, void*, int, int));
extern "C" __declspec(dllexport) void SetCallBack_GetArrayValue(VariableStruct fcbGetArrayValue(wchar_t*));
extern "C" __declspec(dllexport) void SetCallBack_SetReShapeArrayValue(void fcbSetReShapeArrayValue(wchar_t*, void*, int, int));
extern "C" __declspec(dllexport) void SetCallBack_PrintMessage(void fcbPrintMessage(wchar_t*));

extern "C" __declspec(dllexport) int GetTaskType();
extern "C" __declspec(dllexport) bool IsEnableTransfer(wchar_t* pFromType, wchar_t* pToType);

extern "C" __declspec(dllexport) bool OnInit();
extern "C" __declspec(dllexport) bool OnLoad();
extern "C" __declspec(dllexport) bool OnUnload();
extern "C" __declspec(dllexport) bool OnTask();

extern "C" __declspec(dllexport) void OnEditComponent(wchar_t* pStrSymbolName, wchar_t* pStrID);
extern "C" __declspec(dllexport) void OnShowComponent(wchar_t* pStrSymbolName, wchar_t* pStrID);
//////////////////////////////////////////////////////////////////////////////////////////////
// component call functions.




//////////////////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) void SetCallBack_SetValue(void fcbSetValue(wchar_t*, double))
{
	g_fcbSetValue = fcbSetValue;
}

extern "C" __declspec(dllexport) void SetCallBack_GetValue(VariableStruct fcbGetValue(wchar_t*))
{
	g_fcbGetValue = fcbGetValue;
}

extern "C" __declspec(dllexport) void SetCallBack_SetArrayValue(void fcbSetArrayValue(wchar_t*, void*, int, int))
{
	g_fcbSetArrayValue = fcbSetArrayValue;
}

extern "C" __declspec(dllexport) void SetCallBack_GetArrayValue(VariableStruct fcbGetArrayValue(wchar_t*))
{
	g_fcbGetArrayValue = fcbGetArrayValue;
}

extern "C" __declspec(dllexport) void SetCallBack_SetReShapeArrayValue(void fcbSetReShapeArrayValue(wchar_t*, void*, int, int))
{
	g_fcbSetReShapeArrayValue = fcbSetReShapeArrayValue;
}
extern "C" __declspec(dllexport) void SetCallBack_PrintMessage(void fcbPrintMessage(wchar_t*))
{
	g_fcbPrintMessage = fcbPrintMessage;
}

// �������̽� �� ������ ����ü Ŭ���� ����.
void InterfaceDataMapClear()
{
	POSITION mappos = g_DBMapList.GetStartPosition();
	while (mappos)
	{
		VariableStruct *pObject = NULL;
		CString srtVar = L"";
		g_DBMapList.GetNextAssoc(mappos, srtVar, pObject);
		if (srtVar != L"")
		{
			if (pObject)
			{
				delete pObject;
			}
		}
	}
	g_DBMapList.RemoveAll();
}

// enuSpace�� �޸� ����� �ٸ����, enuSpace�� �޸𸮸� ���Ҵ��� �� �����ϴ� �Լ�
// strVariable=ID_OBJECT.input[12][12] (�迭�� SHAPE ���� ����) pSrc=���� �������� ������, iType=���� �������� Ÿ��, iSize=���� �������� ������
// �Էµ� �迭�� ������ enuSpace�� �迭 ������ ��
void SetReShapeArrayValue(std::string strVariable, void* pSrc, int iType, int iSize)
{
	VariableStruct* pData = NULL;
	std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());

	CString strVal = (wchar_t*)widestr.c_str();

	CString strDim;
	int iPos = strVal.Find(L"[");
	if (iPos > 1)
	{
		strDim = strVal.Right(strVal.GetLength() - iPos);
		strVal = strVal.Left(iPos);
	}

	g_DBMapList.Lookup(strVal, pData);
	if (pData)
	{
		// ��û�� ������ ������ Ÿ�� �� �迭�� ���̰� �����ϴٸ�, ���� ������Ʈ ����.
		if (pData->array.size == iSize && pData->type == iType)
		{
			int itemSize = 0;
			void* pTarget = NULL;
			switch (iType)
			{
			case DEF_BOOL:
			{
				itemSize = iSize * sizeof(bool);
				pTarget = ((bool*)pData->pValue);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_INT:
			{
				itemSize = iSize * sizeof(int);
				pTarget = ((int*)pData->pValue);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_FLOAT:
			{
				itemSize = iSize * sizeof(float);
				pTarget = ((float*)pData->pValue);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_DOUBLE:
			{
				itemSize = iSize * sizeof(double);
				pTarget = ((double*)pData->pValue);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_STRING:
			{
				//*(CString*)pTarget = *((CString*)pData->pValue + iIndex);
				//break;
			}
			}
		}
		// �޸��� ������ Ÿ�� �� �迭�� ����� �ٸ��ٸ�, �� ����Ʈ���� ���� ���� �� �޸� RESHAPE �� �� ������Ʈ �Լ� ȣ��
		else
		{
			delete pData;
			pData = NULL;
			g_DBMapList.RemoveKey(strVal);

			if (g_fcbSetReShapeArrayValue)
			{
				g_fcbSetReShapeArrayValue((wchar_t*)widestr.c_str(), pSrc, iType, iSize);
			}
			return;
		}
	}

	// �� ����Ʈ���� �˻��� �������� ���Ͽ��ٸ�, ���� ������Ʈ ��û��. �� ������Ʈ ��û�Լ��� �������� Ÿ�� �� ARRAY ������ �ٸ� ���
	// �޸��� ReShape ������ ���� ������Ʈ ����
	// ����� �޸��� ������ �ʸ���Ʈ�� �߰��Ͽ�, ���� ���� ����� ���� ó���� �� �ֵ��� �߰���.
	if (pData == NULL)
	{
		if (g_fcbGetArrayValue)
		{
			std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());
			VariableStruct Data = g_fcbGetArrayValue(strVal.GetBuffer(0));
			if (Data.pValue)
			{
				// ������ �������� ������ �� Ÿ���� ��� �� ������Ʈ ����.
				if (Data.type == iType && Data.array.size == iSize)
				{
					int itemSize = 0;
					void* pTarget = NULL;
					switch (iType)
					{
					case DEF_BOOL:
					{
						itemSize = iSize * sizeof(bool);
						pTarget = ((bool*)Data.pValue);
						memcpy(pTarget, pSrc, itemSize);
						break;
					}
					case DEF_INT:
					{
						itemSize = iSize * sizeof(int);
						pTarget = ((int*)Data.pValue);
						memcpy(pTarget, pSrc, itemSize);
						break;
					}
					case DEF_FLOAT:
					{
						itemSize = iSize * sizeof(float);
						pTarget = ((float*)Data.pValue);
						memcpy(pTarget, pSrc, itemSize);
						break;
					}
					case DEF_DOUBLE:
					{
						itemSize = iSize * sizeof(double);
						pTarget = ((double*)Data.pValue);
						memcpy(pTarget, pSrc, itemSize);
						break;
					}
					case DEF_STRING:
					{
						//*(CString*)pTarget = *((CString*)pData->pValue + iIndex);
						//break;
					}
					}
					// ����Ʈ �߰� 
					VariableStruct* pNewData = new VariableStruct;
					*pNewData = Data;
					g_DBMapList.SetAt(strVal, pNewData);
				}
				// �Էµ� ������, Ÿ���� enuSpace�� ������, Ÿ���� �ٸ��ٸ� RESHAPE �� �� ������Ʈ ����
				else
				{
					if (g_fcbSetReShapeArrayValue)
					{
						g_fcbSetReShapeArrayValue((wchar_t*)widestr.c_str(), pSrc, iType, iSize);
					}
				}
			}
			else
			{
				std::string msg = string_format("error : SetArrayValue (Unknown Variable id(%s)", strVariable.c_str());
				PrintMessage(msg);
				return;			// enuSpace�� ���� ������ ��û��.
			}
		}
	}
}

// �迭�� ���� �Լ� 
// strVarialbe=ID_OBJECT.input[0], pSrc=�����͸� ������ �迭�� �ּ�, iType=�������� Ÿ��, iSize=�������� ������
// ID_OBJECT.input[10]�� ������ 10���� Array�� ������
// ID_OBJECT.input[5], iSize�� ���� 3�� �Է��Ͽ��� ���, 5��° ���� 7��°���� ���縦 ������.
// ���� �������� ���� ���� �������� ���� �Ѿ�� ��� ���� �������� ����.
void SetArrayValue(std::string strVariable, void* pSrc, int iType, int iSize)
{
	VariableStruct* pData = NULL;
	std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());

	CString strVal = (wchar_t*)widestr.c_str();
	CString strDim;

	// ���� ������ �迭�� �̸��� ���ԵǾ� ���� ����. ����ü ������ ���Ե�.
	int iPos = strVal.Find(L"[");
	if (iPos > 1)
	{
		strDim = strVal.Right(strVal.GetLength() - iPos);
		strVal = strVal.Left(iPos);
	}
	g_DBMapList.Lookup(strVal, pData);

	// ���� ���Ͽ� �������� ����ü ������ ���ٸ�, enuSpace�� �ش� ������ ������ �ּҸ� ��û��.
	if (g_fcbSetArrayValue && pData == NULL)
	{
		VariableStruct Data = g_fcbGetArrayValue(strVal.GetBuffer(0));
		if (Data.pValue)
		{
			VariableStruct* pNewData = new VariableStruct;
			*pNewData = Data;
			g_DBMapList.SetAt((wchar_t*)widestr.c_str(), pNewData);

			pData = pNewData;
		}
	}

	// ����� �޸��� ������ �˻��ϰ� ������ �������, ���� ����.
	if (pData && pData->pValue)
	{
		int itemSize = 0;
		void* pTarget = NULL;
		int iIndex = GetArrayIndexFromDimension(pData->array.dimension, strDim);
		if (iIndex + iSize <= pData->array.size)
		{
			switch (iType)
			{
			case DEF_BOOL:
			{
				itemSize = iSize * sizeof(bool);
				pTarget = ((bool*)pData->pValue + iIndex);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_INT:
			{
				itemSize = iSize * sizeof(int);
				pTarget = ((int*)pData->pValue + iIndex);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_FLOAT:
			{
				itemSize = iSize * sizeof(float);
				pTarget = ((float*)pData->pValue + iIndex);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_DOUBLE:
			{
				itemSize = iSize * sizeof(double);
				pTarget = ((double*)pData->pValue + iIndex);
				memcpy(pTarget, pSrc, itemSize);
				break;
			}
			case DEF_STRING:
			{
				//*(CString*)pTarget = *((CString*)pData->pValue + iIndex);
				//break;
			}
			}
		}
	}
}

// �迭������ ���� ��û�ϴ� �Լ�
double GetArrayValue(std::string strVariable)
{
	VariableStruct* pData = NULL;
	std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());

	CString strVal = (wchar_t*)widestr.c_str();
	CString strDim;

	// ���� ������ �迭�� �̸��� ���ԵǾ� ���� ����. ����ü ������ ���Ե�.
	int iPos = strVal.Find(L"[");
	if (iPos > 1)
	{
		strDim = strVal.Right(strVal.GetLength() - iPos);
		strVal = strVal.Left(iPos);
	}
	g_DBMapList.Lookup(strVal, pData);

	if (g_fcbGetArrayValue && pData == NULL)
	{
		std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());
		VariableStruct Data = g_fcbGetArrayValue(strVal.GetBuffer(0));
		if (Data.pValue)
		{
			VariableStruct* pNewData = new VariableStruct;
			*pNewData = Data;
			g_DBMapList.SetAt(strVal, pNewData);

			pData = pNewData;
		}
	}

	double fReturn = 0;
	if (pData && pData->pValue)
	{
		// ���� �迭������ �迭 ������ �Էµ� ��û�� �迭�� ������ �̿��Ͽ� Array�� ��ġ�� ȹ����.
		int iIndex = GetArrayIndexFromDimension(pData->array.dimension, strDim);
		if (iIndex <= pData->array.size)
		{
			switch (pData->type)
			{
			case DEF_INT:
				fReturn = *(int*)pData->pValue;
				break;
			case DEF_FLOAT:
				fReturn = *(float*)pData->pValue;
				break;
			case DEF_DOUBLE:
				fReturn = *(double*)pData->pValue;
				break;
			case DEF_BOOL:
				if (*(bool*)pData->pValue == TRUE)
					fReturn = 1;
				else
					fReturn = 0;
				break;
			case DEF_STRING:
				fReturn = 0;				// NOT SUPPORT
				break;
			default:
				break;
			}
		}
	}
	return fReturn;
}

void SetValue(std::string strVariable, double fValue)
{
	double fReturn = 0;
	VariableStruct* pData = NULL;
	std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());
	g_DBMapList.Lookup((wchar_t*)widestr.c_str(), pData);

	if (g_fcbSetValue && pData == NULL)
	{
		VariableStruct Data = g_fcbGetValue((wchar_t*)widestr.c_str());
		if (Data.pValue)
		{
			VariableStruct* pNewData = new VariableStruct;
			*pNewData = Data;
			g_DBMapList.SetAt((wchar_t*)widestr.c_str(), pNewData);

			pData = pNewData;
		}
	}

	if (pData && pData->pValue)
	{
		switch (pData->type)
		{
		case DEF_INT:
			*(int*)pData->pValue = (int)fValue;
			break;
		case DEF_FLOAT:
			*(float*)pData->pValue = (float)fValue;
			break;
		case DEF_DOUBLE:
			*(double*)pData->pValue = fValue;
			break;
		case DEF_BOOL:
			if (fValue == 1)
				*(bool*)pData->pValue = true;
			else
				*(bool*)pData->pValue = false;
			break;
		case DEF_STRING:

			break;
		default:
			break;
		}
	}
}

double GetValue(std::string strVariable)
{
	double fReturn = 0;
	VariableStruct* pData = NULL;
	std::wstring widestr = std::wstring(strVariable.begin(), strVariable.end());
	g_DBMapList.Lookup((wchar_t*)widestr.c_str(), pData);

	if (g_fcbGetValue && pData == NULL)
	{
		VariableStruct Data = g_fcbGetValue((wchar_t*)widestr.c_str());
		if (Data.type)
		{
			VariableStruct* pNewData = new VariableStruct;
			*pNewData = Data;
			g_DBMapList.SetAt((wchar_t*)widestr.c_str(), pNewData);

			pData = pNewData;
		}
	}

	if (pData && pData->pValue)
	{
		switch (pData->type)
		{
		case DEF_INT:
			fReturn = *(int*)pData->pValue;
			break;
		case DEF_FLOAT:
			fReturn = *(float*)pData->pValue;
			break;
		case DEF_DOUBLE:
			fReturn = *(double*)pData->pValue;
			break;
		case DEF_BOOL:
			if (*(bool*)pData->pValue == TRUE)
				fReturn = 1;
			else
				fReturn = 0;
			break;
		case DEF_STRING:
			fReturn = 0;				// NOT SUPPORT
			break;
		default:
			break;
		}
	}
	return fReturn;
}

void PrintMessage(std::string strMessage)
{
	std::string strTenMessage = string_format("Eigen -> %s", strMessage.c_str());
	if (g_fcbPrintMessage)
	{
		g_fcbPrintMessage(StringToCString(strTenMessage).GetBuffer(0));
	}
}

extern "C" __declspec(dllexport) int GetTaskType()
{
	return TASK_TYPE_FUNCTION_COMPONENT;
}

extern "C" __declspec(dllexport) bool IsEnableTransfer(wchar_t* pFromType, wchar_t* pToType)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) bool OnLoad()
{
	return true;
}

extern "C" __declspec(dllexport) bool OnInit()
{
	ObjectMapClear();
	return true;
}

extern "C" __declspec(dllexport) bool OnTask()
{
	return true;
}

extern "C" __declspec(dllexport) bool OnUnload()
{
	ObjectMapClear();
	return true;
}

extern "C" __declspec(dllexport) void OnEditComponent(wchar_t* pStrSymbolName, wchar_t* pStrID)
{

}
extern "C" __declspec(dllexport) void OnShowComponent(wchar_t* pStrSymbolName, wchar_t* pStrID)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////


bool AddObjectMap(std::string id, ObjectInfo* pObject)
{
	bool bReturn = false;
	if (pObject)
	{
		ObjectInfo* pObj = nullptr;
		const std::map<std::string, ObjectInfo*>::const_iterator aLookup = m_ObjectMapList.find(id);
		const bool bExists = aLookup != m_ObjectMapList.end();
		if (bExists == false)
		{
			m_ObjectMapList.insert(std::pair<std::string, ObjectInfo*>(id, pObject));
			bReturn = true;
		}
		else
			bReturn = false;
	}
	else
		bReturn = false;

	if (bReturn == false)
	{
		std::string msg = string_format("warning : Could not add to list(all object). existed id(%s).", id.c_str());
		PrintMessage(msg);
	}
	return bReturn;
}

void ObjectMapClear()
{
	std::map<std::string, ObjectInfo*>::iterator vit;
	for (vit = m_ObjectMapList.begin(); vit != m_ObjectMapList.end(); ++vit)
	{
		ObjectInfo* pObject = vit->second;
		if (pObject)
		{
			if (pObject->pItem)
			{
				delete pObject->pItem;
				pObject->pItem = nullptr;
			}
		}
		delete pObject;
		pObject = nullptr;
	}
	m_ObjectMapList.clear();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// function interface

extern "C" __declspec(dllexport) void Eigen_Matrix(wchar_t* id, FuncParam* pParam)
{

}

// MATRIX_C=Eigen_Addition(MATRIX_A,MATRIX_B)
extern "C" __declspec(dllexport) void Eigen_Addition(wchar_t* id, FuncParam* pParam)
{
	if (pParam)
	{
		int MatrixAType = DEF_UNKNOWN;
		int MatrixA_Row = 0;
		int MatrixA_Col = 0;
		bool bMatA = false;

		int MatrixBType = DEF_UNKNOWN;
		int MatrixB_Row = 0;
		int MatrixB_Col = 0;
		bool bMatB = false;

		int MatrixCType = DEF_UNKNOWN;
		int MatrixC_Row = 0;
		int MatrixC_Col = 0;
		bool bMatC = false;

		std::string ObjId = CStringToString(id);
		std::string variable;
		variable = ObjId + ".MATRIX_C";

		const VariableStruct* pMatrixA = pParam->GetParam("MATRIX_A");
		if (pMatrixA)
		{
			MatrixAType = pMatrixA->type;
			if (GetRowColFormDimension(pMatrixA->array.dimension, MatrixA_Row, MatrixA_Col))
			{
				bMatA = true;
			}
		}

		const VariableStruct* pMatrixB = pParam->GetParam("MATRIX_B");
		if (pMatrixB)
		{
			MatrixBType = pMatrixB->type;
			if (GetRowColFormDimension(pMatrixB->array.dimension, MatrixB_Row, MatrixB_Col))
			{
				bMatB = true;
			}
		}

		const VariableStruct* pMatrixC = pParam->GetOutput("MATRIX_C");
		if (pMatrixC)
		{
			MatrixCType = pMatrixC->type;
			if (GetRowColFormDimension(pMatrixC->array.dimension, MatrixC_Row, MatrixC_Col))
			{
				bMatC = true;
			}
		}

		if ( (MatrixAType == MatrixBType) && (MatrixBType == MatrixCType))
		{
			switch (MatrixAType)
			{
			case DEF_DOUBLE:
			{
				MatrixXd mat_a(MatrixA_Row, MatrixA_Col);
				SetMatrixXd_From_Array(mat_a, (double*)pMatrixA->pValue, MatrixA_Row, MatrixA_Col);

				MatrixXd mat_b(MatrixB_Row, MatrixB_Col);
				SetMatrixXd_From_Array(mat_b, (double*)pMatrixB->pValue, MatrixB_Row, MatrixB_Col);
				
				MatrixXd mat_c(MatrixC_Row, MatrixC_Col);
				mat_c = mat_a + mat_b;

				if (MatrixC_Col == mat_c.cols() && MatrixC_Row == mat_c.rows())
					SetArray_From_MatrixXd(mat_c, (double*)pMatrixC->pValue, MatrixC_Row, MatrixC_Col);
				else
					SetReShapeArray_From_MatrixXd(mat_c, variable);

				break;
			}
			case DEF_FLOAT:
			{
				MatrixXf mat_a(MatrixA_Row, MatrixA_Col);
				SetMatrixXf_From_Array(mat_a, (float*)pMatrixA->pValue, MatrixA_Row, MatrixA_Col);

				MatrixXf mat_b(MatrixB_Row, MatrixB_Col);
				SetMatrixXf_From_Array(mat_b, (float*)pMatrixB->pValue, MatrixB_Row, MatrixB_Col);

				MatrixXf mat_c(MatrixC_Row, MatrixC_Col);
				mat_c = mat_a + mat_b;

				if (MatrixC_Col == mat_c.cols() && MatrixC_Row == mat_c.rows())
					SetArray_From_MatrixXf(mat_c, (float*)pMatrixC->pValue, MatrixC_Row, MatrixC_Col);
				else
					SetReShapeArray_From_MatrixXf(mat_c, variable);

				break;
			}
			case DEF_INT:
			{
				MatrixXi mat_a(MatrixA_Row, MatrixA_Col);
				SetMatrixXi_From_Array(mat_a, (int*)pMatrixA->pValue, MatrixA_Row, MatrixA_Col);

				MatrixXi mat_b(MatrixB_Row, MatrixB_Col);
				SetMatrixXi_From_Array(mat_b, (int*)pMatrixB->pValue, MatrixB_Row, MatrixB_Col);

				MatrixXi mat_c(MatrixC_Row, MatrixC_Col);
				mat_c = mat_a + mat_b;

				if (MatrixC_Col == mat_c.cols() && MatrixC_Row == mat_c.rows())
					SetArray_From_MatrixXi(mat_c, (int*)pMatrixC->pValue, MatrixC_Row, MatrixC_Col);
				else
					SetReShapeArray_From_MatrixXi(mat_c, variable);

				break;
			}
			default:
			{
				std::string msg = string_format("error : Eigen_Addition data type are different", CStringToString(id));
				PrintMessage(msg);
				break;
			}
			}
		}
	}
}

