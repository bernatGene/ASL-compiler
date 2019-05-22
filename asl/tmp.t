function f
  params
    v
  endparams

  vars
    c 10
    i 1
  endvars

     %1 = v
     %2 = 0
     %3 = 1
     %6 = 10
  label doACopy1 :
     %5 = %2 < %6
     ifFalse %5 goto enACopy1
     %4 = %1[%2]
     c[%2] = %4
     %2 = %2 + %3
     goto doACopy1
  label enACopy1 :
     %7 = 'e'
     writec %7
     %7 = 'n'
     writec %7
     %7 = ' '
     writec %7
     %7 = 'f'
     writec %7
     %7 = '.'
     writec %7
     %7 = ' '
     writec %7
     %7 = 'c'
     writec %7
     %7 = ':'
     writec %7
     %7 = ' '
     writec %7
     %8 = 0
     i = %8
  label doWhile2 :
     %9 = 10
     %10 = i < %9
     ifFalse %10 goto endWhile2
     %11 = c[i]
     writei %11
     %13 = ' '
     writec %13
     %15 = 1
     %16 = i + %15
     i = %16
     goto doWhile2
  label endWhile2 :
     %17 = '\n'
     writec %17
     return
endfunction

function g
  params
    v
  endparams

  vars
    d 10
    i 1
  endvars

     %1 = 0
     i = %1
  label doWhile1 :
     %2 = 10
     %3 = i < %2
     ifFalse %3 goto endWhile1
     %4 = 1
     %5 = - %4
     d[i] = %5
     %6 = 1
     %7 = i + %6
     i = %7
     goto doWhile1
  label endWhile1 :
     %8 = v
     %9 = 0
     %10 = 1
     %13 = 10
  label doACopy2 :
     %12 = %9 < %13
     ifFalse %12 goto enACopy2
     %11 = d[%9]
     %8[%9] = %11
     %9 = %9 + %10
     goto doACopy2
  label enACopy2 :
     return
endfunction

function main
  vars
    a 10
    b 10
    i 1
    j 1
  endvars

     %1 = 0
     i = %1
  label doWhile1 :
     %2 = 10
     %3 = i < %2
     ifFalse %3 goto endWhile1
     a[i] = i
     %4 = 0
     b[i] = %4
     %5 = 1
     %6 = i + %5
     i = %6
     goto doWhile1
  label endWhile1 :
     %7 = 0
     %8 = 1
     %11 = 10
  label doACopy2 :
     %10 = %7 < %11
     ifFalse %10 goto enACopy2
     %9 = a[%7]
     b[%7] = %9
     %7 = %7 + %8
     goto doACopy2
  label enACopy2 :
     %12 = 'd'
     writec %12
     %12 = 'e'
     writec %12
     %12 = 's'
     writec %12
     %12 = 'p'
     writec %12
     %12 = 'r'
     writec %12
     %12 = 'e'
     writec %12
     %12 = 's'
     writec %12
     %12 = ' '
     writec %12
     %12 = 'd'
     writec %12
     %12 = 'e'
     writec %12
     %12 = ' '
     writec %12
     %12 = 'b'
     writec %12
     %12 = '='
     writec %12
     %12 = 'a'
     writec %12
     %12 = '.'
     writec %12
     %12 = ' '
     writec %12
     %12 = 'b'
     writec %12
     %12 = ':'
     writec %12
     %12 = ' '
     writec %12
     %13 = 0
     i = %13
  label doWhile3 :
     %14 = 10
     %15 = i < %14
     ifFalse %15 goto endWhile3
     %16 = b[i]
     writei %16
     %18 = ' '
     writec %18
     %20 = 1
     %21 = i + %20
     i = %21
     goto doWhile3
  label endWhile3 :
     %22 = '\n'
     writec %22
     %24 = 'd'
     writec %24
     %24 = 'e'
     writec %24
     %24 = 's'
     writec %24
     %24 = 'p'
     writec %24
     %24 = 'r'
     writec %24
     %24 = 'e'
     writec %24
     %24 = 's'
     writec %24
     %24 = ' '
     writec %24
     %24 = 'd'
     writec %24
     %24 = 'e'
     writec %24
     %24 = ' '
     writec %24
     %24 = 'b'
     writec %24
     %24 = '='
     writec %24
     %24 = 'a'
     writec %24
     %24 = '.'
     writec %24
     %24 = ' '
     writec %24
     %24 = 'a'
     writec %24
     %24 = ':'
     writec %24
     %24 = ' '
     writec %24
     %25 = 0
     i = %25
  label doWhile4 :
     %26 = 10
     %27 = i < %26
     ifFalse %27 goto endWhile4
     %28 = a[i]
     writei %28
     %30 = ' '
     writec %30
     %32 = 1
     %33 = i + %32
     i = %33
     goto doWhile4
  label endWhile4 :
     %34 = '\n'
     writec %34
     %36 = &a
     pushparam %36
     call f
     popparam 
     %37 = 'd'
     writec %37
     %37 = 'e'
     writec %37
     %37 = 's'
     writec %37
     %37 = 'p'
     writec %37
     %37 = 'r'
     writec %37
     %37 = 'e'
     writec %37
     %37 = 's'
     writec %37
     %37 = ' '
     writec %37
     %37 = 'd'
     writec %37
     %37 = 'e'
     writec %37
     %37 = ' '
     writec %37
     %37 = 'f'
     writec %37
     %37 = '('
     writec %37
     %37 = 'a'
     writec %37
     %37 = ')'
     writec %37
     %37 = '.'
     writec %37
     %37 = ' '
     writec %37
     %37 = 'a'
     writec %37
     %37 = ':'
     writec %37
     %37 = ' '
     writec %37
     %38 = 0
     i = %38
  label doWhile5 :
     %39 = 10
     %40 = i < %39
     ifFalse %40 goto endWhile5
     %41 = a[i]
     writei %41
     %43 = ' '
     writec %43
     %45 = 1
     %46 = i + %45
     i = %46
     goto doWhile5
  label endWhile5 :
     %47 = '\n'
     writec %47
     %49 = &a
     pushparam %49
     call g
     popparam 
     %50 = 'd'
     writec %50
     %50 = 'e'
     writec %50
     %50 = 's'
     writec %50
     %50 = 'p'
     writec %50
     %50 = 'r'
     writec %50
     %50 = 'e'
     writec %50
     %50 = 's'
     writec %50
     %50 = ' '
     writec %50
     %50 = 'd'
     writec %50
     %50 = 'e'
     writec %50
     %50 = ' '
     writec %50
     %50 = 'g'
     writec %50
     %50 = '('
     writec %50
     %50 = 'a'
     writec %50
     %50 = ')'
     writec %50
     %50 = '.'
     writec %50
     %50 = ' '
     writec %50
     %50 = 'a'
     writec %50
     %50 = ':'
     writec %50
     %50 = ' '
     writec %50
     %51 = 0
     i = %51
  label doWhile6 :
     %52 = 10
     %53 = i < %52
     ifFalse %53 goto endWhile6
     %54 = a[i]
     writei %54
     %56 = ' '
     writec %56
     %58 = 1
     %59 = i + %58
     i = %59
     goto doWhile6
  label endWhile6 :
     %60 = '\n'
     writec %60
     return
endfunction


