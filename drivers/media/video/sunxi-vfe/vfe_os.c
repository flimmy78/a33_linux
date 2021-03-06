#include <linux/module.h>
#include "vfe_os.h"

unsigned int vfe_dbg_en = 0;
unsigned int vfe_dbg_lv = 1;
EXPORT_SYMBOL_GPL(vfe_dbg_en);
EXPORT_SYMBOL_GPL(vfe_dbg_lv);

struct clk *os_clk_get(struct device *dev, const char *id)  
{
#ifdef VFE_CLK
	struct clk *clk_p;
	clk_p =  clk_get(dev, id);
	if(clk_p == NULL)
		vfe_warn("Get clk %s is NULL!\n",id);
  	return clk_p;
#else
	return NULL;
#endif
}
EXPORT_SYMBOL_GPL(os_clk_get);

void  os_clk_put(struct clk *clk)
{
#ifdef VFE_CLK
	clk_put(clk);
#endif
}
EXPORT_SYMBOL_GPL(os_clk_put);

int os_clk_set_parent(struct clk *clk, struct clk *parent) 
{
#ifdef VFE_CLK
	return clk_set_parent(clk, parent);
#else
	return 0;
#endif
}
EXPORT_SYMBOL_GPL(os_clk_set_parent);

int os_clk_set_rate(struct clk *clk, unsigned long rate) 
{
#ifdef VFE_CLK
	int ret;
	ret = clk_set_rate(clk, rate);
	if(ret < 0)
	{
		vfe_warn("Set clk rate %ld is ERR!\n",rate);
	}
	return ret;
#else
	return 0;
#endif
}
EXPORT_SYMBOL_GPL(os_clk_set_rate);

int os_clk_enable(struct clk *clk) 
{
#ifdef VFE_CLK
	return clk_enable(clk);
#else
	return 0;
#endif
}
EXPORT_SYMBOL_GPL(os_clk_enable);

int os_clk_prepare_enable(struct clk *clk)
{
#ifdef VFE_CLK
	return clk_prepare_enable(clk);
#else
	return 0;
#endif
}
EXPORT_SYMBOL_GPL(os_clk_prepare_enable);

void os_clk_disable(struct clk *clk) 
{
#ifdef VFE_CLK
	clk_disable(clk);
#endif
}
EXPORT_SYMBOL_GPL(os_clk_disable);

void os_clk_disable_unprepare(struct clk *clk)
{
#ifdef VFE_CLK
	clk_disable_unprepare(clk);
#endif
}
EXPORT_SYMBOL_GPL(os_clk_disable_unprepare);

int os_clk_reset_assert(struct clk *clk) 
{
#ifdef VFE_CLK
	return sunxi_periph_reset_assert(clk);
#else
	return 0;
#endif
}
EXPORT_SYMBOL_GPL(os_clk_reset_assert);

int os_clk_reset_deassert(struct clk *clk) 
{
#ifdef VFE_CLK
	return sunxi_periph_reset_deassert(clk);
#else
	return 0;
#endif
}
EXPORT_SYMBOL_GPL(os_clk_reset_deassert);

int os_request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
      const char *name, void *dev)
{
	return request_irq(irq,handler,flags,name,dev);
}
EXPORT_SYMBOL_GPL(os_request_irq);

__hdle os_gpio_request(struct vfe_gpio_cfg *gpio_list, __u32 group_count_max)
{    

#ifdef VFE_GPIO
	__hdle ret = 0;
  
	struct gpio_config pin_cfg;
	char   pin_name[32];
	__u32 config;

	if(gpio_list == NULL)
		return 0;
    
	if(gpio_list->gpio == GPIO_INDEX_INVALID)
		return 0;
		
	pin_cfg.gpio = gpio_list->gpio;
	pin_cfg.mul_sel = gpio_list->mul_sel;
	pin_cfg.pull = gpio_list->pull;
	pin_cfg.drv_level = gpio_list->drv_level;
	pin_cfg.data = gpio_list->data;
	ret = gpio_request(pin_cfg.gpio, NULL);
	if(0 != ret)
	{
		vfe_warn("os_gpio_request failed, gpio_name=%s, gpio=%d, ret=0x%x, %d\n", gpio_list->gpio_name, gpio_list->gpio, ret, ret);
		return 0;
	}

	if (!IS_AXP_PIN(pin_cfg.gpio)) {
		/* valid pin of sunxi-pinctrl, 
		 * config pin attributes individually.
		 */
		sunxi_gpio_to_name(pin_cfg.gpio, pin_name);
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, pin_cfg.mul_sel);
		pin_config_set(SUNXI_PINCTRL, pin_name, config);
		if (pin_cfg.pull != GPIO_PULL_DEFAULT) {
			config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_PUD, pin_cfg.pull);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
		if (pin_cfg.drv_level != GPIO_DRVLVL_DEFAULT) {
			config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DRV, pin_cfg.drv_level);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
		if (pin_cfg.data != GPIO_DATA_DEFAULT) {
			config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, pin_cfg.data);
			pin_config_set(SUNXI_PINCTRL, pin_name, config);
		}
	} else if (IS_AXP_PIN(pin_cfg.gpio)) {
		/* valid pin of axp-pinctrl, 
		 * config pin attributes individually.
		 */
		sunxi_gpio_to_name(pin_cfg.gpio, pin_name);
		config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_FUNC, pin_cfg.mul_sel);
		pin_config_set(AXP_PINCTRL, pin_name, config);
		if (pin_cfg.data != GPIO_DATA_DEFAULT) {
			config = SUNXI_PINCFG_PACK(SUNXI_PINCFG_TYPE_DAT, pin_cfg.data);
			pin_config_set(AXP_PINCTRL, pin_name, config);
		}
	} else {
		vfe_warn("invalid pin [%d] from sys-config\n", pin_cfg.gpio);
		return 0;
	}
	return pin_cfg.gpio;
#else
	return 0;
#endif
}
EXPORT_SYMBOL_GPL(os_gpio_request);

int os_gpio_release(__hdle p_handler, __s32 if_release_to_default_status)
{
#ifdef VFE_GPIO
	if(p_handler)
	{
		gpio_free(p_handler);
	}
	else
	{
		vfe_warn("os_gpio_release, hdl is NULL\n");
	}
#endif
	return 0;
}
EXPORT_SYMBOL_GPL(os_gpio_release);

int os_gpio_write(u32 p_handler, __u32 value_to_gpio, const char *gpio_name, int force_value_flag)
{
#ifdef VFE_GPIO
	if(1 == force_value_flag)
	{
		if(p_handler)
		{
			__gpio_set_value(p_handler, value_to_gpio);
		}
		else
		{
			vfe_dbg(0,"os_gpio_write, hdl is NULL\n");
		}

	}
	else
	{
		if(p_handler)
		{
			if(value_to_gpio == 0)
			{
				os_gpio_set_status(p_handler, 1, gpio_name);
				__gpio_set_value(p_handler, value_to_gpio);
			} 
			else
			{
				os_gpio_set_status(p_handler, 0, gpio_name);
			}
		}
		else
		{
			vfe_dbg(0,"os_gpio_write, hdl is NULL\n");
		}

	}
#endif
	return 0;
}
EXPORT_SYMBOL_GPL(os_gpio_write);


int os_gpio_set_status(u32 p_handler, __u32 if_set_to_output_status, const char *gpio_name)
{
	int ret = 0;
#ifdef VFE_GPIO
	if(p_handler)
	{
		if(if_set_to_output_status)
		{
			ret = gpio_direction_output(p_handler, 0);
			if(ret != 0)
			{
				vfe_warn("gpio_direction_output fail!\n");
			}
		}
		else
		{
			ret = gpio_direction_input(p_handler);
			if(ret != 0)
			{
				vfe_warn("gpio_direction_input fail!\n");
			}
		}
	}
	else
	{
		vfe_warn("os_gpio_set_status, hdl is NULL\n");
		ret = -1;
	}
#endif
	return ret;
}
EXPORT_SYMBOL_GPL(os_gpio_set_status);

int os_mem_alloc(struct vfe_mm *mem_man, int force_addr_handle)
{
#ifdef SUNXI_MEM
	unsigned int addr_handle;  
	if(force_addr_handle == VFE_NOT_ADDR)
	{
		addr_handle = sunxi_mem_alloc(mem_man->size);
	}
	else
	{
		addr_handle = force_addr_handle;
	}
	if(addr_handle) {
		mem_man->phy_addr = (void *)addr_handle;
		mem_man->dma_addr = mem_man->phy_addr + HW_DMA_OFFSET- CPU_DRAM_PADDR_ORG;
		//mem_man->vir_addr = (void *)ioremap(addr_handle, mem_man->size);
		mem_man->vir_addr = (void *)sunxi_map_kernel(addr_handle, mem_man->size);
		if(!mem_man->vir_addr) {
			vfe_err("os_mem_alloc map error, phy addr = %x size = %d\n",addr_handle,mem_man->size);
			return -ENOMEM;
		}
		return 0;
	} else {
		vfe_err("os_mem_alloc sunxi_mem_alloc error, size = %d\n",mem_man->size);
		return -ENOMEM;
	}
#else
	mem_man->vir_addr = dma_alloc_coherent(NULL, (size_t)mem_man->size,
			(dma_addr_t*)&mem_man->phy_addr, GFP_KERNEL);

	if (!mem_man->vir_addr) {
		vfe_err("memory alloc size %d failed\n", mem_man->size);
		return -ENOMEM;
	} 
	
	mem_man->dma_addr = mem_man->phy_addr + HW_DMA_OFFSET- CPU_DRAM_PADDR_ORG;
	return 0;
#endif
}
EXPORT_SYMBOL_GPL(os_mem_alloc);

void os_mem_free(struct vfe_mm *mem_man)
{
#ifdef SUNXI_MEM
	if(mem_man->vir_addr)
		//iounmap((void* __iomem)(mem_man->vir_addr));
		sunxi_unmap_kernel(mem_man->vir_addr);
	if(mem_man->phy_addr)
		sunxi_mem_free((unsigned int)mem_man->phy_addr, mem_man->size);	
#else
	if(mem_man->vir_addr)
		dma_free_coherent(NULL, mem_man->size, mem_man->vir_addr, (dma_addr_t)mem_man->phy_addr);
#endif
	mem_man->phy_addr = NULL;
	mem_man->dma_addr = NULL;
	mem_man->vir_addr = NULL;
}
EXPORT_SYMBOL_GPL(os_mem_free);

MODULE_AUTHOR("raymonxiu");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Video front end OSAL for sunxi");
