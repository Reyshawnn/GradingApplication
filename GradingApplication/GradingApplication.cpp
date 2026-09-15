
#include <iostream>
#include <string>
#include<SQLiteCpp/SQLiteCpp.h>
#include <format>
#include <vector>




struct Student 
{

    std::string studentName{};
    int studentID {}; 
    std::string class_ID{};
    std::string assignment_ID{}; 

    Student(std::string setName, int setID)
    {
        studentName = setName;
        studentID = setID;

    }

    //prompt user for name id etc so the object is made with those values

    std::vector <std::string> subAssignment(int student_ID, std::string assignment_ID)
    { 
        std::string query1 = std::format("Insert into student_assignments (Student_ID,Assignment_ID) Values ({},'{}')", student_ID,assignment_ID);
        std::string query2 = std::format("UPDATE student_assignments SET Total_Points = (SELECT a.Total_Points FROM Assignments a WHERE a.Assignment_ID = student_assignments.Assignment_ID)");
        return { query1,query2 };
    }

    std::string joinClass(int student_ID, std::string className)
    {
        std::string query1 = std::format("Insert into student_classes (Student_ID, Class_ID) values ({},'{}')", student_ID, className);
        std::system("cls");
        std::cout << "You've joined " << className << "\n";
        return query1;
    }

    std::string withdrawClass (int student_ID, std::string className)
    {
        std::string query1 = std::format("Delete from student_classes where student_ID = {} and Class_ID = '{}' ", student_ID, className);
        return query1;
    }
    
    void decisionSystem(SQLite::Database& dbName)
    {
        int userChoice{};
        std::cin >> userChoice;
        if (userChoice == 1)
        {
            std::system("cls");
            std::cout << "Enter the Assignment ID" << "\n";
            std::cin >> assignment_ID;


            std::cout << "Assignment Submitted" << "\n";
            for (std::string query : subAssignment(studentID, assignment_ID))
            {
                dbName.exec(query);
            }
            
        }
        else if (userChoice == 2)
        {
            std::system("cls");
            std::cout << "Enter the Class ID you wish to join" << "\n";
            std::cin >> class_ID;
           
            dbName.exec(joinClass(studentID, class_ID));
        }
        else if (userChoice == 3)
        {
            std::system("cls");
            std::cout << "Enter the Class ID you wish to withdraw from" << "\n";
            std::cin >> class_ID;

            dbName.exec(withdrawClass(studentID, class_ID));
            std::system("cls");
            std::cout << "You've withdrawn from " << class_ID << "\n";

        }

        else if (userChoice == 4)
        {
            std::system("cls");
            std::exit;
        }
        
    }

    void nameDisplay()
    {
        std::system("cls");
        std::cout << "STUDENT MENU - STUDENT NAME: { " << studentName << " } STUDENT ID: { " << studentID <<  " }"  << "\n";
    }
};



struct Admin
{
    std::string className{};
    std::string class_ID{};
    std::vector <std::string> multiQ{};
  
    Admin(std::string setName, std::string setID)
    {
        className = setName;
        class_ID = setID;

    }

    std::string addClass(std::string name, std::string classID) 
    {
        std::string addQuery = std::format("Insert into classes (Class_ID, Name) Values ('{}','{}') on conflict(Class_ID) do nothing", classID, name);
        return addQuery;
    }

    std::vector <std::string> deleteClass(std::string ClassID)
    {
        std::string delQuery1 = std::format("Delete from classes where Class_ID = '{}'", ClassID);
        std::string delQuery2 = std::format("Delete from student_classes where Class_ID = '{}' ", ClassID);
        return { delQuery1, delQuery2 };
     
    }

    std::vector <std::string> addAssignment(std::string Assignment_ID, std::string ClassName, std::string AssignmentName, int points, std::string type)
    { 
        std::string addQuery1 = std::format("Insert Into assignments (Assignment_ID, Class_ID, Name, Total_Points, Type) Values ('{}','{}','{}',{},'{}')", Assignment_ID, AssignmentName,ClassName,points, type);
        return { addQuery1};
    }
   

    std::vector <std::string> deleteStudent(int Student_ID)
    {
        std::string delQuery1 = std::format("Delete from students where Student_ID = {}", Student_ID);
        std::string delQuery2 = std::format("Delete from student_classes where Student_ID = {} ", Student_ID);
        std::string delQuery3 = std::format("Delete from student_assignments where Student_ID = {} ", Student_ID);
        std::cout << "Student Deleted" << "\n";
        return { delQuery1, delQuery2, delQuery3 };

    }


    std::string gradeAssignment(int Student_ID, std::string Assignment_ID,int Score)
    {
        std::string gradeQuery1 = std::format("Update student_assignments set Score = {} where Student_ID = {} and Assignment_ID = '{}' ",Score,Student_ID,Assignment_ID);
        return  gradeQuery1;

    }


   

    void decisionSystem(SQLite::Database &dbName)
    {
        int userChoice{};
        std::cin >> userChoice;
        if (userChoice == 1)
        {
            std::system("cls");
            std::cout << "You added the class: " << className << " (" << class_ID << ")" << "\n"; 
             dbName.exec(addClass(class_ID, className));
            
        }
        else if (userChoice == 2)
        {
            std::system("cls");
            std::cout << class_ID << " Was Deleted" << "\n";
            for (std::string query : deleteClass(class_ID))
            {
                dbName.exec(query);
            }

           
        }
        else if (userChoice == 3)
        {
            std::string assignmentName{};
            std::string assignmentID{};
            int points{};
            std::string type{};

            std::system("cls");
            std::cout << "Type in the Assignment Name" << "\n";
            std::getline(std::cin >> std::ws, assignmentName);

            std::system("cls");
            std::cout << "Type in the assignmentID" << "\n";
            std::cin >> assignmentID;

            std::system("cls");
            std::cout << "Type in the points the assignment is worth" << "\n";
            std::cin >> points;

            std::system("cls");
            std::cout << "Type in the assignment type" << "\n";
            std::cin >> type;


            for (std::string query : addAssignment(assignmentID, assignmentName,class_ID,points, type))
            {
                dbName.exec(query);
            }
            std::cout << "Assignment Added" << "\n";

       
          
        }

        else if (userChoice == 4) //grade
        {
            int ID{};
            std::string Assignment_ID{};
            int Score{};

            std::system("cls");
            std::cout << "Enter the Student ID of who you are grading" << "\n";
            std::cin >> ID;

            std::system("cls");
            std::cout << "Enter the Assignment_ID you are grading" << "\n";
            std::cin >> Assignment_ID;

            std::system("cls");
            std::cout << "Enter the Student's score" << "\n";
            std::cin >> Score;


            dbName.exec(gradeAssignment(ID, Assignment_ID, Score));
            std::system("cls");
            std::cout << "Assignment graded" << "\n";

        }


        else if (userChoice == 5) //edit
        {
            
            std::string Assignment_ID{};
            std::string AssignmentName{};
            int TotalPoints{};
            std::string type{};
            std::string response2{};


            std::system("cls");
            std::cout << "Enter the Assignment_ID you are editing" << "\n";
            std::cin >> Assignment_ID;


            std::system("cls");
            std::cout << "How would you like to edit " << Assignment_ID << " ?" << "\n";
            std::cout << "-------------------" << "\n";
            std::cout << "1. Name" << "\n";
            std::cout << "2. Total Points" << "\n";
            std::cout << "3. Assignment Type" << "\n";
            std::cout << "4. Delete Assignment" << "\n";
           
            std::cin >> response2;

            if (response2 == "1")
            {
                std::system("cls");
                std::cout << "Enter the new name for the assignment" << "\n";
                std::cin >> AssignmentName;


                std::string query1 = std::format("Update Assignments set Name = '{}' where Assignment_ID = '{}' ", AssignmentName, Assignment_ID);
                dbName.exec(query1);
            }

            else if (response2 == "2")
            {
                std::system("cls");
                std::cout << "Enter the new Total Points value" << "\n";
                std::cin >> TotalPoints;


                std::string query1 = std::format("Update Assignments set Total_Points = {} where Assignment_ID = '{}' ", TotalPoints, Assignment_ID);
                dbName.exec(query1);
            }


            else if (response2 == "3")
            {
                std::system("cls");
                std::cout << "Enter the new type (HW/TS) for the assignment" << "\n";
                std::cin >> type;


                std::string query1 = std::format("Update Assignments set Name = '{}' where Assignment_ID = '{}' ", type, Assignment_ID);
                dbName.exec(query1);
            }
            

            else if (response2 == "4")
            {

                std::string query1 = std::format("Delete from Assignments where Assignment_ID = '{}' ", Assignment_ID);
                std::string query2 = std::format("Delete from Student_Assignments where Assignment_ID = '{}' ", Assignment_ID);
                dbName.exec(query1);
                dbName.exec(query2);
            }




        }



        else if (userChoice == 6)
        { 
            int ID{};
         
            std::system("cls");
            std::cout << "Enter the Student ID that will be deleted" << "\n";
            std::cin >> ID;



            for (std::string query : deleteStudent(ID)) 
            {
                dbName.exec(query);
            }
        }

        else if (userChoice == 7)
        {
            std::system("cls");
            std::exit;
         }
    }

    void nameDisplay()
    {
        std::system("cls");
        std::cout << "ADMIN MENU - Class Name: { " << className << " } Class ID: { " << class_ID << " }" << "\n";
    }

};


struct sysOptions
{
    void displaySMenu()
    {
        
        std::cout << "Options" << "\n";
        std::cout << "-------------------" << "\n";
        std::cout << "1. Submit assignment" << "\n";
        std::cout << "2. Register for a class" << "\n";
        std::cout << "3. Drop a class" << "\n";
        std::cout << "4. Start Over" << "\n";


    }

    void displayAMenu()
    {
       
        std::cout << "Options" << "\n";
        std::cout << "-------------------" << "\n";
        std::cout << "1. Add new Class" << "\n";
        std::cout << "2. Delete Class" << "\n";
        std::cout << "3. Add Assignment" << "\n";
        std::cout << "4. Grade Assignment" << "\n";
        std::cout << "5. Edit Assignment" << "\n";
        std::cout << "6. Remove a student" << "\n";
        std::cout << "7. Start Over" << "\n";
    }


};







    int main()
    {

        try {


            SQLite::Database db("GradingAPP.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
            std::string query1{ "CREATE TABLE IF NOT EXISTS Students (Student_ID INT, Name VARCHAR(20), PRIMARY KEY (Student_ID));" };
            std::string query2{ "CREATE TABLE IF NOT EXISTS Classes (Class_ID VARCHAR(10), Name VARCHAR(20), Total_Points INT, PRIMARY KEY (Class_ID));" };
            std::string query3{ "CREATE TABLE IF NOT EXISTS Assignments (Assignment_ID VARCHAR(10), Class_ID VARCHAR(10), Name VARCHAR(20), Total_Points INT, Type VARCHAR(2), PRIMARY KEY (Assignment_ID), FOREIGN KEY(Class_ID) REFERENCES Classes(Class_ID));" };
            std::string query4{ "CREATE TABLE IF NOT EXISTS Student_Classes(Student_ID INT, Class_ID VARCHAR(10),  FOREIGN KEY(Student_ID) REFERENCES Students(Student_ID), FOREIGN KEY(Class_ID) REFERENCES Classes(Class_ID));" };
            std::string query5{ "CREATE TABLE IF NOT EXISTS Student_Assignments (Student_ID INT, Assignment_ID VARCHAR(10), Score INT, Total_Points INT, FOREIGN KEY (Student_ID) REFERENCES Students(Student_ID), FOREIGN KEY (Assignment_ID) REFERENCES Assignments(Assignment_ID), CHECK (Score >= 0 AND Score <= Total_Points));" };
            std::string query6{ "DROP VIEW IF EXISTS Grades;" };
            std::string query7{ "CREATE VIEW IF NOT EXISTS Grades AS SELECT sa.Student_ID, sa.Assignment_ID, SUM(sa.Score) AS score, SUM(a.Total_Points) AS total, (SUM(sa.Score) * 100.0 / SUM(a.Total_Points)) AS percent FROM Student_Assignments sa JOIN Assignments a ON sa.Assignment_ID = a.Assignment_ID GROUP BY sa.Student_ID, sa.Assignment_ID;" };

            std::vector<std::string> queryList{ query1,query2,query3,query4,query5, query6, query7 };

            for (std::string query : queryList)
            {
                db.exec(query);
            }


            sysOptions menu;


            
           
            while (true) 
            {
                std::string userResponse{};
                std::string Name{};
                int ID{};
                std::string IDa{};
                std::string userResponseE{};
                std::cout << "Are you a Student or an Admin?" << "\n";
                std::cout << "Type S for student and A for admin" << "\n";
                std::cin >> userResponse;


                if (userResponse == "S" or userResponse == "s")
                {
                    std::system("cls");
                    std::cout << "enter your Name" << "\n";
                    std::getline(std::cin >> std::ws, Name);

                    std::cout << "enter your ID" << "\n";
                    std::cin >> ID;

                    std::string checkQ = std::format("Insert into Students (Student_ID, Name) Values ({},'{}') on conflict(Student_ID) do nothing", ID, Name);
                    db.exec(checkQ);
                    Student userStudent(Name, ID);

                    userStudent.nameDisplay();
                    menu.displaySMenu();
                    userStudent.decisionSystem(db);

                    std::cout << "Quit or Start Over? (Q / S)" << "\n";
                    std::cout << "-------------------" << "\n";
                    std::cin >> userResponseE;
              
                    if (userResponseE == "Q" or userResponseE == "q")
                    {
                        std::system("cls");
                        std::cout << "Goodbye" << "\n";
                        break;
                    }

                    else if (userResponseE == "S" or userResponseE == "s")
                    {
                        std::system("cls");
                        continue;
                    }

                    else 
                    {
                        std::system("cls");
                        std::cout << "Invalid" << "\n";
                        break;
                    }




                }

                else if (userResponse == "A" or userResponse == "a")
                {
                    std::system("cls");
                    std::cout << "enter the class name" << "\n";
                    std::getline(std::cin >> std::ws, Name);

                    std::cout << "enter the class ID" << "\n";
                    std::cin >> IDa;

                    Admin userAdmin(Name, IDa);

                    userAdmin.nameDisplay();
                    menu.displayAMenu();

                    userAdmin.decisionSystem(db);



                    std::cout << "Quit or Start Over? (Q / S)" << "\n";
                    std::cout << "-------------------" << "\n";
                    std::cin >> userResponseE;
                    if (userResponseE == "Q" or userResponseE == "q")
                    {
                        std::system("cls");
                        std::cout << "Goodbye" << "\n";
                        break;
                    }

                    else if (userResponseE == "S" or userResponseE == "s")
                    {
                        std::system("cls");
                        continue;
                    }

                    else
                    {
                        std::system("cls");
                        std::cout << "Invalid" << "\n";
                        break;
                    }


                }

                else
                {
                    std::system("cls");
                    std::cout << "Invalid response" << "\n";
                    break;
                }
            }
        }

        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
            
        }


    }






