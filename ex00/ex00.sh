git clone --depth=1 https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
cd linux/
git checkout master
git pull
make kernelversion
make defconfig
make menuconfig
make -j$(nproc)
cp arch/x86/boot/bzImage /boot/vmlinuz-latest
# Add /boot/vmlinuz-latest in the Menuentry
reboot
dmesg
grep 'CONFIG_LOCALVERSION_AUTO=y' /usr/src/linux/.config
