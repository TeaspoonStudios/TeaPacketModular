import os, subprocess, shutil

mydir = os.path.dirname(__file__)

def find_gameinput():
    dirs = os.listdir(f"{os.environ["VCPKG_ROOT"]}/packages/")
    for dir in dirs:
        if "gameinput" in dir:
            return f"{os.environ["VCPKG_ROOT"]}/packages/{dir}"
    return None

def check():
    if not os.path.exists(f"{mydir}/gameinput_dir.gen"):
        return False
    with open(f"{mydir}/gameinput_dir.gen", "r") as infile:
        text = infile.read()
        if not os.path.isdir(text):
            return False

    return True

def grab():
    subprocess.run(f"vcpkg install gameinput")
    with open(f"{mydir}/gameinput_dir.gen", "w") as outfile:
        outfile.write(find_gameinput())




if not check():
    grab()