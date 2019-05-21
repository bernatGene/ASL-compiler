function main
  vars
    a 2
    b 1
    c 1
  endvars

   %1 = 3
   b = %1
   %2 = 4
   c = %2
   %3 = 1
   %4 = 2
   a[%3] = %4
   %5 = 0
   %6 = 1
   a[%5] = %6
   %7 = 1
   %8 = a[%7]
   b = %8
   %9 = 1
   %10 = a[%9]
   %11 = 2
   %12 = %10 + %11
   %13 = 0
   %14 = a[%13]
   %15 = %12 + %14
   b = %15
   %16 = 0
   %17 = a[%16]
   writei %17
   writeln
   %20 = 1
   %21 = a[%20]
   writei %21
   writeln
   writei b
   writeln
   writei c
   writeln
   return
endfunction


