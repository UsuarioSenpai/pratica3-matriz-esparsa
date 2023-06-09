#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#define MAX_USERS 100
#define MAX_ITEMS 100

typedef struct {
    int user_id;
    int item_id;
    int rating;
} Rating;

typedef struct {
    int num_users;
    int num_items;
    Rating ratings[MAX_USERS * MAX_ITEMS];
} SparseMatrix;

void initMatrix(SparseMatrix *matrix, int num_users, int num_items) {
    matrix->num_users = num_users;
    matrix->num_items = num_items;
}

void addRating(SparseMatrix *matrix, int user_id, int item_id, int rating) {
    Rating new_rating;
    new_rating.user_id = user_id;
    new_rating.item_id = item_id;
    new_rating.rating = rating;
    
    matrix->ratings[user_id * matrix->num_items + item_id] = new_rating;
}

int getRating(SparseMatrix *matrix, int user_id, int item_id) {
    return matrix->ratings[user_id * matrix->num_items + item_id].rating;
}

void loadMatrixFromFile(SparseMatrix *matrix, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    int num_users, num_items;
    fscanf(file, "%d %d", &num_users, &num_items);
    initMatrix(matrix, num_users, num_items);
    
    int user_id, item_id, rating;
    while (fscanf(file, "%d %d %d", &user_id, &item_id, &rating) != EOF) {
        addRating(matrix, user_id, item_id, rating);
    }
    
    fclose(file);
}

void printMatrix(SparseMatrix *matrix) {
    printf("User  Item  Rating\n");
    for (int i = 0; i < matrix->num_users; i++) {
        for (int j = 0; j < matrix->num_items; j++) {
            int rating = getRating(matrix, i, j);
            if (rating != 0) {
                printf("%-6d %-6d %-6d\n", i, j, rating);
            }
        }
    }
}

void recommendItems(SparseMatrix *matrix, int user_id) {
    printf("\nRecomendações para o usuário %d:\n", user_id);
    
    for (int item_id = 0; item_id < matrix->num_items; item_id++) {
        int rating = getRating(matrix, user_id, item_id);
        if (rating == 0) {
            printf("Item %d\n", item_id);
        }
    }
}

int main() {
    SparseMatrix matrix;
    const char *filename = "ratings.txt";
    
    loadMatrixFromFile(&matrix, filename);
    
    printf("\nMatriz Esparsa:\n");
    printMatrix(&matrix);
    
    int user_id;
    char continuar;
    
    do {
        printf("\nDigite o ID do usuário para obter recomendações: ");
        scanf("%d", &user_id);
        
        recommendItems(&matrix, user_id);
        
        printf("\nDeseja ver as recomendações para outro usuário? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');
    
    printf("\nPressione Enter para sair...");
    getchar();
    getchar();
    
    return 0;
}
