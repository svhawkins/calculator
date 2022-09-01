# calculator
string-based calculator using a simple tokeniser and a stack to parse mathematical expressions in order to evaluate them. Able to evaluate real, imginary, and complex numbers, as well as do boolean operations.


running the program:

runnable on command line with the provided makefile, under the execuatable name "calculator" once made.
./calculator

otherwise, it can be run on visual studio (or even vs code and the like) by downloading all files but the makefile, and storing them to a new project solution.


BUGS:
1. operators and operands must be seperated with a space. 
  There are some exceptions to this (such as negative numbers and unary operators)
  There are still issues with the tokeniser/scanner that I still need to resolve in order to read not-as-nicely formatted input correctly. 
  For the time being, please have input strings as close to standard form as possible (that includes proper parenthetization)
  
 2. floating point numbers that are otherwise whole are not considered integers, and may raise errors where it shouldn't.
 
  
  
  OPERATORS:
  there are currently 38 operators: arithmetic, relational, logical, and bitwise.
  
 ARITHMETIC OPERATORS:
+: addition
-: subtraction
*: multiplication
/: division
%: remainder
**: exponentiation
//: floor division
%%: equivalence modulo
sqrt: square root
cbrt: cube root
rt: root (requires 2 operands: rta(b) is the ath root of b, if only 1 provided it defaults to a square root)
ln: natural logarithm
lg: base 2 logarithm
log: logarithm (requires 2 operands: loga(b) is the logarithm of b in base a, if only 1 provided it defaults to a base of 10)
fact: factorial
fib: fibonacci number
<<: left shift (requires 3 operands: a << b c is the same as multiplying a with b, where b is raised to the cth power. if only 2 operands are given, it defaults to a base, b, of 2)
>> : right shift: (requires 3 operands: a >> b c is the same as dividing a by b, where b is raised to the cth power. if only 2 operands are given, it defaults to a base, b, of 2)

RELATIONAL OPERATORS:
**other than == and !=, complex numbers are invalid operands and will result in an error (this includes one operand being imaginary and the other being real)
== : equal to
!=: not equal to
<: less than
<=: less than or equal to
>: greater than
>=: greater than or equal to

LOGICAL OPERATORS:
both operands must be of boolean type
!: logical not
&&: logical and
|| : logical or (inclusive or)
^^ : logical xor (exclusive or)
!&: logical nand
!|: logical nor
!^: logical xnor

BITWISE OPERATORS:
**these accept only either real or imaginary integer operands, otherwise an error occurs.

~: bit inversion
&: bitwise and
|: bitwise or
^: bitwise xor
~&: bitwise nand
~|: bitwise nor
~^: bitwise xnor

*********************************************
since this is only the 1st version so far, it is not the best. I hope to add further features in the future in later versions:

1. bug fixes
2. implementation of trigonometric functions
3. adding graphics to look like calculator



  
