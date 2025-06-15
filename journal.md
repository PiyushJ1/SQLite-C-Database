# SQLite Database in C Dev Log

**Building a SQLite-like database in C, following https://cstack.github.io/db_tutorial/ to understand and apply core low-level concepts. My aim is to learn how databases work at a deeper level and then extend this foundation by adding my own features, such as (planned): Query Parser Improvements, B-Tree indexing, and Persisting to Disk in a binary format.**

## Day 1 - 9/6/25
**What I worked on:**  
- Worked on part 1 of implementing the database. Set up a basic Read, Eval, Print, Loop. Since the getline() function does not exist on Windows in C99, I implemented the function manually. Implemented the elementary functions for parsing user input such as reading the input, printing the prompt, initialising the input buffer struct, and closing the input buffer at the end of the read, eval, print loop.

**What I learned:**  
- Learned how to implement the getline() function, which involved double pointers (an area I'm shaky on). Also learned about the program life cycle, specifically for database programs. Also learned the core difference between fgets() and getline(). fgets() relies on the input text from stdin to be a constant size, and doesn't read characters past the given number. Whilst, getline() can read an input buffer of any size since it can dynamically allocate more memory to hold more chracters.

**What challenges I faced:**  
- The getline() function does not exist in C99, so I could not simply use this function to read user input. Instead, I had to implement the function manually using fgets and use dynamic memory allocation. 

**Next steps:**  
- Start work on part 2, which includes the implementation of Meta commands (not entirely sure of them yet). Need to also update the REPL for this next step.

## Day 1 - 9/6/25
**What I worked on:**  
- Worked on part 1 of implementing the database. Set up a basic Read, Eval, Print, Loop. Since the getline() function does not exist on Windows in C99, I implemented the function manually. Implemented the elementary functions for parsing user input such as reading the input, printing the prompt, initialising the input buffer struct, and closing the input buffer at the end of the read, eval, print loop.

**What I learned:**  
- Learned how to implement the getline() function, which involved double pointers (an area I'm shaky on). Also learned about the program life cycle, specifically for database programs. Also learned the core difference between fgets() and getline(). fgets() relies on the input text from stdin to be a constant size, and doesn't read characters past the given number. Whilst, getline() can read an input buffer of any size since it can dynamically allocate more memory to hold more chracters.

**What challenges I faced:**  
- The getline() function does not exist in C99, so I could not simply use this function to read user input. Instead, I had to implement the function manually using fgets and use dynamic memory allocation. 

**Next steps:**  
- Start work on part 2, which includes the implementation of Meta commands (not entirely sure of them yet). Need to also update the REPL for this next step.

