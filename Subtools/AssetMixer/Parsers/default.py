import os, shutil

def parse_and_copy(source_file, dest_file):
    dest_dir = os.path.dirname(dest_file)
    os.makedirs(dest_dir, exist_ok=True)

    shutil.copyfile(source_file, dest_file)