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
#define DB_FILENAME "platforms.txt"
#define SUCCESS 0
#define ERROR -1

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

/* Прототипы */
void load_existing_platforms(generative_ai_t* platforms, int* size);
void save_platforms_to_file(generative_ai_t* platforms, int size);
void add_platform(generative_ai_t* platforms, int* size);
void print_platforms(generative_ai_t* platforms, int size);
void search_platforms(generative_ai_t* platforms, int size);
void sort_platforms(generative_ai_t* platforms, int size);
void edit_platform(generative_ai_t* platforms, int size);
void delete_platform(generative_ai_t* platforms, int* size);

int main(void) {
    generative_ai_t platforms[MAX_PLATFORMS];
    int size = 0;
    int option = 0;

    system("chcp 1251");
    setlocale(LC_CTYPE, "Rus");

    printf("******************************************\n");
    printf("*  Файловая БД 'Generative AI платформы'  *\n");
    printf("*  Курсовой проект по дисциплине ОПИА     *\n");
    printf("******************************************\n\n");

    load_existing_platforms(platforms, &size);
    printf("Загружено записей: %d\n", size);

    while (1) {
        printf("\n---------------- Меню: ----------------\n");
        printf("1. Очистить список\n");
        printf("2. Добавить платформу\n");
        printf("3. Показать платформы\n");
        printf("4. Найти платформу\n");
        printf("5. Сортировать платформы\n");
        printf("6. Изменить платформу\n");
        printf("7. Удалить платформу\n");
        printf("8. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &option);
        getchar();

        switch (option) {
        case 1:
            size = 0;
            save_platforms_to_file(platforms, size);
            printf("\nСписок очищен.\n");
            break;
        case 2: add_platform(platforms, &size);     break;
        case 3: print_platforms(platforms, size);   break;
        case 4: search_platforms(platforms, size);  break;
        case 5: sort_platforms(platforms, size);    break;
        case 6: edit_platform(platforms, size);     break;
        case 7: delete_platform(platforms, &size);  break;
        case 8:
            printf("Выход из программы...\n");
            return SUCCESS;
        default:
            printf("Неверный выбор. Попробуйте ещё раз.\n");
        }
    }
    return SUCCESS;
}

/*
 * Загружает записи из файла в массив.
 * platforms - массив записей
 * size - счётчик загруженных записей
 */
void load_existing_platforms(generative_ai_t* platforms, int* size) {
    FILE* fpr = fopen(DB_FILENAME, "r");
    int count = 0;
    int rc = 0;

    if (fpr == NULL) {
        printf("Файл %s не найден.\n", DB_FILENAME);
        return;
    }

    while (count < MAX_PLATFORMS) {
        generative_ai_t temp;

        rc = fscanf(fpr, "Name: %29[^\n]\n", temp.name);
        if (rc != 1) break;
        rc = fscanf(fpr, "Generation types: %49[^\n]\n", temp.generation_types);
        if (rc != 1) break;
        rc = fscanf(fpr, "Quality: %f\n", &temp.quality);
        if (rc != 1) break;
        rc = fscanf(fpr, "Control level: %19[^\n]\n", temp.control_level);
        if (rc != 1) break;
        rc = fscanf(fpr, "Training data size: %f\n", &temp.training_data_size);
        if (rc != 1) break;
        rc = fscanf(fpr, "Compute requirements: %f\n", &temp.compute_requirements);
        if (rc != 1) break;
        rc = fscanf(fpr, "Cost: %f\n", &temp.cost);
        if (rc != 1) break;
        rc = fscanf(fpr, "Ethical certification: %d\n\n", &temp.ethical_certification);
        if (rc != 1) break;

        platforms[count] = temp;
        count++;
    }

    fclose(fpr);
    *size = count;
}

/*
 * Сохраняет массив записей в файл.
 * platforms - массив записей
 * size - количество записей
 */
void save_platforms_to_file(generative_ai_t* platforms, int size) {
    FILE* fpw = fopen(DB_FILENAME, "w");
    int i = 0;

    if (fpw == NULL) {
        printf("Ошибка записи файла %s.\n", DB_FILENAME);
        return;
    }

    for (i = 0; i < size; i++) {
        fprintf(fpw, "Name: %s\n", platforms[i].name);
        fprintf(fpw, "Generation types: %s\n", platforms[i].generation_types);
        fprintf(fpw, "Quality: %.1f\n", platforms[i].quality);
        fprintf(fpw, "Control level: %s\n", platforms[i].control_level);
        fprintf(fpw, "Training data size: %.1f\n", platforms[i].training_data_size);
        fprintf(fpw, "Compute requirements: %.1f\n", platforms[i].compute_requirements);
        fprintf(fpw, "Cost: %.1f\n", platforms[i].cost);
        fprintf(fpw, "Ethical certification: %d\n\n", platforms[i].ethical_certification);
    }

    fclose(fpw);
}

/*
 * Выводит список платформ на экран.
 * platforms - массив записей
 * size - количество записей
 */
void print_platforms(generative_ai_t* platforms, int size) {
    int i = 0;

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
        printf("    Этическая сертификация: %s\n",
            platforms[i].ethical_certification ? "да" : "нет");
    }

    printf("\n=== Конец списка ===\n");
}

/*
 * Добавляет новые платформы в массив и сохраняет в файл.
 * platforms - массив записей
 * size - счётчик записей
 */
void add_platform(generative_ai_t* platforms, int* size) {
    int count = 0;
    int i = 0;

    printf("\nСколько платформ добавить? ");
    scanf("%d", &count);
    getchar();

    for (i = 0; i < count; i++) {
        generative_ai_t temp;

        printf("\n--- Платформа %d из %d ---\n", i + 1, count);

        printf("Название (например ChatGPT): ");
        fgets(temp.name, sizeof(temp.name), stdin);
        temp.name[strcspn(temp.name, "\n")] = '\0';

        printf("Типы генерации (text / images / video / code, можно несколько через запятую): ");
        fgets(temp.generation_types, sizeof(temp.generation_types), stdin);
        temp.generation_types[strcspn(temp.generation_types, "\n")] = '\0';

        printf("Качество (0.0 - 10.0): ");
        scanf("%f", &temp.quality);
        getchar();

        printf("Контроль вывода (low / medium / high): ");
        fgets(temp.control_level, sizeof(temp.control_level), stdin);
        temp.control_level[strcspn(temp.control_level, "\n")] = '\0';

        printf("Размер данных в ТБ (например 100.0): ");
        scanf("%f", &temp.training_data_size);
        getchar();

        printf("Требования в GPU-часах (например 1200.0): ");
        scanf("%f", &temp.compute_requirements);
        getchar();

        printf("Стоимость в $/мес (например 20.0, 0 - бесплатно): ");
        scanf("%f", &temp.cost);
        getchar();

        printf("Этическая сертификация (1 - да, 0 - нет): ");
        scanf("%d", &temp.ethical_certification);
        getchar();

        platforms[*size] = temp;
        (*size)++;

        printf("Платформа добавлена.\n");
    }

    save_platforms_to_file(platforms, *size);
    printf("\nВсе изменения сохранены.\n");
}


void search_platforms(generative_ai_t* platforms, int size) { (void)platforms; (void)size; }
void sort_platforms(generative_ai_t* platforms, int size) { (void)platforms; (void)size; }
void edit_platform(generative_ai_t* platforms, int size) { (void)platforms; (void)size; }
void delete_platform(generative_ai_t* platforms, int* size) { (void)platforms; (void)size; }