#pragma once


#include <string>
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include <vector>

#include <Eigen/Eigen>
using namespace Eigen;

std::string string_format(const std::string fmt_str, ...);
CString StringToCString(std::string str);
std::string CStringToString(CString reqStr);

int GetArrayIndexFromDimension(CString strOrgDim, CString strDimension);

// [10][10] 입력시 iRow = 10; iCol = 10의 값을 반환하는 함수
bool GetRowColFormDimension(const wchar_t* dims, int& iRow, int& iCol);

// MAXTRIX의 타입에 따라서 값을 Array의 정보에 전달하고 전달받는 함수.
void SetMatrixXd_From_Array(MatrixXd &mat, double* pValue, int Col, int Row);
void SetArray_From_MatrixXd(MatrixXd &mat, double* pValue, int Col, int Row);
void SetMatrixXf_From_Array(MatrixXf &mat, float* pValue, int Col, int Row);
void SetArray_From_MatrixXf(MatrixXf &mat, float* pValue, int Col, int Row);
void SetMatrixXi_From_Array(MatrixXi &mat, int* pValue, int Col, int Row);
void SetArray_From_MatrixXi(MatrixXi &mat, int* pValue, int Col, int Row);