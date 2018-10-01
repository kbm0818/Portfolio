			Collider* col;
				ModelLoader::ReadCollider(r, &col);
				modelData->embeddedColliders.push_back(make_pair(boneIndex, col));
			}
		}

		modelContainer.insert(make_pair(name, modelData));
	}
	r->Close();

	SAFE_DELETE(r);

}


void AssetManager::AddMesh(string name, Mesh * mesh)
{
	meshContainer[name] = mesh;
}

void AssetManager::AddMaterial(string name, wstring diffuse, wstring specular, wstring ambient, wstring emissive, 
	D3DXCOLOR diffuseColor, D3DXCOLOR specularColor, D3DXCOLOR ambientColor, D3DXCOLOR emissiveColor, int shininess)
{
	Material* mat = new Material();
	mat->SetAmbient(ambientColor);
	mat->SetDiffuse(diffuseColor);
	mat->SetEmissive(emissiveColor);
	mat->SetSpecular(specularColor);
	mat->SetShininess((float)shininess);

	mat->