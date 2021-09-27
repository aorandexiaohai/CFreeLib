cd ..
arr=(FormatHelper CThreadPool CSingleList CSemaphore CDoubleList CBase)
for path in ${arr[*]}
do
    if [ -d "$path" ]; then
        cd ./$path
        cp ./.git/config ./.git/config_bak

        #use github
        if [ -f ./.git/config_github ]; then
            cp ./.git/config_github ./.git/config
            git pull --all
            git push
            echo "sync $path with github"
            echo
        fi

        #use gitee
        if [ -f ./.git/config_gitee ]; then
            cp ./.git/config_gitee ./.git/config
            git pull --all
            git push
            echo "sync $path with gitee"
            echo
        fi

        cp ./.git/config_bak ./.git/config
        cd ..
    fi
done;