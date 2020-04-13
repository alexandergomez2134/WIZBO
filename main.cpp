#include <iostream>
#include <string>
#include "scheduler.hpp"

int main()
{
  const int COURSE_MAX = 100;
  std::string filename;
  Course * courses = new Course[COURSE_MAX];
  int size = 0;

  std::cout << "Welcome to Tuffy Scheduler!\n";
  std::cout << "Please enter the file name containing the list of classes: ";
  std::cin >> filename;

  if (load_schedule(filename, &courses, size))
  {
    std::cout << "Schedule file loaded. Displaying contents ...\n\n";
    display_courses(&courses, size);
  }

  std::cout << "Thank you for using Tuffy Scheduler.\n";

  return 0;

  delete[] courses;
}
