/*
 * Шестакова Ирина
 * Группа: бИЦ-241
 *
 * Курсовой проект: файловая БД "Generative AI платформы"
 * Дисциплина: Основы программирования и алгоритмизации
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_PLATFORMS 100
#define FILENAME_LEN 256

 /* Запись о Generative AI платформе */
typedef struct {
    char name[30];                  /* название */
    char generation_types[50];      /* типы генерации */
    float quality;                  /* качество */
    char control_level[20];         /* контроль вывода */
    float training_data_size;       /* размер данных, ТБ */
    float compute_requirements;     /* требования, GPU-часы */
    float cost;                     /* стоимость, $/мес */
    int ethical_certification;      /* сертификация 0/1 */
} generative_ai_t;

/* Прототипы функций */
void load_existing_platforms(generative_ai_t* platforms, int* size, char* filename);
void save_platforms_to_file(generative_ai_t* platforms, int size, char* filename);
void add_platform(generative_ai_t* platforms, int* size, char* filename);
void print_platforms(generative_ai_t* platforms, int size);
void search_platforms(generative_ai_t* platforms, int size);
void sort_platforms(generative_ai_t* platforms, int size, char* filename);
void edit_platform(generative_ai_t* platforms, int size, char* filename);
void delete_platform(generative_ai_t* platforms, int* size, char* filename);

/* Функции сравнения для qsort */
int compare_by_quality(const void* a, const void* b);
int compare_by_cost(const void* a, const void* b);
int compare_by_inverse_square(const void* a, const void* b);

/*
 * Точка входа в программу.
 * Запрашивает имя файла, загружает данные и показывает меню.
 */
int main(void) {
    generative_ai_t platforms[MAX_PLATFORMS];
    char filename[FILENAME_LEN];
    int size = 0;
    int option = 0;

    system("chcp 1251");
    setlocale(LC_CTYPE, "Rus");

    printf("__________________________________________\n");
    printf("  Файловая БД 'Generative AI платформы'  \n");
    printf("  Курсовой проект по дисциплине ОПИА     \n");
    printf("__________________________________________\n\n");

    printf("Введите имя файла (например platforms.txt): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0) {
        strcpy(filename, "platforms.txt");
        printf("Используется файл по умолчанию: %s\n", filename);
    }

    load_existing_platforms(platforms, &size, filename);
    printf("Загружено записей: %d\n", size);

    while (1) {
        printf("\n---------------- Меню: ----------------\n");
        printf("1. Добавить платформу\n");
        printf("2. Показать платформы\n");
        printf("3. Найти платформу\n");
        printf("4. Сортировать платформы\n");
        printf("5. Изменить платформу\n");
        printf("6. Удалить платформу\n");
        printf("7. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &option);
        getchar();

        if (option < 1 || option > 7) {
            printf("Ошибка: выберите пункт от 1 до 7.\n");
            continue;
        }

        if (option == 1) add_platform(platforms, &size, filename);
        if (option == 2) print_platforms(platforms, size);
        if (option == 3) search_platforms(platforms, size);
        if (option == 4) sort_platforms(platforms, size, filename);
        if (option == 5) edit_platform(platforms, size, filename);
        if (option == 6) delete_platform(platforms, &size, filename);
        if (option == 7) {
            printf("Выход из программы...\n");
            return 0;
        }
    }
    return 0;
}

/*
 * Загружает записи из указанного файла в массив.
 * platforms - массив записей
 * size - счётчик загруженных записей
 * filename - имя файла для загрузки
 */
void load_existing_platforms(generative_ai_t* platforms, int* size, char* filename) {
    FILE* f;
    generative_ai_t temp;
    int count = 0;
    int rc = 0;

    f = fopen(filename, "r");
    if (f == NULL) {
        printf("Файл %s не найден.\n", filename);
        return;
    }

    while (count < MAX_PLATFORMS) {
        rc = fscanf(f, "Name: %29[^\n]\n", temp.name);
        if (rc != 1) break;
        rc = fscanf(f, "Generation types: %49[^\n]\n", temp.generation_types);
        if (rc != 1) break;
        rc = fscanf(f, "Quality: %f\n", &temp.quality);
        if (rc != 1) break;
        rc = fscanf(f, "Control level: %19[^\n]\n", temp.control_level);
        if (rc != 1) break;
        rc = fscanf(f, "Training data size: %f\n", &temp.training_data_size);
        if (rc != 1) break;
        rc = fscanf(f, "Compute requirements: %f\n", &temp.compute_requirements);
        if (rc != 1) break;
        rc = fscanf(f, "Cost: %f\n", &temp.cost);
        if (rc != 1) break;
        rc = fscanf(f, "Ethical certification: %d\n\n", &temp.ethical_certification);
        if (rc != 1) break;

        platforms[count] = temp;
        count = count + 1;
    }

    fclose(f);
    *size = count;
}

/*
 * Сохраняет массив записей в указанный файл.
 * platforms - массив записей
 * size - количество записей
 * filename - имя файла для сохранения
 */
void save_platforms_to_file(generative_ai_t* platforms, int size, char* filename) {
    FILE* f;
    int i;

    f = fopen(filename, "w");
    if (f == NULL) {
        printf("Ошибка записи файла %s.\n", filename);
        return;
    }

    for (i = 0; i < size; i++) {
        fprintf(f, "Name: %s\n", platforms[i].name);
        fprintf(f, "Generation types: %s\n", platforms[i].generation_types);
        fprintf(f, "Quality: %.1f\n", platforms[i].quality);
        fprintf(f, "Control level: %s\n", platforms[i].control_level);
        fprintf(f, "Training data size: %.1f\n", platforms[i].training_data_size);
        fprintf(f, "Compute requirements: %.1f\n", platforms[i].compute_requirements);
        fprintf(f, "Cost: %.1f\n", platforms[i].cost);
        fprintf(f, "Ethical certification: %d\n\n", platforms[i].ethical_certification);
    }

    fclose(f);
}

/*
 * Выводит список платформ на экран.
 * platforms - массив записей
 * size - количество записей
 */
void print_platforms(generative_ai_t* platforms, int size) {
    int i;

    if (size == 0) {
        printf("\nСписок пуст.\n");
        return;
    }

    printf("\n=== Список платформ (%d) ===\n", size);

    for (i = 0; i < size; i++) {
        printf("\n[%d] %s\n", i + 1, platforms[i].name);
        printf("    Типы генерации: %s\n", platforms[i].generation_types);
        printf("    Качество: %.1f\n", platforms[i].quality);
        printf("    Контроль вывода: %s\n", platforms[i].control_level);
        printf("    Размер данных: %.1f ТБ\n", platforms[i].training_data_size);
        printf("    Требования: %.1f GPU-часов\n", platforms[i].compute_requirements);
        printf("    Стоимость: %.1f $/мес\n", platforms[i].cost);
        if (platforms[i].ethical_certification == 1)
            printf("    Этическая сертификация: да\n");
        else
            printf("    Этическая сертификация: нет\n");
    }

    printf("\n=== Конец списка ===\n");
}

/*
 * Добавляет новые платформы в массив.
 * Сначала вводит данные во временный массив,
 * потом показывает и спрашивает о сохранении.
 * platforms - массив записей
 * size - счётчик записей
 * filename - имя файла для сохранения
 */
void add_platform(generative_ai_t* platforms, int* size, char* filename) {
    generative_ai_t temp[MAX_PLATFORMS];
    char copy[50];
    char* word;
    int count = 0;
    int i;
    int save = 0;
    int ok = 0;

    while (1) {
        printf("\nСколько платформ добавить? ");
        scanf("%d", &count);
        getchar();
        if (count < 1) {
            printf("Ошибка: количество должно быть больше 0. Повторите.\n");
            continue;
        }
        if (*size + count > MAX_PLATFORMS) {
            printf("Ошибка: не хватает места. Свободно: %d. Повторите.\n", MAX_PLATFORMS - *size);
            continue;
        }
        break;
    }

    for (i = 0; i < count; i++) {
        printf("\n--- Платформа %d из %d ---\n", i + 1, count);

        while (1) {
            printf("Название: ");
            fgets(temp[i].name, sizeof(temp[i].name), stdin);
            temp[i].name[strcspn(temp[i].name, "\n")] = '\0';
            if (strlen(temp[i].name) == 0) {
                printf("Ошибка: название пустое. Повторите.\n");
                continue;
            }
            break;
        }

        while (1) {
            printf("Типы генерации (текст / изображения / видео / код, через запятую): ");
            fgets(temp[i].generation_types, sizeof(temp[i].generation_types), stdin);
            temp[i].generation_types[strcspn(temp[i].generation_types, "\n")] = '\0';

            strcpy(copy, temp[i].generation_types);
            ok = 1;
            word = strtok(copy, ", ");
            while (word != NULL) {
                if (strcmp(word, "текст") == 0) {}
                else if (strcmp(word, "изображения") == 0) {}
                else if (strcmp(word, "видео") == 0) {}
                else if (strcmp(word, "код") == 0) {}
                else {
                    ok = 0;
                    break;
                }
                word = strtok(NULL, ", ");
            }

            if (ok == 0) {
                printf("Ошибка: введите хотя бы одно из: текст, изображения, видео, код. Повторите.\n");
                continue;
            }
            break;
        }

        while (1) {
            printf("Качество (0 - 10): ");
            scanf("%f", &temp[i].quality);
            getchar();
            if (temp[i].quality < 0 || temp[i].quality > 10) {
                printf("Ошибка: качество должно быть от 0 до 10. Повторите.\n");
                continue;
            }
            break;
        }

        while (1) {
            printf("Контроль вывода (низкий / средний / высокий): ");
            fgets(temp[i].control_level, sizeof(temp[i].control_level), stdin);
            temp[i].control_level[strcspn(temp[i].control_level, "\n")] = '\0';

            if (strcmp(temp[i].control_level, "низкий") == 0 ||
                strcmp(temp[i].control_level, "средний") == 0 ||
                strcmp(temp[i].control_level, "высокий") == 0) {
                break;
            }
            printf("Ошибка: введите низкий, средний или высокий. Повторите.\n");
        }

        while (1) {
            printf("Размер данных в ТБ: ");
            scanf("%f", &temp[i].training_data_size);
            getchar();
            if (temp[i].training_data_size < 0) {
                printf("Ошибка: размер не может быть отрицательным. Повторите.\n");
                continue;
            }
            break;
        }

        while (1) {
            printf("Требования в GPU-часах: ");
            scanf("%f", &temp[i].compute_requirements);
            getchar();
            if (temp[i].compute_requirements < 0) {
                printf("Ошибка: требования не могут быть отрицательными. Повторите.\n");
                continue;
            }
            break;
        }

        while (1) {
            printf("Стоимость в $/мес: ");
            scanf("%f", &temp[i].cost);
            getchar();
            if (temp[i].cost < 0) {
                printf("Ошибка: стоимость не может быть отрицательной. Повторите.\n");
                continue;
            }
            break;
        }

        while (1) {
            printf("Этическая сертификация (1 - да, 0 - нет): ");
            scanf("%d", &temp[i].ethical_certification);
            getchar();
            if (temp[i].ethical_certification != 0 && temp[i].ethical_certification != 1) {
                printf("Ошибка: сертификация должна быть 0 или 1. Повторите.\n");
                continue;
            }
            break;
        }
    }

    printf("\nПрименить и сохранить? (1 - да, 0 - нет): ");
    scanf("%d", &save);
    getchar();

    if (save != 1) {
        printf("Добавление отменено.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        platforms[*size] = temp[i];
        *size = *size + 1;
    }

    save_platforms_to_file(platforms, *size, filename);
    printf("Изменения сохранены.\n");
}

/*
 * Изменяет запись по номеру.
 * Сначала показывает выбранную запись полностью,
 * потом вводит новые данные во временную переменную,
 * показывает и спрашивает о сохранении.
 * platforms - массив записей
 * size - количество записей
 * filename - имя файла для сохранения
 */
void edit_platform(generative_ai_t* platforms, int size, char* filename) {
    generative_ai_t temp;
    char copy[50];
    char* word;
    int index = 0;
    int i;
    int save = 0;
    int ok = 0;

    if (size == 0) {
        printf("\nСписок пуст.\n");
        return;
    }

    printf("\nСписок доступных записей:\n");
    for (i = 0; i < size; i++) {
        printf("%d. %s\n", i + 1, platforms[i].name);
    }

    while (1) {
        printf("\nВведите номер для изменения: ");
        scanf("%d", &index);
        getchar();
        if (index < 1 || index > size) {
            printf("Ошибка: номер от 1 до %d. Повторите.\n", size);
            continue;
        }
        break;
    }

    index = index - 1;

    /* Показываем выбранную запись полностью */
    printf("\n=== Изменяемая запись ===\n");
    printf("Название: %s\n", platforms[index].name);
    printf("Типы генерации: %s\n", platforms[index].generation_types);
    printf("Качество: %.1f\n", platforms[index].quality);
    printf("Контроль вывода: %s\n", platforms[index].control_level);
    printf("Размер данных: %.1f ТБ\n", platforms[index].training_data_size);
    printf("Требования: %.1f GPU-часов\n", platforms[index].compute_requirements);
    printf("Стоимость: %.1f $/мес\n", platforms[index].cost);
    if (platforms[index].ethical_certification == 1)
        printf("Этическая сертификация: да\n");
    else
        printf("Этическая сертификация: нет\n");

    while (1) {
        printf("\nНовое название: ");
        fgets(temp.name, sizeof(temp.name), stdin);
        temp.name[strcspn(temp.name, "\n")] = '\0';
        if (strlen(temp.name) == 0) {
            printf("Ошибка: название пустое. Повторите.\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Новые типы генерации (текст / изображения / видео / код): ");
        fgets(temp.generation_types, sizeof(temp.generation_types), stdin);
        temp.generation_types[strcspn(temp.generation_types, "\n")] = '\0';

        strcpy(copy, temp.generation_types);
        ok = 1;
        word = strtok(copy, ", ");
        while (word != NULL) {
            if (strcmp(word, "текст") == 0) {}
            else if (strcmp(word, "изображения") == 0) {}
            else if (strcmp(word, "видео") == 0) {}
            else if (strcmp(word, "код") == 0) {}
            else {
                ok = 0;
                break;
            }
            word = strtok(NULL, ", ");
        }

        if (ok == 0) {
            printf("Ошибка: введите хотя бы одно из: текст, изображения, видео, код. Повторите.\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Новое качество (0 - 10): ");
        scanf("%f", &temp.quality);
        getchar();
        if (temp.quality < 0 || temp.quality > 10) {
            printf("Ошибка: качество от 0 до 10. Повторите.\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Новый контроль (низкий / средний / высокий): ");
        fgets(temp.control_level, sizeof(temp.control_level), stdin);
        temp.control_level[strcspn(temp.control_level, "\n")] = '\0';

        if (strcmp(temp.control_level, "низкий") == 0 ||
            strcmp(temp.control_level, "средний") == 0 ||
            strcmp(temp.control_level, "высокий") == 0) {
            break;
        }
        printf("Ошибка: введите низкий, средний или высокий. Повторите.\n");
    }

    while (1) {
        printf("Новый размер данных: ");
        scanf("%f", &temp.training_data_size);
        getchar();
        if (temp.training_data_size < 0) {
            printf("Ошибка: размер отрицательный. Повторите.\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Новые требования: ");
        scanf("%f", &temp.compute_requirements);
        getchar();
        if (temp.compute_requirements < 0) {
            printf("Ошибка: требования отрицательные. Повторите.\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Новая стоимость: ");
        scanf("%f", &temp.cost);
        getchar();
        if (temp.cost < 0) {
            printf("Ошибка: стоимость отрицательная. Повторите.\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Новая сертификация (1 - да, 0 - нет): ");
        scanf("%d", &temp.ethical_certification);
        getchar();
        if (temp.ethical_certification != 0 && temp.ethical_certification != 1) {
            printf("Ошибка: сертификация 0 или 1. Повторите.\n");
            continue;
        }
        break;
    }

    printf("\nПрименить и сохранить? (1 - да, 0 - нет): ");
    scanf("%d", &save);
    getchar();

    if (save != 1) {
        printf("Изменение отменено.\n");
        return;
    }

    platforms[index] = temp;
    save_platforms_to_file(platforms, size, filename);
    printf("Изменения сохранены.\n");
}

/*
 * Удаляет запись по номеру.
 * Сначала подтверждение, потом удаление и сохранение.
 * platforms - массив записей
 * size - счётчик записей
 * filename - имя файла для сохранения
 */
void delete_platform(generative_ai_t* platforms, int* size, char* filename) {
    int index = 0;
    int i;
    int save = 0;

    if (*size == 0) {
        printf("\nСписок пуст.\n");
        return;
    }

    printf("\nСписок доступных записей:\n");
    for (i = 0; i < *size; i++) {
        printf("%d. %s\n", i + 1, platforms[i].name);
    }

    while (1) {
        printf("\nВведите номер для удаления: ");
        scanf("%d", &index);
        getchar();
        if (index < 1 || index > *size) {
            printf("Ошибка: номер от 1 до %d. Повторите.\n", *size);
            continue;
        }
        break;
    }

    index = index - 1;

    /* Показываем выбранную запись полностью */
    printf("\n=== Удаляемая запись ===\n");
    printf("Название: %s\n", platforms[index].name);
    printf("Типы генерации: %s\n", platforms[index].generation_types);
    printf("Качество: %.1f\n", platforms[index].quality);
    printf("Контроль вывода: %s\n", platforms[index].control_level);
    printf("Размер данных: %.1f ТБ\n", platforms[index].training_data_size);
    printf("Требования: %.1f GPU-часов\n", platforms[index].compute_requirements);
    printf("Стоимость: %.1f $/мес\n", platforms[index].cost);
    if (platforms[index].ethical_certification == 1)
        printf("Этическая сертификация: да\n");
    else
        printf("Этическая сертификация: нет\n");

    printf("\nУдалить запись? (1 - да, 0 - нет): ");
    scanf("%d", &save);
    getchar();

    if (save != 1) {
        printf("Удаление отменено.\n");
        return;
    }

    for (i = index; i < *size - 1; i++) {
        platforms[i] = platforms[i + 1];
    }
    *size = *size - 1;

    save_platforms_to_file(platforms, *size, filename);
    printf("Запись удалена.\n");
}

/*
 * Ищет платформы по выбранному критерию.
 * platforms - массив записей
 * size - количество записей
 */
void search_platforms(generative_ai_t* platforms, int size) {
    int option = 0;
    int i;
    int found = 0;
    char text[50];
    float min_quality = 0;

    if (size == 0) {
        printf("\nСписок пуст.\n");
        return;
    }

    printf("\n---------------- Поиск: ----------------\n");
    printf("1. По типу генерации\n");
    printf("2. По названию\n");
    printf("3. По минимальному качеству\n");

    while (1) {
        printf("Ваш выбор: ");
        scanf("%d", &option);
        getchar();
        if (option < 1 || option > 3) {
            printf("Ошибка: выбор от 1 до 3. Повторите.\n");
            continue;
        }
        break;
    }

    if (option == 1) {
        printf("Введите тип (текст / изображения / видео / код): ");
        fgets(text, sizeof(text), stdin);
        text[strcspn(text, "\n")] = '\0';

        printf("\n=== Результаты ===\n");
        for (i = 0; i < size; i++) {
            if (strstr(platforms[i].generation_types, text) != NULL) {
                printf("\n[%d] %s | Качество: %.1f | Стоимость: %.1f\n",
                    i + 1, platforms[i].name, platforms[i].quality, platforms[i].cost);
                found = 1;
            }
        }
    }

    if (option == 2) {
        printf("Введите часть названия: ");
        fgets(text, sizeof(text), stdin);
        text[strcspn(text, "\n")] = '\0';

        printf("\n=== Результаты ===\n");
        for (i = 0; i < size; i++) {
            if (strstr(platforms[i].name, text) != NULL) {
                printf("\n[%d] %s | Качество: %.1f | Стоимость: %.1f\n",
                    i + 1, platforms[i].name, platforms[i].quality, platforms[i].cost);
                found = 1;
            }
        }
    }

    if (option == 3) {
        while (1) {
            printf("Минимальное качество (0 - 10): ");
            scanf("%f", &min_quality);
            getchar();
            if (min_quality < 0 || min_quality > 10) {
                printf("Ошибка: качество от 0 до 10. Повторите.\n");
                continue;
            }
            break;
        }

        printf("\n=== Результаты ===\n");
        for (i = 0; i < size; i++) {
            if (platforms[i].quality >= min_quality) {
                printf("\n[%d] %s | Качество: %.1f | Стоимость: %.1f\n",
                    i + 1, platforms[i].name, platforms[i].quality, platforms[i].cost);
                found = 1;
            }
        }
    }

    if (found == 0) {
        printf("\nНичего не найдено.\n");
    }
}

/*
 * Сортирует массив, выводит на экран и спрашивает о сохранении.
 * platforms - массив записей
 * size - количество записей
 * filename - имя файла для сохранения
 */
void sort_platforms(generative_ai_t* platforms, int size, char* filename) {
    int option = 0;
    int save = 0;

    if (size == 0) {
        printf("\nСписок пуст.\n");
        return;
    }

    printf("\n---------------- Сортировка: ----------------\n");
    printf("1. По качеству (возрастание)\n");
    printf("2. По стоимости (возрастание)\n");
    printf("3. По обратному квадрату размера данных\n");

    while (1) {
        printf("Ваш выбор: ");
        scanf("%d", &option);
        getchar();
        if (option < 1 || option > 3) {
            printf("Ошибка: выбор от 1 до 3. Повторите.\n");
            continue;
        }
        break;
    }

    if (option == 1) {
        qsort(platforms, size, sizeof(generative_ai_t), compare_by_quality);
        printf("\nОтсортировано по качеству.\n");
    }

    if (option == 2) {
        qsort(platforms, size, sizeof(generative_ai_t), compare_by_cost);
        printf("\nОтсортировано по стоимости.\n");
    }

    if (option == 3) {
        qsort(platforms, size, sizeof(generative_ai_t), compare_by_inverse_square);
        printf("\nОтсортировано по обратному квадрату.\n");
    }

    print_platforms(platforms, size);

    printf("\nСохранить в файл? (1 - да, 0 - нет): ");
    scanf("%d", &save);
    getchar();

    if (save == 1) {
        save_platforms_to_file(platforms, size, filename);
        printf("Изменения сохранены.\n");
    }
    else {
        printf("Изменения не сохранены.\n");
    }
}

/*
 * Функция сравнения: по качеству (возрастание).
 */
int compare_by_quality(const void* a, const void* b) {
    generative_ai_t* pA = (generative_ai_t*)a;
    generative_ai_t* pB = (generative_ai_t*)b;

    if (pA->quality < pB->quality) return -1;
    if (pA->quality > pB->quality) return 1;
    return 0;
}

/*
 * Функция сравнения: по стоимости (возрастание).
 */
int compare_by_cost(const void* a, const void* b) {
    generative_ai_t* pA = (generative_ai_t*)a;
    generative_ai_t* pB = (generative_ai_t*)b;

    if (pA->cost < pB->cost) return -1;
    if (pA->cost > pB->cost) return 1;
    return 0;
}

/*
 * Функция сравнения: по обратному квадрату размера тренировочных данных.
 * Чем меньше размер данных, тем выше приоритет.
 */
int compare_by_inverse_square(const void* a, const void* b) {
    generative_ai_t* pA = (generative_ai_t*)a;
    generative_ai_t* pB = (generative_ai_t*)b;
    float valA;
    float valB;

    valA = 1.0 / (pA->training_data_size * pA->training_data_size);
    valB = 1.0 / (pB->training_data_size * pB->training_data_size);

    if (valA > valB) return -1;
    if (valA < valB) return 1;
    return 0;
}