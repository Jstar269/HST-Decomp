/* HSTMATCH module=hstp_trans_main.prx start=0x002ad894 end=0x002ae294 symbol=FUN_002ad894 lang=c++ helpers=FUN_002ad710@0x002ad710-0x002ad73c rodata=0x002dba18 */
/* Hand-written reconstruction: clone of the source of file_receive_update_0029e550.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. The empty log stub FUN_002ad710 is a same-file static placed by helpers= (its preserved v0 carries the FUN_002a8370 result past the two log calls). Matching idioms: one-case switch with default for the state==0 test; the save-size compare written need-first (NeedKB(recv) <= remain). */
enum RecvState {
  RECV_STATE_CONNECT,      /* waiting for the peer handshake */
  RECV_STATE_AWAIT_ACK,
  RECV_STATE_CHECK_SPACE,  /* compares the needed size with the free space */
  RECV_STATE_AWAIT_START,
  RECV_STATE_TRANSFER,     /* worker thread running */
  RECV_STATE_SETTLE,       /* counts down `wait` */
  RECV_STATE_FINALIZE,
  RECV_STATE_ABORTING,
  RECV_STATE_END           /* terminal: waits for the dialog to close */
};

enum {
  BLOCK_SIZE = 0x8000,            /* save data is counted in whole blocks */
  RESERVE_KB = 0x60,              /* extra space needed on top of the blocks */
  RECV_SIZE_UNKNOWN = 0x7fffffff, /* total size not announced yet */
  RECV_ERR_NO_SPACE = (int)0x8001001c
};

struct RefObj {
  virtual void Release(int);
  virtual void Slot0();
};

struct Listener {
  virtual void V0();
  virtual void V1();
  virtual void V2();
  virtual void V3();
  virtual void V4();
  virtual void V5();
  virtual void V6();
  virtual void V7();
  virtual void V8();
  virtual void V9();
  virtual void V10();
  virtual void Notify();
};

struct MacAddr {
  char addr[6];
  MacAddr() {}
};

struct Recv {
  int state;               /* 0x00 */
  RefObj *obj;             /* 0x04 */
  int unk08, unk0c;
  int got;                 /* 0x10 */
  int total;               /* 0x14 */
  int extra;               /* 0x18 */
  int progress;            /* 0x1c */
  int error;               /* 0x20 */
  unsigned char failed;    /* 0x24 */
  unsigned char finished;  /* 0x25 */
  int thread;              /* 0x28 */
  char message[0x180];     /* 0x2c */
  int wait;                /* 0x1ac */
  Listener *listener;      /* 0x1b0 */
  char pad1b4[0x201c4 - 0x1b4];
  char text[0x400];        /* 0x201c4 */
};

struct ReceiveScreen {
  char pad[0x3c];
  Recv *recv;
};

extern "C" {
extern char DAT_003eaad8[];
extern char DAT_003f4670[];
extern char DAT_002dc05c[];
extern char DAT_002dc044[];
extern char DAT_002dc030[];
extern void *DAT_003ea8ec;
extern void *DAT_003eaaac;
extern RefObj *DAT_003e1e04;

static void FUN_002ad710(...)
{
}
int FUN_002af6b4(void *);
int FUN_00120618(void *);
int FUN_00120668(void *);
int FUN_001205c8(void *);
int FUN_00120568(void *);
int FUN_001204dc(void *);
int FUN_001205f0(void *);
void FUN_001bfc3c(void *, int);
int FUN_002aaf9c(RefObj *);
MacAddr *FUN_002aaf94(RefObj *);
void FUN_000bde80(RefObj *);
void FUN_002c7b28(void *, MacAddr *);
int FUN_000c3a2c(RefObj *, unsigned char, int, const MacAddr *, const MacAddr *);
void FUN_002aec70(Recv *, int);
void FUN_002ae2b4(Recv *, int);
unsigned int FUN_000c3d50(RefObj *);
int FUN_000c3d7c(RefObj *);
void FUN_000c3d90(RefObj *);
void FUN_002aeb5c(Recv *);
void FUN_002aea50(Recv *, int);
void *FUN_001991d4(int);
char *FUN_0019b98c(void *, int, int);
char *FUN_001a4bcc(void *, const char *, int, char *);
void FUN_0011fe78(void *, int, const char *, int, int);
int FUN_002a5f3c(void);
int FUN_002a8370(int, long long *);
void FUN_002a5b70(void *);
int FUN_002ae294(void);
int FUN_002c76a8(int);
void FUN_002a5ba8(void *);
void FUN_00047cd0(void);
void FUN_002a5b98(void *, float, float);
void FUN_002a5b90(void *, float);
void FUN_000be5d8(RefObj *);
int FUN_002aabfc(void);
int FUN_002aabf0(void);
void FUN_000149a8(char *, const char *);
}

static inline char *Text(Recv *recv, int string_id)
{
  return FUN_001a4bcc(DAT_003eaaac, FUN_0019b98c(FUN_001991d4(0x11), 14, string_id), 0x400, recv->text);
}

static inline void Fail(Recv *recv, int code)
{
  recv->failed = 1;
  FUN_002ae2b4(recv, code);
  FUN_002aec70(recv, RECV_STATE_END);
}

static inline int NeedKB(Recv *recv)
{
  return ((recv->total + (BLOCK_SIZE - 1)) / BLOCK_SIZE + (recv->extra + (BLOCK_SIZE - 1)) / BLOCK_SIZE) * BLOCK_SIZE / 1024 + RESERVE_KB;
}

extern "C" void FUN_002ad894(ReceiveScreen *self)
{
  Recv *recv;
  RefObj *obj;
  int result;
  int error_code;
  MacAddr mine;
  long long remain;  /* free space in KB */

  recv = self->recv;
  if (FUN_002af6b4(DAT_003ea8ec) == 0) {
    if (FUN_00120618(DAT_003ea8ec)) {
      FUN_001bfc3c(DAT_003eaad8, 3);
    } else if (FUN_00120668(DAT_003ea8ec)) {
      FUN_001bfc3c(DAT_003eaad8, 4);
    } else if (FUN_001205c8(DAT_003ea8ec)) {
      FUN_001bfc3c(DAT_003eaad8, 1);
    }
  }
  switch (recv->state) {
  case RECV_STATE_CONNECT:
    switch (FUN_002aaf9c(DAT_003e1e04)) {
    case 0:
      FUN_000bde80(DAT_003e1e04);
      break;
    case 2: {
      FUN_002c7b28(DAT_003f4670, &mine);
      MacAddr peer = *FUN_002aaf94(DAT_003e1e04);
      FUN_000c3a2c(recv->obj, 0, 1, &mine, &peer);
      FUN_002aec70(recv, RECV_STATE_AWAIT_ACK);
      break;
    }
    case 3:
      FUN_002ad710(DAT_002dc05c);
      break;
    }
    break;
  default:
    obj = recv->obj;
    if (obj != 0) {
      if (recv->failed != 0) {
        if (obj != 0) {
          if (obj != 0) {
            obj->Release(1);
          }
          recv->obj = 0;
        }
      } else {
        if (recv->finished == 0 && FUN_000c3d50(obj) != 0 && FUN_000c3d50(recv->obj) != 0x4000) {
          recv->failed = 1;
          FUN_001bfc3c(DAT_003eaad8, 5);
          FUN_0011fe78(DAT_003ea8ec, 0, Text(recv, 22), 0, 0);
          obj = recv->obj;
          if (obj != 0) {
            if (obj != 0) {
              obj->Release(1);
            }
            recv->obj = 0;
          }
        } else if (recv->got >= recv->total && FUN_000c3d7c(recv->obj) <= 0) {
          recv->finished = 1;
        } else {
          FUN_002aeb5c(recv);
        }
      }
    }
    break;
  }
  switch (recv->state) {
  case RECV_STATE_CONNECT:
  case RECV_STATE_AWAIT_ACK:
  case RECV_STATE_CHECK_SPACE:
    FUN_002aea50(recv, 0x20000);
    break;
  }
  switch (recv->state) {
  case RECV_STATE_CONNECT:
    if (FUN_00120568(DAT_003ea8ec)) {
      Fail(recv, 0x80000006);
    }
    break;
  case RECV_STATE_AWAIT_ACK:
    if (FUN_001204dc(DAT_003ea8ec)) {
      if (FUN_001205f0(DAT_003ea8ec) == 0) {
        result = FUN_002a5f3c();
        if (result < 0) {
          Fail(recv, result);
        } else {
          FUN_0011fe78(DAT_003ea8ec, 0, Text(recv, 18), 0, 0);
          FUN_002aec70(recv, RECV_STATE_CHECK_SPACE);
        }
      } else {
        Fail(recv, 0x80000006);
      }
    }
    break;
  case RECV_STATE_CHECK_SPACE:
    if (recv->total == RECV_SIZE_UNKNOWN) {
      if (FUN_00120568(DAT_003ea8ec)) {
        Fail(recv, 0x80000006);
      }
    } else {
      result = FUN_002a8370(0, &remain);
      FUN_002ad710(DAT_002dc044, remain);
      FUN_002ad710(DAT_002dc030, NeedKB(recv));
      if (result == 0) {
        if (NeedKB(recv) <= remain) {
          FUN_0011fe78(DAT_003ea8ec, 1, Text(recv, 17), 0, 0);
          FUN_002a5b70(DAT_003ea8ec);
          FUN_002aec70(recv, RECV_STATE_AWAIT_START);
        } else {
          Fail(recv, RECV_ERR_NO_SPACE);
        }
      } else if (result == (int)0x80020321 || result == (int)0x80010013) {
        Fail(recv, 0x80010013);
      } else {
        Fail(recv, result);
      }
    }
    break;
  case RECV_STATE_AWAIT_START:
    if (FUN_001204dc(DAT_003ea8ec)) {
      if (FUN_001205f0(DAT_003ea8ec) == 0) {
        FUN_0011fe78(DAT_003ea8ec, 4, Text(recv, 6), 0, 0);
        FUN_002aec70(recv, RECV_STATE_TRANSFER);
      } else {
        Fail(recv, 0x80000006);
      }
    }
    break;
  case RECV_STATE_TRANSFER:
    if (FUN_002ae294() != 0 || FUN_00120568(DAT_003ea8ec) != 0) {
      if (FUN_002ae294() != 0) {
        FUN_001bfc3c(DAT_003eaad8, 4);
      }
      recv->failed = 1;
      FUN_002aec70(recv, RECV_STATE_ABORTING);
    } else if (FUN_002c76a8(recv->thread) == 0) {
      if (recv->error == 0) {
        FUN_002aec70(recv, RECV_STATE_SETTLE);
      } else {
        recv->failed = 1;
        FUN_002ae2b4(recv, recv->error);
        FUN_002aec70(recv, RECV_STATE_END);
      }
    }
    break;
  case RECV_STATE_SETTLE:
    if (recv->wait-- <= 0) {
      FUN_002aec70(recv, RECV_STATE_FINALIZE);
    }
    break;
  case RECV_STATE_FINALIZE:
    if (FUN_002c76a8(recv->thread) == 0) {
      FUN_002ae2b4(recv, recv->error);
      FUN_002aec70(recv, RECV_STATE_END);
    }
    break;
  case RECV_STATE_ABORTING:
    if (FUN_002c76a8(recv->thread) == 0) {
      recv->failed = 1;
      FUN_002ae2b4(recv, recv->error);
      FUN_002aec70(recv, RECV_STATE_END);
    }
    break;
  case RECV_STATE_END:
    if (FUN_001204dc(DAT_003ea8ec)) {
      FUN_002a5ba8(DAT_003ea8ec);
      FUN_00047cd0();
    }
    break;
  }
  FUN_002a5b98(DAT_003ea8ec, 0.0f, (float)recv->total);
  FUN_002a5b90(DAT_003ea8ec, (float)recv->progress);
  FUN_000be5d8(DAT_003e1e04);
  switch (recv->state) {
  case RECV_STATE_SETTLE:
  case RECV_STATE_FINALIZE:
  case RECV_STATE_ABORTING:
  case RECV_STATE_END:
    break;
  default:
    if (FUN_002aabfc() == 0) {
      recv->failed = 1;
      error_code = FUN_002aabf0();
      switch (error_code) {
      case (int)0x80410b03:
      case (int)0x80410b0f:
        FUN_002ae2b4(recv, error_code);
        break;
      default:
        FUN_000149a8(recv->message, Text(recv, 22));
        break;
      }
      FUN_000c3d90(recv->obj);
      FUN_002aec70(recv, RECV_STATE_END);
    }
    break;
  }
  if (recv->listener != 0) {
    recv->listener->Notify();
  }
}
