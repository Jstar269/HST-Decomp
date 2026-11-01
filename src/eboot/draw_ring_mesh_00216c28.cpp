/* HSTMATCH module=EBOOT.BIN start=0x00216c28 end=0x00216d60 symbol=FUN_00216c28 lang=c++ */
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
void FUN_00051490(void *tex, int, int, int w, int h, int, int);
void FUN_00052154(void *tex, int, float, float, float, float, float, float, float, float, float, int, int);
void FUN_0005185c(void *tex);

void FUN_00216c28(RingMesh *self) {
    if (self->active == 0)
        return;

    FUN_00051490(self->tex, 0, 0, 480, 272, 0, 0);

    for (int i = 0; i < 16; ++i) {
        int next = (i + 1) % 16;
        FUN_00052154(self->tex, self->color1,
                     self->outer[i].x, self->outer[i].y, 65535.0f,
                     self->outer[next].x, self->outer[next].y, 65535.0f,
                     self->inner[i].x, self->inner[i].y, 65535.0f,
                     self->color1, self->color0);

        FUN_00052154(self->tex, self->color1,
                     self->outer[next].x, self->outer[next].y, 65535.0f,
                     self->inner[i].x, self->inner[i].y, 65535.0f,
                     self->inner[next].x, self->inner[next].y, 65535.0f,
                     self->color0, self->color0);
    }

    FUN_0005185c(self->tex);
}
}
