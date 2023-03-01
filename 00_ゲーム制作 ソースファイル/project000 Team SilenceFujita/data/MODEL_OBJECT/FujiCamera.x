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
 315;
 -740.00842;-0.00620;-500.00842;,
 -340.00784;-0.00620;-500.00842;,
 -340.00784;-0.00620;-100.00845;,
 -740.00842;-0.00620;-100.00845;,
 -740.00000;164.00000;-500.00000;,
 -339.99948;164.00000;-500.00000;,
 -340.00784;-0.00620;-500.00842;,
 -740.00842;-0.00620;-500.00842;,
 -740.00000;164.00000;-500.00000;,
 -740.00842;-0.00620;-500.00842;,
 -740.00842;-0.00620;-100.00845;,
 -740.00000;164.00000;-100.00006;,
 -740.00000;163.98000;500.00000;,
 -740.00842;-0.02620;499.99158;,
 659.99158;-0.02620;499.99158;,
 660.00000;163.98000;500.00000;,
 660.00000;163.98000;-500.00000;,
 659.99158;-0.02620;-500.00842;,
 659.99158;-0.02620;499.99158;,
 660.00000;163.98000;500.00000;,
 719.99158;-0.02620;499.99158;,
 720.00000;163.98000;500.00000;,
 719.99158;-0.02620;-500.00842;,
 719.99158;-0.02620;499.99158;,
 659.99158;-0.02620;499.99158;,
 659.99158;-0.02620;-500.00842;,
 720.00000;163.98000;-500.00000;,
 719.99158;-0.02620;-500.00842;,
 659.99158;-0.02620;-500.00842;,
 660.00000;163.98000;-500.00000;,
 720.00000;163.98000;-500.00000;,
 720.00000;163.98000;500.00000;,
 719.99158;-0.02620;499.99158;,
 719.99158;-0.02620;-500.00842;,
 660.00000;-0.02000;440.00000;,
 660.00842;163.98621;440.00842;,
 -740.00000;-0.02000;440.00000;,
 660.00000;-0.02000;440.00000;,
 -740.00842;-0.02620;499.99158;,
 -739.99158;163.98621;440.00842;,
 -740.00000;-0.02000;440.00000;,
 -740.00000;488.00000;-100.00006;,
 -740.00000;600.00000;-100.00006;,
 -740.00000;600.00000;-500.00000;,
 -740.00000;488.00000;-500.00000;,
 -740.00000;488.00000;-100.00006;,
 -740.00000;488.00000;-500.00000;,
 -740.00000;164.00000;-500.00000;,
 -740.00000;164.00000;-100.00006;,
 -740.00000;488.00000;-500.00000;,
 -740.00000;600.00000;-500.00000;,
 -339.99948;600.00000;-500.00000;,
 -339.99948;488.00000;-500.00000;,
 720.00000;600.00000;500.00000;,
 720.00000;600.00000;-500.00000;,
 -740.00000;488.00000;26.75693;,
 -740.00000;488.00000;-100.00006;,
 -740.00000;163.99580;26.75693;,
 -740.00000;488.00000;241.83160;,
 -740.00000;488.00000;159.25159;,
 -740.00000;163.99139;159.25159;,
 -740.00000;163.98860;241.83160;,
 506.76236;487.99561;-500.00000;,
 506.76236;163.98300;-500.00000;,
 229.71973;163.98860;-500.00000;,
 229.71973;487.99701;-500.00000;,
 -740.00000;488.00000;408.05737;,
 -740.00000;163.98300;408.05737;,
 -740.00000;163.98000;500.00000;,
 -740.00000;599.97998;500.00000;,
 506.76236;487.99561;-500.00000;,
 720.00000;600.00000;-500.00000;,
 506.76236;163.98300;-500.00000;,
 229.71973;163.98860;-500.00000;,
 229.71973;487.99701;-500.00000;,
 -740.00000;488.00000;-500.00000;,
 -740.00000;164.00000;-500.00000;,
 -339.99948;164.00000;-500.00000;,
 -339.99948;488.00000;-500.00000;,
 229.71973;163.98860;-500.00000;,
 229.71973;487.99701;-500.00000;,
 -740.00000;600.00000;-500.00000;,
 -740.00000;600.00000;-100.00006;,
 -740.00000;488.00000;-500.00000;,
 -740.00000;600.00000;-500.00000;,
 -740.00000;488.00000;-500.00000;,
 -339.99948;488.00000;-500.00000;,
 -339.99948;600.00000;-500.00000;,
 720.00000;163.98000;-500.00000;,
 660.00000;163.98000;-500.00000;,
 660.00000;163.98000;500.00000;,
 720.00000;163.98000;500.00000;,
 -740.00000;164.00000;-500.00000;,
 -339.99948;164.00000;-500.00000;,
 -339.99948;164.00000;-100.00006;,
 -740.00000;164.00000;-100.00006;,
 -740.00842;-0.00620;-100.00845;,
 -340.00784;-0.00620;-100.00845;,
 -339.99948;164.00000;-100.00006;,
 -340.00784;-0.00620;-100.00845;,
 -340.00784;-0.00620;-500.00842;,
 -339.99948;164.00000;-500.00000;,
 -740.00000;164.00000;-500.00000;,
 -740.00000;164.00000;-100.00006;,
 -339.99948;164.00000;-100.00006;,
 -339.99948;164.00000;-500.00000;,
 -740.00000;488.00000;241.83160;,
 -740.00000;488.00000;408.05737;,
 -740.00000;163.98300;408.05737;,
 -740.00000;163.98860;241.83160;,
 -740.00000;163.99139;159.25159;,
 -740.00000;163.99580;26.75693;,
 -740.00000;488.00000;26.75693;,
 -740.00000;488.00000;159.25159;,
 720.00000;600.00000;500.00000;,
 -740.00000;599.97998;500.00000;,
 -740.00000;163.98000;500.00000;,
 720.00000;600.00000;-500.00000;,
 -740.00000;600.00000;-500.00000;,
 -740.00000;599.97998;500.00000;,
 720.00000;600.00000;500.00000;,
 -839.99683;0.00940;-559.99731;,
 -739.99683;0.00940;-559.99731;,
 -740.00098;0.00940;-499.99719;,
 -840.00098;0.00940;-499.99719;,
 -239.99954;702.03998;-560.00012;,
 -339.99527;600.00000;-560.00012;,
 -740.00000;600.00000;-560.00012;,
 -840.00000;702.03998;-560.00012;,
 -240.00394;702.03998;-500.00000;,
 -840.00415;702.03998;-500.00000;,
 -339.99948;600.00000;-500.00000;,
 -740.00415;600.00000;-500.00000;,
 -740.00000;164.00000;-560.00012;,
 -740.00415;164.00000;-500.00000;,
 -739.99683;0.00940;-559.99731;,
 -740.00000;244.00000;-560.00012;,
 -740.00000;164.00000;-560.00012;,
 -839.99683;0.00940;-559.99731;,
 -840.00000;702.03998;-560.00012;,
 -740.00000;244.00000;-560.00012;,
 -740.00415;244.00000;-500.00000;,
 -840.00000;702.03998;-560.00012;,
 -839.99683;0.00940;-559.99731;,
 -840.00098;0.00940;-499.99719;,
 -840.00415;702.03998;-500.00000;,
 -740.00415;244.00000;-500.00000;,
 -840.00415;702.03998;-500.00000;,
 -840.00098;0.00940;-499.99719;,
 -740.00415;164.00000;-500.00000;,
 -339.99625;0.00940;-559.99731;,
 -239.99631;0.00940;-559.99731;,
 -240.00070;0.00940;-499.99719;,
 -340.00046;0.00940;-499.99719;,
 -339.99948;600.00000;-500.00000;,
 -240.00394;702.03998;-500.00000;,
 -239.99954;702.03998;-560.00012;,
 -339.99527;600.00000;-560.00012;,
 -339.99948;164.00000;-560.00012;,
 -339.99625;0.00940;-559.99731;,
 -340.00363;164.00000;-500.00000;,
 -339.99948;244.00000;-560.00012;,
 -239.99954;702.03998;-560.00012;,
 -239.99631;0.00940;-559.99731;,
 -339.99948;164.00000;-560.00012;,
 -339.99948;244.00000;-560.00012;,
 -340.00363;244.00000;-500.00000;,
 -239.99954;702.03998;-560.00012;,
 -240.00394;702.03998;-500.00000;,
 -240.00070;0.00940;-499.99719;,
 -239.99631;0.00940;-559.99731;,
 -340.00363;244.00000;-500.00000;,
 -340.00363;164.00000;-500.00000;,
 -240.00070;0.00940;-499.99719;,
 -240.00394;702.03998;-500.00000;,
 -740.00415;164.00000;-500.00000;,
 -740.00000;164.00000;-560.00012;,
 -740.00415;244.00000;-500.00000;,
 -740.00000;244.00000;-560.00012;,
 -339.99948;244.00000;-604.31653;,
 -740.00000;244.00000;-604.31653;,
 -740.00000;164.00000;-604.31653;,
 -339.99948;164.00000;-604.31653;,
 -740.00000;164.00000;-604.31653;,
 -740.00000;244.00000;-604.31653;,
 -339.99948;244.00000;-604.31653;,
 -339.99948;164.00000;-604.31653;,
 -840.00000;164.00000;-604.31653;,
 -880.00000;244.00000;-604.31653;,
 -840.00000;164.00000;-560.00012;,
 -880.00000;244.00000;-560.00012;,
 -199.99951;244.00000;-604.31653;,
 -239.99954;164.00000;-604.31653;,
 -199.99951;244.00000;-560.00012;,
 -239.99954;164.00000;-560.00012;,
 -840.00098;0.00940;-499.99719;,
 -740.00098;0.00940;-499.99719;,
 -839.99683;0.00940;-559.99731;,
 -739.99683;0.00940;-559.99731;,
 -240.00070;0.00940;-499.99719;,
 -340.00046;0.00940;-499.99719;,
 -239.99631;0.00940;-559.99731;,
 -339.99625;0.00940;-559.99731;,
 -240.00394;702.03998;-500.00000;,
 -339.99948;600.00000;-500.00000;,
 -239.99954;702.03998;-560.00012;,
 -339.99527;600.00000;-560.00012;,
 -840.00415;702.03998;-500.00000;,
 -740.00415;600.00000;-500.00000;,
 -840.00000;702.03998;-560.00012;,
 -740.00000;600.00000;-560.00012;,
 -74.90732;163.98621;-499.99158;,
 -74.91571;-0.02000;-500.00000;,
 -74.91193;-0.01380;-459.99170;,
 -74.90353;163.99240;-459.98328;,
 -34.91190;-0.01380;-459.99170;,
 -34.90350;163.99240;-459.98328;,
 -74.90353;163.99240;-459.98328;,
 -74.91193;-0.01380;-459.99170;,
 -34.91570;-0.02000;-500.00000;,
 -34.91190;-0.01380;-459.99170;,
 -74.91193;-0.01380;-459.99170;,
 -74.91571;-0.02000;-500.00000;,
 -34.90730;163.98621;-499.99158;,
 -34.91570;-0.02000;-500.00000;,
 -74.91571;-0.02000;-500.00000;,
 -74.90732;163.98621;-499.99158;,
 -34.90730;163.98621;-499.99158;,
 -34.90350;163.99240;-459.98328;,
 -34.91190;-0.01380;-459.99170;,
 -34.91570;-0.02000;-500.00000;,
 452.48111;163.98621;-499.99158;,
 452.47293;-0.02000;-500.00000;,
 452.47672;-0.01380;-459.99170;,
 452.48514;163.99240;-459.98328;,
 492.47659;-0.01380;-459.99170;,
 492.48502;163.99240;-459.98328;,
 452.48514;163.99240;-459.98328;,
 452.47672;-0.01380;-459.99170;,
 492.47281;-0.02000;-500.00000;,
 492.47659;-0.01380;-459.99170;,
 452.47672;-0.01380;-459.99170;,
 452.47293;-0.02000;-500.00000;,
 492.48099;163.98621;-499.99158;,
 492.47281;-0.02000;-500.00000;,
 452.47293;-0.02000;-500.00000;,
 452.48111;163.98621;-499.99158;,
 492.48099;163.98621;-499.99158;,
 492.48502;163.99240;-459.98328;,
 492.47659;-0.01380;-459.99170;,
 492.47281;-0.02000;-500.00000;,
 -739.99158;163.98621;178.62939;,
 -740.00000;-0.02000;178.62097;,
 -739.99622;-0.01380;218.62958;,
 -739.98779;163.99240;218.63800;,
 -699.99622;-0.01380;218.62958;,
 -699.98779;163.99240;218.63800;,
 -739.98779;163.99240;218.63800;,
 -739.99622;-0.01380;218.62958;,
 -700.00000;-0.02000;178.62097;,
 -699.99622;-0.01380;218.62958;,
 -739.99622;-0.01380;218.62958;,
 -740.00000;-0.02000;178.62097;,
 -699.99158;163.98621;178.62939;,
 -700.00000;-0.02000;178.62097;,
 -740.00000;-0.02000;178.62097;,
 -739.99158;163.98621;178.62939;,
 -699.99158;163.98621;178.62939;,
 -699.98779;163.99240;218.63800;,
 -699.99622;-0.01380;218.62958;,
 -700.00000;-0.02000;178.62097;,
 -739.99158;163.98621;178.62939;,
 -740.00000;-0.02000;178.62097;,
 -739.99622;-0.01380;218.62958;,
 -739.98779;163.99240;218.63800;,
 -699.99622;-0.01380;218.62958;,
 -699.98779;163.99240;218.63800;,
 -700.00000;-0.02000;178.62097;,
 -699.99158;163.98621;178.62939;,
 460.00021;860.00000;-500.00000;,
 720.00000;860.00000;-500.00000;,
 720.00000;600.00378;-500.00000;,
 460.00021;600.00378;-500.00000;,
 720.00000;860.00000;-500.00000;,
 720.00000;860.00000;-240.00006;,
 720.00000;600.00378;-240.00006;,
 720.00000;600.00378;-500.00000;,
 720.00000;860.00000;-240.00006;,
 460.00021;860.00000;-240.00006;,
 460.00021;600.00378;-240.00006;,
 720.00000;600.00378;-240.00006;,
 460.00021;860.00000;-240.00006;,
 460.00021;860.00000;-500.00000;,
 460.00021;600.00378;-500.00000;,
 460.00021;600.00378;-240.00006;,
 460.00021;860.00000;-240.00006;,
 720.00000;860.00000;-240.00006;,
 720.00000;860.00000;-500.00000;,
 460.00021;860.00000;-500.00000;,
 460.00021;600.00378;-500.00000;,
 720.00000;600.00378;-500.00000;,
 -740.00000;488.00000;-500.00000;,
 -339.99948;488.00000;-500.00000;,
 -339.99948;164.00000;-500.00000;,
 -740.00000;164.00000;-500.00000;,
 229.71973;487.99701;-500.00000;,
 229.71973;163.98860;-500.00000;,
 -740.00000;488.00000;159.25159;,
 -740.00000;488.00000;26.75693;,
 -740.00000;163.99580;26.75693;,
 -740.00000;163.99139;159.25159;,
 -740.00000;488.00000;408.05737;,
 -740.00000;488.00000;241.83160;,
 -740.00000;163.98860;241.83160;,
 -740.00000;163.98300;408.05737;;
 
 130;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,15,14;,
 4;22,23,24,25;,
 4;26,27,28,29;,
 4;30,31,32,33;,
 4;34,35,15,14;,
 4;36,37,24,38;,
 4;39,40,13,12;,
 4;35,39,12,15;,
 4;35,34,40,39;,
 4;41,42,43,44;,
 4;45,46,47,48;,
 4;49,50,51,52;,
 4;53,31,30,54;,
 4;55,56,11,57;,
 4;58,59,60,61;,
 4;62,63,64,65;,
 4;66,67,68,69;,
 4;70,71,26,72;,
 4;72,26,71,70;,
 4;73,72,70,74;,
 4;75,76,77,78;,
 4;77,79,80,78;,
 4;81,82,56,83;,
 4;84,85,86,87;,
 4;30,31,53,54;,
 4;88,89,90,91;,
 4;92,93,94,95;,
 4;95,96,97,94;,
 4;98,99,100,101;,
 4;102,103,104,105;,
 4;57,8,83,55;,
 4;59,58,61,60;,
 4;66,69,68,67;,
 4;106,107,108,109;,
 4;110,111,112,113;,
 4;114,115,12,21;,
 4;21,12,115,114;,
 4;102,116,91,89;,
 4;89,91,116,102;,
 4;117,118,119,120;,
 4;120,119,118,117;,
 4;82,56,66,69;,
 4;71,70,86,87;,
 4;121,122,123,124;,
 4;125,126,127,128;,
 4;129,125,128,130;,
 4;131,129,130,132;,
 4;126,131,132,127;,
 4;126,125,129,131;,
 4;133,134,123,135;,
 4;136,137,138,139;,
 4;140,127,132,141;,
 4;140,141,134,133;,
 4;142,143,144,145;,
 4;146,147,148,149;,
 4;150,151,152,153;,
 4;154,155,156,157;,
 4;158,159,153,160;,
 4;161,162,163,164;,
 4;165,166,154,157;,
 4;167,168,169,170;,
 4;171,172,173,174;,
 4;175,176,158,160;,
 4;177,175,160,166;,
 4;178,177,166,165;,
 4;178,176,175,177;,
 4;179,180,178,165;,
 4;181,182,158,176;,
 4;183,184,185,186;,
 4;187,188,180,181;,
 4;189,187,181,176;,
 4;190,189,176,178;,
 4;188,190,178,180;,
 4;188,187,189,190;,
 4;191,192,182,179;,
 4;193,191,179,165;,
 4;194,193,165,158;,
 4;192,194,158,182;,
 4;192,191,193,194;,
 3;195,196,149;,
 3;197,137,198;,
 3;199,172,200;,
 3;201,202,164;,
 3;203,204,171;,
 3;205,161,206;,
 3;207,146,208;,
 3;209,210,136;,
 4;211,212,213,214;,
 4;215,216,217,218;,
 4;219,220,221,222;,
 4;223,224,225,226;,
 4;227,228,229,230;,
 4;223,226,217,216;,
 4;231,232,233,234;,
 4;235,236,237,238;,
 4;239,240,241,242;,
 4;243,244,245,246;,
 4;247,248,249,250;,
 4;243,246,237,236;,
 4;251,252,253,254;,
 4;255,256,257,258;,
 4;259,260,261,262;,
 4;263,264,265,266;,
 4;267,268,269,270;,
 4;263,266,257,256;,
 4;271,272,273,274;,
 4;275,276,274,273;,
 4;277,275,273,272;,
 4;278,277,272,271;,
 4;278,276,275,277;,
 4;278,271,274,276;,
 4;279,280,281,282;,
 4;283,284,285,286;,
 4;287,288,289,290;,
 4;291,292,293,294;,
 4;295,296,297,298;,
 4;299,300,285,294;,
 4;301,302,303,304;,
 4;305,306,303,302;,
 4;307,308,309,310;,
 4;311,312,313,314;,
 4;304,303,302,301;,
 4;302,303,306,305;,
 4;310,309,308,307;,
 4;314,313,312,311;;
 
 MeshMaterialList {
  14;
  130;
  4,
  11,
  3,
  2,
  3,
  2,
  4,
  2,
  3,
  2,
  4,
  2,
  2,
  2,
  7,
  10,
  6,
  3,
  3,
  3,
  9,
  3,
  2,
  2,
  2,
  0,
  0,
  3,
  2,
  3,
  4,
  2,
  2,
  3,
  4,
  3,
  3,
  3,
  1,
  1,
  2,
  2,
  4,
  4,
  4,
  4,
  3,
  2,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  8,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  12,
  3,
  2,
  4,
  2,
  3,
  2,
  3,
  2,
  4,
  2,
  3,
  2,
  3,
  2,
  4,
  2,
  3,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  5,
  5,
  5,
  5,
  4,
  13,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;0.800000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\Glass.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;0.800000;;
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
    "data\\TEXTURE\\wall.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\wall.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\wall.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\FujiCamera.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\FujiCamerayoko.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\FujiCamerayoko.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\FujiCameraSolo.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\kadenLogo.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\kadenLogo.png";
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
   0.800000;0.062400;0.037600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  90;
  -1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  -1.000000;0.000051;0.000000;,
  0.000000;-0.000051;1.000000;,
  0.000000;-0.000051;1.000000;,
  0.000000;0.000051;-1.000000;,
  -1.000000;0.000017;0.000000;,
  0.000000;-0.000026;1.000000;,
  0.000000;-0.000051;1.000000;,
  0.000000;0.000026;-1.000000;,
  0.000000;-0.000026;1.000000;,
  1.000000;-0.000051;0.000140;,
  0.000000;-1.000000;-0.000103;,
  -1.000000;0.000051;-0.000140;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000026;0.000000;,
  -1.000000;0.000051;0.000000;,
  0.000000;-1.000000;-0.000052;,
  1.000000;-0.000026;-0.000000;,
  1.000000;-0.000051;-0.000000;,
  -1.000000;0.000026;-0.000070;,
  0.000000;1.000000;0.000104;,
  0.000004;1.000000;0.000057;,
  0.000000;1.000000;0.000052;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  0.000004;1.000000;0.000005;,
  -0.000004;-1.000000;-0.000005;,
  0.000000;-0.000051;1.000000;,
  1.000000;-0.000051;-0.000000;,
  -0.000007;-1.000000;-0.000010;,
  -0.000007;1.000000;0.000010;,
  0.000007;-1.000000;-0.000010;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  1.000000;0.000010;0.000069;,
  -0.000012;-0.000001;-1.000000;,
  -0.000008;-0.000001;-1.000000;,
  1.000000;0.000000;0.000069;,
  0.000000;1.000000;0.000000;,
  -1.000000;-0.000020;-0.000070;,
  0.000012;-0.000001;-1.000000;,
  0.000008;-0.000001;-1.000000;,
  -1.000000;0.000012;-0.000070;,
  -0.387353;0.921932;-0.000028;,
  -0.000008;0.000001;1.000000;,
  0.000008;0.000001;1.000000;,
  0.387353;-0.921932;0.000028;,
  0.925805;-0.378002;0.000067;,
  1.000000;0.000019;0.000069;,
  -0.000012;-0.000010;-1.000000;,
  -1.000000;-0.000005;-0.000069;,
  0.000012;0.000001;1.000000;,
  0.000012;0.000010;1.000000;,
  -0.925802;0.378010;-0.000065;,
  0.000012;-0.000010;-1.000000;,
  1.000000;0.000005;0.000073;,
  -0.000008;0.000006;1.000000;,
  -0.000012;0.000010;1.000000;,
  -1.000000;0.000000;-0.000069;,
  -0.894427;-0.447214;0.000000;,
  0.894427;-0.447214;-0.000000;,
  0.000000;0.000017;1.000000;,
  0.000000;-0.000017;-1.000000;,
  0.000000;0.000017;1.000000;,
  -1.000000;0.000051;0.000095;,
  0.000000;-1.000000;0.000155;,
  0.000000;-0.000051;1.000000;,
  0.000000;0.000051;-1.000000;,
  -1.000000;0.000051;0.000098;,
  0.000000;0.000051;-1.000000;,
  -1.000000;0.000051;0.000095;,
  0.000000;-1.000000;0.000155;,
  1.000000;-0.000051;-0.000095;,
  0.000000;1.000000;-0.000155;,
  1.000000;-0.000051;-0.000098;,
  1.000000;-0.000051;-0.000095;,
  0.000000;1.000000;-0.000155;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  1.000000;0.000000;0.000000;;
  130;
  4;1,1,1,1;,
  4;6,6,6,6;,
  4;16,17,17,7;,
  4;8,5,4,4;,
  4;3,3,3,3;,
  4;9,11,4,4;,
  4;1,1,18,1;,
  4;10,6,6,6;,
  4;19,19,20,20;,
  4;12,12,12,12;,
  4;13,13,18,13;,
  4;14,14,14,21;,
  4;22,22,23,24;,
  4;6,6,6,6;,
  4;0,0,0,0;,
  4;0,0,16,7;,
  4;2,2,2,2;,
  4;15,19,19,15;,
  4;0,0,7,0;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;0,0,21,0;,
  4;2,2,10,2;,
  4;25,25,25,25;,
  4;25,25,25,25;,
  4;25,25,25,25;,
  4;25,25,25,25;,
  4;15,15,15,15;,
  4;25,25,25,25;,
  4;0,0,0,0;,
  4;26,27,24,27;,
  4;28,1,1,1;,
  4;29,29,29,29;,
  4;30,30,30,30;,
  4;27,26,26,26;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;15,15,15,15;,
  4;25,25,8,11;,
  4;2,2,2,2;,
  4;27,23,27,27;,
  4;31,31,31,28;,
  4;32,32,32,32;,
  4;33,33,33,33;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;34,34,34,34;,
  4;44,35,35,39;,
  4;46,46,41,41;,
  4;36,47,48,36;,
  4;49,49,34,34;,
  4;49,50,50,49;,
  4;37,37,51,51;,
  4;38,52,52,39;,
  4;40,40,40,40;,
  4;40,40,37,37;,
  4;53,53,53,53;,
  4;54,48,55,55;,
  4;34,34,34,34;,
  4;56,46,46,56;,
  4;42,42,42,42;,
  4;43,44,57,57;,
  4;45,45,56,56;,
  4;50,50,58,58;,
  4;47,59,60,47;,
  4;34,34,34,34;,
  4;36,36,59,47;,
  4;41,41,41,41;,
  4;61,61,61,61;,
  4;41,41,41,41;,
  4;34,34,34,34;,
  4;35,35,35,35;,
  4;35,35,35,35;,
  4;34,34,34,34;,
  4;36,36,36,36;,
  4;41,41,41,41;,
  4;62,62,62,62;,
  4;35,35,35,35;,
  4;41,41,41,41;,
  4;36,36,36,36;,
  4;34,34,34,34;,
  4;63,63,63,63;,
  3;55,64,55;,
  3;52,52,65;,
  3;60,59,66;,
  3;57,65,57;,
  3;47,36,47;,
  3;44,43,35;,
  3;48,54,36;,
  3;39,35,38;,
  4;67,67,67,67;,
  4;69,69,69,69;,
  4;68,68,68,68;,
  4;70,70,70,70;,
  4;75,75,75,75;,
  4;76,76,76,76;,
  4;71,71,71,71;,
  4;69,69,69,69;,
  4;68,68,68,68;,
  4;72,72,72,72;,
  4;77,77,77,77;,
  4;76,76,76,76;,
  4;73,73,73,73;,
  4;69,69,69,69;,
  4;74,74,74,74;,
  4;72,72,72,72;,
  4;78,78,78,78;,
  4;79,79,79,79;,
  4;73,73,73,73;,
  4;69,69,69,69;,
  4;74,74,74,74;,
  4;72,72,72,72;,
  4;78,78,78,78;,
  4;79,79,79,79;,
  4;80,80,80,80;,
  4;81,81,81,81;,
  4;82,82,82,82;,
  4;83,83,83,83;,
  4;84,84,84,84;,
  4;85,85,85,85;,
  4;86,86,86,86;,
  4;86,86,86,86;,
  4;87,87,87,87;,
  4;87,87,87,87;,
  4;88,88,88,88;,
  4;88,88,88,88;,
  4;89,89,89,89;,
  4;89,89,89,89;;
 }
 MeshTextureCoords {
  315;
  -1.873451;-0.291173;,
  -0.291173;-0.291173;,
  -0.291173;1.291106;,
  -1.873451;1.291106;,
  0.007167;-0.027021;,
  2.017419;-0.027021;,
  2.017377;1.030647;,
  0.007124;1.030647;,
  -1.477848;1.150316;,
  -1.477881;1.799075;,
  0.104397;1.799075;,
  0.104430;1.150316;,
  -2.427169;1.150338;,
  -2.427202;1.799086;,
  3.110685;1.799086;,
  3.110718;1.150338;,
  -1.477848;1.150396;,
  -1.477881;1.799154;,
  2.477815;1.799154;,
  2.477848;1.150396;,
  3.348023;1.799086;,
  3.348056;1.150338;,
  3.901865;-0.291173;,
  3.901865;3.664524;,
  3.664524;3.664524;,
  3.664524;-0.291173;,
  3.348056;1.150338;,
  3.348023;1.799086;,
  3.110685;1.799086;,
  3.110718;1.150338;,
  -1.477848;1.150396;,
  2.477848;1.150396;,
  2.477815;1.799154;,
  -1.477881;1.799154;,
  3.110718;1.799062;,
  3.110751;1.150313;,
  -1.873418;3.427215;,
  3.664557;3.427215;,
  -1.873451;3.664524;,
  -2.427135;1.150313;,
  -2.427169;1.799062;,
  0.019216;0.907150;,
  0.019216;0.068500;,
  0.989732;0.068500;,
  0.989732;0.907150;,
  0.174396;0.005210;,
  0.825603;0.005210;,
  0.825603;1.003964;,
  0.174396;1.003964;,
  0.011475;0.905983;,
  0.011475;0.024232;,
  1.011475;0.024232;,
  1.011475;0.905983;,
  2.477848;-0.574367;,
  -1.477848;-0.574367;,
  0.605842;-0.131329;,
  0.104430;-0.131329;,
  0.605842;1.150333;,
  1.456612;-0.131329;,
  1.129951;-0.131329;,
  1.129951;1.150350;,
  1.456612;1.150362;,
  0.774319;0.004946;,
  0.774319;1.003752;,
  0.255823;1.003734;,
  0.255823;0.004941;,
  2.114151;-0.131329;,
  2.114151;1.150384;,
  2.477848;1.150396;,
  2.477848;-0.574288;,
  2.504566;-0.131349;,
  3.348056;-0.574398;,
  2.504566;1.150326;,
  1.408687;1.150304;,
  1.408687;-0.131355;,
  -4.772247;-0.000033;,
  -4.772247;2.654573;,
  -0.351576;2.654573;,
  -0.351576;-0.000033;,
  5.944777;2.654667;,
  5.944777;-0.000008;,
  -1.477848;-0.574367;,
  0.104430;-0.574367;,
  -1.477848;-0.131329;,
  -2.427169;-0.574398;,
  -2.427169;-0.131367;,
  -0.844913;-0.131367;,
  -0.844913;-0.574398;,
  3.901899;-0.291139;,
  3.664557;-0.291139;,
  3.664557;3.664557;,
  3.901899;3.664557;,
  -2.427169;1.150259;,
  -0.844913;1.150259;,
  -0.844913;1.150259;,
  -2.427169;1.150259;,
  -2.427202;1.799007;,
  -0.844946;1.799007;,
  0.104430;1.150316;,
  0.104397;1.799075;,
  -1.477881;1.799075;,
  -1.477848;1.150316;,
  -1.873418;-0.291139;,
  -1.873418;1.291139;,
  -0.291139;1.291139;,
  -0.291139;-0.291139;,
  1.965007;-0.026037;,
  5.084860;-0.026037;,
  5.084860;3.089391;,
  1.965007;3.089337;,
  0.415082;3.089310;,
  -2.071679;3.089268;,
  -2.071679;-0.026037;,
  0.415082;-0.026037;,
  3.348056;-0.574398;,
  -2.427169;-0.574318;,
  -1.873418;3.664557;,
  3.901899;-0.291139;,
  -1.873418;-0.291139;,
  -1.873418;3.664557;,
  3.901899;3.664557;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.124960;,
  0.000000;0.124960;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.124960;,
  1.000000;0.124960;,
  0.000000;0.729910;,
  1.000000;0.729910;,
  0.000000;1.000000;,
  1.000000;0.638690;,
  1.000000;0.729910;,
  0.000000;0.729910;,
  0.000000;0.638690;,
  0.000000;0.638690;,
  1.000000;0.638690;,
  1.000000;0.638690;,
  1.000000;0.729910;,
  0.000000;0.729910;,
  0.000000;0.638690;,
  0.000000;0.638690;,
  1.000000;0.638690;,
  1.000000;0.729910;,
  0.000000;0.729910;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.114140;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.114140;,
  1.000000;0.732150;,
  1.000000;1.000000;,
  0.000000;0.732150;,
  0.000000;0.636950;,
  1.000000;0.636950;,
  1.000000;0.732150;,
  0.000000;0.732150;,
  1.000000;0.636950;,
  0.000000;0.636950;,
  0.000000;0.636950;,
  1.000000;0.636950;,
  1.000000;0.732150;,
  0.000000;0.732150;,
  1.000000;0.636950;,
  1.000000;0.732150;,
  0.000000;0.732150;,
  0.000000;0.636950;,
  0.000000;0.732150;,
  1.000000;0.732150;,
  0.000000;0.636950;,
  1.000000;0.636950;,
  1.000000;0.636950;,
  1.000000;0.636950;,
  1.000000;0.732150;,
  1.000000;0.732150;,
  0.097649;0.634460;,
  0.097649;0.308398;,
  0.958679;0.308398;,
  0.958679;0.634460;,
  1.000000;0.732150;,
  1.000000;0.636950;,
  1.000000;0.732150;,
  1.000000;0.636950;,
  1.000000;0.636950;,
  1.000000;0.732150;,
  1.000000;0.636950;,
  1.000000;0.732150;,
  1.000000;0.864955;,
  0.000000;1.000000;,
  0.000000;0.864955;,
  1.000000;1.000000;,
  0.000000;0.866075;,
  1.000000;1.000000;,
  1.000000;0.866075;,
  0.000000;1.000000;,
  0.000000;0.375545;,
  1.000000;0.114140;,
  1.000000;0.375545;,
  0.000000;0.114140;,
  1.000000;0.381825;,
  0.000000;0.124960;,
  0.000000;0.381825;,
  1.000000;0.124960;,
  -1.477815;1.150371;,
  -1.477848;1.799130;,
  -1.319587;1.799105;,
  -1.319554;1.150347;,
  0.361901;1.799037;,
  0.361935;1.150289;,
  0.203709;1.150289;,
  0.203676;1.799037;,
  0.915680;-0.291139;,
  0.915695;-0.132878;,
  0.757467;-0.132878;,
  0.757452;-0.291139;,
  0.361920;1.150313;,
  0.361886;1.799062;,
  0.203661;1.799062;,
  0.203694;1.150313;,
  -1.477815;1.150371;,
  -1.319554;1.150347;,
  -1.319587;1.799105;,
  -1.477848;1.799130;,
  -1.477815;1.150371;,
  -1.477848;1.799130;,
  -1.319587;1.799105;,
  -1.319554;1.150347;,
  2.448057;1.799037;,
  2.448090;1.150289;,
  2.289865;1.150289;,
  2.289832;1.799037;,
  3.001869;-0.291139;,
  3.001884;-0.132878;,
  2.843657;-0.132878;,
  2.843642;-0.291139;,
  2.448074;1.150313;,
  2.448042;1.799062;,
  2.289817;1.799062;,
  2.289849;1.150313;,
  -1.477815;1.150371;,
  -1.319554;1.150347;,
  -1.319587;1.799105;,
  -1.477848;1.799130;,
  1.206604;1.150371;,
  1.206570;1.799130;,
  1.364832;1.799105;,
  1.364866;1.150347;,
  -2.268928;1.799037;,
  -2.268895;1.150289;,
  -2.427120;1.150289;,
  -2.427154;1.799037;,
  -1.715190;2.393279;,
  -1.715175;2.551541;,
  -1.873403;2.551541;,
  -1.873418;2.393279;,
  -2.268910;1.150313;,
  -2.268943;1.799062;,
  -2.427169;1.799062;,
  -2.427135;1.150313;,
  1.206604;1.150371;,
  1.364866;1.150347;,
  1.364832;1.799105;,
  1.206570;1.799130;,
  -2.427135;1.150313;,
  -2.427169;1.799062;,
  -2.427154;1.799037;,
  -2.427120;1.150289;,
  -2.268928;1.799037;,
  -2.268895;1.150289;,
  -2.268943;1.799062;,
  -2.268910;1.150313;,
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
  2.873418;0.737342;,
  3.901899;0.737342;,
  3.901899;-0.291139;,
  2.873418;-0.291139;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  -4.772247;-0.000033;,
  -0.351576;-0.000033;,
  -0.351576;2.654573;,
  -4.772247;2.654573;,
  5.944777;-0.000008;,
  5.944777;2.654667;,
  0.415082;-0.026037;,
  -2.071679;-0.026037;,
  -2.071679;3.089268;,
  0.415082;3.089310;,
  5.084860;-0.026037;,
  1.965007;-0.026037;,
  1.965007;3.089337;,
  5.084860;3.089391;;
 }
}
