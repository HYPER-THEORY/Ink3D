"""
Copyright (C) 2021-2022 Hypertheory

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import fbx
import os


def Convert(load, save, rotate=True):
    manager = fbx.FbxManager()
    scene = fbx.FbxScene.Create(manager, '')
    # Import scene
    importer = fbx.FbxImporter.Create(manager, '')
    importer.Initialize(load)
    importer.Import(scene)
    # Convert Z-up to Y-up
    if rotate:
        scene.GetRootNode().LclRotation.Set(fbx.FbxDouble3(-90, 0, 0))
    # Export scene
    exporter = fbx.FbxExporter.Create(manager, '')
    exporter.Initialize(save)
    exporter.Export(scene)


def ConvertDir(target, rotate=True):
    for root, dirs, files in os.walk(target):
        for f in files:
            loadpath = os.path.join(root, f)
            ext = os.path.splitext(loadpath)
            if ext[1].lower() == '.fbx':
                savepath = ext[0] + ".obj"
                Convert(loadpath, savepath, rotate)
                print('Converting: ' + savepath)
    print('Conversion done.')
