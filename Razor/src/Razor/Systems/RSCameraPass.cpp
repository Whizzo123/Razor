#include "RSCameraPass.h"

namespace Razor
{
	void RSCameraPass::Render(RenderPipelineEntityProperties& EntityProperties)
	{
		std::shared_ptr<CameraInfo> CamInfo = std::make_shared<CameraInfo>(Renderer->GetCameraInfo());
		for (int i = 0; i <= EntityProperties.HighestEntity; i++)
		{
			EntityRenderProperty& EntityProperty = EntityProperties.Properties[i];
			for (int j = 0; j < EntityProperty.GetNumberOfSlots(); j++)
			{
				PropertySlot& Slot = EntityProperty.GetPropertySlot(j);

				Slot.AddMat4Property("projection", CamInfo->CameraProjection);
				Slot.AddMat4Property("view", CamInfo->CameraView);
				Slot.AddVec3Property("viewPos", CamInfo->CameraPosition);
			}
		}

	}
}