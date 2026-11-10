/* HSTMATCH module=EBOOT.BIN start=0x0002fe18 end=0x0002fe80 symbol=FUN_0002fe18 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
int FUN_0002fe18(void *obj, unsigned int param_2)
{
	int *sub = (int *)*(int *)((char *)obj + 0x10);
	unsigned count = *(unsigned short *)((char *)sub + 0x2e);
	if (count == 0)
		return 0;
	unsigned short *arr = (unsigned short *)*(int *)((char *)sub + 0x1c);
	unsigned index = 0;
	if (param_2 == arr[0])
		return ((int *)*(int *)((char *)sub + 0x20))[index];
	do {
		index++;
		if (index >= count)
			return 0;
	} while (param_2 != arr[1]);
	return ((int *)*(int *)((char *)sub + 0x20))[index];
}
/* NEAR 22 words. Retail: lw a2,16(a0); lhu a3,46(a2); beqz a3; ... arr kept in v1; index in a0; result reloaded from sub; sll t0.
   Ours gets sub=a2, arr=v1, result reloaded, beqzl branch right, but MWCC allocates count=a0/index=a3 (swapped vs retail a3/a0),
   branch likelihood differs (bnezl vs beqz), and shift dest sll v1 vs sll t0. Tried: raw pointer fields, no result local,
   index declared first, arr-before-count, if(count!=0){}else{} structure. Could not steer count->a3 / index->a0 / beqz / sll t0
   simultaneously. */
// near miss from lane dsn15 (unreviewed)
