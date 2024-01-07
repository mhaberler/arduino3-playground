
import os
import subprocess
import sys


def is_git_directory(path='.'):
    return subprocess.call(['git', '-C', path, 'status'],
                           stderr=subprocess.STDOUT, stdout=open(os.devnull, 'w')) == 0


buildinfo = open("src/buildinfo.h", 'w')

if not is_git_directory("."):
    print("'-DGIT_REPO_PRESENT=0'")

    buildinfo.write("#define GIT_REPO_PRESENT\t0\n")
    sys.exit(0)

revision = (
    subprocess.check_output(
        ["git", "describe", "--always", "--dirty", "--tags"])
    .strip()
    .decode("utf-8")
)
author = (
    subprocess.check_output(
        ["git", "show", "-s", "--format=format:%an", "HEAD"])
    .strip()
    .decode("utf-8")
)
info = (
    subprocess.check_output(
        ["git", "show", "-s", "--format=format:%s", "HEAD"])
    .strip()
    .decode("utf-8")
)
branch = (
    subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"])
    .strip()
    .decode("utf-8")
)
commitdate = (
    subprocess.check_output(
        ["git", "show", "-s", "--format=format:%cD", "HEAD"])
    .strip()
    .decode("utf-8")
)

buildinfo.write(f"#ifndef _BUILDINFO_H\n")
buildinfo.write(f"#define _BUILDINFO_H\n\n")
buildinfo.write(f"#define GIT_REPO_PRESENT 1\n\n")

buildinfo.write(f"const char *GIT_REV = \"{revision}\";\n")
buildinfo.write(f"const char *GIT_AUTHOR =\t\"{author}\";\n")
buildinfo.write(f"const char *GIT_SUBJECT =\t\"{info}\";\n")
buildinfo.write(f"const char *GIT_BRANCH =\t\"{branch}\";\n")
buildinfo.write(f"const char *GIT_COMMIT_DATE =\t\"{commitdate}\";\n")
buildinfo.write(f"\n#endif\n")
buildinfo.close()
