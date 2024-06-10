#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_MOVIES 100
#define MAX_TITLE_LENGTH 100
#define MAX_SEATS 60

void display_seats();
void reserve_seat();
void cancel_reservation();
void print_movies();


typedef struct {
    char title[MAX_TITLE_LENGTH];
} Movie;

Movie movies[MAX_MOVIES];
int num_movies = 0;

typedef struct {
    char name[50];
    int seat_number;
} Reservation;

Reservation reservations[MAX_SEATS];
int num_reservations = 0;

typedef struct {
    char name[50];
    float total_payment;
    int booking_number;
    int seat_number;
} Receipt;

int booking_counter = 1;

Receipt generate_receipt(const char *name, float total_payment, int seat_number) {
    Receipt receipt;
    strcpy(receipt.name, name);
    total_payment = 19.90;
    receipt.total_payment = total_payment;
    receipt.booking_number = booking_counter++;
    receipt.seat_number = seat_number;
    return receipt;
}

void print_receipt(const Receipt *receipt) {
    printf("Receipt\n");
    printf("--------\n");
    printf("Name: %s\n", receipt->name);
    printf("Total Payment: $%.2f\n", receipt->total_payment);
    printf("Booking Number: %d\n", receipt->booking_number);
    printf("Seat Number: %d\n", receipt->seat_number);
    printf("--------\n");
}

void load_movies_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    char line[MAX_TITLE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (num_movies >= MAX_MOVIES) {
            printf("Maximum number of movies reached.\n");
            break;
        }
        strtok(line, "\n"); // Remove trailing newline character
        strcpy(movies[num_movies].title, line);
        num_movies++;
    }

    fclose(file);
}

void main_menu() {
    int choice;
    int selected_movie_index;
    char name[50];
    int seat_number;
    do{ 
        printf("\n--- Cinema Reservation System ---\n");
        printf("1. Reserve a seat\n");
        printf("2. Choose a movie\n");
        printf("3. Cancel reservation\n");
        printf("4. Print receipt\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                system("cls");
                display_seats();
                printf("\nEnter your name: ");
                scanf("%s", name);
                system("cls");
                display_seats();
                printf("\nEnter seat number that you want %s: ", name);
                scanf("%d", &seat_number);
                reserve_seat(name, seat_number);
                system("cls");
                display_seats();
                sleep(1);
                break;
            case 2:
                system("cls");
                print_movies();
                printf("Choose a movie by entering its number: ");
                scanf("%d", &selected_movie_index);
                if (selected_movie_index >= 1 && selected_movie_index <= num_movies) {
                    printf("You have chosen: %s\n", movies[selected_movie_index - 1].title);
                } else {
                    printf("Invalid movie selection.\n");
                }
                sleep(1);
                system("cls");
                break;
            case 3:
                system("cls");
                display_seats();
                printf("Enter seat number to cancel reservation: ");
                scanf("%d", &seat_number);
                cancel_reservation(seat_number);
                sleep(1);
                break;
            case 4:
                if (selected_movie_index >= 1 && selected_movie_index <= num_movies) {
                    system("cls");
                    printf("Movie: %s\n", movies[selected_movie_index - 1].title);
                    // Assuming total_payment is collected somehow
                    float total_payment = 0.0; // Replace 0.0 with the actual total payment
                    Receipt receipt = generate_receipt(name, total_payment, seat_number);
                    print_receipt(&receipt);
                    printf("TAKE YOUR RECEIPT AND PLEASE PROCEED THE PAYMENT AT COUNTER, THANK YOU!!\n\n");
                    sleep(20);
                } else{
                    printf("Invalid movie selection.\n");
                    sleep(2);
                }
                break;
            case 5:
                printf("Exiting...\n");
                choice = 7;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                system("cls");
                break;
        }
    }while(choice != 7);
}

void reserve_seat(char name[], int seat_number) {
    if (num_reservations >= MAX_SEATS) {
        printf("Sorry, the cinema is fully booked.\n");
        return;
    }

    for (int i = 0; i < num_reservations; i++) {
        if (reservations[i].seat_number == seat_number) {
            printf("Sorry, seat %d is already booked.\n", seat_number);
            sleep(2);
            return;
        }
    }

    strcpy(reservations[num_reservations].name, name);
    reservations[num_reservations].seat_number = seat_number;
    num_reservations++;

    printf("Seat %d reserved for %s.\n", seat_number, name);
    sleep(2);
}

void cancel_reservation(int seat_number) {
    for (int i = 0; i < num_reservations; i++) {
        if (reservations[i].seat_number == seat_number) {
            printf("Reservation for seat %d canceled.\n", seat_number);
            // Remove reservation by shifting all elements after it
            for (int j = i; j < num_reservations - 1; j++) {
                reservations[j] = reservations[j + 1];
            }
            num_reservations--;
            return;
        }
    }

    printf("No reservation found for seat %d.\n", seat_number);
}

void display_seats() {
    printf("\t\t\t\tX = Seat already booked\n\n");
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
            printf("  ");
            for (int k = 0; k < 5; k++) {
                int seat_number = row * 3 * 5 + col * 5 + k + 1;
                int found = 0;
                for (int i = 0; i < num_reservations; i++) {
                    if (reservations[i].seat_number == seat_number) {
                        printf(" X%02d", seat_number);
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf(" 0%02d", seat_number);
                }
                if (k < 4) {
                    printf(" "); // Add space between seat numbers
                }
            }
            printf("  ");
        }
        printf("\n");
    }
}

void print_movies() {
    printf("Movie List:\n");
    for (int i = 0; i < num_movies; i++) {
        printf("%d. %s\n", i + 1, movies[i].title);
    }
}

int main() {
    load_movies_from_file("movies.txt");
    main_menu();
    return 0;
}
