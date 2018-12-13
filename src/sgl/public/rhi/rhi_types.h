#ifndef SGL_RHI_TYPES_H
#define SGL_RHI_TYPES_H

/// @brief List of pixel formats
enum PixelFormat
{
	PF_Unkown = 0
};

/// @brief Buffer usage
enum UniformBufferUsage
{
	UBU_SingleDraw,
	UBU_SingleFrame,
	UBU_MultiFrame
};

/// @brief Type of uniform resource
enum UniformBufferBaseType
{
	UBBT_INVALID,
	UBBT_BOOL,
	UBBT_INT32,
	UBBT_UINT32,
	UBBT_FLOAT32,
	UBBT_STRUCT,
	UBBT_SAMPLE,
	UBBT_TEXTURE,

	// Number of types
	UniformBufferBaseTypeNum
};

#endif