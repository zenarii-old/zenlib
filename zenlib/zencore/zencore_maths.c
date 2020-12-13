#define V2Scale(v, s) v2((v).x * (s), (v).y * (s))
#define V3Scale(v, s) v3((v).x * (s), (v).y * (s), (v).z * (s))
#define V4Scale(v, s) v4((v).x * (s), (v).y * (s), (v).z * (s), (v).w * (s))

//
// Matrices
//

typedef struct matrix4x4 matrix4x4;
struct matrix4x4 {
    union {
        struct {
            f32 _11, _12, _13, _14;
            f32 _21, _22, _23, _24;
            f32 _31, _32, _33, _34;
            f32 _41, _42, _43, _44;
        };
        f32 Elements[16];
    };
};

#define Matrix4x4(...) (matrix4x4){{{__VA_ARGS__}}};

internal matrix4x4
DiagMatrix(f32 a, f32 b, f32 c, f32 d) {
    matrix4x4 DiagonalMatrix = 
        Matrix4x4(a, 0, 0, 0,
                  0, b, 0, 0,
                  0, 0, c, 0,
                  0, 0, 0, d);
    Log("%f", DiagonalMatrix._44);
    return DiagonalMatrix;
}

internal matrix4x4
IdentityMatrix() {
    matrix4x4 I = 
        Matrix4x4(1, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 1);
    return I;
}

internal void
PrintMatrix4x4(matrix4x4 M) {
    Log("matrix: {"\
        "\n\t%f, %f, %f, %f"\
        "\n\t%f, %f, %f, %f"\
        "\n\t%f, %f, %f, %f"\
        "\n\t%f, %f, %f, %f"\
        "\n}\n",
        M._11, M._12, M._13, M._14,
        M._21, M._22, M._23, M._24,
        M._31, M._32, M._33, M._34,
        M._41, M._42, M._43, M._44);
}