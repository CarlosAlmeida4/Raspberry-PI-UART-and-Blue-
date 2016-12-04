
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int argc, char **argv)
{
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };
	
	
	/*////////////////////////////////////////////////////////////////////Scan de dispositivos\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
	//nao ha ligação com clientes, so com o dongle bluetooth
 
	//Aceder ao dongle bluetooth, ainda não faz tem a ver com o controlador
	// Dar NULL à funcao hci_get_route faz com que o programa use o primeiro dongle que encontre
	//hci_open_dev  cria o socket e abre a ligaçao ao dongle*/
	dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    
    //erro socket
    if (dev_id < 0 || sock < 0) {
        perror("opening socket");
        exit(1);
    }

	
    len  = 8;
    max_rsp = 255;
    
    //IREQ_CACHE_FLUSH faz flush da cache e so mostra os clientes ligados atualmente, 0 mostra todos, mesmo aqueles fora de alcance
    flags = IREQ_CACHE_FLUSH;
    
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    
    //hci_inquiry demora 1.28*len e usa um numero em vez de um socket aberto (neste caso sock) por isso passa se o dev_id
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
	
	//erro caso num_rsp
	if( num_rsp < 0 ) perror("hci_inquiry");
    //cout << "Numero dispositivos" << num_rsp << endl;

    for (i = 0; i < num_rsp; i++) {
        //transforma o bdaddr(struct bdaddr_t)(endereço do cliente da struct ii(inquiry_info)) numa string , str2ba tb existe
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
		//hci_read_remote_name retorna 0 se encontra nome, -1 se o nome for muito grande ou der erro
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0) strcpy(name, "[unknown]");
        printf("%s  %s\n", addr, name);
    }

    free( ii );
    close( sock );
    return 0;
}
