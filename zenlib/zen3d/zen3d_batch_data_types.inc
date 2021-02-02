/* NOTE(Abi): name, stride, size, max */
ZEN3DBATCH(Shapes,   (7 * sizeof(f32)), (6 * Zen3D->Shapes.Stride),   1024)
ZEN3DBATCH(Textures, (9 * sizeof(f32)), (6 * Zen3D->Textures.Stride), 1024)
#undef ZEN3DBATCH