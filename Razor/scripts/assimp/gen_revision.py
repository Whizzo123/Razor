import sys
import re
import os

def get_git_info():
    try:
        # Assimp source directory
        here = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "vendor", "assimp"))

        # Get short commit hash
        result_hash = subprocess.run(
            ["git", "rev-parse", "--short=8", "HEAD"],
            cwd=here,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            text=True
        )
        git_hash = result_hash.stdout.strip() or "0"

        # Get current branch name
        result_branch = subprocess.run(
            ["git", "rev-parse", "--abbrev-ref", "HEAD"],
            cwd=here,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            text=True
        )
        git_branch = result_branch.stdout.strip() or "unknown"

        return git_hash, git_branch

    except Exception:
        return "0", "unknown"


git_hash, git_branch = get_git_info()

major, minor, patch, package_version = sys.argv[1:5]

template = open('../../vendor/assimp/include/assimp/revision.h.in').read()

out = template
out = out.replace('@ASSIMP_VERSION_MAJOR@', major)
out = out.replace('@ASSIMP_VERSION_MINOR@', minor)
out = out.replace('@ASSIMP_VERSION_PATCH@', patch)
out = out.replace('@ASSIMP_PACKAGE_VERSION@', package_version)
out = out.replace('@GIT_COMMIT_HASH@', git_hash)
out = out.replace('@GIT_BRANCH@', git_branch)

# Convert #cmakedefine lines to valid C/C++
def replace_cmakedefine(match):
    symbol = match.group(1)
    if symbol in enabled_options:
        return f"#define {symbol} 1"
    else:
        return f"/* #undef {symbol} */"

out = re.sub(r'#cmakedefine (\w+)(?: .*)?', replace_cmakedefine, out)

open('../../vendor/assimp/include/assimp/revision.h', 'w').write(out)

print("revision.h generated.")