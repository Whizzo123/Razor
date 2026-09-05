#include "JoltDebugRenderer.h"
#include "../../Renderer/Debug/DebugLine.h"
#include "../../Renderer/Debug/DebugTriangle.h"


namespace Razor
{
	void JoltDebugRenderer::DrawLine(JPH::RVec3Arg from, JPH::RVec3Arg to, JPH::ColorArg color)
	{
		std::lock_guard<std::mutex> lock(buffer->mutex);
		DebugLine line;
		line.a = Vector3((float)from.GetX(), (float)from.GetY(), (float)from.GetZ());
		line.b = Vector3((float)to.GetX(), (float)to.GetY(), (float)to.GetZ());
		line.color = Vector3(color.r, color.g, color.b);
		buffer->lines.push_back(line);
	}

	void JoltDebugRenderer::DrawTriangle(JPH::RVec3Arg v1, JPH::RVec3Arg v2, JPH::RVec3Arg v3, JPH::ColorArg color, [[maybe_unused]]ECastShadow inCastShadow)
	{
		std::lock_guard<std::mutex> lock(buffer->mutex);
		DebugTriangle triangle;
		triangle.v1 = Vector3((float)v1.GetX(), (float)v1.GetY(), (float)v1.GetZ());
		triangle.v2 = Vector3((float)v2.GetX(), (float)v2.GetY(), (float)v2.GetZ());
		triangle.v3 = Vector3((float)v3.GetX(), (float)v3.GetY(), (float)v3.GetZ());
		triangle.color = Vector3(color.r, color.g, color.b);
		buffer->triangles.push_back(triangle);
	}

	JPH::DebugRenderer::Batch JoltDebugRenderer::CreateTriangleBatch([[maybe_unused]]const Triangle* inTriangles, [[maybe_unused]]int inTriangleCount)
	{
		return Batch();
	}
	JPH::DebugRenderer::Batch JoltDebugRenderer::CreateTriangleBatch([[maybe_unused]]const Vertex* inVertices, [[maybe_unused]]int inVertexCount, [[maybe_unused]]const JPH::uint32* inIndices, [[maybe_unused]]int inIndexCount)
	{
		return Batch();
	}
	void JoltDebugRenderer::DrawGeometry([[maybe_unused]]JPH::RMat44Arg inModelMatrix, [[maybe_unused]]const JPH::AABox& inWorldSpaceBounds, [[maybe_unused]]float inLODScaleSq, [[maybe_unused]]JPH::ColorArg inModelColor, [[maybe_unused]]const GeometryRef& inGeometry,
		[[maybe_unused]]ECullMode inCullMode, [[maybe_unused]]ECastShadow inCastShadow, [[maybe_unused]]EDrawMode inDrawMode)
	{
	}
	void JoltDebugRenderer::DrawText3D([[maybe_unused]]JPH::RVec3Arg inPosition, [[maybe_unused]]const std::string_view& inString, [[maybe_unused]]JPH::ColorArg inColor, [[maybe_unused]]float inHeight)
	{
	}
}