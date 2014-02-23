/*
 * Copyright 2007-2010	Luis R. Rodriguez <mcgrof@winlab.rutgers.edu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Backport functionality introduced in Linux 2.6.26.
 *
 * Copyright holders from ported work:
 *
 * Copyright (c) 2002-2003 Patrick Mochel <mochel@osdl.org>
 * Copyright (c) 2006-2007 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (c) 2006-2007 Novell Inc.
 */
#include <linux/device.h>
#include <net/sock.h>
#include <net/compat.h>

/**
 * kobject_set_name_vargs - Set the name of an kobject
 * @kobj: struct kobject to set the name of
 * @fmt: format string used to build the name
 * @vargs: vargs to format the string.
 */
static
int kobject_set_name_vargs(struct kobject *kobj, const char *fmt,
				  va_list vargs)
{
	const char *old_name = kobj->name;
	char *s;

	if (kobj->name && !fmt)
		return 0;

	kobj->name = kvasprintf(GFP_KERNEL, fmt, vargs);
	if (!kobj->name)
		return -ENOMEM;

	/* ewww... some of these buggers have '/' in the name ... */
	while ((s = strchr(kobj->name, '/')))
		s[0] = '!';

	kfree(old_name);
	return 0;
}

/**
 * dev_set_name - set a device name
 * @dev: device
 * @fmt: format string for the device's name
 */
int dev_set_name(struct device *dev, const char *fmt, ...)
{
	va_list vargs;
	int err;

	va_start(vargs, fmt);
	err = kobject_set_name_vargs(&dev->kobj, fmt, vargs);
	va_end(vargs);
	return err;
}
EXPORT_SYMBOL_GPL(dev_set_name);

