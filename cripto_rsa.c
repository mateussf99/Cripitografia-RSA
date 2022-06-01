#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define llint long long int
#define TAM 100000

void limpar()
{
    system("cls || clear");
}

int primo(llint num)
{
    if (num == 0 || num == 1)
        return 0;

    llint divisor = 2;

    while (divisor <= sqrt(num))
    {
        if (num % divisor == 0)
            return 0;

        divisor++;
    }

    return 1;
}

llint euclides(llint num1, llint num2)
{
    if (num1 % num2 == 0)
        return num2;
    else
        return euclides(num2, num1 % num2);
}

llint buscar_st2(llint n, llint d, llint c[])
{
    llint q;

    if (d == 0)
        return n;

    c[0] = 0;
    c[1] = 1;

    if (n % d != 0)
        q = n / d;
    else
        q = 1;

    llint mdc = buscar_st2(d, n % d, c);

    if (n % d != 0)
    {
        llint aux = c[1];
        c[1] *= q;
        c[1] += c[0];
        c[0] = aux;
    }

    return mdc;
}

llint buscar_st(llint n, llint d, llint c[])
{
    llint mdc = buscar_st2(n, d, c);

    if ((-1 * c[0] * n) + (c[1] * d) == mdc)
        c[0] *= -1;
    else if ((c[0] * n) - (c[1] * d) == mdc)
        c[1] *= -1;

    return mdc;
}

void congruencias(llint a, llint b, llint m, llint c[], llint s[])
{
    for (int i = 0; i < TAM; i++)
        s[i] = -1;

    llint mdc = euclides(a, m);

    if (mdc == 1)
    {
        buscar_st(a, m, c);

        llint x = b * c[0];
        while (x < 0)
            x += m;
        if (x > m)
            x %= m;

        s[0] = x;
    }
    else if ((mdc > 1) && (b % mdc == 0))
    {
        llint a1 = a / mdc;
        llint b1 = b / mdc;
        llint m1 = m / mdc;

        llint c1[2];

        buscar_st(a1, m1, c1);

        llint x = b1 * c1[0];
        while (x < 0)
            x += m1;
        if (x > m)
            x %= m;

        for (llint i = 0; i < mdc; i++)
            s[i] = x + (m1 * i);
    }

    return;
}

void gerar_chave()
{
    llint p, q, e;

    printf("GERAR CHAVES\n");
    printf("-------------\n");

    printf("Digite os números primos (p) e (q) e o expoente (e):\n");
    scanf("%lld %lld %lld", &p, &q, &e);
    getchar();

    limpar();

    if (primo(p) && primo(q))
    {
        llint n = p * q;
        llint phi = (p - 1) * (q - 1);

        if (euclides(n, phi) == 1)
        {
            FILE *publica = fopen("chave_publica.txt", "w");
            fprintf(publica, "n: %lld\n", n);
            fprintf(publica, "e: %lld\n", e);
            fclose(publica);

            llint c[2];
            llint s[TAM];
            congruencias(e, 1, phi, c, s);
            llint d = s[0];

            FILE *privada = fopen("chave_privada.txt", "w");
            fprintf(privada, "d: %lld\n", d);
            fprintf(privada, "n: %lld\n", n);
            fclose(privada);

            printf("Chaves criadas e salva nos arquivos 'chave_publica.txt' e 'chave_privada.txt'.\n");
        }
        else
            printf("ERRO: MDC((P * Q), ((P - 1) * (Q - 1))) != 1\n");
    }
    else
        printf("ERRO: P E Q NÃO SÃO COPRIMOS\n");

    return;
}

void letra_numero(char mensagem[], llint numero[])
{
    llint max = strlen(mensagem);

    for (llint i = 0; i < max; i++)
    {
        if (mensagem[i] == 32)
            numero[i] = 28;
        else if (mensagem[i] >= 65 && mensagem[i] <= 90)
            numero[i] = mensagem[i] - 63;
        else if (mensagem[i] >= 97 && mensagem[i] <= 122)
            numero[i] = mensagem[i] - 95;
    }
}

llint potencia(llint base, llint expoente, llint res)
{
    if (expoente == 0)
    {
        return 1;
    }
    else if (expoente % 2 == 0)
    {
        llint aux = potencia(base, expoente / 2, res);

        return (aux * aux) % res;
    }
    else
    {
        return ((base % res) * potencia(base, expoente - 1, res)) % res;
    }
}

void criptografar()
{
    char mensagem[TAM];
    llint n, e;

    printf("CRIPTOGRAFAR\n");
    printf("-------------\n");
    printf("Digite sua mensagem:\n");
    getchar();
    scanf("%[^\n]s", mensagem);

    printf("Digite a chave pública (n) e (e):\n");
    scanf("%lld %lld", &n, &e);
    getchar();

    llint max = strlen(mensagem);
    llint mensagemInt[max], mensagemCrip[max];

    letra_numero(mensagem, mensagemInt);

    FILE *m = fopen("mensagem_criptografada.txt", "w");

    for (llint i = 0; i < max; i++)
    {
        llint crip = potencia(mensagemInt[i], e, n);
        mensagemCrip[i] = crip;

        fprintf(m, "%lld ", mensagemCrip[i]);
    }

    fclose(m);

    limpar();

    printf("Mensagem criptografada e salva no arquivo 'mensagem_criptografada.txt'.\n");

    return;
}

void ler_messagem(FILE *menCrip, llint mensagemInt[], llint *max)
{
    while (fscanf(menCrip, "%lld", &mensagemInt[*max]) != EOF)
        ++*max;

    fscanf(menCrip, "%lld", &mensagemInt[*max + 1]);
}

llint numero_letra(llint num)
{
    if (num == 28)
        return 32;
    else if (num >= 2 && num <= 27)
        return num + 63;
}

void descriptografar()
{
    llint p, q, e;
    printf("Digite os números primos (p) e (q) e o expoente (e):\n");
    scanf("%lld %lld %lld", &p, &q, &e);
    getchar();

    limpar();

    if (primo(p) && primo(q))
    {
        llint n = p * q;
        llint phi = (p - 1) * (q - 1);

        if (euclides(n, phi) == 1)
        {
            llint c[2];
            llint s[TAM];
            congruencias(e, 1, phi, c, s);
            llint d = s[0];

            llint chave[2];

            FILE *privada = fopen("chave_privada.txt", "r");
            fscanf(privada, "d: %lld\n", &chave[0]);
            fscanf(privada, "n: %lld\n", &chave[1]);
            fclose(privada);

            llint mensagemInt[TAM];
            llint max = 0;

            FILE *menCrip = fopen("mensagem_criptografada.txt", "r");
            ler_messagem(menCrip, mensagemInt, &max);
            fclose(menCrip);

            char mensagemChar[max];

            FILE *menDescrip = fopen("mensagem_descriptografada.txt", "w");

            for (llint i = 0; i < max; i++)
            {
                llint crip = potencia(mensagemInt[i], d, n);
                mensagemChar[i] = (char)numero_letra(crip);

                fprintf(menDescrip, "%c", mensagemChar[i]);
            }

            fclose(menDescrip);

            limpar();

            printf("Mensagem descriptografada:\n%s\n\n", mensagemChar);
            printf("Mensagem descriptografada e salva no arquivo 'mensagem_descriptografada.txt'.\n");
        }
        else
            printf("ERRO: MDC((P * Q), ((P - 1) * (Q - 1))) != 1\n");
    }
    else
        printf("ERRO: P E Q NÃO SÃO COPRIMOS\n");

    return;
}

void menu()
{
    int opcao;

    do
    {
        limpar();

        printf("------------------------------\n");
        printf("############ MENU ############\n");
        printf("------------------------------\n");
        printf("|1| Gerar chaves\n");
        printf("|2| Criptografar\n");
        printf("|3| Descriptografar\n");
        printf("|4| Encerrar\n");
        printf("------------------------------\n\n");

        printf("Opção: ");
        scanf("%d", &opcao);

        limpar();

        switch (opcao)
        {
        case 1:
            gerar_chave();
            break;
        case 2:
            criptografar();
            break;
        case 3:
            descriptografar();
            break;
        case 4:
            return;

        default:
            printf("ERRO: OPÇÃO INVÁLIDA\n");
            getchar();
            break;
        }

        printf("\nPressione qualquer tecla para continuar...");
        getchar();
    } while (opcao != 4);
}

int main()
{
    menu();

    return 0;
}