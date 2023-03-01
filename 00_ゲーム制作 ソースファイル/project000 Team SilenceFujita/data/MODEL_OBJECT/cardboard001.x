xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 114;
 -40.00000;0.00000;-30.00000;,
 40.00000;0.00000;-30.00000;,
 40.00000;0.00000;30.00000;,
 -40.00000;0.00000;30.00000;,
 -40.00000;32.00000;-30.00000;,
 40.00000;32.00000;-30.00000;,
 40.00000;0.00000;-30.00000;,
 -40.00000;0.00000;-30.00000;,
 40.00000;32.00000;-30.00000;,
 40.00000;32.00000;30.00000;,
 40.00000;0.00000;-30.00000;,
 40.00000;32.00000;30.00000;,
 -40.00000;32.00000;30.00000;,
 -40.00000;0.00000;30.00000;,
 40.00000;0.00000;30.00000;,
 -40.00000;32.00000;30.00000;,
 -40.00000;32.00000;-30.00000;,
 -40.00000;0.00000;-30.00000;,
 38.00000;32.00000;-28.00000;,
 -38.00000;32.00000;-28.00000;,
 -38.00000;30.00000;-28.00000;,
 38.00000;30.00000;-28.00000;,
 38.00000;32.00000;28.00000;,
 38.00000;32.00000;-28.00000;,
 38.00000;30.00000;-28.00000;,
 38.00000;30.00000;28.00000;,
 -38.00000;32.00000;28.00000;,
 38.00000;32.00000;28.00000;,
 38.00000;30.00000;28.00000;,
 -38.00000;30.00000;28.00000;,
 -38.00000;32.00000;-28.00000;,
 -38.00000;32.00000;28.00000;,
 -38.00000;30.00000;28.00000;,
 -38.00000;30.00000;-28.00000;,
 40.00000;32.00000;-30.00000;,
 70.00000;10.00000;-30.00000;,
 68.58578;8.58579;-30.00000;,
 40.00000;30.00000;-30.00000;,
 70.00000;10.00000;-30.00000;,
 69.99691;10.00309;30.00000;,
 68.58270;8.58887;30.00000;,
 68.58578;8.58579;-30.00000;,
 69.99691;10.00309;30.00000;,
 39.99691;32.00309;30.00000;,
 40.00000;30.00000;30.00000;,
 68.58270;8.58887;30.00000;,
 39.99691;32.00309;30.00000;,
 40.00000;32.00000;-30.00000;,
 40.00000;30.00000;-30.00000;,
 40.00000;30.00000;30.00000;,
 70.00000;10.00000;-30.00000;,
 40.00000;32.00000;-30.00000;,
 40.00000;30.00000;-30.00000;,
 68.58578;8.58579;-30.00000;,
 -40.00000;32.00000;30.00000;,
 -70.00000;10.00000;30.00000;,
 -68.58578;8.58579;30.00000;,
 -40.00000;30.00000;30.00000;,
 -70.00000;10.00000;30.00000;,
 -69.99692;10.00309;-30.00000;,
 -68.58270;8.58887;-30.00000;,
 -68.58578;8.58579;30.00000;,
 -69.99692;10.00309;-30.00000;,
 -39.99691;32.00309;-30.00000;,
 -40.00000;30.00000;-30.00000;,
 -68.58270;8.58887;-30.00000;,
 -39.99691;32.00309;-30.00000;,
 -40.00000;32.00000;30.00000;,
 -40.00000;30.00000;30.00000;,
 -40.00000;30.00000;-30.00000;,
 -70.00000;10.00000;30.00000;,
 -40.00000;32.00000;30.00000;,
 -40.00000;30.00000;30.00000;,
 -68.58578;8.58579;30.00000;,
 40.00000;32.00000;30.00000;,
 40.00000;10.00000;60.00000;,
 40.00000;8.58579;58.58578;,
 40.00000;30.00000;30.00000;,
 40.00000;10.00000;60.00000;,
 -40.00000;10.00000;60.00000;,
 -40.00000;8.58579;58.58578;,
 40.00000;8.58579;58.58578;,
 -40.00000;10.00000;60.00000;,
 -40.00001;32.00001;29.99999;,
 -40.00001;29.99691;30.00308;,
 -40.00000;8.58579;58.58578;,
 -40.00001;32.00001;29.99999;,
 40.00000;32.00000;30.00000;,
 40.00000;30.00000;30.00000;,
 -40.00001;29.99691;30.00308;,
 40.00000;10.00000;60.00000;,
 40.00000;32.00000;30.00000;,
 40.00000;30.00000;30.00000;,
 40.00000;8.58579;58.58578;,
 -40.00000;32.00000;-30.00000;,
 -40.00001;10.00000;-60.00000;,
 -40.00001;8.58579;-58.58578;,
 -40.00000;30.00000;-30.00000;,
 -40.00001;10.00000;-60.00000;,
 40.00000;10.00000;-60.00002;,
 40.00000;8.58579;-58.58580;,
 -40.00001;8.58579;-58.58578;,
 40.00000;10.00000;-60.00002;,
 40.00001;32.00001;-30.00000;,
 40.00001;29.99691;-30.00309;,
 40.00000;8.58579;-58.58580;,
 40.00001;32.00001;-30.00000;,
 -40.00000;32.00000;-30.00000;,
 -40.00000;30.00000;-30.00000;,
 40.00001;29.99691;-30.00309;,
 -40.00001;10.00000;-60.00000;,
 -40.00000;32.00000;-30.00000;,
 -40.00000;30.00000;-30.00000;,
 -40.00001;8.58579;-58.58578;;
 
 50;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,2,10;,
 4;11,12,13,14;,
 4;15,16,17,3;,
 4;18,19,20,21;,
 4;5,4,19,18;,
 4;22,23,24,25;,
 4;9,8,23,22;,
 4;26,27,28,29;,
 4;12,11,27,26;,
 4;30,31,32,33;,
 4;16,15,31,30;,
 4;3,2,1,0;,
 4;7,6,5,4;,
 4;10,2,9,8;,
 4;14,13,12,11;,
 4;3,17,16,15;,
 4;21,20,19,18;,
 4;18,19,4,5;,
 4;25,24,23,22;,
 4;22,23,8,9;,
 4;29,28,27,26;,
 4;26,27,11,12;,
 4;33,32,31,30;,
 4;30,31,15,16;,
 4;34,35,36,37;,
 4;38,39,40,41;,
 4;42,43,44,45;,
 4;46,47,48,49;,
 4;46,39,50,51;,
 4;52,53,40,49;,
 4;54,55,56,57;,
 4;58,59,60,61;,
 4;62,63,64,65;,
 4;66,67,68,69;,
 4;66,59,70,71;,
 4;72,73,60,69;,
 4;74,75,76,77;,
 4;78,79,80,81;,
 4;82,83,84,85;,
 4;86,87,88,89;,
 4;86,79,90,91;,
 4;92,93,80,89;,
 4;94,95,96,97;,
 4;98,99,100,101;,
 4;102,103,104,105;,
 4;106,107,108,109;,
 4;106,99,110,111;,
 4;112,113,100,109;;
 
 MeshMaterialList {
  1;
  50;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\cardboard001.png";
   }
  }
 }
 MeshNormals {
  34;
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;-0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.707106;-0.707107;0.000073;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  -0.707106;-0.707107;-0.000073;,
  -0.000000;-0.000000;-1.000000;,
  1.000000;0.000000;-0.000000;,
  0.000000;-0.707107;0.707106;,
  -1.000000;-0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.000000;-0.707107;-0.707106;,
  1.000000;-0.000000;-0.000000;,
  -1.000000;-0.000772;-0.000026;,
  -0.894127;-0.447814;-0.000012;,
  0.591364;0.806405;-0.000011;,
  -0.599543;-0.800343;0.000005;,
  1.000000;-0.000771;0.000026;,
  0.894127;-0.447814;0.000011;,
  -0.591364;0.806405;0.000011;,
  0.599543;-0.800343;-0.000005;,
  -0.000019;-0.000771;-1.000000;,
  -0.000009;-0.447814;-0.894127;,
  0.000000;0.806405;0.591364;,
  0.000004;-0.800343;-0.599543;,
  0.000019;-0.000771;1.000000;,
  0.000009;-0.447814;0.894127;,
  -0.000000;0.806405;-0.591364;,
  -0.000004;-0.800343;0.599543;;
  50;
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;2,2,2,2;,
  4;5,5,5,5;,
  4;3,3,3,3;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;6,6,6,6;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;18,18,19,19;,
  4;20,20,20,20;,
  4;19,21,21,19;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;22,22,23,23;,
  4;24,24,24,24;,
  4;23,25,25,23;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;26,26,27,27;,
  4;28,28,28,28;,
  4;27,29,29,27;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;30,30,31,31;,
  4;32,32,32,32;,
  4;31,33,33,31;;
 }
 MeshTextureCoords {
  114;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
