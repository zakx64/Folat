import dlib

def interpreter(line):
    line = line.strip()
    if line[0] != ";": # if line starts with ';', ignore. Line starting with ';' is a comment. You can add inline comments by simply placing the comment at the end of the line text
       if line[0] == '.': # definer section
           if line[1] == 'm':
               macr = line[3:6] # predefined macro name
               mval = line[7:]  # captures all text after index 7
               return dlib.macro_definer(macr, mval)
           else:
               return f"error: unknown definer '{line[1]}'"

       else: # common instruction section
           inst = line[0:3]

           if inst == "set":
               arg1 = line[4:7]
               return dlib.inst_set(arg1)

           elif inst == "out":
               arg1 = line[4:7]
               return dlib.r_var
           else:
               return f"error: unknown instruction '{inst}'"

def main():
    # you can change this part to read your other files! DTM will soon have its own CLI command ...
    with open("example.dtm", "r") as f:
        for line in f:
            output = interpreter(line)

            # verify empty output
            if output != None:
                print(output)
main()