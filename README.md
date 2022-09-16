Ink 3D
========

[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)

Ink 3D is a lightweight and easy to use framework for 3D rendering.

![Forest_Ink3D.png](https://s2.loli.net/2022/09/16/fml5pO9VYWQX1IS.png "Forest - Render by Ink3D")

![Satellite_Ink3D.png](https://s2.loli.net/2022/09/16/dJblkZyncmNR3oF.png "Satellite - Render by Ink3D")

![Game_Ink3D.png](https://s2.loli.net/2022/09/16/YVWnQLMasIb9jSP.png "Game - Render by Ink3D")

### Features ###

- Forward + Deferred Rendering Pipeline

- HDR Rendering Pipeline Supported

- Physically Based Rendering Materials

- Light System (Directional / Point / Spot / Ambient / Hemisphere)

- High Quality Shadows (Hard-Edge / PCF / PCSS)

- Probes System (Light Probe / Environment Probe)

- Global Fog (Linear / Exponential Square Fog)

- Post-Processing (SSAO, Bloom, FXAA, Tone Mapping and more)

### Getting Started ###

- Render a cube with a few lines of code.

```
#include "ink/utils/Everything.h"
#include "ink/utils/Mainloop.h"
#include "ink/utils/Viewer.h"

Ink::Mesh box;
Ink::Material material;

Ink::Scene scene;
Ink::Viewer viewer;
Ink::Renderer renderer;

void conf(Settings& t) {
	t.width = 960;
	t.height = 540;
	t.hide_cursor = true;
	t.lock_cursor = true;
}

void load() {
	box = Ink::BoxMesh::create();
	Ink::Instance* cube = Ink::Instance::create();
	cube->mesh = &box;
	scene.add(cube);
	
	material.color = Ink::Vec3(1.00, 0.77, 0.34);
	scene.set_material("default", &material);
	
	viewer = Ink::Viewer(Ink::PerspCamera(75 * Ink::DEG_TO_RAD, 1.77, 0.05, 1000));
	viewer.set_position(Ink::Vec3(-1, 0, 0));
	
	renderer.load_scene(scene);
	renderer.set_viewport(Ink::Gpu::Rect(960, 540));
}

void update(float dt) {
	viewer.update(dt);
	renderer.update_scene(scene);
	renderer.clear();
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
