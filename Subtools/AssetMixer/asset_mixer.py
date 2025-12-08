import importlib.util, os, sys

def get_dynamic_module(path : str):
    spec = importlib.util.spec_from_file_location(path, path)
    default_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(default_module)
    return default_module

def get_asset_mixer_path() -> str:
    abspath = os.path.abspath(__file__)
    return os.path.dirname(abspath)

def get_dest_dir(asset_platform : str, build_path : str) -> str:
    platform_script = f"{get_asset_mixer_path()}/Platform/{asset_platform}.py"
    if not os.path.exists(platform_script):
        raise Exception(f"Platform {asset_platform} is not a valid Asset Platform!")


    return get_dynamic_module(platform_script).set_dest_dir(build_path)

def get_asset_func(source_file : str, parser_dirs : set):
    parser_dirs.add(os.path.dirname(__file__) + "/Parsers/")

    extension = os.path.splitext(source_file)[1][1:]
    want_script_name = f"{extension}.py"

    for parser_dir in parser_dirs:
        check_script = f"{parser_dir}/{want_script_name}"
        if os.path.exists(check_script):
            module = get_dynamic_module(check_script)
            return module.parse_and_copy
    return get_dynamic_module(f"{get_asset_mixer_path()}/Parsers/default.py").parse_and_copy

def build_asset(source_file : str, dest_file : str, parser_dirs : set):
    dest_dir = os.path.dirname(dest_file)
    os.makedirs(dest_dir, exist_ok=True)

    if os.path.exists(dest_file):
        src_mtime = os.path.getmtime(source_file)
        dest_mtime = os.path.getmtime(dest_file)

        if src_mtime < dest_mtime:
            return

    asset_func = get_asset_func(source_file, parser_dirs)
    asset_func(source_file, dest_file)

def build_asset_files(asset_source, dest_folder, parser_dirs):
    for root, dirs, files in os.walk(asset_source):
        for filename in files:
            relative_path = f"{root}/{filename}".replace(asset_source, "")
            dest_path = f"{dest_folder}{relative_path}"
            build_asset(f"{root}/{filename}", dest_path, parser_dirs)

def build_all(source_folder, dest_folder, asset_platform, parser_dirs):
    dest_dir = get_dest_dir(asset_platform, dest_folder)
    build_asset_files(source_folder, dest_dir, parser_dirs)


if __name__ == "__main__":
    build_all(sys.argv[1], sys.argv[2], sys.argv[3], set(sys.argv[4:]))


"""
run("D:/Programming/TeaPacketModular/TestPrograms/GraphicsTest/assets",
    "D:/Programming/TeaPacketModular/TestPrograms/GraphicsTest/build/windows/Debug",
    "STDFile",
    {"D:/Programming/TeaPacketModular/Modules/Graphics/Direct3D11/AssetMixer/Parsers"})
"""