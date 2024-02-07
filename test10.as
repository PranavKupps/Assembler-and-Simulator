        sw      0       1       6
        lw      0       1       23
        jalr    0       1       30
        add     0       1       2
        jalr    0       7       30
start   noop    1       2       3
        nor     0       1       2
        beq     0       0       start
        noop
done    halt
five    .fill   5
neg1    .fill   -1
stAddr  .fill   start