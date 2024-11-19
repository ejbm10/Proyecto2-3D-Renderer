namespace shapes {

const int vertexCount = 60;
const int triangleCount = 20;

const float vertices[][3] = {
0.1508, -0.0000, -0.5000,
0.1508, -0.0000, 0.5000,
0.0356, 0.1585, 0.5000,
0.1508, -0.0000, -0.5000,
0.0356, 0.1585, 0.5000,
0.0356, 0.1585, -0.5000,
0.0356, 0.1585, -0.5000,
0.0356, 0.1585, 0.5000,
-0.1508, 0.0980, 0.5000,
0.0356, 0.1585, -0.5000,
-0.1508, 0.0980, 0.5000,
-0.1508, 0.0980, -0.5000,
-0.1508, 0.0980, -0.5000,
-0.1508, 0.0980, 0.5000,
-0.1508, -0.0980, 0.5000,
-0.1508, 0.0980, -0.5000,
-0.1508, -0.0980, 0.5000,
-0.1508, -0.0980, -0.5000,
-0.1508, -0.0980, -0.5000,
-0.1508, -0.0980, 0.5000,
0.0356, -0.1585, 0.5000,
-0.1508, -0.0980, -0.5000,
0.0356, -0.1585, 0.5000,
0.0356, -0.1585, -0.5000,
0.0356, -0.1585, -0.5000,
0.0356, -0.1585, 0.5000,
0.1508, 0.0000, 0.5000,
0.0356, -0.1585, -0.5000,
0.1508, 0.0000, 0.5000,
0.1508, 0.0000, -0.5000,
-0.0159, -0.0000, 0.5000,
0.1508, -0.0000, 0.5000,
0.0356, 0.1585, 0.5000,
-0.0159, -0.0000, 0.5000,
0.0356, 0.1585, 0.5000,
-0.1508, 0.0980, 0.5000,
-0.0159, -0.0000, 0.5000,
-0.1508, 0.0980, 0.5000,
-0.1508, -0.0980, 0.5000,
-0.0159, -0.0000, 0.5000,
-0.1508, -0.0980, 0.5000,
0.0356, -0.1585, 0.5000,
-0.0159, -0.0000, 0.5000,
0.0356, -0.1585, 0.5000,
0.1508, 0.0000, 0.5000,
-0.0159, -0.0000, -0.5000,
0.1508, -0.0000, -0.5000,
0.0356, 0.1585, -0.5000,
-0.0159, -0.0000, -0.5000,
0.0356, 0.1585, -0.5000,
-0.1508, 0.0980, -0.5000,
-0.0159, -0.0000, -0.5000,
-0.1508, 0.0980, -0.5000,
-0.1508, -0.0980, -0.5000,
-0.0159, -0.0000, -0.5000,
-0.1508, -0.0980, -0.5000,
0.0356, -0.1585, -0.5000,
-0.0159, -0.0000, -0.5000,
0.0356, -0.1585, -0.5000,
0.1508, 0.0000, -0.5000
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
33, 34, 35,
36, 37, 38,
39, 40, 41,
42, 43, 44,
45, 46, 47,
48, 49, 50,
51, 52, 53,
54, 55, 56,
57, 58, 59
};

const float triangleNormals[][3] = {
-0.8090, -0.5878, 0.0000,
-0.8090, -0.5878, 0.0000,
0.3090, -0.9511, 0.0000,
0.3090, -0.9511, 0.0000,
1.0000, 0.0000, -0.0000,
1.0000, 0.0000, 0.0000,
0.3090, 0.9511, -0.0000,
0.3090, 0.9511, 0.0000,
-0.8090, 0.5878, 0.0000,
-0.8090, 0.5878, 0.0000,
0.0000, 0.0000, 1.0000,
0.0000, 0.0000, 1.0000,
0.0000, 0.0000, 1.0000,
0.0000, 0.0000, 1.0000,
0.0000, 0.0000, 1.0000,
0.0000, 0.0000, -1.0000,
0.0000, 0.0000, -1.0000,
0.0000, 0.0000, -1.0000,
0.0000, 0.0000, -1.0000,
0.0000, 0.0000, -1.0000
};

} // namespace shapes

