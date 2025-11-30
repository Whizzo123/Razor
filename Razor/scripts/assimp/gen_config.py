import sys
import re

def get_git_hash():
    try:
        # Run git from the Assimp source directory
        here = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "vendor", "assimp"))
        result = subprocess.run(
            ["git", "rev-parse", "--short=8", "HEAD"],
            cwd=here,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            text=True
        )
        hash = result.stdout.strip()
        if hash:
            return hash
    except Exception:
        pass
    return "0"

git_hash = get_git_hash()

major, minor, patch, soversion = sys.argv[1:5]
enabled_options = sys.argv[5:]  # e.g., ['ASSIMP_DOUBLE_PRECISION', 'ASSIMP_BUILD_OBJ_IMPORTER']

template = open('../../vendor/assimp/include/assimp/config.h.in').read()

out = template
out = out.replace('@ASSIMP_VERSION_MAJOR@', major)
out = out.replace('@ASSIMP_VERSION_MINOR@', minor)
out = out.replace('@ASSIMP_VERSION_PATCH@', patch)
out = out.replace('@ASSIMP_VERSION@', f"{major}.{minor}.{patch}")
out = out.replace('@ASSIMP_SOVERSION@', soversion)
out = out.replace('@GIT_COMMIT_HASH@', git_hash)

# Convert #cmakedefine lines to valid C/C++
def replace_cmakedefine(match):
    symbol = match.group(1)
    if symbol in enabled_options:
        return f"#define {symbol} 1"
    else:
        return f"/* #undef {symbol} */"

out = re.sub(r'#cmakedefine (\w+)(?: .*)?', replace_cmakedefine, out)

open('../../vendor/assimp/include/assimp/config.h', 'w').write(out)

print("config.h generated.")