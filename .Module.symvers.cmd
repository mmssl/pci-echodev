cmd_/home/mmssl/Project/qemu/pci-echodev/Module.symvers :=  sed 's/ko$$/o/'  /home/mmssl/Project/qemu/pci-echodev/modules.order | scripts/mod/modpost       -o /home/mmssl/Project/qemu/pci-echodev/Module.symvers -n -e -i Module.symvers -T - 