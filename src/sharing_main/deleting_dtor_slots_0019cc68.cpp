/* HSTMATCH module=hstp_trans_main.prx start=0x0019cc68 end=0x0019cdc8 symbol=FUN_0019cc68 lang=c++ */
/* Clone of src/eboot/deleting_dtor_slots_00190458.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma inline_depth(8)

extern void *DAT_003b2444;
extern int DAT_003ea914;

extern "C" {
void FUN_000009e4(void *);
void FUN_000104e0(void *);
void FUN_000463a0(void *, int);
void FUN_00004138(void *, int);
void FUN_0019d32c(void *, int);
void FUN_00000a00(void *);
void FUN_0001ed28(void *, int);
}

struct VC {
  virtual ~VC();
};

struct SubObj {
  char pad[12];
  void *sub_field;
  char pad2[8];
  ~SubObj() {
    FUN_000463a0(&sub_field, -1);
    FUN_00004138(this, 0);
  }
};

struct Layer2 {
  ~Layer2() {
    FUN_0019d32c(this, 0);
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

extern "C" Obj *FUN_0019cc68(Obj *self, short flag)
{
  if (self != 0) {
    self->vtable = &DAT_003b2444;
    DAT_003ea914 = 0;
    if (self->m_60 != 0) {
      if (self->m_60 != 0) {
        delete self->m_60;
        self->m_60 = 0;
      }
    }
    FUN_0001ed28(self, 0);
    if (flag > 0) {
      FUN_00000a00(self);
    }
  }
  return self;
}
