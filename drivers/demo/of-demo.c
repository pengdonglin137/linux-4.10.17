/*
 * Device tree helpers for demo request / controller
 *
 * Based on of_dma.c
 *
 */

#include <linux/device.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_demo.h>

static LIST_HEAD(of_demo_list);
static DEFINE_MUTEX(of_demo_lock);

/**
 * of_demo_find_controller - Get a demo controller in DT demo helpers list
 * @demo_spec:	pointer to demo specifier as found in the device tree
 *
 * Finds a demo controller with matching device node and number for demo cells
 * in a list of registered demo controllers. If a match is found a valid pointer
 * to the demo data stored is retuned. A NULL pointer is returned if no match is
 * found.
 */
static struct of_demo *of_demo_find_controller(struct of_phandle_args *demo_spec)
{
	struct of_demo *ofdemo;

	list_for_each_entry(ofdemo, &of_demo_list, of_demo_controllers)
		if (ofdemo->of_node == demo_spec->np)
			return ofdemo;

	pr_debug("%s: can't find demo controller %s\n", __func__,
		 demo_spec->np->full_name);

	return NULL;
}

/**
 * of_demo_controller_register - Register a demo controller to DT demo helpers
 * @np:			device node of demo controller
 * @of_demo_xlate:	translation function which converts a phandle
 *			arguments list into a demo_chan structure
 * @data		pointer to controller specific data to be used by
 *			translation function
 *
 * Returns 0 on success or appropriate errno value on error.
 *
 * Allocated memory should be freed with appropriate of_demo_controller_free()
 * call.
 */
int of_demo_controller_register(struct device_node *np,
				int (*of_demo_xlate)
				(struct of_phandle_args *, struct of_demo *),
				void *data)
{
	struct of_demo	*ofdemo;

	if (!np || !of_demo_xlate) {
		pr_err("%s: not enough information provided\n", __func__);
		return -EINVAL;
	}

	if (!of_get_property(np, "demo-controller", NULL)) {
		pr_err("%s: can not register demo controller for %s.\n",
			__func__, np->name);
		return -EINVAL;
	}

	ofdemo = kzalloc(sizeof(*ofdemo), GFP_KERNEL);
	if (!ofdemo)
		return -ENOMEM;

	ofdemo->of_node = np;
	ofdemo->of_demo_xlate = of_demo_xlate;
	ofdemo->of_demo_data = data;

	/* Now queue of_demo controller structure in list */
	mutex_lock(&of_demo_lock);
	list_add_tail(&ofdemo->of_demo_controllers, &of_demo_list);
	mutex_unlock(&of_demo_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(of_demo_controller_register);

/**
 * of_demo_controller_free - Remove a demo controller from DT demo helpers list
 * @np:		device node of demo controller
 *
 * Memory allocated by of_demo_controller_register() is freed here.
 */
void of_demo_controller_free(struct device_node *np)
{
	struct of_demo *ofdemo;

	mutex_lock(&of_demo_lock);

	list_for_each_entry(ofdemo, &of_demo_list, of_demo_controllers)
		if (ofdemo->of_node == np) {
			list_del(&ofdemo->of_demo_controllers);
			kfree(ofdemo);
			break;
		}

	mutex_unlock(&of_demo_lock);
}
EXPORT_SYMBOL_GPL(of_demo_controller_free);

/**
 * of_demo_request - Get the demo
 * @np:		device node to get demo request from
 * @name:	name of desired channel
 */
int of_demo_request(struct device_node *np, const char *name)
{
	struct of_phandle_args	demo_spec;
	struct of_demo		*ofdemo;
	int			result;
	int index;

	if (!np || !name) {
		pr_err("%s: not enough information provided\n", __func__);
		return -EINVAL;
	}

	index = of_property_match_string(np, "demo-names", name);
	if (index < 0) {
		pr_err("%s: not match %s in demo-names\n", __func__, name);
		return -ENODEV;
	}

	if (of_parse_phandle_with_args(np, "demos", "#demo-cells", index,
				       &demo_spec)) {
		pr_err("%s: parse %s failed\n", __func__, name);
		return -ENODEV;
	}

	mutex_lock(&of_demo_lock);
	ofdemo = of_demo_find_controller(&demo_spec);

	if (ofdemo) {
		result = ofdemo->of_demo_xlate(&demo_spec, ofdemo);
	} else {
		pr_warn("%s: can not find demo controller for %s\n",
				__func__, name);
		result = -1;
	}

	mutex_unlock(&of_demo_lock);

	of_node_put(demo_spec.np);

	return result;
}
EXPORT_SYMBOL_GPL(of_demo_request);

int of_demo_simple_xlate(struct of_phandle_args *demo_spec,
						struct of_demo *ofdemo)
{
	int count = demo_spec->args_count;
	struct device *dev = ofdemo->of_demo_data;

	if (count != 1)
		return -EINVAL;

	dev_info(dev, "%s enter, count: %d\n", __func__, count);

	return demo_spec->args[0];
}
EXPORT_SYMBOL_GPL(of_demo_simple_xlate);

int of_demo_xlate_two_cells(struct of_phandle_args *demo_spec,
						struct of_demo *ofdemo)
{
	int count = demo_spec->args_count;
	struct device *dev = ofdemo->of_demo_data;
	int result;

	if (count != 2)
		return -EINVAL;

	dev_info(dev, "%s enter, count: %d\n", __func__, count);

	result = demo_spec->args[0] * demo_spec->args[1];

	return result;
}
EXPORT_SYMBOL_GPL(of_demo_xlate_two_cells);

int of_demo_xlate_three_cells(struct of_phandle_args *demo_spec,
						struct of_demo *ofdemo)
{
	int count = demo_spec->args_count;
	struct device *dev = ofdemo->of_demo_data;
	int result;

	if (count != 3)
		return -EINVAL;

	dev_info(dev, "%s enter, count: %d\n", __func__, count);

	result = demo_spec->args[0] + demo_spec->args[1] + demo_spec->args[2];

	return result;
}
EXPORT_SYMBOL_GPL(of_demo_xlate_three_cells);
