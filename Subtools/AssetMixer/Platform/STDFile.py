def set_dest_dir(project_build_dir : str):
    project_build_dir = project_build_dir.replace("\\", "/")
    if project_build_dir[-1] == '/':
        project_build_dir = project_build_dir[:-1]
    return project_build_dir + "/assets/"
