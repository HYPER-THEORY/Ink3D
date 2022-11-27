/**
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

#include "Loader.h"

#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "../../libs/stb/stb_image.h"

#include "../core/Error.h"

namespace Ink {

Image Loader::load_image(const std::string& p) {
	/* create a new image */
	Image image = Image(0, 0, 0, 1);
	
	/* get image data from file */
	auto* image_ptr = stbi_load(p.c_str(), &image.width, &image.height, &image.channel, 0);
	if (image_ptr == nullptr) {
		Error::set("Loader: Error reading from image");
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
		Error::set("Loader: Error reading from image");
		return Image();
	}
	
	/* copy image data to new image */
	image.data.resize(image.width * image.height * image.channel * image.bytes);
	memcpy(image.data.data(), image_ptr, image.data.size());
	
	/* release image data */
	stbi_image_free(image_ptr);
	return image;
}

std::vector<Mesh> Loader::load_obj(const std::string& p, const std::string& g) {
	/* prepare the file stream */
	std::ifstream stream;
	stream.open(p, std::ifstream::in);
	if (!stream) {
		Error::set("Loader: Error reading from obj file");
		return std::vector<Mesh>();
	}
	size_t stream_max = std::numeric_limits<std::streamsize>::max();
	
	/* temporary data */
	std::vector<Vec3> vertex;
	std::vector<Vec3> normal;
	std::vector<Vec2> uv;
	
	/* initialize mesh vector */
	std::vector<Mesh> meshes;
	meshes.emplace_back(Mesh("default"));
	Mesh* current_mesh = &meshes.back();
	
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
				stream.get();
				
				/* check whether uv is omitted */
				if (std::isdigit(stream.peek())) {
					stream >> index;
					current_mesh->uv.emplace_back(uv[index - 1]);
				}
				
				stream.get();
				stream >> index;
				current_mesh->normal.emplace_back(normal[index - 1]);
			}
			
			/* increase the length of current_material */
			current_group->length += 3;
			total_length += 3;
		}
		
		/* create new mesh object and initialize everything */
		else if (keyword == g) {
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
			meshes.emplace_back(Mesh(name));
			current_mesh = &meshes.back();
			
			/* create new mesh group */
			current_mesh->groups.emplace_back<MeshGroup>({name, total_length, 0});
			current_group = &current_mesh->groups.back();
		}
		
		/* use material */
		else if (keyword == "usemtl") {
			std::string name;
			stream >> name;
			
			/* if current mesh group has no data, replace its name */
			if (current_group->length == 0) {
				current_group->name = name;
				continue;
			}
			
			/* create new mesh group */
			current_mesh->groups.emplace_back<MeshGroup>({name, total_length, 0});
			current_group = &current_mesh->groups.back();
		}
		
		/* ignore */
		else {
			stream.ignore(stream_max, '\n');
		}
	}
	
	/* close file stream */
	stream.close();
	
	/* return the number of meshes */
	return meshes;
}

std::vector<Material> Loader::load_mtl(const std::string& p) {
	/* prepare the file stream */
	std::ifstream stream;
	stream.open(p, std::ifstream::in);
	if (!stream) {
		Error::set("Loader: Error reading from mtl file");
		return std::vector<Material>();
	}
	size_t stream_max = std::numeric_limits<std::streamsize>::max();
	
	/* initialize material pointer */
	std::vector<Material> materials;
	Material* current_material = nullptr;
	
	/* read data by line */
	while (!stream.eof()) {
		std::string keyword;
		stream >> keyword;
		
		/* create a new material */
		if (keyword == "newmtl") {
			std::string name;
			stream >> name;
			materials.emplace_back(Material(name));
			current_material = &materials.back();
		}
		
		/* material is not declared */
		else if (current_material == nullptr) {}
		
		/* diffuse color */
		else if (keyword == "Kd") {
			Vec3 diffuse;
			stream >> diffuse.x >> diffuse.y >> diffuse.z;
			current_material->color = diffuse;
		}
		
		/* emissive color */
		else if (keyword == "Ke") {
			Vec3 emissive;
			stream >> emissive.x >> emissive.y >> emissive.z;
			current_material->emissive = emissive;
		}
		
		/* dissolve factor */
		else if (keyword == "d") {
			float dissolve;
			stream >> dissolve;
			current_material->alpha = dissolve;
		}
		
		/* transparency factor */
		else if (keyword == "tr") {
			float transparency;
			stream >> transparency;
			current_material->alpha = 1 - transparency;
		}
		
		/* not used in PBR material */
		
		/* ambient color */
		/* else if (keyword == "Ka") {} */
		
		/* specular color */
		/* else if (keyword == "Ks") {} */
		
		/* transmission color */
		/* else if (keyword == "Tf") {} */
		
		/* specular exponent */
		/* else if (keyword == "Ns") {} */
		
		/* optical density */
		/* else if (keyword == "Ni") {} */
		
		/* sharpness value */
		/* else if (keyword == "sharpness") {} */
		
		/* ignore */
		else {
			stream.ignore(stream_max, '\n');
		}
	}
	
	/* close file stream */
	stream.close();
	
	/* return the number of materials */
	return materials;
}

}
