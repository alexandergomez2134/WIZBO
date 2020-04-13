#include <iostream>
#include <string>

const int MIN_MIN = 0;
const int HR_MIN = 0;
const int MIN_MAX = 59;
const int HR_MAX = 24;
const int LEADING_ZERO_MIN = 10;
const int HR_MIN_SPLITTER = 100;

class Course
{
private:
  std::string course_name_;
  std::string location_;
  std::string weekly_schedule_;
  TimeSpan time_;

public:
  Course() : course_name_(""), location_(""), weekly_schedule_(""), time_(TimeSpan()) {}

  Course(std::string course, std::string location, std::string weekly, TimeSpan time) :
  course_name_(course), location_(location), weekly_schedule_(weekly), time_(time) {}

  std::string course_name() const { return course_name_; }
  std::string location() const { return location_; }
  std::string weekly_schedule() const { return weekly_schedule_; }
  TimeSpan time() const { return time_; }

  void set_course_name(const std::string & course_name)
  {
    course_name_ = course_name;
  }
  void set_location(const std::string & location) { location_ = location; }
  void set_weekly_schedule(const std::string & weekly_schedule)
  {
    weekly_schedule_ = weekly_schedule;
  }
  void set_time(const TimeSpan & time) { time_ = time; }

  void display() const;
};

class TimeSpan
{
private:
  int start_time_;
  int end_time_;

public:
  TimeSpan() : start_time_(0000), end_time_(0000) {}

  TimeSpan(int start, int end) : start_time_(start), end_time_(end) {}

  void display()
  {
    std::string start_leading;
    std::string end_leading;

    int start_hr = start_time_ / HR_MIN_SPLITTER;
    int start_min = start_time_ % HR_MIN_SPLITTER;
    int end_hr = end_time_ / HR_MIN_SPLITTER;
    int end_min = end_time_ % HR_MIN_SPLITTER;

    if (start_min < LEADING_ZERO_MIN)
    {
      start_leading = "0";
    }
    if (end_min < LEADING_ZERO_MIN)
    {
      end_leading = "0";
    }
    std::cout << "\nStart time: " << start_hr << ':' << start_leading << start_min
              << "\nEnd time: " << end_hr << ':' << end_leading << end_min << '\n';
  }

  void set_start_time(int start_time) { start_time_ = start_time; }
  void set_end_time(int end_time) { end_time_ = end_time; }
  int start_time() { return start_time_; }
  int end_time() { return end_time_; }

  bool operator<(TimeSpan & other) { return end_time() < other.start_time(); }

  bool operator>(TimeSpan & other) { return start_time() > other.end_time(); }
};

class CourseSchedule
{
private:
  Course * courses_[HR_MIN_SPLITTER];
  int num_courses_;

  bool has_conflict(Course * ptr)
  {
    for(int i = 0; i < num_courses_; i++)
    {
      if(courses_[i] == ptr)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }

public:
  CourseSchedule() : num_courses_(0) {}

  int num_courses() { return num_courses_; }

  bool add(std::string name, std::string location, std::string day, int start, int end )
  {
    Course * course = new Course;
    course->set_course_name(name);
    course->set_location(location);
    course->set_weekly_schedule(day);
    course->time_(start, end);
  }

  void display()
  {
    for(int i = 0; i < num_courses_; i++ )
    {
      courses_[i]->course_name();
      courses_[i]->location();
      courses_[i]->weekly_schedule();
      course_[i]->time_.display();

    }

  }
};

// global function prototypes
void display_courses(Course * courses[], int size);
bool load_schedule(const std::string & filename, Course * courses[],
                   int & size);
bool has_conflict(TimeSpan one, TimeSpan two[], int size);
