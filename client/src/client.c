#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion; 
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	// Loggear en consola y en un archivo llamado tp0.log con el nivel de log DEBUG
	logger = iniciar_logger();
	if(logger == NULL) {
		fprintf(stderr, "No se pudo crear el logger. Saliendo del programa.\n");
		exit(EXIT_FAILURE);
	}

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	if(config == NULL) {
		log_error(logger, "No se pudo crear el archivo de configuración. Saliendo del programa.");
		exit(EXIT_FAILURE);
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	valor = config_get_string_value(config, "CLAVE"); 
	if (valor == NULL) {
		log_error(logger, "No se pudo obtener el valor de CLAVE del archivo de configuración.");
		exit(EXIT_FAILURE);
	}

	ip = config_get_string_value(config, "IP");
	if (ip == NULL) {
		log_error(logger, "No se pudo obtener la IP del archivo de configuración.");
		exit(EXIT_FAILURE);
	}

	puerto = config_get_string_value(config, "PUERTO"); 
	if (puerto == NULL) {
		log_error(logger, "No se pudo obtener el puerto del archivo de configuración.");
		exit(EXIT_FAILURE);
	}



	// Loggeamos el valor de config
	log_info(logger, "Valor de CLAVE: %s", valor);
	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO: %s", puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	char* file = "cliente.log";
	char* process_name = "CLIENTE_LOGGER";
	bool is_active_console = true;
	t_log_level level = LOG_LEVEL_DEBUG;

	t_log* nuevo_logger = log_create(file, process_name, is_active_console, level);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	char* path = "cliente.config";

	t_config* nuevo_config = config_create(path);

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	do
	{
		log_info(logger, "Leído de consola: %s", leido);
		free(leido); // Liberamos la memoria de la línea leída
		leido = readline("> "); // Leemos la siguiente línea
	} while (leido != NULL);
	
	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	char* leido = readline("> ");
	do
	{
		agregar_a_paquete(paquete, leido, strlen(leido) + 1); // +1 para incluir el '\0'
		free(leido); // Liberamos la memoria de la línea leída
		leido = readline("> "); // Leemos la siguiente línea
	} while (leido != NULL);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	enviar_paquete(paquete, conexion);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	log_destroy(logger);
	config_destroy(config);
	eliminar_paquete(paquete);
	liberar_conexion(conexion);
}
