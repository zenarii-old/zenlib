/* NOTE(Abi): name, stride, size, max */
ZEN2DBATCH(Rect, (6 * sizeof(f32)), 6 * (6 * sizeof(f32)), 1024)
ZEN2DBATCH(Line, (6 * sizeof(f32)), 2 * (6 * sizeof(f32)), 1024)
#undef ZEN2DBATCH