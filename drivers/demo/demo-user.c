#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_demo.h>

MODULE_LICENSE("GPL");

static int demo_user_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	int ret = 0;

	dev_info(dev, "%s enter.\n", __func__);

	if (!np) {
		dev_err(dev, "of_node is NULL\n");
		return -EINVAL;
	}

	ret = of_demo_request(np, "one");
	dev_info(dev, "one: %d\n", ret);

	ret = of_demo_request(np, "two");
	dev_info(dev, "two: %d\n", ret);

	ret = of_demo_request(np, "three");
	dev_info(dev, "two: %d\n", ret);

	return 0;
}

static int demo_user_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;

	dev_info(dev, "%s enter.\n", __func__);
	return 0;
}

static const struct of_device_id demo_user_dt_ids[] = {
	{
		.compatible = "demo_user,one",
	}, {
		.compatible = "demo_user,two",
	}, {
		/* sentinel */
	}
};
MODULE_DEVICE_TABLE(of, demo_user_dt_ids);

static struct platform_driver demo_user_driver = {
	.driver		= {
		.name	= "demo-user",
		.of_match_table = of_match_ptr(demo_user_dt_ids),
	},
	.probe		= demo_user_probe,
	.remove		= demo_user_remove,
};

module_platform_driver(demo_user_driver);
