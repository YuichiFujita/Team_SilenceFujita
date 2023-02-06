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
 208;
 226.48900;398.45300;200.00000;,
 346.17621;398.45300;200.00000;,
 346.17621;398.45300;-200.00000;,
 205.11420;398.45300;-200.00000;,
 212.00000;-1.54700;-200.00000;,
 346.17621;-1.54700;-200.00000;,
 346.17621;-1.54700;200.00000;,
 233.37460;-1.54700;200.00000;,
 3.69480;398.45300;-200.00000;,
 -3.60680;398.45300;200.00000;,
 -9.23180;-1.54700;200.00000;,
 -1.93040;-1.54700;-200.00000;,
 -182.39079;398.45300;-200.00000;,
 -346.17621;398.45300;-200.00000;,
 -346.17621;398.45300;200.00000;,
 -216.76360;398.45300;200.00000;,
 -218.52139;-1.54700;200.00000;,
 -346.17621;-1.54700;200.00000;,
 -346.17621;-1.54700;-200.00000;,
 -184.14841;-1.54700;-200.00000;,
 3.69480;398.45300;-72.87900;,
 -182.39079;398.45300;-72.87900;,
 -182.39079;398.45300;-200.00000;,
 3.69480;398.45300;-200.00000;,
 -184.14841;-1.54700;-72.87900;,
 -1.93040;-1.54700;-72.87900;,
 -1.93040;-1.54700;-200.00000;,
 -184.14841;-1.54700;-200.00000;,
 205.11420;398.45300;-75.06720;,
 3.69480;398.45300;-75.06720;,
 205.11420;398.45300;-200.00000;,
 -1.93040;-1.54700;-75.06720;,
 212.00000;-1.54700;-75.06720;,
 212.00000;-1.54700;-200.00000;,
 205.11420;398.45300;-31.23240;,
 3.69480;398.45300;-31.23240;,
 -1.93040;-1.54700;-31.23240;,
 212.00000;-1.54700;-31.23240;,
 3.69480;398.45300;-26.85200;,
 -182.39079;398.45300;-26.85200;,
 -184.14841;-1.54700;-26.85200;,
 -1.93040;-1.54700;-26.85200;,
 346.17621;274.55539;200.00000;,
 346.17621;-1.54700;-200.00000;,
 346.17621;273.18460;-200.00000;,
 346.17621;398.45300;-200.00000;,
 -346.17621;273.19321;-200.00000;,
 -346.17621;-1.54700;-200.00000;,
 -346.17621;274.56400;200.00000;,
 -346.17621;398.45300;-200.00000;,
 346.17621;273.18460;-200.00000;,
 346.17621;-1.54700;-200.00000;,
 207.27060;273.18640;-200.00000;,
 346.17621;398.45300;-200.00000;,
 228.62180;274.55701;200.00000;,
 233.37460;-1.54700;200.00000;,
 346.17621;-1.54700;200.00000;,
 346.17621;274.55539;200.00000;,
 346.17621;398.45300;200.00000;,
 226.48900;398.45300;200.00000;,
 -5.34900;274.55981;200.00000;,
 -9.23180;-1.54700;200.00000;,
 -3.60680;398.45300;200.00000;,
 -346.17621;273.19321;-200.00000;,
 -346.17621;398.45300;-200.00000;,
 -182.94119;273.19119;-200.00000;,
 -346.17621;-1.54700;-200.00000;,
 -346.17621;274.56400;200.00000;,
 -346.17621;-1.54700;200.00000;,
 -218.52139;-1.54700;200.00000;,
 -217.30800;274.56241;200.00000;,
 -216.76360;398.45300;200.00000;,
 -346.17621;398.45300;200.00000;,
 -182.93941;273.62680;-72.87900;,
 1.93940;273.62460;-72.87900;,
 1.93320;273.18900;-200.00000;,
 1.93920;273.61700;-75.06720;,
 207.26320;273.61459;-75.06720;,
 1.94140;273.76721;-31.23240;,
 207.26060;273.76480;-31.23240;,
 -182.93860;273.78461;-26.85200;,
 1.94160;273.78220;-26.85200;,
 -131.80840;372.89999;-31.40260;,
 -50.72700;372.89999;-31.40260;,
 -50.72700;291.83099;-29.98760;,
 -131.80840;291.83099;-29.98760;,
 68.28540;372.89999;-33.31940;,
 149.36681;372.89999;-33.31940;,
 149.36681;291.83099;-31.90440;,
 68.28540;291.83099;-31.90440;,
 -460.71320;509.09439;-200.00000;,
 473.16760;509.09439;-200.00000;,
 473.16760;399.72821;-200.00000;,
 -460.71320;399.72821;-200.00000;,
 473.16760;509.09439;-200.00000;,
 473.16760;509.09439;200.00000;,
 473.16760;399.72821;200.00000;,
 473.16760;399.72821;-200.00000;,
 473.16760;509.09439;200.00000;,
 -460.71320;509.09439;200.00000;,
 -460.71320;399.72821;200.00000;,
 473.16760;399.72821;200.00000;,
 -460.71320;509.09439;200.00000;,
 -460.71320;509.09439;-200.00000;,
 -460.71320;399.72821;-200.00000;,
 -460.71320;399.72821;200.00000;,
 473.16760;509.09439;-200.00000;,
 -460.71320;509.09439;-200.00000;,
 -460.71320;399.72821;-200.00000;,
 473.16760;399.72821;-200.00000;,
 -285.91739;499.69559;-212.62939;,
 340.70599;499.69559;-212.62939;,
 340.70599;398.73260;-211.21420;,
 -285.91739;398.73260;-211.21420;,
 -156.98560;203.73199;-23.84060;,
 -150.24780;201.93700;-27.73040;,
 -156.98560;201.93700;-31.62040;,
 -156.98560;203.73199;-23.84060;,
 -150.24780;201.93700;-19.95060;,
 -156.98560;203.73199;-23.84060;,
 -156.98560;201.93700;-16.06080;,
 -156.98560;203.73199;-23.84060;,
 -163.72321;201.93700;-19.95060;,
 -156.98560;203.73199;-23.84060;,
 -163.72321;201.93700;-27.73040;,
 -156.98560;203.73199;-23.84060;,
 -156.98560;201.93700;-31.62040;,
 -145.31560;197.03300;-30.57840;,
 -156.98560;197.03300;-37.31580;,
 -145.31560;197.03300;-17.10280;,
 -156.98560;197.03300;-10.36540;,
 -168.65540;197.03300;-17.10280;,
 -168.65540;197.03300;-30.57840;,
 -156.98560;197.03300;-37.31580;,
 -143.51019;190.33420;-31.62040;,
 -156.98560;190.33420;-39.40040;,
 -143.51019;190.33420;-16.06080;,
 -156.98560;190.33420;-8.28060;,
 -170.46080;190.33420;-16.06080;,
 -170.46080;190.33420;-31.62040;,
 -156.98560;190.33420;-39.40040;,
 -145.31560;183.63541;-30.57840;,
 -156.98560;183.63541;-37.31580;,
 -145.31560;183.63541;-17.10280;,
 -156.98560;183.63541;-10.36540;,
 -168.65540;183.63541;-17.10280;,
 -168.65540;183.63541;-30.57840;,
 -156.98560;183.63541;-37.31580;,
 -150.24780;178.73160;-27.73040;,
 -156.98560;178.73160;-31.62040;,
 -150.24780;178.73160;-19.95060;,
 -156.98560;178.73160;-16.06080;,
 -163.72321;178.73160;-19.95060;,
 -163.72321;178.73160;-27.73040;,
 -156.98560;178.73160;-31.62040;,
 -156.98560;176.93660;-23.84060;,
 -156.98560;176.93660;-23.84060;,
 -156.98560;176.93660;-23.84060;,
 -156.98560;176.93660;-23.84060;,
 -156.98560;176.93660;-23.84060;,
 -156.98560;176.93660;-23.84060;,
 46.03460;203.73199;-23.84060;,
 52.77220;201.93700;-27.73040;,
 46.03460;201.93700;-31.62040;,
 46.03460;203.73199;-23.84060;,
 52.77220;201.93700;-19.95060;,
 46.03460;203.73199;-23.84060;,
 46.03460;201.93700;-16.06080;,
 46.03460;203.73199;-23.84060;,
 39.29700;201.93700;-19.95060;,
 46.03460;203.73199;-23.84060;,
 39.29700;201.93700;-27.73040;,
 46.03460;203.73199;-23.84060;,
 46.03460;201.93700;-31.62040;,
 57.70460;197.03300;-30.57840;,
 46.03460;197.03300;-37.31580;,
 57.70460;197.03300;-17.10280;,
 46.03460;197.03300;-10.36540;,
 34.36460;197.03300;-17.10280;,
 34.36460;197.03300;-30.57840;,
 46.03460;197.03300;-37.31580;,
 59.50980;190.33420;-31.62040;,
 46.03460;190.33420;-39.40040;,
 59.50980;190.33420;-16.06080;,
 46.03460;190.33420;-8.28060;,
 32.55920;190.33420;-16.06080;,
 32.55920;190.33420;-31.62040;,
 46.03460;190.33420;-39.40040;,
 57.70460;183.63541;-30.57840;,
 46.03460;183.63541;-37.31580;,
 57.70440;183.63541;-17.10280;,
 46.03460;183.63541;-10.36540;,
 34.36460;183.63541;-17.10280;,
 34.36460;183.63541;-30.57840;,
 46.03460;183.63541;-37.31580;,
 52.77220;178.73160;-27.73040;,
 46.03460;178.73160;-31.62040;,
 52.77220;178.73160;-19.95060;,
 46.03460;178.73160;-16.06080;,
 39.29700;178.73160;-19.95060;,
 39.29700;178.73160;-27.73040;,
 46.03460;178.73160;-31.62040;,
 46.03460;176.93660;-23.84060;,
 46.03460;176.93660;-23.84060;,
 46.03460;176.93660;-23.84060;,
 46.03460;176.93660;-23.84060;,
 46.03460;176.93660;-23.84060;,
 46.03460;176.93660;-23.84060;;
 
 133;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,0,3;,
 4;10,11,4,7;,
 4;12,13,14,15;,
 4;12,15,9,8;,
 4;16,17,18,19;,
 4;16,19,11,10;,
 4;20,21,22,23;,
 4;24,25,26,27;,
 4;28,29,23,30;,
 4;31,32,33,26;,
 4;34,35,29,28;,
 4;36,37,32,31;,
 4;38,39,21,20;,
 4;40,41,25,24;,
 4;42,6,43,44;,
 4;42,44,45,1;,
 4;46,47,17,48;,
 4;46,48,14,49;,
 4;50,51,33,52;,
 4;50,52,30,53;,
 4;54,55,56,57;,
 4;54,57,58,59;,
 4;60,61,55,54;,
 4;60,54,59,62;,
 4;63,64,22,65;,
 4;63,65,27,66;,
 4;67,68,69,70;,
 4;67,70,71,72;,
 4;70,69,61,60;,
 4;70,60,62,71;,
 4;73,24,27,65;,
 4;73,65,22,21;,
 4;74,20,23,75;,
 4;74,75,26,25;,
 4;76,31,26,75;,
 4;76,75,23,29;,
 4;77,28,30,52;,
 4;77,52,33,32;,
 4;78,36,31,76;,
 4;78,76,29,35;,
 4;79,34,28,77;,
 4;79,77,32,37;,
 4;78,35,34,79;,
 4;78,79,37,36;,
 4;80,40,24,73;,
 4;80,73,21,39;,
 4;81,38,20,74;,
 4;81,74,25,41;,
 4;80,39,38,81;,
 4;80,81,41,40;,
 4;82,83,84,85;,
 4;86,87,88,89;,
 4;90,91,92,93;,
 4;94,95,96,97;,
 4;98,99,100,101;,
 4;102,103,104,105;,
 4;102,95,106,107;,
 4;108,109,96,105;,
 4;110,111,112,113;,
 3;114,115,116;,
 3;117,118,115;,
 3;119,120,118;,
 3;121,122,120;,
 3;123,124,122;,
 3;125,126,124;,
 4;116,115,127,128;,
 4;115,118,129,127;,
 4;118,120,130,129;,
 4;120,122,131,130;,
 4;122,124,132,131;,
 4;124,126,133,132;,
 4;128,127,134,135;,
 4;127,129,136,134;,
 4;129,130,137,136;,
 4;130,131,138,137;,
 4;131,132,139,138;,
 4;132,133,140,139;,
 4;135,134,141,142;,
 4;134,136,143,141;,
 4;136,137,144,143;,
 4;137,138,145,144;,
 4;138,139,146,145;,
 4;139,140,147,146;,
 4;142,141,148,149;,
 4;141,143,150,148;,
 4;143,144,151,150;,
 4;144,145,152,151;,
 4;145,146,153,152;,
 4;146,147,154,153;,
 3;149,148,155;,
 3;148,150,156;,
 3;150,151,157;,
 3;151,152,158;,
 3;152,153,159;,
 3;153,154,160;,
 3;161,162,163;,
 3;164,165,162;,
 3;166,167,165;,
 3;168,169,167;,
 3;170,171,169;,
 3;172,173,171;,
 4;163,162,174,175;,
 4;162,165,176,174;,
 4;165,167,177,176;,
 4;167,169,178,177;,
 4;169,171,179,178;,
 4;171,173,180,179;,
 4;175,174,181,182;,
 4;174,176,183,181;,
 4;176,177,184,183;,
 4;177,178,185,184;,
 4;178,179,186,185;,
 4;179,180,187,186;,
 4;182,181,188,189;,
 4;181,183,190,188;,
 4;183,184,191,190;,
 4;184,185,192,191;,
 4;185,186,193,192;,
 4;186,187,194,193;,
 4;189,188,195,196;,
 4;188,190,197,195;,
 4;190,191,198,197;,
 4;191,192,199,198;,
 4;192,193,200,199;,
 4;193,194,201,200;,
 3;196,195,202;,
 3;195,197,203;,
 3;197,198,204;,
 3;198,199,205;,
 3;199,200,206;,
 3;200,201,207;;
 
 MeshMaterialList {
  8;
  133;
  7,
  6,
  7,
  6,
  7,
  7,
  6,
  6,
  6,
  6,
  6,
  6,
  1,
  1,
  0,
  0,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  3,
  2,
  4,
  4,
  4,
  4,
  4,
  4,
  5,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6,
  6;;
  Material {
   0.109600;0.602400;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.483200;0.762400;1.000000;;
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
    "C:\\Users\\student\\Pictures\\3D�f��\\toire.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Pictures\\3D�f��\\toireman.png";
   }
  }
  Material {
   0.172800;0.179200;0.426400;1.000000;;
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
    "C:\\Users\\student\\Pictures\\3D�f��\\TOILET.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Pictures\\3D�f��\\toirekabe.jpg";
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
  115;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  -0.999901;0.014062;0.000000;,
  -0.999901;0.014062;0.000000;,
  0.999990;-0.004394;-0.000000;,
  -0.999852;-0.017212;-0.000000;,
  0.999901;-0.014062;-0.000000;,
  -0.999852;-0.017212;-0.000000;,
  0.999901;-0.014062;-0.000001;,
  -0.999901;0.014061;-0.000000;,
  0.999990;-0.004394;-0.000001;,
  0.000000;-0.017452;-0.999848;,
  0.999990;-0.004394;-0.000000;,
  0.999990;-0.004394;0.000001;,
  0.999990;-0.004394;0.000001;,
  0.999990;-0.004394;0.000001;,
  0.999990;-0.004394;-0.000000;,
  -0.999901;0.014061;0.000000;,
  -0.999901;0.014061;0.000000;,
  -0.999901;0.014062;0.000000;,
  -0.999901;0.014062;0.000000;,
  0.999901;-0.014062;-0.000000;,
  0.999901;-0.014062;0.000000;,
  0.999901;-0.014062;0.000000;,
  0.999901;-0.014062;0.000000;,
  0.999901;-0.014062;-0.000000;,
  -0.999852;-0.017212;-0.000000;,
  -0.999852;-0.017212;-0.000000;,
  -0.999852;-0.017212;-0.000000;,
  -0.999852;-0.017212;-0.000000;,
  -0.999852;-0.017212;-0.000000;,
  0.999901;-0.014062;-0.000001;,
  0.999901;-0.014062;-0.000001;,
  -0.999852;-0.017212;-0.000000;,
  -0.999852;-0.017212;-0.000000;,
  0.999990;-0.004394;-0.000001;,
  0.999990;-0.004395;-0.000001;,
  -0.999901;0.014061;-0.000000;,
  -0.999901;0.014062;-0.000000;,
  0.000000;-0.017452;-0.999848;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.014016;-0.999902;,
  -0.000002;1.000000;0.000000;,
  -0.000003;0.895317;-0.445429;,
  0.385752;0.895317;-0.222716;,
  0.385750;0.895317;0.222718;,
  -0.000003;0.895315;0.445434;,
  -0.385755;0.895316;0.222718;,
  -0.385757;0.895315;-0.222715;,
  -0.000004;0.556613;-0.830772;,
  0.719466;0.556617;-0.415387;,
  0.719465;0.556618;0.415388;,
  -0.000003;0.556615;0.830770;,
  -0.719467;0.556617;0.415385;,
  -0.719469;0.556615;-0.415385;,
  -0.000004;0.000000;-1.000000;,
  0.866022;0.000000;-0.500006;,
  0.866024;-0.000001;0.500003;,
  -0.000003;-0.000000;1.000000;,
  -0.866026;0.000001;0.500000;,
  -0.866024;-0.000000;-0.500003;,
  -0.000004;-0.556621;-0.830766;,
  0.719461;-0.556625;-0.415384;,
  0.719459;-0.556627;0.415385;,
  -0.000004;-0.556623;0.830765;,
  -0.719463;-0.556624;0.415383;,
  -0.719464;-0.556623;-0.415382;,
  -0.000003;-0.895321;-0.445421;,
  0.385746;-0.895321;-0.222712;,
  0.385743;-0.895321;0.222715;,
  -0.000003;-0.895319;0.445426;,
  -0.385748;-0.895319;0.222714;,
  -0.385750;-0.895319;-0.222711;,
  -0.000002;-1.000000;0.000000;,
  -0.000000;1.000000;0.000000;,
  -0.000000;0.895319;-0.445425;,
  0.385750;0.895319;-0.222713;,
  0.385749;0.895318;0.222717;,
  -0.000000;0.895316;0.445432;,
  -0.385749;0.895318;0.222717;,
  -0.385750;0.895319;-0.222713;,
  0.000001;0.556616;-0.830770;,
  0.719468;0.556615;-0.415387;,
  0.719467;0.556614;0.415388;,
  0.000001;0.556614;0.830771;,
  -0.719462;0.556622;0.415387;,
  -0.719463;0.556622;-0.415386;,
  0.000002;-0.000000;-1.000000;,
  0.866024;-0.000005;-0.500002;,
  0.866025;-0.000007;0.500001;,
  0.000003;-0.000004;1.000000;,
  -0.866024;-0.000001;0.500003;,
  -0.866022;-0.000000;-0.500006;,
  0.000000;-0.556623;-0.830765;,
  0.719464;-0.556625;-0.415380;,
  0.719462;-0.556623;0.415386;,
  0.000003;-0.556625;0.830764;,
  -0.719456;-0.556632;0.415383;,
  -0.719458;-0.556629;-0.415384;,
  -0.000001;-0.895322;-0.445419;,
  0.385747;-0.895321;-0.222708;,
  0.385746;-0.895320;0.222715;,
  0.000001;-0.895319;0.445425;,
  -0.385742;-0.895322;0.222713;,
  -0.385745;-0.895322;-0.222710;,
  -0.000000;-1.000000;0.000000;;
  133;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;8,16,17,18;,
  4;8,18,19,20;,
  4;7,21,22,6;,
  4;7,6,23,24;,
  4;10,25,26,27;,
  4;10,27,28,29;,
  4;9,30,31,32;,
  4;9,32,33,34;,
  4;12,35,25,10;,
  4;12,10,29,36;,
  4;11,37,30,9;,
  4;11,9,34,38;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;14,39,16,8;,
  4;14,8,20,40;,
  4;13,41,21,7;,
  4;13,7,24,42;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;15,15,15,15;,
  4;43,43,43,43;,
  4;44,44,44,44;,
  4;45,45,45,45;,
  4;46,46,46,46;,
  4;47,47,47,47;,
  4;48,48,48,48;,
  4;49,49,49,49;,
  4;50,50,50,50;,
  3;51,53,52;,
  3;51,54,53;,
  3;51,55,54;,
  3;51,56,55;,
  3;51,57,56;,
  3;51,52,57;,
  4;52,53,59,58;,
  4;53,54,60,59;,
  4;54,55,61,60;,
  4;55,56,62,61;,
  4;56,57,63,62;,
  4;57,52,58,63;,
  4;58,59,65,64;,
  4;59,60,66,65;,
  4;60,61,67,66;,
  4;61,62,68,67;,
  4;62,63,69,68;,
  4;63,58,64,69;,
  4;64,65,71,70;,
  4;65,66,72,71;,
  4;66,67,73,72;,
  4;67,68,74,73;,
  4;68,69,75,74;,
  4;69,64,70,75;,
  4;70,71,77,76;,
  4;71,72,78,77;,
  4;72,73,79,78;,
  4;73,74,80,79;,
  4;74,75,81,80;,
  4;75,70,76,81;,
  3;76,77,82;,
  3;77,78,82;,
  3;78,79,82;,
  3;79,80,82;,
  3;80,81,82;,
  3;81,76,82;,
  3;83,85,84;,
  3;83,86,85;,
  3;83,87,86;,
  3;83,88,87;,
  3;83,89,88;,
  3;83,84,89;,
  4;84,85,91,90;,
  4;85,86,92,91;,
  4;86,87,93,92;,
  4;87,88,94,93;,
  4;88,89,95,94;,
  4;89,84,90,95;,
  4;90,91,97,96;,
  4;91,92,98,97;,
  4;92,93,99,98;,
  4;93,94,100,99;,
  4;94,95,101,100;,
  4;95,90,96,101;,
  4;96,97,103,102;,
  4;97,98,104,103;,
  4;98,99,105,104;,
  4;99,100,106,105;,
  4;100,101,107,106;,
  4;101,96,102,107;,
  4;102,103,109,108;,
  4;103,104,110,109;,
  4;104,105,111,110;,
  4;105,106,112,111;,
  4;106,107,113,112;,
  4;107,102,108,113;,
  3;108,109,114;,
  3;109,110,114;,
  3;110,111,114;,
  3;111,112,114;,
  3;112,113,114;,
  3;113,108,114;;
 }
 MeshTextureCoords {
  208;
  0.827130;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.796260;1.000000;,
  0.806200;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.837070;1.000000;,
  0.505340;1.000000;,
  0.494790;0.000000;,
  0.486670;1.000000;,
  0.497210;0.000000;,
  0.236560;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.186920;0.000000;,
  0.184380;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.234020;0.000000;,
  0.505340;0.000000;,
  0.236560;0.000000;,
  0.236560;0.000000;,
  0.505340;0.000000;,
  0.234020;1.000000;,
  0.497210;1.000000;,
  0.497210;1.000000;,
  0.234020;1.000000;,
  0.796260;0.000000;,
  0.505340;0.000000;,
  0.796260;0.000000;,
  0.497210;1.000000;,
  0.806200;1.000000;,
  0.806200;1.000000;,
  0.796260;0.000000;,
  0.505340;0.000000;,
  0.497210;1.000000;,
  0.806200;1.000000;,
  0.505340;0.000000;,
  0.236560;0.000000;,
  0.234020;1.000000;,
  0.497210;1.000000;,
  1.000000;0.309740;,
  0.000000;1.000000;,
  0.000000;0.313170;,
  0.000000;0.000000;,
  1.000000;0.313150;,
  1.000000;1.000000;,
  0.000000;0.309720;,
  1.000000;0.000000;,
  1.000000;0.313170;,
  1.000000;1.000000;,
  0.799370;0.313170;,
  1.000000;0.000000;,
  0.169790;0.309740;,
  0.162930;1.000000;,
  0.000000;1.000000;,
  0.000000;0.309740;,
  0.000000;0.000000;,
  0.172870;0.000000;,
  0.507730;0.309730;,
  0.513330;1.000000;,
  0.505210;0.000000;,
  0.000000;0.313150;,
  0.000000;0.000000;,
  0.235770;0.313150;,
  0.000000;1.000000;,
  1.000000;0.309720;,
  1.000000;1.000000;,
  0.815620;1.000000;,
  0.813870;0.309730;,
  0.813080;0.000000;,
  1.000000;0.000000;,
  0.235770;0.312070;,
  0.502800;0.312070;,
  0.502790;0.313160;,
  0.502800;0.312090;,
  0.799360;0.312100;,
  0.502800;0.311710;,
  0.799360;0.311720;,
  0.235770;0.311670;,
  0.502800;0.311680;,
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
  0.083330;0.000000;,
  0.166670;0.166670;,
  0.000000;0.166670;,
  0.250000;0.000000;,
  0.333330;0.166670;,
  0.416670;0.000000;,
  0.500000;0.166670;,
  0.583330;0.000000;,
  0.666670;0.166670;,
  0.750000;0.000000;,
  0.833330;0.166670;,
  0.916670;0.000000;,
  1.000000;0.166670;,
  0.166670;0.333330;,
  0.000000;0.333330;,
  0.333330;0.333330;,
  0.500000;0.333330;,
  0.666670;0.333330;,
  0.833330;0.333330;,
  1.000000;0.333330;,
  0.166670;0.500000;,
  0.000000;0.500000;,
  0.333330;0.500000;,
  0.500000;0.500000;,
  0.666670;0.500000;,
  0.833330;0.500000;,
  1.000000;0.500000;,
  0.166670;0.666670;,
  0.000000;0.666670;,
  0.333330;0.666670;,
  0.500000;0.666670;,
  0.666670;0.666670;,
  0.833330;0.666670;,
  1.000000;0.666670;,
  0.166670;0.833330;,
  0.000000;0.833330;,
  0.333330;0.833330;,
  0.500000;0.833330;,
  0.666670;0.833330;,
  0.833330;0.833330;,
  1.000000;0.833330;,
  0.083330;1.000000;,
  0.250000;1.000000;,
  0.416670;1.000000;,
  0.583330;1.000000;,
  0.750000;1.000000;,
  0.916670;1.000000;,
  0.083330;0.000000;,
  0.166670;0.166670;,
  0.000000;0.166670;,
  0.250000;0.000000;,
  0.333330;0.166670;,
  0.416670;0.000000;,
  0.500000;0.166670;,
  0.583330;0.000000;,
  0.666670;0.166670;,
  0.750000;0.000000;,
  0.833330;0.166670;,
  0.916670;0.000000;,
  1.000000;0.166670;,
  0.166670;0.333330;,
  0.000000;0.333330;,
  0.333330;0.333330;,
  0.500000;0.333330;,
  0.666670;0.333330;,
  0.833330;0.333330;,
  1.000000;0.333330;,
  0.166670;0.500000;,
  0.000000;0.500000;,
  0.333330;0.500000;,
  0.500000;0.500000;,
  0.666670;0.500000;,
  0.833330;0.500000;,
  1.000000;0.500000;,
  0.166670;0.666670;,
  0.000000;0.666670;,
  0.333330;0.666670;,
  0.500000;0.666670;,
  0.666670;0.666670;,
  0.833330;0.666670;,
  1.000000;0.666670;,
  0.166670;0.833330;,
  0.000000;0.833330;,
  0.333330;0.833330;,
  0.500000;0.833330;,
  0.666670;0.833330;,
  0.833330;0.833330;,
  1.000000;0.833330;,
  0.083330;1.000000;,
  0.250000;1.000000;,
  0.416670;1.000000;,
  0.583330;1.000000;,
  0.750000;1.000000;,
  0.916670;1.000000;;
 }
}
