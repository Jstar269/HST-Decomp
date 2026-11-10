/* HSTMATCH module=hstp_trans_main.prx start=0x00104e48 end=0x00104f10 symbol=FUN_00104e48 lang=c99 */
/* Clone of src/eboot/ring_write_000ff5b0.c (hstclone.py tag c145607): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction (lane lnm1 near miss wip/ring_write_00104e48.c, finished by lane pn4: the length parameter is a signed `int` tested and returned as is, and the clamped count is a separate unsigned local, which keeps the incoming a2 for the early test and copies it to s0 after the ring loads like retail); exact match verified; names and types await human review. */
/* Ring-buffer write: copies up to n bytes (clamped to the free space) at the write index, wrapping once. */
extern void FUN_002c75b0(char *dst, char *src, unsigned n);
extern void FUN_00011090(char *dst, char *src, unsigned n);

typedef struct Ring {
    int pad0;
    char *data;
    unsigned cap;
    unsigned rd;
    unsigned wr;
} Ring;

int FUN_00104e48(Ring *r, char *src, int n)
{
    unsigned cnt;
    unsigned avail;
    if (n <= 0)
        return n;
    cnt = n;
    avail = r->cap - (r->wr - r->rd);
    if (avail < cnt) cnt = avail;
    {
        unsigned off = r->wr % r->cap;
        unsigned chunk = r->cap - off;
        if (cnt < chunk)
            chunk = cnt;
        FUN_002c75b0(r->data + off, src, chunk);
        if (chunk != cnt)
            FUN_00011090(r->data, src + chunk, cnt - chunk);
    }
    r->wr = r->wr + cnt;
    return cnt;
}
