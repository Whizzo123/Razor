import sys
import re

template = open('../../vendor/assimp/contrib/zlib/zconf.h.in').read()

out = template

# Convert #cmakedefine lines to valid C/C++
def replace_cmakedefine(match):
    symbol = match.group(1)
    if symbol in enabled_options:
        return f"#define {symbol} 1"
    else:
        return f"/* #undef {symbol} */"

out = re.sub(r'#cmakedefine (\w+)(?: .*)?', replace_cmakedefine, out)

open('../../vendor/assimp/contrib/zlib/zconf.h', 'w').write(out)

print("zconf.h generated.")