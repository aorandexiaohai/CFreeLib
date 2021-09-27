import requests
import os
import sys
import shutil

need_files_idx = 0
cache = set()
garbage = set()
need_files = set()
current_dir = "../"+os.getcwd().split("/")[-1].split("\\")[-1]

def fetch_need_files(file_path):
    global need_files
    if file_path in need_files:
        print(file_path +" has already been processed")
        return
    try:
        if not os.path.exists(file_path):
            return
        need_files.add(file_path)
        with open(file_path, "r") as f:
            for line in f.readlines():
                s = line.strip()
                if len(s) == 0:
                    continue
                single_file_path = "../" + s
                fetch_single_file(single_file_path)
    except Exception as err:
        print(err)

def fetch_single_file(single_file_path):
    global need_files_idx
    global garbage

    if single_file_path in cache:
        print(single_file_path + " has already been fetched")
        return

    try:
        splits = single_file_path.split('/')
        dir_name = splits[1]
        fetch_need_files("../"+dir_name+"/need_files.txt")

        file_name = splits[-1]
        flag = False
        if file_name == "need_files.txt":
            need_files_idx = need_files_idx + 1
            file_name = file_name + str(need_files_idx)
            flag = True

        dst_path = current_dir + "/"+ file_name
        shutil.copyfile(single_file_path, dst_path)
        
        if flag:
            garbage.add(dst_path)
            fetch_need_files(dst_path)
        cache.add(single_file_path)
    except Exception as err:
        print(err)

if __name__ =='__main__':
    if len(sys.argv) >= 2:
        base_url = sys.argv[1]
    try:
        fetch_need_files(current_dir + "/need_files.txt")
        for f in garbage:
            os.remove(f)
    except Exception as err:
        print(err)
