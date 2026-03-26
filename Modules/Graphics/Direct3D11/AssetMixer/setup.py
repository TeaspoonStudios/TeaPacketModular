import sys, os

os.chdir(os.path.dirname(__file__))

from Setup import spirv_cross

def setup():
    if not spirv_cross.check():
        spirv_cross.grab()