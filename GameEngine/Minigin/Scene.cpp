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
	m_pObjectsToDelete.clear();
	for (auto& object : m_Objects)
	{
		object->Update(deltaTime);
		if (object->GetNeedsKilling() == true)
		{
			m_pObjectsToDelete.push_back(object);
		}
	}
	if (m_pObjectsToDelete.size() >=1)
	{
		for (auto& object : m_pObjectsToDelete)
		{
			//remove object from vector
			m_Objects.erase(std::find(m_Objects.begin(),m_Objects.end(),object));
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

