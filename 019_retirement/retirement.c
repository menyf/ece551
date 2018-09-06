#include <stdio.h>
#include <stdlib.h>

struct retire_info_tag {
  int months;
  double contribution;
  double rate_of_return;
};
typedef struct retire_info_tag retire_info_t;

double func(int startAge, double initial, retire_info_t info) {
  double current_balance = initial;
  for (int i = 0; i < info.months; i++) {
    printf("Age %3d month %2d you have $%.2lf\n",
           (startAge + i) / 12,
           (startAge + i) % 12,
           current_balance);
    current_balance += current_balance * info.rate_of_return;
    current_balance += info.contribution;
  }
  return current_balance;
}

void retirement(int startAge,           //in months
                double initial,         //initial savings in dollars
                retire_info_t working,  // info about working
                retire_info_t retired)  // info about being retired
{
  double balance1 = func(startAge, initial, working);
  func(startAge + working.months, balance1, retired);
  return;
}

int main(void) {
  retire_info_t working;
  retire_info_t retired;
  working.months = 489;
  working.contribution = 1000;
  working.rate_of_return = 0.045 / 12;
  retired.months = 384;
  retired.contribution = -4000;
  retired.rate_of_return = 0.01 / 12;
  int startAge = 327;
  double initial = 21345;
  retirement(startAge, initial, working, retired);
  return EXIT_SUCCESS;
}
