#/bin/sh

make
insmod debugfs.ko
chmod 777 /sys/kernel/debug
echo -n "alev" > /sys/kernel/debug/fortytwo/id
echo -n "anthony1" > /sys/kernel/debug/fortytwo/id
echo -n "alevasse" > /sys/kernel/debug/fortytwo/id
{
	dmesg | tail -n 5
	echo -n "ft_read: "
	cat /sys/kernel/debug/fortytwo/id
	echo ""
	head -c 4 /sys/kernel/debug/fortytwo/id
	dmesg | tail -n 1
	echo -n "Jiffies value: "
	cat /sys/kernel/debug/fortytwo/jiffies
	dmesg | tail -n 1
	echo -n "User root is writing hello in foo: "
} > proof.log
echo "hello" > /sys/kernel/debug/fortytwo/foo
{
	cat /sys/kernel/debug/fortytwo/foo
	dmesg | tail -n 3
	echo "Start test mutex:"
	echo "Test mutex" > /sys/kernel/debug/fortytwo/foo &
	sleep 0.5
} >> proof.log
su - alevasse -c "
       	date
       	cat /sys/kernel/debug/fortytwo/foo
	date
	dmesg | tail -n 1
" > user_proof.log
sleep 5
{
	cat /home/alevasse/user_proof.log
	echo "alevasse is trying to write something in foo..."
} >> proof.log
sudo -u alevasse bash -c "echo 'Hello'" >> \
	/sys/kernel/debug/fortytwo/foo 2>> proof.log
rmmod debugfs.ko
dmesg | tail -n 1 >> proof.log
make clean
