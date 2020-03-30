/* FNA3D - 3D Graphics Library for FNA
 *
 * Copyright (c) 2020 Ethan Lee
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software in a
 * product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Ethan "flibitijibibo" Lee <flibitijibibo@flibitijibibo.com>
 *
 */

#ifndef FNA3D_DRIVER_H
#define FNA3D_DRIVER_H

#include "FNA3D.h"
#include "mojoshader.h"

/* Internal Helper Macros */

#define LinkedList_Add(start, toAdd, curr) \
	toAdd->next = NULL; \
	if (start == NULL) \
	{ \
		start = toAdd; \
	} \
	else \
	{ \
		curr = start; \
		while (curr->next != NULL) \
		{ \
			curr = curr->next; \
		} \
		curr->next = toAdd; \
	}

/* Logging */

extern void FNA3D_LogInfo(const char *fmt, ...);
extern void FNA3D_LogWarn(const char *fmt, ...);
extern void FNA3D_LogError(const char *fmt, ...);

/* FNA3D_Device Definition */

typedef struct FNA3D_Renderer FNA3D_Renderer;

struct FNA3D_Device
{
	/* Quit */

	void (*DestroyDevice)(FNA3D_Device *device);

	/* Begin/End Frame */

	void (*BeginFrame)(FNA3D_Renderer *driverData);

	void (*SwapBuffers)(
		FNA3D_Renderer *driverData,
		FNA3D_Rect *sourceRectangle,
		FNA3D_Rect *destinationRectangle,
		void* overrideWindowHandle
	);

	void (*SetPresentationInterval)(
		FNA3D_Renderer *driverData,
		FNA3D_PresentInterval presentInterval
	);

	/* Drawing */

	void (*Clear)(
		FNA3D_Renderer *driverData,
		FNA3D_ClearOptions options,
		FNA3D_Vec4 *color,
		float depth,
		int32_t stencil
	);

	void (*DrawIndexedPrimitives)(
		FNA3D_Renderer *driverData,
		FNA3D_PrimitiveType primitiveType,
		int32_t baseVertex,
		int32_t minVertexIndex,
		int32_t numVertices,
		int32_t startIndex,
		int32_t primitiveCount,
		FNA3D_Buffer *indices,
		FNA3D_IndexElementSize indexElementSize
	);
	void (*DrawInstancedPrimitives)(
		FNA3D_Renderer *driverData,
		FNA3D_PrimitiveType primitiveType,
		int32_t baseVertex,
		int32_t minVertexIndex,
		int32_t numVertices,
		int32_t startIndex,
		int32_t primitiveCount,
		int32_t instanceCount,
		FNA3D_Buffer *indices,
		FNA3D_IndexElementSize indexElementSize
	);
	void (*DrawPrimitives)(
		FNA3D_Renderer *driverData,
		FNA3D_PrimitiveType primitiveType,
		int32_t vertexStart,
		int32_t primitiveCount
	);
	void (*DrawUserIndexedPrimitives)(
		FNA3D_Renderer *driverData,
		FNA3D_PrimitiveType primitiveType,
		void* vertexData,
		int32_t vertexOffset,
		int32_t numVertices,
		void* indexData,
		int32_t indexOffset,
		FNA3D_IndexElementSize indexElementSize,
		int32_t primitiveCount
	);
	void (*DrawUserPrimitives)(
		FNA3D_Renderer *driverData,
		FNA3D_PrimitiveType primitiveType,
		void* vertexData,
		int32_t vertexOffset,
		int32_t primitiveCount
	);

	/* Mutable Render States */

	void (*SetViewport)(FNA3D_Renderer *driverData, FNA3D_Viewport *viewport);
	void (*SetScissorRect)(FNA3D_Renderer *driverData, FNA3D_Rect *scissor);

	void (*GetBlendFactor)(
		FNA3D_Renderer *driverData,
		FNA3D_Color *blendFactor
	);
	void (*SetBlendFactor)(
		FNA3D_Renderer *driverData,
		FNA3D_Color *blendFactor
	);

	int32_t (*GetMultiSampleMask)(FNA3D_Renderer *driverData);
	void (*SetMultiSampleMask)(FNA3D_Renderer *driverData, int32_t mask);

	int32_t (*GetReferenceStencil)(FNA3D_Renderer *driverData);
	void (*SetReferenceStencil)(FNA3D_Renderer *driverData, int32_t ref);

	/* Immutable Render States */

	void (*SetBlendState)(
		FNA3D_Renderer *driverData,
		FNA3D_BlendState *blendState
	);
	void (*SetDepthStencilState)(
		FNA3D_Renderer *driverData,
		FNA3D_DepthStencilState *depthStencilState
	);
	void (*ApplyRasterizerState)(
		FNA3D_Renderer *driverData,
		FNA3D_RasterizerState *rasterizerState
	);
	void (*VerifySampler)(
		FNA3D_Renderer *driverData,
		int32_t index,
		FNA3D_Texture *texture,
		FNA3D_SamplerState *sampler
	);

	/* Vertex State */

	void (*ApplyVertexBufferBindings)(
		FNA3D_Renderer *driverData,
		FNA3D_VertexBufferBinding *bindings,
		int32_t numBindings,
		uint8_t bindingsUpdated,
		int32_t baseVertex
	);

	void (*ApplyVertexDeclaration)(
		FNA3D_Renderer *driverData,
		FNA3D_VertexDeclaration *vertexDeclaration,
		void* ptr,
		int32_t vertexOffset
	);

	/* Render Targets */

	void (*SetRenderTargets)(
		FNA3D_Renderer *driverData,
		FNA3D_RenderTargetBinding *renderTargets,
		int32_t numRenderTargets,
		FNA3D_Renderbuffer *renderbuffer,
		FNA3D_DepthFormat depthFormat
	);

	void (*ResolveTarget)(
		FNA3D_Renderer *driverData,
		FNA3D_RenderTargetBinding *target
	);

	/* Backbuffer Functions */

	void (*ResetBackbuffer)(
		FNA3D_Renderer *driverData,
		FNA3D_PresentationParameters *presentationParameters
	);

	void (*ReadBackbuffer)(
		FNA3D_Renderer *driverData,
		void* data,
		int32_t dataLen,
		int32_t startIndex,
		int32_t elementCount,
		int32_t elementSizeInBytes,
		int32_t x,
		int32_t y,
		int32_t w,
		int32_t h
	);

	void (*GetBackbufferSize)(
		FNA3D_Renderer *driverData,
		int32_t *w,
		int32_t *h
	);

	FNA3D_SurfaceFormat (*GetBackbufferSurfaceFormat)(FNA3D_Renderer *driverData);

	FNA3D_DepthFormat (*GetBackbufferDepthFormat)(FNA3D_Renderer *driverData);

	int32_t (*GetBackbufferMultiSampleCount)(FNA3D_Renderer *driverData);

	/* Textures */

	FNA3D_Texture* (*CreateTexture2D)(
		FNA3D_Renderer *driverData,
		FNA3D_SurfaceFormat format,
		int32_t width,
		int32_t height,
		int32_t levelCount,
		uint8_t isRenderTarget
	);
	FNA3D_Texture* (*CreateTexture3D)(
		FNA3D_Renderer *driverData,
		FNA3D_SurfaceFormat format,
		int32_t width,
		int32_t height,
		int32_t depth,
		int32_t levelCount
	);
	FNA3D_Texture* (*CreateTextureCube)(
		FNA3D_Renderer *driverData,
		FNA3D_SurfaceFormat format,
		int32_t size,
		int32_t levelCount,
		uint8_t isRenderTarget
	);
	void (*AddDisposeTexture)(
		FNA3D_Renderer *driverData,
		FNA3D_Texture *texture
	);
	void (*SetTextureData2D)(
		FNA3D_Renderer *driverData,
		FNA3D_Texture *texture,
		FNA3D_SurfaceFormat format,
		int32_t x,
		int32_t y,
		int32_t w,
		int32_t h,
		int32_t level,
		void* data,
		int32_t dataLength
	);
	void (*SetTextureData3D)(
		FNA3D_Renderer *driverData,
		FNA3D_Texture *texture,
		FNA3D_SurfaceFormat format,
		int32_t level,
		int32_t left,
		int32_t top,
		int32_t right,
		int32_t bottom,
		int32_t front,
		int32_t back,
		void* data,
		int32_t dataLength
	);
	void (*SetTextureDataCube)(
		FNA3D_Renderer *driverData,
		FNA3D_Texture *texture,
		FNA3D_SurfaceFormat format,
		int32_t x,
		int32_t y,
		int32_t w,
		int32_t h,
		FNA3D_CubeMapFace cubeMapFace,
		int32_t level,
		void* data,
		int32_t dataLength
	);
	void (*SetTextureDataYUV)(
		FNA3D_Renderer *driverData,
		FNA3D_Texture *y,
		FNA3D_Texture *u,
		FNA3D_Texture *v,
		int32_t w,
		int32_t h,
		void* ptr
	);
	void (*GetTextureData2D)(
		FNA3D_Renderer *driverData,
		FNA3D_Texture *texture,
		FNA3D_SurfaceFormat format,
		int32_t textureWidth,
		int32_t textureHeight,
		int32_t level,
		int32_t x,
		int32_t y,
		int32_t w,
		int32_t h,
		void* data,
		int32_t startIndex,
		int32_t elementCount,
		int32_t elementSizeInBytes
	);
	void (*GetTextureData3D)(
		FNA3D_Renderer *driverData,
		FNA3D_Texture *texture,
		FNA3D_SurfaceFormat format,
		int32_t left,
		int32_t top,
		int32_t front,
		int32_t right,
		int32_t bottom,
		int32_t back,
		int32_t level,
		void* data,
		int32_t startIndex,
		int32_t elementCount,
		int32_t elementSizeInBytes
	);
	void (*GetTextureDataCube)(
		FNA3D_Renderer *driverData,
		FNA3D_Texture *texture,
		FNA3D_SurfaceFormat format,
		int32_t textureSize,
		FNA3D_CubeMapFace cubeMapFace,
		int32_t level,
		int32_t x,
		int32_t y,
		int32_t w,
		int32_t h,
		void* data,
		int32_t startIndex,
		int32_t elementCount,
		int32_t elementSizeInBytes
	);

	/* Renderbuffers */

	FNA3D_Renderbuffer* (*GenColorRenderbuffer)(
		FNA3D_Renderer *driverData,
		int32_t width,
		int32_t height,
		FNA3D_SurfaceFormat format,
		int32_t multiSampleCount,
		FNA3D_Texture *texture
	);
	FNA3D_Renderbuffer* (*GenDepthStencilRenderbuffer)(
		FNA3D_Renderer *driverData,
		int32_t width,
		int32_t height,
		FNA3D_DepthFormat format,
		int32_t multiSampleCount
	);
	void (*AddDisposeRenderbuffer)(
		FNA3D_Renderer *driverData,
		FNA3D_Renderbuffer *renderbuffer
	);

	/* Vertex Buffers */

	FNA3D_Buffer* (*GenVertexBuffer)(
		FNA3D_Renderer *driverData,
		uint8_t dynamic,
		FNA3D_BufferUsage usage,
		int32_t vertexCount,
		int32_t vertexStride
	);
	void (*AddDisposeVertexBuffer)(
		FNA3D_Renderer *driverData,
		FNA3D_Buffer *buffer
	);
	void (*SetVertexBufferData)(
		FNA3D_Renderer *driverData,
		FNA3D_Buffer *buffer,
		int32_t offsetInBytes,
		void* data,
		int32_t dataLength,
		FNA3D_SetDataOptions options
	);
	void (*GetVertexBufferData)(
		FNA3D_Renderer *driverData,
		FNA3D_Buffer *buffer,
		int32_t offsetInBytes,
		void* data,
		int32_t startIndex,
		int32_t elementCount,
		int32_t elementSizeInBytes,
		int32_t vertexStride
	);

	/* Index Buffers */

	FNA3D_Buffer* (*GenIndexBuffer)(
		FNA3D_Renderer *driverData,
		uint8_t dynamic,
		FNA3D_BufferUsage usage,
		int32_t indexCount,
		FNA3D_IndexElementSize indexElementSize
	);
	void (*AddDisposeIndexBuffer)(
		FNA3D_Renderer *driverData,
		FNA3D_Buffer *buffer
	);
	void (*SetIndexBufferData)(
		FNA3D_Renderer *driverData,
		FNA3D_Buffer *buffer,
		int32_t offsetInBytes,
		void* data,
		int32_t dataLength,
		FNA3D_SetDataOptions options
	);
	void (*GetIndexBufferData)(
		FNA3D_Renderer *driverData,
		FNA3D_Buffer *buffer,
		int32_t offsetInBytes,
		void* data,
		int32_t startIndex,
		int32_t elementCount,
		int32_t elementSizeInBytes
	);

	/* Effects */

	FNA3D_Effect* (*CreateEffect)(
		FNA3D_Renderer *driverData,
		uint8_t *effectCode,
		uint32_t effectCodeLength
	);
	FNA3D_Effect* (*CloneEffect)(
		FNA3D_Renderer *driverData,
		FNA3D_Effect *effect
	);
	void (*AddDisposeEffect)(
		FNA3D_Renderer *driverData,
		FNA3D_Effect *effect
	);
	void (*ApplyEffect)(
		FNA3D_Renderer *driverData,
		FNA3D_Effect *effect,
		MOJOSHADER_effectTechnique *technique,
		uint32_t pass,
		MOJOSHADER_effectStateChanges *stateChanges
	);
	void (*BeginPassRestore)(
		FNA3D_Renderer *driverData,
		FNA3D_Effect *effect,
		MOJOSHADER_effectStateChanges *stateChanges
	);
	void (*EndPassRestore)(
		FNA3D_Renderer *driverData,
		FNA3D_Effect *effect
	);

	/* Queries */

	FNA3D_Query* (*CreateQuery)(FNA3D_Renderer *driverData);
	void (*AddDisposeQuery)(FNA3D_Renderer *driverData, FNA3D_Query *query);
	void (*QueryBegin)(FNA3D_Renderer *driverData, FNA3D_Query *query);
	void (*QueryEnd)(FNA3D_Renderer *driverData, FNA3D_Query *query);
	uint8_t (*QueryComplete)(FNA3D_Renderer *driverData, FNA3D_Query *query);
	int32_t (*QueryPixelCount)(
		FNA3D_Renderer *driverData,
		FNA3D_Query *query
	);

	/* Feature Queries */

	uint8_t (*SupportsDXT1)(FNA3D_Renderer *driverData);
	uint8_t (*SupportsS3TC)(FNA3D_Renderer *driverData);
	uint8_t (*SupportsHardwareInstancing)(FNA3D_Renderer *driverData);
	uint8_t (*SupportsNoOverwrite)(FNA3D_Renderer *driverData);

	int32_t (*GetMaxTextureSlots)(FNA3D_Renderer *driverData);
	int32_t (*GetMaxMultiSampleCount)(FNA3D_Renderer *driverData);

	/* Debugging */

	void (*SetStringMarker)(FNA3D_Renderer *driverData, const char *text);

	/* Buffer Objects */

	intptr_t (*GetBufferSize)(FNA3D_Buffer *buffer);

	/* Effect Objects */

	MOJOSHADER_effect* (*GetEffectData)(FNA3D_Effect *effect);

	/* Opaque pointer for the Driver */
	FNA3D_Renderer *driverData;
};

#define ASSIGN_DRIVER_FUNC(func, name) \
	result->func = name##_##func;
#define ASSIGN_DRIVER(name) \
	ASSIGN_DRIVER_FUNC(DestroyDevice, name) \
	ASSIGN_DRIVER_FUNC(BeginFrame, name) \
	ASSIGN_DRIVER_FUNC(SwapBuffers, name) \
	ASSIGN_DRIVER_FUNC(SetPresentationInterval, name) \
	ASSIGN_DRIVER_FUNC(SetPresentationInterval, name) \
	ASSIGN_DRIVER_FUNC(Clear, name) \
	ASSIGN_DRIVER_FUNC(DrawIndexedPrimitives, name) \
	ASSIGN_DRIVER_FUNC(DrawInstancedPrimitives, name) \
	ASSIGN_DRIVER_FUNC(DrawPrimitives, name) \
	ASSIGN_DRIVER_FUNC(DrawUserIndexedPrimitives, name) \
	ASSIGN_DRIVER_FUNC(DrawUserPrimitives, name) \
	ASSIGN_DRIVER_FUNC(SetViewport, name) \
	ASSIGN_DRIVER_FUNC(SetScissorRect, name) \
	ASSIGN_DRIVER_FUNC(GetBlendFactor, name) \
	ASSIGN_DRIVER_FUNC(SetBlendFactor, name) \
	ASSIGN_DRIVER_FUNC(GetMultiSampleMask, name) \
	ASSIGN_DRIVER_FUNC(SetMultiSampleMask, name) \
	ASSIGN_DRIVER_FUNC(GetReferenceStencil, name) \
	ASSIGN_DRIVER_FUNC(SetReferenceStencil, name) \
	ASSIGN_DRIVER_FUNC(SetBlendState, name) \
	ASSIGN_DRIVER_FUNC(SetDepthStencilState, name) \
	ASSIGN_DRIVER_FUNC(ApplyRasterizerState, name) \
	ASSIGN_DRIVER_FUNC(VerifySampler, name) \
	ASSIGN_DRIVER_FUNC(ApplyVertexBufferBindings, name) \
	ASSIGN_DRIVER_FUNC(ApplyVertexDeclaration, name) \
	ASSIGN_DRIVER_FUNC(SetRenderTargets, name) \
	ASSIGN_DRIVER_FUNC(ResolveTarget, name) \
	ASSIGN_DRIVER_FUNC(ResetBackbuffer, name) \
	ASSIGN_DRIVER_FUNC(ReadBackbuffer, name) \
	ASSIGN_DRIVER_FUNC(GetBackbufferSize, name) \
	ASSIGN_DRIVER_FUNC(GetBackbufferSurfaceFormat, name) \
	ASSIGN_DRIVER_FUNC(GetBackbufferDepthFormat, name) \
	ASSIGN_DRIVER_FUNC(GetBackbufferMultiSampleCount, name) \
	ASSIGN_DRIVER_FUNC(CreateTexture2D, name) \
	ASSIGN_DRIVER_FUNC(CreateTexture3D, name) \
	ASSIGN_DRIVER_FUNC(CreateTextureCube, name) \
	ASSIGN_DRIVER_FUNC(AddDisposeTexture, name) \
	ASSIGN_DRIVER_FUNC(SetTextureData2D, name) \
	ASSIGN_DRIVER_FUNC(SetTextureData3D, name) \
	ASSIGN_DRIVER_FUNC(SetTextureDataCube, name) \
	ASSIGN_DRIVER_FUNC(SetTextureDataYUV, name) \
	ASSIGN_DRIVER_FUNC(GetTextureData2D, name) \
	ASSIGN_DRIVER_FUNC(GetTextureData3D, name) \
	ASSIGN_DRIVER_FUNC(GetTextureDataCube, name) \
	ASSIGN_DRIVER_FUNC(GenColorRenderbuffer, name) \
	ASSIGN_DRIVER_FUNC(GenDepthStencilRenderbuffer, name) \
	ASSIGN_DRIVER_FUNC(AddDisposeRenderbuffer, name) \
	ASSIGN_DRIVER_FUNC(GenVertexBuffer, name) \
	ASSIGN_DRIVER_FUNC(GenIndexBuffer, name) \
	ASSIGN_DRIVER_FUNC(AddDisposeVertexBuffer, name) \
	ASSIGN_DRIVER_FUNC(SetVertexBufferData, name) \
	ASSIGN_DRIVER_FUNC(GetVertexBufferData, name) \
	ASSIGN_DRIVER_FUNC(AddDisposeIndexBuffer, name) \
	ASSIGN_DRIVER_FUNC(SetIndexBufferData, name) \
	ASSIGN_DRIVER_FUNC(GetIndexBufferData, name) \
	ASSIGN_DRIVER_FUNC(CreateEffect, name) \
	ASSIGN_DRIVER_FUNC(CloneEffect, name) \
	ASSIGN_DRIVER_FUNC(AddDisposeEffect, name) \
	ASSIGN_DRIVER_FUNC(ApplyEffect, name) \
	ASSIGN_DRIVER_FUNC(BeginPassRestore, name) \
	ASSIGN_DRIVER_FUNC(EndPassRestore, name) \
	ASSIGN_DRIVER_FUNC(CreateQuery, name) \
	ASSIGN_DRIVER_FUNC(AddDisposeQuery, name) \
	ASSIGN_DRIVER_FUNC(QueryBegin, name) \
	ASSIGN_DRIVER_FUNC(QueryEnd, name) \
	ASSIGN_DRIVER_FUNC(QueryComplete, name) \
	ASSIGN_DRIVER_FUNC(QueryPixelCount, name) \
	ASSIGN_DRIVER_FUNC(SupportsDXT1, name) \
	ASSIGN_DRIVER_FUNC(SupportsS3TC, name) \
	ASSIGN_DRIVER_FUNC(SupportsHardwareInstancing, name) \
	ASSIGN_DRIVER_FUNC(SupportsNoOverwrite, name) \
	ASSIGN_DRIVER_FUNC(GetMaxTextureSlots, name) \
	ASSIGN_DRIVER_FUNC(GetMaxMultiSampleCount, name) \
	ASSIGN_DRIVER_FUNC(SetStringMarker, name) \
	ASSIGN_DRIVER_FUNC(GetBufferSize, name) \
	ASSIGN_DRIVER_FUNC(GetEffectData, name)

typedef struct FNA3D_Driver
{
	const char *Name;
	uint8_t (*PrepareWindowAttributes)(uint32_t *flags);
	void (*GetDrawableSize)(void* window, int32_t *x, int32_t *y);
	FNA3D_Device* (*CreateDevice)(
		FNA3D_PresentationParameters *presentationParameters,
		uint8_t debugMode
	);
} FNA3D_Driver;

extern FNA3D_Driver VulkanDriver;
extern FNA3D_Driver D3D11Driver;
extern FNA3D_Driver MetalDriver;
extern FNA3D_Driver OpenGLDriver;
extern FNA3D_Driver ModernGLDriver;
extern FNA3D_Driver ThreadedGLDriver;
extern FNA3D_Driver GNMXDriver;

#endif /* FNA3D_DRIVER_H */

/* vim: set noexpandtab shiftwidth=8 tabstop=8: */
