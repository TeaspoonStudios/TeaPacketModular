import os, sys, shutil
sys.path.insert(0, os.path.dirname(__file__))
import hlsl

def parse_and_copy(source_file, dest_file, root_dir):
    hlsl.parse_and_copy(source_file, dest_file, root_dir)