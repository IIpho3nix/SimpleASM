INSTRUCTIONS (addresses are signed 64-bit and values are signed 64-bit):

ADD <IN ADDR 1>, <IN ADDR 2>, <OUT ADDR> -- performs addition from inputs and saves the value to the output
SUB <IN ADDR 1>, <IN ADDR 2>, <OUT ADDR> -- performs subtraction from inputs and saves the value to the output
MUL <IN ADDR 1>, <IN ADDR 2>, <OUT ADDR> -- performs multiplication from inputs and saves the value to the output
DIV <IN ADDR 1>, <IN ADDR 2>, <OUT ADDR> -- performs division from inputs and saves the value to the output
REM <IN ADDR 1>, <IN ADDR 2>, <OUT ADDR> -- performs remainder from inputs and saves the value to the output

STR <VALUE>, <ADDR TO STORE> -- stores a 64-bit signed integer on address
CPY <IN ADDR>, <CPY ADDR> -- copies from one address to another

AND <IN ADDR 1>, <IN ADDR 2>, <OUT ADDR> -- performs an AND operation from inputs and saves it to the output
OR <IN ADDR 1>, <IN ADDR 2>, <OUT ADDR> -- performs an OR operation from inputs and saves it to the output
XOR <IN ADDR 1>, <IN ADDR 2>, <OUT ADDR> -- performs an XOR operation from inputs and saves it to the output
NOT <IN ADDR>, <OUT ADDR> -- performs a NOT operation on input and saves it to the output

JMP <LABEL> -- jumps to label
JMPBZ <IN ADDR>, <LABEL> -- jumps to label if the value at the input address is bigger than 0
JMPZ <IN ADDR>, <LABEL> -- jumps to label if the value at the input address is 0
JMPIF <IN ADDR 1>, <IN ADDR 2>, <LABEL> -- jumps to label if the value at input 1 is equal to the value at input 2

SOUT <IN ADDR 1>, <IN ADDR 2> -- the first variable defines the starting of the string to be written to the system output represented in ASCII, and the second variable defines the string length of the string
SIN <IN ADDR>, <OUT ADDR> -- reads the specified length of input from the system input and saves it to memory starting from the output address

RIN <ADDR 1>, <ADDR 2> -- starts writing command line arguments passed starting address 1 and saves the length to address 2

HALT <VALUE> -- stops the program with the given value as the exit code

EXAMPLE CODE:

	HELLO WORLD:
		<main>:
		    STR 72,1   -- Store ASCII 'H' at address 1
		    STR 101,2  -- Store ASCII 'e' at address 2
		    STR 108,3  -- Store ASCII 'l' at address 3
		    STR 108,4  -- Store ASCII 'l' at address 4
		    STR 111,5  -- Store ASCII 'o' at address 5
		    STR 44,6   -- Store ASCII ',' at address 6
		    STR 32,7   -- Store ASCII ' ' at address 7
		    STR 87,8   -- Store ASCII 'W' at address 8
		    STR 111,9  -- Store ASCII 'o' at address 9
		    STR 114,10 -- Store ASCII 'r' at address 10
		    STR 108,11 -- Store ASCII 'l' at address 11
		    STR 100,12 -- Store ASCII 'd' at address 12
		    STR 33,13  -- Store ASCII '!' at address 13
		    STR 13,14 -- Store lenght of the string at adress 14
		    SOUT 1,14  -- Output the string
		    HALT 0     -- End the program
			
	CHECK IF 74 IS EVEN:
		<main>:
			STR 74,11 -- store value 74 at adress 11
			JMP <iseven>
	
		<print>:
			STR 1,18
			SOUT 15,18
			HALT 0
	
		<iseven>:
			STR 1,12 -- store 1 at adress 12
			AND 11,12,13 -- AND adress 11 (external input) with 1 and save result to adress 13
			JMPZ 13,<even> -- jump to even if even
			JMP <odd> -- jump to odd
		
		<even>:
			STR 69,15 -- store ascii "E" in adress 15 (external output)
			JMP <print>
			
		<odd>:
			STR 79,15 -- store ascii "O" in adress 15 (external output)
			JMP <print>
		
