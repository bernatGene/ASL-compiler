function f
  params
    _result
    a
    b
  endparams

   %1 = a + b
   %2 = 1
   %3 = %1 + %2
   _result = %3
   return
   return
endfunction

function main
  vars
    a 1
    b 1
    c 5
  endvars

   %1 = 1
   a = %1
   %2 = 2
   b = %2
   pushparam 
   pushparam a
   pushparam b
   call f
   popparam 
   popparam 
   popparam %3
   writei %3
   writeln
   pushparam 
   pushparam a
   pushparam 
   pushparam a
   pushparam b
   call f
   popparam 
   popparam 
   popparam %6
   pushparam %6
   call f
   popparam 
   popparam 
   popparam %7
   writei %7
   writeln
   readi b
   writei b
   writeln
   %12 = 0
   readi %13
   c[%12] = %13
   %14 = 0
   %15 = c[%14]
   writei %15
   writeln
   return
endfunction


