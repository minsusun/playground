# This script is for auto git gc-ing the _argocd-repo
# This script tries to limit the total size of the repo in the argocd repo server
# due to unpacked loos objects in git repository

mkdir -p /tmp/_argocd-repo
while :
do
    chmod +rx /tmp/_argocd-repo
    if [ $(ls /tmp/_argocd-repo -l | grep '^d' | awk '{print $9}') ]; then
    chmod +rx /tmp/_argocd-repo/*
    cd /tmp/_argocd-repo/$(ls /tmp/_argocd-repo -l | grep '^d' | awk '{print $9}');
    before=$(du /tmp/_argocd-repo -h --max-depth=1 | head -n 1 | awk '{print $1}');
    git reflog expire --all --expire=now;
    git gc --prune=now;
    after=$(du /tmp/_argocd-repo -h --max-depth=1 | head -n 1 | awk '{print $1}');
    if [ "$before" = "$after" ]; then
        echo $(date) "| git gc with no change";
    else
        echo $(date) "| git gc with change" $before "->" $after;
    fi
    else 
    echo "argocd repo directory not found";
    fi
    sleep 30s
done
