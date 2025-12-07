class macros:
    def __init__(self):
        self.dm1 = ""
        self.dm2 = ""
        self.dm3 = ""
global r_var
global _macro
r_var = ""
_macro = macros()

def macro_definer(macro, val):
    global _macro
    if macro == "dm1":
        _macro.dm1 = val
    elif macro == "dm2":
        _macro.dm2 = val
    elif macro == "dm3":
        _macro.dm3 = val

def inst_set(arg1):
    global r_var
    if arg1 == "dm1":
        r_var = _macro.dm1
    elif arg1 == "dm2":
        r_var = _macro.dm2
    elif arg1 == "dm3":
        r_var = _macro.dm3
    else:
        return f"error: macro '{arg1}' does not exist"

def inst_out():
    return r_var