/* HSTMATCH module=hstp_trans_main.prx start=0x000661a8 end=0x00066258 symbol=FUN_000661a8 lang=c++ */
/* Hand-written reconstruction: clone of the source of csv_cell_value_00066038.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* csv_reader.cpp (file name and assert texts from the strings): debug build with both assertions kept; returns the value
   of the cell at (row, column) through its second virtual. The row/column containers are a vector-like template whose
   inline operator[] computes the element address (base-first addu, as retail); the same subscript written directly in
   the caller is offset-first. The getters are non-const so the row address is CSE-d between the column test and the cell read. */
extern "C" void FUN_0006600c(...);
extern const char DAT_00377108[];   /* "assertion \"%s\" failed: file \"%s\", line %d\n" */
extern const char DAT_00377134[];   /* "row < GetNumRow()" */
extern const char DAT_00377148[];   /* "csv_reader.cpp" */
extern const char DAT_002cc4cc[];   /* "assertion \"%s\" failed: file \"%s\", line %d\n" */
extern const char DAT_002cc4b0[];   /* "column < GetNumColumn(row)" */
extern const char DAT_002cc4a0[];   /* "csv_reader.cpp" */
#define HST_ASSERT(cond, fmt, text, file, line) do { if (!(cond)) { FUN_0006600c(fmt, text, file, line); for (;;) {} } } while (0)

struct Cell {
    virtual void v0();
    virtual int v1();
    virtual void v2();
    virtual int v3();
};

template <class T> struct Vec {
    T *data;
    int num;
    int cap;
    int size() { return num; }
    T &operator[](int i) { return data[i]; }
};

struct CsvReader {
    int pad0;
    Vec<Vec<Cell *> > rows;
    int GetNumRow() { return rows.size(); }
    int GetNumColumn(int row) { return rows[row].size(); }
    Cell *GetCell(int row, int column)
    {
        HST_ASSERT(row < GetNumRow(), DAT_00377108, DAT_00377134, DAT_00377148, 204);
        HST_ASSERT(column < GetNumColumn(row), DAT_002cc4cc, DAT_002cc4b0, DAT_002cc4a0, 210);
        return rows[row][column];
    }
};

extern "C" int FUN_000661a8(CsvReader *self, int row, int column)
{
    return self->GetCell(row, column)->v3();
}
