#include "functii.h"

int main()
{//SIGINT- semnalul aferent intreruperii unui proces
	signal(SIGINT, signal_callback_handler);
	server();
	return EXIT_SUCCESS;
}
