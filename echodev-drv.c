#include <linux/delay.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/pci_regs.h>

#define VID 0x1234
#define DID 0xbeef

static struct pci_device_id echo_ids[] = {

    {PCI_DEVICE(VID, DID)},
    {},

};

MODULE_DEVICE_TABLE(pci, echo_ids);

static int echo_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
  int status;
  void __iomem *ptr_bar0, __iomem *ptr_bar1;
  status = pcim_enable_device(pdev);
  if (status != 0) {

    printk("echodev-drv - Error enabling device\n ");
    return status;
  }

  ptr_bar0 = pcim_iomap(pdev, 0, pci_resource_len(pdev, 0));
  if (!ptr_bar0) {
    printk("echodev-drv - Error mapping bar0\n");
    return -ENODEV;
  }

  ptr_bar1 = pcim_iomap(pdev, 1, pci_resource_len(pdev, 1));
  if (!ptr_bar0) {
    printk("echodev-drv - Error mapping bar1\n");
    return -ENODEV;
  }
  printk("echodev-drv - ID: 0x%x\n", ioread32(ptr_bar0));
  printk("echodev-drv - Random value: 0x%x\n", ioread32(ptr_bar0 + 0xc));

  iowrite32(0x11223344, ptr_bar0 + 4);
  mdelay(1);
  printk("echodev-drv - Inverse Patern: 0x%x\n", ioread32(ptr_bar0 + 0x4));

  iowrite32(0x44332211, ptr_bar1);
  printk("echodev-drv - BAR1 Offset 0: 0x%x\n", ioread8(ptr_bar1));
  printk("echodev-drv - BAR1 Offset 0: 0x%x\n", ioread16(ptr_bar1));
  printk("echodev-drv - BAR1 Offset 0: 0x%x\n", ioread32(ptr_bar1));
  return 0;
}
static void echo_remove(struct pci_dev *pdev) {
  printk("Echo dev : Removing the devices\n");
}

static struct pci_driver echo_driver = {
    .name = "echodev-driver",
    .probe = echo_probe,
    .remove = echo_remove,
    .id_table = echo_ids,
};

module_pci_driver(echo_driver);

MODULE_LICENSE("GPL");
