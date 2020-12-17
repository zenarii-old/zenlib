#define V2Scale(v, s) v2((v).x * (s), (v).y * (s))
#define V3Scale(v, s) v3((v).x * (s), (v).y * (s), (v).z * (s))
#define V4Scale(v, s) v4((v).x * (s), (v).y * (s), (v).z * (s), (v).w * (s))

//
// Matrices
//
// NOTE(Abi): Row Major.
typedef struct matrix4x4 matrix4x4;
struct matrix4x4 {
    union {
        struct {
            f32 _11, _12, _13, _14;
            f32 _21, _22, _23, _24;
            f32 _31, _32, _33, _34;
            f32 _41, _42, _43, _44;
        };
        f32 Elements[4][4];
    };
};

internal inline matrix4x4
DiagMatrix(f32 a, f32 b, f32 c, f32 d) {
    matrix4x4 Result = {0};
    
    Result._11 = a;
    Result._22 = b;
    Result._33 = c;
    Result._44 = d;
    
    return Result;
}

internal matrix4x4
IdentityMatrix() {
    matrix4x4 Iden = {0};
    Iden._11 = 1.0;
    Iden._22 = 1.0;
    Iden._33 = 1.0;
    Iden._44 = 1.0;
    return Iden;
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