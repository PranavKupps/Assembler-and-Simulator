        lw     0     1    answer      int answer = 0; 
        lw     0     2    mplier      int mplier = 3;
        lw     0     3    mcand       int mcand = 7;
        lw     0     4    bitloc     int bitloc = 1;
        lw     0     5    i
        lw     0     6    bits
loop    beq    5     6    else        if(i == bits)
        nor    4     2    0           int bit = ~(bitloc | mplier);
        beq    0     4    if          if(bit == bitloc)
        beq    0     0    rest        else from above
if      add    1     3    1           answer += mcand;
rest    add    4     4    4           bitloc += bitloc;
        add    3     3    3           mcand += mcand;
        lw     0     6    one         load 1 into bits
        add    5     6    5           supposed to do i+=1
        lw     0     6    bits        reload bits back into register 6
        beq    0     0    loop        loop back to if(i == bits)
else    noop                        this is to end the loop
done    halt                        exit program
answer  .fill 0
mplier  .fill 1429
mcand   .fill 6203
bitloc .fill 1
i       .fill 1
bits    .fill 16
one     .fill 1
zero    .fill 0
