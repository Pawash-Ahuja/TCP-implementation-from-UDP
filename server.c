#include	"unpifiplus.h"

extern struct ifi_info *Get_ifi_info_plus(int family, int doaliases);
extern        void      free_ifi_info_plus(struct ifi_info *ifihead);

struct serv_conn {
           int ssockfd;
           in_addr_t ip_addr_bound;
	         in_addr_t nw_mask;
           in_addr_t subnet_addr;
           };   //Structure with details of all IP addresses bound;

int
main(int argc, char **argv)
{
	struct ifi_info	*ifi, *ifihead;
	struct sockaddr	*sa;
	struct sockaddr_in	*sain;
	u_char		*ptr;
	int		i, family, doaliases,servport,sockfd,maxfd;
 	const int			on = 1;
  fd_set		rset;
	
	
struct serv_conn conn[10];	
int ind,ind_used;

servport=9870; //well known port number

	if (argc != 3)
		err_quit("usage: prifinfo_plus <inet4|inet6> <doaliases>");

	if (strcmp(argv[1], "inet4") == 0)
		family = AF_INET;
#ifdef	IPv6
	else if (strcmp(argv[1], "inet6") == 0)
		family = AF_INET6;
#endif
	else
		err_quit("invalid <address-family>");
	doaliases = atoi(argv[2]);

	for (ifihead = ifi = Get_ifi_info_plus(family, doaliases),ind=0;
		 ifi != NULL; ifi = ifi->ifi_next,ind++) {
		printf("%s: ", ifi->ifi_name);
		if (ifi->ifi_index != 0)
			printf("(%d) ", ifi->ifi_index);
		printf("<");
/* *INDENT-OFF* */
		if (ifi->ifi_flags & IFF_UP)			printf("UP ");
		if (ifi->ifi_flags & IFF_BROADCAST)		printf("BCAST ");
		if (ifi->ifi_flags & IFF_MULTICAST)		printf("MCAST ");
		if (ifi->ifi_flags & IFF_LOOPBACK)		printf("LOOP ");
		if (ifi->ifi_flags & IFF_POINTOPOINT)	printf("P2P ");
		printf(">\n");
/* *INDENT-ON* */

		if ( (i = ifi->ifi_hlen) > 0) {
			ptr = ifi->ifi_haddr;
			do {
				printf("%s%x", (i == ifi->ifi_hlen) ? "  " : ":", *ptr++);
			} while (--i > 0);
			printf("\n");
		}
		if (ifi->ifi_mtu != 0)
			printf("  MTU: %d\n", ifi->ifi_mtu);

		if ( (sa = ifi->ifi_addr) != NULL)
		{
			printf("  IP addr: %s\n",
						Sock_ntop_host(sa, sizeof(*sa)));
                                             
			sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	    Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

		    sain = (struct sockaddr_in *) ifi->ifi_addr;
		    sain->sin_family = AF_INET;
		    sain->sin_port = htons(servport);
		    Bind(sockfd, (SA *) sain, sizeof(*sain));
		    
		    printf("bound %s\n", Sock_ntop((SA *) sain, sizeof(*sain)));
      conn[ind].ssockfd=sockfd;
			conn[ind].ip_addr_bound=(*sain).sin_addr.s_addr;
        }
           
/*=================== cse 533 Assignment 2 modifications ======================*/

		if ( (sa = ifi->ifi_ntmaddr) != NULL)
		{
			printf("  network mask: %s\n",
						Sock_ntop_host(sa, sizeof(*sa)));
                                             
      sain = (struct sockaddr_in *) ifi->ifi_ntmaddr;                                       
			conn[ind].nw_mask=(*sain).sin_addr.s_addr;
	  	conn[ind].subnet_addr =
                      conn[ind].ip_addr_bound & 
                      conn[ind].nw_mask;
            	
    }
           
                    

/*=============================================================================*/

		if ( (sa = ifi->ifi_brdaddr) != NULL)
			printf("  broadcast addr: %s\n",
						Sock_ntop_host(sa, sizeof(*sa)));
		if ( (sa = ifi->ifi_dstaddr) != NULL)
			printf("  destination addr: %s\n",
						Sock_ntop_host(sa, sizeof(*sa)));
	}
	free_ifi_info_plus(ifihead);
  ind_used=ind-1;
 
  FD_ZERO(&rset);
  maxfd=0;
 for (ind=0;ind<=ind_used;ind++) 
 {
     printf("\n Structure # %d\n",ind);
     printf("\n sockfd= %d",conn[ind].ssockfd);
     FD_SET(conn[ind].ssockfd, &rset);
     if  (conn[ind].ssockfd > maxfd)
         maxfd = conn[ind].ssockfd;
     
     printf("\n ip_addr_bound= %d ",conn[ind].ip_addr_bound);
     bzero(sain, sizeof(struct sockaddr_in));
     (*sain).sin_addr.s_addr=conn[ind].ip_addr_bound;
     sain->sin_family = AF_INET;
     printf("\n ip_addr_bound= %s ",Sock_ntop((SA *) sain, sizeof(*sain)));
     
     
     printf("\n Network mask=  %d",conn[ind].nw_mask);
     bzero(sain, sizeof(struct sockaddr_in));
     (*sain).sin_addr.s_addr=conn[ind].nw_mask;
     sain->sin_family = AF_INET;
     printf("\n Network mask= %s ",Sock_ntop((SA *) sain, sizeof(*sain)));
     
     printf("\n Subnet Address=  %d",conn[ind].subnet_addr);
     bzero(sain, sizeof(struct sockaddr_in));
     (*sain).sin_addr.s_addr=conn[ind].subnet_addr;
     sain->sin_family = AF_INET;
     printf("\n Subnet Address= %s ",Sock_ntop((SA *) sain, sizeof(*sain)));
     
     printf("\n-------------------------");
 }
     
     printf("\n maxfd= %d ",maxfd);		
     printf("\nEntering main select...");
     
     Select(maxfd+1, &rset, NULL, NULL, NULL);  
  //blocks until there is an incoming client
	//	printf("Exited main select");
 
 for (ind=0;ind<=ind_used;ind++) 
 {
 	   if (FD_ISSET(conn[ind].ssockfd, &rset))
       continue; 
       //recvfrom or recvmsg
      //save from and to client IP address  + logic
      
      
      
//      if ( (pid = Fork()) == 0) {		/* child */
//		child(keysockfd, (SA *) &cliaddr, sizeof(cliaddr), (SA *) sa);
//		exit(0);		/* never executed */
//	}
     //fork off child child()


 }


 
	exit(0);
}








