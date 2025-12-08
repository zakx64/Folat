# DTM (DaTa Manager BETA v0.0.1) esolang

DTM is a primitive Finite State Machine computational class esolang. This repository presents the complete DTM project, being fully Open-Source under the MIT license, allowing anyone who clones this repository to change, add or remove data and files, even allowing the republication of this modified repository. It was officially created in 2025.

## Content
- "dlib.py"     : Library with functions that perform the instructions.
- "example.dtm" : Example of how to print a "Hello, World!" in DTM.
- "main.py"     : Main file, contains the interpreter.

## Instalation and execution
```bash
git clone https://github.com/mker-bin/DTM/
cd DTM
python3 main.py
```

If you does'nt have Python 3:
```bash
sudo apt update
sudo apt install python3
```

## Errors and Meanings
| Error                             | Meaning                         |
| --------------------------------- | ------------------------------- |
| `error: unknown definer '*'`      | Definer written wrong           |
| `error: unknown instruction '*'`  | Written instruction incorrectly |
| `error: macro '*' does not exist` | Macro name misspelled           |

## Required
Only [Python 3](https://www.python.org/downloads/) is required in this repository in this version.
> Note: The project was done in Python 3, so previous versions may not work!

## Examples
### "Hello, World!":
```
.m dm1 Hello, World!
set dm1
out 10r
```
Output: `Hello, World!`

> Note: more examples and resources will come up over time!

## Links
- Original and confirmed author on GitHub:       [mker-bin](https://github.com/mker-bin/)
- Original and confirmed author on Esolangs.org: [Mker-bin](https://esolangs.org/wiki/User:Mker-bin/)
- GitHub repository:                             [DTM repository](https://github.com/mker-bin/DTM)
- DTM page on esolangs.org:                      [DTM page](https://esolangs.org/wiki/DTM/)
