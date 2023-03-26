(SIASL)² or siasl2 is the follow-up language to SIASL. 
Their design is somewhat similar but (SIASL)² is much richer. 
and has a important differences with SIASL.

Like SIASL (SIASL)² is a turing tarpit inspired by brainfuck. 

a program a set on instructions applied on a 2d grid. 
However, (SIASL)² doesn't work the way siasl does. 

An instruction/symbols in (SIASL)² is 2 characters instead of one. 

Some instruction/symbols  are predefined and are immutable some other can be user defined. 

To learn more about how (SIASL)² works and how to write (SIASL)² programs; please check out the 
docu/docu.html file.

To compile the (SIASL)² interpreter you'll need flex, bison ,gcc and make.

once compiler the (SIASL)² interpreter can be called with arguments : 

-c loads the command line interactive interpreter 
-s starts (SIASL)² with bf-like symbol lexic.
-f reads a program from a file
-h prints out help 
-x starts (SIASL)² with hexadecimal lexic instead of normal bf-like symbols 
   nb: hexsiasl's current implementation is pretty awful and has worse performance than normal
       (SIASL)²

the default behavior of siasl2 when executed is to load the command line interpreter with the 
bf-like lexic (-sc)

If you call (SIASL)² with mutually exclusive options (like -sx) , the LAST one passed will be the 
one used at runtime.