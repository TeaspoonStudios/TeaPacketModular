import os.path
import shutil
import tempfile
import importlib.util
import sys

def build_assets(src_dir : str, dest_dir, asset_pass_scripts=None, rebuild_all = False):
    if asset_pass_scripts is None:
        asset_pass_scripts = list()

    for i, path in enumerate(asset_pass_scripts):
        asset_pass_scripts[i] = os.path.abspath(path)

    with tempfile.TemporaryDirectory() as temp_dir:
        # Move to temporary dir
        shutil.copytree(src_dir, temp_dir, dirs_exist_ok=True)

        # Run all asset passes
        for path in asset_pass_scripts:
            #print(path)
            spec = importlib.util.spec_from_file_location(path, path)
            default_module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(default_module)

            function = getattr(default_module, "asset_pass")

            if function and callable(function):
                function(temp_dir)
            else:
                raise Exception(f"Error loading asset pass script: {path}")


        # Move to final dir
        shutil.copytree(temp_dir, dest_dir, dirs_exist_ok=True)



