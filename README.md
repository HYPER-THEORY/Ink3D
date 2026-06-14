Ink3D
========

[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)

Ink3D is a lightweight and easy to use framework for 3D rendering.

### Screenshots ###

![Ink3D - Black Cat.png](https://s2.loli.net/2023/03/28/znxmQqjsBlkL2M8.png "Black Cat - Rendered by Ink3D")

![Ink3D - Coffee Time.png](https://s2.loli.net/2023/03/28/1z5ipOKQlqru4Zg.png "Coffee Time - Rendered by Ink3D")

![Ink3D - Paper Plane.png](https://s2.loli.net/2023/02/01/HuALipG27Nal3r4.png "Paper Plane - Rendered by Ink3D")

### Features ###

- Forward / Deferred Rendering Pipeline

- Physically Based Rendering Materials

- Light & Fog System

- High Quality Shadows (Hard-Edge / PCF / PCSS)

- Probe Based Reflection

- Post-Processing (SSAO, SSR, Bloom, FXAA and more)

### Getting Started ###

Get the source with the **Download ZIP** button or use git to clone the repository.

```
git clone --depth=1 https://github.com/HYPER-THEORY/Ink3D.git
```

Run **Setup.bat** to configure SDL2 dependency and create Visual Studio 2022 project files. If you are using Visual Studio 2026, replace "Visual Studio 17 2022" in Setup.bat with "Visual Studio 18 2026".

### Usage ###

Use the following code to render a question mark block.

```CPP
#include "addons/Mainloop.h"
#include "addons/window/Viewer.h"

#define A "\0\0\0"
#define B "\xEE\xBB\x66"
#define C "\xFF\xDD\xBB"

using namespace ink;

const char* TEXTURE =
A B B B B B B B B B B A B B B B B A A A B B B B B B B B C C C A B B B B
B B B B C C C B B B B B B B B B B A A A B B B B B B B B C C C A A A B B
B B B B C C C C C A A B B B A A A B B C C C A B B C C C A A A C C C A B
B C C C C C C C C C B B B B C C C C C C C B B B A B B B B B B B B B B A;

Scene scene;
Renderer renderer;
Viewer viewer;

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
    std::copy_n(TEXTURE, 12 * 12 * 3, &image->data[0]);
    
    Material* material = new Material();
    material->color_map = image;
    scene.set_material("default", material);
    
    HemisphereLight* light = new HemisphereLight(Vec3(1), Vec3(0.5));
    light->direction = Vec3(0, 0, -1);
    scene.add_light(light);
    
    renderer.set_rendering_mode(FORWARD_RENDERING);
    renderer.set_texture_callback([](gpu::Texture& t) -> void {
        t.set_filters(TEXTURE_NEAREST, TEXTURE_NEAREST);
    });
    renderer.load_scene(scene);
    renderer.set_viewport(gpu::Rect(960, 540));
    
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

### Requirements ###

- OpenGL core 4.1 capable GPU

- 1 GB Graphics memory recommended
