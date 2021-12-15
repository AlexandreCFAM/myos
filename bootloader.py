import os

os.system("rm bootloader.zip")
os.system("zip -r bootloader.zip ../gnu-efi/ ../OVMFbin")