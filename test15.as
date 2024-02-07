        lw     1     0     add1
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
add1    .fill  2
four    .fill  4
five    .fill  5
one     .fill  1
ten     .fill  10
