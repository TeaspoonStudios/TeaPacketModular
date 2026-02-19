import os.path
import shutil
import tempfile
import importlib.util
from pathlib import Path


def get_need_files(src_dir : str, dest_dir : str, loaded_modules, rebuild_all : bool):
    need_files = []

    if rebuild_all:
        return [
            p for p in Path(src_dir).rglob("*")
            if not p.is_dir()]

    for file in Path(src_dir).rglob("*"):
        if file.is_dir():
            continue

        rel_src_path = file.relative_to(src_dir)
        rel_dest_path = Path(dest_dir) / rel_src_path
        if (
                not rel_dest_path.exists() or
                file.stat().st_mtime > rel_dest_path.stat().st_mtime
        ):
            need_files.append(file)

    for module in loaded_modules:
        function = getattr(module, "get_updated_files")

        if function and callable(function):
            function(src_dir, dest_dir)
        else:
            raise Exception(f"Error loading asset pass script: {module}")

    return need_files

def build_assets(src_dir : str, dest_dir, asset_pass_scripts=None, rebuild_all = False):
    if asset_pass_scripts is None:
        asset_pass_scripts = list()

    for i, path in enumerate(asset_pass_scripts):
        asset_pass_scripts[i] = os.path.abspath(path)

    loaded_modules = []

    for path in asset_pass_scripts:
        spec = importlib.util.spec_from_file_location(path, path)
        default_module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(default_module)
        loaded_modules.append(default_module)


    need_files : list[Path] = get_need_files(src_dir, dest_dir, loaded_modules, rebuild_all)
    #print(need_files)

    with tempfile.TemporaryDirectory() as temp_dir:

        # Move to temporary dir
        for file in need_files:
            rel_src_path = file.relative_to(src_dir)
            rel_temp_path = Path(temp_dir) / rel_src_path

            rel_temp_path.parent.mkdir(parents=True, exist_ok=True)
            file.copy(rel_temp_path)


        # Run all asset passes
        for module in loaded_modules:
            function = getattr(module, "asset_pass")

            if function and callable(function):
                function(temp_dir)
            else:
                raise Exception(f"Error loading asset pass script: {module}")


        # Move to final dir
        shutil.copytree(temp_dir, dest_dir, dirs_exist_ok=True)



