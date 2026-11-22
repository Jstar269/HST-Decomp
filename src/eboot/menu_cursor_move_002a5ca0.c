/* HSTMATCH module=EBOOT.BIN start=0x002a5ca0 end=0x002a5e3c symbol=FUN_002a5ca0 lang=c99 */
/* Hand-written reconstruction from the disassembly (restructured from the near miss wip/draft_002a5ca0.c); exact match verified; names and types await human review. */
extern unsigned char FUN_001fe080(int, int, int);

typedef struct Menu {
  char pad000[0x17c];
  unsigned char active;
  char pad17d[0x880 - 0x17d];
  int mode;
  int cur;
  int in_up;
  int in_down;
  int in_left;
  int in_right;
} Menu;

void FUN_002a5ca0(Menu *m)
{
  unsigned char up = FUN_001fe080((int)&m->in_up, 0x10, 0x300);
  unsigned char down = FUN_001fe080((int)&m->in_down, 0x40, 0x300);
  unsigned char left = FUN_001fe080((int)&m->in_left, 0x80, 0x300);
  unsigned char right = FUN_001fe080((int)&m->in_right, 0x20, 0x300);
  if (m->active != 0) {
    if (up) {
      int v = m->cur;
      if ((unsigned)(v - 1) < 2) {
        m->cur = 13;
      } else if (v != 9) {
        m->cur = v - 1;
      }
    } else if (down) {
      int v = m->cur;
      if (v == 13) {
        switch (m->mode) {
        case 0: m->cur = 1; break;
        case 1: m->cur = 2; break;
        }
      } else if (v != 1 && v != 7) {
        m->cur = v + 1;
      }
    } else if (left) {
      if (m->cur < 9 || 13 < m->cur) m->cur = 9;
    } else if (right) {
      if (!(m->cur > 0 && m->cur <= 7)) {
        switch (m->mode) {
        case 0: m->cur = 1; break;
        case 1: m->cur = 2; break;
        }
      }
    }
  }
}
