import sys
import os
import subprocess

def is_git_directory(path = '.'):
    return subprocess.call(['git', '-C', path, 'status'],
         stderr=subprocess.STDOUT, stdout = open(os.devnull, 'w')) == 0

if not is_git_directory("."):
    print("'-DGIT_REPO_PRESENT=0'")
    sys.exit(0)

print("'-DGIT_REPO_PRESENT=1'")

revision = (
    subprocess.check_output(["git", "describe", "--always", "--dirty", "--tags"])
    .strip()
    .decode("utf-8")
)
print("'-DGIT_REV=\"%s\"'" % revision)

author = (
    subprocess.check_output(["git", "show", "-s", "--format=format:%an", "HEAD"])
    .strip()
    .decode("utf-8")
)
print("'-DGIT_AUTHOR=\"%s\"'" % author)

info = (
    subprocess.check_output(["git", "show", "-s", "--format=format:%s", "HEAD"])
    .strip()
    .decode("utf-8")
)
print("'-DGIT_SUBJECT=\"%s\"'" % info)

branch = (
    subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"])
    .strip()
    .decode("utf-8")
)
print("'-DGIT_BRANCH=\"%s\"'" % branch)

commitdate = (
    subprocess.check_output(["git", "show", "-s", "--format=format:%cD", "HEAD"])
    .strip()
    .decode("utf-8")
)
print("'-DGIT_COMMIT_DATE=\"%s\"'" % commitdate)
