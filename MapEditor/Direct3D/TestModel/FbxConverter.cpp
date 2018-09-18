#include "../stdafx.h"
#include "FbxConverter.h"
#include "ModelScene.h"

wstring FbxConverter::destModelsPath = L"C:/Users/Me/Documents/Visual Studio 2015/Projects/_Contents/Models/BinaryModels";
FbxConverter::FbxConverter()
{

}

FbxConverter::~FbxConverter()
{
}

void FbxConverter::FbxToBin(wstring path, wstring destPath)
{
	this->path = path;
	this->destPath = destPath;

	if (FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(path.c_str()))
	{
		FindAllFiles(&files, path.c_str());
	}

	for (size_t i = 0; i < files.size(); i++)
	{
		Convert(files[i], destPath);
	}
}

void FbxConverter::Convert(wstring path, wstring destPath)
{
	ModelScene temp = ModelScene();
	temp.LoadFbx(path);
	temp.SaveBinary(destPath, temp.GetFileName());
}

void FbxConverter::FindAllFiles(vector<wstring>* fileNames, wstring FilePath)
{
	WIN32_FIND_DATA FindFileData;
	wstring fullPath = FilePath + L"*";
	HANDLE hFind = FindFirstFile(fullPath.c_str(), &FindFileData);
	assert(hFind != NULL);

	wstring pPath;
	wstring strTmp;

	if (hFind == INVALID_HANDLE_VALUE)   // ���� ó��
	{
		FindClose(hFind);
		return;
	}

	do {
		pPath = FilePath;
		pPath += FindFileData.cFileName;

		if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			if (wcscmp(FindFileData.cFileName, L".") == 0)
				continue;
			if (wcscmp(FindFileData.cFileName, L"..") == 0)
				continue;

			pPath += L"/";
			// ����Լ� ȣ��(�ش� ���� ������ ���� �˻�)
			FindAllFiles(fileNames, pPath);
		}
		else {
			wstring::size_type stTmp = pPath.find(L"fbx");
			if (stTmp != 0)
			{
				fileNames->push_back(pPath);
			}
		}

	} while (FindNextFile(hFind, &FindFileData));
}
