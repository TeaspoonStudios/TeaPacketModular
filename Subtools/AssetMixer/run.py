import importlib.util, os, sys, pathlib

def run(asset_source, build_path, asset_platform, parser_dirs : set=set()):
    parser_dirs.add(os.path.dirname(__file__) + "/Parsers/")

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
    default_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(default_module)


    for root, dirs, files in os.walk(asset_source):
        for filename in files:
            foundSpec = False
            asset_path = os.path.join(root,filename).replace(asset_source, "")[1:]

            src_path = os.path.join(root,filename)
            dest_path = dest_dir + asset_path

            pathlib.Path(os.path.dirname(dest_path)).mkdir(parents=True, exist_ok=True)
            
            # SUPER TEMP
            for parser_dir in parser_dirs:
                script_src = parser_dir + os.path.splitext(asset_path)[1][1:] + ".py"
                if (os.path.exists(script_src)):
                    parser_path = script_src
                    spec = importlib.util.spec_from_file_location(parser_path, parser_path)
                    special_module = importlib.util.module_from_spec(spec)
                    spec.loader.exec_module(special_module)
                    special_module.parse_and_copy(src_path, dest_path)
                    foundSpec = True
                    break
            if not foundSpec:
                default_module.parse_and_copy(src_path,dest_path)

if __name__ == "__main__":
    run(sys.argv[1], sys.argv[2], sys.argv[3])