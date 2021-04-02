# i forgot how to make makefile so i instead made an arch linux install script
all:
	timedatectl set-ntp true
	mkfs.ext4 -F /dev/sda1
	mount /dev/sda1 /mnt
	pacstrap /mnt base linux linux-firmware
	genfstab -U /mnt >> /mnt/etc/fstab
	arch-chroot /mnt
	ln -sf /usr/share/zoneinfo/Region/City /etc/localtime
	hwclock --systohc
	locale-gen
	echo "LANG=en_US.UTF-8" > /etc/locale.conf
	echo "archbarch" > /etc/hostname
	echo "127.0.0.1	localhost" >> /etc/hosts
	mkinitcpio -P
	bootctl install
	exit
	umount /mnt
	reboot
