import importlib.util, os, sys

def run(asset_source, build_path, asset_platform):
    abspath = os.path.abspath(__file__)
    dname = os.path.dirname(abspath)
    os.chdir(dname)

    platform_script = "Platform/" + asset_platform + ".py"
    if not os.path.exists(platform_script):
        raise Exception("Platform " + asset_platform + " is not a valid Asset Platform!")


    spec = importlib.util.spec_from_file_location(platform_script, platform_script)
    import_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(import_module)
    dest_dir = import_module.set_dest_dir(build_path)


    # SUPER TEMP
    parser_path = os.getcwd() + "/Parsers/default.py"
    spec = importlib.util.spec_from_file_location(parser_path, parser_path)
    import_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(import_module)

    for root, dirs, files in os.walk(asset_source):
        for filename in files:
            asset_path = os.path.join(root,filename).replace(asset_source, "")[1:]
            # SUPER TEMP
            import_module.parse_and_copy(os.path.join(root,filename), dest_dir + asset_path)

if __name__ == "__main__":
    run(sys.argv[1], sys.argv[2], sys.argv[3])