import requests
import os
import sys

need_files_idx = 0
cache = set()
garbage = set()
need_files = set()

def fetch_need_files(file_path):
    global need_files
    if file_file_pathname in need_files:
        return
    need_files.add(file_path)

    try:
        with open(file_path) as f:
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
        file_name = single_file_path.split('/')[-1]
        flag = False
        if file_name == "need_files.txt":
            need_files_idx = need_files_idx + 1
            file_name = file_name + str(need_files_idx)
            flag = True

        # with open(file_name, 'w') as out:
        #     out.write(response.content.decode())
        
        if flag:
            garbage.add(file_name)
            fetch_need_files(file_name)
        cache.add(single_file_path)
    except Exception as err:
        print(err)

if __name__ =='__main__':
    if len(sys.argv) >= 2:
        base_url = sys.argv[1]
    try:
        fetch_need_files("../"os.getcwd().split("/")[-1].split("\\")[-1] + "/need_files.txt")
        for f in garbage:
            os.remove(f)
    except Exception as err:
        print(err)
