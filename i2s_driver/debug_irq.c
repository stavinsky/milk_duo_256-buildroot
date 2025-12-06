#include <linux/interrupt.h>
#include <linux/module.h>

static int irq = 12; /* Linux IRQ number from /proc/interrupts */
module_param(irq, int, 0444);

static irqreturn_t debug_irq_handler(int irq, void* dev_id) {
    pr_info("DEBUG IRQ: got interrupt %d\n", irq);
    return IRQ_HANDLED;
}

static int __init debug_irq_init(void) {
    int ret;

    pr_info("DEBUG IRQ: requesting irq %d\n", irq);

    ret = request_irq(irq, debug_irq_handler,
                      IRQF_SHARED, "debug-irq", &debug_irq_handler);
    if (ret) {
        pr_err("DEBUG IRQ: request_irq(%d) failed: %d\n", irq, ret);
        return ret;
    }

    return 0;
}

static void __exit debug_irq_exit(void) {
    free_irq(irq, &debug_irq_handler);
    pr_info("DEBUG IRQ: freed irq %d\n", irq);
}

module_init(debug_irq_init);
module_exit(debug_irq_exit);

MODULE_LICENSE("GPL");