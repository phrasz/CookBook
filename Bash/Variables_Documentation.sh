#!/bin/bash
#Arithmetic Expansion in Bash Shell
# http://bash.cyberciti.biz/guide/Perform_arithmetic_operations

# Arithmetic expansion and evaluation is done by placing an integer expression using the following format:
#$((expression))
#$(( n1+n2 ))
#$(( n1/n2 ))
#$(( n1-n2 ))
#Examples
#Add two numbers on fly using the echo command:
echo $(( 10 + 5 ))
#Add two numbers using x and y variable. Create a shell program called add.sh using a text editor:

x=5
y=10
ans=$(( x + y ))
echo "$x + $y = $ans"
#Save and close the file. Run it as follows:
#chmod +x add.sh
#./add.sh
#Sample Outputs:
#5 + 10 = 15


#Create an interactive program using the read command called add1.sh using a text editor:
read -p "Enter two numbers : " x y
ans=$(( x + y ))
echo "$x + $y = $ans"
#Save and close the file. Run it as follows:
#Sample Outputs:
#Enter two numbers : 20 30
#20 + 30 = 50

#################################################################
#Mathematical Operators With Integers
#Operator	Description	Example	Evaluates To
#+	 Addition	 echo $(( 20 + 5 ))	 25
#-	 Subtraction	 echo $(( 20 - 5 ))	 15
#/	 Division	 echo $(( 20 / 5 ))	 4
#*	 Multiplication	 echo $(( 20 * 5 ))	 100
# %	 Modulus	 echo $(( 20 % 3 ))	 2
#++	 post-increment (add variable value by 1)
x=5
echo $(( x++ ))
echo $(( x++ ))
#5
#6
#--	 post-decrement (subtract variable value by 1)	 x=5
echo $(( x-- ))
# 4
#**	 Exponentiation
x=2
y=3
echo $(( x ** y ))
#8

#########################################################
#Order of Precedence
#Operators are evaluated in order of precedence. The levels are listed in order of decreasing precedence (quoting form the bash man page).
#      id++ id--
#             variable post-increment and post-decrement
#       ++id --id
#              variable pre-increment and pre-decrement
#       - +    unary minus and plus
#       ! ~    logical and bitwise negation
#       **     exponentiation
#       * / %  multiplication, division, remainder
#       + -    addition, subtraction
#       << >>  left and right bitwise shifts
#       <= >= < >
#              comparison
#       == !=  equality and inequality
#       &      bitwise AND
#       ^      bitwise exclusive OR
#       |      bitwise OR
#       &&     logical AND
#       ||     logical OR
#       expr?expr:expr
#              conditional operator
#       = *= /= %= += -= <<= >>= &= ^= |=
#              assignment
#       expr1 , expr2
#              comma
