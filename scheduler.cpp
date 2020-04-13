#include <fstream>
#include "scheduler.hpp"

void Course::display() const
{
  std::cout << "Course name: " << course_name_ << "\nLocation: " << location_
            << "\nWeekly schedule: " << weekly_schedule_
            << time_.display()
            << '\n';
}

// display all courses from the input array parameter
void display_courses(Course * courses[], int size)
{
  for (int i = 0; i < size; i++)
  {
    courses[i]->display();
    std::cout << '\n';
  }
}
// read all courses from the file into the array, and return whether the file
// is valid
bool load_schedule(const std::string & filename, Course * courses[], int & size)
{
  std::ifstream input_file;
  bool valid_format = true;
  size = 0;

  input_file.open(filename);

  if (!input_file)
  {
    valid_format = false;
    std::cout << "Error: File does not exist - " << filename << ".\n";
  }
  else
  {
    std::string course;
    std::string location;
    std::string day_of_week;
    int start_hr;
    int start_min;
    char start_colon;
    int end_hr;
    int end_min;
    char end_colon;

    while (getline(input_file, course) && valid_format)
    {
      // initialize the value for the colons in order to be able to verify
      // whether the colon is successfully read from the start and end times
      start_colon = ' ';
      end_colon = ' ';

      if (course.empty())
      {
        valid_format = false;
        std::cout << "Error: Unable to get course name.\n";
      }
      else if (!getline(input_file, location))
      {
        valid_format = false;
        std::cout << "Error: Unable to get a location.\n";
      }
      else if (location.empty())
      {
        valid_format = false;
        std::cout << "Error: Unable to get a location.\n";
      }
      else if (!getline(input_file, day_of_week))
      {
        valid_format = false;
        std::cout << "Error: Unable to get a weekly schedule.\n";
      }
      else if (day_of_week != "M" && day_of_week != "T" && day_of_week != "W" &&
               day_of_week != "H" && day_of_week != "F" && day_of_week != "S" &&
               day_of_week != "MW" && day_of_week != "MF" &&
               day_of_week != "TH")
      {
        valid_format = false;
        std::cout << "Error: Invalid weekly schedule symbol " << day_of_week
                  << ".\n";
      }
      // read the start time into 3 variables in order to check for valid hour
      // and minutes
      else if (!(input_file >> start_hr >> start_colon >> start_min))
      {
        valid_format = false;
        std::cout << "Error: Unable to get a valid start time.\n";
      }
      else if (start_hr < HR_MIN || start_hr > HR_MAX || start_min < MIN_MIN ||
               start_min > MIN_MAX || start_colon != ':')
      {
        valid_format = false;
        std::cout << "Error: " << start_hr << start_colon
                  << ((start_min < 10 && start_min >= 0) ? "0" : "")
                  << start_min << " is an invalid start time.\n";
      }
      // read the end time into 3 variables in order to check for valid hour
      // and minutes
      else if (!(input_file >> end_hr >> end_colon >> end_min))
      {
        valid_format = false;
        std::cout << "Error: Unable to get a valid end time.\n";
      }
      else if (end_hr < HR_MIN || end_hr > HR_MAX || end_min < MIN_MIN ||
               end_min > MIN_MAX || end_colon != ':')
      {
        valid_format = false;
        std::cout << "Error: " << end_hr << end_colon
                  << ((end_min < 10 && end_min >= 0) ? "0" : "") << end_min
                  << " is an invalid end time.\n";
      }
      // make sure the start time comes before the end time
      else if (end_hr < start_hr || (start_hr == end_hr && end_min < start_min))
      {
        valid_format = false;
        std::cout << "Error: The start time " << start_hr << start_colon
                  << ((start_min < 10 && start_min >= 0) ? "0" : "")
                  << start_min << " should happen before the end time "
                  << end_hr << end_colon
                  << ((end_min < 10 && end_min >= 0) ? "0" : "") << end_min
                  << ".\n";
      }

      // move file pointer to the next line after having read an int
      input_file.ignore();

      // set all values for the current array location

      Course * cour = new Course;
      cour->set_course_name(course);
      cour->set_location(location);
      cour->set_weekly_schedule(day_of_week);
      cour->set_start_time(start_hr * HR_MIN_SPLITTER + start_min);
      cour->set_end_time(end_hr * HR_MIN_SPLITTER + end_min);
      *(courses + size) = cour;
      size++;
    }
    input_file.close();
  }
  if (!valid_format)
  {
    std::cout << "Invalid file.\n";
  }
  return valid_format;
}

bool has_conflict(TimeSpan one, TimeSpan two[], int size)
{
  for (int i = 0; i < size; i++)
  {
    if (one.start_time() < two[i].end_time() &&
        one.end_time() > two[i].start_time())
    {
      return true;
    }
  }
  return false;
}

/*bool intersects(Course * course)
{
  if()
}*/
