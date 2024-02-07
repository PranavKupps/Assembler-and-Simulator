        lw      0       1       answer  int answer = 0;
        lw      0       2       mcand   int mcand = input
        lw      0       3       mplier  int mplier = next input
        lw      0       4       bitloc
        lw      0       5       bits
loop    beq     4       5       else    while(bit_loc !=32768)
        nor     2       2       6       ~mplier
        nor     4       4       7       ~bit_loc
        nor     6       7       0       int bit = ~(~bit_loc | ~mplier);
if      beq     0       4       next
        beq     0       0       zero
next    add     1       3       1
zero    add     4       4       4
        add     3       3       3
        beq     0       0       loop
else    noop
done    halt                            end of program
answer  .fill   0
mcand   .fill   6203
mplier  .fill   1429
bitloc  .fill   1
bits    .fill   32768
