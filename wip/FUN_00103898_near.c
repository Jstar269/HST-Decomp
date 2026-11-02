/* HSTMATCH module=EBOOT.BIN start=0x00103898 end=0x001039d8 symbol=FUN_00103898 lang=c99 */
struct Keyed { char pad[0x20]; float key; };
struct Slot  { char pad[0x20]; struct Keyed *keyed; };
struct Group { char pad0[0x10]; struct Slot **slots; int count; char pad1[8]; float key; };
struct Self {
    char pad0[4];
    struct Group **begin;
    int n;
    char pad1[4];
    struct Keyed **arr;
    int count;
    int cap;
};
extern void FUN_001073ac(struct Keyed ***, void *, int);
extern void FUN_00106edc(struct Keyed **, void *, int);

void FUN_00103898(struct Self *self)
{
    struct Group **it = self->begin;
    if (it != self->begin + self->n) {
        do {
            struct Group *g = *it;
            float key = g->key;
            int i = 0;
            if (0 < g->count) {
                do {
                    struct Keyed *k = g->slots[i]->keyed;
                    if (k != 0) {
                        if (key != k->key) {
                            if (self->count < self->cap) {
                                int m = self->count + 1;
                                self->count = m;
                                self->arr[m - 1] = k;
                            } else {
                                FUN_001073ac(&self->arr, &k, 0);
                            }
                        }
                    }
                    i++;
                } while (i < g->count);
            }
            it++;
        } while (it != self->begin + self->n);
    }
    FUN_00106edc(self->arr, self->arr + self->count, 0);
}
// near miss: 73/80 words, 79 differ, first +0x0 (frame -64 vs retail -80). Reconstructed from the
// address-labelled disassembly: outer walk over Group** with a float-key dedup inner loop into self->arr.
// Residual: an extra callee-saved register (self cached in s4) and the outer bound folded to n==0
// (`beqz v0`) where retail re-reads begin/n and uses xor/sltiu/xori each pass; Group layout fixed to
// key@0x20. Same register/frame class as the earlier dsn3 park (64-byte frame, one register short).
// near miss from lane dsn6 (unreviewed)
