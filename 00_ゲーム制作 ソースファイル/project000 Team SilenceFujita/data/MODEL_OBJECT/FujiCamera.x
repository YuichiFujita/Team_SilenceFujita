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
 327;
 -600.00836;-0.00620;-200.00836;,
 -200.00836;-0.00620;-200.00836;,
 -200.00836;-0.00620;199.99164;,
 -600.00836;-0.00620;199.99164;,
 -600.00000;164.00000;-200.00000;,
 -200.00000;164.00000;-200.00000;,
 -200.00836;-0.00620;-200.00836;,
 -600.00836;-0.00620;-200.00836;,
 -600.00000;164.00000;-200.00000;,
 -600.00836;-0.00620;-200.00836;,
 -600.00836;-0.00620;199.99164;,
 -600.00000;163.99998;200.00002;,
 -600.00000;163.97995;800.00000;,
 -600.00836;-0.02622;799.99164;,
 799.99170;-0.02622;799.99164;,
 800.00006;163.97995;800.00000;,
 800.00000;163.97995;-199.99998;,
 799.99164;-0.02622;-200.00836;,
 799.99170;-0.02622;799.99164;,
 800.00006;163.97995;800.00000;,
 859.99170;-0.02624;799.99164;,
 860.00006;163.97993;800.00000;,
 859.99164;-0.02624;-200.00836;,
 859.99170;-0.02624;799.99164;,
 799.99170;-0.02622;799.99164;,
 799.99164;-0.02622;-200.00836;,
 860.00000;163.97993;-199.99998;,
 859.99164;-0.02624;-200.00836;,
 799.99164;-0.02622;-200.00836;,
 800.00000;163.97995;-199.99998;,
 860.00000;163.97993;-199.99998;,
 860.00006;163.97993;800.00000;,
 859.99170;-0.02624;799.99164;,
 859.99164;-0.02624;-200.00836;,
 800.00000;-0.02000;740.00000;,
 800.00836;163.98618;740.00836;,
 -600.00006;-0.02000;740.00000;,
 800.00000;-0.02000;740.00000;,
 -600.00836;-0.02622;799.99164;,
 -599.99170;163.98618;740.00836;,
 -600.00006;-0.02000;740.00000;,
 -600.00000;488.00000;200.00000;,
 -600.00000;600.00000;200.00000;,
 -600.00000;600.00000;-200.00000;,
 -600.00000;488.00003;-200.00000;,
 -600.00000;488.00000;200.00000;,
 -600.00000;488.00003;-200.00000;,
 -600.00000;164.00000;-200.00000;,
 -600.00000;163.99998;200.00002;,
 -600.00000;488.00003;-200.00000;,
 -600.00000;600.00000;-200.00000;,
 -200.00000;600.00000;-200.00000;,
 -200.00000;488.00003;-200.00000;,
 -600.00000;488.00000;800.00000;,
 860.00000;487.99487;800.00000;,
 860.00000;600.00000;800.00000;,
 -600.00000;599.97998;800.00000;,
 860.00000;487.99487;800.00000;,
 860.00000;487.99487;-200.00000;,
 860.00000;600.00000;-200.00000;,
 860.00000;600.00000;800.00000;,
 -600.00000;488.00000;326.75708;,
 -600.00000;488.00000;200.00000;,
 -600.00000;163.99576;326.75708;,
 -600.00000;488.00000;541.83154;,
 -600.00000;488.00000;459.25159;,
 -600.00000;163.99133;459.25159;,
 -600.00000;163.98857;541.83154;,
 646.76221;487.99567;-200.00000;,
 646.76221;163.98302;-199.99998;,
 369.71918;163.98857;-200.00000;,
 369.71918;487.99707;-200.00000;,
 -600.00000;488.00000;708.05731;,
 -600.00000;163.98302;708.05731;,
 -600.00000;163.97995;800.00000;,
 -600.00000;488.00000;800.00000;,
 860.00000;600.00000;-200.00000;,
 860.00000;487.99487;-200.00000;,
 -200.00000;488.00003;-200.00000;,
 -200.00000;600.00000;-200.00000;,
 646.76221;487.99567;-200.00000;,
 646.76221;163.98302;-199.99998;,
 369.71918;163.98857;-200.00000;,
 369.71918;487.99707;-200.00000;,
 -600.00000;488.00003;-200.00000;,
 -600.00000;164.00000;-200.00000;,
 -200.00000;164.00000;-200.00000;,
 -200.00000;488.00003;-200.00000;,
 369.71918;163.98857;-200.00000;,
 369.71918;487.99707;-200.00000;,
 -600.00000;599.97998;800.00000;,
 -600.00000;600.00000;200.00000;,
 -600.00000;600.00000;-200.00000;,
 -600.00000;488.00003;-200.00000;,
 -600.00000;600.00000;-200.00000;,
 -600.00000;488.00003;-200.00000;,
 860.00000;163.97993;-199.99998;,
 800.00000;163.97995;-199.99998;,
 800.00006;163.97995;800.00000;,
 860.00006;163.97993;800.00000;,
 -600.00000;164.00000;-200.00000;,
 -200.00000;164.00000;-200.00000;,
 -200.00000;164.00000;200.00000;,
 -600.00000;163.99998;200.00002;,
 -600.00836;-0.00620;199.99164;,
 -200.00836;-0.00620;199.99164;,
 -200.00000;164.00000;200.00000;,
 -200.00836;-0.00620;199.99164;,
 -200.00836;-0.00620;-200.00836;,
 -200.00000;164.00000;-200.00000;,
 -600.00000;164.00000;-200.00000;,
 -600.00000;163.99998;200.00002;,
 -200.00000;164.00000;200.00000;,
 -200.00000;164.00000;-200.00000;,
 -600.00000;488.00000;541.83154;,
 -600.00000;488.00000;708.05731;,
 -600.00000;163.98302;708.05731;,
 -600.00000;163.98857;541.83154;,
 -600.00000;163.99133;459.25159;,
 -600.00000;163.99576;326.75708;,
 -600.00000;488.00000;326.75708;,
 -600.00000;488.00000;459.25159;,
 -600.00000;163.97995;800.00000;,
 860.00000;600.00000;-200.00000;,
 -600.00000;600.00000;-200.00000;,
 -600.00000;599.97998;800.00000;,
 860.00000;600.00000;800.00000;,
 -699.99670;0.00946;-259.99716;,
 -599.99683;0.00946;-259.99716;,
 -600.00110;0.00946;-199.99716;,
 -700.00098;0.00946;-199.99716;,
 -100.00006;702.03998;-260.00000;,
 -199.99573;600.00000;-260.00000;,
 -600.00000;600.00000;-260.00000;,
 -700.00000;702.03998;-260.00000;,
 -100.00433;702.03998;-200.00000;,
 -700.00427;702.03998;-200.00000;,
 -200.00000;600.00000;-200.00000;,
 -600.00421;600.00000;-200.00002;,
 -600.00000;164.00000;-260.00000;,
 -599.99683;0.00946;-259.99716;,
 -699.99670;0.00946;-259.99716;,
 -699.99988;164.00000;-260.00000;,
 -600.00000;164.00000;-260.00000;,
 -600.00427;164.00000;-200.00000;,
 -599.99683;0.00946;-259.99716;,
 -699.99988;164.00000;-260.00000;,
 -699.99670;0.00946;-259.99716;,
 -700.00415;164.00000;-200.00000;,
 -600.00427;164.00000;-200.00000;,
 -700.00415;164.00000;-200.00000;,
 -700.00098;0.00946;-199.99716;,
 -600.00110;0.00946;-199.99716;,
 -600.00000;244.00000;-260.00000;,
 -699.99988;244.00000;-260.00000;,
 -700.00000;702.03998;-260.00000;,
 -600.00000;600.00000;-260.00000;,
 -600.00000;244.00000;-260.00000;,
 -600.00427;244.00000;-200.00002;,
 -699.99988;244.00000;-260.00000;,
 -700.00415;244.00000;-200.00002;,
 -700.00427;702.03998;-200.00000;,
 -700.00000;702.03998;-260.00000;,
 -600.00427;244.00000;-200.00002;,
 -600.00421;600.00000;-200.00002;,
 -700.00427;702.03998;-200.00000;,
 -700.00415;244.00000;-200.00002;,
 -199.99670;0.00946;-259.99716;,
 -99.99689;0.00946;-259.99716;,
 -100.00116;0.00946;-199.99716;,
 -200.00098;0.00946;-199.99716;,
 -200.00000;600.00000;-200.00000;,
 -100.00433;702.03998;-200.00000;,
 -100.00006;702.03998;-260.00000;,
 -199.99573;600.00000;-260.00000;,
 -200.00000;164.00000;-260.00000;,
 -100.00014;164.00000;-260.00000;,
 -99.99689;0.00946;-259.99716;,
 -199.99670;0.00946;-259.99716;,
 -200.00000;164.00000;-260.00000;,
 -199.99670;0.00946;-259.99716;,
 -200.00426;164.00000;-200.00000;,
 -100.00014;164.00000;-260.00000;,
 -100.00441;164.00000;-200.00000;,
 -99.99689;0.00946;-259.99716;,
 -200.00426;164.00000;-200.00000;,
 -200.00098;0.00946;-199.99716;,
 -100.00116;0.00946;-199.99716;,
 -100.00441;164.00000;-200.00000;,
 -200.00000;244.00000;-260.00000;,
 -199.99573;600.00000;-260.00000;,
 -100.00006;702.03998;-260.00000;,
 -100.00014;244.00000;-260.00000;,
 -200.00000;244.00000;-260.00000;,
 -200.00426;243.99998;-200.00000;,
 -100.00014;244.00000;-260.00000;,
 -100.00006;702.03998;-260.00000;,
 -100.00433;702.03998;-200.00000;,
 -100.00439;243.99998;-200.00000;,
 -200.00426;243.99998;-200.00000;,
 -100.00439;243.99998;-200.00000;,
 -100.00433;702.03998;-200.00000;,
 -200.00000;600.00000;-200.00000;,
 -600.00427;164.00000;-200.00000;,
 -600.00000;164.00000;-260.00000;,
 -600.00427;244.00000;-200.00002;,
 -600.00000;244.00000;-260.00000;,
 -200.00000;244.00000;-304.31641;,
 -600.00000;244.00000;-304.31641;,
 -600.00000;164.00000;-304.31641;,
 -200.00000;164.00000;-304.31641;,
 -600.00000;164.00000;-304.31641;,
 -600.00000;244.00000;-304.31641;,
 -200.00000;244.00000;-304.31641;,
 -200.00000;164.00000;-304.31641;,
 -700.00000;164.00000;-304.31641;,
 -740.00000;244.00000;-304.31641;,
 -700.00000;164.00000;-260.00000;,
 -740.00000;244.00000;-260.00000;,
 -60.00000;244.00000;-304.31641;,
 -100.00000;164.00000;-304.31641;,
 -60.00000;244.00000;-260.00000;,
 -100.00000;164.00000;-260.00000;,
 65.09218;163.98618;-199.99162;,
 65.08382;-0.02000;-200.00000;,
 65.08763;-0.01378;-159.99164;,
 65.09599;163.99240;-159.98322;,
 105.08763;-0.01378;-159.99164;,
 105.09599;163.99240;-159.98322;,
 65.09599;163.99240;-159.98322;,
 65.08763;-0.01378;-159.99164;,
 105.08382;-0.02000;-200.00000;,
 105.08763;-0.01378;-159.99164;,
 65.08763;-0.01378;-159.99164;,
 65.08382;-0.02000;-200.00000;,
 105.09218;163.98618;-199.99162;,
 105.08382;-0.02000;-200.00000;,
 65.08382;-0.02000;-200.00000;,
 65.09218;163.98618;-199.99162;,
 105.09218;163.98618;-199.99162;,
 105.09599;163.99240;-159.98322;,
 105.08763;-0.01378;-159.99164;,
 105.08382;-0.02000;-200.00000;,
 592.48090;163.98618;-199.99162;,
 592.47253;-0.02000;-200.00000;,
 592.47632;-0.01378;-159.99164;,
 592.48474;163.99240;-159.98322;,
 632.47632;-0.01378;-159.99164;,
 632.48474;163.99240;-159.98322;,
 592.48474;163.99240;-159.98322;,
 592.47632;-0.01378;-159.99164;,
 632.47253;-0.02000;-200.00000;,
 632.47632;-0.01378;-159.99164;,
 592.47632;-0.01378;-159.99164;,
 592.47253;-0.02000;-200.00000;,
 632.48090;163.98618;-199.99162;,
 632.47253;-0.02000;-200.00000;,
 592.47253;-0.02000;-200.00000;,
 592.48090;163.98618;-199.99162;,
 632.48090;163.98618;-199.99162;,
 632.48474;163.99240;-159.98322;,
 632.47632;-0.01378;-159.99164;,
 632.47253;-0.02000;-200.00000;,
 -599.99164;163.98618;478.62949;,
 -600.00000;-0.02000;478.62109;,
 -599.99622;-0.01378;518.62952;,
 -599.98779;163.99240;518.63794;,
 -559.99622;-0.01378;518.62952;,
 -559.98779;163.99240;518.63794;,
 -599.98779;163.99240;518.63794;,
 -599.99622;-0.01378;518.62952;,
 -560.00000;-0.02000;478.62109;,
 -559.99622;-0.01378;518.62952;,
 -599.99622;-0.01378;518.62952;,
 -600.00000;-0.02000;478.62109;,
 -559.99164;163.98618;478.62949;,
 -560.00000;-0.02000;478.62109;,
 -600.00000;-0.02000;478.62109;,
 -599.99164;163.98618;478.62949;,
 -559.99164;163.98618;478.62949;,
 -559.98779;163.99240;518.63794;,
 -559.99622;-0.01378;518.62952;,
 -560.00000;-0.02000;478.62109;,
 -599.99164;163.98618;478.62949;,
 -600.00000;-0.02000;478.62109;,
 -599.99622;-0.01378;518.62952;,
 -599.98779;163.99240;518.63794;,
 -559.99622;-0.01378;518.62952;,
 -559.98779;163.99240;518.63794;,
 -560.00000;-0.02000;478.62109;,
 -559.99164;163.98618;478.62949;,
 600.00000;860.00000;-200.00000;,
 860.00000;860.00000;-200.00000;,
 860.00000;600.00378;-200.00000;,
 600.00000;600.00378;-200.00000;,
 860.00000;860.00000;-200.00000;,
 860.00000;860.00000;60.00000;,
 860.00000;600.00378;60.00000;,
 860.00000;600.00378;-200.00000;,
 860.00000;860.00000;60.00000;,
 600.00000;860.00000;60.00000;,
 600.00000;600.00378;60.00000;,
 860.00000;600.00378;60.00000;,
 600.00000;860.00000;60.00000;,
 600.00000;860.00000;-200.00000;,
 600.00000;600.00378;-200.00000;,
 600.00000;600.00378;60.00000;,
 600.00000;860.00000;60.00000;,
 860.00000;860.00000;60.00000;,
 860.00000;860.00000;-200.00000;,
 600.00000;860.00000;-200.00000;,
 600.00000;600.00378;-200.00000;,
 860.00000;600.00378;-200.00000;,
 -600.00000;488.00003;-200.00000;,
 -200.00000;488.00003;-200.00000;,
 -200.00000;164.00000;-200.00000;,
 -600.00000;164.00000;-200.00000;,
 369.71918;487.99707;-200.00000;,
 369.71918;163.98857;-200.00000;,
 -600.00000;488.00000;459.25159;,
 -600.00000;488.00000;326.75708;,
 -600.00000;163.99576;326.75708;,
 -600.00000;163.99133;459.25159;,
 -600.00000;488.00000;708.05731;,
 -600.00000;488.00000;541.83154;,
 -600.00000;163.98857;541.83154;,
 -600.00000;163.98302;708.05731;;
 
 139;
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
 4;53,54,55,56;,
 4;57,31,30,58;,
 4;57,58,59,60;,
 4;61,62,11,63;,
 4;64,65,66,67;,
 4;68,69,70,71;,
 4;72,73,74,75;,
 4;76,77,78,79;,
 4;80,77,26,81;,
 4;81,26,77,80;,
 4;82,81,80,83;,
 4;84,85,86,87;,
 4;86,88,89,87;,
 4;79,78,77,76;,
 4;90,91,62,75;,
 4;75,62,91,90;,
 4;92,91,62,93;,
 4;94,95,78,79;,
 4;59,58,57,60;,
 4;30,31,57,58;,
 4;96,97,98,99;,
 4;100,101,102,103;,
 4;103,104,105,102;,
 4;106,107,108,109;,
 4;110,111,112,113;,
 4;63,8,93,61;,
 4;65,64,67,66;,
 4;72,75,74,73;,
 4;114,115,116,117;,
 4;118,119,120,121;,
 4;54,53,12,21;,
 4;21,12,53,54;,
 4;110,122,99,97;,
 4;97,99,122,110;,
 4;123,124,125,126;,
 4;126,125,124,123;,
 4;127,128,129,130;,
 4;131,132,133,134;,
 4;135,131,134,136;,
 4;137,135,136,138;,
 4;132,137,138,133;,
 4;132,131,135,137;,
 4;139,140,141,142;,
 4;143,144,129,145;,
 4;146,147,130,148;,
 4;149,150,151,152;,
 4;153,139,142,154;,
 4;153,154,155,156;,
 4;157,133,138,158;,
 4;157,158,144,143;,
 4;159,146,148,160;,
 4;159,160,161,162;,
 4;163,164,165,166;,
 4;163,166,150,149;,
 4;167,168,169,170;,
 4;171,172,173,174;,
 4;175,176,177,178;,
 4;179,180,170,181;,
 4;182,183,169,184;,
 4;185,186,187,188;,
 4;189,190,191,192;,
 4;189,192,176,175;,
 4;193,194,171,174;,
 4;195,196,197,198;,
 4;195,198,183,182;,
 4;199,185,188,200;,
 4;199,200,201,202;,
 4;203,204,179,181;,
 4;205,203,181,194;,
 4;206,205,194,193;,
 4;206,204,203,205;,
 4;207,208,206,193;,
 4;209,210,179,204;,
 4;211,212,213,214;,
 4;215,216,208,209;,
 4;217,215,209,204;,
 4;218,217,204,206;,
 4;216,218,206,208;,
 4;216,215,217,218;,
 4;219,220,210,207;,
 4;221,219,207,193;,
 4;222,221,193,179;,
 4;220,222,179,210;,
 4;220,219,221,222;,
 4;223,224,225,226;,
 4;227,228,229,230;,
 4;231,232,233,234;,
 4;235,236,237,238;,
 4;239,240,241,242;,
 4;235,238,229,228;,
 4;243,244,245,246;,
 4;247,248,249,250;,
 4;251,252,253,254;,
 4;255,256,257,258;,
 4;259,260,261,262;,
 4;255,258,249,248;,
 4;263,264,265,266;,
 4;267,268,269,270;,
 4;271,272,273,274;,
 4;275,276,277,278;,
 4;279,280,281,282;,
 4;275,278,269,268;,
 4;283,284,285,286;,
 4;287,288,286,285;,
 4;289,287,285,284;,
 4;290,289,284,283;,
 4;290,288,287,289;,
 4;290,283,286,288;,
 4;291,292,293,294;,
 4;295,296,297,298;,
 4;299,300,301,302;,
 4;303,304,305,306;,
 4;307,308,309,310;,
 4;311,312,297,306;,
 4;313,314,315,316;,
 4;317,318,315,314;,
 4;319,320,321,322;,
 4;323,324,325,326;,
 4;316,315,314,313;,
 4;314,315,318,317;,
 4;322,321,320,319;,
 4;326,325,324,323;;
 
 MeshMaterialList {
  14;
  139;
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
  2,
  3,
  3,
  3,
  3,
  9,
  3,
  2,
  2,
  2,
  2,
  0,
  0,
  2,
  3,
  3,
  3,
  2,
  3,
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
    "data\\texture\\Glass.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;0.800000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\Glass.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\wall.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\wall.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\wall.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\FujiCamera.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\FujiCamerayoko.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\FujiCamerayoko.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\FujiCameraSolo.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\kadenLogo.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\kadenLogo.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\doar000.png";
   }
  }
  Material {
   0.800000;0.062745;0.037647;1.000000;;
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
  110;
  -1.000000;0.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  -1.000000;0.000051;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.000000;-0.000051;1.000000;,
  0.000000;-0.000051;1.000000;,
  0.000000;0.000051;-1.000000;,
  -1.000000;0.000017;0.000000;,
  0.000000;-0.000025;1.000000;,
  1.000000;0.000000;0.000000;,
  -0.000000;-1.000000;0.000000;,
  -0.000000;-0.000051;1.000000;,
  0.000000;0.000026;-1.000000;,
  -0.000000;-0.000025;1.000000;,
  1.000000;-0.000051;0.000138;,
  0.000000;-1.000000;-0.000104;,
  -1.000000;0.000051;-0.000138;,
  1.000000;0.000000;-0.000000;,
  0.000000;-0.000000;-1.000000;,
  0.000000;-0.000000;-1.000000;,
  0.000000;-1.000000;-0.000000;,
  -1.000000;0.000025;0.000000;,
  -1.000000;0.000051;0.000000;,
  -0.000000;-1.000000;-0.000052;,
  0.000000;0.000051;-1.000000;,
  1.000000;-0.000025;-0.000000;,
  1.000000;-0.000051;-0.000000;,
  -1.000000;0.000025;-0.000069;,
  0.000000;1.000000;0.000104;,
  0.000004;1.000000;0.000057;,
  0.000000;1.000000;0.000052;,
  0.000000;0.000051;-1.000000;,
  -0.000000;-0.000000;-1.000000;,
  -0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  -0.000000;0.000000;1.000000;,
  -1.000000;-0.000000;0.000000;,
  -1.000000;-0.000000;0.000000;,
  0.000000;1.000000;-0.000000;,
  0.000004;1.000000;0.000005;,
  -0.000004;-1.000000;-0.000005;,
  0.000000;-0.000051;1.000000;,
  1.000000;-0.000051;-0.000000;,
  0.000004;1.000000;0.000005;,
  -0.000000;1.000000;0.000000;,
  -0.000007;-1.000000;-0.000010;,
  -0.000007;1.000000;0.000010;,
  0.000007;-1.000000;-0.000010;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-0.000000;1.000000;,
  -0.000000;-0.000000;1.000000;,
  0.000000;-0.000009;-1.000000;,
  1.000000;0.000010;0.000071;,
  -1.000000;-0.000010;-0.000071;,
  1.000000;-0.000000;0.000071;,
  -1.000000;-0.000000;-0.000071;,
  -0.387353;0.921932;-0.000028;,
  -1.000000;-0.000020;-0.000071;,
  1.000000;0.000010;0.000071;,
  -1.000000;0.000012;-0.000071;,
  1.000000;-0.000000;0.000071;,
  0.000000;1.000000;0.000000;,
  -0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.387353;-0.921932;0.000028;,
  0.925804;-0.378005;0.000066;,
  0.000000;-0.000017;-1.000000;,
  1.000000;0.000019;0.000071;,
  -1.000000;-0.000019;-0.000071;,
  0.000000;0.000009;1.000000;,
  0.000000;0.000017;1.000000;,
  1.000000;-0.000000;0.000071;,
  -1.000000;-0.000000;-0.000071;,
  0.000000;0.000000;1.000000;,
  -0.925801;0.378010;-0.000066;,
  1.000000;0.000020;0.000071;,
  -0.000000;0.000006;1.000000;,
  0.000000;0.000009;1.000000;,
  -0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  -1.000000;-0.000000;-0.000071;,
  -0.894427;-0.447214;0.000000;,
  0.894427;-0.447214;-0.000000;,
  -1.000000;0.000051;0.000095;,
  0.000000;-1.000000;0.000156;,
  0.000000;-0.000051;1.000000;,
  0.000000;0.000051;-1.000000;,
  -1.000000;0.000051;0.000095;,
  -1.000000;0.000051;0.000095;,
  0.000000;-1.000000;0.000156;,
  0.000000;0.000051;-1.000000;,
  1.000000;-0.000051;-0.000095;,
  0.000000;1.000000;-0.000156;,
  1.000000;-0.000051;-0.000095;,
  1.000000;-0.000051;-0.000095;,
  0.000000;1.000000;-0.000156;,
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
  139;
  4;1,1,1,1;,
  4;7,7,7,7;,
  4;22,23,23,8;,
  4;9,6,5,5;,
  4;3,3,3,3;,
  4;12,14,5,5;,
  4;11,11,24,11;,
  4;13,25,25,25;,
  4;26,26,27,27;,
  4;15,15,15,15;,
  4;16,16,24,16;,
  4;17,17,17,28;,
  4;29,29,30,31;,
  4;32,32,32,32;,
  4;0,0,0,0;,
  4;0,0,22,8;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;18,26,26,18;,
  4;18,18,10,10;,
  4;0,0,8,0;,
  4;0,0,0,0;,
  4;20,20,19,19;,
  4;0,0,28,0;,
  4;2,33,2,2;,
  4;20,33,13,20;,
  4;34,35,36,34;,
  4;37,34,34,37;,
  4;4,4,4,4;,
  4;4,37,37,4;,
  4;4,4,36,4;,
  4;0,0,0,0;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;4,4,4,4;,
  4;0,38,38,0;,
  4;39,39,38,38;,
  4;40,41,31,41;,
  4;42,21,21,21;,
  4;43,43,43,43;,
  4;44,44,44,44;,
  4;45,46,46,46;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;4,4,9,14;,
  4;2,2,2,2;,
  4;45,30,41,41;,
  4;47,47,47,42;,
  4;48,48,48,48;,
  4;49,49,49,49;,
  4;50,50,50,50;,
  4;51,51,51,51;,
  4;60,60,52,52;,
  4;54,54,53,53;,
  4;68,68,50,50;,
  4;68,69,69,68;,
  4;55,70,70,55;,
  4;56,56,71,71;,
  4;57,72,72,57;,
  4;73,73,74,74;,
  4;51,55,55,51;,
  4;51,51,51,51;,
  4;58,75,75,58;,
  4;58,58,56,56;,
  4;59,57,57,59;,
  4;59,59,76,76;,
  4;77,53,53,77;,
  4;77,77,73,73;,
  4;50,50,50,50;,
  4;78,60,60,78;,
  4;55,55,70,70;,
  4;61,61,61,61;,
  4;62,62,79,79;,
  4;80,74,74,81;,
  4;51,51,51,51;,
  4;51,51,55,55;,
  4;63,63,78,78;,
  4;64,69,69,64;,
  4;64,64,62,62;,
  4;82,80,81,67;,
  4;82,67,54,54;,
  4;50,50,50,50;,
  4;66,66,80,82;,
  4;65,83,83,65;,
  4;84,84,84,84;,
  4;52,52,65,65;,
  4;50,50,50,50;,
  4;51,51,51,51;,
  4;51,51,51,51;,
  4;50,50,50,50;,
  4;67,67,67,67;,
  4;52,52,65,52;,
  4;85,85,85,85;,
  4;51,51,51,51;,
  4;52,52,52,65;,
  4;67,67,67,67;,
  4;50,50,50,50;,
  4;86,86,86,86;,
  4;87,87,87,87;,
  4;89,89,89,89;,
  4;88,88,88,88;,
  4;90,90,90,90;,
  4;95,95,95,95;,
  4;96,96,96,96;,
  4;91,91,91,91;,
  4;89,89,89,89;,
  4;88,88,88,88;,
  4;90,90,90,90;,
  4;97,97,97,97;,
  4;96,96,96,96;,
  4;92,92,92,92;,
  4;89,89,89,89;,
  4;93,93,93,93;,
  4;94,94,94,94;,
  4;98,98,98,98;,
  4;99,99,99,99;,
  4;92,92,92,92;,
  4;89,89,89,89;,
  4;93,93,93,93;,
  4;94,94,94,94;,
  4;98,98,98,98;,
  4;99,99,99,99;,
  4;100,100,100,100;,
  4;101,101,101,101;,
  4;102,102,102,102;,
  4;103,103,103,103;,
  4;104,104,104,104;,
  4;105,105,105,105;,
  4;106,106,106,106;,
  4;106,106,106,106;,
  4;107,107,107,107;,
  4;107,107,107,107;,
  4;108,108,108,108;,
  4;108,108,108,108;,
  4;109,109,109,109;,
  4;109,109,109,109;;
 }
 MeshTextureCoords {
  327;
  -1.873451;-0.291172;,
  -0.291172;-0.291172;,
  -0.291172;1.291106;,
  -1.873451;1.291106;,
  0.007165;-0.027029;,
  2.017407;-0.027029;,
  2.017365;1.030645;,
  0.007123;1.030645;,
  -0.291139;1.150316;,
  -0.291172;1.799075;,
  1.291106;1.799075;,
  1.291139;1.150316;,
  -1.873381;1.150340;,
  -1.873414;1.799089;,
  3.664475;1.799089;,
  3.664508;1.150340;,
  -0.291139;1.150396;,
  -0.291172;1.799154;,
  3.664524;1.799154;,
  3.664557;1.150396;,
  3.901814;1.799089;,
  3.901847;1.150340;,
  3.901866;-0.291172;,
  3.901866;3.664524;,
  3.664524;3.664524;,
  3.664524;-0.291172;,
  3.901846;1.150340;,
  3.901813;1.799089;,
  3.664475;1.799089;,
  3.664508;1.150340;,
  -0.291139;1.150396;,
  3.664557;1.150396;,
  3.664524;1.799154;,
  -0.291172;1.799154;,
  3.664508;1.799064;,
  3.664541;1.150316;,
  -1.873418;3.427215;,
  3.664557;3.427215;,
  -1.873451;3.664524;,
  -1.873348;1.150316;,
  -1.873381;1.799064;,
  0.019215;0.907153;,
  0.019215;0.068492;,
  0.989733;0.068492;,
  0.989733;0.907153;,
  0.174397;0.005209;,
  0.825603;0.005209;,
  0.825603;1.003965;,
  0.174397;1.003965;,
  0.011474;0.905983;,
  0.011474;0.024220;,
  1.011474;0.024220;,
  1.011474;0.905983;,
  -1.873381;-0.131365;,
  3.901846;-0.131344;,
  3.901846;-0.574396;,
  -1.873381;-0.574317;,
  3.664557;-0.131309;,
  -0.291139;-0.131309;,
  -0.291139;-0.574367;,
  3.664557;-0.574367;,
  1.792552;-0.131329;,
  1.291139;-0.131329;,
  1.792552;1.150333;,
  2.643321;-0.131329;,
  2.316660;-0.131329;,
  2.316660;1.150351;,
  2.643321;1.150362;,
  0.774318;0.004946;,
  0.774318;1.003752;,
  0.255822;1.003734;,
  0.255822;0.004941;,
  3.300860;-0.131329;,
  3.300860;1.150384;,
  3.664557;1.150396;,
  3.664557;-0.131329;,
  3.901846;-0.574396;,
  3.901846;-0.131344;,
  -0.291127;-0.131365;,
  -0.291127;-0.574396;,
  3.058355;-0.131348;,
  3.058355;1.150328;,
  1.962474;1.150306;,
  1.962474;-0.131353;,
  -4.772210;-0.000042;,
  -4.772210;2.654596;,
  -0.351578;2.654596;,
  -0.351578;-0.000042;,
  5.944719;2.654690;,
  5.944719;-0.000018;,
  3.664557;-0.574288;,
  1.291139;-0.574367;,
  -0.291139;-0.574367;,
  -0.291139;-0.131329;,
  -1.873381;-0.574396;,
  -1.873381;-0.131365;,
  3.901899;-0.291139;,
  3.664557;-0.291139;,
  3.664557;3.664557;,
  3.901899;3.664557;,
  -1.873381;1.150261;,
  -0.291127;1.150261;,
  -0.291127;1.150261;,
  -1.873381;1.150261;,
  -1.873414;1.799010;,
  -0.291160;1.799010;,
  1.291139;1.150316;,
  1.291106;1.799075;,
  -0.291172;1.799075;,
  -0.291139;1.150316;,
  -1.873418;-0.291139;,
  -1.873418;1.291139;,
  -0.291139;1.291139;,
  -0.291139;-0.291139;,
  1.965013;-0.026033;,
  5.084850;-0.026033;,
  5.084850;3.089422;,
  1.965013;3.089369;,
  0.415096;3.089342;,
  -2.071650;3.089300;,
  -2.071650;-0.026033;,
  0.415096;-0.026033;,
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
  0.000000;0.124959;,
  0.000000;0.124959;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.124959;,
  1.000000;0.124959;,
  1.000000;0.729909;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.729909;,
  0.000000;0.729909;,
  1.000000;0.729909;,
  0.000000;1.000000;,
  1.000000;0.729909;,
  1.000000;1.000000;,
  0.000000;0.729909;,
  0.000000;0.729909;,
  1.000000;0.729909;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.638694;,
  0.000000;0.638694;,
  0.000000;0.124959;,
  1.000000;0.124959;,
  0.000000;0.638694;,
  1.000000;0.638694;,
  1.000000;0.638694;,
  0.000000;0.638694;,
  0.000000;0.124959;,
  1.000000;0.124959;,
  0.000000;0.638694;,
  0.000000;0.124959;,
  1.000000;0.124959;,
  1.000000;0.638694;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.114135;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.114135;,
  0.000000;0.732147;,
  1.000000;0.732147;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.732147;,
  1.000000;1.000000;,
  0.000000;0.732147;,
  0.000000;0.732147;,
  1.000000;0.732147;,
  0.000000;1.000000;,
  1.000000;0.732147;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.732147;,
  0.000000;0.636954;,
  0.000000;0.114135;,
  1.000000;0.114135;,
  1.000000;0.636954;,
  1.000000;0.636954;,
  0.000000;0.636954;,
  0.000000;0.636954;,
  0.000000;0.114135;,
  1.000000;0.114135;,
  1.000000;0.636954;,
  1.000000;0.636954;,
  0.000000;0.636954;,
  0.000000;0.114135;,
  1.000000;0.114135;,
  0.000000;0.732147;,
  1.000000;0.732147;,
  0.000000;0.636954;,
  1.000000;0.636954;,
  1.000000;0.636954;,
  1.000000;0.636954;,
  1.000000;0.732147;,
  1.000000;0.732147;,
  0.097652;0.634459;,
  0.097652;0.308394;,
  0.958681;0.308394;,
  0.958681;0.634459;,
  1.000000;0.732147;,
  1.000000;0.636954;,
  1.000000;0.732147;,
  1.000000;0.636954;,
  1.000000;0.636954;,
  1.000000;0.732147;,
  1.000000;0.636954;,
  1.000000;0.732147;,
  -0.291106;1.150371;,
  -0.291139;1.799130;,
  -0.132878;1.799105;,
  -0.132845;1.150347;,
  0.915688;1.799040;,
  0.915721;1.150291;,
  0.757496;1.150291;,
  0.757463;1.799040;,
  0.915680;-0.291139;,
  0.915695;-0.132878;,
  0.757467;-0.132878;,
  0.757452;-0.291139;,
  0.915706;1.150316;,
  0.915673;1.799064;,
  0.757448;1.799064;,
  0.757481;1.150316;,
  -0.291106;1.150371;,
  -0.132845;1.150347;,
  -0.132878;1.799105;,
  -0.291139;1.799130;,
  -0.291106;1.150371;,
  -0.291139;1.799130;,
  -0.132878;1.799105;,
  -0.132845;1.150347;,
  3.001846;1.799040;,
  3.001879;1.150291;,
  2.843653;1.150291;,
  2.843620;1.799040;,
  3.001869;-0.291139;,
  3.001884;-0.132878;,
  2.843656;-0.132878;,
  2.843641;-0.291139;,
  3.001864;1.150316;,
  3.001831;1.799064;,
  2.843605;1.799064;,
  2.843638;1.150316;,
  -0.291106;1.150371;,
  -0.132845;1.150347;,
  -0.132878;1.799105;,
  -0.291139;1.799130;,
  2.393313;1.150371;,
  2.393280;1.799130;,
  2.551541;1.799105;,
  2.551574;1.150347;,
  -1.715141;1.799040;,
  -1.715107;1.150291;,
  -1.873333;1.150291;,
  -1.873366;1.799040;,
  -1.715190;2.393280;,
  -1.715175;2.551541;,
  -1.873403;2.551541;,
  -1.873418;2.393280;,
  -1.715123;1.150316;,
  -1.715156;1.799064;,
  -1.873381;1.799064;,
  -1.873348;1.150316;,
  2.393313;1.150371;,
  2.551574;1.150347;,
  2.551541;1.799105;,
  2.393280;1.799130;,
  -1.873348;1.150316;,
  -1.873381;1.799064;,
  -1.873366;1.799040;,
  -1.873333;1.150291;,
  -1.715141;1.799040;,
  -1.715107;1.150291;,
  -1.715156;1.799064;,
  -1.715123;1.150316;,
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
  -4.772210;-0.000042;,
  -0.351578;-0.000042;,
  -0.351578;2.654596;,
  -4.772210;2.654596;,
  5.944719;-0.000018;,
  5.944719;2.654690;,
  0.415096;-0.026033;,
  -2.071650;-0.026033;,
  -2.071650;3.089300;,
  0.415096;3.089342;,
  5.084850;-0.026033;,
  1.965013;-0.026033;,
  1.965013;3.089369;,
  5.084850;3.089422;;
 }
}
