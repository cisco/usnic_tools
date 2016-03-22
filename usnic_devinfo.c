/*
 * Copyright (c) 2016, Cisco Systems, Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <ctype.h>
#include <rdma/fabric.h>
#include <rdma/fi_ext_usnic.h>
#include <rdma/fi_errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <stdlib.h>

static int devinfo(struct fi_usnic_info *finfo, int brief)
{
	int padding;
	struct fi_usnic_info_v2 *info = &finfo->ui.v2;
	struct fi_usnic_cap **cap;
	struct in_addr in;
	const char *desc;

	in.s_addr = info->ui_ipaddr_be;

	if (brief) {
		printf("%8s: ", info->ui_devname);
		printf("%5s ", info->ui_ifname);
		printf("%02x:%02x:%02x:%02x:%02x:%02x ",
				info->ui_mac_addr[0], info->ui_mac_addr[1],
				info->ui_mac_addr[2], info->ui_mac_addr[3],
				info->ui_mac_addr[4], info->ui_mac_addr[5]);
		if (info->ui_ipaddr_be != 0) {
			printf("%14s/%-2d ", inet_ntoa(in), info->ui_prefixlen);
		}  else {
			printf("<IP not set>    ");
		}

		if (info->ui_nicname != NULL && *info->ui_nicname != '\0')
			printf("%10s ", info->ui_nicname);
		else
			printf("%10s ", info->ui_pid);

		printf("%10s ", info->ui_firmware);
		printf("%4s ", (info->ui_link_up) ? "UP" : "DOWN");
		printf("\n");

		return 0;
	}

	printf("%s:\n", info->ui_devname);
	printf("\tInterface:               %s\n", info->ui_ifname);
	printf("\tMAC Address:             %02x:%02x:%02x:%02x:%02x:%02x\n",
			info->ui_mac_addr[0], info->ui_mac_addr[1],
			info->ui_mac_addr[2], info->ui_mac_addr[3],
			info->ui_mac_addr[4], info->ui_mac_addr[5]);

	printf("\tIP Address:              %s\n", (info->ui_ipaddr_be != 0) ?
			inet_ntoa(in) : "<IP not set>");

	in.s_addr = info->ui_netmask_be;
	printf("\tNetmask:                 %s\n", (info->ui_netmask_be != 0) ?
			inet_ntoa(in) : "<netmask not set>");
	printf("\tPrefix len:              %u\n", info->ui_prefixlen);

	printf("\tMTU:                     %u\n", info->ui_mtu);
	printf("\tLink State:              %s\n",
			(info->ui_link_up) ? "UP" : "DOWN");
	printf("\tBandwidth:               %u Gb/s\n",
			info->ui_link_speed / 1000);

	printf("\tDevice ID:               %s ", info->ui_pid);
	if (info->ui_nicname != NULL && *info->ui_nicname != '\0')
		printf("[%s] ", info->ui_nicname);

	printf("[0x%04x]\n", info->ui_device_id);

	printf("\tVendor ID:               %u\n", info->ui_vendor_id);
	printf("\tVendor Part ID:          %u\n", info->ui_vendor_part_id);
	printf("\tFirmware:                %s\n", info->ui_firmware);
	printf("\tVFs:                     %u\n", info->ui_num_vf);
	printf("\tCQ per VF:               %u\n", info->ui_cq_per_vf);
	printf("\tQP per VF:               %u\n", info->ui_qp_per_vf);
	printf("\tInterrupts per VF:       %u\n", info->ui_intr_per_vf);
	printf("\tMax CQ:                  %u\n", info->ui_max_cq);
	printf("\tMax CQ Entries:          %u\n", info->ui_max_cqe);
	printf("\tMax QP:                  %u\n", info->ui_max_qp);
	printf("\tMax Send Credits:        %u\n", info->ui_max_send_credits);
	printf("\tMax Recv Credits:        %u\n", info->ui_max_recv_credits);
	printf("\tCapabilities:\n");

	for (cap = info->ui_caps; *cap; cap++) {
		if (strcmp((*cap)->uc_capability, "USD_CAP_CQ_SHARING")
				== 0) {
			// This capability was never released to customers
			continue;
		} else if (strcmp((*cap)->uc_capability, "USD_CAP_MAP_PER_RES")
				== 0) {
			desc = "Map per res";
		} else if (strcmp((*cap)->uc_capability, "USD_CAP_PIO")
				== 0) {
			desc = "PIO sends";
		} else if (strcmp((*cap)->uc_capability, "USD_CAP_CQ_INTR")
				== 0) {
			// This capability was never released to customers
			continue;
		} else if (strcmp((*cap)->uc_capability, "USD_CAP_GRP_INTR")
				== 0) {
			desc = "CQ interrupts";
		} else {
			desc = (*cap)->uc_capability;
		}

		// 25 is the indenting level of all the other printf's
		padding = 25 - 3 - (int) strlen(desc);
		padding = (padding < 0) ? 0 : padding;
		printf("\t  %s:%*s%s\n", desc, padding, "",
			(*cap)->uc_present ? "yes" : "no");
	}

	return 0;
}

void show_help(const char *argv0)
{
    printf("Usage: %s [-b] [-d device]\n", argv0);
    printf("\n");
    printf("-b        - Brief output\n");
    printf("-d device - Specify the local device (e.g., -d 0 or -d usnic_0)\n");
}

int main(int argc, char *argv[]) {
	struct fi_info *hints;
	struct fi_info *info;
	struct fi_info *info_list;
	struct fi_usnic_info usnic_info;
	struct fi_usnic_ops_fabric *usnic_fabric_ops;
	struct fid_fabric *fabric;
	struct fi_usnic_cap **cap;
	char c;
	char name[32];
	int brief = 0;
	int devno = -1;
	int ret;

	while ((c = getopt(argc, argv, "bd:")) != EOF) {
		switch (c) {
		case 'b':
			brief = 1;
			break;
		case 'd':
			if (isdigit(optarg[0])) {
				devno = atoi(optarg);
			} else if (strncmp(optarg, "usnic_", 6) == 0) {
				devno = atoi(optarg + 6);
			} else {
				fprintf(stderr, "%s: bad device\n", optarg);
			}
			break;
		default:
			show_help(argv[0]);
			exit(1);
		}
	}

	hints = fi_allocinfo();

	hints->fabric_attr->prov_name = strdup("usnic");
	hints->ep_attr->type = FI_EP_DGRAM;

	hints->caps = FI_MSG;
	hints->mode = FI_LOCAL_MR;
	hints->addr_format = FI_SOCKADDR;

	if (devno != -1) {
		sprintf(name, "usnic_%d", devno);
		hints->fabric_attr->name = strdup(name);
	}

	ret = fi_getinfo(FI_VERSION(1, 2), NULL, 0, 0, hints, &info_list);
	if (ret) {
		fprintf(stderr, "getinfo: %s\n", fi_strerror(-ret));
		return -ret;
	}

	for (info = info_list; info; info = info->next) {
		ret = fi_fabric(info->fabric_attr, &fabric, NULL);
		if (ret) {
			fprintf(stderr, "fabric: %s\n", fi_strerror(-ret));
			return -ret;
		}

		ret = fi_open_ops(&fabric->fid, FI_USNIC_FABRIC_OPS_1, 0,
				(void **) &usnic_fabric_ops, NULL);
		if (ret) {
			fprintf(stderr, "fi_open_ops: %s\n", fi_strerror(-ret));
			return -ret;
		}

		ret = usnic_fabric_ops->getinfo(FI_EXT_USNIC_INFO_VERSION,
				fabric, &usnic_info);
		if (ret) {
			fprintf(stderr, "usnic_getinfo: %s\n", fi_strerror(-ret));
			return -ret;
		}

		ret = devinfo(&usnic_info, brief);
		if (ret)
			return ret;

		for (cap = usnic_info.ui.v2.ui_caps; *cap; cap++) {
			free(*cap);
		}

		free(usnic_info.ui.v2.ui_caps);

		ret = fi_close(&fabric->fid);
		if (ret) {
			fprintf(stderr, "fi_close: %s\n", fi_strerror(-ret));
			return -ret;
		}
	}

	fi_freeinfo(info_list);
	fi_freeinfo(hints);

	return 0;
}
