      lw      0       1       32767   negative registry
      lw      0       1       -32768  negative registry
      beq     0       1       32767   negative registry
      nor     0       1       2       test nor
      jalr    0       4               test jalr
v1    .fill   -3
v2    .fill   -32769                   test .fill
