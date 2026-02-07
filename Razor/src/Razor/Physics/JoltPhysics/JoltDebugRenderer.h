#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Renderer/DebugRenderer.h>
#include "../../Utils/Vector.h"
#include "../IPhysicsDebugRenderer.h"
#include <mutex>
#include "../../Renderer/Debug/DebugLine.h"
#include "../../Renderer/Debug/DebugTriangle.h"

namespace Razor
{

	class PhysicsDebugDrawBuffer
	{
	public:
		std::mutex mutex;
		std::vector<DebugLine> lines;
		std::vector<DebugTriangle> triangles;
	};

	class JoltDebugRenderer : public JPH::DebugRenderer, public IPhysicsDebugRenderer
	{
	public:
		JoltDebugRenderer(PhysicsDebugDrawBuffer* b) : buffer(b) {}

		void DrawLine(JPH::RVec3Arg from, JPH::RVec3Arg to, JPH::ColorArg color) override;

		void DrawTriangle(JPH::RVec3Arg v1, JPH::RVec3Arg v2, JPH::RVec3Arg v3, JPH::ColorArg color, ECastShadow inCastShadow = ECastShadow::Off) override;

		Batch CreateTriangleBatch(const Triangle* inTriangles, int inTriangleCount) override;
		Batch CreateTriangleBatch(const Vertex* inVertices, int inVertexCount, const JPH::uint32* inIndices, int inIndexCount) override;

		void DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode = ECullMode::CullBackFace, ECastShadow inCastShadow = ECastShadow::On, EDrawMode inDrawMode = EDrawMode::Solid) override;

		void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor = JPH::Color::sWhite, float inHeight = 0.5f) override;

	private:
		PhysicsDebugDrawBuffer* buffer;
	};
}