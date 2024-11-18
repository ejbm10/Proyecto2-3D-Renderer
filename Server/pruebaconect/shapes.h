namespace shapes {

const int vertexCount = 36;
const int triangleCount = 12;

const float vertices[][3] = {
-0.5000, -0.5000, 0.5000,
0.5000, -0.5000, 0.5000,
0.5000, 0.5000, 0.5000,
-0.5000, -0.5000, 0.5000,
0.5000, 0.5000, 0.5000,
-0.5000, 0.5000, 0.5000,
-0.5000, -0.5000, -0.5000,
0.5000, -0.5000, -0.5000,
0.5000, 0.5000, -0.5000,
-0.5000, -0.5000, -0.5000,
0.5000, 0.5000, -0.5000,
-0.5000, 0.5000, -0.5000,
-0.5000, -0.5000, 0.5000,
0.5000, -0.5000, 0.5000,
0.5000, -0.5000, -0.5000,
-0.5000, -0.5000, 0.5000,
0.5000, -0.5000, -0.5000,
-0.5000, -0.5000, -0.5000,
0.5000, 0.5000, 0.5000,
-0.5000, 0.5000, 0.5000,
-0.5000, 0.5000, -0.5000,
0.5000, 0.5000, 0.5000,
-0.5000, 0.5000, -0.5000,
0.5000, 0.5000, -0.5000,
0.5000, -0.5000, 0.5000,
0.5000, 0.5000, 0.5000,
0.5000, 0.5000, -0.5000,
0.5000, -0.5000, 0.5000,
0.5000, 0.5000, -0.5000,
0.5000, -0.5000, -0.5000,
-0.5000, -0.5000, 0.5000,
-0.5000, 0.5000, 0.5000,
-0.5000, 0.5000, -0.5000,
-0.5000, -0.5000, 0.5000,
-0.5000, 0.5000, -0.5000,
-0.5000, -0.5000, -0.5000
};

const unsigned short triangles[][3] = {
0, 1, 2,
3, 4, 5,
6, 7, 8,
9, 10, 11,
12, 13, 14,
15, 16, 17,
18, 19, 20,
21, 22, 23,
24, 25, 26,
27, 28, 29,
30, 31, 32,
33, 34, 35
};

const float triangleNormals[][3] = {
0.0000, 0.0000, 1.0000,
0.0000, 0.0000, 1.0000,
0.0000, 0.0000, 1.0000,
0.0000, 0.0000, 1.0000,
-0.0000, 1.0000, 0.0000,
0.0000, 1.0000, 0.0000,
-0.0000, -1.0000, 0.0000,
0.0000, -1.0000, -0.0000,
-1.0000, 0.0000, 0.0000,
-1.0000, 0.0000, 0.0000,
-1.0000, 0.0000, 0.0000,
-1.0000, 0.0000, 0.0000
};

} // namespace shapes

