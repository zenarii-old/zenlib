#define EPSILON 0.00001
#define SquareRoot sqrt

//
// ~Vectors
//
#define V2Scale(v, s) v2((v).x * (s), (v).y * (s))
#define V3Scale(v, s) v3((v).x * (s), (v).y * (s), (v).z * (s))
#define V4Scale(v, s) v4((v).x * (s), (v).y * (s), (v).z * (s), (v).w * (s))

// NOTE(Abi): Component wise operations
#define VecOp(name, op) \
internal inline v2 name##V2(v2 a, v2 b) { return v2(a.x op b.x, a.y op b.y); }\
internal inline v3 name##V3(v3 a, v3 b) { return v3(a.x op b.x, a.y op b.y, a.z op b.z); }\
internal inline v4 name##V4(v4 a, v4 b) {\
return v4(a.x op b.x, a.y op b.y, a.z op b.z, a.w op b.w); }

VecOp(Subtract, -)
VecOp(Add, +)
VecOp(Divide, /)
VecOp(Multiply, *)

internal inline v3
NormaliseV3(v3 v) {
    f32 Squared = v.x * v.x + v.y * v.y + v.z * v.z;
    if(Squared > 1.f + EPSILON || Squared < 1.f - EPSILON) {
        f32 Length = sqrt(Squared);
        v = v3(v.x/Length, v.y/Length, v.z/Length);
    }
    return v;
}

internal inline v3
CrossV3(v3 a, v3 b) {
    v3 Cross = {
        a.y * b.z - b.y * a.z,
        b.x * a.z - a.x * b.z,
        a.x * b.y - b.x * a.y
    };
    return Cross;
}

internal inline f32
DotV3(v3 a, v3 b) {
    v3 c = MultiplyV3(a, b);
    return c.x + c.y + c.z;
}

//
// ~Matrices
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
#define Matrix4x4(...) (matrix4x4){{{__VA_ARGS__}}}

internal inline matrix4x4
DiagMatrix(f32 a, f32 b, f32 c, f32 d) {
    matrix4x4 Result = {0};
    
    Result._11 = a;
    Result._22 = b;
    Result._33 = c;
    Result._44 = d;
    
    return Result;
}

internal inline matrix4x4
ScaleMatrix(f32 x, f32 y, f32 z) {
    matrix4x4 Result = {0};
    
    Result._11 = x;
    Result._22 = y;
    Result._33 = z;
    Result._44 = 1;
    
    return Result;
}

internal inline matrix4x4
IdentityMatrix() {
    matrix4x4 Iden = {0};
    Iden._11 = 1.0;
    Iden._22 = 1.0;
    Iden._33 = 1.0;
    Iden._44 = 1.0;
    return Iden;
}

internal inline matrix4x4
TranslationMatrix(f32 x, f32 y, f32 z) {
    matrix4x4 Trans = Matrix4x4(1.f, 0.f, 0.f, x,
                                0.f, 1.f, 0.f, y,
                                0.f, 0.f, 1.f, z,
                                0.f, 0.f, 0.f, 1.f);
    return Trans;
}

internal inline matrix4x4
TranslationMatrixV3(v3 v) {
    matrix4x4 Trans = Matrix4x4(1.f, 0.f, 0.f, v.x,
                                0.f, 1.f, 0.f, v.y,
                                0.f, 0.f, 1.f, v.z,
                                0.f, 0.f, 0.f, 1.f);
    return Trans;
}

internal inline matrix4x4
FrustrumMatrix(f32 Width, f32 Height, f32 Near, f32 Far) {
    f32 Diff = Far - Near;
    matrix4x4 Result = Matrix4x4((Near*2.f)/Width,  0.f, 0.f, 0.f,
                                 0.f, (Near*2.f)/Height, 0.f, 0.f,
                                 0.f, 0.f, -(Far+Near)/Diff, -(Far*Near*2.f)/Diff,
                                 0.f, 0.f, -1.f, 0.f);
    return Result;
}

internal inline matrix4x4
PerspectiveMatrix(f32 Angle, f32 Aspect, f32 Near, f32 Far) {
    f32 Height = 2.f * Near * tan(Angle * 0.5f);
    f32 Width = Height * Aspect;
    matrix4x4 Result = FrustrumMatrix(Width, Height, Near, Far);
    return Result;
}
// TODO(Abi): may need to negate Eye?
internal inline matrix4x4
LookAt(v3 Eye, v3 Target, v3 Up) {
    matrix4x4 Result = {0};
    
    v3 z = SubtractV3(Eye, Target);
    z = NormaliseV3(z);
    v3 x = CrossV3(Up, z);
    x = NormaliseV3(x);
    v3 y = CrossV3(x, z);
    
    Result = Matrix4x4(x.x, x.y, x.z, -DotV3(x, Eye),
                       y.x, y.y, y.z, -DotV3(y, Eye),
                       z.x, z.y, z.z, -DotV3(z, Eye),
                       0.f, 0.f, 0.f, 1.f);
    
    return Result;
}

//
// ~Operations
//
internal inline matrix4x4
MultM4M4(matrix4x4 m1, matrix4x4 m2) {
    matrix4x4 Result = {0};
    for(i32 n = 0; n < 4; ++n) {
        for(i32 m = 0; m < 4; ++m) {
            for(i32 i = 0; i < 4; ++i) {
                Result.Elements[n][m] += m1.Elements[n][i] * m2.Elements[i][m];
            }
        }
    }
    return Result;
}

// TODO(Abi): Test
internal inline v4
MultM4V4(matrix4x4 m, v4 v) {
    v4 Result = {0};
    Result.x = (v.x * m._11) + (v.y * m._12) + (v.z * m._13) + (v.w * m._14);
    Result.y = (v.x * m._21) + (v.y * m._22) + (v.z * m._23) + (v.w * m._24);
    Result.z = (v.x * m._31) + (v.y * m._32) + (v.z * m._33) + (v.w * m._34);
    Result.w = (v.x * m._41) + (v.y * m._42) + (v.z * m._43) + (v.w * m._44);
    return Result;
}

internal inline matrix4x4
TransposeMatrix(matrix4x4 mat) {
    matrix4x4 mT = {0};
    
    for(i32 n = 0; n < 4; ++n) {
        for(i32 m = 0; m < 4; ++m) {
            mT.Elements[n][m] = mat.Elements[m][n];
        }
    }
    
    return mT;
}

//
// ~Debug
//

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
