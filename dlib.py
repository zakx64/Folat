# The code includes C representations for future updates

# struct r_mcr {
#     char dm1[30];
#     char dm2[30];
#     char dm3[30];
# };
class macros:
    def __init__(self):
        self.dm1 = ""
        self.dm2 = ""
        self.dm3 = ""
global r_var
global r_mcr

# resouces:
r_var = ""       # main var ; C representation: char r_var[50];
r_mcr = macros() # predefined macros

# .m <dm1, dm2, dm3> <value>
def macro_definer(macro, val):
    global r_mcr
    if macro == "dm1":
        r_mcr.dm1 = val
    elif macro == "dm2":
        r_mcr.dm2 = val
    elif macro == "dm3":
        r_mcr.dm3 = val

# set <dm1, dm2, dm3>
def inst_set(arg1):
    global r_var

    # checks if the macro name exists in r_mcr
    if arg1 == "dm1":
        r_var = r_mcr.dm1
    elif arg1 == "dm2":
        r_var = r_mcr.dm2
    elif arg1 == "dm3":
        r_var = r_mcr.dm3
    else:
        return f"error: macro '{arg1}' does not exist"

# the "out" instruction is not necessary here