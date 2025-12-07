import dlib

def interpreter(line):
    if line[0] == '.':
        if line[1] == 'm':
            macr = line[3:6]
            mval = line[7:]
            return dlib.macro_definer(macr, mval)
        else:
            return f"error: unknown definer '{line[1]}'"
    else:
        inst = line[0:3]
        if inst == "set":
            arg1 = line[4:7]
            return dlib.inst_set(arg1)
        elif inst == "out":
            arg1 = line[4:7]
            return dlib.inst_out()
        else:
            return f"error: unknown instruction '{inst}'"

def main():
    with open("t.dtm", "r") as f:
        for line in f:
            output = interpreter(line)
            if output != None:
                print(output)
main()