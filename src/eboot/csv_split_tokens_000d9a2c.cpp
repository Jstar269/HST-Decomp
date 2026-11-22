/* HSTMATCH module=EBOOT.BIN start=0x000d9a2c end=0x000d9b1c symbol=FUN_000d9a2c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Tag {};
struct Vec {
  char **data;
  unsigned int size;
  unsigned int cap;
};

extern "C" {
extern char DAT_002d6acc[];
void FUN_0005da00(Vec *, char *const *, Tag);
}

static inline void PushBack(Vec *v, char *const &x)
{
  if (v->size < v->cap) {
    v->size++;
    v->data[v->size - 1] = x;
  } else {
    FUN_0005da00(v, &x, Tag());
  }
}

extern "C" void FUN_000d9a2c(Vec *vec, char *s)
{
  char *tok = 0;
  while (*s != 0) {
    unsigned char c = (unsigned char)*s;
    if (c == ',' || c == '\r' || c == '\n') {
      *s++ = 0;
      if (tok == 0)
        tok = DAT_002d6acc;
      PushBack(vec, tok);
      tok = 0;
    } else {
      if (tok == 0)
        tok = s;
      s++;
    }
  }
}
