/* HSTMATCH module=EBOOT.BIN start=0x00092e50 end=0x00092ff8 symbol=FUN_00092e50 lang=c++ */
struct Rec { char pad[0x62]; char b62; };
struct Info { char pad[0x1b4]; int id; short f1b8; };
struct Name { char pad[4]; char *text; };
struct Ref { char pad[4]; Name **names; };
struct Self {
  char pad[0x50];
  Info *info;
  char pad54[0x1c];
  Ref *ref;
};
extern "C" {
extern Rec *DAT_00338b08;
extern char DAT_002d29b4[];
extern void FUN_000149a8(char *, char *);
extern void FUN_00014b74(char *, char *, unsigned);
extern char *FUN_00014cfc(char *, int);
extern void FUN_0005ec14(Info *, int, int, int, int, char *, char *, char *);
extern void FUN_00093094(Self *, char *, char *, int, int);
extern Rec *FUN_000d8c24(Rec *, int, int);

void FUN_00092e50(Self *self, int a1, int a2, int a3, int a4, int flag)
{
  char buf1[64];
  char buf2[64];
  if (self->info != 0) {
    int id = self->info->id;
    Rec *rec = FUN_000d8c24(DAT_00338b08, 0, id);
    if (rec != 0) {
      bool ok = false;
      char *pc = 0;
      if (flag != 0) pc = DAT_002d29b4;
      if ((((int)rec->b62 << 28) >> 28) == 0) {
        if (flag != 0 && self->ref != 0) {
          Name *n = *self->ref->names;
          char *s2;
          if (n != 0 && (s2 = n->text, s2 != 0)) {
            char *colon = FUN_00014cfc(s2, ':');
            char *start = colon != 0 ? colon + 1 : s2;
            char *us = FUN_00014cfc(s2, '_');
            if (us != 0) {
              int len = (us + 1) - start;
              FUN_00014b74(buf1, start, len);
              buf1[len] = 0;
              FUN_000149a8(buf2, buf1);
              ok = true;
            }
          }
        
        } else {
          ok = true;
        }
      } else {
        FUN_00093094(self, buf1, buf2, id, self->info->f1b8);
        ok = true;
      }
      if (ok) {
        FUN_0005ec14(self->info, a1, a2, a3, a4, pc, buf1, buf2);
      }
    }
  }
}
}

// near miss api-hstm-20261121: 106/106 words, 11 differ (was 75): only the colouring of the two string pointers in the name-split
// block is swapped (retail: text s2, later reused for the length; start s3 = the dead flag parameter's register; ours text s3,
// start s2). Found: the `flag != 0 && ref != 0` guard with `else ok = true` after it (retail's shared ok block), buf1 declared
// before buf2 (strncpy dest at sp+112), `start = colon ? colon + 1 : text`, no `int` dead locals. Declaration orders at block
// level, int-typed text/start/len reuse, reusing the flag parameter as start (changes the prologue colouring): not exact.
// improved by lane api-hstm-20261121 (unreviewed): 75 -> 11 differing words
