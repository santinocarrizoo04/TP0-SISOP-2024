#include "client.h"

int main(void)
{
	
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger, "Hola! Este es el inicio del programa-------");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	log_info(logger, "Cargando config... El valor de la clave es : %s", valor);

	/* --------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);
	log_info(logger, "Terminado de leer de consola!");

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	log_info(logger, "Vamos a crear una conexion hacia el servidor...");

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto, logger);

	log_info(logger, "Conexion creada!, enviamos al servidor el valor de CLAVE como msj...");
	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	log_info(logger, "Ahora armemos y enviemos un paquete!");
	paquete(conexion);

	log_info(logger, "FIN!");

	terminar_programa(conexion, logger, config);

}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO);
	if (nuevo_logger == NULL){
		printf("No se pudo crear el LOG\n");
		exit(1);
	}
	else{
		return nuevo_logger;
	}
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("cliente.config");
	if(nuevo_config == NULL){
		printf("No pudo crearse el config\n");
		exit(1);
	}else return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	leido = readline("> ");
	while(!string_is_empty(leido)){
		log_info(logger, "Lo leido por la consola es: %s", leido);
		leido = readline("> ");
	}

	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	while(!string_is_empty(leido)){
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		leido = readline("> ");
	}

	enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger != NULL){
		log_destroy(logger);
	}
	if(config != NULL){
		config_destroy(config);
	}
	liberar_conexion(conexion);
}
