/* HSTMATCH module=EBOOT.BIN start=0x00190458 end=0x001905b8 symbol=FUN_00190458 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma inline_depth(8)

extern void *DAT_00309178;
extern int DAT_003415d4;

extern "C" {
void FUN_000009e4(void *);
void FUN_000104e0(void *);
void FUN_000462a0(void *, int);
void FUN_00004138(void *, int);
void FUN_00190b1c(void *, int);
void FUN_00000a00(void *);
void FUN_0001f0f0(void *, int);
}

struct VC {
  virtual ~VC();
};

struct SubObj {
  char pad[12];
  void *sub_field;
  char pad2[8];
  ~SubObj() {
    FUN_000462a0(&sub_field, -1);
    FUN_00004138(this, 0);
  }
};

struct Layer2 {
  ~Layer2() {
    FUN_00190b1c(this, 0);
  }
};

struct Layer1 : Layer2 {
  ~Layer1() {}
};

struct SubObjB : Layer1 {
  ~SubObjB() {}
};

struct Slots {
  char pad0[20];
  void *m_14;
  char pad1[156];
  SubObjB m_b4; // 180
  char pad2[591];
  SubObj m_304; // 772
  VC *m_31c;    // 796
  SubObj m_320; // 800
  char pad3[576];
  void *m_578;  // 1400

  ~Slots() {
    if (m_578 != 0) {
      delete[] (char *)m_578;
      m_578 = 0;
    }
    if (m_31c != 0) {
      delete m_31c;
      m_31c = 0;
    }
    if (m_14 != 0) {
      FUN_000104e0(m_14);
      m_14 = 0;
    }
  }
};

struct Obj {
  void *vtable;
  char pad[56];
  Slots *m_60;
};

extern "C" Obj *FUN_00190458(Obj *self, short flag)
{
  if (self != 0) {
    self->vtable = &DAT_00309178;
    DAT_003415d4 = 0;
    if (self->m_60 != 0) {
      if (self->m_60 != 0) {
        delete self->m_60;
        self->m_60 = 0;
      }
    }
    FUN_0001f0f0(self, 0);
    if (flag > 0) {
      FUN_00000a00(self);
    }
  }
  return self;
}
