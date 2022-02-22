#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>&object)
{
	m_Objects.push_back(object);
}

void Scene::Update(double deltaTime)
{
	std::vector<std::shared_ptr<GameObject>> objectsToDelete; //add a way to delete objects, needs to delete everything

	for (auto& object : m_Objects)
	{
		object->Update(deltaTime);
		if (object->GetNeedsKilling() == true)
		{
			objectsToDelete.push_back(object);
		}
	}
	if (objectsToDelete.size() >=1)
	{
		for (auto& object : objectsToDelete)
		{
			delete(&object);
		}
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

