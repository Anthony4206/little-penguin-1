# In the Makefile's folder
echo 'alias="dmesg -T --color=always"' >> $HOME/.bashrc
make
insmod main.ko
dmesg | tail -1
rmmod main.ko
dmesg | tail -1
