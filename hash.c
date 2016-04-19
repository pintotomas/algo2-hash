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
	bool ocupado;
	bool borrado;
	} nodo_hash_t;

/*nodo_hash_t nuevo_nodo(void);

nodo_hash_t nuevo_nodo(void){
    
    nodo_hash_t nodo;
    nodo.clave = NULL;
    nodo.valor = NULL;
    nodo.ocupado = false;
    nodo.borrado = false;
    return nodo;
    }
*/
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
    free(hash->tabla);
    free(hash);
    }

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    unsigned long posicion = hash_generar_clave(clave,hash->capacidad);
    printf("La posicion sera.. %lu\n",posicion);
    char *clave_copia = strdup(clave);
    printf("la copia es: %s su direccion de memoria es %p y la vieja era %p\n",clave_copia,(void*)&clave_copia,(void*)&clave);
    //aca primero ver el factor de carga para redimensionar
    bool insertado = false;
    
    while (!insertado){
        puts("Se va a hacer la primer comparacion");
        if ((hash->tabla[posicion].clave) && (strcmp(hash->tabla[posicion].clave,clave_copia)) == 0){
            free(clave_copia);
            break;
            }
        puts("Se va a hacer la segunda comparacion");
        if((hash->tabla[posicion].ocupado == false) || (hash->tabla[posicion].borrado == true)){
            puts("Se entro al else if");
            hash->tabla[posicion].ocupado = true;
            hash->tabla[posicion].borrado = false;
            hash->tabla[posicion].valor = dato;
            hash->tabla[posicion].clave = clave_copia;
            insertado = true;
            }
        else {
            posicion+=1;
            }
        }
    return insertado;
    }   
    

void *hash_borrar(hash_t *hash, const char *clave){
    return NULL;
    }
void *hash_obtener(const hash_t *hash, const char *clave){
    return NULL;
    }
bool hash_pertenece(const hash_t *hash, const char *clave){
    return false;
    }
int main(){
    hash_t* new_table = hash_crear(NULL);
    printf("La capacidad de nt es %d\n",new_table->capacidad);
    for (size_t k = 0; k < TAMANO_INICIAL; k++){
        printf("Deberese ser null: %p\n",new_table->tabla[k].valor);
        }
	char clave1[] = "gato";
    int a = 3;
    int *b = &a;
	bool blabla = hash_guardar(new_table,clave1,(void*)b);

    if (blabla){puts("KE");}
    for (size_t k = 0; k < TAMANO_INICIAL; k++){
        printf("Deberese ser null: %p\n",new_table->tabla[k].valor);
        }
    free((void*)new_table->tabla[5].clave);  //cuando vaya a borrar un elemento voy a tener que hacer un casteo
    hash_destruir(new_table);
	return 0;
	}
