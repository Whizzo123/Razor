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

	void JoltDebugRenderer::DrawTriangle(JPH::RVec3Arg v1, JPH::RVec3Arg v2, JPH::RVec3Arg v3, JPH::ColorArg color, ECastShadow inCastShadow)
	{
		std::lock_guard<std::mutex> lock(buffer->mutex);
		DebugTriangle triangle;
		triangle.v1 = Vector3((float)v1.GetX(), (float)v1.GetY(), (float)v1.GetZ());
		triangle.v2 = Vector3((float)v2.GetX(), (float)v2.GetY(), (float)v2.GetZ());
		triangle.v3 = Vector3((float)v3.GetX(), (float)v3.GetY(), (float)v3.GetZ());
		triangle.color = Vector3(color.r, color.g, color.b);
		buffer->triangles.push_back(triangle);
	}

	JPH::DebugRenderer::Batch JoltDebugRenderer::CreateTriangleBatch(const Triangle* inTriangles, int inTriangleCount)
	{
		return Batch();
	}
	JPH::DebugRenderer::Batch JoltDebugRenderer::CreateTriangleBatch(const Vertex* inVertices, int inVertexCount, const JPH::uint32* inIndices, int inIndexCount)
	{
		return Batch();
	}
	void JoltDebugRenderer::DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode, ECastShadow inCastShadow, EDrawMode inDrawMode)
	{
	}
	void JoltDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor, float inHeight)
	{
	}
}