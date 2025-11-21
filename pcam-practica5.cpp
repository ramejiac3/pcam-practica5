#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <chrono>
#include <cstdlib>

#define N 100000000 // tamaño de la secuencia

using namespace std;

const std::string PATTERN = "ATGC";
const int P_LEN = PATTERN.length();

// Genera una secuencia de ADN simulada
void generate_sequence(vector<char>& seq) {
for (int i = 0; i < N - P_LEN; ++i) {
seq[i] = "ATGC"[rand() % 4];
}


// Insertar el patrón cerca del final para asegurar hallazgo
for (int i = 0; i < P_LEN; ++i) {
    seq[N - 10000 + i] = PATTERN[i];
}


}

// Ejecuta búsqueda con configuración indicada
void run_search(int num_threads, const char* schedule_type, int chunk_size) {


vector<char> dna_sequence(N);
generate_sequence(dna_sequence);

long long first_index = -1;
omp_set_num_threads(num_threads);

auto start = chrono::high_resolution_clock::now();

// Aplicar schedule según parámetro recibido
if (strcmp(schedule_type, "static") == 0) {

    #pragma omp parallel for schedule(static, chunk_size)
    for (int i = 0; i < N - P_LEN; ++i) {
        if (first_index != -1) continue;

        bool match = true;
        for (int j = 0; j < P_LEN; ++j) {
            if (dna_sequence[i + j] != PATTERN[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            #pragma omp critical
            {
                if (first_index == -1 || i < first_index) {
                    first_index = i;
                }
            }
        }
    }

} else if (strcmp(schedule_type, "dynamic") == 0) {

    #pragma omp parallel for schedule(dynamic, chunk_size)
    for (int i = 0; i < N - P_LEN; ++i) {
        if (first_index != -1) continue;

        bool match = true;
        for (int j = 0; j < P_LEN; ++j) {
            if (dna_sequence[i + j] != PATTERN[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            #pragma omp critical
            {
                if (first_index == -1 || i < first_index) {
                    first_index = i;
                }
            }
        }
    }

} else if (strcmp(schedule_type, "guided") == 0) {

    #pragma omp parallel for schedule(guided, chunk_size)
    for (int i = 0; i < N - P_LEN; ++i) {
        if (first_index != -1) continue;

        bool match = true;
        for (int j = 0; j < P_LEN; ++j) {
            if (dna_sequence[i + j] != PATTERN[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            #pragma omp critical
            {
                if (first_index == -1 || i < first_index) {
                    first_index = i;
                }
            }
        }
    }

} else if (strcmp(schedule_type, "auto") == 0) {

    #pragma omp parallel for schedule(auto)
    for (int i = 0; i < N - P_LEN; ++i) {
        if (first_index != -1) continue;

        bool match = true;
        for (int j = 0; j < P_LEN; ++j) {
            if (dna_sequence[i + j] != PATTERN[j]) {
                match = false;
                break;
            }
        }

        if (match) {
            #pragma omp critical
            {
                if (first_index == -1 || i < first_index) {
                    first_index = i;
                }
            }
        }
    }
}

auto end = chrono::high_resolution_clock::now();
chrono::duration<double> elapsed = end - start;

cout << "Hilos: " << num_threads
     << ", Schedule: " << schedule_type
     << " (" << chunk_size << ")"
     << ", Tiempo: " << elapsed.count() << " s"
     << ", Posición: " << first_index << endl;


}

int main() {


cout << "--- Busqueda de Patron en ADN (" << PATTERN << ") ---" << endl;

// Ejecutar pruebas con diferentes schedules
run_search(4, "static", 1000);
run_search(4, "dynamic", 1000);
run_search(4, "guided", 1000);
run_search(4, "auto", 1000);

return 0;


}
