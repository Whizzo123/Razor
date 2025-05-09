#pragma once
#include <set>
#include "../Core.h"
#include <glm/glm.hpp>
#include "../Utils/RazorMacros.h"
#include "../Scene/Scene.h"

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
		std::unordered_map<entt::entity, EntityRenderProperty> Properties;
	};
	

	class System
	{
	public:
		System(Ref<Scene> Scene) : CurrentScene(Scene) {}
		virtual void Init() {}
		virtual void Run(float dt) {}

	protected:
		Ref<Scene> CurrentScene;
	};

	class RenderSystem : public System
	{
	public:
		RenderSystem(Ref<Scene> Scene) : System(Scene) {}
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
			//// Setup Signature
			//Signature SystemSignature;
			//for (const ComponentType& Type : SystemInstPtr->Signature)
			//{
			//	SystemSignature.set(Type);
			//}
			//SetSignature<T>(SystemSignature);
			return SystemInstPtr;
		}

		void RunSystems(float dt);
		void RunRenderSystems(RenderPipelineConfig& PipelineConfig);
		void InitSystems();
	private:
		std::unordered_map<const char*, std::shared_ptr<System>> Systems{};
		RenderSystemPipeline RenderPipeline;
		
	};

}