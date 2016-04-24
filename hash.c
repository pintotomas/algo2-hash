#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hash.h"
#include <stdio.h>
#define TAMANO_INICIAL 13
#define TRIPLE 3


//Devuelve true si el numero recibido por parametro es primo, caso contrario, false
bool es_primo(size_t n);

bool es_primo(size_t n){
	size_t i;
	for (i = 2; i < n; i++){ //ya se que es divisible por si mismo y por 1
		if (n % i == 0){
			return false;
			}
		}
	return true;
	}

//busca un numero primo entre los valores principio y fin
//pre: principio debe ser menor que fin
//post: devuelve el mayor numero primo entre mayor y fin 
size_t encontrar_proximo_primo(size_t principio, size_t fin){

size_t encontrar_proximo_primo(size_t principio, size_t fin);
	size_t primo_encontrado = principio;
	size_t j;
	for(j = principio+1; j <= fin; j++){ //asegurarse de que siempre hay un primo entre principio y fin
		if (es_primo(j)){
			primo_encontrado = j;
			break;
			}
		}
	return primo_encontrado;
	}
			

//Recibe una cadena y devuelve una copia de la misma
char *strdup(const char *old);

char *strdup(const char *old) {
    char *new;
    if((new = malloc(strlen(old) + 1)) == NULL){
        return NULL;
        }
    
    strcpy(new, old);
    return new;
}
	
unsigned long djb2(const char *str);

/* djb2
 * This algorithm was first reported by Dan Bernstein
 * many years ago in comp.lang.c
 */
unsigned long djb2(const char *str){
	unsigned long hash = 5381;
	int c; 
	while ((c = *str++)){
		hash = ((hash << 5) + hash) + c; // hash*33 + c
		}
	return hash;
}

//Recibe una clave y el tamaño del hash, y devuelve la posicion en la que
//Se puede encontrar la clave en el hash.
//Pre: El hash fue craedo.
unsigned long hash_generar_clave(const char clave[],size_t tam_hash);

unsigned long hash_generar_clave(const char clave[],size_t tam_hash){
	size_t longitud_str = strlen(clave);
	unsigned long hash_key = 0;
	size_t k;
	for ( k = 0; k < longitud_str; k++){
		hash_key+= djb2(&clave[k]);
		}
	//reemplazar %tam_hash por un valor que sea el primo mas cercano (y menor que el tamaño)
	return hash_key%tam_hash;
	}

//Incrementa el iterador de un ciclo a el valor siguiente.
//Si el valor siguiente es igual al limite, iterador pasara a valer 0.
void incrementar_iterador (unsigned long* iterador, size_t limite);

void incrementar_iterador (unsigned long* iterador, size_t limite){
	*iterador+=1;
	if (*iterador == limite){
		*iterador = 0;
		}
	}

typedef struct nodo { 
    const char *clave;
	void *valor;
	bool borrado;
	} nodo_hash_t;

//Inicializa los nodos al crear la tabla de hash
//Pre: la tabla fue creada.
void inicializar_nodos(nodo_hash_t* hash,size_t cantidad_nodos);

void inicializar_nodos(nodo_hash_t* tabla_hash,size_t cantidad_nodos){
    size_t k;
    for(k = 0; k < cantidad_nodos; k++){
    
		tabla_hash[k].clave = NULL;
		tabla_hash[k].valor = NULL;
		tabla_hash[k].borrado = false;
        }
    }

typedef struct hash {
	nodo_hash_t *tabla;
	size_t capacidad;
	size_t cantidad_elementos;
	void (*como_destruir)(void*);
	} hash_t;

//Devuelve true si la relacion entre los elementos del hash y la capacidad actual
//del hash es mas grande que una constante 
//ya definida 
//Pre: El hash fue creado. 
bool hash_capacidad_excedente(hash_t* hash);

bool hash_capacidad_excedente(hash_t* hash){
bool capacidad_excedente = false;
	if (((hash->capacidad != TAMANO_INICIAL) && (hash->capacidad > TAMANO_INICIAL)) && (((float)hash->cantidad_elementos) <= (((float)1/(2*TRIPLE)) * (float)hash->capacidad))){
		capacidad_excedente = true;
		}
	
	return capacidad_excedente;
	}

//Devuelve true si el factor de carga del hash es mas alto que una constante ya definida  
//Pre: El hash fue creado.
bool hash_sobrecargara(hash_t* hash);

bool hash_sobrecargara(hash_t* hash){	
	bool sobrecargara = false;
	const double valor_recomendado = 0.6;
	float factor_carga = (float)(hash->cantidad_elementos+1)/(float)hash->capacidad;
	
	if (factor_carga >= valor_recomendado){
		sobrecargara = true;
		}
	return sobrecargara;
	}

//Asigna en la posicion pos de la tabla, la clave y el dato recibidos.
//Pre: La tabla fue creada.
bool hash_asignar(nodo_hash_t *tabla_hash, const char *clave, void *dato, unsigned long pos);

bool hash_asignar(nodo_hash_t *tabla_hash, const char *clave, void *dato, unsigned long pos){
	tabla_hash[pos].borrado = false;
    tabla_hash[pos].valor = dato;
    tabla_hash[pos].clave = clave;
	return true;
	}

//Asigna un par clave:valor de la tabla de hash anterior a
//la nueva tabla de hash.
//Pre: El hash fue redimensionado.
void asignar_a_nueva_tabla(nodo_hash_t* tabla_hash_viejo, size_t pos_actual, nodo_hash_t* tabla_hash_nuevo, size_t nuevo_tamano);

void asignar_a_nueva_tabla(nodo_hash_t* tabla_hash_viejo, size_t pos_actual, nodo_hash_t* tabla_hash_nuevo, size_t nuevo_tamano){
	unsigned long n_posicion = hash_generar_clave(tabla_hash_viejo[pos_actual].clave,nuevo_tamano);
	bool insertado = false;
	while (!insertado){
		if (!tabla_hash_nuevo[n_posicion].clave){
			insertado = hash_asignar(tabla_hash_nuevo,tabla_hash_viejo[pos_actual].clave,tabla_hash_viejo[pos_actual].valor,n_posicion);
			}
		else {
			incrementar_iterador(&n_posicion,nuevo_tamano);
	         }
		}		
	}									

//Al redimensionar el hash, asigna las claves:valores de la tabla de hash anterior a
//la nueva tabla de hash.
//Pre: El hash fue redimensionado.
void reubicar(nodo_hash_t* tabla_hash_viejo, nodo_hash_t* tabla_hash_nuevo,size_t viejo_tamano, size_t nuevo_tamano);
void reubicar(nodo_hash_t* tabla_hash_viejo, nodo_hash_t* tabla_hash_nuevo,size_t viejo_tamano, size_t nuevo_tamano){
	size_t pos_actual = 0;	
	while (pos_actual < viejo_tamano){
		if ((tabla_hash_viejo[pos_actual].clave)){
			if (tabla_hash_viejo[pos_actual].borrado){ //El dato habia sido borrado, tengo que liberar clave
				free((void*)tabla_hash_viejo[pos_actual].clave);
				}
			else{
				asignar_a_nueva_tabla(tabla_hash_viejo,pos_actual,tabla_hash_nuevo,nuevo_tamano);
				}	
			}
		pos_actual++;
		}	
	}

//Redimensiona el hash.
//Pre: El hash sobrecargo/su relacion entre cantidad de elementos y capacidad es muy chica
bool hash_redimensionar(hash_t* hash, size_t tamano);

bool hash_redimensionar(hash_t* hash, size_t tamano){
	
	bool redimensiono = true;
	nodo_hash_t *vieja_tabla = hash->tabla;
	nodo_hash_t *nueva_tabla = malloc(tamano*(sizeof(nodo_hash_t)));
	if (!nueva_tabla){
		return !redimensiono;
		}
	inicializar_nodos(nueva_tabla,tamano);
	reubicar(vieja_tabla,nueva_tabla,hash->capacidad,tamano);
	free(vieja_tabla);
	hash->tabla = nueva_tabla;
	hash->capacidad = tamano;
	return redimensiono;
	}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* nuevo_hash = malloc(sizeof(hash_t));
	if (!nuevo_hash){
		return NULL;
		}
	nuevo_hash->tabla = malloc(TAMANO_INICIAL*(sizeof(nodo_hash_t)));
    
	if (!nuevo_hash->tabla){
		free(nuevo_hash);
		return NULL;
		}
    inicializar_nodos(nuevo_hash->tabla,TAMANO_INICIAL);
	nuevo_hash->capacidad = TAMANO_INICIAL;
	nuevo_hash->cantidad_elementos = 0;
	nuevo_hash->como_destruir = destruir_dato;
	return nuevo_hash;
	}

size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad_elementos;
    }

void hash_destruir(hash_t *hash){
	size_t pos_actual = 0;
	while (pos_actual < hash->capacidad){
		if (hash->tabla[pos_actual].clave){
			free((void*)hash->tabla[pos_actual].clave);
			if (!hash->tabla[pos_actual].borrado && hash->como_destruir){
				hash->como_destruir(hash->tabla[pos_actual].valor);
				}
			}
		pos_actual++;
		}
    free(hash->tabla);
    free(hash);
    }



//Reemplaza el dato de una clave que ya habia sido asignado en el hash, destruyendo el dato
//anterior si es necesario.
//En el caso en el que la clave habia sido borrada, actualiza los elementos del hash
//Pre: El hash fue creado y la tabla en la posicion pos ya estaba en uso.
bool reemplazar_clave_actual(hash_t *hash, const char *clave_copia, void *dato, unsigned long posicion);

bool reemplazar_clave_actual(hash_t *hash, const char *clave_copia, void *dato, unsigned long posicion){
	if(hash->tabla[posicion].borrado == true){
		hash->cantidad_elementos++;
		}
	if(hash->como_destruir){
		hash->como_destruir(hash->tabla[posicion].valor);
		}
	free((void*)hash->tabla[posicion].clave); 
	hash_asignar(hash->tabla,clave_copia,dato,posicion);
	return true;
	}				
		
bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    char *clave_copia = strdup(clave);
	if (!clave_copia){return false;}
    if (hash_sobrecargara(hash)){
		size_t rango_busqueda = 20;
		size_t nuevo_tamano = encontrar_proximo_primo(TRIPLE*hash->capacidad,(TRIPLE*hash->capacidad)+rango_busqueda);
		hash_redimensionar(hash,nuevo_tamano);
	}
	bool insertado = false;
    unsigned long posicion = hash_generar_clave(clave,hash->capacidad);
    
    while (!insertado){
		if (!hash->tabla[posicion].clave){ //la clave vale NULL, => esta libre
			insertado = hash_asignar(hash->tabla,clave_copia,dato,posicion);
			hash->cantidad_elementos++;
			}
		else{ //La clave != NULL
			if((strcmp(hash->tabla[posicion].clave,clave_copia)) == 0){ //lo que se habia guardado fue borrado O las claves son las mismas
				insertado = reemplazar_clave_actual(hash,clave_copia,dato,posicion);
				}
			else{ //no fue borrada, la clave esta en uso y es distinta => esta ocupado
				incrementar_iterador(&posicion,hash->capacidad);
				}
			}
		}
    return insertado;
    }

//Busca una clave en la tabla de hash. Devuelve su posicion o -1 si la clave
//no se encuentra.
unsigned long hash_buscar(nodo_hash_t *tabla, const char *clave, size_t capacidad_hash);

unsigned long hash_buscar(nodo_hash_t *tabla, const char *clave, size_t capacidad_hash){
	unsigned long posicion = hash_generar_clave(clave,capacidad_hash);
	while(tabla[posicion].clave){
		if(((strcmp(tabla[posicion].clave,clave)) == 0) && !tabla[posicion].borrado){ //Es la misma clave y no fue borrado su contenido anteriormente
			return posicion;
			}
		else{
			incrementar_iterador(&posicion,capacidad_hash);
			}
		}
	return -1;
	}

void *hash_borrar(hash_t *hash, const char *clave){
	void* dato = NULL;
	unsigned long pos_dato = hash_buscar(hash->tabla,clave,hash->capacidad);
	if (pos_dato != -1){
		dato = hash->tabla[pos_dato].valor;
		hash->tabla[pos_dato].borrado = true;
		hash->cantidad_elementos--;
		}	
	if (hash_capacidad_excedente(hash)){
		size_t tercera_parte_capacidad = (size_t)(((float)1/3)*((float)hash->capacidad));
		const size_t rango_busqueda = 10;
		size_t nuevo_tam = encontrar_proximo_primo(tercera_parte_capacidad,tercera_parte_capacidad+rango_busqueda);
		hash_redimensionar(hash,nuevo_tam);
		}
	return dato;
    }

void *hash_obtener(const hash_t *hash, const char *clave){
	void* dato = NULL;
	unsigned long pos_dato = hash_buscar(hash->tabla,clave,hash->capacidad);
	if (pos_dato != -1){
		dato = hash->tabla[pos_dato].valor;
		}
	return dato;
    }

bool hash_pertenece(const hash_t *hash, const char *clave){
	unsigned long pos_dato = hash_buscar(hash->tabla,clave,hash->capacidad);
	bool pertenece = false;
	if (pos_dato != -1){
		pertenece = true;
		}
	return pertenece;
    }

typedef struct hash_iter { const hash_t* hash;
						   const char* clave_actual;
						   size_t posicion;
						   size_t limite;
						   size_t elementos_a_iterar;
						   size_t elementos_iterados;
						 } hash_iter_t;

//Avanza el iterador del hash, segun el caso en el que se encuentre
//Pre: El iterador fue creado.
void avanzar_iterador(hash_iter_t* iter, bool no_primera_iteracion);

void avanzar_iterador(hash_iter_t* iter, bool no_primera_iteracion){
	if (no_primera_iteracion && (iter->posicion < iter->limite)){
		iter->posicion++; //si no, al evaluar en el while queda en el mismo elemento
		}
	while((!iter->hash->tabla[iter->posicion].clave || iter->hash->tabla[iter->posicion].borrado) && (iter->posicion < iter->limite)){ 
		iter->posicion++;
		}
	iter->clave_actual = iter->hash->tabla[iter->posicion].clave;
	if (iter->elementos_iterados < (iter->elementos_a_iterar+1)){
		iter->elementos_iterados++;
		}
	}

hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* nuevo_iter = malloc(sizeof(hash_iter_t));
	if (!nuevo_iter){
		return NULL;
		}
	nuevo_iter->hash = hash;
	nuevo_iter->limite = hash->capacidad-1;
	nuevo_iter->posicion = 0;
	nuevo_iter->elementos_a_iterar = hash->cantidad_elementos;
	nuevo_iter->elementos_iterados = 0;
	avanzar_iterador(nuevo_iter,false);	
	return nuevo_iter;
	}

bool hash_iter_avanzar(hash_iter_t *iter){
	bool avanzo = false;
	size_t pos_vieja = iter->posicion;
	avanzar_iterador(iter,true);
	if (pos_vieja != iter->posicion){
		avanzo = true;
		}
	if (avanzo == false){
		iter->clave_actual = NULL;
		}
	return avanzo;
	}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	return iter->clave_actual;
	}

bool hash_iter_al_final(const hash_iter_t *iter){
	return (iter->elementos_iterados == (iter->elementos_a_iterar+1));
	}

void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
	}
