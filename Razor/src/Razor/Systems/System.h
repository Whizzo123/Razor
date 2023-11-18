#pragma once
#include <set>
#include "../Core.h"
#include "../EntityManager.h"
#include "../ComponentManager.h"

namespace Razor
{
	enum class RenderStage {RENDER_STAGE_MATERIAL_PASS, RENDER_STAGE_LIGHTING_PASS, RENDER_STAGE_TRANSFORMATION_PASS, RENDER_STAGE_CAMERA_PASS};
	struct RenderPipelineEntityProperties
	{
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
		private:
			std::vector<FProperty> FProperties;
			std::vector<VProperty> VProperties;
			std::vector<MProperty> MProperties;
			std::vector<BProperty> BProperties;
			std::vector<IProperty> IProperties;
		};
		void GeneratePropertySlots(uint8_t NumberOfSlots)
		{
			Slots.reserve(NumberOfSlots);
		}
	private:
		std::vector<PropertySlot> Slots;
		

	public:
		PropertySlot& GetPropertySlot(uint8_t SlotIndex)
		{
			if (SlotIndex > 0 && SlotIndex < Slots.size())
			{
				Slots[SlotIndex];
			}
		}
	};
	struct RenderSystemPipeline
	{
		std::unordered_map < RenderStage, std::unordered_map<const char*, std::shared_ptr<RenderSystem>>> PipelineSystems{};
		std::array<RenderPipelineEntityProperties, MAX_ENTITIES> EntityRenderProperties;
		void RegisterSystem(RenderStage Stage, const char* TypeName, std::shared_ptr<RenderSystem> System)
		{
			PipelineSystems[Stage].insert({ TypeName, System });
		}
		void RunSystemsFor(RenderStage Stage)
		{
			for (std::pair<const char*, std::shared_ptr<RenderSystem>> System : PipelineSystems[Stage])
			{
				System.second->Render();
			}
		}

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
		virtual void Render(std::array<RenderPipelineEntityProperties, MAX_ENTITIES>& Properties) {};
	protected:
		static RenderStage SystemRenderStage;
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
			return SystemInstPtr;
		}

		template<typename T>
		std::shared_ptr<T> RegisterRenderSystem(T SystemInst)
		{
			const char* TypeName = typeid(T).name();
			std::shared_ptr<RenderSystem> RenderSystemInstPtr = std::make_shared<T>(SystemInst);
			//if (RenderSystemPipeline[RenderSystemInstPtr->SystemRenderStage].find(TypeName) == RenderSystemPipeline[RenderSystemInstPtr->SystemRenderStage].end())
			{
				RZ_ERROR("Registering system twice" + TypeName);
				return nullptr;
			}
			//RenderSystemPipeline[RenderSystemInstPtr->SystemRenderStage].insert({ TypeName, RenderSystemInstPtr });
			return RenderSystemInstPtr;
		}

		template<typename T>
		void SetSignature(Signature InSignature)
		{
			const char* typeName = typeid(T).name();
			assert(Systems.find(typeName) != Systems.end() && "System does not exist");
			Signatures.insert({ typeName, InSignature });
		}

		void EntityDestroyed(Entity InEntity);
		void EntitySignatureChanged(Entity InEntity, Signature EntitySignature);
		void RunSystems(float dt);
		void RunRenderSystems();
		void InitSystems();
	private:
		std::unordered_map<const char*, Signature> Signatures{};
		std::unordered_map<const char*, std::shared_ptr<System>> Systems{};
		RenderSystemPipeline RenderPipeline;
		
	};

}