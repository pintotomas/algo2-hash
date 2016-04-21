
char *strdup(const char *old);
char *strdup(const char *old) {
    char *new;
    if((new = malloc(strlen(old) + 1)) == NULL){
        return NULL;
        }
    
    strcpy(new, old);
    return new;
}


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
	size_t primo_encontrado;
	size_t j;
	for(j = principio+1; j <= fin; j++){ //asegurarse de que siempre hay un primo entre principio y fin
		if (es_primo(j)){
			primo_encontrado = j;
			break;
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
	bool borrado;
	} nodo_hash_t;

typedef struct hash {
	nodo_hash_t *tabla;
	size_t capacidad;
	size_t cantidad_elementos;
	void (*como_destruir)(void*);
	} hash_t;

bool hash_capacidad_excedente(hash_t* hash);

bool hash_capacidad_excedente(hash_t* hash){
	bool capacidad_excedente = false;
	if (((hash->capacidad != TAMANO_INICIAL) && (hash->capacidad > TAMANO_INICIAL)) && (((float)hash->cantidad_elementos) <= (((float)1/(2*TRIPLE)) * (float)hash->capacidad))){
		capacidad_excedente = true;
		}
	
	return capacidad_excedente;
	}
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
void inicializar_nodos(nodo_hash_t* hash,size_t cantidad_nodos);

void inicializar_nodos(nodo_hash_t* tabla_hash,size_t cantidad_nodos){
    size_t k;
    for(k = 0; k < cantidad_nodos; k++){
    
        tabla_hash[k].clave = NULL;
		tabla_hash[k].valor = NULL;
		tabla_hash[k].borrado = false;
        }
    }
bool hash_asignar(nodo_hash_t *tabla_hash, const char *clave, void *dato, unsigned long pos);

bool hash_asignar(nodo_hash_t *tabla_hash, const char *clave, void *dato, unsigned long pos){
	tabla_hash[pos].borrado = false;
    tabla_hash[pos].valor = dato;
    tabla_hash[pos].clave = clave;
	return true;
	}
void incrementar_iterador (unsigned long* iterador, size_t limite);

void incrementar_iterador (unsigned long* iterador, size_t limite){
	*iterador+=1;
	if (*iterador == limite){
		*iterador = 0;
		}
	}

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
    //aca primero ver el factor de carga para redimensionar
    if (hash_sobrecargara(hash)){
		size_t rango_busqueda = 20;
		size_t nuevo_tamano = encontrar_proximo_primo(TRIPLE*hash->capacidad,(TRIPLE*hash->capacidad)+rango_busqueda);
		hash_redimensionar(hash,nuevo_tamano);
	}
	bool insertado = false;
    unsigned long posicion = hash_generar_clave(clave,hash->capacidad);
    
    while (!insertado){
		if (!hash->tabla[posicion].clave){ //Esto significa que la clave vale NULL, => esta libre
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

