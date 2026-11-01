/* HSTMATCH module=hstp_trans_main.prx start=0x00225c2c end=0x00225d64 symbol=FUN_00225c2c lang=c++ */
/* Clone of src/eboot/draw_ring_mesh_00216c28.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Draws a 16-segment textured 2D ring mesh connecting outer and inner vertex loops. */

struct Point {
    float x;
    float y;
};

struct RingMesh {
    unsigned char active;
    char pad[3];
    void *tex;
    Point outer[16];
    Point inner[16];
    int color0;
    int color1;
};

extern "C" {
void FUN_00051e30(void *tex, int, int, int w, int h, int, int);
void FUN_00052c14(void *tex, int, float, float, float, float, float, float, float, float, float, int, int);
void FUN_000522c0(void *tex);

void FUN_00225c2c(RingMesh *self) {
    if (self->active == 0)
        return;

    FUN_00051e30(self->tex, 0, 0, 480, 272, 0, 0);

    for (int i = 0; i < 16; ++i) {
        int next = (i + 1) % 16;
        FUN_00052c14(self->tex, self->color1,
                     self->outer[i].x, self->outer[i].y, 65535.0f,
                     self->outer[next].x, self->outer[next].y, 65535.0f,
                     self->inner[i].x, self->inner[i].y, 65535.0f,
                     self->color1, self->color0);

        FUN_00052c14(self->tex, self->color1,
                     self->outer[next].x, self->outer[next].y, 65535.0f,
                     self->inner[i].x, self->inner[i].y, 65535.0f,
                     self->inner[next].x, self->inner[next].y, 65535.0f,
                     self->color0, self->color0);
    }

    FUN_000522c0(self->tex);
}
}
