#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> 

// Parámetros de la ecuación del calor
#define L 1.0  // Longitud de la barra
#define alpha 0.01  // Difusividad térmica
#define T_max 0.5  // Tiempo máximo de simulación
#define N_x 100  // Número de puntos espaciales
#define N_t 1000  // Número de pasos de tiempo


// Función para inicializar la condición inicial
void inicializar_condicion_inicial(double* u, double dx) {
    for (int i = 0; i < N_x; i++) {
        double x = i * dx;
        u[i] = sin(M_PI * x / L);  // Condición inicial: onda sinusoidal
    }
}

// Función para aplicar la condición de frontera
void aplicar_condiciones_frontera(double* u) {
    u[0] = 0.0;  // Extremo izquierdo a 0°C
    u[N_x - 1] = 0.0;  // Extremo derecho a 0°C
}

// Función principal que resuelve la ecuación del calor en 1D (versión serial)
void resolver_ecuacion_calor(double* u, double* u_nueva, double dx, double dt) {
    // Coeficiente de estabilidad (Coeficiente de la fórmula)
    double coef = alpha * dt / (dx * dx);

    // Iteración temporal (bucle sobre t)
    for (int t = 0; t < N_t; t++) {
        // Iteración espacial (bucle sobre x)
        for (int i = 1; i < N_x - 1; i++) {
            // Diferencias finitas para calcular u en el siguiente paso temporal
            u_nueva[i] = u[i] + coef * (u[i+1] - 2 * u[i] + u[i-1]);
        }

        // Aplicar condiciones de frontera
        aplicar_condiciones_frontera(u_nueva);

        // Copiar la solución nueva en la antigua para el siguiente paso
        memcpy(u, u_nueva, N_x * sizeof(double));
    }
}

// Función principal
int main() {
    // Definir las variables necesarias
    double dx = L / (N_x - 1);  // Tamaño del paso espacial
    double dt = T_max / N_t;  // Tamaño del paso temporal

    // Crear dos arrays: uno para la solución actual y otro para la solución nueva
    double* u = (double*) malloc(N_x * sizeof(double));
    double* u_nueva = (double*) malloc(N_x * sizeof(double));

    // Inicializar la condición inicial (t=0)
    inicializar_condicion_inicial(u, dx);

    // Aplicar las condiciones de frontera en el tiempo t=0
    aplicar_condiciones_frontera(u);

    // Resolver la ecuación del calor
    resolver_ecuacion_calor(u, u_nueva, dx, dt);

    // Mostrar los resultados finales (temperatura en cada punto al final del tiempo)
    printf("Resultados de la simulación al tiempo T = %f:\n", T_max);
    for (int i = 0; i < N_x; i++) {
        double x = i * dx;
        printf("u(%f) = %f\n", x, u[i]);
    }

    // Liberar la memoria dinámica
    free(u);
    free(u_nueva);

    return 0;
}
