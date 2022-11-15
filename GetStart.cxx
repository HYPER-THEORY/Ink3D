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
    t.show_cursor = false;
    t.lock_cursor = true;
    t.background_color = Ink::Vec3(1, 0.93, 0.8);
}

void load() {
    Ink::Instance* cube = Ink::Instance::create();
    cube->mesh = new Ink::Mesh(Ink::BoxMesh::create());
    scene.add(cube);
    
    Ink::Image* image = new Ink::Image(12, 12, 3);
    std::copy_n(block, 12 * 12 * 3, &image->data[0]);
    
    Ink::Material* material = new Ink::Material();
    material->color_map = image;
    scene.set_material("default", material);
	
	Ink::HemisphereLight* light = new Ink::HemisphereLight();
	light->ground_color = Ink::Vec3(0.5, 0.5, 0.5);
	light->direction = Ink::Vec3(0, 0, -1);
	scene.add_light(light);
    
    viewer = Ink::Viewer(Ink::PerspCamera(75 * Ink::DEG_TO_RAD, 1.77, 0.05, 1000));
    viewer.set_position(Ink::Vec3(0, 0, -2));
    
	renderer.set_rendering_mode(Ink::FORWARD_RENDERING);
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
