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
 53;
 138.44504;395.08002;-200.00000;,
 -176.49861;395.08002;-200.00000;,
 -176.49861;395.08002;200.00000;,
 138.44504;395.08002;200.00000;,
 237.35010;395.08002;-200.00000;,
 237.35010;395.08002;200.00000;,
 408.82150;395.08002;-200.00000;,
 408.82150;395.08002;200.00000;,
 490.81918;395.08002;200.00000;,
 490.81918;395.08002;-200.00000;,
 237.35010;283.30585;-200.00000;,
 408.82153;283.30585;-200.00000;,
 408.82153;98.78491;-200.00000;,
 237.35010;98.78491;-200.00000;,
 -253.48871;395.08002;-200.00000;,
 -253.48871;395.08002;200.00000;,
 -435.17532;395.08002;-200.00000;,
 -490.81918;395.08002;-200.00000;,
 -490.81918;395.08002;200.00000;,
 -435.17535;395.08002;200.00000;,
 -435.17532;283.30585;-200.00000;,
 -253.48871;283.30585;-200.00000;,
 -253.48871;98.78491;-200.00000;,
 -435.17535;98.78491;-200.00000;,
 -176.49861;283.30585;-200.00000;,
 138.44504;283.30585;-200.00000;,
 138.44504;-4.92000;-200.00000;,
 -176.49861;-4.92000;-200.00000;,
 150.26157;243.22040;-203.33490;,
 -190.73239;243.22040;-203.33490;,
 -190.73239;315.56558;-203.33490;,
 150.26157;315.56558;-203.33490;,
 -490.81918;-4.92000;200.00000;,
 -490.81918;-4.92000;-200.00000;,
 490.81918;-4.92000;-200.00000;,
 490.81918;-4.92000;200.00000;,
 -490.81918;-4.92000;200.00000;,
 490.81918;-4.92000;200.00000;,
 490.81918;-4.92000;-200.00000;,
 -507.69861;615.87488;-246.60666;,
 503.99335;615.87482;-246.60666;,
 503.99332;421.02667;-246.60666;,
 -507.69861;421.02667;-246.60666;,
 558.77783;645.69421;-246.60666;,
 558.77783;645.69421;246.60666;,
 558.77783;393.96988;246.60666;,
 558.77783;393.96988;-246.60666;,
 -558.77783;645.69421;246.60666;,
 -558.77783;645.69421;-246.60666;,
 -558.77783;393.96988;-246.60666;,
 -558.77783;393.96988;246.60666;,
 -558.77783;393.96988;-246.60666;,
 558.77783;393.96988;-246.60666;;
 
 32;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;6,7,8,9;,
 4;10,11,12,13;,
 4;14,15,2,1;,
 4;16,17,18,19;,
 4;16,19,15,14;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;18,17,33,36;,
 4;9,8,37,38;,
 4;18,32,35,8;,
 4;17,20,23,33;,
 4;23,22,27,33;,
 4;27,22,21,24;,
 4;12,34,26,13;,
 4;34,12,11,9;,
 4;25,10,13,26;,
 4;20,17,9,11;,
 4;39,40,41,42;,
 4;43,44,45,46;,
 4;47,48,49,50;,
 4;45,44,47,50;,
 4;50,51,52,45;,
 4;46,41,40,43;,
 4;49,42,41,46;,
 4;48,39,42,49;,
 4;43,40,39,48;,
 4;44,43,48,47;;
 
 MeshMaterialList {
  7;
  32;
  0,
  0,
  0,
  0,
  3,
  0,
  0,
  0,
  4,
  5,
  2,
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
  6,
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
   0.655686;0.655686;0.655686;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.021961;0.727843;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.034510;0.602353;0.800000;1.000000;;
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
    "data\\texture\\doar000.png";
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\doar002.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\texture\\DENTALCLINIC.png";
   }
  }
 }
 MeshNormals {
  12;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;-0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;;
  32;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;8,8,8,8;,
  4;7,7,7,7;,
  4;6,6,6,6;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;8,8,8,8;,
  4;8,8,8,8;,
  4;8,8,8,8;,
  4;8,8,8,8;,
  4;11,11,11,11;;
 }
 MeshTextureCoords {
  53;
  0.641035;1.000000;,
  0.320200;1.000000;,
  0.320200;0.000000;,
  0.641035;0.000000;,
  0.741790;1.000000;,
  0.741790;0.000000;,
  0.916469;1.000000;,
  0.916469;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.015352;-0.021021;,
  1.016186;-0.021021;,
  1.016186;1.003581;,
  0.015352;1.003581;,
  0.241770;1.000000;,
  0.241770;0.000000;,
  0.056685;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.056685;0.000000;,
  -0.003784;-0.021021;,
  1.056673;-0.021021;,
  1.056673;1.003581;,
  -0.003784;1.003581;,
  0.013741;-0.032377;,
  1.004141;-0.032377;,
  1.004141;0.989260;,
  0.013741;0.989260;,
  0.045706;0.118461;,
  0.000000;0.118461;,
  0.000000;0.000000;,
  0.045706;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.015848;0.052623;,
  0.967416;0.052623;,
  0.967416;0.618364;,
  0.015848;0.618364;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;;
 }
}
