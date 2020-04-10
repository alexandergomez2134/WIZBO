#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include "gtest_ext.h"
#include "../scheduler.hpp"

using ::testing::HasSubstr;
using ::testing::ContainsRegex;
using ::testing::AnyOf;
using ::testing::AllOf;
using ::testing::Not;

class LoadSchedule : public UnitTestFileManager
{
protected:
  LoadSchedule() : UnitTestFileManager("sched.txt") {}
};

class CoursePointer : public ::testing::Test
{
protected:
  void SetUp() override {
    first_course = new Course;
    second_course = new Course;
  }

  void TearDown() override {
    delete first_course;
    delete second_course;
  }

  Course * first_course;
  Course * second_course;
};

TEST(TimeSpan, DefaultConstructor)
{
  TimeSpan your_timespan;
  ASSERT_EQ(your_timespan.start_time(), 0) << "Default start time should be 0000";
  ASSERT_EQ(your_timespan.end_time(), 0) << "Default start time should be 0000";
}

TEST(TimeSpan, NonDefaultConstructor1)
{
  TimeSpan your_timespan(1430, 1630);
  ASSERT_EQ(your_timespan.start_time(), 1430) << "Start time should be 1430";
  ASSERT_EQ(your_timespan.end_time(), 1630) << "Start time should be 1630";
}

TEST(TimeSpan, NonDefaultConstructor2)
{
  TimeSpan your_timespan(645, 745);
  ASSERT_EQ(your_timespan.start_time(), 645) << "Start time should be 645";
  ASSERT_EQ(your_timespan.end_time(), 745) << "Start time should be 745s";
}

TEST(TimeSpan, Display1)
{
  TimeSpan your_timespan(1130, 1230);
  SIMULATE_SIO("", your_timespan.display(), {
    std::string your_function_output = your_output;
    ASSERT_THAT(your_function_output, HasSubstr("11:30"));
    ASSERT_THAT(your_function_output, HasSubstr("12:30"));
  })
}

TEST(TimeSpan, Display2)
{
  TimeSpan your_timespan(645, 805);
  SIMULATE_SIO("", your_timespan.display(), {
    std::string your_function_output = your_output;
    ASSERT_THAT(your_function_output, HasSubstr("6:45"));
    ASSERT_THAT(your_function_output, HasSubstr("8:05"));
  })
}

TEST(TimeSpan, StartTimeAccessorMutator)
{
  TimeSpan your_time_span;
  your_time_span.set_start_time(1532);
  ASSERT_EQ(your_time_span.start_time(), 1532);
}

TEST(TimeSpan, EndTimeAccessorMutator)
{
  TimeSpan your_time_span;
  your_time_span.set_end_time(1920);
  ASSERT_EQ(your_time_span.end_time(), 1920);
}

TEST(TimeSpan, OperatorLTTrue)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(800);
  first_time_span.set_end_time(900);
  TimeSpan second_time_span;
  second_time_span.set_start_time(1000);
  second_time_span.set_end_time(1100);
  ASSERT_TRUE(first_time_span.operator<(second_time_span)) << "Time span 800 - 900 should be less than 1000 - 1100";
}

TEST(TimeSpan, OperatorLTEdgeTrue)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(800);
  first_time_span.set_end_time(900);
  TimeSpan second_time_span;
  second_time_span.set_start_time(901);
  second_time_span.set_end_time(1001);
  ASSERT_TRUE(first_time_span.operator<(second_time_span)) << "Time span 800 - 900 should be less than 901 - 1001";
}

TEST(TimeSpan, OperatorLTEdgeFalse)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(800);
  first_time_span.set_end_time(900);
  TimeSpan second_time_span;
  second_time_span.set_start_time(900);
  second_time_span.set_end_time(1000);
  ASSERT_FALSE(first_time_span.operator<(second_time_span)) << "Time span 800 - 900 should not be less than 900 - 1000";
}

TEST(TimeSpan, OperatorLTEndIntersectFalse)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(800);
  first_time_span.set_end_time(900);
  TimeSpan second_time_span;
  second_time_span.set_start_time(830);
  second_time_span.set_end_time(930);
  ASSERT_FALSE(first_time_span.operator<(second_time_span)) << "Time span 800 - 900 should not be less than 830 - 930";
}


TEST(TimeSpan, OperatorLTStartEndIntersectFalse)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(800);
  first_time_span.set_end_time(900);
  TimeSpan second_time_span;
  second_time_span.set_start_time(730);
  second_time_span.set_end_time(930);
  ASSERT_FALSE(first_time_span.operator<(second_time_span)) << "Time span 800 - 900 should not be less than 730 - 930";
}

TEST(TimeSpan, OperatorLTStartIntersectFalse)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(900);
  first_time_span.set_end_time(1000);
  TimeSpan second_time_span;
  second_time_span.set_start_time(830);
  second_time_span.set_end_time(930);
  ASSERT_FALSE(first_time_span.operator<(second_time_span)) << "Time span 900 - 1000 should not be less than 830 - 930";
}

TEST(TimeSpan, OperatorLTStartEndOverFalse)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(1000);
  first_time_span.set_end_time(1100);
  TimeSpan second_time_span;
  second_time_span.set_start_time(800);
  second_time_span.set_end_time(900);
  ASSERT_FALSE(first_time_span.operator<(second_time_span)) << "Time span 1000 - 1100 should not be less than 800 - 900";
}

TEST(TimeSpan, OperatorGTTrue)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(1000);
  first_time_span.set_end_time(1100);
  TimeSpan second_time_span;
  second_time_span.set_start_time(800);
  second_time_span.set_end_time(900);
  ASSERT_TRUE(first_time_span.operator>(second_time_span)) << "Time span 1000 - 1100 should be greater than 800 - 900";
}

TEST(TimeSpan, OperatorGTEdgeTrue)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(1001);
  first_time_span.set_end_time(1101);
  TimeSpan second_time_span;
  second_time_span.set_start_time(900);
  second_time_span.set_end_time(1000);
  ASSERT_TRUE(first_time_span.operator>(second_time_span)) << "Time span 1001 - 1101 should be greater than 900 - 1000";
}

TEST(TimeSpan, OperatorGTEdgeFalse)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(1000);
  first_time_span.set_end_time(1100);
  TimeSpan second_time_span;
  second_time_span.set_start_time(900);
  second_time_span.set_end_time(1000);
  ASSERT_FALSE(first_time_span.operator>(second_time_span)) << "Time span 1000 - 1100 should not be greater than 900 - 1000";
}

TEST(TimeSpan, OperatorGTStartIntersectFalse)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(900);
  first_time_span.set_end_time(1000);
  TimeSpan second_time_span;
  second_time_span.set_start_time(830);
  second_time_span.set_end_time(930);
  ASSERT_FALSE(first_time_span.operator>(second_time_span)) << "Time span 900 - 1000 should not be greater than 830 - 930";
}


TEST(TimeSpan, OperatorGTStartEndIntersectFalse)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(800);
  first_time_span.set_end_time(900);
  TimeSpan second_time_span;
  second_time_span.set_start_time(730);
  second_time_span.set_end_time(930);
  ASSERT_FALSE(first_time_span.operator>(second_time_span)) << "Time span 800 - 900 should not be greater than 730 - 930";
}

TEST(TimeSpan, OperatorGTEndIntersectFalse)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(730);
  first_time_span.set_end_time(900);
  TimeSpan second_time_span;
  second_time_span.set_start_time(830);
  second_time_span.set_end_time(930);
  ASSERT_FALSE(first_time_span.operator>(second_time_span)) << "Time span 730 - 900 should not be greater than 830 - 930";
}

TEST(TimeSpan, OperatorGTStartEndOverFalse)
{
  TimeSpan first_time_span;
  first_time_span.set_start_time(700);
  first_time_span.set_end_time(800);
  TimeSpan second_time_span;
  second_time_span.set_start_time(800);
  second_time_span.set_end_time(900);
  ASSERT_FALSE(first_time_span.operator<(second_time_span)) << "Time span 700 - 800 should not be greater than 800 - 900";
}

TEST(Course, DefaultConstructor)
{
  Course your_course;
  ASSERT_EQ(your_course.course_name(), "") << "The default course name should be empty";
  ASSERT_EQ(your_course.location(), "") << "The default location should be empty";
  ASSERT_EQ(your_course.weekly_schedule(), "") << "The default weekly schedule should be empty";
  ASSERT_EQ(your_course.time().start_time(), 0) << "The default start time should be 0000";
  ASSERT_EQ(your_course.time().end_time(), 0) << "The default end time should be 0000";
}

TEST(Course, NonDefaultConstructor1)
{
  Course your_course("CPSC 223W", "CS 101", "TH", TimeSpan(1330, 1520));
  ASSERT_EQ(your_course.course_name(), "CPSC 223W") << "Check how your constructor sets the course name";
  ASSERT_EQ(your_course.location(), "CS 101") << "Check how your constructor sets the the location";
  ASSERT_EQ(your_course.weekly_schedule(), "TH") << "Check how your constructor sets the weekly schedule";
  ASSERT_EQ(your_course.time().start_time(), 1330) << "Check how your constructor sets the start time";
  ASSERT_EQ(your_course.time().end_time(), 1520) << "Check how your constructor sets the end time";
}

TEST(Course, NonDefaultConstructor2)
{
  Course your_course("CPSC 121", "CS 102B", "TH", TimeSpan(900, 950));
  ASSERT_EQ(your_course.course_name(), "CPSC 121") << "Check how your constructor sets the course name";
  ASSERT_EQ(your_course.location(), "CS 102B") << "Check how your constructor sets the the location";
  ASSERT_EQ(your_course.weekly_schedule(), "TH") << "Check how your constructor sets the weekly schedule";
  ASSERT_EQ(your_course.time().start_time(), 900) << "Check how your constructor sets the start time";
  ASSERT_EQ(your_course.time().end_time(), 950) << "Check how your constructor sets the end time";
}

TEST(Course, CourseNameAccessorMutator)
{
  Course your_course;
  your_course.set_course_name("CPSC 121");
  ASSERT_EQ(your_course.course_name(), "CPSC 121");
}

TEST(Course, LocationAccessorMutator)
{
  Course your_course;
  your_course.set_location("E202");
  ASSERT_EQ(your_course.location(), "E202");
}

TEST(Course, WeeklyScheduleAccessorMutator)
{
  Course your_course;
  your_course.set_weekly_schedule("MW");
  ASSERT_EQ(your_course.weekly_schedule(), "MW");
}

TEST(Course, TimeAccessorMutator)
{
  Course your_course;
  your_course.set_time(TimeSpan(800, 900));
  ASSERT_EQ(your_course.time().start_time(), 800);
  ASSERT_EQ(your_course.time().end_time(), 900);
}

TEST(Course, Display)
{
  std::string expected_output =
      "Course name: CPSC 362\nLocation: CS 101\nWeekly schedule: MW\nStart "
      "time: 8:00\nEnd time: 9:50\n";
  Course your_course;
  your_course.set_course_name("CPSC 362");
  your_course.set_location("CS 101");
  your_course.set_weekly_schedule("MW");
  your_course.set_time(TimeSpan(800, 950));

  SIMULATE_SIO("", your_course.display(), {
    std::string your_function_output = your_output;
    ASSERT_EQ(your_function_output, expected_output);
  })
}

TEST_F(CoursePointer, IntersectsDifferentDayTimeIntersect)
{
  first_course->set_weekly_schedule("MW");
  first_course->set_time(TimeSpan(830, 920));
  second_course->set_weekly_schedule("TH");
  second_course->set_time(TimeSpan(900, 1000));
  ASSERT_FALSE(first_course->intersects(second_course)) << "The first course (MW 8:30 - 9:20) does not intersect with the second course (TH 9:00 - 10:00)";
}

TEST_F(CoursePointer, IntersectsDifferentDayNoTimeIntersect)
{
  first_course->set_weekly_schedule("MW");
  first_course->set_time(TimeSpan(830, 920));
  second_course->set_weekly_schedule("F");
  second_course->set_time(TimeSpan(1300, 1400));
  ASSERT_FALSE(first_course->intersects(second_course)) << "The first course (MW 8:30 - 9:20) does not intersect with the second course (F 13:00 - 14:00)";
}

TEST_F(CoursePointer, IntersectsSameDayTimeIntersect)
{
  first_course->set_weekly_schedule("MW");
  first_course->set_time(TimeSpan(830, 920));
  second_course->set_weekly_schedule("MW");
  second_course->set_time(TimeSpan(900, 1000));
  ASSERT_TRUE(first_course->intersects(second_course)) << "The first course (MW 8:30 - 9:20) intersects with the second course (MW 9:00-10:00)";
}

TEST_F(CoursePointer, IntersectsSameDayNoTimeIntersect)
{
  first_course->set_weekly_schedule("MW");
  first_course->set_time(TimeSpan(830, 920));
  second_course->set_weekly_schedule("MW");
  second_course->set_time(TimeSpan(1400, 1600));
  ASSERT_FALSE(first_course->intersects(second_course)) << "The first course (MW 8:30 - 9:20) does not intersect with the second course (MW 14:00-16:00)";
}

TEST(CourseSchedule, DefaultConstructorAccessor)
{
  CourseSchedule your_course_schedule;
  ASSERT_EQ(your_course_schedule.num_courses(), 0) << "CourseSchedules should have no courses by default";
}

TEST(CourseSchedule, DisplayOneCourse)
{
  CourseSchedule your_schedule;
  your_schedule.add("CPSC 223W", "CS 101", "TH", 1330, 1520);
  SIMULATE_SIO("", your_schedule.display(), {
    ASSERT_THAT(your_output, ContainsRegex("CPSC 223W.*\\s*CS 101.*\\s*TH.*\\s*13:30.*\\s*15:20")) << "Your program should display the course schedule CPSC 223W at CS 101 on TH from 13:30 - 15:20";
  })
}

TEST(CourseSchedule, DisplayMultiCourse)
{
  CourseSchedule your_schedule;
  your_schedule.add("CPSC 223W", "CS 101", "TH", 1330, 1520);
  your_schedule.add("CPSC 121", "CS 102B", "F", 900, 950);
  your_schedule.add("CPSC 131", "EC 109", "MW", 1600, 1715);
  SIMULATE_SIO("", your_schedule.display(), {
    ASSERT_THAT(your_output, ContainsRegex("CPSC 223W.*\\s*CS 101.*\\s*TH.*\\s*13:30.*\\s*15:20")) << "Your program should display the course schedule CPSC 223W at CS 101 on TH from 13:30 - 15:20";
    ASSERT_THAT(your_output, ContainsRegex("CPSC 121.*\\s*CS 102B.*\\s*F.*\\s*9:00.*\\s*9:50")) << "Your program should display the course schedule CPSC 121 at CS 102B on F from 9:00 - 9:50";
    ASSERT_THAT(your_output, ContainsRegex("CPSC 131.*\\s*EC 109.*\\s*MW.*\\s*16:00.*\\s*17:15")) << "Your program should display the course schedule CPSC 131 at EC 109 on MW from 16:00 - 17:15";
  })

}

TEST(CourseSchedule, AddDisplay_OneElemNoConflict)
{
  CourseSchedule your_schedule;
  your_schedule.add("","","MW", 730, 900); // base element
  bool add_result = your_schedule.add("","","F", 1030, 1130); // check element
  ASSERT_TRUE(add_result) << "The function should return true because (F 10:30 - 11:30) does not conflict with any of the following course schedules (MW 7:30 - 9:00)";
  SIMULATE_SIO("", your_schedule.display(), {
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*7:30.*\\s*9:00")) << "The display function should print the schedule MW 7:30 - 9:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("F.*\\s*10:30.*\\s*11:30")) << "The display function should print the schedule F 10:30 - 11:30 because there was no confllict";
  })
}

TEST(CourseSchedule, AddDisplay_OneElemConflict)
{
  CourseSchedule your_schedule;
  your_schedule.add("","","MW", 730, 900); // base element
  bool add_result = your_schedule.add("","","MW", 800, 900); // check element
  ASSERT_FALSE(add_result) << "The function should return false because (MW 8:00 - 9:00) conflicts with one of the following course schedules (MW 7:30 - 9:00)";
  SIMULATE_SIO("", your_schedule.display(), {
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*7:30.*\\s*9:00")) << "The display function should only print the schedule MW 7:30 - 9:00";
  })
}

TEST(CourseSchedule, AddDisplay_NoConflict)
{
  CourseSchedule your_schedule;
  your_schedule.add("","","MW", 1000, 1100); // base element
  your_schedule.add("","","MW", 1200, 1300); // base element
  your_schedule.add("","","MW", 1525, 1600); // base element
  bool add_result = your_schedule.add("","","F", 800, 900); // check element
  ASSERT_TRUE(add_result) << "The function should return true because (F 8:00 - 9:00) does not conflict with any of the following course schedules (MW 10:00 - 11:00, MW 12:00 - 13:00, MW 15:25 - 16:00)";
  SIMULATE_SIO("", your_schedule.display(), {
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*10:00.*\\s*11:00")) << "The display function should print the schedule MW 10:00 - 11:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*12:00.*\\s*13:00")) << "The display function should print the schedule MW 12:00 - 13:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*15:25.*\\s*16:00")) << "The display function should print the schedule MW 15:25 - 16:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("F.*\\s*8:00.*\\s*9:00")) << "The display function should print the schedule F 8:00 - 9:00 because there was no confllict";
  })

}

TEST(CourseSchedule, AddDisplay_OneConflictFront)
{
  CourseSchedule your_schedule;
  your_schedule.add("","","MW", 1000, 1100); // base element
  your_schedule.add("","","MW", 1200, 1300); // base element
  your_schedule.add("","","MW", 1525, 1600); // base element
  bool add_result = your_schedule.add("","","MW", 900, 1100); // check element
  ASSERT_FALSE(add_result) << "The function should return false because (MW 9:00 - 11:00) conflicts with one of the following course schedules (MW 10:00 - 11:00, MW 12:00 - 13:00, MW 15:25 - 16:00)";
  SIMULATE_SIO("", your_schedule.display(), {
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*10:00.*\\s*11:00")) << "The display function should print the schedule MW 10:00 - 11:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*12:00.*\\s*13:00")) << "The display function should print the schedule MW 12:00 - 13:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*15:25.*\\s*16:00")) << "The display function should print the schedule MW 15:25 - 16:00 because there was no confllict";
    ASSERT_THAT(your_output, Not(ContainsRegex("F.*\\s*8:00.*\\s*9:00"))) << "The display function should not print the schedule F 8:00 - 9:00 because there was a confllict";
  })
}
TEST(CourseSchedule, AddDisplay_OneConflictMid)
{
  CourseSchedule your_schedule;
  your_schedule.add("","","MW", 1000, 1100); // base element
  your_schedule.add("","","MW", 1200, 1300); // base element
  your_schedule.add("","","MW", 1525, 1600); // base element
  bool add_result = your_schedule.add("","","MW", 1230, 1330); // check element
  ASSERT_FALSE(add_result) << "The function should return false because (MW 12:30 - 13:30) conflicts with one of the following course schedules (MW 10:00 - 11:00, MW 12:00 - 13:00, MW 15:25 - 16:00)";
  SIMULATE_SIO("", your_schedule.display(), {
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*10:00.*\\s*11:00")) << "The display function should print the schedule MW 10:00 - 11:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*12:00.*\\s*13:00")) << "The display function should print the schedule MW 12:00 - 13:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*15:25.*\\s*16:00")) << "The display function should print the schedule MW 15:25 - 16:00 because there was no confllict";
    ASSERT_THAT(your_output, Not(ContainsRegex("MW.*\\s*12:30.*\\s*13:30"))) << "The display function should not print the schedule MW 12:30 - 13:30 because there was a confllict";
  })
}

TEST(CourseSchedule, AddDisplay_OneConflictLast)
{
  CourseSchedule your_schedule;
  your_schedule.add("","","MW", 1000, 1100); // base element
  your_schedule.add("","","MW", 1200, 1300); // base element
  your_schedule.add("","","MW", 1525, 1600); // base element
  bool add_result = your_schedule.add("","","MW", 1545, 1700); // check element
  ASSERT_FALSE(add_result) << "The function should return false because (MW 15:45 - 17:00) conflicts with one of the following course schedules (MW 10:00 - 11:00, MW 12:00 - 13:00, MW 15:25 - 16:00)";
  SIMULATE_SIO("", your_schedule.display(), {
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*10:00.*\\s*11:00")) << "The display function should print the schedule MW 10:00 - 11:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*12:00.*\\s*13:00")) << "The display function should print the schedule MW 12:00 - 13:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*15:25.*\\s*16:00")) << "The display function should print the schedule MW 15:25 - 16:00 because there was no confllict";
    ASSERT_THAT(your_output, Not(ContainsRegex("MW.*\\s*15:45.*\\s*17:00"))) << "The display function should not print the schedule MW 15:45 - 17:00 because there was a confllict";
  })
}

TEST(CourseSchedule, AddDisplay_MultiConflict)
{
  CourseSchedule your_schedule;
  your_schedule.add("","","MW", 1000, 1100); // base element
  your_schedule.add("","","MW", 1200, 1300); // base element
  your_schedule.add("","","MW", 1525, 1600); // base element
  bool add_result = your_schedule.add("","","MW", 1230, 1600); // check element
  ASSERT_FALSE(add_result) << "The function should return false because (MW 12:30 - 16:00) conflicts with one or more of the following course schedules (MW 10:00 - 11:00, MW 12:00 - 13:00, MW 15:25 - 16:00)";
  SIMULATE_SIO("", your_schedule.display(), {
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*10:00.*\\s*11:00")) << "The display function should print the schedule MW 10:00 - 11:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*12:00.*\\s*13:00")) << "The display function should print the schedule MW 12:00 - 13:00 because there was no confllict";
    ASSERT_THAT(your_output, ContainsRegex("MW.*\\s*15:25.*\\s*16:00")) << "The display function should print the schedule MW 15:25 - 16:00 because there was no confllict";
    ASSERT_THAT(your_output, Not(ContainsRegex("MW.*\\s*12:30.*\\s*16:00"))) << "The display function should not print the schedule MW 12:30 - 16:00 because there was a confllict";
  })
}

TEST_F(LoadSchedule, FileDoesntExist)
{
  std::string user_input = "nonexistent.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("does not exist"),
                                  HasSubstr("doesn't exist")),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file doesn't exist.";
          // Ensure no other errors are show
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("start time"),
                        HasSubstr("end time"), HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, NoLocation)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();

  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("location"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "location. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("weekly schedule"), HasSubstr("symbol"),
                        HasSubstr("start time"), HasSubstr("end time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, NoWeeklySchedule)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();

  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("weekly schedule"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "weekly schedule. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(your_function_output,
                      Not(AnyOf(HasSubstr("location"), HasSubstr("symbol"),
                                HasSubstr("start time"), HasSubstr("end time"),
                                HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, InvalidDay1)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202\nX";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("weekly schedule symbol"), HasSubstr("X"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the weekly schedule "
                 "uses an "
                 "invalid symbol. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("start time"),
                        HasSubstr("end time"), HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, InvalidDay2)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202\nMWX";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("weekly schedule symbol"),
                            HasSubstr("MWX"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the weekly schedule "
                 "uses an "
                 "invalid symbol. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("start time"),
                        HasSubstr("end time"), HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, NoStartTime1)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202\nMW\n1124";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();

  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("start time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "start time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("end time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, NoStartTime2)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202\nMW\nabcd";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("start time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "start time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("end time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, NoEndTime1)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202\nMW\n11:30\n1124";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("end time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain an "
                 "end time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("start time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, NoEndTime2)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202\nMW\n8:35\nabcd";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();

  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("end time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain an "
                 "end time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("start time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, InvalidStartTime1)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202\nMW\n25:30";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();

  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(HasSubstr("25:30"),
                            AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("start time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "valid start time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("end time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, InvalidStartTime2)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202\nMW\n15:88";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(HasSubstr("15:88"),
                            AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("start time")))
              << "The function should show an error if the file does not "
                 "contain a "
                 "valid start time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("end time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, InvalidEndTime1)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202\nMW\n11:30\n100:25";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(HasSubstr("100:25"),
                            AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("end time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "valid end time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("start time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, InvalidEndTime2)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202\nMW\n8:35\n12:999";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(HasSubstr("12:999"),
                            AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("end time")))
              << "The function should show an error if the file does not "
                 "contain a "
                 "valid end time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("start time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, StartLaterThanEnd)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSCC 121\nE202\nMW\n18:35\n11:30\n";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(HasSubstr("18:35"), HasSubstr("11:30"),
                            HasSubstr("happen before"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file contains a "
                 "start time "
                 "that happens after the end time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown

          ASSERT_THAT(
              your_function_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

// Test cases for schedule files with multiple schedules
TEST_F(LoadSchedule, MultSchedNoLocation)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 121\nE202\nMW\n11:30\n13:20\nCPSC 131";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();

  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("location"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "location. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("weekly schedule"), HasSubstr("symbol"),
                        HasSubstr("start time"), HasSubstr("end time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedNoWeeklySchedule)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPS 131\nE101\nMW\n8:30\n11:20\nCPSC 121\nE202";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();

  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("weekly schedule"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "weekly schedule. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(your_function_output,
                      Not(AnyOf(HasSubstr("location"), HasSubstr("symbol"),
                                HasSubstr("start time"), HasSubstr("end time"),
                                HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedInvalidDay1)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPS 131\nE101\nMW\n8:30\n11:20\nCPSC 121\nE202\nX";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("weekly schedule symbol"), HasSubstr("X"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the weekly schedule "
                 "uses an "
                 "invalid symbol. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("start time"),
                        HasSubstr("end time"), HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedInvalidDay2)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPS 131\nE101\nMW\n8:30\n11:20\nCPSC 121\nE202\nMWX";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("weekly schedule symbol"),
                            HasSubstr("MWX"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the weekly schedule "
                 "uses an "
                 "invalid symbol. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("start time"),
                        HasSubstr("end time"), HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedNoStartTime1)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPS 131\nE101\nMW\n8:30\n11:20\nCPSC 121\nE202\nMW\n1124";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();

  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("start time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "start time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("end time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedNoStartTime2)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPS 131\nE101\nMW\n8:30\n11:20\nCPSC 121\nE202\nMW\nabcd";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("start time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "start time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("end time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedNoEndTime1)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPS 131\nE101\nMW\n8:30\n11:20\nCPSC 121\nE202\nMW\n11:30\n1124";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("end time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain an "
                 "end time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("start time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedNoEndTime2)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPS 131\nE101\nMW\n8:30\n11:20\nCPSC 121\nE202\nMW\n8:35\nabcd";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();

  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(AnyOf(HasSubstr("Unable"), HasSubstr("unable")),
                            HasSubstr("end time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain an "
                 "end time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("start time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedInvalidStartTime1)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPS 131\nE101\nMW\n8:30\n11:20\nCPSC 121\nE202\nMW\n25:30";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();

  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(HasSubstr("25:30"),
                            AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("start time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "valid start time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("end time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedInvalidStartTime2)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPS 131\nE101\nMW\n8:30\n11:20\nCPSC 121\nE202\nMW\n15:88";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(HasSubstr("15:88"),
                            AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("start time")))
              << "The function should show an error if the file does not "
                 "contain a "
                 "valid start time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("end time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedInvalidEndTime1)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPS 131\nE101\nMW\n8:30\n11:20\nCPSC 121\nE202\nMW\n11:30\n100:25";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(HasSubstr("100:25"),
                            AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("end time"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file does not "
                 "contain a "
                 "valid end time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("start time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedInvalidEndTime2)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPS 131\nE101\nMW\n8:30\n11:20\nCPSC 121\nE202\nMW\n8:35\n12:999";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(HasSubstr("12:999"),
                            AnyOf(HasSubstr("Invalid"), HasSubstr("invalid")),
                            HasSubstr("end time")))
              << "The function should show an error if the file does not "
                 "contain a "
                 "valid end time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown
          ASSERT_THAT(
              your_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"), HasSubstr("start time"),
                        HasSubstr("happen before"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, MultSchedStartLaterThanEnd)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPS 131\nE101\nMW\n8:30\n11:20\nCPSCC 121\nE202\nMW\n18:35\n11:30\n";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    SIMULATE_SIO(
        "", function_result = load_schedule(user_input, your_schedule), {
          std::string your_function_output = your_output;
          ASSERT_THAT(your_function_output,
                      AllOf(HasSubstr("18:35"), HasSubstr("11:30"),
                            HasSubstr("happen before"),
                            AnyOf(HasSubstr("Invalid file"),
                                  HasSubstr("invalid file"))))
              << "The function should show an error if the file contains a "
                 "start time "
                 "that happens after the end time. File contents:\n"
              << file_contents;
          // Ensure no other errors are shown

          ASSERT_THAT(
              your_function_output,
              Not(AnyOf(HasSubstr("location"), HasSubstr("weekly schedule"),
                        HasSubstr("symbol"))))
              << "The function should not show other errors";
        })
    ASSERT_FALSE(function_result)
        << "The function should return false because there are errors in the "
           "schedule file format.";
  })
}

TEST_F(LoadSchedule, ValidOneCourse)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents = "CPSC 131\nE101\nMW\n8:30\n11:20\n";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    function_result = load_schedule(user_input, your_schedule);
    ASSERT_TRUE(function_result) << "The function should return true because "
                                    "the file is valid. File contents:\n"
                                 << file_contents;
    ASSERT_EQ(your_schedule.num_courses(), 1) << "The function should set size to 1, because there is "
                          "one course in the schedule. File contents:\n"
                       << file_contents;

    SIMULATE_SIO("", your_schedule.display(), {
      ASSERT_THAT(your_output, ContainsRegex("CPSC 131.*\\s*E101.*\\s*MW.*\\s*8:30.*\\s*11:20")) << "Your program should display the course schedule CPSC 223W at CS 101 on TH from 13:30 - 15:20. File contents:\n"
      << file_contents;
    })
  })
}

TEST_F(LoadSchedule, ValidMultiCourse)
{
  std::string user_input = "sched.txt";
  CourseSchedule your_schedule;
  int size = 0;
  bool function_result = true;

  std::string file_contents =
      "CPSC 131\nE101\nMW\n8:30\n11:20\nCPSC 481\nCS "
      "408\nMW\n16:00\n17:15\nCPSC 362\nCS 101\nF\n8:00\n9:50";
  std::ofstream outfile("sched.txt");
  outfile << file_contents;
  outfile.close();
  ASSERT_DURATION_LE(3, {
    function_result = load_schedule(user_input, your_schedule);
    ASSERT_TRUE(function_result) << "The function should return true because "
                                    "the file is valid. File contents:\n"
                                 << file_contents;
    ASSERT_EQ(your_schedule.num_courses(), 3)
        << "The function should set size to 3, because there are "
           "three courses in the schedule. File contents:\n"
        << file_contents;

    SIMULATE_SIO("", your_schedule.display(), {
      ASSERT_THAT(your_output, ContainsRegex("CPSC 131.*\\s*E101.*\\s*MW.*\\s*8:30.*\\s*11:20")) << "Your program should display the course schedule CPSC 223W at CS 101 on TH from 13:30 - 15:20. File contents:\n"
      << file_contents;

      ASSERT_THAT(your_output, ContainsRegex("CPSC 481.*\\s*CS 408.*\\s*MW.*\\s*16:00.*\\s*17:15")) << "Your program should display the course schedule CPSC 481 at CS 408 on MW from 16:00 - 17:15. File contents:\n"
      << file_contents;

      ASSERT_THAT(your_output, ContainsRegex("CPSC 362.*\\s*CS 101.*\\s*F.*\\s*8:00.*\\s*9:50")) << "Your program should display the course schedule CPSC 362 at CS 101 on F from 8:00 - 9:50. File contents:\n"
      << file_contents;
    })
  })
}

int main(int argc, char ** argv)
{
  testing::InitGoogleTest(&argc, argv);
  ::testing::UnitTest::GetInstance()->listeners().Append(new SkipListener());
  return RUN_ALL_TESTS();
}
