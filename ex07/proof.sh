#/bin/sh

make
insmod debugfs.ko
echo -n "alev" > /sys/kernel/debug/fortytwo/id
echo -n "anthony1" > /sys/kernel/debug/fortytwo/id
echo -n "alevasse" > /sys/kernel/debug/fortytwo/id
dmesg | tail -n 4 > proof.log
echo -n "ft_read: " >> proof.log
cat /sys/kernel/debug/fortytwo/id >> proof.log
echo "" >> proof.log
head -c 4 /sys/kernel/debug/fortytwo/id
rmmod debugfs.ko
dmesg | tail -n 4 >> proof.log
make clean

