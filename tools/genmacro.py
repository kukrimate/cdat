#
# Generate a correctly padded multi-line macro
#

import sys

if len(sys.argv) < 2:
    print(f"Usage: {sys.argv[0]} FILE", file=sys.stderr)
    exit(1)

# Target path
path = sys.argv[1]

# Read file
with open(path, "r") as f:
    data = f.read()

# Output data
outdata = ""

START_MARKER = "##MACRO##\n"
END_MARKER = "##ENDMACRO##\n"

# Find all macros
previdx = 0
while True:
    startidx = data.find(START_MARKER, previdx)
    if startidx < 0:
        break
    endidx = data.find(END_MARKER, startidx)
    if endidx < 0:
        break

    # Append preceeding text
    outdata += data[previdx:startidx]

    # Get macro lines
    lines = data[startidx + len(START_MARKER):endidx].split("\n")
    if lines[-1] == "":
        lines.pop(-1)

    # Append each line properly padded
    for line in lines:
        outdata += line.rstrip().ljust(79) + "\\\n"

    previdx = endidx + len(END_MARKER)

# Rest of the file
outdata += data[previdx:]

# Write file
with open(path, "w") as f:
    f.write(outdata)
