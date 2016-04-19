#include <stdio.h> //Esto hay q sacarlo
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hash.h"

#define TAMANO_INICIAL 7

char *strdup(const char *old);
char *strdup(const char *old) {
    char *new;
    if((new = malloc(strlen(old) + 1)) == NULL){
        return NULL;
        }
    
    strcpy(new, old);
    return new;
}

//Devuelve true si el numero recibido por parametro es n, si no, false
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
	size_t primo_encontrado;
	size_t j;
	for(j = principio+1; j <= fin; j++){ //asegurarse de que siempre hay un primo entre principio y fin
		if (es_primo(j)){
			primo_encontrado = j;
			}
		}
	return primo_encontrado;
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

unsigned long hash_generar_clave(const char clave[],size_t tam);

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

typedef struct nodo { 
    const char *clave;
	void *valor;
	bool ocupado; //me parece que esta variable se puede borrar despues (usar el mismo clave como ocupado)
	bool borrado;
	} nodo_hash_t;

typedef struct hash {
	nodo_hash_t *tabla;
	size_t capacidad;
	size_t cantidad_elementos;
	void (*como_destruir)(void*);
    float factor_carga;
	} hash_t;


void inicializar_nodos(hash_t* hash,size_t cantidad_nodos);

void inicializar_nodos(hash_t* hash,size_t cantidad_nodos){
    size_t k;
    for(k = 0; k < cantidad_nodos; k++){
        printf("K vale %d\n",k);
        hash->tabla[k].clave = NULL;
		hash->tabla[k].valor = NULL;
		hash->tabla[k].ocupado = false;
		hash->tabla[k].borrado = false;
        }
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
    inicializar_nodos(nuevo_hash,TAMANO_INICIAL);
	nuevo_hash->capacidad = TAMANO_INICIAL;
	nuevo_hash->cantidad_elementos = 0;
	nuevo_hash->como_destruir = destruir_dato;
    nuevo_hash->factor_carga = 0; //al principio esta vacio
	return nuevo_hash;
	}

size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad_elementos;
    }

void hash_destruir(hash_t *hash){
	//iterar para que borre las claves o libere valores
    free(hash->tabla);
    free(hash);
    }

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	bool insertado = false;
    unsigned long posicion = hash_generar_clave(clave,hash->capacidad);
    printf("La posicion sera.. %lu\n",posicion);
    char *clave_copia = strdup(clave);
	if (!clave_copia){return false;}
    printf("la copia es: %s su direccion de memoria es %p y la vieja era %p\n",clave_copia,(void*)&clave_copia,(void*)&clave);
    //aca primero ver el factor de carga para redimensionar
    
    
    while (!insertado){
		if (!hash->tabla[posicion].clave){ //Esto significa que la clave vale NULL, => esta libre
			hash->tabla[posicion].ocupado = true;
        	hash->tabla[posicion].borrado = false;
        	hash->tabla[posicion].valor = dato;
        	hash->tabla[posicion].clave = clave_copia;
			hash->cantidad_elementos++;
        	insertado = true;
			}
		else{ //La clave != NULL
			if((strcmp(hash->tabla[posicion].clave,clave_copia)) == 0){ //lo que se habia guardado fue borrado O las claves son las mismas
				//if destruir dato ... 
				free((void*)hash->tabla[posicion].clave); //copias que hace alan, las elimina alan
				hash->tabla[posicion].clave = clave_copia;
				hash->tabla[posicion].ocupado = true;
        		hash->tabla[posicion].borrado = false;
        		hash->tabla[posicion].valor = dato;
        		insertado = true;
				hash->cantidad_elementos++;
				}
			else{ //no fue borrada, la clave esta en uso y es distinta => esta ocupado
				posicion++;
				if (posicion == hash->capacidad){
					posicion = 0;
					}
				}
			}
		}
    return insertado;
    }

void *hash_borrar(hash_t *hash, const char *clave){
    return NULL;
	
    }
void *hash_obtener(const hash_t *hash, const char *clave){
	unsigned long posicion = hash_generar_clave(clave,hash->capacidad);
    void* dato = NULL;
	while(hash->tabla[posicion].clave){
		printf("Posicion vale %lu\n",posicion);
		if((strcmp(hash->tabla[posicion].clave,clave)) == 0){
			dato = hash->tabla[posicion].valor;
			break;
			}
		else{
			posicion++;
			if(posicion == hash->capacidad){
				posicion = 0;
				}
			
			printf("Posicion valdra en la prox iteracion %lu\n",posicion);
			}
		}
	return dato;
    }

bool hash_pertenece(const hash_t *hash, const char *clave){
    unsigned long posicion = hash_generar_clave(clave,hash->capacidad);
	bool pertenece = false;
	while(hash->tabla[posicion].clave){
		printf("Posicion vale %lu\n",posicion);
		if((strcmp(hash->tabla[posicion].clave,clave)) == 0){
			pertenece = true;
			break;
			}
		else{
			posicion++;
			if(posicion == hash->capacidad){
				posicion = 0;
				}
			
			printf("Posicion valdra en la prox iteracion %lu\n",posicion);
			}
		}
	return pertenece;
    }
