#pragma once

#include <bitset>
#include <unordered_map>
#include "Core.h"
#include <memory>
#include "IO/RazorIO.h"
#include "Renderer/Model.h"
#include "Assets/AssetDirectory.h"
#include "Utils/Quaternion.h"
#include "Utils/Vector3.h"
#include "Utils/Matrix4.h"
#include "Utils/RazorMaths.h"

namespace Razor
{
	struct Transform
	{
		Vector3 Position;
		Vector3 Scale;
		Vector3 Rotation;

		Transform() : Position{0.0f, 0.0f, 0.0f}, Scale{1.0f, 1.0f, 1.0f}, Rotation{0.0f, 0.0f, 0.0f}, RotationQ(0.0f, 0.0f, 0.0f, 1.0f) {}
		Transform(Vector3 Pos, Vector3 Scl, Vector3 Rot) : Position(Pos), Scale(Scl), Rotation(Rot), RotationQ(0.0f, 0.0f, 0.0f, 1.0f) {}

		void Rotate(Vector3 EulerAngles)
		{
			Rotation = EulerAngles;
			RotationQ = Quaternion(RazorMaths::Radians(EulerAngles));

		}
		Matrix4 Get()
		{
			Matrix4 model = Matrix4(1.0f);
			model = Matrix4::Translate(model, Position);
			model = model * Matrix4::ToMat4(RotationQ);
			model = Matrix4::Scale(model, Scale);
			return model;
		}

	private:
		Quaternion RotationQ;

		void Move(Vector3 MoveVector)
		{
			Position += MoveVector;
		}

	};

	struct Light
	{
		Vector3 Position;
		Vector3 Diffuse;
		Vector3 Ambient;
		Vector3 Specular;
		Light() {}
		virtual ~Light() {}
	};

	struct DirectionalLight
	{
		Vector3 Diffuse;
		Vector3 Ambient;
		Vector3 Specular;
		Vector3 Direction;

		DirectionalLight() : Diffuse{1.0f, 1.0f, 1.0f}, Ambient{1.0f, 1.0f, 1.0f}, Specular{1.0f, 1.0f, 1.0f}, Direction{1.0f, 0.0f, 0.0f} {}
	};

	struct PointLight : public Light
	{
		float Constant;
		float Linear;
		float Quadratic;
	};

	struct SpotLight : public DirectionalLight
	{
		float Cutoff;
		float OuterCutoff;
	};


	struct Mesh
	{
		Mesh() : mKey(AssetKey("")) {}
		Mesh(AssetKey assetKey) : mKey(assetKey) {}
		AssetKey mKey;
	};

	struct Collider
	{
		float Radius;
	};

	struct RAZOR_API Camera
	{
		// TODO Potentially move this and pass responsiblity onto the developer
		Camera()
		{
			CameraPos = Vector3{0.0f, 0.0f, 3.0f};
			CameraTarget = Vector3{0.0f, 0.0f, 0.0f};
			CameraDirection = Vector3::Normalise(CameraPos - CameraTarget);

			Up = Vector3{0.0f, 1.0f, 0.0f};
			CameraRight = Vector3::Normalise(Vector3::Cross(Up, CameraDirection));
			CameraUp = Vector3::Cross(CameraDirection, CameraRight);
			CameraFront = Vector3{0.0f, 0.0f, -1.0f};
		}
		Vector3 CameraPos;
		Vector3 CameraTarget;
		Vector3 CameraDirection;

		Vector3 Up;
		Vector3 CameraRight;
		Vector3 CameraUp;
		Vector3 CameraFront;
	};

	struct CameraInfo
	{
		Matrix4 CameraView;
		Matrix4 CameraProjection;
		Vector3 CameraPosition;
	};

	struct Input
	{
		// KeyMap [Key] -> State (0, 1)
		RazorKey Keyboard;
		// MouseBut [Key] -> State (0, 1)
		// MousePos -> Pos
	};

	struct ScriptComponent
	{
		std::vector<uint64_t> mScriptInstances;

		ScriptComponent() = default;
	};

	enum class CollisionEventType { Started, Ended };

	struct CollisionEvent
	{
		CollisionEventType Type;
		uint32_t OtherEntityId;
		std::vector<Vector3> mCollisionPoints;
	};

	struct CollisionComponent
	{
		std::vector<CollisionEvent> Events;
		bool bIsTrigger;
	};
}
