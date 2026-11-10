/* HSTMATCH module=EBOOT.BIN start=0x000c8708 end=0x000c896c symbol=FUN_000c8708 lang=c++ */

struct Vec4 {
    float x, y, z, w;
};

union Matrix44 {
    float m[16];
    Vec4 row[4];
};

extern "C" {
    extern const float DAT_002bc410;
    extern const float DAT_002bc414;
    extern const float DAT_002bc418;
    extern const float DAT_002bc41c;
    extern const float DAT_002bc420;
    extern const float DAT_002bc424;
    extern const float DAT_002bc428;
    extern const float DAT_002bc42c;
    extern const float DAT_002bc430;
    extern const float DAT_002bc434;
    extern const float DAT_002bc438;
    extern const float DAT_002bc43c;
    extern const float DAT_002bc440;
    extern const float DAT_002bc444;
    extern const float DAT_002bc448;
    extern const float DAT_002bc44c;
    extern void *DAT_003386b0;

    void FUN_00022434(Vec4 *out, void *mat, const Vec4 *in);
    void FUN_00022518(Vec4 *out, const Vec4 *in);
    void FUN_00055240(Matrix44 *out, void *obj, int entry_val);
    void FUN_00022498(float scale, Vec4 *out, const Vec4 *in);
    void FUN_00022418(Vec4 *out, const Vec4 *in, const Vec4 *pos);
    void FUN_000ce754(float p1, float p2, float p3, void *obj, const Matrix44 *mat, int zero, int one);
}

extern "C" void FUN_000c8708(char *param_1, int param_2) {
    Matrix44 mat;
    Vec4 dir;
    Vec4 v0_copy;
    Vec4 scaled_dir2;
    Matrix44 bone_mat;
    Vec4 out_v1;
    Vec4 final_pos;
    Vec4 scaled_dir;
    Vec4 norm_dir;

    int boff = param_2 * 4;
    char *tab = (char *)(boff + (int)DAT_003386b0);
    void *a0_obj = *(void **)(*(char **)(*(char **)(tab + 0x70) + 0x5bc) + 0x58);

    mat.m[0] = DAT_002bc410;
    mat.m[1] = DAT_002bc414;
    mat.m[2] = DAT_002bc418;
    mat.m[3] = DAT_002bc41c;
    mat.m[4] = DAT_002bc420;
    mat.m[5] = DAT_002bc424;
    mat.m[6] = DAT_002bc428;
    mat.m[7] = DAT_002bc42c;
    mat.m[8] = DAT_002bc430;
    mat.m[9] = DAT_002bc434;
    mat.m[10] = DAT_002bc438;
    mat.m[11] = DAT_002bc43c;
    mat.m[12] = DAT_002bc440;
    mat.m[13] = DAT_002bc444;
    mat.m[14] = DAT_002bc448;
    mat.m[15] = DAT_002bc44c;

    void *v0 = *(void **)((char *)DAT_003386b0 + 0x88);
    v0_copy.x = *(float *)((char *)v0 + 0x10);
    v0_copy.y = *(float *)((char *)v0 + 0x14);
    v0_copy.z = *(float *)((char *)v0 + 0x18);
    v0_copy.w = *(float *)((char *)v0 + 0x1c);

    FUN_00022434(&out_v1, (char *)a0_obj + 0x60, &v0_copy);

    dir.x = out_v1.x;
    dir.y = 0.0f;
    dir.z = out_v1.z;
    dir.w = out_v1.w;

    FUN_00022518(&norm_dir, &dir);

    float zero = 0.0f;
    dir.w = -zero;
    dir.x = -norm_dir.x;
    dir.y = -norm_dir.y;
    dir.z = -norm_dir.z;
    *(int *)&norm_dir.w = 0;

    int eoff = param_2 * 36;
    int entry_val = *(int *)((char *)(eoff + (int)param_1) + 0xd8);
    FUN_00055240(&bone_mat, a0_obj, entry_val);

    Vec4 *bone_pos = (Vec4 *)(int)bone_mat.m;
    bone_pos += 3;
    FUN_00022498(0.14999999f, &scaled_dir, &dir);

    scaled_dir2 = scaled_dir;
    FUN_00022418(&final_pos, &scaled_dir2, bone_pos);

    mat.row[3] = final_pos;

    void *snd_obj = *(void **)(*(char **)((char *)DAT_003386b0 + 0x90) + 0x44);
    FUN_000ce754(3500.0f, 1.0f, 0.0f, snd_obj, &mat, 0, 1);
}
// near miss from lane ag4 (unreviewed)
// Near miss re-worked by lane w3f3: 33/153 words differ, all in one contiguous block from
// the FUN_00022498 call to the tail. Fixes that landed: (1) split the indexed global read so
// the addu is offset+base, (2) entry_val offset+base, (3) `dir.w = -zero;` with a local
// `float zero = 0.0f;` reproduces retail's mtc1 zero/neg.s instead of a 0x80000000 constant,
// (4) Vec4* third arg of FUN_00022418. Remaining: retail materialises &bone_mat.row[3]
// (sp+128) in the FUN_00022498 jal delay slot into s0 and uses `move a2,s0`; ours computes
// it after the scaled_dir2 copy (v0 base + 48). Tried: plain pointer local before/after the
// call, direct &bone_mat.row[3], bone_mat.row+3, reference param (all 42 diffs), int-cast
// (33). The delay-slot placement of the addiu is the last open item.
// improved by lane w3f3 (unreviewed): 67 -> 33 differing words
