#pragma once
#include "IRenderer.h"
#include "Font/Font.h"


typedef unsigned int GLenum;
typedef unsigned int GLuint;
struct GLFWwindow;

namespace Razor
{

	class RAZOR_API OpenGLRenderer : public IRenderer
	{

	public:
		void InitRendererAPI() override;
		void PollForEvents() override;
		void TerminateRendererAPI() override;
		void EnableDepthTesting(bool bEnable) override;
		void EnableStencilTesting(bool bEnable) override;
		void SetStencilOperation(EStencilAction StencilFailAction, EStencilAction DepthFailAction, EStencilAction SDPassAction) override;
		void SetStencilFunction(EStencilFunc Function, unsigned int Ref, unsigned int Mask) override;
		void EnableStencilWriting(bool bEnable) override;
		CameraInfo GetCameraInfo() override;
		void DrawMesh(RendererInfo Info) override;
		void DrawLine(const DebugLine& line) override;
		void DrawTriangle(const DebugTriangle& triangle) override;
		void ClearBuffer() override;
		void BindFrameBuffer(uint32_t BufferIndex = 0) override;
		Ref<Framebuffer> CreateFrameBuffer(uint32_t Width, uint32_t Height) override;
		void ReadPixels(unsigned int X, unsigned int Y, unsigned int I1, unsigned int I2, float* OutPixels, unsigned int Buffer) override;
		void SwapBuffer(Window& window) override;
		void BackupContext() override;
		void ResetCurrentContext() override;
		void* GetSceneRenderedToTexture() override;
		void SetFrameBufferTextureForScene(unsigned int SceneTexture) override;
		void ResizeFramebuffers(unsigned int Width, unsigned int Height) override;
		/**
		* Function to set viewport
		* 
		* @param X - lower left corner x-coord of viewport rectangle (in pixels)
		* @param Y - lower left corner y-coord of viewport rectangle (in pixels)
		* @param Width - width of viewport
		* @param Height - height of viewport
		*/
		void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) override;
		/**
		* Function to set shader to use while rendering
		* 
		* @param ShaderProgramId - id for shader program to use
		*/
		void UseShader(uint32_t ShaderProgramId) override;
		/**
		* Function to create shader of type
		*
		* @param ShaderType - type of shader to create
		*/
		uint32_t CreateShader(EShader ShaderType) override;
		/**
		* Function to set shader source code
		*
		* @param ShaderHandle - handle for shader
		* @param Count - specifies number of elements in string and length arrays
		* @param String - array of pointers to strings containing the source code
		* @param Length - array of String lengths
		*/
		void SetShaderSource(uint32_t ShaderHandle, int32_t Count, const char* const* String, const int* Length) override;
		/**
		* Function to compile shader
		*
		* @param ShaderHandle - handle for shader
		*/
		void CompileShader(uint32_t ShaderHandle) override;
		/**
		* Function to get shader status param value
		*
		* @param ShaderHandle - handle for shader
		* @param Param - status param to query value
		* @param SuccessCode - value of param
		*/
		void GetShaderStatusParam(uint32_t ShaderHandle, EStatusParam Param, int* SuccessCode) override;
		/**
		* Function to get shader log
		*
		* @param ShaderHandle - handle for shader
		* @param MaxLength - size of the char buffer for storing log
		* @param Length - actual length of string in Log
		* @param Log - array of chars to return the information log
		*/
		void GetShaderLog(uint32_t ShaderHandle, int32_t MaxLength, int32_t* Length, char* Log) override;
		/**
		* Function to create shader program
		*
		* @return - a handle to the shader program or -1 if failed to create
		*/
		uint32_t CreateShaderProgram() override;
		/**
		* Function to attach shader to program
		*
		* @param ShaderProgramHandle - handle for shader program to use
		* @param ShaderHandle - handle for shader to attach
		*/
		void AttachShader(uint32_t ShaderProgramHandle, uint32_t ShaderHandle) override;
		/**
		* Function to link attached shaders in program together
		*
		* @param ShaderProgramHandle - handle for shader program to use
		*/
		void LinkShaderProgram(uint32_t ShaderProgramHandle) override;
		/**
		* Function to get status param value for shader program
		*
		* @param ShaderProgramHandle - handle for shader program to use
		* @param Param - status param to get value for
		* @param SuccessCode - value of status param
		*/
		void GetShaderProgramStatusParam(uint32_t ShaderProgramHandle, EProgramStatusParam Param, int* SuccessCode) override;
		/**
		* Function to get shader program log
		*
		* @param ShaderProgramHandle - handle for shader program to use
		* @param MaxLength - size of the char buffer for storing log
		* @param Length - actual length of string in Log
		* @param Log - array of chars to return the information log
		*/
		void GetShaderProgramLog(uint32_t ShaderProgramHandle, int32_t MaxLength, int32_t* Length, char* Log) override;
		/**
		* Function to delete shader
		*
		* @param ShaderProgramHandle - handle for shader program to delete
		*/
		void DeleteShader(uint32_t ShaderHandle) override;
		/**
		* Function to generate textures
		*
		* @param NumberOfTextures - number of textures to generate
		* @param Textures - array to store texture handles
		*/
		void GenerateTextures(int32_t NumberOfTextures, uint32_t* Textures) override;
		/**
		* Function to bind texture for use
		*
		* @param TextureType - type of texture
		* @param TextureHandle - handle for texture to bind
		*/
		void BindTexture(ETextureType TextureType, uint32_t TextureHandle) override;
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
		void WriteTexture2dData(ETextureType TextureType, int32_t LevelOfDetail, EPixelDataFormat NumberOfColorComponents, int32_t Width, int32_t Height, int32_t Border, EPixelDataFormat DataFormat, EPixelDataType DataType, const void* data) override;
		/**
		* Function to generate mipmap
		*
		* @param TextureType - type of texture
		*/
		void GenerateMipmap(ETextureType TextureType) override;

		void WriteTextureBitmapData(Bitmap& bitmap) override;

		void DisableByteAlignment() override;

		void SetByteAlignment(unsigned int alignment) override;

		void CreateText(Text& text) override;

		void RenderText(Text& text, Font& font) override;

		void SetTextureParameterInt(ETextureType TextureType, ETextureOption TextureOption, ETextureValue TextureValue) override;
	private:
		GLenum Translate(EStencilAction StencilAction);
		GLenum Translate(EStencilFunc StencilFunction);

		GLFWwindow* GLFWBackupContext;

		std::unordered_map<EStencilAction, GLenum> StencilActionTranslation;
		std::unordered_map<EStencilFunc, GLenum> StencilFuncTranslation;
		std::unordered_map<EShader, GLenum> ShaderTranslation; /** Translation map for shader type */
		std::unordered_map<EStatusParam, GLenum> StatusParamTranslation; /** Translation map for shader status param */
		std::unordered_map<EProgramStatusParam, GLenum> ProgramStatusParamTranslation; /** Translation map for shader program status param */
		std::unordered_map<ETextureType, GLenum> TextureTypeTranslation; /** Translation map for texture type */
		std::unordered_map<ETextureOption, GLenum> TextureOptionTranslation; /** Translation map for texture option */
		std::unordered_map<ETextureValue, GLenum> TextureValueTranslation; /** Translation map for texture value */
		std::unordered_map<EPixelDataFormat, GLenum> PixelDataFormatTranslation; /** Translation map for pixel data format */
		std::unordered_map<EPixelDataType, GLenum> PixelDataTypeTranslation; /** Translation map for pixel data type */

		GLuint SceneTexture;
	};

}