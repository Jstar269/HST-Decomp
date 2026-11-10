/* HSTMATCH module=EBOOT.BIN start=0x000f19e4 end=0x000f1ad4 symbol=FUN_000f19e4 lang=c++ */
extern "C" float DAT_002d7800;
extern "C" void FUN_0010e5b8(int, int, int, int, int, int, int, int);
extern "C" void FUN_000f19e4(float f, char *o)
{
    *(float *)(o + 0x80) = f * DAT_002d7800;
    FUN_0010e5b8(*(int *)(o + 0x6c), 0, 0, (int)(*(float *)(o + 0x80) + 51.0f), 13, 0, 0, 3);
    
    char *e = *(char **)(o + 0x58);
    short old = *(short *)(e + 4);
    float div = *(float *)(o + 0x80) / 3.0f;
    short nv = (short)(int)(*(float *)(o + 0x70) + div);
    *(short *)(e + 4) = nv;
    *(short *)(e + 8) = (short)(*(short *)(e + 8) + (short)(nv - old));
    
    e = *(char **)(o + 0x5c);
    old = *(short *)(e + 4);
    div = *(float *)(o + 0x80) / 3.0f;
    nv = (short)(int)(*(float *)(o + 0x70) + div * 2.0f);
    *(short *)(e + 4) = nv;
    *(short *)(e + 8) = (short)(*(short *)(e + 8) + (short)(nv - old));
}
// near miss from lane w3n2 (unreviewed)
