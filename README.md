Ink 3D
========

[![License](http://img.shields.io/:license-mit-blue.svg)](http://doge.mit-license.org)

Ink 3D is a lightweight and easy to use framework for 3D rendering.

### Features ###

High Performance Rendering Pipeline

Forward Rendering / Deferred Rendering

Physically Based Rendering

Lighting / Shadows

Post Processing

### Getting Started ###

Render a scene with a few lines of code.

```
#include "utils/everything.h"
#include "utils/mainloop.h"
#include "utils/viewer.h"
#include "../demos/nightscene/nightscene.h"

scene the_scene;
renderer the_renderer;
viewer the_viewer;

void conf(config& t) {
	t.width = 960;
	t.height = 540;
	t.hide_cursor = true;
	t.lock_cursor = true;
}

void load() {
	nightscene::init();
	the_scene = nightscene::get_scene();
	the_renderer.preload(the_scene);
	the_viewer = {{M_PI * 0.4, 960.f / 540.f, .05, 1000}};
}

void update(float dt) {
	the_renderer.render(the_scene, the_viewer.update());
}

void quit() {}
```

### Dependencies ###

[SDL2](https://libsdl.org)

[SDL_image](https://www.libsdl.org/projects/SDL_image/)

### Requirements ###

OpenGL 4.1 capable GPU

1 GB Graphics Memory recommended
