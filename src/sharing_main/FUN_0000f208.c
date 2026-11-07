/* HSTMATCH module=hstp_trans_main.prx start=0x0000f208 end=0x0000f2ec symbol=FUN_0000f208 lang=c99 cc=gcc171-O2 */
/* Hand-written reconstruction: clone of the source of FUN_0000f208.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Library source: newlib libc stdio findfp.c function __sfp, older form (BSD-derived; Regents of the University of California, BSD licence), adapted by hand; exact match verified; names and types await human review. */
struct __sbuf { unsigned char *_base; int _size; };
typedef struct __sFILE {
  unsigned char *_p; int _r; int _w; short _flags; short _file; struct __sbuf _bf; int _lbfsize;
  void *_cookie; int (*_read)(); int (*_write)(); long (*_seek)(); int (*_close)();
  struct __sbuf _ub; unsigned char *_up; int _ur; unsigned char _ubuf[3]; unsigned char _nbuf[1];
  struct __sbuf _lb; int _blksize; int _offset; struct _reent *_data; int _extra;
} FILE;
struct _glue { struct _glue *_next; int _niobs; FILE *_iobs; };
struct _reent { int _errno; char pad[52]; int __sdidinit; char pad2[604 - 60]; struct _glue __sglue; };
extern struct _reent *DAT_00374b48;   /* _global_impure_ptr */
#define _GLOBAL_REENT DAT_00374b48
extern void FUN_0000f330(struct _reent *);              /* __sinit */
extern struct _glue *FUN_0000f194(struct _reent *, int); /* __sfmoreglue */
#define NDYNAMIC 4

FILE *FUN_0000f208(struct _reent *d)
{
  FILE *fp;
  int n;
  struct _glue *g;

  if (!_GLOBAL_REENT->__sdidinit)
    FUN_0000f330 (_GLOBAL_REENT);
  for (g = &_GLOBAL_REENT->__sglue;; g = g->_next)
    {
      for (fp = g->_iobs, n = g->_niobs; --n >= 0; fp++)
	if (fp->_flags == 0)
	  goto found;
      if (g->_next == 0 &&
	  (g->_next = FUN_0000f194 (d, NDYNAMIC)) == 0)
	break;
    }
  d->_errno = 12;
  return 0;

found:
  fp->_flags = 1;		/* reserve this slot; caller sets real flags */
  fp->_p = 0;			/* no current pointer */
  fp->_w = 0;			/* nothing to read or write */
  fp->_r = 0;
  fp->_bf._base = 0;		/* no buffer */
  fp->_bf._size = 0;
  fp->_lbfsize = 0;		/* not line buffered */
  fp->_file = -1;		/* no file */
  /* fp->_cookie = <any>; */	/* caller sets cookie, _read/_write etc */
  fp->_ub._base = 0;		/* no ungetc buffer */
  fp->_ub._size = 0;
  fp->_lb._base = 0;		/* no line buffer */
  fp->_lb._size = 0;
  return fp;
}
