import os
import yaml
import json
from pathlib import Path
from bsp_parser import BspParser

class BspChecker(object):
    """
    This is the project generator class, it contains the basic parameters and methods in all type of projects
    """

    def __init__(self,
                 bsp_path,
                 output_report_path):
        self.bsp_path = Path(bsp_path)
        self.output_report_path = output_report_path
        self.series_dict = None
        self.pack_dict = None
        self.current_location = None
        self.__dsc2dict()
        self.parser= BspParser(bsp_path)

    def __dsc2dict(self):
        desc_file_path = None
        self.pack_dict = None
        for file in os.listdir(self.bsp_path):
            if ".yaml" in file:
                desc_file_path = file
                with open(self.bsp_path.joinpath(desc_file_path), mode='r', encoding="utf-8") as f:
                    data = f.read()
                self.pack_dict = yaml.load(data, Loader=yaml.FullLoader)
                break
        if not desc_file_path:
            for file in os.listdir(self.bsp_path):
                if ".json" in file:
                    desc_file_path = file
                    with open(self.bsp_path.joinpath(desc_file_path), mode='r', encoding="utf-8") as f:
                        data = f.read()
                    self.pack_dict = json.loads(data)
                    break
        if self.pack_dict:
            pass
        else:
            return False
        return True

    @staticmethod
    def __fetch_obj_in_dict(dict_obj, index_list):
        for index in index_list:
            dict_obj = dict_obj[index]
        return dict_obj

    def built_in_files_check(self):

        for project in self.parser.all_projects:
            project_name = project["project_name"]
            self.current_location = "project_name: "+ project_name
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

    def is_str_empty(self, strs, err_msg):
        if strs == "":
            print("ERROR @ : " + str(self.current_location))
            print("".format(err_msg))


if __name__ == "__main__":
    bsp_path = "."
    bsp_parser = BspParser(bsp_path)
    output_report_path = "."
    bsp_checker = BspChecker(bsp_path,
                             output_report_path)
    bsp_checker.built_in_files_check()
