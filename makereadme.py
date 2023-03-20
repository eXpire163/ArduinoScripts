import glob
import os


prefix = "// about: "
placeholder = prefix+"no header yet\n"

def addHeader(file):
    f = open(file,'r+')
    lines = f.readlines() # read old content
    f.seek(0) # go back to the beginning of the file
    f.write(placeholder) # write new content at the beginning
    for line in lines: # write old content after new
        f.write(line)
    f.close()


def parseHeader(line, file):
    if(not line.startswith(prefix)):
        addHeader(file)
        return "no header"
    return line[len(prefix):]


# test
assert(parseHeader("// about: test") == "test")

files = glob.glob("./**/*.ino")
readme = "# Arduino script collection\n\n"
readme += "| File | Details |\n"
readme += "| --- | --- |\n"

for file in files:
    with open(file) as f:
        first_line = f.readline()
        readme += f'| {os.path.basename(file)} | {parseHeader(first_line, file)} |\n'


f = open("demofile2.md", "w")
f.write(readme)
f.close()
