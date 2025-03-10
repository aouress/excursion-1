# excursion-1
An excursion for Programming for Electrical Engineering II tasking us to create a circuit analysis tool for resistive circuits.

We will not be attempting the bonus. 

## Compiliation and Execution 

### VS Code
- To run the code in VS Code through `main`, ensure the `tasks.json` file is configured such that `args` includes the separate `.cpp` project files, not including `main`. The `args` section in `tasks.json` should look like the following: 

```
"args": [
                "-fdiagnostics-color=always",
                "-g",
                "parse.cpp",
                "matrix_solve.cpp",
                "code_tu_matrix.cpp",
                "${file}",
                "-o",
                "${fileDirname}\\${fileBasenameNoExtension}.exe"
            ],
```
- Then, open `main.cpp`, click **Run** to build and execute the program.

### Terminal 
1. Navigate to the directory that includes the project files. You will need all `.cpp` and `.h` files included in the submission.
2. Ensure that a file named `netlist.txt` containing the resistive circuit is located in the same directory as all other files. There should be no empty lines in the file.
3. Run the following command to compile the program and generate the `ex1` executable:

```
g++ main.cpp parse.cpp code_tu_matrix.cpp matrix_solve.cpp -o ex1
```

4. Run the code by providing the following command

```
.\ex1
```
