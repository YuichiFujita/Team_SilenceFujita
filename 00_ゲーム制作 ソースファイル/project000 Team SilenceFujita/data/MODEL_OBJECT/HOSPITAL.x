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
 153;
 -602.26422;693.96198;448.28540;,
 -602.26422;693.96198;157.08130;,
 -602.26422;479.48251;157.08130;,
 -602.26422;479.48251;448.28540;,
 -329.29099;693.96198;448.28540;,
 -329.29099;479.48251;448.28540;,
 -329.29099;693.96198;157.08130;,
 -329.29099;479.48251;157.08130;,
 -602.26422;693.96198;157.08130;,
 -602.26422;693.96198;448.28540;,
 -329.29099;693.96198;448.28540;,
 -329.29099;693.96198;157.08130;,
 -602.26422;154.89540;59.23530;,
 -329.29099;154.89540;59.23530;,
 -329.29361;-1.16800;59.23330;,
 -602.26672;-1.16800;59.23330;,
 -329.29099;216.47900;-125.14830;,
 -602.26422;216.47900;-128.61470;,
 -602.26422;216.47900;59.23530;,
 -329.29099;216.47900;59.23530;,
 -329.29099;154.89540;-125.14830;,
 -329.29099;154.89540;59.23530;,
 -602.26422;154.89540;-128.61470;,
 -602.26422;154.89540;59.23530;,
 -602.26422;297.88129;59.23530;,
 -602.26422;376.24921;59.23530;,
 -329.29099;376.24921;59.23530;,
 -329.29099;297.88129;59.23530;,
 -602.26422;297.88129;59.23530;,
 -329.29099;297.88129;59.23530;,
 369.56079;376.24921;59.23530;,
 369.56079;297.88129;59.23530;,
 -83.91160;297.88129;59.23530;,
 -83.91160;376.24921;59.23530;,
 369.56079;297.88129;59.23530;,
 369.56079;216.47900;59.23530;,
 -83.91160;216.47900;59.23530;,
 -83.91160;297.88129;59.23530;,
 369.56079;216.47900;59.23530;,
 369.56079;154.89540;59.23530;,
 -83.91160;154.89540;59.23530;,
 -83.91160;216.47900;59.23530;,
 369.56079;154.89540;59.23530;,
 455.67941;-1.16800;59.23330;,
 -329.29361;-1.16800;59.23330;,
 -83.91160;154.89540;59.23530;,
 455.68201;376.24921;-373.57080;,
 455.68201;297.88129;-373.57080;,
 455.68201;297.88129;-40.04410;,
 455.68201;376.24921;-40.04410;,
 455.68201;376.24921;-373.57080;,
 455.68201;376.24921;-40.04410;,
 455.68201;479.48251;59.23530;,
 455.68201;479.48251;-491.43011;,
 455.68201;297.88129;-373.57080;,
 455.68201;216.47900;-373.57080;,
 455.68201;216.47900;-40.04410;,
 455.68201;297.88129;-40.04410;,
 455.68201;216.47900;-373.57080;,
 455.68201;154.89540;-373.57080;,
 455.68201;154.89540;-40.04410;,
 455.68201;216.47900;-40.04410;,
 455.68201;154.89540;-373.57080;,
 455.67941;-1.16800;-491.43201;,
 455.67941;-1.16800;59.23330;,
 455.68201;154.89540;-40.04410;,
 652.41058;376.24921;-491.43011;,
 652.41058;297.88129;-491.43011;,
 517.23022;297.88129;-491.43011;,
 517.23022;376.24921;-491.43011;,
 652.41058;376.24921;-491.43011;,
 517.23022;376.24921;-491.43011;,
 455.68201;479.48251;-491.43011;,
 709.63019;479.48251;-491.43011;,
 652.41058;297.88129;-491.43011;,
 652.41058;216.47900;-491.43011;,
 517.23022;216.47900;-491.43011;,
 517.23022;297.88129;-491.43011;,
 652.41058;216.47900;-491.43011;,
 652.41058;154.89540;-491.43011;,
 517.23022;154.89540;-491.43011;,
 517.23022;216.47900;-491.43011;,
 652.41058;154.89540;-491.43011;,
 709.62762;-1.16800;-491.43201;,
 455.67941;-1.16800;-491.43201;,
 517.23022;154.89540;-491.43011;,
 -697.65802;-1.16800;448.28339;,
 -697.65802;-1.16800;59.23330;,
 709.62762;-1.16800;448.28339;,
 709.63019;479.48251;448.28540;,
 709.62762;-1.16800;448.28339;,
 709.62762;-1.16800;-491.43201;,
 709.63019;479.48251;-491.43011;,
 -697.65540;479.48251;59.23530;,
 -697.65540;479.48251;448.28540;,
 709.62762;-1.16800;448.28339;,
 709.63019;479.48251;448.28540;,
 455.68201;479.48251;59.23530;,
 455.68201;376.24921;59.23530;,
 -83.91160;376.24921;59.23530;,
 -83.91160;479.48251;59.23530;,
 710.57397;135.15170;-427.23181;,
 710.57397;213.51961;-427.23181;,
 710.57397;213.51961;387.01019;,
 710.57397;135.15170;387.01019;,
 710.57397;297.88129;-427.23181;,
 710.57397;376.24921;-427.23181;,
 710.57397;376.24921;387.01019;,
 710.57397;297.88129;387.01019;,
 -697.65540;479.48251;59.23530;,
 -602.26672;-1.16800;59.23330;,
 -329.29361;-1.16800;59.23330;,
 709.63019;479.48251;448.28540;,
 -585.35907;172.76260;-113.74390;,
 -537.09253;172.76260;-113.74390;,
 -537.09137;-1.16800;-113.74670;,
 -585.35797;-1.16800;-113.74670;,
 -537.09253;172.76260;-113.74390;,
 -537.09253;172.76260;-65.47690;,
 -537.09137;-1.16800;-65.47970;,
 -537.09137;-1.16800;-113.74670;,
 -537.09253;172.76260;-65.47690;,
 -585.35907;172.76260;-65.47690;,
 -585.35797;-1.16800;-65.47970;,
 -537.09137;-1.16800;-65.47970;,
 -585.35907;172.76260;-65.47690;,
 -585.35907;172.76260;-113.74390;,
 -585.35797;-1.16800;-113.74670;,
 -585.35797;-1.16800;-65.47970;,
 -537.09253;172.76260;-113.74390;,
 -585.35907;172.76260;-113.74390;,
 -585.35797;-1.16800;-113.74670;,
 -537.09137;-1.16800;-113.74670;,
 -395.01041;172.76260;-113.74390;,
 -346.74359;172.76260;-113.74390;,
 -346.74240;-1.16800;-113.74670;,
 -395.00919;-1.16800;-113.74670;,
 -346.74359;172.76260;-113.74390;,
 -346.74359;172.76260;-65.47690;,
 -346.74240;-1.16800;-65.47970;,
 -346.74240;-1.16800;-113.74670;,
 -346.74359;172.76260;-65.47690;,
 -395.01041;172.76260;-65.47690;,
 -395.00919;-1.16800;-65.47970;,
 -346.74240;-1.16800;-65.47970;,
 -395.01041;172.76260;-65.47690;,
 -395.01041;172.76260;-113.74390;,
 -395.00919;-1.16800;-113.74670;,
 -395.00919;-1.16800;-65.47970;,
 -346.74359;172.76260;-113.74390;,
 -395.01041;172.76260;-113.74390;,
 -395.00919;-1.16800;-113.74670;,
 -346.74240;-1.16800;-113.74670;;
 
 59;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;1,6,7,2;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,16,19,21;,
 4;22,20,21,23;,
 4;17,22,23,18;,
 4;17,16,20,22;,
 4;24,25,26,27;,
 4;28,29,19,18;,
 4;30,31,32,33;,
 4;34,35,36,37;,
 4;38,39,40,41;,
 4;42,43,44,45;,
 4;46,47,48,49;,
 4;50,51,52,53;,
 4;54,55,56,57;,
 4;58,59,60,61;,
 4;62,63,64,65;,
 4;66,67,68,69;,
 4;70,71,72,73;,
 4;74,75,76,77;,
 4;78,79,80,81;,
 4;82,83,84,85;,
 4;86,87,64,88;,
 4;89,90,91,92;,
 4;93,87,86,94;,
 4;94,86,95,96;,
 4;97,98,99,100;,
 4;101,102,103,104;,
 4;105,106,107,108;,
 4;63,59,46,53;,
 4;52,49,60,64;,
 4;43,39,30,98;,
 4;109,25,110,87;,
 4;26,100,40,111;,
 4;25,109,100,26;,
 4;2,93,94,3;,
 4;93,2,7,52;,
 4;112,52,7,5;,
 4;72,69,80,84;,
 4;83,79,66,73;,
 4;73,53,52,112;,
 4;64,63,83,88;,
 4;113,114,115,116;,
 4;117,118,119,120;,
 4;121,122,123,124;,
 4;125,126,127,128;,
 4;125,118,129,130;,
 4;131,132,119,128;,
 4;133,134,135,136;,
 4;137,138,139,140;,
 4;141,142,143,144;,
 4;145,146,147,148;,
 4;145,138,149,150;,
 4;151,152,139,148;;
 
 MeshMaterialList {
  11;
  59;
  0,
  2,
  0,
  2,
  0,
  10,
  1,
  1,
  1,
  1,
  1,
  8,
  0,
  6,
  0,
  7,
  0,
  9,
  0,
  0,
  9,
  0,
  4,
  0,
  0,
  5,
  0,
  0,
  0,
  0,
  0,
  3,
  9,
  9,
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.696800;0.596000;1.000000;;
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
    "data\\TEXTURE\\akajuuji.png";
   }
  }
  Material {
   0.775200;0.749600;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\HOSPITAL.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\doar000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\doar000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\doar000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\doar000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\doar000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Glass.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\doar002.png";
   }
  }
 }
 MeshNormals {
  45;
  0.000000;-1.000000;-0.000000;,
  -1.000000;0.000005;0.000000;,
  1.000000;-0.000005;-0.000000;,
  0.000001;0.000009;-1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000013;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000001;0.000001;-1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000001;0.000005;-1.000000;,
  0.000001;0.000009;-1.000000;,
  -0.000003;0.000001;-1.000000;,
  0.000001;0.000002;-1.000000;,
  -0.000002;0.000005;-1.000000;,
  -0.000003;0.000008;-1.000000;,
  -1.000000;0.000001;-0.000003;,
  -1.000000;0.000001;-0.000004;,
  -1.000000;0.000006;-0.000003;,
  -1.000000;0.000001;0.000004;,
  -1.000000;0.000006;0.000004;,
  0.000003;0.000001;-1.000000;,
  -1.000000;0.000001;0.000007;,
  0.000003;0.000005;-1.000000;,
  -1.000000;0.000010;0.000007;,
  -0.000007;0.000001;-1.000000;,
  -0.000010;0.000001;-1.000000;,
  -0.000007;0.000005;-1.000000;,
  -0.000010;0.000007;-1.000000;,
  0.012698;0.000000;-0.999919;,
  -1.000000;0.000010;-0.000004;,
  0.000005;0.000001;-1.000000;,
  0.000005;0.000007;-1.000000;,
  0.000000;-0.000004;1.000000;,
  0.000001;0.000002;-1.000000;,
  0.000002;0.000005;-1.000000;,
  0.000000;0.000016;-1.000000;,
  1.000000;0.000007;0.000000;,
  0.000000;-0.000016;1.000000;,
  1.000000;0.000007;0.000000;,
  -1.000000;-0.000006;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -1.000000;-0.000007;0.000000;;
  59;
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;10,10,10,10;,
  4;8,8,8,8;,
  4;7,7,12,3;,
  4;8,8,8,8;,
  4;6,6,6,6;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;30,30,30,30;,
  4;10,14,9,10;,
  4;10,10,10,10;,
  4;13,10,10,10;,
  4;10,10,10,10;,
  4;10,15,11,10;,
  4;15,16,12,11;,
  4;20,4,4,17;,
  4;20,17,18,23;,
  4;4,4,4,4;,
  4;4,21,19,4;,
  4;21,25,31,19;,
  4;26,10,10,22;,
  4;26,22,32,27;,
  4;10,10,10,10;,
  4;10,28,24,10;,
  4;28,29,33,24;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;34,34,34,34;,
  4;10,13,10,9;,
  4;6,6,6,6;,
  4;6,6,6,6;,
  4;25,21,20,23;,
  4;18,17,19,31;,
  4;16,15,13,13;,
  4;35,14,3,36;,
  4;9,9,11,12;,
  4;14,35,9,9;,
  4;8,8,8,8;,
  4;8,8,8,8;,
  4;8,8,8,8;,
  4;32,22,24,33;,
  4;29,28,26,27;,
  4;8,8,8,8;,
  4;0,0,0,0;,
  4;37,37,37,37;,
  4;38,38,38,38;,
  4;39,39,39,39;,
  4;41,41,41,41;,
  4;42,42,42,42;,
  4;43,43,43,43;,
  4;37,37,37,37;,
  4;40,40,40,40;,
  4;39,39,39,39;,
  4;44,44,44,44;,
  4;42,42,42,42;,
  4;43,43,43,43;;
 }
 MeshTextureCoords {
  153;
  0.149830;0.214210;,
  0.149830;0.214210;,
  0.149830;0.761210;,
  0.149830;0.761210;,
  0.846010;0.214210;,
  0.846010;0.761210;,
  0.846010;0.214210;,
  0.846010;0.761210;,
  0.064650;0.748500;,
  0.064650;0.000000;,
  0.249650;0.000000;,
  0.249650;0.748500;,
  0.007820;-0.010370;,
  1.014770;-0.010370;,
  1.014760;0.996080;,
  0.007810;0.996080;,
  0.249650;0.406010;,
  0.064650;0.406010;,
  0.064650;0.406010;,
  0.249650;0.406010;,
  0.249650;0.572610;,
  0.249650;0.572610;,
  0.064650;0.572610;,
  0.064650;0.572610;,
  -0.994260;1.019870;,
  -0.994260;-0.037440;,
  2.022560;-0.037440;,
  2.022560;1.019870;,
  0.064650;0.280340;,
  0.249650;0.280340;,
  2.988220;-0.002320;,
  2.988220;1.013650;,
  -1.035920;1.013650;,
  -1.035920;-0.002320;,
  0.723290;0.280340;,
  0.723290;0.406010;,
  0.415950;0.406010;,
  0.415950;0.280340;,
  2.988220;-0.030630;,
  2.988220;1.004110;,
  -1.035920;1.004110;,
  -1.035920;-0.030630;,
  0.723290;0.572610;,
  0.723290;1.000000;,
  0.415950;1.000000;,
  0.415950;0.572610;,
  0.128340;0.436990;,
  0.128340;0.619750;,
  0.442040;0.619750;,
  0.442040;0.436990;,
  0.781650;0.159370;,
  0.781650;0.159370;,
  0.781650;0.000000;,
  0.781650;0.000000;,
  0.781650;0.280340;,
  0.781650;0.406010;,
  0.781650;0.406010;,
  0.781650;0.280340;,
  0.128340;0.809590;,
  0.128340;0.953210;,
  0.442040;0.953210;,
  0.442040;0.809590;,
  0.781650;0.572610;,
  0.781650;1.000000;,
  0.781650;1.000000;,
  0.781650;0.572610;,
  1.034120;0.006690;,
  1.034120;1.002570;,
  -0.009140;1.002570;,
  -0.009140;0.006690;,
  0.950800;0.159370;,
  0.834570;0.159370;,
  0.834570;0.000000;,
  0.950800;0.000000;,
  0.950800;0.280340;,
  0.950800;0.406010;,
  0.834570;0.406010;,
  0.834570;0.280340;,
  1.044860;-0.000580;,
  1.044860;1.013710;,
  -0.019380;1.013710;,
  -0.019380;-0.000580;,
  0.950800;0.572610;,
  0.950800;1.000000;,
  0.834570;1.000000;,
  0.834570;0.572610;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.218350;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.981570;0.387820;,
  0.981570;0.661820;,
  0.145840;0.661820;,
  0.145840;0.387820;,
  0.077870;0.999250;,
  0.077870;0.816490;,
  0.843720;0.816490;,
  0.843720;0.999250;,
  0.077870;0.619750;,
  0.077870;0.436990;,
  0.843720;0.436990;,
  0.843720;0.619750;,
  0.064650;1.000000;,
  0.007820;1.008470;,
  1.014770;1.008470;,
  0.218350;0.000000;,
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
