/*
 *                
 * Filename:      flexmem.c
 * Version:       0.5
 * Description:   Transfer from/to Siemens Mobile Equipment via OBEX
 * Status:        Experimental.
 * Author:        Christian W. Zuckschwerdt <zany@triq.net>
 * Created at:    Don, 17 Jan 2002 18:27:25 +0100
 * Modified at:   Don,  7 Feb 2002 12:24:55 +0100
 * Modified by:   Christian W. Zuckschwerdt <zany@triq.net>
 *
 *   Copyright (c) 2002 Christian W. Zuckschwerdt <zany@triq.net>
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *   for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *     
 */

#include <glib.h>
#include <string.h>
#define _GNU_SOURCE
#include <getopt.h>

#include "debug.h"
#include "ircp.h"
#include "ircp_client.h"
#include "ircp_server.h"

#include "cobex_bfb.h"


//
//
//
void ircp_info_cb(gint event, gchar *param)
{
	DEBUG(4, G_GNUC_FUNCTION "()\n");
	switch (event) {

	case IRCP_EV_ERRMSG:
		g_print("Error: %s\n", param);
		break;

	case IRCP_EV_ERR:
		g_print("failed: %s\n", param);
		break;
	case IRCP_EV_OK:
		g_print("done\n");
		break;


	case IRCP_EV_CONNECTING:
		g_print("Connecting...");
		break;
	case IRCP_EV_DISCONNECTING:
		g_print("Disconnecting...");
		break;
	case IRCP_EV_SENDING:
		g_print("Sending %s...", param);
		break;
	case IRCP_EV_RECEIVING:
		g_print("Receiving %s...", param);
		break;

	case IRCP_EV_LISTENING:
		g_print("Waiting for incoming connection\n");
		break;

	case IRCP_EV_CONNECTIND:
		g_print("Incoming connection\n");
		break;
	case IRCP_EV_DISCONNECTIND:
		g_print("Disconnecting\n");
		break;



	}
}

//
//
//
int main(int argc, char *argv[])
{
	int c;

	ircp_client_t *cli;
	ircp_server_t *srv;
	gchar *inbox;
	gchar *device = NULL;
        obex_ctrans_t *ctrans = NULL;

	while (1) {
		int option_index = 0;
		static struct option long_options[] = {
			{"device", 1, 0, 'd'},
			{"cable", 0, 0, 'c'},
			{"list", 2, 0, 'l'},
			{"get", 1, 0, 'g'},
			{"put", 1, 0, 'p'},
			{"info", 0, 0, 'i'},
			{"move", 1, 0, 'm'},
			{"delete", 1, 0, 'k'},
			{"receive", 0, 0, 'r'},
			{"help", 0, 0, 'h'},
			{"usage", 0, 0, 'u'},
			{0, 0, 0, 0}
		};
		
		c = getopt_long (argc, argv, "d:cl::g:p:im:k:rh",
				 long_options, &option_index);
		if (c == -1)
			break;
		
		switch (c) {
			
		case 'd':
			g_print ("device `%s'\n", optarg);
			device = optarg;
			break;

		case 'c':
			ctrans = cobex_ctrans();
			break;

		case 'l':
			cli = ircp_cli_open(ircp_info_cb, ctrans);
			if(cli == NULL) {
				g_print("Error opening ircp-client\n");
				return -1;
			}
			
			// Connect
			if(ircp_cli_connect(cli) >= 0) {
				ircp_list(cli, optarg, optarg);

				// Disconnect
				ircp_cli_disconnect(cli);
			}
			ircp_cli_close(cli);
			break;

		case 'g':
			cli = ircp_cli_open(ircp_info_cb, ctrans);
			if(cli == NULL) {
				g_print("Error opening ircp-client\n");
				return -1;
			}
			
			// Connect
			if(ircp_cli_connect(cli) >= 0) {
				// Get all files
				ircp_get(cli, optarg, optarg);

				// Disconnect
				ircp_cli_disconnect(cli);
			}
			ircp_cli_close(cli);
			break;

		case 'p':
			cli = ircp_cli_open(ircp_info_cb, ctrans);
			if(cli == NULL) {
				g_print("Error opening ircp-client\n");
				return -1;
			}
			
			// Connect
			if(ircp_cli_connect(cli) >= 0) {
				// Send all files
				ircp_put(cli, optarg);

				// Disconnect
				ircp_cli_disconnect(cli);
			}
			ircp_cli_close(cli);
			break;

		case 'i':
			cli = ircp_cli_open(ircp_info_cb, ctrans);
			if(cli == NULL) {
				g_print("Error opening ircp-client\n");
				return -1;
			}
			
			// Connect
			if(ircp_cli_connect(cli) >= 0) {
				// Retrieve Info
				ircp_info(cli, 0x01);
				ircp_info(cli, 0x02);

				// Disconnect
				ircp_cli_disconnect(cli);
			}
			ircp_cli_close(cli);
			break;

		case 'm':
			cli = ircp_cli_open(ircp_info_cb, ctrans);
			if(cli == NULL) {
				g_print("Error opening ircp-client\n");
				return -1;
			}
			
			// Connect
			if(ircp_cli_connect(cli) >= 0) {
				// Rename a file
				g_print ("ircp_rename(cli, %s, %s);\n", optarg, argv[optind+2]);
				//ircp_rename(cli, optarg, argv[optind+2]);

				// Disconnect
				ircp_cli_disconnect(cli);
			}
			ircp_cli_close(cli);
			break;

		case 'k':
			cli = ircp_cli_open(ircp_info_cb, ctrans);
			if(cli == NULL) {
				g_print("Error opening ircp-client\n");
				return -1;
			}
			
			// Connect
			if(ircp_cli_connect(cli) >= 0) {
				// Delete file
				ircp_del(cli, optarg);

				// Disconnect
				ircp_cli_disconnect(cli);
			}
			ircp_cli_close(cli);
			break;

		case 'r':
			srv = ircp_srv_open(ircp_info_cb);
			if(srv == NULL) {
				g_print("Error opening ircp-server\n");
				return -1;
			}

			if(optarg)
				inbox = optarg;
			else
				inbox = ".";

			ircp_srv_recv(srv, inbox);
#ifdef DEBUG_TCP
			sleep(2);
#endif
			ircp_srv_close(srv);
			break;

		case 'h':
		case 'u':
			g_print("Usage: %s -[dclgpimkrh]... [<file>...]\n"
				"Transfer files from/to Siemens Mobile Equipment.\n"
				"Copyright (c) 2002 Christian W. Zuckschwerdt\n"
				"\n"
				" -d, --device <device>       use this device\n"
				" -c, --cable                 use serial cable\n"
				" -l, --list [<FOLDER>]       list a folder\n"
				" -g, --get <SOURCE>          fetch files\n"
				" -p, --put <SOURCE>          send files\n"
				" -i, --info                  retrieve misc infos\n\n"
				" -m, --move <SRC> <DEST>     move files\n"
				" -k, --delete <SOURCE>       delete files\n"
				" -r, --receive [<DEST>       receive files\n"
				" -h, --help, --usage         this help text\n"
				"\n",
				argv[0]);
			break;

		default:
			g_print ("Try `%s --help' for more information.\n",
				 argv[0]);
		}
	}

	if (optind < argc) {
		g_print ("non-option ARGV-elements: ");
		while (optind < argc)
			g_print ("%s ", argv[optind++]);
		g_print ("\n");
	}

	exit (0);

}
