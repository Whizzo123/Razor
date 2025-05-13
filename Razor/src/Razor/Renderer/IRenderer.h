#pragma once

#include "../Core.h"
#include "../Component.h"
#include "../Window.h"
#include "Framebuffer.h"

namespace Razor
{

	struct RendererInfo
	{
		MeshData ObjMesh;
	};

	enum class EStencilAction
	{
		KEEP,
		REPLACE
	};

	enum class EStencilFunc
	{
		ALWAYS
	};

	enum class EShader
	{
		VERTEX,
		FRAGMENT
	};

	enum class EStatusParam
	{
		COMPILE
	};

	enum class EProgramStatusParam
	{
		LINK
	};

	enum class ETextureType
	{
		TEXTURE_2D
	};

	enum class EPixelDataFormat
	{
		RGBA
	};

	enum class EPixelDataType
	{
		UNSIGNED_BYTE
	};

	class IRenderer
	{
	public:
		virtual void InitRendererAPI() = 0;
		virtual void PollForEvents() = 0;
		virtual void TerminateRendererAPI() = 0;
		virtual void EnableDepthTesting(bool bEnable) = 0;
		virtual void EnableStencilTesting(bool bEnable) = 0;
		virtual void SetStencilOperation(EStencilAction StencilFailAction, EStencilAction DepthFailAction, EStencilAction SDPassAction) = 0;
		virtual void SetStencilFunction(EStencilFunc Function, unsigned int Ref, unsigned int Mask) = 0;
		virtual void EnableStencilWriting(bool bEnable) = 0;
		virtual void DrawMesh(RendererInfo Info) = 0;
		virtual void SwapBuffer(Window& RenderWindow) = 0;
		virtual void ClearBuffer() = 0;
		virtual void BindFrameBuffer(uint32_t = 0) = 0;
		virtual Ref<Framebuffer> CreateFrameBuffer(uint32_t Width, uint32_t Height) = 0;
		virtual CameraInfo GetCameraInfo() = 0;
		/**
		* Function to read pixels from a frame buffer out to a float ptr
		* 
		* @param X - the x-coord we are looking for in the buffer
		* @param Y - the y-coord we are looking for in the buffer
		* @param Width - the width of pixels we are reading 
		* @param Height - the height of pixels we are reading
		* @param OutPixels - the pixels resulting from the read
		* @param Buffer - int corresponding to buffer we are reading from
		*/
		virtual void ReadPixels(unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height, float* OutPixels, unsigned int Buffer) = 0;
		virtual void BackupContext() = 0;
		virtual void ResetCurrentContext() = 0;
		virtual void* GetSceneRenderedToTexture() = 0;
		virtual void SetFrameBufferTextureForScene(unsigned int SceneTexture) = 0;
		virtual void ResizeFramebuffers(unsigned int Width, unsigned int Height) = 0;
		/**
		* Function to set the viewport
		* 
		* @param X - lower left corner x-coord of viewport rectangle (in pixels)
		* @param Y - lower left corner y-coord of viewport rectangle (in pixels)
		* @param Width - width of viewport
		* @param Height - height of viewport
		*/
		virtual void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) = 0;
		/**
		* Function to set shader to use while rendering
		* 
		* @param ShaderProgramId - id for shader program to use
		*/
		virtual void UseShader(uint32_t ShaderProgramId) = 0;
		/**
		* Function to create shader of type
		*
		* @param ShaderType - type of shader to create
		*/
		virtual uint32_t CreateShader(EShader ShaderType) = 0;
		/**
		* Function to set shader source code
		*
		* @param ShaderHandle - handle for shader
		* @param Count - specifies number of elements in string and length arrays
		* @param String - array of pointers to strings containing the source code
		* @param Length - array of String lengths
		*/
		virtual void SetShaderSource(uint32_t ShaderHandle, int32_t Count, const char* const *String, const int* Length) = 0;
		/**
		* Function to compile shader
		*
		* @param ShaderHandle - handle for shader
		*/
		virtual void CompileShader(uint32_t ShaderHandle) = 0;
		/**
		* Function to get shader status param value
		*
		* @param ShaderHandle - handle for shader
		* @param Param - status param to query value
		* @param SuccessCode - value of param
		*/
		virtual void GetShaderStatusParam(uint32_t ShaderHandle, EStatusParam Param, int* SuccessCode) = 0;
		/**
		* Function to get shader log
		*
		* @param ShaderHandle - handle for shader
		* @param MaxLength - size of the char buffer for storing log
		* @param Length - actual length of string in Log
		* @param Log - array of chars to return the information log
		*/
		virtual void GetShaderLog(uint32_t ShaderHandle, int32_t MaxLength, int32_t* Length, char* Log) = 0;
		/**
		* Function to create shader program
		*
		* @return - a handle to the shader program or -1 if failed to create
		*/
		virtual uint32_t CreateShaderProgram() = 0;
		/**
		* Function to attach shader to program
		*
		* @param ShaderProgramHandle - handle for shader program to use
		* @param ShaderHandle - handle for shader to attach
		*/
		virtual void AttachShader(uint32_t ShaderProgramHandle, uint32_t ShaderHandle) = 0;
		/**
		* Function to link attached shaders in program together
		*
		* @param ShaderProgramHandle - handle for shader program to use
		*/
		virtual void LinkShaderProgram(uint32_t ShaderProgramHandle) = 0;
		/**
		* Function to get status param value for shader program
		*
		* @param ShaderProgramHandle - handle for shader program to use
		* @param Param - status param to get value for
		* @param SuccessCode - value of status param
		*/
		virtual void GetShaderProgramStatusParam(uint32_t ShaderProgramHandle, EProgramStatusParam Param, int* SuccessCode) = 0;
		/**
		* Function to get shader program log
		*
		* @param ShaderProgramHandle - handle for shader program to use
		* @param MaxLength - size of the char buffer for storing log
		* @param Length - actual length of string in Log
		* @param Log - array of chars to return the information log
		*/
		virtual void GetShaderProgramLog(uint32_t ShaderProgramHandle, int32_t MaxLength, int32_t* Length, char* Log) = 0;
		/**
		* Function to delete shader
		*
		* @param ShaderProgramHandle - handle for shader program to delete
		*/
		virtual void DeleteShader(uint32_t ShaderHandle) = 0;
		/**
		* Function to generate textures
		*
		* @param NumberOfTextures - number of textures to generate
		* @param Textures - array to store texture handles
		*/
		virtual void GenerateTextures(int32_t NumberOfTextures, uint32_t* Textures) = 0;
		/**
		* Function to bind texture for use
		*
		* @param TextureType - type of texture
		* @param TextureHandle - handle for texture to bind
		*/
		virtual void BindTexture(ETextureType TextureType, uint32_t TextureHandle) = 0;
		/**
		* Function to write data to bound texture
		*
		* @param TextureType - type of texture
		* @param LevelOfDetail - level of detail for texture
		* @param NumberOfColorComponents - specifies number of color components in the texture
		* @param Width - width of the texture
		* @param Height - height of the texture
		* @param Border - value must be 0
		* @param DataFormat - format of the pixel data 
		* @param DataType - type of the pixel data
		* @param Data - pointer to image data in memory
		*/
		virtual void WriteTexture2dData(ETextureType TextureType, int32_t LevelOfDetail, EPixelDataFormat NumberOfColorComponents, int32_t Width, int32_t Height, int32_t Border, EPixelDataFormat DataFormat, EPixelDataType DataType, const void* data) = 0;
		/**
		* Function to generate mipmap
		*
		* @param TextureType - type of texture
		*/
		virtual void GenerateMipmap(ETextureType TextureType) = 0;
		//TODO remove this
		Camera RendererCamera;
	};

}