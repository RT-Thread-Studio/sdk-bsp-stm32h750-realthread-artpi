from pathlib import Path

from bsp_parser import BspParser


class BspChecker(object):
    """
    This is the project generator class, it contains the basic parameters and methods in all type of projects
    """

    def __init__(self,
                 bsp_location):
        self.bsp_path = Path(bsp_location)
        self.series_dict = None
        self.pack_dict = None
        self.current_location = None
        self.parser = BspParser(bsp_location)

    def built_in_files_check(self):
        for project in self.parser.all_projects:
            project_name = project["project_name"]
            self.current_location = "project_name: " + project_name
            built_in_files_list = self.parser.get_builtin_file_from_project(project)
            if self.is_file_exist_in_bsp(built_in_files_list):
                pass
            else:
                return False
        print("done!")
        return True

    def is_file_exist_in_bsp(self, files):
        for file in files:
            if file != "none":
                file_path = self.bsp_path.joinpath(file)
                if not file_path.exists():
                    print("ERROR @ : " + str(self.current_location))
                    print("file '{0}' is not found in package".format(file))
                    return False
                else:
                    pass
        return True


if __name__ == "__main__":
    bsp_path = "."
    bsp_checker = BspChecker(bsp_path)
    bsp_checker.built_in_files_check()
