# ZigZag


# Project Overview

This project is primarily inspired by Crow's videos. => https://www.youtube.com/watch?v=A6EKDAKBXPs 

I introduced an automated PID search to eliminate the need for manual search. 

It's a straightforward implementation of shellcode injection.

Notably, the executable still maintains a good score on VirusTotal! (15/71) :D
With a good cryptor and better shellcode encoding, there's surely something to be done. 

# Usage 

1.  Clone the repository:
        
    `git clone https://github.com/creacitysec/ZigZag` 
    
2.  Change the shellcode to the one you prefer in the `needNuke[]` array.
    
3.  Update the target executable (the one undergoing injection) in the `ProcessUtil.cpp` file within the module search.
    
4.  Compile in x64 debug mode.
    
5.  Voilà.


# :)

I'll keep maintaining this project on my spare time so come check for new updates!! 
