#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* macros de testes - baseado em minUnit: www.jera.com/techinfo/jtns/jtn002.html */
#define verifica(mensagem, teste) do { if (!(teste)) return mensagem; } while (0)
#define executa_teste(teste) do { char *mensagem = teste(); testes_executados++; \
                                if (mensagem) return mensagem; } while (0)
int testes_executados = 0;

// ========================================
// CIRCULAR BUFFER EXERCISE
// ========================================


#define BUFFER_SUCCESS 0
#define BUFFER_ERROR_FULL -1
#define BUFFER_ERROR_EMPTY -2
#define BUFFER_ERROR_NULL -3

typedef struct {
    int *buffer;          // Array to store data
    int capacity;         // Maximum number of elements
    int head;            // Index where next element will be inserted
    int tail;            // Index where next element will be removed
    int count;           // Current number of elements
} CircularBuffer;

// Function declarations
CircularBuffer* cb_create(int capacity);
void cb_destroy(CircularBuffer* cb);
int cb_put(CircularBuffer* cb, int data);
int cb_get(CircularBuffer* cb, int* data);
int cb_peek(CircularBuffer* cb, int* data);
bool cb_is_empty(CircularBuffer* cb);
bool cb_is_full(CircularBuffer* cb);
int cb_size(CircularBuffer* cb);
int cb_capacity(CircularBuffer* cb);
void cb_clear(CircularBuffer* cb);

static char * executa_testes(void);

int main()
{
    char *resultado = executa_testes();
    if (resultado != 0)
    {
        printf("%s\n", resultado);
    }
    else
    {
        printf("TODOS OS TESTES PASSARAM\n");
    }
    printf("Testes executados: %d\n", testes_executados);

    return resultado != 0;
}

// ========================================
// CIRCULAR BUFFER IMPLEMENTATION
// ========================================

CircularBuffer* cb_create(int capacity) {
    if (capacity <= 0) return NULL;
    
    CircularBuffer* cb = malloc(sizeof(CircularBuffer));
    if (!cb) return NULL;
    
    cb->buffer = malloc(capacity * sizeof(int));
    if (!cb->buffer) {
        free(cb);
        return NULL;
    }
    
    cb->capacity = capacity;
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    
    return cb;
}

void cb_destroy(CircularBuffer* cb) {
    if (cb) {
        free(cb->buffer);
        free(cb);
    }
}

int cb_put(CircularBuffer* cb, int data) {
    if (!cb) return BUFFER_ERROR_NULL;
    if (cb_is_full(cb)) return BUFFER_ERROR_FULL;
    
    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1) % cb->capacity;
    cb->count++;
    
    return BUFFER_SUCCESS;
}

int cb_get(CircularBuffer* cb, int* data) {
    if (!cb || !data) return BUFFER_ERROR_NULL;
    if (cb_is_empty(cb)) return BUFFER_ERROR_EMPTY;
    
    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % cb->capacity;
    cb->count--;
    
    return BUFFER_SUCCESS;
}

int cb_peek(CircularBuffer* cb, int* data) {
    if (!cb || !data) return BUFFER_ERROR_NULL;
    if (cb_is_empty(cb)) return BUFFER_ERROR_EMPTY;
    
    *data = cb->buffer[cb->tail];
    return BUFFER_SUCCESS;
}

bool cb_is_empty(CircularBuffer* cb) {
    return cb ? (cb->count == 0) : true;
}

bool cb_is_full(CircularBuffer* cb) {
    return cb ? (cb->count == cb->capacity) : false;
}

int cb_size(CircularBuffer* cb) {
    return cb ? cb->count : 0;
}

int cb_capacity(CircularBuffer* cb) {
    return cb ? cb->capacity : 0;
}

void cb_clear(CircularBuffer* cb) {
    if (cb) {
        cb->head = 0;
        cb->tail = 0;
        cb->count = 0;
    }
}

// ========================================
// TESTS
// ========================================

static char * test_create_buffer(void) {
    CircularBuffer* cb = cb_create(3);
    verifica("erro: falha ao criar buffer", cb != NULL);
    verifica("erro: buffer deve estar vazio", cb_is_empty(cb));
    verifica("erro: tamanho deve ser 0", cb_size(cb) == 0);
    verifica("erro: capacidade deve ser 3", cb_capacity(cb) == 3);
    cb_destroy(cb);
    return 0;
}

static char * test_put_get_element(void) {
    CircularBuffer* cb = cb_create(3);
    int data;
    
    verifica("erro: falha ao inserir elemento", cb_put(cb, 42) == BUFFER_SUCCESS);
    verifica("erro: buffer não deve estar vazio", !cb_is_empty(cb));
    verifica("erro: tamanho deve ser 1", cb_size(cb) == 1);
    verifica("erro: falha ao obter elemento", cb_get(cb, &data) == BUFFER_SUCCESS);
    verifica("erro: elemento incorreto", data == 42);
    verifica("erro: buffer deve estar vazio", cb_is_empty(cb));
    
    cb_destroy(cb);
    return 0;
}

static char * test_fifo_order(void) {
    CircularBuffer* cb = cb_create(3);
    int data;
    
    cb_put(cb, 1);
    cb_put(cb, 2);
    cb_put(cb, 3);
    
    cb_get(cb, &data);
    verifica("erro: primeiro deve ser 1", data == 1);
    cb_get(cb, &data);
    verifica("erro: segundo deve ser 2", data == 2);
    cb_get(cb, &data);
    verifica("erro: terceiro deve ser 3", data == 3);
    
    cb_destroy(cb);
    return 0;
}

static char * test_full_buffer(void) {
    CircularBuffer* cb = cb_create(2);
    
    cb_put(cb, 1);
    cb_put(cb, 2);
    verifica("erro: buffer deve estar cheio", cb_is_full(cb));
    verifica("erro: não deve inserir em buffer cheio", cb_put(cb, 3) == BUFFER_ERROR_FULL);
    verifica("erro: tamanho deve ser 2", cb_size(cb) == 2);
    
    cb_destroy(cb);
    return 0;
}

static char * test_empty_buffer(void) {
    CircularBuffer* cb = cb_create(2);
    int data;
    
    verifica("erro: não deve obter de buffer vazio", cb_get(cb, &data) == BUFFER_ERROR_EMPTY);
    verifica("erro: buffer deve estar vazio", cb_is_empty(cb));
    verifica("erro: tamanho deve ser 0", cb_size(cb) == 0);
    
    cb_destroy(cb);
    return 0;
}

static char * test_peek_function(void) {
    CircularBuffer* cb = cb_create(3);
    int data;
    
    cb_put(cb, 100);
    verifica("erro: falha ao fazer peek", cb_peek(cb, &data) == BUFFER_SUCCESS);
    verifica("erro: peek deve retornar 100", data == 100);
    verifica("erro: peek não deve alterar tamanho", cb_size(cb) == 1);
    verifica("erro: buffer não deve estar vazio após peek", !cb_is_empty(cb));
    
    cb_destroy(cb);
    return 0;
}

static char * test_clear_function(void) {
    CircularBuffer* cb = cb_create(3);
    
    cb_put(cb, 1);
    cb_put(cb, 2);
    verifica("erro: tamanho deve ser 2", cb_size(cb) == 2);
    
    cb_clear(cb);
    verifica("erro: buffer deve estar vazio após clear", cb_is_empty(cb));
    verifica("erro: tamanho deve ser 0 após clear", cb_size(cb) == 0);
    verifica("erro: deve conseguir inserir após clear", cb_put(cb, 10) == BUFFER_SUCCESS);
    
    cb_destroy(cb);
    return 0;
}

/***********************************************/

static char * executa_testes(void) {
    executa_teste(test_create_buffer);
    executa_teste(test_put_get_element);
    executa_teste(test_fifo_order);
    executa_teste(test_full_buffer);
    executa_teste(test_empty_buffer);
    executa_teste(test_peek_function);
    executa_teste(test_clear_function);
    
    return 0;
}