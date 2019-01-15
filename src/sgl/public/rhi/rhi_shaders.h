#pragma once

#include "core_types.h"
#include "rhi_resource.h"

/**
 * @class RHIShaderrhi/rhi_shader.h
 * @brief Base class for all platform-specific shader handlers
 */
class RHIShader : public RHIResource
{

};

/// @brief Shaders handlers base classes
/// @{
class RHIVertexShader : public RHIShader {};
class RHIGeometryShader : public RHIShader {};
class RHIPixelShader : public RHIShader {};
/// @}

