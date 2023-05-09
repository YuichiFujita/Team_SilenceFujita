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
 188;
 -300.00000;925.63467;-300.00000;,
 300.00000;925.63467;-300.00000;,
 300.00000;-0.03360;-300.00000;,
 -300.00000;-0.03360;-300.00000;,
 300.00000;925.63467;-300.00000;,
 300.00000;925.63467;300.00000;,
 300.00000;-0.03360;300.00000;,
 300.00000;-0.03360;-300.00000;,
 300.00000;925.63467;300.00000;,
 -300.00000;925.63467;300.00000;,
 -300.00000;-0.03360;300.00000;,
 300.00000;-0.03360;300.00000;,
 -300.00000;925.63467;300.00000;,
 -300.00000;925.63467;-300.00000;,
 -300.00000;-0.03360;-300.00000;,
 -300.00000;-0.03360;300.00000;,
 300.00000;925.63467;-300.00000;,
 -300.00000;925.63467;-300.00000;,
 -300.00000;-0.03360;-300.00000;,
 300.00000;-0.03360;-300.00000;,
 148.31910;104.44260;-328.37640;,
 332.30910;104.44260;-328.37640;,
 332.30910;75.08730;-328.37640;,
 148.31910;75.08730;-328.37640;,
 332.30910;104.44260;-328.37640;,
 332.30910;104.44260;-144.38670;,
 332.30910;75.08730;-144.38670;,
 332.30910;75.08730;-328.37640;,
 332.30910;104.44260;-144.38670;,
 148.31910;104.44260;-144.38670;,
 148.31910;75.08730;-144.38670;,
 332.30910;75.08730;-144.38670;,
 148.31910;104.44260;-144.38670;,
 148.31910;104.44260;-328.37640;,
 148.31910;75.08730;-328.37640;,
 148.31910;75.08730;-144.38670;,
 332.30910;104.44260;-328.37640;,
 148.31910;104.44260;-328.37640;,
 148.31910;75.08730;-328.37640;,
 332.30910;75.08730;-328.37640;,
 283.53090;50.24160;-312.58411;,
 299.74619;50.24160;-312.58411;,
 299.74619;-0.82290;-312.58411;,
 283.53090;-0.82290;-312.58411;,
 299.74619;50.24160;-312.58411;,
 299.74619;50.24160;-296.36879;,
 299.74619;-0.82290;-296.36879;,
 299.74619;-0.82290;-312.58411;,
 299.74619;50.24160;-296.36879;,
 283.53090;50.24160;-296.36879;,
 283.53090;-0.82290;-296.36879;,
 299.74619;-0.82290;-296.36879;,
 283.53090;50.24160;-296.36879;,
 283.53090;50.24160;-312.58411;,
 283.53090;-0.82290;-312.58411;,
 283.53090;-0.82290;-296.36879;,
 299.74619;50.24160;-312.58411;,
 283.53090;50.24160;-312.58411;,
 283.53090;-0.82290;-312.58411;,
 299.74619;-0.82290;-312.58411;,
 198.84210;50.24160;-312.58411;,
 215.05740;50.24160;-312.58411;,
 215.05740;-0.82290;-312.58411;,
 198.84210;-0.82290;-312.58411;,
 215.05740;50.24160;-312.58411;,
 215.05740;50.24160;-296.36879;,
 215.05740;-0.82290;-296.36879;,
 215.05740;-0.82290;-312.58411;,
 215.05740;50.24160;-296.36879;,
 198.84210;50.24160;-296.36879;,
 198.84210;-0.82290;-296.36879;,
 215.05740;-0.82290;-296.36879;,
 198.84210;50.24160;-296.36879;,
 198.84210;50.24160;-312.58411;,
 198.84210;-0.82290;-312.58411;,
 198.84210;-0.82290;-296.36879;,
 215.05740;50.24160;-312.58411;,
 198.84210;50.24160;-312.58411;,
 198.84210;-0.82290;-312.58411;,
 215.05740;-0.82290;-312.58411;,
 116.67120;50.24160;-312.58411;,
 132.88620;50.24160;-312.58411;,
 132.88620;-0.82290;-312.58411;,
 116.67120;-0.82290;-312.58411;,
 132.88620;50.24160;-312.58411;,
 132.88620;50.24160;-296.36879;,
 132.88620;-0.82290;-296.36879;,
 132.88620;-0.82290;-312.58411;,
 132.88620;50.24160;-296.36879;,
 116.67120;50.24160;-296.36879;,
 116.67120;-0.82290;-296.36879;,
 132.88620;-0.82290;-296.36879;,
 116.67120;50.24160;-296.36879;,
 116.67120;50.24160;-312.58411;,
 116.67120;-0.82290;-312.58411;,
 116.67120;-0.82290;-296.36879;,
 132.88620;50.24160;-312.58411;,
 116.67120;50.24160;-312.58411;,
 116.67120;-0.82290;-312.58411;,
 132.88620;-0.82290;-312.58411;,
 64.35840;78.04290;-372.86911;,
 370.99109;78.04290;-372.86911;,
 370.99109;49.73880;-372.86911;,
 64.35840;49.73880;-372.86911;,
 370.99109;78.04290;-372.86911;,
 370.99109;78.04290;-104.07780;,
 370.99109;49.73880;-104.07780;,
 370.99109;49.73880;-372.86911;,
 370.99109;78.04290;-104.07780;,
 64.35840;78.04290;-104.07780;,
 64.35840;49.73880;-104.07780;,
 370.99109;49.73880;-104.07780;,
 64.35840;78.04290;-104.07780;,
 370.99109;78.04290;-372.86911;,
 64.35840;78.04290;-372.86911;,
 64.35840;49.73880;-372.86911;,
 370.99109;49.73880;-372.86911;,
 64.35840;49.73880;-104.07780;,
 -267.88651;78.04290;-372.86911;,
 -267.88651;78.04290;-104.07780;,
 64.35840;78.04290;-372.86911;,
 -267.88651;49.73880;-104.07780;,
 -267.88651;49.73880;-372.86911;,
 64.35840;49.73880;-372.86911;,
 -267.88651;78.04290;-566.18495;,
 -267.88651;49.73880;-576.27603;,
 64.35840;78.04290;-566.18495;,
 64.35840;49.73880;-576.27603;,
 102.19980;129.67500;-372.86911;,
 370.99109;129.67500;-372.86911;,
 370.99109;101.37090;-372.86911;,
 102.19980;101.37090;-372.86911;,
 370.99109;129.67500;-372.86911;,
 370.99109;129.67500;-104.07780;,
 370.99109;101.37090;-104.07780;,
 370.99109;101.37090;-372.86911;,
 370.99109;129.67500;-104.07780;,
 102.19980;129.67500;-104.07780;,
 102.19980;101.37090;-104.07780;,
 370.99109;101.37090;-104.07780;,
 102.19980;129.67500;-104.07780;,
 102.19980;129.67500;-372.86911;,
 102.19980;101.37090;-372.86911;,
 102.19980;101.37090;-104.07780;,
 370.99109;129.67500;-372.86911;,
 102.19980;129.67500;-372.86911;,
 102.19980;101.37090;-372.86911;,
 370.99109;101.37090;-372.86911;,
 32.17530;50.24160;-547.37645;,
 48.39030;50.24160;-547.37645;,
 48.39030;-0.82290;-547.37645;,
 32.17530;-0.82290;-547.37645;,
 48.39030;50.24160;-547.37645;,
 48.39030;50.24160;-531.16145;,
 48.39030;-0.82290;-531.16145;,
 48.39030;-0.82290;-547.37645;,
 48.39030;50.24160;-531.16145;,
 32.17530;50.24160;-531.16145;,
 32.17530;-0.82290;-531.16145;,
 48.39030;-0.82290;-531.16145;,
 32.17530;50.24160;-531.16145;,
 32.17530;50.24160;-547.37645;,
 32.17530;-0.82290;-547.37645;,
 32.17530;-0.82290;-531.16145;,
 48.39030;50.24160;-547.37645;,
 32.17530;50.24160;-547.37645;,
 32.17530;-0.82290;-547.37645;,
 48.39030;-0.82290;-547.37645;,
 -255.53911;50.24160;-547.37645;,
 -239.32409;50.24160;-547.37645;,
 -239.32409;-0.82290;-547.37645;,
 -255.53911;-0.82290;-547.37645;,
 -239.32409;50.24160;-547.37645;,
 -239.32409;50.24160;-531.16145;,
 -239.32409;-0.82290;-531.16145;,
 -239.32409;-0.82290;-547.37645;,
 -239.32409;50.24160;-531.16145;,
 -255.53911;50.24160;-531.16145;,
 -255.53911;-0.82290;-531.16145;,
 -239.32409;-0.82290;-531.16145;,
 -255.53911;50.24160;-531.16145;,
 -255.53911;50.24160;-547.37645;,
 -255.53911;-0.82290;-547.37645;,
 -255.53911;-0.82290;-531.16145;,
 -239.32409;50.24160;-547.37645;,
 -255.53911;50.24160;-547.37645;,
 -255.53911;-0.82290;-547.37645;,
 -239.32409;-0.82290;-547.37645;;
 
 62;
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
 4;112,105,113,114;,
 4;115,116,106,117;,
 4;118,119,112,120;,
 4;121,122,123,117;,
 4;119,121,117,112;,
 4;119,118,122,121;,
 4;124,125,122,118;,
 4;126,124,118,120;,
 4;127,126,120,123;,
 4;125,127,123,122;,
 4;125,124,126,127;,
 4;128,129,130,131;,
 4;132,133,134,135;,
 4;136,137,138,139;,
 4;140,141,142,143;,
 4;140,133,144,145;,
 4;146,147,134,143;,
 4;148,149,150,151;,
 4;152,153,154,155;,
 4;156,157,158,159;,
 4;160,161,162,163;,
 4;160,153,164,165;,
 4;166,167,154,163;,
 4;168,169,170,171;,
 4;172,173,174,175;,
 4;176,177,178,179;,
 4;180,181,182,183;,
 4;180,173,184,185;,
 4;186,187,174,183;;
 
 MeshMaterialList {
  4;
  62;
  0,
  0,
  1,
  0,
  1,
  1,
  2,
  1,
  1,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\CityOffFront.png";
   }
  }
  Material {
   0.568000;0.326400;0.009600;1.000000;;
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
    "data\\texture\\CityOfficeFront.png";
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
  55;
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
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.335819;-0.941927;,
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
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  62;
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
  4;33,33,33,33;,
  4;34,34,34,34;,
  4;32,32,32,32;,
  4;35,35,35,35;,
  4;35,35,35,35;,
  4;33,33,33,33;,
  4;31,31,31,31;,
  4;34,34,34,34;,
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
  4;54,54,54,54;;
 }
 MeshTextureCoords {
  188;
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
  1.004575;0.091113;,
  1.596906;0.091113;,
  1.596906;0.185618;,
  1.004575;0.185618;,
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
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
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
