#include "stdafx.h"
#include "String.h"

//////////////////////////////////////////////////////////////////////////
///@brief 문자열 자르기
///@param orgin : 원본 문자열
///@param tok : 자를 기준이 되는 문자열
///@return 완료된 문자열 배열
//////////////////////////////////////////////////////////////////////////
void String::SplitString(vector<string>* result, string origin, string tok)
{
	result->clear();

	int cutAt = 0; //자를 위치s
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //자르는 위치가 0보다크면
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //자르고도 남은 것이 있다면
		result->push_back(origin.substr(0, cutAt));
}

void String::SplitString(vector<wstring>* result, wstring origin, wstring tok)
{
	result->clear();

	int cutAt = 0; //자를 위치s
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //자르는 위치가 0보다크면
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origin.length() > 0) //자르고도 남은 것이 있다면
		result->push_back(origin.substr(0, cutAt));
}

//////////////////////////////////////////////////////////////////////////
///@brief 파일의 전체 경로에서 경로와 파일명으로 분리
///@param fullPath : 전체 경로
///@param filePath : 경로
///@param fileName : 파일명
//////////////////////////////////////////////////////////////////////////
void String::SplitFilePath(string fullPath, string * filePath, string * fileName)
{
	size_t index = fullPath.find_last_of('/');

	if (filePath != NULL)
		*filePath = fullPath.substr(0, index + 1);

	if (fileName != NULL)
		*fileName = fullPath.substr(index + 1, fullPath.length());
}

void String::SplitFilePath(wstring fullPath, wstring* filePath, wstring* fileName)
{
	size_t index = fullPath.find_last_of('/');

	if (filePath != NULL)
		*filePath = fullPath.substr(0, index + 1);

	if (fileName != NULL)
		*fileName = fullPath.substr(index + 1, fullPath.length());
}

//////////////////////////////////////////////////////////////////////////
///@brief 파일의 전체 경로에서 경로와 파일명으로 분리
///@param fullPath : 전체 경로
///@param fileName : 파일명
//////////////////////////////////////////////////////////////////////////
void String::SplitFileNameOnly(string fullPath, string * fileName)
{
	SplitFilePath(fullPath, NULL, fileName);

	size_t index = fileName->find_last_of('.');

	if (fileName != NULL)
		*fileName = fileName->substr(0, index);
}

void String::SplitFileNameOnly(wstring fullPath, wstring * fileName)
{
	SplitFilePath(fullPath, NULL, fileName);

	size_t index = fileName->find_last_of('.');

	if (fileName != NULL)
		*fileName = fileName->substr(0, index);
}


//////////////////////////////////////////////////////////////////////////
///@brief 시작 문자열이 같은지 체크
///@param str : 체크하려는 문자열
///@param comp : 시작 비교문자열
//////////////////////////////////////////////////////////////////////////
bool String::StartsWith(string str, string comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && (int)index == 0)
		return true;

	return false;
}

bool String::StartsWith(wstring str, wstring comp)
{
	wstring::size_type index = str.find(comp);
	if (index != wstring::npos && (int)index == 0)
		return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////
///@brief 해당 문자열이 포함되어 있는지
///@param str : 체크하려는 문자열
///@param comp : 비교문자열
//////////////////////////////////////////////////////////////////////////
bool String::Contain(string str, string comp)
{
	size_t found = str.find(comp);

	return found != wstring::npos;
}

bool String::Contain(wstring str, wstring comp)
{
	size_t found = str.find(comp);

	return found != wstring::npos;
}


//////////////////////////////////////////////////////////////////////////
///@brief 해당 문자열에서 comp 문자를 rep로 변경
///@param str : 체크하려는 문자열
///@param comp : 비교문자열
///@param rep : 바꿀문자열
//////////////////////////////////////////////////////////////////////////
void String::Replace(string * str, string comp, string rep)
{
	string temp = *str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	*str = temp;
}

void String::Replace(wstring* str, wstring comp, wstring rep)
{
	wstring temp = *str;

	size_t start_pos = 0;
	while ((start_pos = temp.find(comp, start_pos)) != wstring::npos)
	{
		temp.replace(start_pos, comp.length(), rep);
		start_pos += rep.length();
	}

	*str = temp;
}

//////////////////////////////////////////////////////////////////////////
///@brief string형을 wstring형으로 변경
///@param value : 변환할 문자열
///@return 변환 완료 문자열
//////////////////////////////////////////////////////////////////////////
wstring String::StringToWString(string value)
{
	wstring temp = L"";
	temp.assign(value.begin(), value.end());

	return temp;
}

//////////////////////////////////////////////////////////////////////////
///@brief wstring형을 string형으로 변경
///@param value : 변환할 문자열
///@return 변환 완료 문자열
//////////////////////////////////////////////////////////////////////////
string String::WStringToString(wstring value)
{
	string temp = "";
	temp.assign(value.begin(), value.end());

	return temp;
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
///@brief wstring형을 string형으로 변경
///@param value : 변환할 문자열
///@return 변환 완료 문자열
//////////////////////////////////////////////////////////////////////////
const char * String::ToCSharp(string value)
{
	UINT size = value.size() + 1;
	char* str = (char *)LocalAlloc(LPTR, value.size() + 1);
	strcpy_s(str, size, value.c_str());

	return (const char *)str;
}

//////////////////////////////////////////////////////////////////////////
///@brief wstring형의 문자열의 . 다음 3자리를 자른다 ( 확장자 제거에 쓰임)
///@param value : 변환할 문자열 ( ./ 으로 시작하여야된다)
///@return 변환 완료 문자열
//////////////////////////////////////////////////////////////////////////
void String::DeleteExt(wstring * result, wstring origin)
{
	// .인 지점을 찾아내어 확장자도 잘라낸다
	wstring::size_type stTmp = origin.find(L".", 1);
	*result = origin;
	(*result).erase(stTmp, 4);
}

//////////////////////////////////////////////////////////////////////////
///@brief wstring형의 문자열의 . 다음 3자리를 자른다 ( 확장자 제거에 쓰임)
///@param value : 변환할 문자열 ( ./ 으로 시작하여야된다)
///@return 변환 완료 문자열
//////////////////////////////////////////////////////////////////////////
void String::DeleteExt(string * result, string origin)
{
	// .인 지점을 찾아내어 확장자도 잘라낸다
	string::size_type stTmp = origin.find(".", 1);
	*result = origin;
	(*result).erase(stTmp, 4);
}