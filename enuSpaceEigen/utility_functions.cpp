#include "stdafx.h"
#include "utility_functions.h"
#include "GlobalHeader.h"


std::string string_format(const std::string fmt_str, ...)
{
	int final_n, n = ((int)fmt_str.size()) * 2; 
	std::string str;
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1) 
	{
		formatted.reset(new char[n]);
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}

	return std::string(formatted.get());
}

CString StringToCString(std::string str)
{
	CString result;
	result = CString::CStringT(CA2CT(str.c_str()));
	return result;
}

std::string CStringToString(CString reqStr)
{
	std::string result;
	result = std::string(CT2CA(reqStr.operator LPCWSTR()));
	return result;
}

int GetArrayIndexFromDimension(CString strOrgDim, CString strDimension)
{
	CString Token;
	CString Seperator = _T("[]");
	int Position = 0;
	CString strBuffer = strOrgDim;

	bool berror = false;
	int idimcount = 0;
	int idim[20];

	Token = strBuffer.Tokenize(Seperator, Position);
	while (Token != L"")
	{
		Token.Trim();
		int iValue = _wtoi(Token);
		if (iValue > 0)
		{
			idim[idimcount] = iValue;
			idimcount++;
		}
		else
		{
			berror = true;
			break;
		}
		Token = strBuffer.Tokenize(Seperator, Position);
	}

	if (berror)
		return 0;

	/////////////////////////////////////////////////////////
	int iIndex = 0;
	strBuffer = strDimension;
	Position = 0;

	int isdimcount = 0;
	int isdim[20];

	Token = strBuffer.Tokenize(Seperator, Position);
	while (Token != L"")
	{
		Token.Trim();
		int iValue = _wtoi(Token);
		if (iValue >= 0)
		{
			isdim[isdimcount] = iValue;
			isdimcount++;
		}
		else
		{
			berror = true;
			break;
		}
		Token = strBuffer.Tokenize(Seperator, Position);
	}

	if (berror || isdimcount != idimcount)
		return 0;

	for (int i = 0; i < idimcount; i++)
	{
		int imux = 1;
		for (int j = i + 1; j < idimcount; j++)
		{
			imux = imux * idim[j];
		}
		iIndex = iIndex + isdim[i] * imux;
	}

	return iIndex;
}

bool GetRowColFormDimension(const wchar_t* dims, int& iRow, int& iCol)
{
	wchar_t vals[10];
	int iPos = 0;

	int iCount = 0;

	int ilen = wcslen(dims);
	for (int i = 0; i < ilen; i++)
	{
		if (dims[i] == L'[')
		{
			iPos = 0;
		}
		else if (dims[i] == L']')
		{
			if (iCount == 0)
				iRow = _wtoi(vals);
			else if (iCount == 1)
				iCol = _wtoi(vals);
			else
				return false;

			iCount++;
			iPos = 0;
		}
		else
		{
			vals[iPos] = dims[i];
			iPos++;
		}
	}
}

void SetMatrixXd_From_Array(MatrixXd &mat, double* pValue, int Col, int Row)
{
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			mat(i, j) = *((double*)pValue + (i*Col + j));
		}
	}
}

void SetArray_From_MatrixXd(MatrixXd &mat, double* pValue, int Col, int Row)
{
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			*((double*)pValue + (i*Col + j)) = mat(i, j);
		}
	}
}

void SetMatrixXf_From_Array(MatrixXf &mat, float* pValue, int Col, int Row)
{
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			mat(i, j) = *((float*)pValue + (i*Col + j));
		}
	}
}

void SetArray_From_MatrixXf(MatrixXf &mat, float* pValue, int Col, int Row)
{
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			*((float*)pValue + (i*Col + j)) = mat(i, j);
		}
	}
}

void SetMatrixXi_From_Array(MatrixXi &mat, int* pValue, int Col, int Row)
{
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			mat(i, j) = *((int*)pValue + (i*Col + j));
		}
	}
}

void SetArray_From_MatrixXi(MatrixXi &mat, int* pValue, int Col, int Row)
{
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			*((int*)pValue + (i*Col + j)) = mat(i, j);
		}
	}
}