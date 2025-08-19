#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* macros de testes - baseado em minUnit: www.jera.com/techinfo/jtns/jtn002.html */
#define verifica(mensagem, teste) do { if (!(teste)) return mensagem; } while (0)
#define executa_teste(teste) do { char *mensagem = teste(); testes_executados++; \
                                if (mensagem) return mensagem; } while (0)
int testes_executados = 0;

// ========================================
// EXERCÍCIO 2: DECIMAL TO BCD
// ========================================


uint16_t decimal_to_bcd(uint16_t decimal);
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

uint16_t decimal_to_bcd(uint16_t decimal) {
    if (decimal > 9999) return 0;  // Valor inválido
    
    uint16_t bcd = 0;
    
    // Extrair cada dígito e converter para BCD
    uint16_t thousands = (decimal / 1000) % 10;
    uint16_t hundreds  = (decimal / 100) % 10;
    uint16_t tens      = (decimal / 10) % 10;
    uint16_t units     = decimal % 10;
    
    // Montar BCD: cada dígito ocupa 4 bits
    bcd = (thousands << 12) | (hundreds << 8) | (tens << 4) | units;
    
    return bcd;
}

// ========================================
// TESTES
// ========================================

static char * test_bcd_single_digit(void) {
    // Teste com dígito único
    verifica("erro: 5 deve ser 0x0005", decimal_to_bcd(5) == 0x0005);
    verifica("erro: 9 deve ser 0x0009", decimal_to_bcd(9) == 0x0009);
    verifica("erro: 0 deve ser 0x0000", decimal_to_bcd(0) == 0x0000);
    return 0;
}

static char * test_bcd_two_digits(void) {
    // Teste com dois dígitos  
    verifica("erro: 23 deve ser 0x0023", decimal_to_bcd(23) == 0x0023);
    verifica("erro: 99 deve ser 0x0099", decimal_to_bcd(99) == 0x0099);
    verifica("erro: 10 deve ser 0x0010", decimal_to_bcd(10) == 0x0010);
    return 0;
}

static char * test_bcd_three_digits(void) {
    // Teste com três dígitos
    verifica("erro: 123 deve ser 0x0123", decimal_to_bcd(123) == 0x0123);
    verifica("erro: 456 deve ser 0x0456", decimal_to_bcd(456) == 0x0456);
    verifica("erro: 789 deve ser 0x0789", decimal_to_bcd(789) == 0x0789);
    return 0;
}

static char * test_bcd_four_digits(void) {
    // Teste com quatro dígitos
    verifica("erro: 1234 deve ser 0x1234", decimal_to_bcd(1234) == 0x1234);
    verifica("erro: 9876 deve ser 0x9876", decimal_to_bcd(9876) == 0x9876);
    verifica("erro: 9999 deve ser 0x9999", decimal_to_bcd(9999) == 0x9999);
    return 0;
}


/***********************************************/

static char * executa_testes(void) {
    executa_teste(test_bcd_single_digit);
    executa_teste(test_bcd_two_digits);
    executa_teste(test_bcd_three_digits);
    executa_teste(test_bcd_four_digits);

    
    return 0;
}