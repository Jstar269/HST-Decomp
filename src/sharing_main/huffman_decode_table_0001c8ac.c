/* HSTMATCH module=hstp_trans_main.prx start=0x0001c8ac end=0x0001c984 symbol=FUN_0001c8ac lang=c99 */
/* Hand-written reconstruction: clone of the source of huffman_decode_table_0001cc60.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Found with hstnearperm.py (tag d1) from the lane-ag5 near miss; minimised to two edits: (int) on the stride added to the table pointer and the loop bound spelled rev <= 1023. Exact match verified; names and types await human review. */

struct Entry {
    unsigned char len;
    unsigned char val;
};

unsigned char * FUN_0001c8ac(unsigned char *p, struct Entry *table)
{
    int code = 0;
    int len = 1;
    int count_minus_1;
    unsigned int rev;
    unsigned int c;
    int k;
    int one = 1;
    int n = *p++;

    if (n > 0) {
        do {
            int count = *p++;
            count_minus_1 = count - 1;
            if (count > 0) {
                unsigned int step = one << len;
                do {
                    c = code;
                    rev = 0;
                    k = len - 1;
                    if (len > 0) {
                        do {
                            rev = (rev << 1) | (c & 1);
                            c >>= 1;
                        } while (k-- > 0);
                    }
                    unsigned char val = *p++;
                    if (rev < 1024) {
                        struct Entry *entry = &table[rev];
                        do {
                            entry->len = len;
                            rev += step;
                            entry->val = val;
                            entry += (int)step;
                        } while (rev <= 1023);
                    }
                    if (len < 11) {
                        code++;
                    }
                } while (count_minus_1-- > 0);
            }
            len++;
            code <<= 1;
        } while (len <= n);
    }

    if ((unsigned int)p & 1) {
        p++;
    }
    return p;
}
