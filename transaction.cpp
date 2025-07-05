#include "transaction.h"
#include "library.h"

int calculateFine(Book &book, int today_date) {
    if (today_date <= book.due_date || book.due_date == -1) return 0;
    int overdue_days = today_date - book.due_date;
    return overdue_days * 10; // Rs.10 per day fine example
}