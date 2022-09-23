Ink 3D
========

[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)

Ink 3D is a lightweight and easy to use framework for 3D rendering.

![Forest_Ink3D.png](https://s2.loli.net/2022/09/16/HWl1xMqmyFhAvNp.png "Forest - Render by Ink3D")

![Satellite_Ink3D.png](https://s2.loli.net/2022/09/16/dJblkZyncmNR3oF.png "Satellite - Render by Ink3D")

![Platformer_Ink3D.png](https://s2.loli.net/2022/09/19/eO61ZPmyVGjtCSA.png "Platformer Game - Render by Ink3D")

### Features ###

- Forward + Deferred Rendering Pipeline

- HDR Rendering Pipeline Supported

- Physically Based Rendering Materials

- Light System (Directional / Point / Spot / Ambient / Hemisphere)

- High Quality Shadows (Hard-Edge / PCF / PCSS)

- Probes System (Light Probe / Environment Probe)

- Global Fog (Linear / Exponential Square Fog)

- Post-Processing (SSAO, Bloom, FXAA, Tone Mapping and more)

- Built-in Efficient Software Rasterization

### Getting Started ###

- Just a few lines of code to render a Question Block.

```
#include "ink/utils/Everything.h"
#include "ink/utils/Mainloop.h"
#include "ink/utils/Viewer.h"

#define A "\0\0\0"
#define B "\xEE\xBB\x66"
#define C "\xFF\xDD\xBB"

const char* block =
A B B B B B B B B B B A B B B B B A A A B B B B B B B B C C C A B B B B
B B B B C C C B B B B B B B B B B A A A B B B B B B B B C C C A A A B B
B B B B C C C C C A A B B B A A A B B C C C A B B C C C A A A C C C A B
B C C C C C C C C C B B B B C C C C C C C B B B A B B B B B B B B B B A;

Ink::Scene scene;
Ink::Viewer viewer;
Ink::Renderer renderer;

void conf(Settings& t) {
	t.title = "Ink3D Example";
	t.width = 960;
	t.height = 540;
	t.hide_cursor = true;
	t.lock_cursor = true;
	t.background_color = Ink::Vec3(1, 0.93, 0.8);
}

void load() {
	Ink::Instance* cube = Ink::Instance::create();
	cube->mesh = new Ink::Mesh(Ink::BoxMesh::create());
	scene.add(cube);
	
	Ink::Image* image = new Ink::Image(12, 12, 3);
	std::copy_n(block, 432, image->data.data());
	
	Ink::Material* material = new Ink::Material();
	material->color_map = image;
	scene.set_material("default", material);
	
	viewer = Ink::Viewer(Ink::PerspCamera(75 * Ink::DEG_TO_RAD, 1.77, 0.05, 1000));
	viewer.set_position(Ink::Vec3(0, 0, -2));
	
	renderer.set_texture_callback([](Ink::Gpu::Texture& t) -> void {
		t.set_filters(Ink::TEXTURE_NEAREST, Ink::TEXTURE_NEAREST);
	});
	
	renderer.load_scene(scene);
	renderer.set_viewport(Ink::Gpu::Rect(960, 540));
}

void update(float dt) {
	viewer.update(dt);
	renderer.update_scene(scene);
	renderer.render(scene, viewer.get_camera());
}

void quit() {}
```

### Dependencies ###

- [SDL2](https://libsdl.org) Simple Directmedia Layer

- [Glad](https://glad.dav1d.de) Multi-Language Loader-Generator

- [Stb_image](https://github.com/nothings/stb) Image loading/decoding from file/memory

### Requirements ###

- OpenGL core 4.1 capable GPU

- 1 GB Graphics memory recommended
