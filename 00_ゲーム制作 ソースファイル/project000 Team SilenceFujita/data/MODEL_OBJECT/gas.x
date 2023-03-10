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
 140;
 -290.60248;170.81166;-244.03673;,
 231.05174;170.81166;-244.03673;,
 231.05174;152.91762;-244.03673;,
 -290.60248;152.91762;-244.03673;,
 231.05174;170.81166;-244.03673;,
 250.23602;170.81166;141.34401;,
 230.55229;152.91762;90.01198;,
 231.05174;152.91762;-244.03673;,
 250.23602;170.81166;141.34401;,
 -290.10306;170.81166;90.01198;,
 -290.10306;152.91762;90.01198;,
 230.55229;152.91762;90.01198;,
 -290.10306;170.81166;90.01198;,
 -290.60248;170.81166;-244.03673;,
 -290.60248;152.91762;-244.03673;,
 -290.10306;152.91762;90.01198;,
 231.05174;170.81166;-244.03673;,
 -290.60248;170.81166;-244.03673;,
 -290.60248;152.91762;-244.03673;,
 231.05174;152.91762;-244.03673;,
 -40.16021;156.25749;-105.85998;,
 -19.39051;156.25749;-105.85998;,
 -19.39051;-0.39804;-105.85998;,
 -40.16021;-0.39804;-105.85998;,
 -19.39051;156.25749;-105.85998;,
 -19.88991;156.25749;-85.58968;,
 -19.88991;-0.39804;-85.58968;,
 -19.39051;-0.39804;-105.85998;,
 -19.88991;156.25749;-85.58968;,
 -39.66081;156.25749;-85.58968;,
 -39.66081;-0.39804;-85.58968;,
 -19.88991;-0.39804;-85.58968;,
 -39.66081;156.25749;-85.58968;,
 -40.16021;156.25749;-105.85998;,
 -40.16021;-0.39804;-105.85998;,
 -39.66081;-0.39804;-85.58968;,
 -19.39051;156.25749;-105.85998;,
 -40.16021;156.25749;-105.85998;,
 -40.16021;-0.39804;-105.85998;,
 -19.39051;-0.39804;-105.85998;,
 119.34419;155.78676;-105.98408;,
 139.61449;155.78676;-105.98408;,
 139.61449;-0.86879;-105.98408;,
 119.34419;-0.86879;-105.98408;,
 139.61449;155.78676;-105.98408;,
 139.61449;155.78676;-85.71378;,
 139.61449;-0.86879;-85.71378;,
 139.61449;-0.86879;-105.98408;,
 139.61449;155.78676;-85.71378;,
 119.34419;155.78676;-85.71378;,
 119.34419;-0.86879;-85.71378;,
 139.61449;-0.86879;-85.71378;,
 119.34419;155.78676;-85.71378;,
 119.34419;155.78676;-105.98408;,
 119.34419;-0.86879;-105.98408;,
 119.34419;-0.86879;-85.71378;,
 139.61449;155.78676;-105.98408;,
 119.34419;155.78676;-105.98408;,
 119.34419;-0.86879;-105.98408;,
 139.61449;-0.86879;-105.98408;,
 -188.79536;155.78676;-105.98408;,
 -168.52501;155.78676;-105.98408;,
 -168.52501;-0.86879;-105.98408;,
 -188.79536;-0.86879;-105.98408;,
 -168.52501;155.78676;-105.98408;,
 -168.52501;155.78676;-85.71378;,
 -168.52501;-0.86879;-85.71378;,
 -168.52501;-0.86879;-105.98408;,
 -168.52501;155.78676;-85.71378;,
 -188.79536;155.78676;-85.71378;,
 -188.79536;-0.86879;-85.71378;,
 -168.52501;-0.86879;-85.71378;,
 -188.79536;155.78676;-85.71378;,
 -188.79536;155.78676;-105.98408;,
 -188.79536;-0.86879;-105.98408;,
 -188.79536;-0.86879;-85.71378;,
 -168.52501;155.78676;-105.98408;,
 -188.79536;155.78676;-105.98408;,
 -188.79536;-0.86879;-105.98408;,
 -168.52501;-0.86879;-105.98408;,
 -168.18352;56.81871;-155.19952;,
 -169.74696;56.81871;-110.42838;,
 -169.74696;-0.95539;-110.42838;,
 -168.18352;-0.95539;-155.19952;,
 -169.74696;56.81871;-110.42838;,
 -191.90776;56.81871;-111.20228;,
 -191.90776;-0.95539;-111.20228;,
 -169.74696;-0.95539;-110.42838;,
 -191.90776;56.81871;-111.20228;,
 -190.34421;56.81871;-155.97348;,
 -190.34421;-0.95539;-155.97348;,
 -191.90776;-0.95539;-111.20228;,
 -190.34421;56.81871;-155.97348;,
 -168.18352;56.81871;-155.19952;,
 -168.18352;-0.95539;-155.19952;,
 -190.34421;-0.95539;-155.97348;,
 -169.74696;56.81871;-110.42838;,
 -168.18352;56.81871;-155.19952;,
 -168.18352;-0.95539;-155.19952;,
 -169.74696;-0.95539;-110.42838;,
 142.54533;56.81871;-155.19952;,
 140.98198;56.81871;-110.42838;,
 140.98198;-0.95539;-110.42838;,
 142.54533;-0.95539;-155.19952;,
 140.98198;56.81871;-110.42838;,
 118.82118;56.81871;-111.20228;,
 118.82118;-0.95539;-111.20228;,
 140.98198;-0.95539;-110.42838;,
 118.82118;56.81871;-111.20228;,
 120.38464;56.81871;-155.97348;,
 120.38464;-0.95539;-155.97348;,
 118.82118;-0.95539;-111.20228;,
 120.38464;56.81871;-155.97348;,
 142.54533;56.81871;-155.19952;,
 142.54533;-0.95539;-155.19952;,
 120.38464;-0.95539;-155.97348;,
 140.98198;56.81871;-110.42838;,
 142.54533;56.81871;-155.19952;,
 142.54533;-0.95539;-155.19952;,
 140.98198;-0.95539;-110.42838;,
 -16.96006;56.81871;-155.19952;,
 -18.52351;56.81871;-110.42838;,
 -18.52351;-0.95539;-110.42838;,
 -16.96006;-0.95539;-155.19952;,
 -18.52351;56.81871;-110.42838;,
 -40.68427;56.81871;-111.20228;,
 -40.68427;-0.95539;-111.20228;,
 -18.52351;-0.95539;-110.42838;,
 -40.68427;56.81871;-111.20228;,
 -39.12077;56.81871;-155.97348;,
 -39.12077;-0.95539;-155.97348;,
 -40.68427;-0.95539;-111.20228;,
 -39.12077;56.81871;-155.97348;,
 -16.96006;56.81871;-155.19952;,
 -16.96006;-0.95539;-155.19952;,
 -39.12077;-0.95539;-155.97348;,
 -18.52351;56.81871;-110.42838;,
 -16.96006;56.81871;-155.19952;,
 -16.96006;-0.95539;-155.19952;,
 -18.52351;-0.95539;-110.42838;;
 
 42;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;12,5,16,17;,
 4;18,19,6,15;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;32,25,36,37;,
 4;38,39,26,35;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;48,49,50,51;,
 4;52,53,54,55;,
 4;52,45,56,57;,
 4;58,59,46,55;,
 4;60,61,62,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;72,65,76,77;,
 4;78,79,66,75;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;88,89,90,91;,
 4;92,93,94,95;,
 4;92,85,96,97;,
 4;98,99,86,95;,
 4;100,101,102,103;,
 4;104,105,106,107;,
 4;108,109,110,111;,
 4;112,113,114,115;,
 4;112,105,116,117;,
 4;118,119,106,115;,
 4;120,121,122,123;,
 4;124,125,126,127;,
 4;128,129,130,131;,
 4;132,133,134,135;,
 4;132,125,136,137;,
 4;138,139,126,135;;
 
 MeshMaterialList {
  5;
  42;
  2,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  1,
  4,
  1,
  1,
  1,
  1,
  1,
  4,
  1,
  1,
  1,
  1,
  1,
  4,
  1,
  3,
  0,
  0,
  0,
  0,
  0,
  3,
  0,
  0,
  0,
  0,
  0,
  3,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.292000;0.266400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.674400;0.749600;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.661600;0.313600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\gosolene000.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  43;
  0.000000;0.000000;-1.000000;,
  0.927895;-0.372380;-0.018560;,
  -0.019731;-0.574110;0.818541;,
  -0.011120;-0.887161;0.461326;,
  -0.999999;0.000000;0.001495;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.999697;0.000000;0.024630;,
  0.000000;0.000000;1.000000;,
  -0.999697;0.000000;0.024630;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.999391;0.000000;0.034900;,
  -0.034901;0.000000;0.999391;,
  -0.999391;0.000000;-0.034902;,
  0.034903;0.000000;-0.999391;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.999391;0.000000;0.034898;,
  -0.034901;0.000000;0.999391;,
  -0.999391;0.000000;-0.034900;,
  0.034903;0.000000;-0.999391;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.999391;0.000000;0.034900;,
  -0.034901;0.000000;0.999391;,
  -0.999391;0.000000;-0.034901;,
  0.034903;0.000000;-0.999391;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;;
  42;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,3,3;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;15,15,15,15;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;18,18,18,18;,
  4;19,19,19,19;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;23,23,23,23;,
  4;24,24,24,24;,
  4;25,25,25,25;,
  4;26,26,26,26;,
  4;27,27,27,27;,
  4;28,28,28,28;,
  4;29,29,29,29;,
  4;30,30,30,30;,
  4;31,31,31,31;,
  4;32,32,32,32;,
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;35,35,35,35;,
  4;36,36,36,36;,
  4;37,37,37,37;,
  4;38,38,38,38;,
  4;39,39,39,39;,
  4;40,40,40,40;,
  4;41,41,41,41;,
  4;42,42,42,42;;
 }
 MeshTextureCoords {
  140;
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
