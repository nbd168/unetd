// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2022 Felix Fietkau <nbd@nbd.name>
 */
#ifndef __UNETD_UTILS_H
#define __UNETD_UTILS_H

#include <netinet/in.h>

union network_addr {
	struct {
		uint8_t network_id[8];
		uint8_t host_addr[8];
	};
	struct in_addr in;
	struct in6_addr in6;
};

union network_endpoint {
	struct sockaddr sa;
	struct sockaddr_in in;
	struct sockaddr_in6 in6;
};

static inline void *
network_endpoint_addr(union network_endpoint *ep, int *addr_len)
{
	if (ep->sa.sa_family == AF_INET6) {
		*addr_len = sizeof(ep->in6.sin6_addr);
		return &ep->in6.sin6_addr;
	}

	*addr_len = sizeof(ep->in.sin_addr);
	return &ep->in.sin_addr;
}

static inline bool
network_endpoint_addr_equal(union network_endpoint *ep1, union network_endpoint *ep2)
{
	const void *a1, *a2;
	int len;

	if (ep1->sa.sa_family != ep2->sa.sa_family)
		return false;

	a1 = network_endpoint_addr(ep1, &len);
	a2 = network_endpoint_addr(ep2, &len);

	return !memcmp(a1, a2, len);
}

int network_get_endpoint(union network_endpoint *dest, const char *str,
			 int default_port, int idx);
int network_get_subnet(int af, union network_addr *addr, int *mask,
		       const char *str);
int network_get_local_addr(void *local, const union network_endpoint *target);

#endif
