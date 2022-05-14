/*
 * Copyright (C) 2021-2022 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "loader.h"

void loader::init() {
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
}

int loader::load_image(const std::string& f, image& i, bool s) {
	SDL_Surface* surface = IMG_Load(f.c_str());
	if (!surface) return set_error("[Loader Error] Error reading from image");
	i.width = surface->w;
	i.height = surface->h;
	i.bytes = surface->format->BytesPerPixel;
	i.data.resize(i.width * i.height * i.bytes);
	SDL_memcpy(i.data.data(), surface->pixels, i.data.size());
	SDL_FreeSurface(surface);
	if (s) i.swap_rb();
	return 0;
}

int loader::load_obj(const std::string& f, mesh* m) {
	std::ifstream in;
	in.open(f, std::ifstream::in);
	if (!in) return set_error("[Loader Error] Error reading from obj file");
	/* temporary data */
	std::vector<vec3> vertex;
	std::vector<vec3> normal;
	std::vector<vec2> uv;
	/* initialize mesh object */
	mesh* current_mesh = m;
	*current_mesh = mesh();
	current_mesh->name = "default";
	/* initialize material */
	meshgroup* current_material = nullptr;
	/* initialize group */
	current_mesh->groups.push_back({"default", 0, 0});
	meshgroup* current_group = &current_mesh->groups.back();
	/* initialize total length */
	int total_length = 0;
	/* read data by line */
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream stream(line.c_str());
		std::string keyword;
		stream >> keyword;
		if (keyword == "v") {
			/* add vertex to temporary array */
			vec3 v;
			stream >> v.x >> v.y >> v.z;
			vertex.push_back(v);
		} else if (keyword == "vn") {
			/* add normal to temporary array */
			vec3 vn;
			stream >> vn.x >> vn.y >> vn.z;
			normal.push_back(vn);
		} else if (keyword == "vt") {
			/* add uv to temporary array */
			vec2 vt;
			stream >> vt.x >> vt.y;
			uv.push_back(vt);
		} else if (keyword == "f") {
			/*
			 * search for data by index and add it to current_mesh
			 * increase the length of current_material, current_group
			 */
			int index;
			for (int i = 0; i < 3; ++i) {
				stream >> index;
				current_mesh->vertex.push_back(vertex[index - 1]);
				stream.get();
				/* whether uv is omitted */
				if (std::isdigit(stream.peek())) {
					stream >> index;
					current_mesh->uv.push_back(uv[index - 1]);
				}
				stream.get();
				stream >> index;
				current_mesh->normal.push_back(normal[index - 1]);
			}
			if (current_material) {
				current_material->length += 3;
			} else {
				set_error("[Loader Error] Material is not assigned");
			}
			current_group->length += 3;
			total_length += 3;
		} else if (keyword == "g") {
			/* add new group */
			std::string name;
			stream >> name;
			/* if default group has no data, just replace its name */
			if (current_group->name == "default" && current_group->length == 0) {
				current_group->name = name;
			} else {
				/* new group */
				current_mesh->groups.push_back({name, total_length, 0});
				current_group = &current_mesh->groups.back();
			}
		} else if (keyword == "o") {
			/* create new mesh object and initialize everything */
			std::string name;
			stream >> name;
			/* if default mesh data has no data, just replace its name */
			if (current_mesh->name == "default" && vertex.empty()) {
				current_mesh->name = name;
			} else {
				/* new mesh object */
				*(++current_mesh) = mesh();
				current_mesh->name = name;
				/* initialize total length */
				total_length = 0;
				/* initialize group */
				current_mesh->groups.push_back({"default", 0, 0});
				current_group = &current_mesh->groups.back();
				/* initialize material */
				current_material = nullptr;
			}
		} else if (keyword == "usemtl") {
			/* use material */
			std::string name;
			stream >> name;
			/* new material reference */
			current_mesh->material_groups.push_back({name, total_length, 0});
			current_material = &current_mesh->material_groups.back();
			/* update material library */
			if (current_mesh->material_library.count(name) == 0) {
				current_mesh->material_library.insert({name, nullptr});
			}
		}
	}
	in.close();
	return static_cast<int>(current_mesh - m + 1);
}

int loader::load_mtl(const std::string& f, simplemat* m) {
	std::ifstream in;
	in.open(f, std::ifstream::in);
	if (!in) return set_error("[Loader Error] Error reading from mtl file");
	std::string line;
	simplemat* current_material = m - 1;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream stream(line.c_str());
		std::string keyword;
		stream >> keyword;
		if (keyword == "newmtl") {
			*(++current_material) = simplemat();
			stream >> current_material->name;
		} else if (current_material < m) {
			/* material is not declared */
		} else if (keyword == "Kd") {
			vec3& color = current_material->color;
			stream >> color.x >> color.y >> color.z;
		}
	}
	in.close();
	return static_cast<int>(current_material - m + 1);
}
