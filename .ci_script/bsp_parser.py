import json
import logging
import os
from pathlib import Path

import yaml


class BspParser(object):
    """
    This is the project generator class, it contains the basic parameters and methods in all type of projects
    """

    def __init__(self,
                 bsp_path):
        self.bsp_path = Path(bsp_path)
        self.pack_dict = None
        self.selected_project_dict = None
        self.__dsc2dict()
        self.all_projects = []
        self.select_all_project()

    def select_all_project(self):

        self.all_projects = self.template_projects + self.example_projects

        return True

    def __dsc2dict(self):
        desc_file_path = None
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
            # bsp version <3 ,template_projects is named default_projects
            if "template_projects" in self.pack_dict.keys():
                self.template_projects = self.pack_dict["template_projects"]
            else:
                self.template_projects = self.pack_dict["default_projects"]
            self.example_projects = self.pack_dict["example_projects"]
        else:
            return False
        return True

    @staticmethod
    def get_builtin_file_from_project(project):
        internal_files_folders_list = []
        try:
            builtin_files_dict_list = project["builtin_files"]
            for builtin_files_dict in builtin_files_dict_list:
                source_path_offset = builtin_files_dict["source_path_offset"]
                for item in builtin_files_dict["files_and_folders"]:
                    internal_files_folders_list.append(Path(source_path_offset).joinpath(item))
            return internal_files_folders_list
        except Exception as e:
            logging.error("BSP parser error:{0}".format(e))
            return []
