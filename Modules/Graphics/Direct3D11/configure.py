import importlib.util, os, sys

sys.path.insert(0, os.path.dirname(__file__))

import AssetMixer.setup

def configure(teapacket_root):
    old_cwd = os.getcwd()

    abspath = os.path.abspath(__file__)
    dname = os.path.dirname(abspath)
    os.chdir(dname)

    spec = importlib.util.spec_from_file_location("enum_map_gen", teapacket_root + "/Generators/EnumMapGen/main.py")
    import_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(import_module)
    print(os.getcwd())
    import_module.generate(os.getcwd() + "/generators/Texture.gmap")
    import_module.generate(os.getcwd() + "/generators/TextureFilter.gmap")
    import_module.generate(os.getcwd() + "/generators/TextureWrap.gmap")

    AssetMixer.setup.setup()

    os.chdir(old_cwd)