# Lex Shader DSL — RFC

**Version:** 0.1.0-draft
**Date:** 2026-03-02
**Status:** Draft - Community Feedback Needed
**Author:** David
**Target:** Lex v0.3.0

---

## Overview

Lex Shader DSL estende il linguaggio Lex per supportare la definizione dichiarativa di shader grafici. Un singolo file `.lex` può generare:

- **GLSL** (OpenGL/Vulkan)
- **HLSL** (DirectX)
- **MSL** (Metal/macOS)
- **C++ headers** (uniforms structs)
- **Aether context** (documentazione automatica)

---

## Motivation

### Problems with Current Approach

| Problem | Current | With Lex Shader DSL |
|---------|---------|---------------------|
| Cross-compilation | Manual GLSL→HLSL→MSL | Automatic |
| Quality variants | `#ifdef` spaghetti | Declarative variants |
| Type safety | None between C++/GLSL | Generated structs match |
| Documentation | Separate from code | Auto-generated |
| Hot-reload | Manual file watching | Integrated with engine |

### Why Not Existing Solutions?

| Solution | Why Not Suitable |
|----------|------------------|
| **bgfx** | Too low-level, C++ only |
| **SPIRV-Cross** | Compiler, not DSL |
| **ShaderToy** | Web-only, no export |
| **Unity ShaderLab** | Unity-specific |
| **Unreal HLSL** | Unreal-specific |

Lex Shader DSL è **game-engine agnostic** ma si integra nativamente con Imperium.

---

## Proposed Syntax

### Basic Shader

```lex
shader Unlit {
    name: "unlit"
    targets: [glsl, hlsl, msl]
    
    inputs: {
        position: vec3 @location(0)
        uv: vec2 @location(1)
    }
    
    uniforms: {
        model: mat4
        view: mat4
        projection: mat4
        mainTexture: texture2D
    }
    
    vertex: {
        output.position = projection * view * model * vec4(position, 1.0)
        output.uv = uv
    }
    
    fragment: {
        output.color = texture(mainTexture, input.uv)
    }
}
```

### PBR Material Shader

```lex
shader PBRMaterial {
    name: "pbr_model"
    targets: [glsl, hlsl, msl]
    
    inputs: {
        position: vec3 @location(0)
        normal: vec3 @location(1)
        tangent: vec3 @location(2)
        uv: vec2 @location(3)
    }
    
    uniforms: {
        // Transform
        model: mat4
        view: mat4
        projection: mat4
        normalMatrix: mat3
        
        // Camera
        cameraPosition: vec3
        
        // Material
        albedo: texture2D
        normalMap: texture2D
        metallicRoughness: texture2D
        metallic: float = 0.5
        roughness: float = 0.5
        
        // Lighting
        lightDirection: vec3
        lightColor: vec3
        ambientColor: vec3
    }
    
    vertex: {
        output.worldPos = (model * vec4(position, 1.0)).xyz
        output.normal = normalize(normalMatrix * normal)
        output.tangent = normalize(normalMatrix * tangent)
        output.uv = uv
        output.position = projection * view * model * vec4(position, 1.0)
    }
    
    fragment: {
        // Sample textures
        let albedoColor = texture(albedo, input.uv).rgb
        let metallicValue = texture(metallicRoughness, input.uv).b * metallic
        let roughnessValue = texture(metallicRoughness, input.uv).g * roughness
        
        // Normal mapping
        let N = calculateNormal(input.normal, input.tangent, normalMap, input.uv)
        
        // PBR lighting (Cook-Torrance)
        let V = normalize(cameraPosition - input.worldPos)
        let L = normalize(lightDirection)
        let H = normalize(V + L)
        
        // BRDF
        let F = fresnelSchlick(max(dot(H, V), 0.0), mix(vec3(0.04), albedoColor, metallicValue))
        let D = distributionGGX(N, H, roughnessValue)
        let G = geometrySmith(N, V, L, roughnessValue)
        
        let numerator = D * G * F
        let denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001
        let specular = numerator / denominator
        
        let kD = (1.0 - F) * (1.0 - metallicValue)
        let diffuse = kD * albedoColor / PI
        
        let NdotL = max(dot(N, L), 0.0)
        output.color = vec4((diffuse + specular) * lightColor * NdotL + ambientColor * albedoColor, 1.0)
    }
    
    // Quality variants
    variants: {
        low: {
            defines: { SAMPLES: 1, SHADOWS: false, SSAO: false }
        }
        medium: {
            defines: { SAMPLES: 4, SHADOWS: true, SSAO: false }
        }
        high: {
            defines: { SAMPLES: 16, SHADOWS: true, SSAO: true }
        }
    }
}
```

### Post-Process Shader

```lex
shader Bloom {
    name: "bloom"
    type: post_process
    targets: [glsl]
    
    uniforms: {
        sceneTexture: texture2D
        threshold: float = 0.8
        intensity: float = 1.0
    }
    
    fragment: {
        let color = texture(sceneTexture, input.uv)
        
        // Extract bright areas
        let brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722))
        if (brightness > threshold) {
            output.color = color * intensity
        } else {
            output.color = vec4(0.0)
        }
    }
}
```

---

## Generated Output

### File Structure

```
shaders/
├── glsl/
│   ├── pbr_model.vert
│   ├── pbr_model.frag
│   ├── pbr_model_low.frag
│   ├── pbr_model_medium.frag
│   └── pbr_model_high.frag
├── hlsl/
│   ├── pbr_model.vs
│   ├── pbr_model.ps
│   └── ...
├── msl/
│   └── pbr_model.metal
└── generated/
    ├── pbr_material.hpp        # C++ uniforms struct
    └── pbr_material_aether.md  # AI context
```

### GLSL Output Example

```glsl
// Auto-generated by Lex Compiler v0.3.0
// Source: pbr_material.lex
// Target: GLSL 450

#version 450 core

// Inputs
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uv;

// Uniforms
layout(set = 0, binding = 0) uniform Uniforms {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat3 normalMatrix;
    vec3 cameraPosition;
    float metallic;
    float roughness;
    vec3 lightDirection;
    vec3 lightColor;
    vec3 ambientColor;
};

layout(set = 0, binding = 1) uniform sampler2D albedo;
layout(set = 0, binding = 2) uniform sampler2D normalMap;
layout(set = 0, binding = 3) uniform sampler2D metallicRoughness;

// Outputs
layout(location = 0) out vec4 fragColor;

// ... (rest of generated shader)
```

### C++ Header Output

```cpp
// Auto-generated by Lex Compiler v0.3.0
// Source: pbr_material.lex
// DO NOT EDIT MANUALLY

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace imperium::shaders {

struct PBRMaterialUniforms {
    glm::mat4 model{1.0f};
    glm::mat4 view{1.0f};
    glm::mat4 projection{1.0f};
    glm::mat3 normalMatrix{1.0f};
    glm::vec3 cameraPosition{0.0f};
    float metallic = 0.5f;
    float roughness = 0.5f;
    glm::vec3 lightDirection{0.0f, -1.0f, 0.0f};
    glm::vec3 lightColor{1.0f};
    glm::vec3 ambientColor{0.1f};
    
    // Texture handles (set separately)
    uint32_t albedo = 0;
    uint32_t normalMap = 0;
    uint32_t metallicRoughness = 0;
};

} // namespace imperium::shaders
```

### Aether Context Output

```markdown
# PBRMaterial Shader — Aether Context

## Overview
Physically-based rendering shader for Imperium engine.
Implements Cook-Torrance BRDF with GGX distribution.

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| metallic | float | 0.5 | Metalness factor (0=dielectric, 1=metal) |
| roughness | float | 0.5 | Surface roughness (0=smooth, 1=rough) |
| albedo | texture2D | - | Base color texture |
| normalMap | texture2D | - | Tangent-space normal map |

## Performance Notes

- **Low**: 1 sample, no shadows, no SSAO
- **Medium**: 4 samples, shadows enabled
- **High**: 16 samples, shadows + SSAO

## Integration

```cpp
#include "generated/pbr_material.hpp"

imperium::shaders::PBRMaterialUniforms uniforms;
uniforms.metallic = 0.8f;
uniforms.roughness = 0.3f;
```

## Generated Files

- `glsl/pbr_model.vert` - Vertex shader (GLSL 450)
- `glsl/pbr_model.frag` - Fragment shader (GLSL 450)
- `hlsl/pbr_model.vs` - Vertex shader (HLSL SM 5.0)
- `hlsl/pbr_model.ps` - Fragment shader (HLSL SM 5.0)
- `msl/pbr_model.metal` - Metal shader (macOS/iOS)
```

---

## Implementation Plan

### Phase 1: Core DSL (v0.3.0)

1. **Lexer Extensions**
   - New keywords: `shader`, `vertex`, `fragment`, `uniforms`, `inputs`, `outputs`, `variants`
   - New types: `vec2`, `vec3`, `vec4`, `mat3`, `mat4`, `texture2D`, `sampler`
   - Annotations: `@location`, `@binding`, `@set`

2. **AST Nodes**
   - `ShaderDefinition` - Top-level shader container
   - `ShaderStage` - Vertex/Fragment/Compute
   - `ShaderUniform` - Uniform buffer entries
   - `ShaderVariant` - Quality presets

3. **Backends**
   - `ShaderBackend` - Abstract interface
   - `GlslBackend` - GLSL 450 output
   - `HlslBackend` - HLSL Shader Model 5.0
   - `MslBackend` - Metal Shading Language
   - `CppHeaderBackend` - C++ structs
   - `AetherBackend` - Documentation

### Phase 2: Advanced Features (v0.4.0)

- Compute shaders
- Geometry shaders
- Tessellation shaders
- Ray tracing shaders (VK_KHR_ray_tracing)
- Include system (`#include` support)

### Phase 3: Tooling (v0.5.0)

- VS Code extension (syntax highlighting)
- Live preview integration
- Performance profiler hints
- Hot-reload integration

---

## Open Questions

1. **Include system**: How to handle shared shader code (e.g., noise functions)?
   - Option A: `#include "noise.glsl"` (passthrough)
   - Option B: `includes: [noise]` (Lex-managed)
   
2. **Custom functions**: Support user-defined functions?
   - Option A: Inline GLSL blocks
   - Option B: Lex function syntax

3. **SPIR-V**: Direct SPIR-V output or transpile from GLSL?
   - Option A: Use glslang for GLSL→SPIR-V
   - Option B: Direct SPIR-V generation (complex)

4. **Debugging**: Source maps for shader debugging?
   - Map Lex line numbers to generated GLSL lines

---

## Success Criteria

| Criterion | Metric |
|-----------|--------|
| Cross-compilation | Single `.lex` → GLSL + HLSL + MSL |
| Type safety | C++ struct matches shader uniforms exactly |
| Performance | Generated code as fast as hand-written |
| UX | < 1 second compile time for typical shader |
| Adoption | 3+ shaders converted from manual GLSL |

---

## Risks

| Risk | Mitigation |
|------|------------|
| GLSL/HLSL/MSL differences | Comprehensive test suite |
| Performance regression | Benchmark against hand-written |
| Learning curve | Interactive tutorial, examples |
| Tooling support | VS Code extension from day 1 |

---

## References

- [GLSL Specification](https://www.khronos.org/opengl/wiki/Core_Language_(GLSL))
- [HLSL Reference](https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl)
- [Metal Shading Language](https://developer.apple.com/metal/Metal-Shading-Language-Specification.pdf)
- [Cook-Torrance BRDF](https://blog.selfshadow.com/publications/s2017-shading-course/)

---

**Next Steps:**

1. Gather community feedback
2. Prototype syntax with 2-3 example shaders
3. Implement lexer extensions
4. Build GLSL backend (MVP)
5. Add HLSL/MSL backends
