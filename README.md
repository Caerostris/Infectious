# Infectious

A research project aiming to inject code into Linux ELF-binaries.  
The idea is to do the following:  

1. Parse an ELF-binary
2. Read `e_entry` field from parsed binary
3. Update user-defined code to jump to address `e_entry` when finished
4. Insert updated user-defined code into the .text section
5. Update the `e_entry` field to point to user-defined code

In practice, this is a little more complicated. Inserting some code will
change the offsets of all references, and it currently looks like half
the file will have to be re-written in the process to adjust the offsets.

## Project Status

The project is in its early stages.  
Infectious is currently capable of parsing the relevant headers and sections.  
The code must be considered unstable and may contain bugs and security holes.
