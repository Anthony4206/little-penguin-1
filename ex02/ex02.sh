cd /usr/src/linux
git branch alevasse
git checkout alevasse
vim Makefile
# Add to EXTRAVERSION = > -thor_kernel
git add Makefile
git commit -m "Add -thor_kernel to EXTRAVERSION"
git format-patch -1 HEAD
make -j$(nproc)
make modules_install
cp arch/x86/boot/bzImage /boot/vmlinuz-6.15.0-thor_kernel
cp System.map /boot/System.map-6.15.0-thor_kernel
cp .config /boot/config-6.15.0-thor_kernel
# Modify the grub.cfg
reboot
uname -r
dmesg > boot.log
