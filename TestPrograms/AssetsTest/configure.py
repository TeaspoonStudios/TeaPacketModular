import os, importlib.util


def configure(teapacket_root):
    old_cwd = os.getcwd()

    abspath = os.path.abspath(__file__)
    dname = os.path.dirname(abspath)
    os.chdir(teapacket_root)

    spec = importlib.util.spec_from_file_location("Subtools/AssetMixer/run.py", "Subtools/AssetMixer/run.py")
    import_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(import_module)
    os.chdir(dname)
    import_module.run(os.path.abspath("assets"), os.path.abspath("build/windows/Debug/"), "STDFile")

    os.chdir(old_cwd)


if __name__ == "__main__":
    configure("D:\\Programming\\TeaPacketModular")