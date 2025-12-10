#include <iostream>     // Para imprimir mensajes en consola
#include <string>       // Para manejar std::string
#include <cstring>      // Para memset y funciones C
#include <unistd.h>     // Para read(), write(), close()
#include <sys/socket.h> // Para socket(), bind(), listen(), accept()
#include <sys/select.h> // Para select()
#include <netinet/in.h> // Para sockaddr_in y htons

int main() {
    // =====================================================
    // 1. Crear el socket del servidor (IPv4, TCP)
    // =====================================================
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    // Permite reutilizar el puerto inmediatamente después de cerrar el programa
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // =====================================================
    // 2. Configurar la dirección del servidor
    // =====================================================
    sockaddr_in address;
    address.sin_family = AF_INET;         // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Escuchar en todas las interfaces (0.0.0.0)
    address.sin_port = htons(8080);       // Puerto 8080

    // =====================================================
    // 3. Asociar el socket al puerto
    // =====================================================
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind");
        return 1;
    }

    // =====================================================
    // 4. Escuchar conexiones entrantes
    // =====================================================
    if (listen(server_fd, 10) < 0) {  // backlog = 10
        perror("listen");
        return 1;
    }

    std::cout << "Servidor con select() escuchando en puerto 8080...\n";

    // =====================================================
    // 5. Conjuntos de file descriptors para select()
    // =====================================================
    fd_set master_set;  // Este conjunto almacena TODOS los fds activos
    fd_set read_set;    // Copia que select() modificará

    FD_ZERO(&master_set); // Inicializar vacío
    FD_ZERO(&read_set);

    // Añadimos el socket del servidor al conjunto principal
    FD_SET(server_fd, &master_set);

    // De momento, el descriptor más alto es el del servidor
    int fd_max = server_fd;

    // =====================================================
    // 6. Bucle principal del servidor
    // =====================================================
    while (true) {
        // Cada vez que llamemos a select, debemos copiar el master_set
        read_set = master_set;

        std::cout << "\nEsperando actividad...\n";

        // =====================================================
        // 7. select() bloquea hasta que hay actividad en algún fd
        // =====================================================
        int activity = select(fd_max + 1, &read_set, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select");
            continue;
        }

        // =====================================================
        // 8. Recorremos todos los fds para ver cuáles tienen actividad
        // =====================================================
        for (int fd = 0; fd <= fd_max; fd++) {

            // Si este fd tiene actividad, FD_ISSET devuelve true
            if (FD_ISSET(fd, &read_set)) {

                // =====================================================
                // 9. Si la actividad es en el socket del servidor:
                //    → nueva conexión entrante
                // =====================================================
                if (fd == server_fd) {
                    sockaddr_in client_addr;
                    socklen_t addr_len = sizeof(client_addr);

                    // accept() devuelve un nuevo fd para ese cliente
                    int client_fd = accept(server_fd,
                                           (struct sockaddr*)&client_addr,
                                           &addr_len);

                    if (client_fd < 0) {
                        perror("accept");
                        continue;
                    }

                    std::cout << "Nuevo cliente conectado: fd=" << client_fd << "\n";

                    // Añadir el cliente al conjunto de fds
                    FD_SET(client_fd, &master_set);

                    // Actualizar el máximo descriptor si es necesario
                    if (client_fd > fd_max)
                        fd_max = client_fd;
                }

                // =====================================================
                // 10. Actividad en un cliente ya conectado:
                //      → leer petición HTTP
                // =====================================================
                else {
                    char buffer[4096] = {0};

                    // Leer datos enviados por el cliente
                    int bytes_read = read(fd, buffer, sizeof(buffer));

                    std::cout << "buffer: " << buffer << std::endl;

                    // Si bytes_read <= 0 → cliente cerró conexión
                    if (bytes_read <= 0) {
                        std::cout << "Cliente desconectado: fd=" << fd << "\n";

                        close(fd);               // Cerramos su socket
                        FD_CLR(fd, &master_set); // Lo quitamos del select
                    }

                    // Si recibimos datos válidos:
                    else {
                        std::cout << "\nPetición recibida desde fd=" << fd << ":\n"
                                  << buffer << "\n";

                        // =====================================================
                        // 11. Preparar y enviar respuesta HTTP simple
                        // =====================================================
                        std::string response =
                            "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/plain\r\n"
                            "Content-Length: 12\r\n"
                            "\r\n"
                            "Hola gayyys!";

                        write(fd, response.c_str(), response.size());

                        // Por simplicidad: cerrar después de responder (como HTTP/1.0)
                        close(fd);
                        FD_CLR(fd, &master_set);
                    }
                }
            }
        }
    }

    // Nunca llega aquí, pero por limpieza:
    close(server_fd);
    return 0;
}