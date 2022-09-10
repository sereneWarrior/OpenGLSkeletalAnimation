# OpenGLKeyframeAnimation

## Loading the bones from the input file

The [assimp](https://assimp-docs.readthedocs.io/en/v5.1.0/) library is used for loading and extracting all the data of an input file (I use a *.dae* file).

A mesh is loaded using a BasicMesh's  `LoadMesh` function.

Using the assimp Importer the data is loaded into an `aiScene` object. Since *file* can contain multiple objects each will be loaded into a single `BasicMeshEntry` object. These objects are stored in a vector field `m_Meshes` of BasicMesh.

`BasicMashEntry` data is set in  `SetMeshData` method. If a mesh has **bones** the data for the **bone weights** are read and stored into a vector of `Bone` objects (which are a property of `BasicMeshEntry`).

## Curent State
Snippet of the current output
```
 Bone 0: 'Armature_Bone' num vertices affected by this bone: 66
 0: vertex id 9 weight 0.95
 1: vertex id 10 weight 0.95
 2: vertex id 11 weight 0.94
 3: vertex id 18 weight 0.85
 4: vertex id 19 weight 0.95
 5: vertex id 20 weight 0.94
 6: vertex id 21 weight 0.14
 7: vertex id 22 weight 0.85
 8: vertex id 18 weight 0.85
 9: vertex id 24 weight 0.14
 10: vertex id 21 weight 0.14
...
 65: vertex id 48 weight 0.14
```
Input file in Blender:
![alt text](images/testObj.jpg)

## Setting up OpenGL & assimp
See:
[Installing Assimp](https://www.youtube.com/watch?v=oci7xJEg6sU&list=PLysLvOneEETPlOI_PI4mJnocqIpr2cSHS&index=19&t=363s),
[SetUp OpenGL](https://www.youtube.com/watch?v=bQ3CPgw4lxY&list=PLysLvOneEETPlOI_PI4mJnocqIpr2cSHS&index=2)