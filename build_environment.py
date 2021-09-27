import requests
import os
import sys

need_files_idx = 0
cache = set()
garbage = set()

necessary_url = ["FormatHelper/raw/master/.clang-format"]


def fetch_need_files(file_name):
    try:
        with open(file_name) as f:
            for line in f.readlines():
                s = line.strip()
                if len(s) == 0:
                    continue
                url = base_url + s
                fetch_url(url)
    except Exception as err:
        print(err)

def fetch_url(url):
    global need_files_idx
    global garbage

    if url in cache:
        print(url + " has already been fetched")
        return

    try:
        response = requests.get(url)
        file_name = url.split('/')[-1]
        flag = False
        if file_name == "need_files.txt":
            need_files_idx = need_files_idx + 1
            file_name = file_name + str(need_files_idx)
            flag = True

        with open(file_name, 'w') as out:
            out.write(response.content.decode())
        
        if flag:
            garbage.add(file_name)
            fetch_need_files(file_name)
        print("fetch " + url + " success")
        cache.add(url)
    except Exception as err:
        print(err)

if __name__ =='__main__':
    if len(sys.argv) >= 2:
        base_url = sys.argv[1]
    try:
        fetch_need_files("need_files.txt")
        for x in necessary_url:
            fetch_url(base_url + x)
        for f in garbage:
            os.remove(f)
    except Exception as err:
        print(err)
