/* HSTMATCH module=hstp_trans_main.prx start=0x0001c984 end=0x0001ca74 symbol=FUN_0001c984 lang=c99 */
/* Clone of src/eboot/huffman_decode_0001cd38.c (hstclone.py tag c145607): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly (lane rt3 near miss wip/huffman_decode_0001c984.c, finished by lane pn4: the source pointer is the second parameter, overwritten by the table builder's result); exact match verified; matching idioms: `(ushort)buf & 0x3ff` and `*dst++ = (ushort)buf` keep retail's andi 0xffff; names and types await human review. */
/* Decodes n bytes of a 10-bit-table Huffman stream (literal or 8-bit escape) into dst. */
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef struct Entry { uchar len; uchar val; } Entry;
extern ushort *FUN_0001c8ac(ushort *a, Entry *tab);

void FUN_0001c984(uchar *dst, ushort *src, int n)
{
    Entry tab[1024];
    uchar *end;
    int nbits;
    uint buf;
    src = FUN_0001c8ac(src, tab);
    end = dst + n;
    buf = 0;
    nbits = 0;
    if (dst < end) {
        do {
            Entry *e;
            if (nbits < 16) {
                buf |= (uint)*src++ << nbits;
                nbits += 16;
            }
            e = &tab[(ushort)buf & 0x3ff];
            if (e->len < 11) {
                *dst++ = e->val;
                buf >>= e->len;
                nbits -= e->len;
            } else {
                buf >>= 10;
                nbits -= 10;
                if (nbits < 16) {
                    buf |= (uint)*src++ << nbits;
                    nbits += 16;
                }
                *dst++ = (ushort)buf;
                buf >>= 8;
                nbits -= 8;
            }
        } while (dst < end);
    }
}
