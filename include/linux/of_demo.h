#ifndef __LINUX_OF_DEMO_H
#define __LINUX_OF_DEMO_H

#include <linux/of.h>

struct device_node;

struct of_demo {
	struct list_head	of_demo_controllers;
	struct device_node	*of_node;
	int			(*of_demo_xlate)
				(struct of_phandle_args *, struct of_demo *);
	void			*of_demo_data;
};

#ifdef CONFIG_DEMO_OF
extern int of_demo_controller_register(struct device_node *np,
		int (*of_demo_xlate) (struct of_phandle_args *, struct of_demo *),
		void *data);
extern void of_demo_controller_free(struct device_node *np);

extern int of_demo_request(struct device_node *np, const char *name);

extern int of_demo_simple_xlate(struct of_phandle_args *demo_spec,
		struct of_demo *ofdemo);

#else
static inline int of_demo_controller_register(struct device_node *np,
		int (*of_demo_xlate) (struct of_phandle_args *, struct of_demo *),
		void *data)
{
	return -ENODEV;
}

static inline int of_demo_request(struct device_node *np, const char *name)
{
	return 0;
}

static inline void of_demo_controller_free(struct device_node *np)
{
}

static inline int of_demo_simple_xlate(struct of_phandle_args *demo_spec,
		struct of_demo *ofdemo)
{
	return 0;
}

#endif

#endif
