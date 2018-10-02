#include "stdafx.h"
#include "MeshNodeData.h"

#include "Systems\GameSystems\Frustum.h"

MeshNodeData::MeshNodeData()
	: node(nullptr), meshRenderOption(0)
{
}

MeshNodeData::~MeshNodeData()
{
	ReleaseNode(node);
}

void MeshNodeData::Update(GlobalValues * values)
{
}

void MeshNodeData::Draw(GlobalValues* values, D3D_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	OnOption(values);
	RenderNode(node, values);
	OffOption(values);
}

void MeshNodeData::RenderNode(MeshNode * node, GlobalValues* values)
{
	bool result = values->Frustum->CheckCube(node->positionX, 0.0f, node->positionZ, (node->width) / 2.0f);
	UINT count = 0;

	if (!result)
		return;

	for (UINT i = 0; i < 4; i++)
	{
		if (node->nodes[i] != nullptr)
		{
			count++;
			RenderNode(node->nodes[i], values);
		}
	}

	if (count != 0)
		return;

	node->mesh->Draw(values);
}

void MeshNodeData::ReleaseNode(MeshNode * node)
{
	for (UINT i = 0; i < 4; i++)
	{
		if (node->nodes[i] != 0)
		{
			ReleaseNode(node->nodes[i]);
		}
	}

	SAFE_DELETE(node->mesh);
	
	for (UINT i = 0; i < 4; i++)
		SAFE_DELETE(node->nodes[i]);
}

void MeshNodeData::OnOption(GlobalValues * values)
{
	if (meshRenderOption == 0)
		return;

	unsigned int partOption = meshRenderOption & 15;
	if (partOption == (unsigned int)MeshRenderOption::R_CullReverce)
	{
		values->RenderOption->TurnOnCountCullMode();
	}
	else if (partOption == (unsigned int)MeshRenderOption::R_CullOff)
	{
		values->RenderOption->TurnOnAllCullMode();
	}
	else if (partOption == (unsigned int)MeshRenderOption::R_WireFrame)
	{
		values->RenderOption->TurnOnWireframeMode();
	}

	partOption = meshRenderOption & 240;
	if (partOption == (unsigned int)MeshRenderOption::D_DepthOff)
	{
		values->RenderOption->TurnOffZBuffer();
	}

	partOption = meshRenderOption & 3840;
	if (partOption == (unsigned int)MeshRenderOption::B_BlendOn)
	{
		values->RenderOption->TurnOnAlphaBlending();
	}
	else if (partOption == (unsigned int)MeshRenderOption::B_BlendOn2)
	{
		values->RenderOption->TurnOnOtherBlending();
	}
}

void MeshNodeData::OffOption(GlobalValues * values)
{
	if (meshRenderOption == 0)
		return;

	values->RenderOption->TurnOnCullMode();
	values->RenderOption->TurnOnZBuffer();
	values->RenderOption->TurnOffAlphaBlending();
}
