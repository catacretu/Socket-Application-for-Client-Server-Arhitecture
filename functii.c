#include"functii.h"

void signal_callback_handler(int signum) {
   puts("Activare handler");
   
   exit(signum);
   //exit(0)- incheiere cu succes
}

void client_f(int socket_accept)
{
struct addrinfo hints, *res;
//structura care ofera informatii despre serviciile TCP/IP
int socket_client,receive;
int status;
char recvbuf[1024];
FILE *file;
//creare fisier
if((file = fopen("index.html", "w"))==NULL)
{
	puts("Eroare la crearea fisierului!");
	exit(1);
}
socket_client = socket(AF_INET6, SOCK_STREAM, 0); //creare socket client de tip IPv6
if (socket_client < 0)
{	
	puts("Eroare la socket-ul clientului!");
	exit(1);
}
memset(&hints, 0, sizeof(hints)); //initializam structura cu 0
hints.ai_family = AF_INET6;		  //familia de adrese IPv6
hints.ai_socktype = SOCK_STREAM;  //TCP socket
if (status = getaddrinfo("www.google.com", "80", &hints, &res) < 0)
{//obtinem o lista de adrese pe baza criteriilor mentionate in hints
	puts("Eroare getaddrinfo!");
	close(socket_client);
	exit(1);
}

if (connect(socket_client, res->ai_addr, res->ai_addrlen) < 0) //incercam sa stabilim o conexiune cu server-ul IPv6
{//res->ai_addr=este o structura ce contine portul si adresa IP destinatie
	puts("Eroare connect!");
	close(socket_client);
	exit(1);
}
char *msg = "GET / HTTP/1.0\r\n\r\n";
int len, bytes_sent;
len = strlen(msg);
bytes_sent = send(socket_client, msg, len, 0); //trimitem comanda GET/HTTP... pe conexiunea stabilita anterior
if (bytes_sent < 0)
{
	puts("Eroare la trimiterea comenzi HTTTP");
	exit(1);
}
int recvbuf_len = 1023;
while ((bytes_sent = recv(socket_client, recvbuf, recvbuf_len, 0)) > 0)//primim datele cerute de la server( pagina html)
{
	recvbuf[bytes_sent] = 0;
	int len_buf=bytes_sent;
	int pagina = send(socket_accept, recvbuf, len_buf, 0);
	if (pagina < 0)
	{
		puts("Eroare la trimiterea paginii HTML");
		break;
	}
	//puts(recvbuf);
	fputs(recvbuf, file);//scriem datele receptionate intr-n fisiser
}
fclose(file);
puts("Pagina a fost receptionata cu succes");
close(socket_client);

}

void server()
{
int socket_descriptor,socket_accept,recm;
	char buff[100];
	char *mesaj = "Comanda neimplementata";
	struct sockaddr_in server, client;
	//structura pentru gestionarea adreselor de internet
	socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	//creare socket IPv4, TCP socket, 0-alege automat TCP sau UDP
	//in functie de tipul streamul-ui
	if (socket_descriptor == -1)
	{
		puts("Nu s-a putut crea socket-ul");
		exit(1);
	}

	//Initializare proprietati server
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY; // clientul se va putea conecta la toate adresele asignate server-ului IPv4
	server.sin_port = htons(22116);		 //g1s1e6, lucreaza pe 16 biti
	//host byte order to network byte order
	//Little-endian(Intel) ->Big-endian 

	int yes=1;
	
	if(setsockopt(socket_descriptor,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes)<0)
	//permite conectarea la un port agatat intr-o conexiune anterioara nefininalizata  
	
	{
		puts("setsockopt->error");
		exit(1);
	}
	puts("setsockopt->OK");
	
	int dim_server = sizeof(server);
	if (bind(socket_descriptor, (struct sockaddr *)&server, dim_server) < 0) 
	{
		puts("Eroare la bind");
		puts(strerror(errno)); // afiseaza tipul de eroare
		//errno-pastreaza codul erorii de la ultimul apel al sistemului
		exit(1);
	}
	puts("Bind-> OK");

	//listen for incoming connections
	if (listen(socket_descriptor, 1) < 0)
	{
		puts("Eroare la listen");
		exit(1);
	}
	puts("listen-> OK");
	//Acceptarea conexiunii cu clientul
	puts("Se asteapta conexiuni");
	int new_size = sizeof(client);
	do
	{
		socket_accept = accept(socket_descriptor, (struct sockaddr *)&client, (socklen_t *)&new_size);
		//stabilim conexiunea cu primul client din coada de asteptare
		//extragem un nou fisier de tip descriptor ce referentiaza catre socket-ul clientului
		if (socket_accept < 0)
		{
			puts("Cererea de acceptare a esuat");
			break;
		}

		puts("Conexiune acceptata");

		for (;;)
		{
			recm = recv(socket_accept, buff, sizeof(buff), 0);
			if (recm < 0)
			{
				puts("Eroare la receptie");
				break;
			}
			else if (recm > 0)
			{
				if (strncmp(buff, "05#", 3) != 0)
				{
					int comanda = send(socket_accept, mesaj, strlen(mesaj), 0);
					if (comanda < 0)
					{
						puts("Eroare la trimiterea mesajului comenzii neimplementate");
						break;
					}
					puts(mesaj);
					
				}

				else
				{
					client_f(socket_accept);
					//break;
				}
			}
			if (recm == 0)
			{
				puts("Conexiunea a fost incheiata cu succes");
				break;
			}
		}
		close(socket_accept);
	} while (1);
	close(socket_descriptor);
}
