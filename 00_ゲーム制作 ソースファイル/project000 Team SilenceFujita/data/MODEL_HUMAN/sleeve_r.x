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
 84;
 -3.97032;-4.82430;5.17894;,
 -3.97030;0.10258;7.32414;,
 -9.77232;0.10260;8.56876;,
 -9.77230;-5.66152;6.05902;,
 -3.97030;5.02958;5.17894;,
 -9.77230;5.86666;6.05902;,
 -3.97030;7.07034;0.00000;,
 -9.77232;8.25436;0.00000;,
 -3.97030;5.02958;-5.17894;,
 -9.77230;5.86666;-6.05902;,
 -3.97030;0.10258;-7.32414;,
 -9.77232;0.10260;-8.56876;,
 -3.97032;-4.82430;-5.17894;,
 -9.77230;-5.66152;-6.05902;,
 -3.97030;-6.86510;0.00000;,
 -9.77232;-8.04910;0.00000;,
 -16.26234;0.10258;0.00000;,
 -18.67974;-4.50224;4.84040;,
 -18.67976;0.10258;6.84536;,
 -18.67976;4.70744;4.84040;,
 -18.67978;6.61478;0.00000;,
 -18.67976;4.70744;-4.84040;,
 -18.67976;0.10258;-6.84536;,
 -18.67974;-4.50224;-4.84040;,
 -18.66906;-6.40962;0.00000;,
 4.18960;0.10258;0.00000;,
 3.35060;0.10264;3.31310;,
 3.35058;-2.12612;2.34272;,
 3.35058;2.33134;2.34272;,
 3.35058;3.25440;0.00000;,
 3.35058;2.33134;-2.34272;,
 3.35060;0.10264;-3.31310;,
 3.35058;-2.12612;-2.34272;,
 3.35058;-3.04922;-0.00000;,
 -16.16344;0.10258;9.70800;,
 -16.16342;-6.42790;6.86458;,
 -16.16344;6.63312;6.86458;,
 -16.16344;9.33812;0.00000;,
 -16.16344;6.63312;-6.86458;,
 -16.16344;0.10258;-9.70800;,
 -16.16342;-6.42790;-6.86458;,
 -16.16344;-9.13292;0.00000;,
 1.06994;-5.13424;-0.00000;,
 1.06996;-3.60040;3.89248;,
 1.06994;-3.60044;-3.89248;,
 1.06994;0.10258;-5.50480;,
 1.06994;3.80562;-3.89248;,
 1.06994;5.33950;0.00000;,
 1.06994;3.80562;3.89248;,
 1.06994;0.10258;5.50480;,
 -23.12458;0.10262;10.44610;,
 -23.12454;-6.92440;7.38652;,
 -23.12454;7.12964;7.38652;,
 -23.12454;10.04030;0.00000;,
 -23.12454;7.12964;-7.38652;,
 -23.12458;0.10262;-10.44610;,
 -23.12454;-6.92440;-7.38652;,
 -23.12456;-9.83510;0.00000;,
 -23.12454;-6.92440;7.38652;,
 -23.12458;0.10262;10.44610;,
 -23.12454;7.12964;7.38652;,
 -23.12454;10.04030;0.00000;,
 -23.12454;7.12964;-7.38652;,
 -23.12458;0.10262;-10.44610;,
 -23.12454;-6.92440;-7.38652;,
 -23.12456;-9.83510;0.00000;,
 -23.18718;3.38520;3.36748;,
 -23.18716;0.01770;4.76234;,
 -16.33716;0.01774;5.01298;,
 -16.33718;3.56248;3.54472;,
 -16.33716;0.01774;0.00000;,
 -23.18714;-3.34980;3.36748;,
 -16.33714;-3.52700;3.54472;,
 -23.18714;-4.74462;0.00000;,
 -16.33714;-4.99524;0.00000;,
 -23.18714;-3.34980;-3.36748;,
 -16.33714;-3.52700;-3.54472;,
 -23.18716;0.01770;-4.76234;,
 -16.33716;0.01774;-5.01298;,
 -23.18718;3.38520;-3.36748;,
 -16.33718;3.56248;-3.54472;,
 -23.18718;4.78004;0.00000;,
 -16.33718;5.03070;0.00000;,
 -20.81740;0.01772;0.00000;;
 
 88;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 3;16,17,18;,
 3;16,18,19;,
 3;16,19,20;,
 3;16,20,21;,
 3;16,21,22;,
 3;16,22,23;,
 3;16,23,24;,
 3;16,24,17;,
 3;25,26,27;,
 3;25,28,26;,
 3;25,29,28;,
 3;25,30,29;,
 3;25,31,30;,
 3;25,32,31;,
 3;25,33,32;,
 3;25,27,33;,
 4;3,2,34,35;,
 4;2,5,36,34;,
 4;5,7,37,36;,
 4;7,9,38,37;,
 4;9,11,39,38;,
 4;11,13,40,39;,
 4;13,15,41,40;,
 4;15,3,35,41;,
 4;42,43,0,14;,
 4;44,42,14,12;,
 4;45,44,12,10;,
 4;46,45,10,8;,
 4;47,46,8,6;,
 4;48,47,6,4;,
 4;49,48,4,1;,
 4;43,49,1,0;,
 4;35,34,50,51;,
 4;34,36,52,50;,
 4;36,37,53,52;,
 4;37,38,54,53;,
 4;38,39,55,54;,
 4;39,40,56,55;,
 4;40,41,57,56;,
 4;41,35,51,57;,
 4;18,17,58,59;,
 4;19,18,59,60;,
 4;20,19,60,61;,
 4;21,20,61,62;,
 4;22,21,62,63;,
 4;23,22,63,64;,
 4;24,23,64,65;,
 4;17,24,65,58;,
 4;27,26,49,43;,
 4;26,28,48,49;,
 4;28,29,47,48;,
 4;29,30,46,47;,
 4;30,31,45,46;,
 4;31,32,44,45;,
 4;32,33,42,44;,
 4;33,27,43,42;,
 4;66,67,68,69;,
 3;68,70,69;,
 4;67,71,72,68;,
 3;72,70,68;,
 4;71,73,74,72;,
 3;74,70,72;,
 4;73,75,76,74;,
 3;76,70,74;,
 4;75,77,78,76;,
 3;78,70,76;,
 4;77,79,80,78;,
 3;80,70,78;,
 4;79,81,82,80;,
 3;82,70,80;,
 4;81,66,69,82;,
 3;69,70,82;,
 3;81,79,83;,
 3;79,77,83;,
 3;77,75,83;,
 3;75,73,83;,
 3;83,73,71;,
 3;67,83,71;,
 3;66,83,67;,
 3;66,81,83;;
 
 MeshMaterialList {
  1;
  88;
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
  91;
  0.521407;-0.614981;0.591551;,
  0.530513;-0.000000;0.847677;,
  0.521408;0.614980;0.591552;,
  0.512628;0.858611;0.000000;,
  0.521408;0.614980;-0.591552;,
  0.530513;-0.000000;-0.847677;,
  0.521407;-0.614982;-0.591550;,
  0.512625;-0.858613;0.000001;,
  1.000000;-0.000000;-0.000000;,
  0.188025;-0.706790;0.681978;,
  0.192653;-0.000001;0.981267;,
  0.188021;0.706788;0.681981;,
  0.183599;0.983001;0.000000;,
  0.188021;0.706788;-0.681981;,
  0.192653;-0.000001;-0.981267;,
  0.188025;-0.706790;-0.681978;,
  0.183604;-0.983000;-0.000000;,
  0.269115;-0.693246;0.668570;,
  0.263057;-0.964780;0.000001;,
  0.269114;-0.693247;-0.668570;,
  0.275446;-0.000002;-0.961316;,
  0.269114;0.693244;-0.668574;,
  0.263056;0.964781;0.000000;,
  0.269114;0.693244;0.668574;,
  0.275447;-0.000002;0.961316;,
  0.137149;-0.712757;0.687872;,
  0.140581;-0.000000;0.990069;,
  0.137150;0.712756;0.687873;,
  0.133876;0.990998;0.000000;,
  0.137150;0.712756;-0.687873;,
  0.140581;-0.000000;-0.990069;,
  0.137149;-0.712757;-0.687872;,
  0.133875;-0.990998;-0.000000;,
  0.102841;-0.715700;0.690794;,
  0.100362;-0.994951;-0.000000;,
  0.102841;-0.715700;-0.690794;,
  0.105442;-0.000001;-0.994425;,
  0.102844;0.715701;-0.690794;,
  0.100363;0.994951;0.000000;,
  0.102844;0.715701;0.690794;,
  0.105442;-0.000001;0.994425;,
  -0.820495;-0.000001;-0.571654;,
  -0.814246;0.418705;-0.402105;,
  -1.000000;-0.000381;0.000000;,
  -0.814351;-0.419056;-0.401528;,
  -0.808328;-0.588733;0.000000;,
  -0.814351;-0.419056;0.401528;,
  -0.820495;-0.000001;0.571654;,
  -0.814246;0.418705;0.402105;,
  -0.808595;0.588366;0.000000;,
  0.862544;-0.365475;0.349922;,
  0.867449;0.000002;0.497527;,
  0.862544;0.365476;0.349921;,
  0.857720;0.514116;0.000000;,
  0.862544;0.365476;-0.349921;,
  0.867448;0.000002;-0.497527;,
  0.862544;-0.365475;-0.349922;,
  0.857720;-0.514117;0.000001;,
  -0.619474;0.566032;-0.543931;,
  -0.629469;0.000001;-0.777026;,
  -0.619820;-0.565980;-0.543590;,
  -0.610432;-0.792068;0.000000;,
  -0.619820;-0.565980;0.543590;,
  -0.629469;0.000001;0.777026;,
  -0.619474;0.566032;0.543931;,
  -0.609966;0.792427;0.000000;,
  -0.036570;0.706636;0.706632;,
  -0.036566;-0.000000;0.999331;,
  -0.036563;-0.706636;0.706632;,
  -0.036562;-0.999331;0.000000;,
  -0.036563;-0.706636;-0.706632;,
  -0.036566;-0.000000;-0.999331;,
  -0.036570;0.706636;-0.706632;,
  -0.036571;0.999331;0.000000;,
  1.000000;0.000005;0.000000;,
  -1.000000;-0.000005;0.000000;,
  1.000000;0.000006;0.000000;,
  1.000000;0.000005;0.000001;,
  1.000000;0.000005;-0.000001;,
  1.000000;0.000004;0.000000;,
  1.000000;0.000005;0.000001;,
  1.000000;0.000005;-0.000001;,
  1.000000;0.000004;0.000000;,
  -0.895281;-0.445501;0.000000;,
  -0.895282;-0.315018;0.315014;,
  -0.895284;-0.000005;0.445497;,
  -0.895286;0.315010;0.315011;,
  -0.895285;0.445494;0.000000;,
  -0.895286;0.315010;-0.315011;,
  -0.895284;-0.000005;-0.445497;,
  -0.895282;-0.315018;-0.315014;;
  88;
  4;17,24,10,9;,
  4;24,23,11,10;,
  4;23,22,12,11;,
  4;22,21,13,12;,
  4;21,20,14,13;,
  4;20,19,15,14;,
  4;19,18,16,15;,
  4;18,17,9,16;,
  3;43,42,41;,
  3;43,41,44;,
  3;43,44,45;,
  3;43,45,46;,
  3;43,46,47;,
  3;43,47,48;,
  3;43,48,49;,
  3;43,49,42;,
  3;8,51,50;,
  3;8,52,51;,
  3;8,53,52;,
  3;8,54,53;,
  3;8,55,54;,
  3;8,56,55;,
  3;8,57,56;,
  3;8,50,57;,
  4;9,10,26,25;,
  4;10,11,27,26;,
  4;11,12,28,27;,
  4;12,13,29,28;,
  4;13,14,30,29;,
  4;14,15,31,30;,
  4;15,16,32,31;,
  4;16,9,25,32;,
  4;7,0,17,18;,
  4;6,7,18,19;,
  4;5,6,19,20;,
  4;4,5,20,21;,
  4;3,4,21,22;,
  4;2,3,22,23;,
  4;1,2,23,24;,
  4;0,1,24,17;,
  4;25,26,40,33;,
  4;26,27,39,40;,
  4;27,28,38,39;,
  4;28,29,37,38;,
  4;29,30,36,37;,
  4;30,31,35,36;,
  4;31,32,34,35;,
  4;32,25,33,34;,
  4;41,42,58,59;,
  4;44,41,59,60;,
  4;45,44,60,61;,
  4;46,45,61,62;,
  4;47,46,62,63;,
  4;48,47,63,64;,
  4;49,48,64,65;,
  4;42,49,65,58;,
  4;50,51,1,0;,
  4;51,52,2,1;,
  4;52,53,3,2;,
  4;53,54,4,3;,
  4;54,55,5,4;,
  4;55,56,6,5;,
  4;56,57,7,6;,
  4;57,50,0,7;,
  4;66,67,67,66;,
  3;76,74,77;,
  4;67,68,68,67;,
  3;78,74,76;,
  4;68,69,69,68;,
  3;79,74,78;,
  4;69,70,70,69;,
  3;80,74,79;,
  4;70,71,71,70;,
  3;76,74,80;,
  4;71,72,72,71;,
  3;81,74,76;,
  4;72,73,73,72;,
  3;82,74,81;,
  4;73,66,66,73;,
  3;77,74,82;,
  3;83,84,75;,
  3;84,85,75;,
  3;85,86,75;,
  3;86,87,75;,
  3;75,87,88;,
  3;89,75,88;,
  3;90,75,89;,
  3;90,83,75;;
 }
 MeshTextureCoords {
  84;
  0.836660;0.572960;,
  0.836600;0.608800;,
  0.881710;0.608870;,
  0.881780;0.566940;,
  0.836540;0.644630;,
  0.881640;0.650790;,
  0.836520;0.659470;,
  0.881610;0.668160;,
  0.836540;0.644630;,
  0.881640;0.650790;,
  0.836600;0.608800;,
  0.881710;0.608870;,
  0.836660;0.572960;,
  0.881780;0.566940;,
  0.836680;0.558120;,
  0.881810;0.549580;,
  0.719240;0.152010;,
  0.739340;0.168570;,
  0.747670;0.152010;,
  0.739340;0.135440;,
  0.719240;0.128580;,
  0.699130;0.135440;,
  0.690800;0.152010;,
  0.699130;0.168570;,
  0.719240;0.175420;,
  0.765610;0.608680;,
  0.771710;0.608690;,
  0.771740;0.592480;,
  0.771690;0.624900;,
  0.771680;0.631620;,
  0.771690;0.624900;,
  0.771710;0.608690;,
  0.771740;0.592480;,
  0.771750;0.585770;,
  0.928200;0.608940;,
  0.928270;0.561440;,
  0.928120;0.656440;,
  0.928090;0.676120;,
  0.928120;0.656440;,
  0.928200;0.608940;,
  0.928270;0.561440;,
  0.928300;0.541770;,
  0.788370;0.570630;,
  0.788350;0.581780;,
  0.788350;0.581780;,
  0.788300;0.608720;,
  0.788260;0.635650;,
  0.788240;0.646810;,
  0.788260;0.635650;,
  0.788300;0.608720;,
  0.984650;0.609040;,
  0.984730;0.557920;,
  0.984560;0.660150;,
  0.984530;0.681320;,
  0.984560;0.660150;,
  0.984650;0.609040;,
  0.984730;0.557920;,
  0.984760;0.536750;,
  0.739820;0.176720;,
  0.748350;0.159760;,
  0.739820;0.142800;,
  0.719240;0.135770;,
  0.698660;0.142800;,
  0.690130;0.159760;,
  0.698660;0.176720;,
  0.719240;0.183750;,
  0.794890;0.322910;,
  0.801370;0.330170;,
  0.801370;0.330170;,
  0.794890;0.322910;,
  0.801370;0.330170;,
  0.807860;0.337440;,
  0.807860;0.337440;,
  0.810540;0.340450;,
  0.810540;0.340450;,
  0.807860;0.337440;,
  0.807860;0.337440;,
  0.801370;0.330170;,
  0.801370;0.330170;,
  0.794890;0.322910;,
  0.794890;0.322910;,
  0.792200;0.319900;,
  0.792200;0.319900;,
  0.801370;0.330180;;
 }
}
