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
 50;
 6.48896;2.86985;2.85219;,
 1.50528;3.15284;3.13512;,
 1.50526;0.01770;4.43373;,
 6.48896;0.01766;4.03360;,
 1.50524;-3.11744;3.13512;,
 6.48896;-2.83451;2.85219;,
 1.50524;-4.41601;-0.00000;,
 6.48896;-4.01590;-0.00000;,
 1.50524;-3.11744;-3.13512;,
 6.48896;-2.83451;-2.85219;,
 1.50526;0.01770;-4.43373;,
 6.48896;0.01766;-4.03360;,
 1.50528;3.15284;-3.13512;,
 6.48896;2.86985;-2.85219;,
 1.50528;4.45143;0.00000;,
 6.48896;4.05128;0.00000;,
 10.97722;2.41967;2.40200;,
 10.97722;0.01761;3.39695;,
 10.97722;-2.38436;2.40200;,
 10.97720;-3.37932;-0.00000;,
 10.97722;-2.38436;-2.40200;,
 10.97722;0.01761;-3.39695;,
 10.97722;2.41967;-2.40200;,
 10.97724;3.41460;0.00000;,
 14.77630;2.00247;1.98484;,
 14.77626;0.01761;2.80698;,
 14.77626;-1.96723;1.98484;,
 14.77626;-2.78933;-0.00000;,
 14.77626;-1.96723;-1.98486;,
 14.77626;0.01761;-2.80698;,
 14.77630;2.00247;-1.98486;,
 14.77632;2.82459;0.00000;,
 -0.57620;2.22921;2.21149;,
 -1.93516;0.01772;0.00000;,
 -0.57622;0.01770;3.12751;,
 -0.57620;3.14521;0.00000;,
 -0.57620;2.22921;-2.21149;,
 -0.57622;0.01770;-3.12751;,
 -0.57622;-2.19380;-2.21149;,
 -0.57622;-3.10979;0.00000;,
 -0.57622;-2.19380;2.21149;,
 15.65506;1.90130;0.00000;,
 15.65504;1.34958;1.33197;,
 16.43300;0.01761;0.00000;,
 15.65504;1.34958;-1.33197;,
 15.65502;0.01761;-1.88369;,
 15.65502;-1.31435;-1.33197;,
 15.65502;-1.86606;-0.00000;,
 15.65502;-1.31435;1.33197;,
 15.65502;0.01761;1.88369;;
 
 56;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,1,0;,
 4;16,0,3,17;,
 4;17,3,5,18;,
 4;18,5,7,19;,
 4;19,7,9,20;,
 4;20,9,11,21;,
 4;21,11,13,22;,
 4;22,13,15,23;,
 4;23,15,0,16;,
 4;24,16,17,25;,
 4;25,17,18,26;,
 4;26,18,19,27;,
 4;27,19,20,28;,
 4;28,20,21,29;,
 4;29,21,22,30;,
 4;30,22,23,31;,
 4;31,23,16,24;,
 3;32,33,34;,
 3;35,33,32;,
 3;36,33,35;,
 3;37,33,36;,
 3;38,33,37;,
 3;39,33,38;,
 3;40,33,39;,
 3;40,34,33;,
 4;37,36,12,10;,
 4;14,12,36,35;,
 4;1,14,35,32;,
 4;2,1,32,34;,
 4;2,34,40,4;,
 4;40,39,6,4;,
 4;39,38,8,6;,
 4;38,37,10,8;,
 3;41,42,43;,
 3;44,41,43;,
 3;45,44,43;,
 3;46,45,43;,
 3;47,46,43;,
 3;48,47,43;,
 3;49,48,43;,
 3;49,43,42;,
 4;49,42,24,25;,
 4;24,42,41,31;,
 4;31,41,44,30;,
 4;30,44,45,29;,
 4;46,28,29,45;,
 4;28,46,47,27;,
 4;27,47,48,26;,
 4;49,25,26,48;;
 
 MeshMaterialList {
  1;
  56;
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
  }
 }
 MeshNormals {
  50;
  0.469391;0.624364;0.624372;,
  0.469384;-0.000006;0.882994;,
  0.469382;-0.624375;0.624369;,
  0.469378;-0.882997;0.000001;,
  0.469384;-0.624374;-0.624368;,
  0.469386;-0.000006;-0.882993;,
  0.469394;0.624363;-0.624372;,
  0.469393;0.882989;0.000001;,
  -0.241178;0.686235;0.686233;,
  -0.241180;0.000001;0.970480;,
  -0.241183;-0.686235;0.686230;,
  -0.241182;-0.970480;0.000000;,
  -0.241183;-0.686235;-0.686230;,
  -0.241180;0.000001;-0.970480;,
  -0.241178;0.686235;-0.686233;,
  -0.241176;0.970481;0.000000;,
  1.000000;-0.000009;-0.000000;,
  0.110306;0.702791;0.702793;,
  0.110302;-0.000002;0.993898;,
  0.110297;-0.702795;0.702790;,
  0.110295;-0.993899;-0.000000;,
  0.110297;-0.702795;-0.702790;,
  0.110302;-0.000002;-0.993898;,
  0.110306;0.702791;-0.702793;,
  0.110307;0.993897;0.000000;,
  0.146956;0.699429;0.699431;,
  0.146952;-0.000004;0.989144;,
  0.146947;-0.699433;0.699429;,
  0.146945;-0.989145;0.000000;,
  0.146947;-0.699432;-0.699429;,
  0.146950;-0.000004;-0.989144;,
  0.146956;0.699428;-0.699431;,
  0.146957;0.989143;0.000000;,
  -1.000000;0.000004;0.000000;,
  -0.762756;0.000004;0.646686;,
  -0.762755;0.457280;0.457274;,
  -0.762753;0.646690;0.000000;,
  -0.762755;0.457280;-0.457274;,
  -0.762756;0.000004;-0.646686;,
  -0.762759;-0.457275;-0.457272;,
  -0.762756;-0.646686;0.000000;,
  -0.762759;-0.457275;0.457272;,
  0.839918;0.383748;0.383765;,
  0.839922;0.542708;0.000000;,
  0.839919;0.383746;-0.383763;,
  0.839914;-0.000007;-0.542719;,
  0.839913;-0.383764;-0.383759;,
  0.839909;-0.542727;0.000000;,
  0.839912;-0.383765;0.383760;,
  0.839913;-0.000007;0.542722;;
  56;
  4;17,8,9,18;,
  4;18,9,10,19;,
  4;19,10,11,20;,
  4;20,11,12,21;,
  4;21,12,13,22;,
  4;22,13,14,23;,
  4;23,14,15,24;,
  4;24,15,8,17;,
  4;25,17,18,26;,
  4;26,18,19,27;,
  4;27,19,20,28;,
  4;28,20,21,29;,
  4;29,21,22,30;,
  4;30,22,23,31;,
  4;31,23,24,32;,
  4;32,24,17,25;,
  4;0,25,26,1;,
  4;1,26,27,2;,
  4;2,27,28,3;,
  4;3,28,29,4;,
  4;4,29,30,5;,
  4;5,30,31,6;,
  4;6,31,32,7;,
  4;7,32,25,0;,
  3;35,33,34;,
  3;36,33,35;,
  3;37,33,36;,
  3;38,33,37;,
  3;39,33,38;,
  3;40,33,39;,
  3;41,33,40;,
  3;41,34,33;,
  4;38,37,14,13;,
  4;15,14,37,36;,
  4;8,15,36,35;,
  4;9,8,35,34;,
  4;9,34,41,10;,
  4;41,40,11,10;,
  4;40,39,12,11;,
  4;39,38,13,12;,
  3;43,42,16;,
  3;44,43,16;,
  3;45,44,16;,
  3;46,45,16;,
  3;47,46,16;,
  3;48,47,16;,
  3;49,48,16;,
  3;49,16,42;,
  4;49,42,0,1;,
  4;0,42,43,7;,
  4;7,43,44,6;,
  4;6,44,45,5;,
  4;46,4,5,45;,
  4;4,46,47,3;,
  4;3,47,48,2;,
  4;49,1,2,48;;
 }
 MeshTextureCoords {
  50;
  0.780100;0.336660;,
  0.794890;0.322910;,
  0.801370;0.330170;,
  0.786310;0.343610;,
  0.807860;0.337440;,
  0.792520;0.350570;,
  0.810540;0.340450;,
  0.795090;0.353450;,
  0.807860;0.337440;,
  0.792520;0.350570;,
  0.801370;0.330170;,
  0.786310;0.343610;,
  0.794890;0.322910;,
  0.780100;0.336660;,
  0.792200;0.319900;,
  0.777530;0.333770;,
  0.765640;0.350980;,
  0.771140;0.357150;,
  0.776650;0.363310;,
  0.778930;0.365870;,
  0.776650;0.363310;,
  0.771140;0.357150;,
  0.765640;0.350980;,
  0.763360;0.348420;,
  0.756360;0.360870;,
  0.761070;0.366140;,
  0.765770;0.371410;,
  0.767720;0.373590;,
  0.765770;0.371410;,
  0.761070;0.366140;,
  0.756360;0.360870;,
  0.754420;0.358680;,
  0.798130;0.326540;,
  0.801370;0.330180;,
  0.801370;0.330170;,
  0.796790;0.325040;,
  0.798130;0.326540;,
  0.801370;0.330170;,
  0.804620;0.333810;,
  0.805960;0.335310;,
  0.804620;0.333810;,
  0.757740;0.362410;,
  0.758720;0.363510;,
  0.761070;0.366140;,
  0.758720;0.363510;,
  0.761070;0.366140;,
  0.763420;0.368780;,
  0.764400;0.369870;,
  0.763420;0.368780;,
  0.761070;0.366140;;
 }
}
