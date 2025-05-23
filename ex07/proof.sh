#/bin/sh

make
insmod debugfs.ko
chmod 777 /sys/kernel/debug
echo -n "alev" > /sys/kernel/debug/fortytwo/id
echo -n "anthony1" > /sys/kernel/debug/fortytwo/id
echo -n "alevasse" > /sys/kernel/debug/fortytwo/id
{
	dmesg | tail -n 4
	echo -n "ft_read: "
	cat /sys/kernel/debug/fortytwo/id
	echo ""
} > proof.log
head -c 4 /sys/kernel/debug/fortytwo/id
{
	echo -n "Jiffies value: "
	cat /sys/kernel/debug/fortytwo/jiffies
} >> proof.log
echo "hello" > /sys/kernel/debug/fortytwo/foo
cat /sys/kernel/debug/fortytwo/foo
rmmod debugfs.ko
dmesg | tail -n 5 >> proof.log
make clean
