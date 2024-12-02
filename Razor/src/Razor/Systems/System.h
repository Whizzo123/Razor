#pragma once
#include <set>
#include "../Core.h"
#include "../EntityManager.h"
#include "../ComponentManager.h"
#include <glm/glm.hpp>
#include "../Utils/RazorMacros.h"

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
	
	using RenderPipelineConfig = std::vector<RenderStageConfig>;

	class IProperty
	{
	public:
		virtual ~IProperty() = default;
		virtual const char* GetType() = 0;
	};

	template<typename T>
	class Property : public IProperty
	{
	public:
		Property() {}
		~Property() {}
		Property(std::string Name, T Value)
		{
			this->Name = Name;
			this->Value = Value;
		}
		const char* GetType() override
		{
			return typeid(T).name();
		}
	public:
		std::string Name;
		T Value;
	};

	struct PropertySlot
	{
		template<typename T>
		void AddProperty(std::string Name, T Value)
		{
			Properties.emplace_back(new Property<T>(Name, Value));
		}
		
		std::vector<Scope<IProperty>>& GetProperties()
		{
			return Properties;
		}
		
	private:
		std::vector<Scope<IProperty>> Properties;
	};
	struct EntityRenderProperty
	{
		void GeneratePropertySlots(size_t NumberOfSlots)
		{
			Slots.reserve(NumberOfSlots);
			for (int i = 0; i < NumberOfSlots; i++)
			{
				Slots.push_back(PropertySlot());
			}
		}
		int32_t GetNumberOfSlots()
		{
			return Size_tToInt(Slots.size());
		}
		void Clear()
		{
			Slots.clear();
		}
	private:
		std::vector<PropertySlot> Slots;


	public:
		PropertySlot& GetPropertySlot(int32_t SlotIndex)
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
	

	class   System
	{
	public:
		std::set<Entity> Entities;
		virtual void Init() {}
		virtual void Run(float dt) {}
	};

	class RenderSystem : public System
	{
	public:
		virtual void Render(RenderPipelineEntityProperties& Properties) {}
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

	class   SystemManager
	{
	public:

		SystemManager() {}

		template<typename T>
		std::shared_ptr<T> RegisterSystem(T SystemInst, ComponentType* SignatureComponents, std::uint8_t Size)
		{
			const char* typeName = typeid(T).name();
			assert(Systems.find(typeName) == Systems.end() && "Registering system twice");
			auto SystemInstPtr = std::make_shared<T>(SystemInst);
			Systems.insert({ typeName, SystemInstPtr });
			if (std::shared_ptr<RenderSystem> RndrSystem = std::dynamic_pointer_cast<RenderSystem>(SystemInstPtr))
			{
				RenderPipeline.PipelineSystems[RndrSystem->SystemRenderStage].insert({ typeName, RndrSystem });
			}
			// Setup Signature
			Signature SystemSignature;
			for (int i = 0; i < Size; i++)
			{
				SystemSignature.set(SignatureComponents[i]);
			}
			SetSignature<T>(SystemSignature);
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
		void RunRenderSystems(RenderPipelineConfig& PipelineConfig);
		void InitSystems();
	private:
		std::unordered_map<const char*, Signature> Signatures{};
		std::unordered_map<const char*, std::shared_ptr<System>> Systems{};
		RenderSystemPipeline RenderPipeline;
		
	};

}