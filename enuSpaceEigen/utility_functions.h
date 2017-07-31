#pragma once


#include <string>
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include <vector>

#include "enuSpaceEigen.h"
#include <Eigen/Eigen>
using namespace Eigen;

std::string string_format(const std::string fmt_str, ...);
CString StringToCString(std::string str);
std::string CStringToString(CString reqStr);

int GetArrayIndexFromDimension(CString strOrgDim, CString strDimension);

// [10][10] �Է½� iRow = 10; iCol = 10�� ���� ��ȯ�ϴ� �Լ�
bool GetRowColFormDimension(const wchar_t* dims, int& iRow, int& iCol);

// MAXTRIX�� Ÿ�Կ� ���� ���� Array�� ������ �����ϰ� ���޹޴� �Լ�.
void SetMatrixXd_From_Array(MatrixXd &mat, double* pValue, int Row, int Col);
void SetArray_From_MatrixXd(MatrixXd &mat, double* pValue, int Row, int Col);
void SetMatrixXf_From_Array(MatrixXf &mat, float* pValue, int Row, int Col);
void SetArray_From_MatrixXf(MatrixXf &mat, float* pValue, int Row, int Col);
void SetMatrixXi_From_Array(MatrixXi &mat, int* pValue, int Row, int Col);
void SetArray_From_MatrixXi(MatrixXi &mat, int* pValue, int Row, int Col);

void SetReShapeArray_From_MatrixXd(MatrixXd &mat, std::string variable);
void SetReShapeArray_From_MatrixXf(MatrixXf &mat, std::string variable);
void SetReShapeArray_From_MatrixXi(MatrixXi &mat, std::string variable);