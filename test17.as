        lw     0     1    answer      int answer = 0;
        lw     0     2    mplier      int mplier = 3;
        lw     0     3    mcand       int mcand = 7;
        lw     0     4    bitloc     int bitloc = 1;
        lw     0     5    i
        lw     0     6    bits
        add    1     0     3
        sw     1     0     0
        lw     1     4     four
        lw     1     5     five
        nor    4     5     6
start   beq    4     5     next
        lw     1     2     one
        add    4     2     4
        beq    0     0     start
next    add    6     6     6
        lw     7     1     ten
        add    4     4     4
        add    4     4     4
        jalr   4     7
        add    0     0     0
        add    6     6     6
        add    6     6     6
        add    6     6     6
        add    6     6     6
end     halt
answer  .fill 0
mplier  .fill 3
mcand   .fill 7
bitloc .fill 1
i       .fill 0
bits    .fill 15
one     .fill 1
zero    .fill 0
