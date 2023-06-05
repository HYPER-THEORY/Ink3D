/**
 * Copyright (C) 2021-2023 Hypertheory
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

#include "Loader.h"

#include "../core/Error.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <fstream>
#include <sstream>

namespace Ink {

Image Loader::load_image(const std::string& p) {
	/* create a new image */
	Image image = Image(0, 0, 0, 1);
	
	/* get image data from file */
	auto* image_ptr = stbi_load(p.c_str(), &image.width, &image.height, &image.channel, 0);
	if (image_ptr == nullptr) {
		Error::set("Loader", "Failed to read from image");
		return Image();
	}
	
	/* copy image data to new image */
	image.data.resize(image.width * image.height * image.channel * image.bytes);
	memcpy(image.data.data(), image_ptr, image.data.size());
	
	/* release image data */
	stbi_image_free(image_ptr);
	return image;
}

Image Loader::load_image_hdr(const std::string& p) {
	/* create a new image */
	Image image = Image(0, 0, 0, 4);
	
	/* get image data from file */
	auto* image_ptr = stbi_loadf(p.c_str(), &image.width, &image.height, &image.channel, 0);
	if (image_ptr == nullptr) {
		Error::set("Loader", "Failed to read from image");
		return Image();
	}
	
	/* copy image data to new image */
	image.data.resize(image.width * image.height * image.channel * image.bytes);
	memcpy(image.data.data(), image_ptr, image.data.size());
	
	/* release image data */
	stbi_image_free(image_ptr);
	return image;
}

LoadObject Loader::load_mtl(const std::string& p) {
	/* prepare the file stream */
	std::ifstream stream;
	stream.open(p, std::ifstream::in);
	if (stream.fail()) {
		Error::set("Loader", "Failed to read from mtl file");
		return LoadObject();
	}
	size_t streamsize_max = std::numeric_limits<std::streamsize>::max();
	
	/* initialize load object */
	LoadObject object;
	Material* current_material = nullptr;
	
	/* read data by line */
	while (!stream.eof()) {
		std::string keyword;
		stream >> keyword;
		
		/* create a new material */
		if (keyword == "newmtl") {
			std::string name;
			stream >> name;
			current_material = &object.materials.emplace_back(Material(name));
		}
		
		/* material is not declared */
		else if (current_material == nullptr) {}
		
		/* Kd: diffuse color */
		else if (keyword == "Kd") {
			Vec3 kd;
			stream >> kd.x >> kd.y >> kd.z;
			current_material->color = kd;
		}
		
		/* Ke: emissive color */
		else if (keyword == "Ke") {
			Vec3 ke;
			stream >> ke.x >> ke.y >> ke.z;
			current_material->emissive = ke;
		}
		
		/* d: dissolve factor */
		else if (keyword == "d") {
			float d;
			stream >> d;
			current_material->alpha = d;
		}
		
		/* tr: transparency factor */
		else if (keyword == "tr") {
			float tr;
			stream >> tr;
			current_material->alpha = 1 - tr;
		}
		
		/* ignore unknown keyword */
		else {
			stream.ignore(streamsize_max, '\n');
		}
	}
	
	/* close file stream */
	stream.close();
	
	/* return the load object */
	return object;
}

LoadObject Loader::load_obj(const std::string& p, const ObjOptions& o) {
	/* prepare the file stream */
	std::ifstream stream;
	stream.open(p, std::ifstream::in);
	if (stream.fail()) {
		Error::set("Loader", "Failed to read from obj file");
		return LoadObject();
	}
	size_t streamsize_max = std::numeric_limits<std::streamsize>::max();
	
	/* temporary data */
	std::vector<Vec3> vertex;
	std::vector<Vec3> normal;
	std::vector<Vec2> uv;
	std::vector<Vec3> color;
	
	/* initialize load object */
	LoadObject object;
	Mesh* current_mesh = &object.meshes.emplace_back(Mesh("default"));
	
	/* initialize mesh group pointer */
	current_mesh->groups.emplace_back<MeshGroup>({"default", 0, 0});
	MeshGroup* current_group = &current_mesh->groups.back();
	
	/* initialize total length */
	int total_length = 0;
	
	/* read data by line */
	while (!stream.eof()) {
		std::string keyword;
		stream >> keyword;
		
		/* add vertex to temporary array */
		if (keyword == "v") {
			Vec3 v;
			stream >> v.x >> v.y >> v.z;
			vertex.emplace_back(v);
			if (o.vertex_color) {
				stream >> v.x >> v.y >> v.z;
				color.emplace_back(v);
			}
		}
		
		/* add normal to temporary array */
		else if (keyword == "vn") {
			Vec3 vn;
			stream >> vn.x >> vn.y >> vn.z;
			normal.emplace_back(vn);
		}
		
		/* add uv to temporary array */
		else if (keyword == "vt") {
			Vec2 vt;
			stream >> vt.x >> vt.y;
			uv.emplace_back(vt);
		}
		
		/* search for data by index and add it to current mesh */
		else if (keyword == "f") {
			int index = 0;
			for (int i = 0; i < 3; ++i) {
				stream >> index;
				current_mesh->vertex.emplace_back(vertex[index - 1]);
				if (o.vertex_color) {
					current_mesh->color.emplace_back(color[index - 1]);
				}
				
				/* check whether uv is omitted */
				if (stream.peek() != '/') continue;
				stream.get();
				if (std::isdigit(stream.peek())) {
					stream >> index;
					current_mesh->uv.emplace_back(uv[index - 1]);
				}
				
				/* check whether normal is omitted */
				if (stream.peek() != '/') continue;
				stream.get();
				stream >> index;
				current_mesh->normal.emplace_back(normal[index - 1]);
			}
			
			/* increase the length of current_material */
			current_group->length += 3;
			total_length += 3;
		}
		
		/* create new mesh object and initialize everything */
		else if (keyword == o.group) {
			std::string name;
			stream >> name;
			
			/* if current mesh has no data, replace its name */
			if (current_mesh->vertex.empty()) {
				current_mesh->name = name;
				continue;
			}
			
			/* initialize total length */
			total_length = 0;
			
			/* create new mesh object */
			current_mesh = &object.meshes.emplace_back(Mesh(name));
			
			/* create new mesh group */
			current_group = &current_mesh->groups.emplace_back(MeshGroup{name, total_length, 0});
		}
		
		/* create new mesh group */
		else if (keyword == "usemtl") {
			std::string name;
			stream >> name;
			
			/* if current mesh group has no data, replace its name */
			if (current_group->length == 0) {
				current_group->name = name;
				continue;
			}
			
			/* create new mesh group */
			current_group = &current_mesh->groups.emplace_back(MeshGroup{name, total_length, 0});
		}
		
		/* ignore unknown keyword */
		else {
			stream.ignore(streamsize_max, '\n');
		}
	}
	
	/* close file stream */
	stream.close();
	
	/* return the load object */
	return object;
}

}
