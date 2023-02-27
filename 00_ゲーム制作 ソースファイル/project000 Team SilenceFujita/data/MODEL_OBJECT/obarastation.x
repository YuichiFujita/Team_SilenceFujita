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
 220;
 -148.88451;291.65866;-248.44440;,
 197.07410;291.65866;-248.44440;,
 197.07410;-2.39440;-248.44440;,
 -148.88451;-2.39440;-248.44440;,
 197.07410;291.65866;-248.44440;,
 197.07410;291.65866;356.83829;,
 197.07410;-2.39440;356.83829;,
 197.07410;-2.39440;-248.44440;,
 197.07410;291.65866;356.83829;,
 -148.88451;291.65866;356.83829;,
 -148.88451;-2.39440;356.83829;,
 197.07410;-2.39440;356.83829;,
 -148.88451;291.65866;356.83829;,
 -148.88451;291.65866;-248.44440;,
 -148.88451;-2.39440;-248.44440;,
 -148.88451;-2.39440;356.83829;,
 197.07410;291.65866;-248.44440;,
 -148.88451;291.65866;-248.44440;,
 -148.88451;-2.39440;-248.44440;,
 197.07410;-2.39440;-248.44440;,
 -558.13708;293.61627;-283.98160;,
 -260.11789;293.61627;-283.98160;,
 -260.11789;-0.43660;-283.98160;,
 -558.13708;-0.43660;-283.98160;,
 -260.11789;293.61627;-283.98160;,
 -260.12000;291.66238;356.84000;,
 -260.11789;-0.43660;356.83829;,
 -260.11789;-0.43660;-283.98160;,
 -260.12000;291.66238;356.84000;,
 -556.77069;293.61627;356.83829;,
 -556.77069;-0.43660;356.83829;,
 -260.11789;-0.43660;356.83829;,
 -556.77069;293.61627;356.83829;,
 -558.13708;293.61627;-283.98160;,
 -558.13708;-0.43660;-283.98160;,
 -556.77069;-0.43660;356.83829;,
 -260.11789;293.61627;-283.98160;,
 -558.13708;293.61627;-283.98160;,
 -558.13708;-0.43660;-283.98160;,
 -260.11789;-0.43660;-283.98160;,
 303.62909;293.61627;-189.14079;,
 576.70801;293.61627;-189.14079;,
 576.70801;-0.43660;-189.14079;,
 303.62909;-0.43660;-189.14079;,
 576.70801;293.61627;-189.14079;,
 576.70801;293.61627;356.83829;,
 576.70801;-0.43660;356.83829;,
 576.70801;-0.43660;-189.14079;,
 576.70801;293.61627;356.83829;,
 303.62909;293.61627;356.83829;,
 303.62909;-0.43660;356.83829;,
 576.70801;-0.43660;356.83829;,
 303.62909;293.61627;356.83829;,
 303.62909;293.61627;-189.14079;,
 303.62909;-0.43660;-189.14079;,
 303.62909;-0.43660;356.83829;,
 576.70801;293.61627;-189.14079;,
 303.62909;293.61627;-189.14079;,
 303.62909;-0.43660;-189.14079;,
 576.70801;-0.43660;-189.14079;,
 -225.13420;639.81866;-189.14079;,
 253.70750;639.81866;-189.14079;,
 253.70750;328.66586;-189.14079;,
 -225.13420;328.66586;-189.14079;,
 253.70750;639.81866;-189.14079;,
 253.70750;639.81866;356.83829;,
 251.26390;328.66586;356.83829;,
 253.70750;328.66586;-189.14079;,
 253.70750;639.81866;356.83829;,
 -225.13420;639.81866;356.83829;,
 -226.93559;328.66586;356.83829;,
 251.26390;328.66586;356.83829;,
 -225.13420;639.81866;356.83829;,
 -225.13420;639.81866;-189.14079;,
 -225.13420;328.66586;-189.14079;,
 -226.93559;328.66586;356.83829;,
 253.70750;639.81866;-189.14079;,
 -225.13420;639.81866;-189.14079;,
 -225.13420;328.66586;-189.14079;,
 253.70750;328.66586;-189.14079;,
 253.70750;639.81866;-191.04660;,
 577.92358;639.81866;-191.04660;,
 576.70801;328.18246;-191.04660;,
 253.70750;328.66586;-191.04660;,
 577.92358;639.81866;-191.04660;,
 577.92358;639.81866;354.93250;,
 576.70801;328.18246;354.93250;,
 576.70801;328.18246;-191.04660;,
 577.92358;639.81866;354.93250;,
 253.70750;639.81866;354.93250;,
 252.78300;328.66586;354.93250;,
 576.70801;328.18246;354.93250;,
 253.70750;639.81866;354.93250;,
 253.70750;639.81866;-191.04660;,
 253.70750;328.66586;-191.04660;,
 252.78300;328.66586;354.93250;,
 577.92358;639.81866;-191.04660;,
 253.70750;639.81866;-191.04660;,
 253.70750;328.66586;-191.04660;,
 576.70801;328.18246;-191.04660;,
 -557.56372;639.81866;-191.04660;,
 -225.13420;639.81866;-191.04660;,
 -225.13420;328.66586;-191.04660;,
 -557.56372;328.25128;-191.04660;,
 -225.13420;639.81866;-191.04660;,
 -225.13420;639.81866;354.93250;,
 -227.66240;328.66586;354.93250;,
 -225.13420;328.66586;-191.04660;,
 -225.13420;639.81866;354.93250;,
 -557.56372;639.81866;354.93250;,
 -556.77069;328.18246;354.93250;,
 -227.66240;328.66586;354.93250;,
 -557.56372;639.81866;354.93250;,
 -557.56372;639.81866;-191.04660;,
 -557.56372;328.25128;-191.04660;,
 -556.77069;328.18246;354.93250;,
 -225.13420;639.81866;-191.04660;,
 -557.56372;639.81866;-191.04660;,
 -557.56372;328.25128;-191.04660;,
 -225.13420;328.66586;-191.04660;,
 -555.55750;330.29398;-354.66681;,
 576.21527;330.29398;-354.66681;,
 576.21527;294.62326;-354.66681;,
 -555.55750;294.62326;-354.66681;,
 576.21527;330.29398;-354.66681;,
 576.21527;330.29398;379.56720;,
 576.21527;294.62326;379.56720;,
 576.21527;294.62326;-354.66681;,
 576.21527;330.29398;379.56720;,
 -555.55750;330.29398;379.56720;,
 -555.55750;294.62326;379.56720;,
 576.21527;294.62326;379.56720;,
 -555.55750;330.29398;379.56720;,
 -555.55750;330.29398;-354.66681;,
 -555.55750;294.62326;-354.66681;,
 -555.55750;294.62326;379.56720;,
 576.21527;330.29398;-354.66681;,
 -555.55750;330.29398;-354.66681;,
 -555.55750;294.62326;-354.66681;,
 576.21527;294.62326;-354.66681;,
 -260.11789;293.61627;-283.98160;,
 -148.88451;291.65866;-281.39929;,
 -148.69240;-0.43660;-283.98160;,
 -258.16501;-0.43660;-283.98160;,
 -148.88451;291.65866;-281.39929;,
 -148.88451;291.65866;356.83829;,
 -148.69240;-0.43660;356.83829;,
 -148.69240;-0.43660;-283.98160;,
 -148.88451;291.65866;356.83829;,
 -260.11789;293.61627;356.83829;,
 -258.16501;-0.43660;356.83829;,
 -148.69240;-0.43660;356.83829;,
 -260.11789;293.61627;356.83829;,
 -260.11789;293.61627;-283.98160;,
 -258.16501;-0.43660;-283.98160;,
 -258.16501;-0.43660;356.83829;,
 -148.88451;291.65866;-281.39929;,
 -260.11789;293.61627;-283.98160;,
 -258.16501;-0.43660;-283.98160;,
 -148.69240;-0.43660;-283.98160;,
 197.77000;291.66238;-283.98001;,
 307.24319;291.66238;-283.98001;,
 307.24689;-0.43660;-283.98160;,
 197.77370;-0.43660;-283.98160;,
 307.24319;291.66238;-283.98001;,
 303.62909;293.61627;356.83829;,
 307.24689;-0.43660;356.83829;,
 307.24689;-0.43660;-283.98160;,
 303.62909;293.61627;356.83829;,
 197.07410;291.65866;356.83829;,
 197.77370;-0.43660;356.83829;,
 307.24689;-0.43660;356.83829;,
 197.07410;291.65866;356.83829;,
 197.77000;291.66238;-283.98001;,
 197.77370;-0.43660;-283.98160;,
 197.77370;-0.43660;356.83829;,
 307.24319;291.66238;-283.98001;,
 197.77000;291.66238;-283.98001;,
 197.77370;-0.43660;-283.98160;,
 307.24689;-0.43660;-283.98160;,
 319.93729;246.38300;-288.38580;,
 515.00787;246.38300;-288.38580;,
 515.00787;-3.32950;-288.38580;,
 319.93729;-3.32950;-288.38580;,
 515.00787;246.38300;-288.38580;,
 515.00787;246.38300;-196.69141;,
 515.00787;-3.32950;-196.69141;,
 515.00787;-3.32950;-288.38580;,
 515.00787;246.38300;-196.69141;,
 319.93729;246.38300;-196.69141;,
 319.93729;-3.32950;-196.69141;,
 515.00787;-3.32950;-196.69141;,
 319.93729;246.38300;-196.69141;,
 319.93729;246.38300;-288.38580;,
 319.93729;-3.32950;-288.38580;,
 319.93729;-3.32950;-196.69141;,
 515.00787;246.38300;-288.38580;,
 319.93729;246.38300;-288.38580;,
 319.93729;-3.32950;-288.38580;,
 515.00787;-3.32950;-288.38580;,
 -554.44458;638.81702;356.69730;,
 572.76398;638.81702;356.69730;,
 572.76398;1.54000;356.69730;,
 -554.44458;1.54000;356.69730;,
 572.76398;638.81702;356.69730;,
 572.76398;638.81702;386.62881;,
 572.76398;1.54000;386.62881;,
 572.76398;1.54000;356.69730;,
 572.76398;638.81702;386.62881;,
 -554.44458;638.81702;386.62881;,
 -554.44458;1.54000;386.62881;,
 572.76398;1.54000;386.62881;,
 -554.44458;638.81702;386.62881;,
 -554.44458;638.81702;356.69730;,
 -554.44458;1.54000;356.69730;,
 -554.44458;1.54000;386.62881;,
 572.76398;638.81702;356.69730;,
 -554.44458;638.81702;356.69730;,
 -554.44458;1.54000;356.69730;,
 572.76398;1.54000;356.69730;;
 
 66;
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
 4;138,139,126,135;,
 4;140,141,142,143;,
 4;144,145,146,147;,
 4;148,149,150,151;,
 4;152,153,154,155;,
 4;152,145,156,157;,
 4;158,159,146,155;,
 4;160,161,162,163;,
 4;164,165,166,167;,
 4;168,169,170,171;,
 4;172,173,174,175;,
 4;172,165,176,177;,
 4;178,179,166,175;,
 4;180,181,182,183;,
 4;184,185,186,187;,
 4;188,189,190,191;,
 4;192,193,194,195;,
 4;192,185,196,197;,
 4;198,199,186,195;,
 4;200,201,202,203;,
 4;204,205,206,207;,
 4;208,209,210,211;,
 4;212,213,214,215;,
 4;212,205,216,217;,
 4;218,219,206,215;;
 
 MeshMaterialList {
  10;
  66;
  4,
  0,
  0,
  0,
  0,
  0,
  5,
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
  7,
  0,
  0,
  0,
  0,
  0,
  6,
  0,
  0,
  0,
  0,
  0,
  6,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  0,
  1,
  1,
  9,
  8,
  0,
  0,
  9,
  0,
  0,
  8,
  0,
  0,
  0,
  0,
  0,
  3,
  3,
  3,
  3,
  2,
  2,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.100000;0.100000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.091200;0.091200;0.091200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.514400;0.668000;1.000000;;
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
    "data\\TEXTURE\\UFOcatcher.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\ObaraStation002.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\ObaraStation003.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\BillTex002.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\ObaraStation000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\ObaraStation001.png";
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
  66;
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000004;0.000002;,
  -0.000003;-0.000003;1.000000;,
  -0.999998;0.000000;0.002132;,
  0.003293;0.999993;0.001524;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.999990;-0.003927;0.002238;,
  0.000000;0.000000;1.000000;,
  -0.999995;0.002895;-0.001650;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.999992;-0.003901;-0.000000;,
  0.000000;0.000000;1.000000;,
  -0.999999;0.001486;-0.000847;,
  0.000000;1.000000;0.000000;,
  -0.001494;-0.999999;-0.000001;,
  0.000000;0.000000;-1.000000;,
  0.999989;-0.004063;0.002315;,
  0.000000;0.000000;1.000000;,
  -0.999999;-0.001272;0.000726;,
  0.000000;1.000000;0.000000;,
  0.001358;-0.999999;-0.000061;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.011607;0.004498;-0.999923;,
  1.000000;0.000658;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.999978;-0.006641;0.000000;,
  0.017596;0.999845;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000005;-1.000000;,
  0.999977;0.006158;0.002801;,
  0.000000;0.000000;1.000000;,
  -0.999999;-0.001204;-0.000543;,
  -0.009185;0.999957;-0.001532;,
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
  0.000000;-1.000000;-0.000000;;
  66;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
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
  4;42,42,42,42;,
  4;43,43,43,43;,
  4;44,44,44,44;,
  4;45,45,45,45;,
  4;46,46,46,46;,
  4;47,47,47,47;,
  4;48,48,48,48;,
  4;49,49,49,49;,
  4;50,50,50,50;,
  4;51,51,51,51;,
  4;52,52,52,52;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,56,56,56;,
  4;57,57,57,57;,
  4;58,58,58,58;,
  4;59,59,59,59;,
  4;60,60,60,60;,
  4;61,61,61,61;,
  4;62,62,62,62;,
  4;63,63,63,63;,
  4;64,64,64,64;,
  4;65,65,65,65;;
 }
 MeshTextureCoords {
  220;
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
