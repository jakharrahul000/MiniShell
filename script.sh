#!/usr/bin/zsh

gcc capability.c error_functions.c parse.c execute.c builtin.c helper.c main.c -lcrypt -lcap -lreadline -o main -L./lib/ -lbuiltin
sudo setcap "cap_dac_override,cap_setuid+p" main
sudo chown root main
sudo chmod u+s main
