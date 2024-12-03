#pragma once

// FOR USE BY RAZOR APPLICATIONS
#include <stdio.h>
#include "Razor/Application.h"
#include "Razor/Log.h"
#include "Razor/Engine.h"
#include "Razor/Systems/System.h"
#include "Razor/Renderer/Model.h"
#include "Razor/Renderer/Shaders/DefaultMeshShader.h"
#include "Razor/Renderer/Shaders/DebugLightShader.h"
#include "Razor/Systems/RSCameraPass.h"
#include "Razor/Systems/RSMaterialPass.h"
#include "Razor/Systems/RSDirectionalLightingPass.h"
#include "Razor/Systems/RSSpotlightingPass.h"
#include "Razor/Systems/RSPointLightingPass.h"
#include "Razor/Systems/RSRenderPass.h"
#include "Razor/Systems/RSTransformationsPass.h"
#include "Razor/Systems/RSPickBufferRenderPass.h"
#include "Razor/Systems/RSPickBufferMaterialPass.h"
#include "Razor/Renderer/Framebuffer.h"
#include "Razor/Scene/SceneSerializer.h"