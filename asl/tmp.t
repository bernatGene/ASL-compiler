function f
  params
    v
  endparams

  vars
    c 10
    i 1
  endvars

     %1 = v
     c = %1
     %2 = 'e'
     writec %2
     %2 = 'n'
     writec %2
     %2 = ' '
     writec %2
     %2 = 'f'
     writec %2
     %2 = '.'
     writec %2
     %2 = ' '
     writec %2
     %2 = 'c'
     writec %2
     %2 = ':'
     writec %2
     %2 = ' '
     writec %2
     %3 = 0
     i = %3
  label doWhile1 :
     %4 = 10
     %5 = i < %4
     ifFalse %5 goto endWhile1
     %6 = c[i]
     writei %6
     %8 = ' '
     writec %8
     %10 = 1
     %11 = i + %10
     i = %11
     goto doWhile1
  label endWhile1 :
     %12 = '\n'
     writec %12
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
     %8 = d
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
     b = a
     %7 = 'd'
     writec %7
     %7 = 'e'
     writec %7
     %7 = 's'
     writec %7
     %7 = 'p'
     writec %7
     %7 = 'r'
     writec %7
     %7 = 'e'
     writec %7
     %7 = 's'
     writec %7
     %7 = ' '
     writec %7
     %7 = 'd'
     writec %7
     %7 = 'e'
     writec %7
     %7 = ' '
     writec %7
     %7 = 'b'
     writec %7
     %7 = '='
     writec %7
     %7 = 'a'
     writec %7
     %7 = '.'
     writec %7
     %7 = ' '
     writec %7
     %7 = 'b'
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
     %11 = b[i]
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
     %19 = 'd'
     writec %19
     %19 = 'e'
     writec %19
     %19 = 's'
     writec %19
     %19 = 'p'
     writec %19
     %19 = 'r'
     writec %19
     %19 = 'e'
     writec %19
     %19 = 's'
     writec %19
     %19 = ' '
     writec %19
     %19 = 'd'
     writec %19
     %19 = 'e'
     writec %19
     %19 = ' '
     writec %19
     %19 = 'b'
     writec %19
     %19 = '='
     writec %19
     %19 = 'a'
     writec %19
     %19 = '.'
     writec %19
     %19 = ' '
     writec %19
     %19 = 'a'
     writec %19
     %19 = ':'
     writec %19
     %19 = ' '
     writec %19
     %20 = 0
     i = %20
  label doWhile3 :
     %21 = 10
     %22 = i < %21
     ifFalse %22 goto endWhile3
     %23 = a[i]
     writei %23
     %25 = ' '
     writec %25
     %27 = 1
     %28 = i + %27
     i = %28
     goto doWhile3
  label endWhile3 :
     %29 = '\n'
     writec %29
     %31 = &a
     pushparam %31
     call f
     popparam 
     %32 = 'd'
     writec %32
     %32 = 'e'
     writec %32
     %32 = 's'
     writec %32
     %32 = 'p'
     writec %32
     %32 = 'r'
     writec %32
     %32 = 'e'
     writec %32
     %32 = 's'
     writec %32
     %32 = ' '
     writec %32
     %32 = 'd'
     writec %32
     %32 = 'e'
     writec %32
     %32 = ' '
     writec %32
     %32 = 'f'
     writec %32
     %32 = '('
     writec %32
     %32 = 'a'
     writec %32
     %32 = ')'
     writec %32
     %32 = '.'
     writec %32
     %32 = ' '
     writec %32
     %32 = 'a'
     writec %32
     %32 = ':'
     writec %32
     %32 = ' '
     writec %32
     %33 = 0
     i = %33
  label doWhile4 :
     %34 = 10
     %35 = i < %34
     ifFalse %35 goto endWhile4
     %36 = a[i]
     writei %36
     %38 = ' '
     writec %38
     %40 = 1
     %41 = i + %40
     i = %41
     goto doWhile4
  label endWhile4 :
     %42 = '\n'
     writec %42
     %44 = &a
     pushparam %44
     call g
     popparam 
     %45 = 'd'
     writec %45
     %45 = 'e'
     writec %45
     %45 = 's'
     writec %45
     %45 = 'p'
     writec %45
     %45 = 'r'
     writec %45
     %45 = 'e'
     writec %45
     %45 = 's'
     writec %45
     %45 = ' '
     writec %45
     %45 = 'd'
     writec %45
     %45 = 'e'
     writec %45
     %45 = ' '
     writec %45
     %45 = 'g'
     writec %45
     %45 = '('
     writec %45
     %45 = 'a'
     writec %45
     %45 = ')'
     writec %45
     %45 = '.'
     writec %45
     %45 = ' '
     writec %45
     %45 = 'a'
     writec %45
     %45 = ':'
     writec %45
     %45 = ' '
     writec %45
     %46 = 0
     i = %46
  label doWhile5 :
     %47 = 10
     %48 = i < %47
     ifFalse %48 goto endWhile5
     %49 = a[i]
     writei %49
     %51 = ' '
     writec %51
     %53 = 1
     %54 = i + %53
     i = %54
     goto doWhile5
  label endWhile5 :
     %55 = '\n'
     writec %55
     return
endfunction


