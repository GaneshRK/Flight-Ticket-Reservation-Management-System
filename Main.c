#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define NAME_LEN   50
#define DEST_LEN   50
#define DATA_FILE  "bookings.dat"

typedef struct Node {
    int ticketID;
    char name[NAME_LEN];
    int age;
    char gender;
    char destination[DEST_LEN];
    struct Node *next;
} Node;

Node *front = NULL;
Node *rear  = NULL;
int nextTicketID = 1001;

Node *create_node(const char *name, int age, char gender, const char *destination);
void enqueue(Node *nn);
void dequeue();
void cancel_by_id(int tid);
void display_all();
Node* search_by_id(int tid);
int count_bookings();
void save_to_file();
void load_from_file();
void free_queue();
void read_line(char *buffer, int length);
int main();

Node *create_node(const char *name, int age, char gender, const char *destination) {
    Node *nn = (Node *) malloc(sizeof(Node));
    if (!nn) {
        printf("Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    nn->ticketID = nextTicketID++;
    strncpy(nn->name, name, NAME_LEN - 1);
    nn->name[NAME_LEN - 1] = '\0';
    nn->age = age;
    nn->gender = gender;
    strncpy(nn->destination, destination, DEST_LEN - 1);
    nn->destination[DEST_LEN - 1] = '\0';
    nn->next = NULL;
    return nn;
}

void read_line(char *buffer, int length) {
    if (fgets(buffer, length, stdin)) {
        size_t ln = strlen(buffer) - 1;
        if (buffer[ln] == '\n') {
            buffer[ln] = '\0';
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}


void enqueue(Node *nn) {
    if (rear == NULL) {
        front = rear = nn;
    } else {
        rear->next = nn;
        rear = nn;
    }
    printf("Ticket booked successfully! Ticket ID: %d\n", nn->ticketID);
}

void dequeue() {
    if (front == NULL) {
        printf("Queue is empty. No customers to serve.\n");
        return;
    }
    Node *temp = front;
    printf("Serving Customer: Ticket ID %d, Name: %s\n", temp->ticketID, temp->name);
    front = front->next;
    if (front == NULL) {
        rear = NULL;
    }
    free(temp);
}

void cancel_by_id(int tid) {
    if (front == NULL) {
        printf("Queue is empty. Nothing to cancel.\n");
        return;
    }
    Node *curr = front, *prev = NULL;
    while (curr != NULL && curr->ticketID != tid) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL) {
        printf("Ticket ID %d not found.\n", tid);
        return;
    }
    if (prev == NULL) {
        front = curr->next;
        if (front == NULL) {
            rear = NULL;
        }
    } else {
        prev->next = curr->next;
        if (curr == rear) {
            rear = prev;
        }
    }
    printf("Ticket ID %d cancelled (Name: %s).\n", curr->ticketID, curr->name);
    free(curr);
}

void display_all() {
    if (front == NULL) {
        printf("No bookings currently.\n");
        return;
    }
    Node *temp = front;
    printf("\nCurrent Bookings:\n");
    printf("-------------------------------------------------------------\n");
    printf("| TicketID | Name                     | Age | G | Destination |\n");
    printf("-------------------------------------------------------------\n");
    while (temp != NULL) {
        printf("| %7d | %-24s | %3d | %c | %-11s |\n", temp->ticketID, temp->name, temp->age, temp->gender, temp->destination);
        temp = temp->next;
    }
    printf("-------------------------------------------------------------\n");
}

Node* search_by_id(int tid) {
    Node *temp = front;
    while (temp != NULL) {
        if (temp->ticketID == tid) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

int count_bookings() {
    int cnt = 0;
    Node *temp = front;
    while (temp != NULL) {
        cnt++;
        temp = temp->next;
    }
    return cnt;
}

void free_queue() {
    Node *temp;
    while (front != NULL) {
        temp = front;
        front = front->next;
        free(temp);
    }
    rear = NULL;
}

void save_to_file() {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (!fp) {
        printf("Warning: Could not open file '%s' to save data.\n", DATA_FILE);
        return;
    }
    fwrite(&nextTicketID, sizeof(int), 1, fp);
    Node *temp = front;
    while (temp != NULL) {
        fwrite(temp, sizeof(Node), 1, fp);
        temp = temp->next;
    }
    fclose(fp);
    printf("Bookings saved to file '%s'.\n", DATA_FILE);
}

void load_from_file() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        return;
    }

    if (fread(&nextTicketID, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return;
    }

    Node tempNode;
    free_queue();

    while (fread(&tempNode, sizeof(Node), 1, fp) == 1) {
        Node *nn = (Node *) malloc(sizeof(Node));
        if (!nn) {
            printf("Memory allocation failed during load!\n");
            fclose(fp);
            return;
        }
        *nn = tempNode;
        nn->next = NULL;

        if (rear == NULL) {
            front = rear = nn;
        } else {
            rear->next = nn;
            rear = nn;
        }
    }
    fclose(fp);
    printf("Bookings loaded from file '%s'.\n", DATA_FILE);
}

int main() {
    load_from_file();
    int choice;
    printf("\n--- Flight Ticket Reservation Management System ---\n");

    while (1) {
        printf("\nMenu:\n");
        printf("1) Book Ticket\n");
        printf("2) Serve Customer (Dequeue)\n");
        printf("3) Cancel Ticket by Ticket ID\n");
        printf("4) Display All Bookings\n");
        printf("5) Search by Ticket ID\n");
        printf("6) Count Bookings\n");
        printf("7) Save Bookings to File\n");
        printf("8) Exit (Auto-save)\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Please enter a number between 1-8.\n");
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (choice == 1) {
            char name[NAME_LEN], dest[DEST_LEN];
            int age;
            char gender_c;

            printf("Enter passenger name: ");
            read_line(name, NAME_LEN);

            printf("Enter age: ");
            if (scanf("%d", &age) != 1) {
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Invalid age entered. Booking cancelled.\n");
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Enter gender (M/F/O): ");
            gender_c = getchar();
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Enter destination: ");
            read_line(dest, DEST_LEN);

            Node *nn = create_node(name, age, gender_c, dest);
            enqueue(nn);

        } else if (choice == 2) {
            dequeue();

        } else if (choice == 3) {
            int tid;
            printf("Enter Ticket ID to cancel: ");
            if (scanf("%d", &tid) != 1) {
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Invalid ID.\n");
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);
            cancel_by_id(tid);

        } else if (choice == 4) {
            display_all();

        } else if (choice == 5) {
            int tid;
            printf("Enter Ticket ID to search: ");
            if (scanf("%d", &tid) != 1) {
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Invalid ID.\n");
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            Node *res = search_by_id(tid);
            if (res) {
                printf("Ticket Found:\nID: %d\nName: %s\nAge: %d\nGender: %c\nDestination: %s\n",
                       res->ticketID, res->name, res->age, res->gender, res->destination);
            } else {
                printf("Ticket ID %d not found.\n", tid);
            }

        } else if (choice == 6) {
            printf("Total bookings: %d\n", count_bookings());

        } else if (choice == 7) {
            save_to_file();

        } else if (choice == 8) {
            save_to_file();
            printf("Exiting... Summary:\nTotal bookings currently: %d\n", count_bookings());
            free_queue();
            break;

        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
