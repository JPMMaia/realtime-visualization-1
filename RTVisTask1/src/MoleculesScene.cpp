#include "MoleculesScene.h"
#include "MoleculesRenderItem.h"

using namespace RTVis;
using namespace OpenGLEngine;

void MoleculesScene::Initialize(Graphics* graphics, const std::vector<Atom>& atoms)
{
	InitializeGeometry(atoms);
	InitializeRenderItems(graphics);
}

void MoleculesScene::InitializeGeometry(const std::vector<Atom>& atoms)
{
	using namespace VertexTypes;

	// Create atoms mesh:
	{
		GeometryGenerator::MeshData<PositionColorRadiusVertexType> atomsMesh;

		atomsMesh.Vertices.reserve(atoms.size());
		for (const auto& atom : atoms)
		{
			PositionColorRadiusVertexType vertex;
			vertex.Position = QVector3D(atom.position.x, atom.position.y, atom.position.z);
			vertex.Color = QVector3D(atom.color.x, atom.color.y, atom.color.z);
			vertex.Radius = atom.radius;

			atomsMesh.Vertices.push_back(vertex);
		}

		m_meshes.emplace("Atoms Mesh", std::move(atomsMesh));
	}
}

void MoleculesScene::InitializeRenderItems(Graphics* graphics)
{
	// Create molecules render item:
	{
		auto moleculesRenderItem = std::make_unique<MoleculesRenderItem>();
		moleculesRenderItem->Mesh = &m_meshes.at("Atoms Mesh");
		moleculesRenderItem->PrimitiveType = GL_POINTS;

		graphics->AddRenderItem(std::move(moleculesRenderItem));
	}
}
