#pragma once
#include <set>
#include "../Core.h"
#include "../EntityManager.h"
#include "../ComponentManager.h"
#include <glm/glm.hpp>

namespace Razor
{
	#ifndef ENUM_RENDERSTAGE
	#define ENUM_RENDERSTAGE
	enum class RenderStage { RENDER_STAGE_MATERIAL_PASS, RENDER_STAGE_LIGHTING_PASS, RENDER_STAGE_TRANSFORMATION_PASS, RENDER_STAGE_CAMERA_PASS, RENDER_STAGE_RENDER };
	#endif
	struct RenderStageConfig
	{
		RenderStage Stage;
		std::vector<const char*> StageSystemsToRun;
	};
	struct RenderPipelineConfig
	{
		std::vector<RenderStageConfig> Configuration;
	};
	struct FProperty
	{
		std::string Name;
		float Value;
	};
	struct VProperty
	{
		std::string Name;
		glm::vec3 Value;
	};
	struct MProperty
	{
		std::string Name;
		glm::mat4 Value;
	};
	struct BProperty
	{
		std::string Name;
		bool Value;
	};
	struct IProperty
	{
		std::string Name;
		int Value;
	};
	struct PropertySlot
	{
		void AddFloatProperty(std::string Name, float Value)
		{
			FProperties.push_back({ Name, Value });
		}

		void AddIntProperty(std::string Name, int Value)
		{
			IProperties.push_back({ Name, Value });
		}

		void AddBoolProperty(std::string Name, bool Value)
		{
			BProperties.push_back({ Name, Value });
		}

		void AddVec3Property(std::string Name, glm::vec3 Value)
		{
			VProperties.push_back({ Name, Value });
		}

		void AddMat4Property(std::string Name, glm::mat4 Value)
		{
			MProperties.push_back({ Name, Value });
		}
		std::vector<FProperty>& GetFloatProperties()
		{
			return FProperties;
		}
		std::vector<VProperty>& GetVec3Properties()
		{
			return VProperties;
		}
		std::vector<MProperty>& GetMat4Properties()
		{
			return MProperties;
		}
		std::vector<BProperty>& GetBoolProperties()
		{
			return BProperties;
		}
		std::vector<IProperty>& GetIntProperties()
		{
			return IProperties;
		}
	private:
		std::vector<FProperty> FProperties;
		std::vector<VProperty> VProperties;
		std::vector<MProperty> MProperties;
		std::vector<BProperty> BProperties;
		std::vector<IProperty> IProperties;
	};
	struct EntityRenderProperty
	{
		void GeneratePropertySlots(uint8_t NumberOfSlots)
		{
			Slots.reserve(NumberOfSlots);
			for (int i = 0; i < NumberOfSlots; i++)
			{
				Slots.push_back(PropertySlot());
			}
		}
		int GetNumberOfSlots()
		{
			return Slots.size();
		}
		void Clear()
		{
			Slots.clear();
		}
	private:
		std::vector<PropertySlot> Slots;


	public:
		PropertySlot& GetPropertySlot(uint8_t SlotIndex)
		{
			if (SlotIndex >= 0 && SlotIndex < Slots.size())
			{
				return Slots[SlotIndex];
			}
			RZ_CORE_ERROR("Attempting to get property slot with an invalid index returning slot 0");
			return Slots[0];
		}
	};
	struct RenderPipelineEntityProperties
	{
		std::array<EntityRenderProperty, MAX_ENTITIES> Properties;
		uint8_t HighestEntity = 0;
	};
	

	class RAZOR_API System
	{
	public:
		std::set<Entity> Entities;
		virtual void Init() {};
		virtual void Run(float dt) {};
	};

	class RenderSystem : public System
	{
	public:
		virtual void Render(RenderPipelineEntityProperties& Properties) {};
		static RenderStage SystemRenderStage;
	};

	struct RenderSystemPipeline
	{
		std::unordered_map <RenderStage, std::unordered_map<const char*, std::shared_ptr<RenderSystem>>> PipelineSystems{};
		RenderPipelineEntityProperties EntityRenderProperties;
		void RunSystemsFor(RenderStage Stage)
		{
			for (std::pair<const char*, std::shared_ptr<RenderSystem>> System : PipelineSystems[Stage])
			{
				System.second->Render(EntityRenderProperties);
			}
		}
	};

	class RAZOR_API SystemManager
	{
	public:

		SystemManager() {};

		template<typename T>
		std::shared_ptr<T> RegisterSystem(T SystemInst)
		{
			const char* typeName = typeid(T).name();
			assert(Systems.find(typeName) == Systems.end() && "Registering system twice");
			auto SystemInstPtr = std::make_shared<T>(SystemInst);
			Systems.insert({ typeName, SystemInstPtr });
			if (std::shared_ptr<RenderSystem> RndrSystem = std::dynamic_pointer_cast<RenderSystem>(SystemInstPtr))
			{
				RenderPipeline.PipelineSystems[RndrSystem->SystemRenderStage].insert({ typeName, RndrSystem });
			}
			return SystemInstPtr;
		}

		template<typename T>
		void SetSignature(Signature InSignature)
		{
			const char* typeName = typeid(T).name();
			std::string LogMsg = "System does not exist" + std::string(typeName);
			if (!Systems.count(typeName))
			{
				RZ_CORE_ERROR(LogMsg);
			} 
			Signatures.insert({ typeName, InSignature });
		}

		void EntityDestroyed(Entity InEntity);
		void EntitySignatureChanged(Entity InEntity, Signature EntitySignature);
		void RunSystems(float dt);
		void RunRenderSystems(RenderPipelineConfig PipelineConfig);
		void InitSystems();
	private:
		std::unordered_map<const char*, Signature> Signatures{};
		std::unordered_map<const char*, std::shared_ptr<System>> Systems{};
		RenderSystemPipeline RenderPipeline;
		
	};

}