#include "precomp.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "DefaultMaterial.h"
#include "Object.h"


Object::Object()
{
	material = new DefaultMaterial();
	meshRenderer.Initialize();
	camera = Camera::GetInstance();
}

void Object::Update()
{
}

void Object::Draw()
{
	if (isActive)
	{
		material->ActivateMaterial(transform.GetModelMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
		meshRenderer.Draw();
	}
}