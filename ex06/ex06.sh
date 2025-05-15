git clone --depth=1 https://git.kernel.org/pub/scm/linux/kernel/git/next/linux-next.git
cd linux-next/
make kernelversion
make defconfig
make menuconfig -> "General setup" -> "Local version" -> -next
make -j$(nproc)
cp arch/x86/boot/bzImage /boot/vmlinuz-next
# Add /boot/vmlinuz-latest in the Menuentry
reboot
dmesg
grep 'CONFIG_LOCALVERSION_AUTO=y' /usr/src/linux/.config
