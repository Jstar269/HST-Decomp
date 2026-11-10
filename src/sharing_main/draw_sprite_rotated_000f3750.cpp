/* HSTMATCH module=hstp_trans_main.prx start=0x000f3750 end=0x000f3ab0 symbol=FUN_000f3750 lang=c++ */
/* Clone of src/eboot/draw_sprite_rotated_000ee088.cpp (hstclone.py tag c085007): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" {
void FUN_00051e30(void *h, int a, int b, int c, int d, int e, int f);
void FUN_00052354(float u0, float v0, float x0, float y0, float u1, float v1, float x1, float y1, void *h, int color, float z);
void FUN_00052860(void *hd, int c0, int c1, int c2, float u0, float v0, float x0, float y0, float z0, float u1, float v1, float x1, float y1, float z1,
                  float u2, float v2, float x2, float y2, float z2);
void FUN_000522c0(void *h);
float FUN_0000bc4c(float a);
float FUN_0000bf4c(float a);
float FUN_0000bd44(float a);
}

extern "C" void FUN_000f3750(float x, float y, float a, float ang, float scale, float sx, float sy, float w, float hgt, void *hd, int n)
{
    unsigned int color;
    float c[8];
    ((unsigned char *)&color)[0] = 0x80;
    ((unsigned char *)&color)[1] = 0x80;
    ((unsigned char *)&color)[2] = 0x80;
    ((unsigned char *)&color)[3] = (unsigned char)(unsigned int)(a * 128.0f);
    float hw = w * scale / 2.0f;
    float hh = hgt * scale / 2.0f;
    float fx = (float)(int)(x - hw);
    fx += hw;
    float fy = (float)(int)(y - hh);
    fy += hh;
    c[0] = -hw;
    c[1] = -hh;
    c[2] = hw;
    c[3] = -hh;
    c[4] = -hw;
    c[5] = hh;
    c[6] = hw;
    c[7] = hh;
    if (ang != 0.0f) {
        for (int i = 0; i < 4; i++) {
            float px = c[i * 2];
            float py = c[i * 2 + 1];
            c[i * 2] = FUN_0000bd44(px * FUN_0000bc4c(ang) - py * FUN_0000bf4c(ang));
            c[i * 2 + 1] = FUN_0000bd44(px * FUN_0000bf4c(ang) + py * FUN_0000bc4c(ang));
        }
        FUN_00051e30(hd, 0, 0, 480, 272, 0, 0);
        unsigned int col = color;
        FUN_00052860(hd, col, col, col, sx, sy, fx + c[0], fy + c[1], (float)n, (sx + w), sy, fx + c[2], fy + c[3], (float)n, sx, (sy + hgt), fx + c[4], fy + c[5], (float)n);
        FUN_00052860(hd, col, col, col, (sx + w), sy, fx + c[2], fy + c[3], (float)n, (sx + w), (sy + hgt), fx + c[6], fy + c[7], (float)n, sx, (sy + hgt), fx + c[4], fy + c[5], (float)n);
        FUN_000522c0(hd);
    } else {
        FUN_00051e30(hd, 0, 0, 480, 272, 0, 0);
        FUN_00052354(sx, sy, fx + c[0], fy + c[1], sx + w, sy + hgt, fx + c[6], fy + c[7], hd, color, (float)n);
        FUN_000522c0(hd);
    }
}
