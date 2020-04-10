# TuffyScheduler Milestone 5

This is milestone 5 of the [TuffyScheduler Project](https://docs.google.com/document/d/1439ruL-xNEz58-ND_3Li7nthr1om_dwXJ3zQlB42oPI/edit?usp=sharing).

# Requirements
In this milestone we will *refactor* our code by creating a **CourseSchedule** object to manage the creation and management of dynamically allocated **Course** objects representing your schedule. *Refactor* means reorganizing the structure of your code to make it more readable, adaptable, and/or scalable while keeping its intended properties.

## Classes
### TimeSpan
*Summary: Provide constructors for the **TimeSpan** class to make them easier to use. This allows you to use constructors instead of mutators when initializing your objects.*

#### Default constructor
Create a default constructor that set both the start and end time of the object to `0000`.

#### Non-default constructor
Create a non-default constructor that accepts two parameters: a start time and end time. Use an initialization list to assign the object's start and end time data members using the parameters provided.

#### display
Create a function called *display* that prints on-screen information about the object's start and end time. It should not take any parameters nor return any values. Below is an example of what your display function might display. Take note that it should display the time in military format.

```
Start time: 9:00
End time: 13:30
```

### Course
*Summary: Replace the integer start and end time data members with a single **TimeSpan** object. Using a **TimeSpan** data member implements object composition. Your code should now be easier to manage as all time-related functionalities are already implemented in the **TimeSpan** object and are only called by member functions inside **Course***

#### Data members
Replace the start time and end time integers with a single **TimeSpan** object, *time_*.

#### Default Constructor
Create a default constructor that sets *course_name_*, *location_*, and *weekly_schedule_* to an empty string (`""`). Set *time_* to a **TimeSpan** object.

*Hint: You can create an unnamed object by using the class name followedy by ( ). For example, `Rectangle()` will create a Rectangle object using its default constructor.

#### NonDefault Constructor
Create a nondefault constructor that takes in four parameters: course name, location, weekly schedule, and a time span. It should set the parameters' values to the corresponding data members.

#### Accessors and mutators
Remove the accessors and mutators for start and end time, and replace them with an accessor and mutator for the *time_* data member.

#### display
Update your *display* member function so that it still prints information about the **Course**. However, it now calls the appropriate member function provided by the **TimeSpan** class.

#### intersects
Create a function called *intersects* that accepts a **Course** pointer as a parameter and returns a boolean value. The function will compare the **Course** object against the other **Course** object referenced by the pointer (this was passed as a parameter). 

**Course** objects intersect when they are scheduled on the same day and their start and end times intersect. You can use the same logic from Milestone 4, but also consider the weekly schedule.

### CourseSchedule
*Summary: Create a new class called **CourseSchedule** that wil manage your dynamically allocated **Course** objects.*

#### Data members
Create an array of **Course** pointers caled *courses_* that has a maximum capacity of 100 elements. This is the same data type you used in the *main* function from Milestone 4.

Create an integer data member called *num_courses_* that will keep track of the total number of **Courses** added to *courses_*.

#### Default constructor
Create a default constructor that initializes *num_courses_* to 0. This indicates that there are no **Course** objects in the array when a **CourseSchedule** object is created.

#### Accessors and mutators
Create an accessor for the *num_courses_* data member. There is no need to create accessors or mutators for other data members.

#### has_conflict
Create a *private* member function called *has_conflict*. It should accept a **Course** pointer as a parameter and return a boolean value.

The function should dereference and compare the **Course** object passed as a parameter against each **Course** object referenced in the *course_* array. If the passed **Course** object intersects with at least one of those objects, then the function should return `true`. Otherwise, it should return `false`.

#### add
Create a member function called *add* that accepts five parameters that describe a course: course name, course location, day of the week, start time, and end time. This function needs to dynamically allocate a **Course** object in the heap. Then, assign the values from the function's parameters to the corresponding data member of the object.

Call the *has_conflict* function to check if the newly created **Course** object conflicts with any of the **Course**s inside *courses_*. If there are no conflicts, then add the newly created **Course** object to the *course_* array and return `true`. However, if it intersects with any of the **Course** objects in the *course_* array, remove the newly created **Course** object from the heap and return `false`.

#### display
Create a *display* member function that does not accept any parameter or return any value. The function goes through each **Course** object referenced in *courses_* and displays information about the object on the screen. Use the appropriate member function(s) to print information about the object on the screen. 

#### Destructor
Create a destructor that will remove (deallocate) all **Course** objects that were dynamically allocated in the heap.

## load_schedule 
Modify your *load_schedule* global function to accept two parameters: a filename and a reference to a **CourseSchedule** object. The **CourseSchedule** parameter is passed by reference so that any changes will be reflected back to whoever calls the *load_schedule* function.

The function should behave the same way as in Milestone 4. However, it should use the *add* member function of the passed **CourseSchedule** object instead. Use the appropriate variables as arguments to the *add* function. Take note that this new code will replace the implementation from Milestone 4 where we dynamically created and added **Course** objects to an array.

## main function (Driver)
*Summary: Modify your *main* function to use a **CourseSchedule** object instead of manually manipulating your array of **Course** pointers.*

Replace your array of **Course** pointers with a single **CourseSchedule** object.

Modify the call to *load_schedule* so that it passes the filename and the **CourseSchedule** object as arguments.

Call the appropriate **CourseSchedule** member function to display information about all **Courses** that were loaded.

## Removed functions
The following functions will no longer be used as they are refactored into our classes. Feel free to remove them.

- display_courses: refactored as the **CourseSchedule**'s *display* member function
- has_conflict: refactored as the **CourseSchedule**'s *has_conflict* member function

Your program should behave the same way as it did in Milestone 4.

# Sample output
Below are samples of the program's output given different cases.

## Valid file
<pre>
Welcome to Tuffy Scheduler!
Please enter the file name containing the list of classes: <b>sched.txt</b>
Schedule file loaded. Displaying contents ...

Course name: CPSC 131
Location: EC 109
Weekly schedule: MW
Start time: 16:00
End time: 17:15

Course name: CPSC 481
Location: CS 408
Weekly schedule: MW
Start time: 16:00
End time: 17:15

Course name: CPSC 362
Location: CS 101
Weekly schedule: MW
Start time: 8:00
End time: 9:50

Thank you for using Tuffy Scheduler.
</pre>

## Start time happens after end time
<pre>
Welcome to Tuffy Scheduler!
Please enter the file name containing the list of classes: <b>sched.txt</b>
Error: The start time 17:15 should happen before the end time 16:00.
Invalid file.
Thank you for using Tuffy Scheduler.
</pre>

## Invalid weekly schedule
<pre>
Welcome to Tuffy Scheduler!
Please enter the file name containing the list of classes: <b>sched.txt</b>
Error: Invalid weekly schedule symbol MX.
Invalid file.
Thank you for using Tuffy Scheduler.
</pre>

# Submission checklist
1. Compiled and ran the driver (`main`).
1. Manually checked for compilation and logical errors.
1. Designed and tested your own test cases.
1. Ensured no errors on the unit test (`make test`).
1. Ensured no errors on the style check (`make stylecheck`).
1. Ensured no errors on the format check (`make formatcheck`).
1. Completed reflection file ([reflection.md](reflection.md))

# Code evaluation
Open the terminal and navigate to the folder that contains this exercise. Assuming you have pulled the code inside of `/home/student/ts05-tuffy` and you are currently in `/home/student` you can issue the following commands

```
cd ts05-tuffy
```

Use the `clang++` command to compile your code and the `./` command to run it. The sample code below shows how you would compile code save in `main.cpp` and into the executable file `main`. Make sure you use the correct filenames required in this problem.  Take note that if you make any changes to your code, you will need to compile it first before you see changes when running it.

```
clang++ -std=c++17 main.cpp scheduler.cpp -o main
./main
```

You can run one, two, or all the commands below to `test` your code, `stylecheck` your code's design, or `formatcheck` your work. Kindly make sure that you have compiled and executed your code before issuing any of the commands below to avoid errors.

```
make test
make stylecheck
make formatcheck
```

A faster way of running all these tests uses the `all` parameter.

```
make all
```

# Submission
1. When everything runs correctly,  let's copy your code into the Github repository. The first step is to add your code to what is called the staging area using git's `add` command. The parameter after `add` is the name of the file you want to add. There are cases when you have multiple changed files, so you can just type . (period) to add all modified files.

    ```
    git add main.cpp
    ```
1. Once everything is in the staging area, we use the `commit` command to tell git that we have added everything we need into the staging area.

    ```
    git commit
    ```

    Alternatively, you could add a comment to the `commit` command in order to s
kip the *nano* editor step described below.

    ```
    git commit -m "Finished milestone"
    ```
1. In case it asks you  to configure global variables for an email and name, just copy the commands it provides then replace the dummy text with your email and Github username.

    ```
    git config --global user.email "tuffy@csu.fullerton.edu"
    git config --global user.name "Tuffy Titan"
    ```
    When you're done, make sure you type `git commit` again.    
1. Git will ask you to describe what you have added to the staging area. By default, you will use a command-line based editor called *nano*. Go ahead and provide a description then press <kbd>Ctrl</kbd> + <kbd>x</kbd> to exit. Press <kbd>Y</kbd> to confirm that you want to make changes and then press <kbd>Enter</kbd>.
1. Lets push all changes to the Github repository using git's `push` command. Provide your Github username and password when you are asked.

    ```
    git push
    ```
1. When you finish the exercise, go back to Titanium and click on the `Add submission` button in the lab exercise page. Provide a short message in the text area such as "finished lab exercise" and click on `Save changes`. Finally click on `Submit assignment` to inform your instructor that you are done.
