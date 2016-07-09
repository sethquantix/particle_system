#!/bin/zsh
GITLIB='https://github.com/sethquantix/libft.git'

mkdir -p ~/Documents/42projects/libft

cd ~/Documents/42projects/libft
if [[ -d '.git' ]]
then
git pull
else
git clone $GITLIB ../libft
fi
cd -

ln -s ~/Documents/42projects/libft
