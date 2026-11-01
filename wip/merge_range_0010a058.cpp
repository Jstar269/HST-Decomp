/* HSTMATCH module=EBOOT.BIN start=0x0010a058 end=0x0010a184 symbol=FUN_0010a058 lang=c++ */

extern "C" {
void *memmove(void *dst, const void *src, unsigned int n);
}

struct Item {
    char pad[32];
    float key;
};

struct Iter {
    Item **ptr;
    int pad;
};

static inline int operator==(const Iter &a, const Iter &b) {
    return a.ptr == b.ptr;
}

static inline int operator!=(const Iter &a, const Iter &b) {
    return (a.ptr == b.ptr) ^ 1;
}

extern "C" Item **FUN_0010a058(Item **&out, Iter first1, Iter last1, Item **first2, Item **last2, Item **result)
{
    while (first1.ptr != last1.ptr && first2 != last2) {
        if (!((*first2)->key > (*first1.ptr)->key)) {
            *result++ = *first1.ptr++;
        } else {
            *result++ = *first2++;
        }
    }

    if (first1.ptr == last1.ptr) {
        int diff = (char *)last2 - (char *)first2;
        if (diff < 0) diff += 3;
        int count = diff >> 2;
        memmove(result, first2, count * 4);
        result += count;
    } else {
        while (first1.ptr != last1.ptr) {
            *result++ = *first1.ptr++;
        }
    }

    out = result;
    return result;
}
// Near miss: 64 words differ out of 75 words.
// std::merge implementation merging two sorted ranges of Item* based on float key at +32 using c.ngt.s comparison.
// Iterators first1 and last1 are 8-byte structs copied with lwc1/swc1 into the frame.
// Tail uses memmove when first1 == last1 and a pointer copy loop when first2 == last2.
// near miss from lane dsn5 (unreviewed)
