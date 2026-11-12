#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 100

/* Structure for passenger booking details */
struct Booking {
    int bookingID;
    char passengerName[50];
    int age;
    char gender[15];
    char source[30];
    char destination[30];
    char flightDate[20];
    char seatClass[20];
    float ticketPrice;
};

/* Global variables */
struct Booking bookings[MAX];
int count = 0;
int nextID = 1001;

/* Function declarations */
void readLine(char message[], char text[], int size);
int readInt(char message[], int min, int max);

void selectDestination(char destination[]);
float calculateFare(const char destination[], const char seatClass[]);
float calculateBookingPrice(const char destination[],
                            const char seatClass[],
                            const char flightDate[]);

void selectClass(struct Booking *b);

void bookTicket(void);
void displayBookings(void);
int findBooking(int id);
void searchBooking(void);
void updateBooking(void);
void cancelBooking(void);

void sortBookings(void);
void generateReport(void);

void saveToFile(void);
void loadFromFile(void);


/* STUDENT 1: INPUT FUNCTIONS */

void readLine(char message[], char text[], int size) {
    printf("%s", message);

    if (fgets(text, size, stdin) != NULL) {
        if (strchr(text, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        text[strcspn(text, "\n")] = '\0';
    }
    else {
        text[0] = '\0';
    }
}

int readInt(char message[], int min, int max) {
    char input[100];
    char *end;
    long number;

    while (1) {
        readLine(message, input, sizeof(input));
        number = strtol(input, &end, 10);

        while (*end == ' ' || *end == '\t') {
            end++;
        }

        if (end != input && *end == '\0' &&
            number >= min && number <= max) {
            return (int)number;
        }

        printf("Invalid input! Enter a number between %d and %d.\n",
               min, max);
    }
}


/* STUDENT 2: MAIN MENU AND BOOKING */

int main(void) {
    int choice;

    loadFromFile();

    do {
        printf("\n============================================\n");
        printf("       AIRLINE TICKET BOOKING SYSTEM        \n");
        printf("============================================\n");
        printf("1. Book a Ticket\n");
        printf("2. Display All Bookings\n");
        printf("3. Search Booking\n");
        printf("4. Update Booking\n");
        printf("5. Cancel Booking\n");
        printf("6. Sort Bookings\n");
        printf("7. Generate Report\n");
        printf("8. Exit\n");
        printf("============================================\n");

        choice = readInt("Enter your choice: ", 1, 8);

        switch (choice) {
            case 1: bookTicket(); break;
            case 2: displayBookings(); break;
            case 3: searchBooking(); break;
            case 4: updateBooking(); break;
            case 5: cancelBooking(); break;
            case 6: sortBookings(); break;
            case 7: generateReport(); break;
            case 8:
                saveToFile();
                printf("\nThank you for using our system!\n");
                break;
        }
    } while (choice != 8);

    return 0;
}


/* Select destination */
void selectDestination(char destination[]) {
    int choice;

    printf("\nAvailable Destinations:\n");
    printf("1. Dubai\n");
    printf("2. Saudi Arabia\n");
    printf("3. Turkey\n");
    printf("4. America\n");

    choice = readInt("Select destination: ", 1, 4);

    switch (choice) {
        case 1: strcpy(destination, "Dubai"); break;
        case 2: strcpy(destination, "Saudi Arabia"); break;
        case 3: strcpy(destination, "Turkey"); break;
        case 4: strcpy(destination, "America"); break;
    }
}


/* Calculate base fare according to destination and seat class */
float calculateFare(const char destination[], const char seatClass[]) {
    if (strcmp(destination, "Dubai") == 0) {
        if (strcmp(seatClass, "Economy") == 0) return 30000;
        else if (strcmp(seatClass, "Business") == 0) return 60000;
        else if (strcmp(seatClass, "First Class") == 0) return 100000;
    }
    else if (strcmp(destination, "Saudi Arabia") == 0) {
        if (strcmp(seatClass, "Economy") == 0) return 40000;
        else if (strcmp(seatClass, "Business") == 0) return 80000;
        else if (strcmp(seatClass, "First Class") == 0) return 130000;
    }
    else if (strcmp(destination, "Turkey") == 0) {
        if (strcmp(seatClass, "Economy") == 0) return 65000;
        else if (strcmp(seatClass, "Business") == 0) return 120000;
        else if (strcmp(seatClass, "First Class") == 0) return 180000;
    }
    else if (strcmp(destination, "America") == 0) {
        if (strcmp(seatClass, "Economy") == 0) return 150000;
        else if (strcmp(seatClass, "Business") == 0) return 250000;
        else if (strcmp(seatClass, "First Class") == 0) return 400000;
    }

    return 0;
}


/* Add 20% to the base fare during vacation months:
   December, January, June and July. */
float calculateBookingPrice(const char destination[],
                            const char seatClass[],
                            const char flightDate[]) {
    int day, month, year;
    float fare;

    fare = calculateFare(destination, seatClass);

    if (sscanf(flightDate, "%d/%d/%d", &day, &month, &year) == 3) {
        if (month == 12 || month == 1 || month == 6 || month == 7) {
            fare = fare * 1.20;
        }
    }

    return fare;
}


/* Select ticket class using structure pointer */
void selectClass(struct Booking *b) {
    int choice;

    printf("\nSelect Seat Class:\n");
    printf("1. Economy\n");
    printf("2. Business\n");
    printf("3. First Class\n");

    choice = readInt("Enter your choice: ", 1, 3);

    switch (choice) {
        case 1: strcpy(b->seatClass, "Economy"); break;
        case 2: strcpy(b->seatClass, "Business"); break;
        case 3: strcpy(b->seatClass, "First Class"); break;
    }

    b->ticketPrice = calculateBookingPrice(
        b->destination, b->seatClass, b->flightDate
    );

    printf("\nSelected Seat Class: %s\n", b->seatClass);
    printf("Ticket Price: Rs. %.2f\n", b->ticketPrice);
}


/* Add new booking */
void bookTicket(void) {
    struct Booking b;

    if (count >= MAX) {
        printf("\nBooking limit reached!\n");
        return;
    }

    printf("\n========== BOOK A TICKET ==========\n");
    b.bookingID = nextID++;

    readLine("Enter passenger name: ",
             b.passengerName, sizeof(b.passengerName));

    if (strlen(b.passengerName) == 0) {
        printf("Passenger name cannot be empty!\n");
        nextID--;
        return;
    }

    b.age = readInt("Enter age: ", 1, 120);

    readLine("Enter gender: ", b.gender, sizeof(b.gender));

    if (strlen(b.gender) == 0) {
        printf("Gender cannot be empty!\n");
        nextID--;
        return;
    }

    strcpy(b.source, "Pakistan");
    printf("\nDeparture Country: Pakistan\n");

    selectDestination(b.destination);

    readLine("Enter flight date (DD/MM/YYYY): ",
             b.flightDate, sizeof(b.flightDate));

    if (strlen(b.flightDate) == 0) {
        printf("Flight date cannot be empty!\n");
        nextID--;
        return;
    }

    selectClass(&b);

    bookings[count] = b;
    count++;

    saveToFile();

    printf("\nTicket booked successfully!\n");
    printf("Your Booking ID is: %d\n", b.bookingID);
}


/* STUDENT 3: DISPLAY, SEARCH, UPDATE AND DELETE */

void displayBookings(void) {
    int i;

    if (count == 0) {
        printf("\nNo bookings available!\n");
        return;
    }

    printf("\n========== ALL BOOKINGS ==========\n");

    for (i = 0; i < count; i++) {
        printf("\nBooking ID: %d\n", bookings[i].bookingID);
        printf("Passenger Name: %s\n", bookings[i].passengerName);
        printf("Age: %d\n", bookings[i].age);
        printf("Gender: %s\n", bookings[i].gender);
        printf("From: %s\n", bookings[i].source);
        printf("To: %s\n", bookings[i].destination);
        printf("Flight Date: %s\n", bookings[i].flightDate);
        printf("Seat Class: %s\n", bookings[i].seatClass);
        printf("Ticket Price: Rs. %.2f\n", bookings[i].ticketPrice);
        printf("----------------------------------\n");
    }
}

int findBooking(int id) {
    int i;

    for (i = 0; i < count; i++) {
        if (bookings[i].bookingID == id) {
            return i;
        }
    }

    return -1;
}

void searchBooking(void) {
    int id, index;

    id = readInt("Enter Booking ID to search: ", 1, 999999);
    index = findBooking(id);

    if (index == -1) {
        printf("\nBooking not found!\n");
    }
    else {
        printf("\n========== BOOKING FOUND ==========\n");
        printf("Booking ID: %d\n", bookings[index].bookingID);
        printf("Passenger Name: %s\n", bookings[index].passengerName);
        printf("Age: %d\n", bookings[index].age);
        printf("Gender: %s\n", bookings[index].gender);
        printf("From: %s\n", bookings[index].source);
        printf("To: %s\n", bookings[index].destination);
        printf("Flight Date: %s\n", bookings[index].flightDate);
        printf("Seat Class: %s\n", bookings[index].seatClass);
        printf("Ticket Price: Rs. %.2f\n", bookings[index].ticketPrice);
    }
}


/* Update booking */
void updateBooking(void) {
    int id, index, choice;

    id = readInt("Enter Booking ID to update: ", 1, 999999);
    index = findBooking(id);

    if (index == -1) {
        printf("\nBooking not found!\n");
        return;
    }

    do {
        printf("\n========== UPDATE BOOKING ==========\n");
        printf("1. Update Passenger Name\n");
        printf("2. Update Age\n");
        printf("3. Update Gender\n");
        printf("4. Update Destination\n");
        printf("5. Update Flight Date\n");
        printf("6. Update Seat Class\n");
        printf("7. Finish Updating\n");

        choice = readInt("Enter choice: ", 1, 7);

        switch (choice) {
            case 1:
                readLine("Enter new name: ",
                         bookings[index].passengerName, 50);
                if (strlen(bookings[index].passengerName) == 0) {
                    printf("Name cannot be empty!\n");
                }
                break;

            case 2:
                bookings[index].age =
                    readInt("Enter new age: ", 1, 120);
                break;

            case 3:
                readLine("Enter new gender: ",
                         bookings[index].gender, 15);
                if (strlen(bookings[index].gender) == 0) {
                    printf("Gender cannot be empty!\n");
                }
                break;

            case 4:
                selectDestination(bookings[index].destination);
                bookings[index].ticketPrice = calculateBookingPrice(
                    bookings[index].destination,
                    bookings[index].seatClass,
                    bookings[index].flightDate
                );
                printf("\nDestination updated!\n");
                printf("New Ticket Price: Rs. %.2f\n",
                       bookings[index].ticketPrice);
                break;

            case 5:
                readLine("Enter new flight date: ",
                         bookings[index].flightDate, 20);
                if (strlen(bookings[index].flightDate) == 0) {
                    printf("Flight date cannot be empty!\n");
                }
                else {
                    bookings[index].ticketPrice = calculateBookingPrice(
                        bookings[index].destination,
                        bookings[index].seatClass,
                        bookings[index].flightDate
                    );
                    printf("New Ticket Price: Rs. %.2f\n",
                           bookings[index].ticketPrice);
                }
                break;

            case 6:
                selectClass(&bookings[index]);
                break;

            case 7:
                printf("Updating completed!\n");
                break;
        }
    } while (choice != 7);

    saveToFile();
    printf("\nBooking updated successfully!\n");
}


/* Delete or cancel booking */
void cancelBooking(void) {
    int id, index, i;

    id = readInt("Enter Booking ID to cancel: ", 1, 999999);
    index = findBooking(id);

    if (index == -1) {
        printf("\nBooking not found!\n");
        return;
    }

    for (i = index; i < count - 1; i++) {
        bookings[i] = bookings[i + 1];
    }

    count--;
    saveToFile();

    printf("\nBooking cancelled successfully!\n");
}


/* STUDENT 4: SORTING AND REPORTS */

void sortBookings(void) {
    int i, j;
    struct Booking temp;

    if (count < 2) {
        printf("\nNot enough bookings to sort!\n");
        return;
    }

    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (strcmp(bookings[j].passengerName,
                       bookings[j + 1].passengerName) > 0) {
                temp = bookings[j];
                bookings[j] = bookings[j + 1];
                bookings[j + 1] = temp;
            }
        }
    }

    saveToFile();
    printf("\nBookings sorted alphabetically!\n");
    displayBookings();
}

void generateReport(void) {
    int i;
    float totalRevenue = 0;

    if (count == 0) {
        printf("\nNo booking records available!\n");
        return;
    }

    for (i = 0; i < count; i++) {
        totalRevenue += bookings[i].ticketPrice;
    }

    printf("\n========== BOOKING REPORT ==========\n");
    printf("Total Bookings: %d\n", count);
    printf("Total Revenue: Rs. %.2f\n", totalRevenue);
    printf("Average Ticket Price: Rs. %.2f\n",
           totalRevenue / count);
}


/* FILE HANDLING */

void saveToFile(void) {
    FILE *fp;

    fp = fopen("bookings.dat", "wb");

    if (fp == NULL) {
        printf("\nError saving file!\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, fp);
    fwrite(bookings, sizeof(struct Booking), count, fp);
    fclose(fp);
}

void loadFromFile(void) {
    FILE *fp;
    int i;

    fp = fopen("bookings.dat", "rb");

    if (fp == NULL) {
        return;
    }

    if (fread(&count, sizeof(int), 1, fp) != 1 ||
        count < 0 || count > MAX) {
        count = 0;
        fclose(fp);
        return;
    }

    if (fread(bookings, sizeof(struct Booking), count, fp)
        != (size_t)count) {
        count = 0;
        fclose(fp);
        return;
    }

    nextID = 1001;

    for (i = 0; i < count; i++) {
        if (bookings[i].bookingID >= nextID) {
            nextID = bookings[i].bookingID + 1;
        }
    }

    fclose(fp);
    printf("\nPrevious booking records loaded successfully!\n");
}
