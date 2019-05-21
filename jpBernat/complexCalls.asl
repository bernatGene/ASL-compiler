func f(a:int,b:int) : int
  return a+b+1;
endfunc

func main() 
  var a : int
  var b : int
  var c : array[5] of int
  a = 1;
  b = 2;
  write f(a,b);
  write "\n";
  write f(a, f(a,b));
  write "\n";
  read b;
  write b;
  write "\n";
  read c[0];
  write c[0];
  write "\n";
endfunc
  
  
