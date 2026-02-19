from pathlib import Path
import sys, os, subprocess

def asset_pass(asset_dir):
    sys.path.append(os.path.dirname(__file__))

    import Setup
    Setup.setup()

    #print(asset_dir)
    working_dir = Path(__file__).parents[1] / "external_tools" / "shader_transpiler"

    types = ("*.frag", "*.vert", "*.vs", "*.fs", "*.psh", "*.fsh", "*.vsh", "*.glsl")

    files_to_convert = []
    path_to_search = Path(asset_dir)
    for filetype in types:
        files_to_convert.extend(path_to_search.rglob(filetype))

    for file_path in files_to_convert:
        #print(file_path)
        if not file_path.is_file():
            continue

        def run_command(command):
            try:
                subprocess.run(command, cwd=working_dir, shell=True,
                           check=True, capture_output=True, text=True, encoding='utf-8')
            except subprocess.CalledProcessError as e:
                print(f"SHADER COMPILATION FAILED! Error code: {e.returncode}", file=sys.stderr)
                print(f"STDOUT:{e.stdout}", file=sys.stderr)
                #print(f"STDERR:{e.stderr}", file=sys.stderr)
                raise e

        #print(file_path)
        run_command(f"glslangValidator.exe -H -V -o shader.spv {file_path}")
        run_command(f"spirv-cross.exe --hlsl --shader-model 50 shader.spv --output {file_path}")
        os.remove(working_dir / "shader.spv")

    sys.path.pop()