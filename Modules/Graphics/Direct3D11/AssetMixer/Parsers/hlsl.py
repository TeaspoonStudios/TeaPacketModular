import os, subprocess, pathlib


def parse_and_copy(source_file, dest_file):
    work_dir = str(pathlib.Path(__file__).parents[1]) + "/external_tools/shader_transpiler/"

    subprocess.run(f"glslangValidator.exe -H -V -o shader.spv {source_file}", cwd=work_dir, shell=True, check=True)
    subprocess.run(f"spirv-cross.exe --hlsl --shader-model 50 shader.spv --output {dest_file}", cwd=work_dir, shell=True, check=True)
    os.remove(work_dir + "/shader.spv")