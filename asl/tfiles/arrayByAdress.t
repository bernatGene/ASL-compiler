function main
vars 
b 10
endvars
%1 = 5
b[%1] = %1
%2 = b[%1]
writei %2
%3 = &b
%4 = %3[%1]
writei %4
return
endfunction
