#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* macros de testes - baseado em minUnit: www.jera.com/techinfo/jtns/jtn002.html */
#define verifica(mensagem, teste) do { if (!(teste)) return mensagem; } while (0)
#define executa_teste(teste) do { char *mensagem = teste(); testes_executados++; \
                                if (mensagem) return mensagem; } while (0)
int testes_executados = 0;

// ========================================
// EXERCÍCIO 1: FIND FIRST BIT
// ========================================
// Objetivo: Encontrar e retornar a posição do primeiro
// bit igual a 1, em uma variável de 32 bits sem sinal,
// a partir do bit mais significativo.

int find_first_bit(uint32_t value);
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
// IMPLEMENTAÇÃO
// ========================================

int find_first_bit(uint32_t value) {
    if (value == 0) return -1;  // Nenhum bit setado
    
    // Procurar do MSB (bit 31) para o LSB (bit 0)
    for (int i = 31; i >= 0; i--) {
        if ((value >> i) & 1) {  // Se bit i está setado
            return 31 - i;       // Retorna posição a partir do MSB
        }
    }
    
    return -1;  // Nunca deveria chegar aqui
}

// ========================================
// TESTES
// ========================================

static char * test_find_first_bit_msb(void) {
    // Teste com MSB setado: 0x80000000 = bit 31
    verifica("erro: MSB deve retornar 0", find_first_bit(0x80000000U) == 0);
    return 0;
}

static char * test_find_first_bit_example(void) {
    // Exemplo: 0b00100111 = 39 decimal
    // Em 32 bits: 00000000000000000000000000100111
    // Primeiro 1 está na posição 26 (contando do MSB, base 0)
    uint32_t value = 0b00100111;  // = 39 decimal
    verifica("erro: deve retornar 26", find_first_bit(value) == 26);
    return 0;
}

static char * test_find_first_bit_lsb(void) {
    // Teste com apenas LSB setado: 0x00000001 = bit 0
    verifica("erro: LSB deve retornar 31", find_first_bit(1) == 31);
    return 0;
}

static char * test_find_first_bit_zero(void) {
    // Teste com valor 0
    verifica("erro: zero deve retornar -1", find_first_bit(0) == -1);
    return 0;
}

static char * test_find_first_bit_middle(void) {
    // Teste com bit no meio: 0x00010000 = bit 16
    verifica("erro: bit 16 deve retornar 15", find_first_bit(0x00010000U) == 15);
    return 0;
}

/***********************************************/

static char * executa_testes(void) {
    executa_teste(test_find_first_bit_msb);
    executa_teste(test_find_first_bit_example);
    executa_teste(test_find_first_bit_lsb);
    executa_teste(test_find_first_bit_zero);
    executa_teste(test_find_first_bit_middle);
    
    return 0;
}