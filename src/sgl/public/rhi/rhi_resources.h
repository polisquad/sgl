#pragma once

#include "rhi_resource.h"
#include "rhi_textures.h"
#include "rhi_viewport.h"
#include "templates/ref_count.h"

/////////////////////////////////////////////////
// Resources ref pointers                      //
/////////////////////////////////////////////////

typedef RefCountPtr<RHITexture> RHITextureRef;
typedef RefCountPtr<RHITexture2D> RHITexture2DRef;
typedef RefCountPtr<RHIViewport> RHIViewportRef;

