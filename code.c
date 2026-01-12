#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>

#pragma pack(push, 1)

#define MAX_RECORDS 100
#define MAX_RESULTS 100

struct iot_platform {
    char name[50]; // Название платформы
    char protocols[100]; // Поддерживаемые протоколы
    int max_devices; // Кол-во устройств
    char analytics[100]; // Возможность аналитики
    int latency_ms; // Задержка связи
    char security[100]; // Уровень безопасности
    float cost_per_device; // Цена за платформу
    char scalability[100]; // Масштабируемость
};
#pragma pack(pop)

typedef struct iot_platform IoTPlatform;


// Прототипы функций
void print_menu();
int load_from_file(IoTPlatform platforms[], int* count, const char* filename);
int save_to_file(IoTPlatform platforms[], int count, const char* filename);
void view_all_records(IoTPlatform platforms[], int count);
int search_by_cost(IoTPlatform platforms[], int count, float min_val, float max_val, IoTPlatform results[], int* found_count);
int search_by_devices(IoTPlatform platforms[], int count, int min_val, int max_val, IoTPlatform results[], int* found_count);
int search_by_latency(IoTPlatform platforms[], int count, int min_val, int max_val, IoTPlatform results[], int* found_count);
void sort_records(IoTPlatform platforms[], int count);
int add_record(IoTPlatform platforms[], int* count);
int input_platform(IoTPlatform* p);
void print_platform(IoTPlatform p);
int compare_by_inv_log_devices(const void* a, const void* b);

// Функции сравнения для qsort
int compare_by_inv_log_devices(const void* a, const void* b);

int main() {
    system("chcp 1251");

    IoTPlatform platforms[MAX_RECORDS];
    IoTPlatform search_results[MAX_RESULTS];
    int count = 0;
    char filename[100];
    int choice;
    int result;
    int found_count;
    float min_val, max_val;
    int min_int, max_int;

    printf("=== Система управления IoT платформами ===\n");

    do {
        print_menu();
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Введите имя файла для загрузки: ");
            scanf("%s", filename);
            result = load_from_file(platforms, &count, filename);
            if (result == -1) {
                printf("Ошибка открытия файла для чтения!\n");
            }
            else if (result == 0) {
                printf("Файл пуст!\n");
            }
            else {
                printf("Загружено %d записей из файла %s\n", result, filename);
            }
            break;

        case 2:
            view_all_records(platforms, count);
            break;

        case 3:
            printf("\n=== Поиск по диапазону ===\n");
            printf("Выберите поле для поиска:\n");
            printf("1. Стоимость за устройство\n");
            printf("2. Максимальное количество устройств\n");
            printf("3. Задержка связи (мс)\n");
            printf("Ваш выбор: ");
            scanf("%d", &choice);

            if (choice < 1 || choice > 3) {
                printf("Неверный выбор поля!\n");
                break;
            }

            switch (choice) {
            case 1:
                printf("Введите минимальную стоимость: ");
                scanf("%f", &min_val);
                printf("Введите максимальную стоимость: ");
                scanf("%f", &max_val);

                if (min_val > max_val) {
                    printf("Ошибка: минимальная стоимость не может быть больше максимальной!\n");
                    break;
                }

                result = search_by_cost(platforms, count, min_val, max_val, search_results, &found_count);
                if (result == 0) {
                    printf("\n=== Результаты поиска (стоимость от %.2f до %.2f) ===\n", min_val, max_val);
                    if (found_count == 0) {
                        printf(" Записей по вашему запросу не найдено\n");
                    }
                    else {
                        printf(" Найдено записей: %d\n", found_count);
                        for (int i = 0; i < found_count; i++) {
                            printf("\n--- Запись %d ---", i + 1);
                            print_platform(search_results[i]);
                        }
                    }
                }
                break;

            case 2:
                printf("Введите минимальное количество устройств: ");
                scanf("%d", &min_int);
                printf("Введите максимальное количество устройств: ");
                scanf("%d", &max_int);

                result = search_by_devices(platforms, count, min_int, max_int, search_results, &found_count);
                if (result == 0) {
                    printf("\n=== Результаты поиска (устройства от %d до %d) ===\n", min_int, max_int);
                    if (found_count == 0) {
                        printf(" Записей по вашему запросу не найдено\n");
                    }
                    else {
                        printf(" Найдено записей: %d\n", found_count);
                        for (int i = 0; i < found_count; i++) {
                            printf("\n--- Запись %d ---", i + 1);
                            print_platform(search_results[i]);
                        }
                    }
                }
                break;

            case 3:
                printf("Введите минимальную задержку (мс): ");
                scanf("%d", &min_int);
                printf("Введите максимальную задержку (мс): ");
                scanf("%d", &max_int);

                result = search_by_latency(platforms, count, min_int, max_int, search_results, &found_count);
                if (result == 0) {
                    printf("\n=== Результаты поиска (задержка от %d до %d мс) ===\n", min_int, max_int);
                    if (found_count == 0) {
                        printf(" Записей по вашему запросу не найдено\n");
                    }
                    else {
                        printf(" Найдено записей: %d\n", found_count);
                        for (int i = 0; i < found_count; i++) {
                            printf("\n--- Запись %d ---", i + 1);
                            print_platform(search_results[i]);
                        }
                    }
                }
                break;
            }
            break;

        case 4:
            sort_records(platforms, count);
            break;

        case 5:
            result = add_record(platforms, &count);
            if (result == 1) {
                printf("Запись успешно добавлена!\n");
            }
            else {
                printf("Достигнуто максимальное количество записей (%d)!\n", MAX_RECORDS);
            }
            break;

        case 6:
            printf("Введите имя файла для сохранения: ");
            scanf("%s", filename);
            result = save_to_file(platforms, count, filename);
            if (result == -1) {
                printf("Ошибка открытия файла для записи!\n");
            }
            else if (result == 0) {
                printf("Нет данных для сохранения!\n");
            }
            else {
                printf("Сохранено %d записей в файл %s\n", result, filename);
            }
            break;

        case 7:
            printf("Выход из программы.\n");
            break;

        default:
            printf("Неверный выбор!\n");
        }
    } while (choice != 7);

    return 0;
}

// Менюшка
void print_menu() {
    printf("\n=== Меню ===\n");
    printf("1. Загрузка данных из файла\n");
    printf("2. Просмотр всех записей\n");
    printf("3. Поиск по заданному диапазону\n");
    printf("4. Сортировка по обратной величине логарифма количества устройств\n");
    printf("5. Добавление новой записи\n");
    printf("6. Сохранение данных в файл\n");
    printf("7. Выход\n");
}

// Работа с файлом
int load_from_file(IoTPlatform platforms[], int* count, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size == 0) {
        fclose(file);
        *count = 0;
        return 0;
    }

    int temp_count = file_size / sizeof(IoTPlatform);

    if (temp_count > MAX_RECORDS) {
        fclose(file);
        return -1;
    }

    size_t read_count = fread(platforms, sizeof(IoTPlatform), temp_count, file);
    fclose(file);

    if (read_count != temp_count) {
        return -1;
    }

    *count = temp_count;
    return temp_count;
}

// Функция сохранения файла
int save_to_file(IoTPlatform platforms[], int count, const char* filename) {
    if (count == 0) {
        return 0;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        return -1;
    }

    size_t written = fwrite(platforms, sizeof(IoTPlatform), count, file);
    fclose(file);

    if (written != count) {
        return -1;
    }

    return count;
}

// Показ записей
void view_all_records(IoTPlatform platforms[], int count) {
    if (count == 0) {
        printf("Нет записей для отображения\n");
        return;
    }

    printf("\n=== Все записи (%d) ===\n", count);
    for (int i = 0; i < count; i++) {
        printf("\n--- Запись %d ---", i + 1);
        print_platform(platforms[i]);
    }
}

// Поиск по диапазону
int search_by_cost(IoTPlatform platforms[], int count, float min_val, float max_val, IoTPlatform results[], int* found_count) {
    *found_count = 0;

    if (count == 0) {
        return 0;
    }

    for (int i = 0; i < count; i++) {
        if (platforms[i].cost_per_device >= min_val &&
            platforms[i].cost_per_device <= max_val) {
            results[*found_count] = platforms[i];
            (*found_count)++;
        }
    }

    return 0;
}

int search_by_devices(IoTPlatform platforms[], int count, int min_val, int max_val, IoTPlatform results[], int* found_count) {
    *found_count = 0;

    if (count == 0) {
        return 0;
    }

    for (int i = 0; i < count; i++) {
        if (platforms[i].max_devices >= min_val &&
            platforms[i].max_devices <= max_val) {
            results[*found_count] = platforms[i];
            (*found_count)++;
        }
    }

    return 0;
}

int search_by_latency(IoTPlatform platforms[], int count, int min_val, int max_val, IoTPlatform results[], int* found_count) {
    *found_count = 0;

    if (count == 0) {
        return 0;
    }

    for (int i = 0; i < count; i++) {
        if (platforms[i].latency_ms >= min_val &&
            platforms[i].latency_ms <= max_val) {
            results[*found_count] = platforms[i];
            (*found_count)++;
        }
    }

    return 0;
}

// Условие сортировки
int compare_by_inv_log_devices(const void* a, const void* b) {
    const IoTPlatform* platformA = (const IoTPlatform*)a;
    const IoTPlatform* platformB = (const IoTPlatform*)b;

    double inv_log_A = 1.0 / log(platformA->max_devices + 1);
    double inv_log_B = 1.0 / log(platformB->max_devices + 1);

    if (inv_log_A < inv_log_B) return -1;
    if (inv_log_A > inv_log_B) return 1;
    return 0;
}

// Статус сортировки
void sort_records(IoTPlatform platforms[], int count) {
    if (count == 0) {
        printf("Нет записей для сортировки\n");
        return;
    }

    qsort(platforms, count, sizeof(IoTPlatform), compare_by_inv_log_devices);
    printf("Платформы отсортированны\n");
}

// Статус записи
int add_record(IoTPlatform platforms[], int* count) {
    if (*count >= MAX_RECORDS) {
        return 0;
    }

    int result = input_platform(&platforms[*count]);
    if (result == 1) {
        (*count)++;
        return 1;
    }
    return 0;
}

// Создание записи платформы
int input_platform(IoTPlatform* p) {
    printf("\n=== Ввод данных новой платформы ===\n");

    printf("Название платформы: ");
    printf("\n>>> Введите название (Можно через пробел): ");
    scanf(" %[^\n]", p->name);

    printf("Поддерживаемые протоколы: ");
    printf("\n>>> Перечислите через запятую (например: MQTT, HTTP): ");
    scanf(" %[^\n]", p->protocols);

    printf("Максимальное количество устройств: ");
    printf("\n>>> Введите целое число: ");
    scanf("%d", &p->max_devices);

    printf("Возможности аналитики: ");
    printf("\n>>> Например: машинное обучение, статистика: ");
    scanf(" %[^\n]", p->analytics);

    printf("Задержка связи: ");
    printf("\n>>> Введите целое число (Задержка в мс): ");
    scanf("%d", &p->latency_ms);

    printf("Уровень безопасности:");
    printf("\n>>> Выберите цифру от 1 до 5:\n");
    printf("   1 - Базовая\n");
    printf("   2 - Стандартная\n");
    printf("   3 - Повышенная\n");
    printf("   4 - Высокая\n");
    printf("   5 - Максимальная\n");
    printf(">>> Ваш выбор (1-5): ");
    int security_level;
    scanf("%d", &security_level);

    switch (security_level) {
    case 1: strcpy(p->security, "Базовая"); break;
    case 2: strcpy(p->security, "Стандартная"); break;
    case 3: strcpy(p->security, "Повышенная"); break;
    case 4: strcpy(p->security, "Высокая"); break;
    case 5: strcpy(p->security, "Максимальная"); break;
    default:
        strcpy(p->security, "Не указано");
        break;
    }

    printf("Стоимость за устройство: ");
    printf("\n>>> Введите стоимость в долларах (например: 1.50, 2.00,): $");
    scanf("%f", &p->cost_per_device);

    printf("Уровень масштабируемости:");
    printf("\n>>> Выберите цифру от 1 до 5:\n");
    printf("   1 - Низкая (до 1.000 устройств)\n");
    printf("   2 - Средняя (до 10.000 устройств)\n");
    printf("   3 - Высокая (до 100.000 устройств)\n");
    printf("   4 - Очень высокая (до 1.000.000 устройств)\n");
    printf("   5 - Максимальная (более 1.000.000 устройств)\n");
    printf(">>> Ваш выбор (1-5): ");
    int scalability_level;
    scanf("%d", &scalability_level);

    switch (scalability_level) {
    case 1: strcpy(p->scalability, "Низкая"); break;
    case 2: strcpy(p->scalability, "Средняя"); break;
    case 3: strcpy(p->scalability, "Высокая"); break;
    case 4: strcpy(p->scalability, "Очень высокая"); break;
    case 5: strcpy(p->scalability, "Максимальная"); break;
    default:
        strcpy(p->scalability, "Не указано");
        break;
    }

    printf("\n>>> Данные введены успешно!\n");
    return 1;
}

// Вывод записи платформы
void print_platform(IoTPlatform p) {
    printf("\nПлатформа: %s\n", p.name);
    printf("  Протоколы: %s\n", p.protocols);
    printf("  Макс. устройств: %d\n", p.max_devices);
    printf("  Аналитика: %s\n", p.analytics);
    printf("  Задержка: %d мс\n", p.latency_ms);
    printf("  Безопасность: %s\n", p.security);
    printf("  Стоимость за устройство: $%.2f\n", p.cost_per_device);
    printf("  Масштабируемость: %s\n", p.scalability);
}