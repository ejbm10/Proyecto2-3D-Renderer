namespace shapes {

const int vertexCount = 180;
const int triangleCount = 60;

const float vertices[][3] = {
0.0000, -0.5000, 0.0000,
0.5000, -0.5000, 0.0000,
0.4891, -0.5000, 0.1040,
0.0000, -0.5000, 0.0000,
0.4891, -0.5000, 0.1040,
0.4568, -0.5000, 0.2034,
0.0000, -0.5000, 0.0000,
0.4568, -0.5000, 0.2034,
0.4045, -0.5000, 0.2939,
0.0000, -0.5000, 0.0000,
0.4045, -0.5000, 0.2939,
0.3346, -0.5000, 0.3716,
0.0000, -0.5000, 0.0000,
0.3346, -0.5000, 0.3716,
0.2500, -0.5000, 0.4330,
0.0000, -0.5000, 0.0000,
0.2500, -0.5000, 0.4330,
0.1545, -0.5000, 0.4755,
0.0000, -0.5000, 0.0000,
0.1545, -0.5000, 0.4755,
0.0523, -0.5000, 0.4973,
0.0000, -0.5000, 0.0000,
0.0523, -0.5000, 0.4973,
-0.0523, -0.5000, 0.4973,
0.0000, -0.5000, 0.0000,
-0.0523, -0.5000, 0.4973,
-0.1545, -0.5000, 0.4755,
0.0000, -0.5000, 0.0000,
-0.1545, -0.5000, 0.4755,
-0.2500, -0.5000, 0.4330,
0.0000, -0.5000, 0.0000,
-0.2500, -0.5000, 0.4330,
-0.3346, -0.5000, 0.3716,
0.0000, -0.5000, 0.0000,
-0.3346, -0.5000, 0.3716,
-0.4045, -0.5000, 0.2939,
0.0000, -0.5000, 0.0000,
-0.4045, -0.5000, 0.2939,
-0.4568, -0.5000, 0.2034,
0.0000, -0.5000, 0.0000,
-0.4568, -0.5000, 0.2034,
-0.4891, -0.5000, 0.1040,
0.0000, -0.5000, 0.0000,
-0.4891, -0.5000, 0.1040,
-0.5000, -0.5000, -0.0000,
0.0000, -0.5000, 0.0000,
-0.5000, -0.5000, -0.0000,
-0.4891, -0.5000, -0.1040,
0.0000, -0.5000, 0.0000,
-0.4891, -0.5000, -0.1040,
-0.4568, -0.5000, -0.2034,
0.0000, -0.5000, 0.0000,
-0.4568, -0.5000, -0.2034,
-0.4045, -0.5000, -0.2939,
0.0000, -0.5000, 0.0000,
-0.4045, -0.5000, -0.2939,
-0.3346, -0.5000, -0.3716,
0.0000, -0.5000, 0.0000,
-0.3346, -0.5000, -0.3716,
-0.2500, -0.5000, -0.4330,
0.0000, -0.5000, 0.0000,
-0.2500, -0.5000, -0.4330,
-0.1545, -0.5000, -0.4755,
0.0000, -0.5000, 0.0000,
-0.1545, -0.5000, -0.4755,
-0.0523, -0.5000, -0.4973,
0.0000, -0.5000, 0.0000,
-0.0523, -0.5000, -0.4973,
0.0523, -0.5000, -0.4973,
0.0000, -0.5000, 0.0000,
0.0523, -0.5000, -0.4973,
0.1545, -0.5000, -0.4755,
0.0000, -0.5000, 0.0000,
0.1545, -0.5000, -0.4755,
0.2500, -0.5000, -0.4330,
0.0000, -0.5000, 0.0000,
0.2500, -0.5000, -0.4330,
0.3346, -0.5000, -0.3716,
0.0000, -0.5000, 0.0000,
0.3346, -0.5000, -0.3716,
0.4045, -0.5000, -0.2939,
0.0000, -0.5000, 0.0000,
0.4045, -0.5000, -0.2939,
0.4568, -0.5000, -0.2034,
0.0000, -0.5000, 0.0000,
0.4568, -0.5000, -0.2034,
0.4891, -0.5000, -0.1040,
0.0000, -0.5000, 0.0000,
0.4891, -0.5000, -0.1040,
0.5000, -0.5000, 0.0000,
0.0000, 0.5000, 0.0000,
0.5000, -0.5000, 0.0000,
0.4891, -0.5000, 0.1040,
0.0000, 0.5000, 0.0000,
0.4891, -0.5000, 0.1040,
0.4568, -0.5000, 0.2034,
0.0000, 0.5000, 0.0000,
0.4568, -0.5000, 0.2034,
0.4045, -0.5000, 0.2939,
0.0000, 0.5000, 0.0000,
0.4045, -0.5000, 0.2939,
0.3346, -0.5000, 0.3716,
0.0000, 0.5000, 0.0000,
0.3346, -0.5000, 0.3716,
0.2500, -0.5000, 0.4330,
0.0000, 0.5000, 0.0000,
0.2500, -0.5000, 0.4330,
0.1545, -0.5000, 0.4755,
0.0000, 0.5000, 0.0000,
0.1545, -0.5000, 0.4755,
0.0523, -0.5000, 0.4973,
0.0000, 0.5000, 0.0000,
0.0523, -0.5000, 0.4973,
-0.0523, -0.5000, 0.4973,
0.0000, 0.5000, 0.0000,
-0.0523, -0.5000, 0.4973,
-0.1545, -0.5000, 0.4755,
0.0000, 0.5000, 0.0000,
-0.1545, -0.5000, 0.4755,
-0.2500, -0.5000, 0.4330,
0.0000, 0.5000, 0.0000,
-0.2500, -0.5000, 0.4330,
-0.3346, -0.5000, 0.3716,
0.0000, 0.5000, 0.0000,
-0.3346, -0.5000, 0.3716,
-0.4045, -0.5000, 0.2939,
0.0000, 0.5000, 0.0000,
-0.4045, -0.5000, 0.2939,
-0.4568, -0.5000, 0.2034,
0.0000, 0.5000, 0.0000,
-0.4568, -0.5000, 0.2034,
-0.4891, -0.5000, 0.1040,
0.0000, 0.5000, 0.0000,
-0.4891, -0.5000, 0.1040,
-0.5000, -0.5000, -0.0000,
0.0000, 0.5000, 0.0000,
-0.5000, -0.5000, -0.0000,
-0.4891, -0.5000, -0.1040,
0.0000, 0.5000, 0.0000,
-0.4891, -0.5000, -0.1040,
-0.4568, -0.5000, -0.2034,
0.0000, 0.5000, 0.0000,
-0.4568, -0.5000, -0.2034,
-0.4045, -0.5000, -0.2939,
0.0000, 0.5000, 0.0000,
-0.4045, -0.5000, -0.2939,
-0.3346, -0.5000, -0.3716,
0.0000, 0.5000, 0.0000,
-0.3346, -0.5000, -0.3716,
-0.2500, -0.5000, -0.4330,
0.0000, 0.5000, 0.0000,
-0.2500, -0.5000, -0.4330,
-0.1545, -0.5000, -0.4755,
0.0000, 0.5000, 0.0000,
-0.1545, -0.5000, -0.4755,
-0.0523, -0.5000, -0.4973,
0.0000, 0.5000, 0.0000,
-0.0523, -0.5000, -0.4973,
0.0523, -0.5000, -0.4973,
0.0000, 0.5000, 0.0000,
0.0523, -0.5000, -0.4973,
0.1545, -0.5000, -0.4755,
0.0000, 0.5000, 0.0000,
0.1545, -0.5000, -0.4755,
0.2500, -0.5000, -0.4330,
0.0000, 0.5000, 0.0000,
0.2500, -0.5000, -0.4330,
0.3346, -0.5000, -0.3716,
0.0000, 0.5000, 0.0000,
0.3346, -0.5000, -0.3716,
0.4045, -0.5000, -0.2939,
0.0000, 0.5000, 0.0000,
0.4045, -0.5000, -0.2939,
0.4568, -0.5000, -0.2034,
0.0000, 0.5000, 0.0000,
0.4568, -0.5000, -0.2034,
0.4891, -0.5000, -0.1040,
0.0000, 0.5000, 0.0000,
0.4891, -0.5000, -0.1040,
0.5000, -0.5000, 0.0000
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
57, 58, 59,
60, 61, 62,
63, 64, 65,
66, 67, 68,
69, 70, 71,
72, 73, 74,
75, 76, 77,
78, 79, 80,
81, 82, 83,
84, 85, 86,
87, 88, 89,
90, 91, 92,
93, 94, 95,
96, 97, 98,
99, 100, 101,
102, 103, 104,
105, 106, 107,
108, 109, 110,
111, 112, 113,
114, 115, 116,
117, 118, 119,
120, 121, 122,
123, 124, 125,
126, 127, 128,
129, 130, 131,
132, 133, 134,
135, 136, 137,
138, 139, 140,
141, 142, 143,
144, 145, 146,
147, 148, 149,
150, 151, 152,
153, 154, 155,
156, 157, 158,
159, 160, 161,
162, 163, 164,
165, 166, 167,
168, 169, 170,
171, 172, 173,
174, 175, 176,
177, 178, 179
};

const float triangleNormals[][3] = {
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
0.0000, -1.0000, 0.0000,
-0.2068, -0.1034, 0.9729,
-0.4046, -0.1034, 0.9086,
-0.5846, -0.1034, 0.8047,
-0.7392, -0.1034, 0.6655,
-0.8614, -0.1034, 0.4973,
-0.9460, -0.1034, 0.3074,
-0.9892, -0.1034, 0.1040,
-0.9892, -0.1034, -0.1040,
-0.9460, -0.1034, -0.3074,
-0.8614, -0.1034, -0.4973,
-0.7392, -0.1034, -0.6655,
-0.5846, -0.1034, -0.8047,
-0.4046, -0.1034, -0.9086,
-0.2068, -0.1034, -0.9729,
0.0000, -0.1034, -0.9946,
0.2068, -0.1034, -0.9729,
0.4046, -0.1034, -0.9086,
0.5846, -0.1034, -0.8047,
0.7392, -0.1034, -0.6655,
0.8614, -0.1034, -0.4973,
0.9460, -0.1034, -0.3074,
0.9892, -0.1034, -0.1040,
0.9892, -0.1034, 0.1040,
0.9460, -0.1034, 0.3074,
0.8614, -0.1034, 0.4973,
0.7392, -0.1034, 0.6655,
0.5846, -0.1034, 0.8047,
0.4046, -0.1034, 0.9086,
0.2068, -0.1034, 0.9729,
-0.0000, -0.1034, 0.9946
};

} // namespace shapes

