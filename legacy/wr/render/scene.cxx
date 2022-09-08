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

#include "scene.h"

scene::scene(const std::initializer_list<instance>& i) : instances(i) {}

size_t scene::size() const {
	return instances.size();
}

void scene::clear() {
	instances.clear();
}

void scene::add_instance(const instance& i) {
	instances.push_back(i);
}

void scene::add_instances(const std::initializer_list<instance>& i) {
	instances.insert(instances.end(), i);
}

instance* scene::get_instances() {
	return instances.data();
}

const instance* scene::get_instances() const {
	return instances.data();
}

instance& scene::get_instance(int i) {
	return instances[i];
}

const instance& scene::get_instance(int i) const {
	return instances[i];
}
