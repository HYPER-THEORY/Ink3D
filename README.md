Ink 3D
========

[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)

Ink 3D is a lightweight and easy to use framework for 3D rendering.

### Screenshots ###

![Ink3D - Black Cat.png](https://s2.loli.net/2023/03/28/znxmQqjsBlkL2M8.png "Black Cat - Rendered by Ink3D")

![Ink3D - Coffee Time.png](https://s2.loli.net/2023/03/28/1z5ipOKQlqru4Zg.png "Coffee Time - Rendered by Ink3D")

![Ink3D - Paper Plane.png](https://s2.loli.net/2023/02/01/HuALipG27Nal3r4.png "Paper Plane - Rendered by Ink3D")

### Features ###

- Forward + Deferred Rendering Pipeline

- HDR Rendering Pipeline Supported

- Physically Based Rendering Materials

- Light System (Directional / Point / Spot / Hemisphere)

- High Quality Shadows (Hard-Edge / PCF / PCSS)

- Probes System (Reflection Probe)

- Global Fog (Linear Fog / Exponential Square Fog)

- Post-Processing (SSAO, SSR, Bloom, FXAA and more)

- Built-in Efficient Software Rasterization

### Getting Started ###

- A few lines of code to render a question block.

```CPP
#include "ink/utils/Mainloop.h"

#define A "\0\0\0"
#define B "\xEE\xBB\x66"
#define C "\xFF\xDD\xBB"

using namespace Ink;

const char* BLOCK =
A B B B B B B B B B B A B B B B B A A A B B B B B B B B C C C A B B B B
B B B B C C C B B B B B B B B B B A A A B B B B B B B B C C C A A A B B
B B B B C C C C C A A B B B A A A B B C C C A B B C C C A A A C C C A B
B C C C C C C C C C B B B B C C C C C C C B B B A B B B B B B B B B B A;

Scene scene;
Viewer viewer;
Renderer renderer;

void conf(Settings& t) {
    t.title = "Ink3D Example";
    t.show_cursor = false;
    t.lock_cursor = true;
    t.background_color = Vec3(1.0, 0.93, 0.8);
}

void load() {
    Instance* instance = new Instance();
    instance->mesh = new Mesh(BoxMesh::create());
    scene.add(instance);
    
    Image* image = new Image(12, 12, 3);
    std::copy_n(BLOCK, 12 * 12 * 3, &image->data[0]);
    
    Material* material = new Material();
    material->color_map = image;
    scene.set_material("default", material);
    
    HemisphereLight* light = new HemisphereLight(Vec3(1), Vec3(0.5));
    light->direction = Vec3(0, 0, -1);
    scene.add_light(light);
    
    renderer.set_rendering_mode(FORWARD_RENDERING);
    renderer.set_texture_callback([](Gpu::Texture& t) -> void {
        t.set_filters(TEXTURE_NEAREST, TEXTURE_NEAREST);
    });
    renderer.load_scene(scene);
    renderer.set_viewport(Gpu::Rect(960, 540));
    
    viewer = Viewer(new PerspCamera(75 * DEG_TO_RAD, 1.77, 0.05, 500));
    viewer.set_position(Vec3(0, 0, -2));
}

void update(float dt) {
    Renderer::update_scene(scene);
    viewer.update(dt);
    renderer.render(scene, *viewer.get_camera());
}

void quit() {}
```

### Dependencies ###

- [SDL2](https://libsdl.org) Simple Directmedia Layer

- [stb_image](https://github.com/nothings/stb) Image loading/decoding from file/memory

- [fmt](https://github.com/fmtlib/fmt) A modern formatting library

### Requirements ###

- OpenGL core 4.1 capable GPU

- 1 GB Graphics memory recommended
