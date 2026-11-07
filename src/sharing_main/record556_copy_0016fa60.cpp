/* HSTMATCH module=hstp_trans_main.prx start=0x0016fa60 end=0x0016ff4c symbol=FUN_0016fa60 lang=c++ */
/* Hand-written reconstruction: clone of the source of record556_copy_001634d4.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* The body is one memberwise copy of a 556-byte record (the stride 0x22c is confirmed by its accessor
   src/eboot/FUN_001639c0.c). The record layout is read straight off the retail copy sequence: a
   `short` at 0x00, a 16-byte char array at 0x02 (copied as a 2-byte-per-iteration loop), three chars at
   0x12-0x14, then floats with `int`/`short` members interleaved at the offsets below, ending in sixteen
   single chars at 0x21c-0x22b. The first parameter is unused in the body; the guard tests the
   destination. Member names are offsets because no caller identifies the record. */
struct Item556 {
  short m00;
  char m02[16];
  char m12;
  char m13;
  char m14;
  float m018;
  float m01c;
  float m020;
  float m024;
  float m028;
  float m02c;
  float m030;
  float m034;
  float m038;
  float m03c;
  float m040;
  float m044;
  float m048;
  float m04c;
  float m050;
  float m054;
  float m058;
  float m05c;
  float m060;
  float m064;
  float m068;
  float m06c;
  float m070;
  float m074;
  float m078;
  float m07c;
  float m080;
  float m084;
  float m088;
  float m08c;
  float m090;
  float m094;
  float m098;
  float m09c;
  float m0a0;
  float m0a4;
  int m0a8;
  float m0ac;
  float m0b0;
  float m0b4;
  float m0b8;
  float m0bc;
  short m0c0;
  short m0c2;
  float m0c4;
  float m0c8;
  float m0cc;
  float m0d0;
  float m0d4;
  float m0d8;
  float m0dc;
  int m0e0;
  float m0e4;
  float m0e8;
  float m0ec;
  int m0f0;
  float m0f4;
  float m0f8;
  float m0fc;
  int m100;
  float m104;
  float m108;
  float m10c;
  float m110;
  float m114;
  int m118;
  float m11c;
  float m120;
  float m124;
  float m128;
  int m12c;
  float m130;
  float m134;
  float m138;
  float m13c;
  float m140;
  float m144;
  int m148;
  float m14c;
  float m150;
  float m154;
  float m158;
  int m15c;
  float m160;
  float m164;
  float m168;
  float m16c;
  int m170;
  float m174;
  float m178;
  float m17c;
  float m180;
  float m184;
  int m188;
  float m18c;
  float m190;
  float m194;
  int m198;
  float m19c;
  float m1a0;
  float m1a4;
  float m1a8;
  float m1ac;
  int m1b0;
  float m1b4;
  float m1b8;
  float m1bc;
  float m1c0;
  float m1c4;
  int m1c8;
  float m1cc;
  float m1d0;
  float m1d4;
  float m1d8;
  int m1dc;
  float m1e0;
  float m1e4;
  float m1e8;
  float m1ec;
  int m1f0;
  float m1f4;
  float m1f8;
  float m1fc;
  float m200;
  int m204;
  float m208;
  float m20c;
  float m210;
  float m214;
  int m218;
  char m21c;
  char m21d;
  char m21e;
  char m21f;
  char m220;
  char m221;
  char m222;
  char m223;
  char m224;
  char m225;
  char m226;
  char m227;
  char m228;
  char m229;
  char m22a;
  char m22b;
};

extern "C" void FUN_0016fa60(int unused, Item556 *dst, const Item556 *src)
{
  if (dst != 0) {
    *dst = *src;
  }
}
