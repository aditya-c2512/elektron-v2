# Elektron Engine

## INTRODUCTION
Elektron Engine is my toy rendering engine that I have been developing in order to learn advanced computer graphics techniques. Based on DirectX11(for now), Elektron V2 is being developed with efficiency and organisation in mind. Elektron V2 has the following functionalities:
- Centre-pivoted Camera
- Dark mode GUI
- GGX-based diffuse and specular shading
- HDRI Sky Sphere
- Supports Wavefront OBJ meshes and materials(.mtl)
- Supports glTF meshes
- Efficient texture loading system(WIP)

## INSTALLATION
> NOTE : Elektron Engine is currently Windows only
### Dependencies
- Open Asset Importer(Assimp)
- DirectXTex
- Dear ImGui

### Getting the Source Code
### Building the Source Code

## Current WIP
- [x] Camera system
- [x] Dear ImGui Integration
- [x] Lights and Shading
- [x] Model Scene Graph
- [x] Material Loading
- [x] Dear ImGUI Docking => Update Dear ImGUI version
- [x] HDRI Skybox
- [x] Shift to GGX Shading
- [x] Shift Texture Loading to DirectXTex
- [x] ElekTexMap: Hash-map based approach to storing and loading texture instances.
- In-Engine CPU Profiler

## FUTURE WORK
- [ ] PBR Materials
- [ ] Implement Cubemap Reflections
- [ ] Optimise Texture Loading System
- [ ] File browser GUI to select and load mesh
- [ ] Multiple Lights Shading and UI support
- [ ] Shadow Mapping
- [ ] Voxel Cone Tracing Global Illumination (VXGI)

## Acknowledgments
- Dear ImGui: [Omar Cornut](https://github.com/ocornut)
- [Assimp](https://github.com/assimp/assimp)
- Dear ImGui Dark Mode: [Jan Bielak](https://github.com/ocornut/imgui/issues/707#issuecomment-917151020)