#include "../includes/webserv.hpp"

/* int main(int argc, char **argv)
{
    std::string fdName = "default.conf";
    if (argc == 2)
        fdName = argv[1];
} */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main()
{
  int server_fd, client_fd;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  const char *response = "HTTP/1.1 200 OK\n"
                         "Content-Type: text/plain\n"
                         "Content-Length: 13\r\n\r\n"
                         "Hello, World!";

  // 1. Crear el socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  // 2. Configurar opciones del socket
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // 3. Configurar dirección y puerto
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // 4. Poner el socket en modo escucha
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  std::cout << "Servidor escuchando en el puerto " << PORT << std::endl;

  // 5. Aceptar una conexión
  if ((client_fd = accept(server_fd, (struct sockaddr *)&address,
                          (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // 6. Responder al cliente
  send(client_fd, response, strlen(response), 0);
  std::cout << "Respuesta enviada al cliente" << std::endl;

  // 7. Cerrar la conexión
  close(client_fd);
  close(server_fd);

  return 0;
}