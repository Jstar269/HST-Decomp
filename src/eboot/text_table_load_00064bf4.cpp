/* HSTMATCH module=EBOOT.BIN start=0x00064bf4 end=0x00064f40 symbol=FUN_00064bf4 lang=c++ */
/* Clone of src/sharing_main/text_table_load_00065ab8.cpp (hstclone.py tag c145620): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Text-table loader: reads the file, turns CR into LF, skips a UTF-8 BOM, splits lines with strtok (FUN_00015ea0) into a vector of line pointers (the first one skipped when skip != 0), splits each non-empty line into a field vector (FUN_00064fd0), shrinks it to fit and appends it to the row vector, then shrinks the row vector to fit. */
struct InTag {};
struct FwdTag : InTag {};
struct Tag {};

struct It { char **p; It(char **x) : p(x) {} };
inline bool operator==(const It &a, const It &b) { return a.p == b.p; }
inline bool operator!=(const It &a, const It &b) { return !(a == b); }
struct Lines {
  char **data; unsigned size; unsigned capacity;
  It begin() const { return It(data); }
  It end() const { return It(data + size); }
};
struct Row { char **data; unsigned size; unsigned capacity; };
struct Rows { Row *data; unsigned size; unsigned capacity; };

struct Parser {
  virtual void v0();
  virtual void Clear();
  Rows rows;
};

extern "C" {
extern char *DAT_002d11cc;
char *FUN_0004cad0(const char *name, int *len, int align, int flag);
char *FUN_00015ea0(char *s, const char *delim);
int FUN_00014a40(const char *s);
void FUN_000104e0(void *p);
void FUN_0005da00(Lines *v, char **x, Tag);
void FUN_0005da44(Lines *v, int flag);
void FUN_00065e64(Rows *v, unsigned n);
void FUN_000660dc(Row *v, unsigned n);
void FUN_00064fd0(Row *v, char *line, const char *delim);
void FUN_00065ce0(Row *v, char **first, char **last, FwdTag);
void FUN_000661d8(Row *v, int flag);
void FUN_00064f40(Rows *v, Row *x);
void FUN_00065a88(Rows *v, Row *first, Row *last, FwdTag);
void FUN_00065fa0(Rows *v, int flag);
}

template <class T> inline void Swap(T &a, T &b) { T t = a; a = b; b = t; }

extern "C" unsigned char FUN_00064bf4(Parser *self, const char *name, int skip, const char *delim)
{
  int len;
  self->Clear();
  char *buf = FUN_0004cad0(name, &len, 16, 1);
  if (buf == 0 || len < 1)
    return 0;
  buf[len - 1] = 0;
  for (char *p = buf; *p != 0; p++) {
    if (*p == '\r')
      *p = '\n';
  }
  Lines lines;
  lines.data = 0;
  lines.size = 0;
  lines.capacity = 0;
  int count = 0;
  char *s = buf;
  if (s[0] == (char)0xef && s[1] == (char)0xbb && s[2] == (char)0xbf)
    s = buf + 3;
  char *tok;
  for (tok = FUN_00015ea0(s, DAT_002d11cc); tok != 0; tok = FUN_00015ea0(0, DAT_002d11cc)) {
    count++;
    if (count > 1 || skip == 0) {
      if (lines.size < lines.capacity) {
        lines.size = lines.size + 1;
        lines.data[lines.size - 1] = tok;
      } else {
        FUN_0005da00(&lines, &tok, Tag());
      }
    }
  }
  if (self->rows.capacity < 16)
    FUN_00065e64(&self->rows, 16);
  for (It it = lines.begin(); it != lines.end(); ++it.p) {
    if (FUN_00014a40(*it.p) != 0) {
      Row toks;
      toks.data = 0;
      toks.size = 0;
      toks.capacity = 0;
      if (toks.capacity < 16)
        FUN_000660dc(&toks, 16);
      FUN_00064fd0(&toks, *it.p, delim);
      Row tmp;
      tmp.data = 0;
      tmp.size = 0;
      tmp.capacity = 0;
      FUN_00065ce0(&tmp, toks.data, toks.data + toks.size, FwdTag());
      Swap(tmp.capacity, toks.capacity);
      Swap(tmp.data, toks.data);
      Swap(tmp.size, toks.size);
      FUN_000661d8(&tmp, 0);
      if (toks.size != 0)
        FUN_00064f40(&self->rows, &toks);
      FUN_000661d8(&toks, 0);
    }
  }
  FUN_000104e0(buf);
  Rows tmp2;
  tmp2.data = 0;
  tmp2.size = 0;
  tmp2.capacity = 0;
  FUN_00065a88(&tmp2, self->rows.data, self->rows.data + self->rows.size, FwdTag());
  Swap(tmp2.capacity, self->rows.capacity);
  Swap(tmp2.data, self->rows.data);
  Swap(tmp2.size, self->rows.size);
  FUN_00065fa0(&tmp2, 0);
  FUN_0005da44(&lines, 0);
  return 1;
}
