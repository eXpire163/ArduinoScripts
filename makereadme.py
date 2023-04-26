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

def makeTitle(file):
    readme = os.path.basename(file)[:-4]+"/README.md";
    if os.path.exists(readme):
        return f'[{os.path.basename(file)}](./{readme})'
    else:
        return os.path.basename(file)




files = glob.glob("./**/*.ino")
files.sort(key=lambda v: v.upper())
readme = "# Arduino script collection\n\n"
readme += "| File | Details |\n"
readme += "| --- | --- |\n"

for file in files:
    with open(file) as f:
        first_line = f.readline().rstrip()
        title = makeTitle(file)
        readme += f'| {title} | {parseHeader(first_line, file)} |\n'


readme += "\n\n\n*This file is generated via github actions, please do not change it by hand*"

f = open("README.md", "w")
f.write(readme)
f.close()
