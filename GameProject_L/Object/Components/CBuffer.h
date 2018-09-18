#pragma once

template <typename T>
class CBuffer : public ShaderBuffer
{
public:
	CBuffer()
		: dataNum(0)
	{
		paddingNum = 16 / (UINT)sizeof(T);
		AddPadding(paddingNum);
	}

	~CBuffer()
	{
		datas.clear();
	}

	void ResetBuffer()
	{
		SAFE_RELEASE(buffer);

		data = &(datas[0]);
		dataSize = sizeof(T) * datas.size();

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = dataSize;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
		assert(SUCCEEDED(hr));
	}

	void AddData(T data)
	{
		//padding 지우기
		DeletePadding(paddingNum);

		//값넣기
		dataNum++;
		datas.push_back(data);

		//paddingNum 갱신
		paddingNum = (16 - (sizeof(T) * dataNum) % 16)/ (UINT)sizeof(T);

		//paddingData 넣기
		AddPadding(paddingNum);

		ResetBuffer();
	}

	void SetData(UINT index, T data)
	{
		if (index >= datas.size())
			return;

		datas[index] = data;
	}

private:
	void AddPadding(UINT num)
	{
		for (UINT i = 0; i < num; i++)
		{
			T temp;
			datas.push_back(temp);
		}
		
	}
	void DeletePadding(UINT num)
	{
		for (UINT i = 0; i < num; i++)
			datas.pop_back();
	}

	vector<T> datas;
	UINT dataNum, paddingNum;
};
