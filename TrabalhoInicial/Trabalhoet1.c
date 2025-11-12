#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura Final
typedef struct {
char id[20]; // ID do dispositivo (ex: "SENS-TEMP")
int num; //Número do dispositivo (ex: 3)
char comando[20]; // Ação a ser tomada (ex: "SET_VELOCIDADE")
float valor; // Valor numérico (se aplicável, ex: 1.25)
int status_valor; // 0 se o comando não usa 'VALOR', 1 se usa.
int status; // 1 se o comando é válido 0 se inválido
} ComandoDispositivo;

// Estrutura Inicial
typedef struct {
    char id[20];
    char num[5];
    char comando[20];
    char valor[10];
    char ComandoString[100];
} PacoteComando;

int ehNumeroValido(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0; // String nula ou vazia
    }

    int pontoEncontrado = 0; // 0 = não, 1 = sim

    for (int i = 0; str[i] != '\0'; i++) {
        // Se for um dígito, é válido, continue
        if (isdigit((unsigned char)str[i])) {
            continue;
        }

        // Se for um ponto
        if (str[i] == '.') {
            if (pontoEncontrado) {
                return 0; // Encontrou um segundo ponto
            }
            pontoEncontrado = 1; // Marca que encontrou o primeiro ponto
        }
        // Se não for dígito nem ponto
        else {
            return 0; // Caractere inválido (ex: 'a', '-', ' ')
        }
    }
    
    // Caso especial: a string é SÓ "."
    if (strcmp(str, ".") == 0) {
        return 0;
    }

    return 1; // Passou por tudo, é um número válido
}



void MenuDispositivo(ComandoDispositivo *cmd, PacoteComando *pacote)
{
    // Variável para armazenar a escolha do usuário
    int escolha;

    // Exibir menu de dispositivos
    printf("Menu de Dispositivos:\n");
    printf("Escolha o dispositivo:\n");
    printf("1 - SENS-TEMP\n");
    printf("2 - MTR-ESTEIRA\n");
    printf("3 - VAL-OX\n");
    
    // Ler escolha do usuário
    printf("Digite o numero do dispositivo: ");
    scanf("%d", &escolha);
    getchar(); // Limpar o buffer do teclado
    
    // Atribuir ID com base na escolha do usuário
    switch(escolha) { 
        case 1:
            strcpy(pacote->id, "SENS-TEMP");
            printf("registrado com sucesso!\n");
            break;
        case 2:
            strcpy(pacote->id, "MTR-ESTEIRA");
            printf("registrado com sucesso!\n");
            break;
        case 3:
            strcpy(pacote->id, "VAL-OX");
            printf("Sensor registrado com sucesso!\n");
            break;
            
        default:
            printf("Dispositivo invalido!\n");
            cmd->status = 0; // 1 se o comando é válido 0 se inválido
            break;
    }

    // Ler número do dispositivo
    printf("Digite o numero do dispositivo: ");
    fgets(pacote->num, sizeof(pacote->num), stdin);
    strtok(pacote->num, "\n"); // Remover o caractere de nova linha
    if (ehNumeroValido(pacote->num) == 0) {
        printf("Erro: O numero do dispositivo deve ser numerico!\n");
        cmd->status = 0; // Marca o comando como inválido
    }
    


}

void MenuComandos(ComandoDispositivo *cmd, PacoteComando *pacote)
{
    // Variavel de Escolha
    int escolha;

    // Exibir menu de comandos
    printf("Menu de Comandos:\n");
    printf("Escolha o comando:\n");
    printf("1 - SET_VELOCIDADE\n");
    printf("2 - SET_ESTADO\n");
    printf("3 - LER_DADO\n");

    // Ler escolha do usuário
    printf("Digite o numero do comando: ");
    scanf("%d", &escolha);
    getchar(); // Limpar o buffer do teclado

    // Atribuir comando com base na escolha do usuário
    switch(escolha) {
        case 1:
            strcpy(pacote->comando, "SET_VELOCIDADE");
            printf("Comando registrado com sucesso!\n");
            // Ler valor
            printf("Digite o valor: ");
            fgets(pacote->valor, sizeof(pacote->valor), stdin);
            strtok(pacote->num, "\n"); // Remover o caractere de nova linha
            //Validação do valor
            if (ehNumeroValido(pacote->valor) == 0) {
                printf("Erro: Valor invalido! Deve ser um numero (ex: 1.25).\n");
                cmd->status = 0;
            } else {
                printf("Comando registrado com sucesso!\n");
                cmd->status_valor = 1; // Comando usa valor
            }
            break;
            
        case 2:
            strcpy(pacote->comando, "SET_ESTADO");
            printf("Comando registrado com sucesso!\n");
            // Ler valor
            printf("Digite o valor (0 -> Desligado ou 1 -> Ligado): ");
            fgets(pacote->valor, sizeof(pacote->valor), stdin);
            strtok(pacote->num, "\n"); // Remover o caractere de nova linha
            //Validação do valor
            if (ehNumeroValido(pacote->valor) == 0) {
                printf("Erro: Valor invalido! Deve ser 0 ou 1.\n");
                cmd->status = 0;
            } else if (strcmp(pacote->valor, "0") != 0 && strcmp(pacote->valor, "1") != 0) {
                // É um número válido (ex: "5" ou "1.2"), mas não é "0" nem "1"
                printf("Erro: Valor invalido! Deve ser 0 ou 1.\n");
                cmd->status = 0;
            } else {
                printf("Comando registrado com sucesso!\n");
                cmd->status_valor = 1; // Comando usa valor
            }
            break;
        case 3:
            strcpy(pacote->comando, "LER_DADO");
            printf("Comando registrado com sucesso!\n");
            fgets(pacote->valor, sizeof(pacote->valor), stdin);
            strtok(pacote->num, "\n"); // Remover o caractere de nova linha
            cmd->status_valor = 0; // Comando não usa valor
            break;
        default:
            printf("Comando invalido!\n");
            cmd->status = 0; // Comando inválido
            break;
            
    }
}

void MontarStringComando(PacoteComando *pacote)
{
// Montar a string final do comando
sprintf(pacote->ComandoString, "%s;%s;%s;%s", pacote->id, pacote->num, pacote->comando, pacote->valor);
}



int main()
{
    //Declaração da variável inicial
    PacoteComando pacote;
    // Declaração da variável final
    ComandoDispositivo cmd;
    cmd.status = 1; // Inicializa como válido

    // Chamada da função de menu dispositivos
    MenuDispositivo(&cmd, &pacote);

    if (cmd.status == 0) {
        return 0; // Sai se o dispositivo for inválido
    }
    // Chamada da função de menu de comandos
    MenuComandos(&cmd, &pacote);

    if (cmd.status == 0) {
        return 0; // Sai se o comando for inválido
    }

    MontarStringComando(&pacote);
    printf("Comando Final: %s\n", pacote.ComandoString);

    

    
    return 0;
}
