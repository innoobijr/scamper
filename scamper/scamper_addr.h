/*
 * scamper_addr.h
 *
 * $Id: scamper_addr.h,v 1.38 2024/01/09 06:16:19 mjl Exp $
 *
 * Copyright (C) 2004-2006 Matthew Luckie
 * Copyright (C) 2006-2011 The University of Waikato
 * Copyright (C) 2013      The Regents of the University of California
 * Copyright (C) 2016-2023 Matthew Luckie
 * Author: Matthew Luckie
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __SCAMPER_ADDR_H
#define __SCAMPER_ADDR_H

/*
 * the scamper_addr code has a number of purposes:
 *
 * first, it provides a way to record different address types in the same
 * structure regardless of the operating system scamper is used on.
 *
 * second, it provides a way to reference count the addresses contained, so
 * that users of addresses do not have to allocate copies of the address
 * each time they use the address internally.
 *
 * third, it can provide a 'cache' of addresses so that only one copy of
 * the address needs to be kept among all users of scamper_addr.
 */

/*
 * these are the types of addresses that scamper understands
 */
#define SCAMPER_ADDR_TYPE_IPV4     0x01 /* 32 bit IPv4 address */
#define SCAMPER_ADDR_TYPE_IPV6     0x02 /* 128 bit IPv6 address */
#define SCAMPER_ADDR_TYPE_ETHERNET 0x03 /* 48 bit ethernet mac address */
#define SCAMPER_ADDR_TYPE_FIREWIRE 0x04 /* 64 bit firewire link address */

#define SCAMPER_ADDR_TYPE_UNSPEC   0x00 /* unspecified type */
#define SCAMPER_ADDR_TYPE_MAX      SCAMPER_ADDR_TYPE_FIREWIRE

/*
 * scamper_addr:
 * this structure is used to store an address in scamper.
 */
typedef struct scamper_addr scamper_addr_t;

int scamper_addr_type_get(const scamper_addr_t *addr);
const void *scamper_addr_addr_get(const scamper_addr_t *addr);

/*
 * scamper_addr_size
 *  return the size of the underlying address stored in the scamper_addr
 *  structure.  useful for writing address objects to disk...
 */
size_t scamper_addr_len_get(const scamper_addr_t *addr);

/*
 * scamper_addr_alloc:
 *  allocate an address of the specified type; the reference count of the
 *  address is initialised to one
 *
 * scamper_addr_use:
 *  increment the reference count by one, and return a reference to the
 *  address passed in as a convenience.
 *
 * scamper_addr_free:
 *  decrement the reference count held on the address.  when the reference
 *  count becomes zero, the address is freed.
 *
 * scamper_addr_fromstr:
 *  attempt to resolve the string as getaddrinfo would, and return the address
 *
 * scamper_addr_af:
 *  return the address family of the address
 *
 * scamper_addr_inprefix:
 *  return if the address is in the prefix.
 *
 * scamper_addr_prefix:
 *  return the longest number of matching bits for the two addresses.
 *
 * scamper_addr_prefixhosts:
 *  return the longest network prefix that would allow for these two hosts.
 *
 * scamper_addr_netaddr:
 *  return the network address for the supplied address given prefix length.
 */

#ifndef DMALLOC
scamper_addr_t *scamper_addr_alloc(const int type, const void *addr);
scamper_addr_t *scamper_addr_fromstr(int type, const char *str);
#else
scamper_addr_t *scamper_addr_alloc_dm(const int type, const void *addr,
				      const char *file, const int line);
scamper_addr_t *scamper_addr_fromstr_dm(int type, const char *str,
					const char *file, const int line);
#define scamper_addr_alloc(type, addr) \
  scamper_addr_alloc_dm((type), (addr), __FILE__, __LINE__)
#define scamper_addr_fromstr(type, addr) \
  scamper_addr_fromstr_dm((type), (addr), __FILE__, __LINE__)
#endif

scamper_addr_t *scamper_addr_use(scamper_addr_t *sa);
void scamper_addr_free(scamper_addr_t *sa);

int scamper_addr_af(const scamper_addr_t *sa);
int scamper_addr_inprefix(const scamper_addr_t *sa, const void *p, int len);
int scamper_addr_prefix(const scamper_addr_t *a, const scamper_addr_t *b);
int scamper_addr_prefixhosts(const scamper_addr_t *a, const scamper_addr_t *b);
int scamper_addr_netaddr(const scamper_addr_t *a, void *net, int netlen);
int scamper_addr_bit(const scamper_addr_t *a, int bit);
int scamper_addr_fbd(const scamper_addr_t *a, const scamper_addr_t *b);

/*
 * scamper_addr_alloc_[ipv4|ipv6|ethernet|firewire]
 *
 * these macros are provided as a convenience as the type constants can
 * become unwieldy to use
 */
#define scamper_addr_alloc_ipv4(addr) \
 scamper_addr_alloc(SCAMPER_ADDR_TYPE_IPV4, addr)

#define scamper_addr_alloc_ipv6(addr) \
 scamper_addr_alloc(SCAMPER_ADDR_TYPE_IPV6, addr)

#define scamper_addr_alloc_ethernet(addr) \
 scamper_addr_alloc(SCAMPER_ADDR_TYPE_ETHERNET, addr)

#define scamper_addr_alloc_firewire(addr) \
 scamper_addr_alloc(SCAMPER_ADDR_TYPE_FIREWIRE, addr)

/*
 * scamper_addr_fromstr_[unspec|ipv4|ipv6]
 *
 * these macros are provided as a convenience as the type constants can
 * become unwieldy to use
 */
#define scamper_addr_fromstr_unspec(addr) \
 scamper_addr_fromstr(SCAMPER_ADDR_TYPE_UNSPEC, addr)

#define scamper_addr_fromstr_ipv4(addr) \
 scamper_addr_fromstr(SCAMPER_ADDR_TYPE_IPV4, addr)

#define scamper_addr_fromstr_ipv6(addr) \
 scamper_addr_fromstr(SCAMPER_ADDR_TYPE_IPV6, addr)

/*
 * scamper_addr_cmp:
 *  given two addresses, return their sort order.
 */
int scamper_addr_cmp(const scamper_addr_t *a, const scamper_addr_t *b);
int scamper_addr_human_cmp(const scamper_addr_t *a, const scamper_addr_t *b);
int scamper_addr_raw_cmp(const scamper_addr_t *a, const void *raw);

/*
 * scamper_addr_tostr:
 *  given a scamper address, convert it to a string representation in the
 *  buffer of specified size.
 */
const char *scamper_addr_tostr(const scamper_addr_t *sa,
			       char *dst, const size_t size);

/*
 * scamper_addr_islinklocal:
 * determine if the address is a link-local IPv4 or IPv6 address.
 *
 * scamper_addr_isrfc1918:
 * determine if the address is an RFC 1918 address.
 */
int scamper_addr_islinklocal(const scamper_addr_t *a);
int scamper_addr_isrfc1918(const scamper_addr_t *a);
int scamper_addr_isunicast(const scamper_addr_t *a);
int scamper_addr_is6to4(const scamper_addr_t *a);
int scamper_addr_isreserved(const scamper_addr_t *a);
int scamper_addr_isipv4(const scamper_addr_t *a);
int scamper_addr_isipv6(const scamper_addr_t *a);
int scamper_addr_isethernet(const scamper_addr_t *a);
int scamper_addr_isfirewire(const scamper_addr_t *a);

#endif /* __SCAMPER_ADDR_H */
