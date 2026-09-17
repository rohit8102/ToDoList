#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include <limits>

using namespace std;

void setColor(int color)
{
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        color
    );
}

void resetColor()
{
    setColor(7);
}

struct Task
{
    int id;
    string title;
    string priority;
    bool completed;
    string date;
};

class TodoApp
{
private:

    vector<Task> tasks;

    string filename = "tasks.txt";

    int nextId = 1;

public:

    TodoApp()
    {
        loadTasks();
    }

    string getDateTime()
    {
        time_t now = time(0);

        tm* localTime = localtime(&now);

        stringstream ss;

        ss << put_time(localTime, "%d-%m-%Y %H:%M");

        return ss.str();
    }

    void saveTasks()
    {
        ofstream file(filename);

        if (!file)
        {
            setColor(12);

            cout << "\nError: Cannot open tasks.txt\n";

            resetColor();

            return;
        }

        for (const auto& task : tasks)
        {
            file << task.id << "|"
                 << task.title << "|"
                 << task.priority << "|"
                 << task.completed << "|"
                 << task.date << "\n";
        }

        file.close();
    }

    void loadTasks()
    {
        ifstream file(filename);

        if (!file)
        {
            return;
        }

        string line;

        while (getline(file, line))
        {
            stringstream ss(line);

            string id;
            string title;
            string priority;
            string completed;
            string date;

            getline(ss, id, '|');
            getline(ss, title, '|');
            getline(ss, priority, '|');
            getline(ss, completed, '|');
            getline(ss, date, '|');

            if (id.empty() || title.empty())
            {
                continue;
            }

            Task task;

            task.id = stoi(id);
            task.title = title;
            task.priority = priority;
            task.completed = (completed == "1");
            task.date = date;

            tasks.push_back(task);

            if (task.id >= nextId)
            {
                nextId = task.id + 1;
            }
        }

        file.close();
    }

    void header()
    {
        system("cls");

        setColor(11);

        cout << "\n";
        cout << "============================================================\n";
        cout << "                    TODO LIST APPLICATION                  \n";
        cout << "============================================================\n";

        resetColor();

        setColor(8);

        cout << "              C++ | File Handling | OOP\n";

        resetColor();

        cout << "\n";
    }

    void addTask()
    {
        header();

        Task task;

        task.id = nextId++;

        cout << "Enter your task title: ";

        getline(cin, task.title);

        if (task.title.empty())
        {
            setColor(12);

            cout << "\nTask title cannot be empty!\n";

            resetColor();

            system("pause");

            return;
        }

        cout << "\nSelect Priority:\n";

        cout << "1. High\n";
        cout << "2. Medium\n";
        cout << "3. Low\n";

        int choice;

        cout << "\nEnter choice: ";

        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
            case 1:
                task.priority = "High";
                break;

            case 2:
                task.priority = "Medium";
                break;

            case 3:
                task.priority = "Low";
                break;

            default:
                task.priority = "Medium";
        }

        task.completed = false;

        task.date = getDateTime();

        tasks.push_back(task);

        saveTasks();

        setColor(10);

        cout << "\nTask added successfully!\n";

        resetColor();

        system("pause");
    }

    void viewTasks()
    {
        header();

        if (tasks.empty())
        {
            setColor(14);

            cout << "No tasks available.\n";

            resetColor();

            system("pause");

            return;
        }

        cout << left
             << setw(5) << "ID"
             << setw(30) << "TASK"
             << setw(12) << "PRIORITY"
             << setw(15) << "STATUS"
             << "DATE\n";

        cout << "------------------------------------------------------------\n";

        for (const auto& task : tasks)
        {
            cout << left
                 << setw(5) << task.id
                 << setw(30) << task.title
                 << setw(12) << task.priority;

            if (task.completed)
            {
                setColor(10);

                cout << setw(15) << "Completed";

                resetColor();
            }
            else
            {
                setColor(14);

                cout << setw(15) << "Pending";

                resetColor();
            }

            cout << task.date << "\n";
        }

        cout << "\n";

        system("pause");
    }

    void completeTask()
    {
        header();

        if (tasks.empty())
        {
            setColor(14);

            cout << "No tasks available.\n";

            resetColor();

            system("pause");

            return;
        }

        int id;

        cout << "Enter Task ID to mark completed: ";

        cin >> id;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        bool found = false;

        for (auto& task : tasks)
        {
            if (task.id == id)
            {
                task.completed = true;

                found = true;

                break;
            }
        }

        if (found)
        {
            saveTasks();

            setColor(10);

            cout << "\nTask marked as completed!\n";

            resetColor();
        }
        else
        {
            setColor(12);

            cout << "\nTask ID not found!\n";

            resetColor();
        }

        system("pause");
    }

    void deleteTask()
    {
        header();

        if (tasks.empty())
        {
            setColor(14);

            cout << "No tasks available.\n";

            resetColor();

            system("pause");

            return;
        }

        int id;

        cout << "Enter Task ID to delete: ";

        cin >> id;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        bool found = false;

        for (auto it = tasks.begin(); it != tasks.end(); ++it)
        {
            if (it->id == id)
            {
                tasks.erase(it);

                found = true;

                break;
            }
        }

        if (found)
        {
            saveTasks();

            setColor(10);

            cout << "\nTask deleted successfully!\n";

            resetColor();
        }
        else
        {
            setColor(12);

            cout << "\nTask ID not found!\n";

            resetColor();
        }

        system("pause");
    }

    void statistics()
    {
        header();

        int completed = 0;
        int pending = 0;

        for (const auto& task : tasks)
        {
            if (task.completed)
                completed++;
            else
                pending++;
        }

        cout << "Total Tasks     : " << tasks.size() << "\n";

        setColor(10);

        cout << "Completed Tasks : " << completed << "\n";

        resetColor();

        setColor(14);

        cout << "Pending Tasks   : " << pending << "\n";

        resetColor();

        cout << "\n";

        system("pause");
    }

    void run()
    {
        int choice;

        do
        {
            header();

            setColor(11);

            cout << "                    MAIN MENU\n";

            resetColor();

            cout << "\n";

            cout << "  [1]  Add New Task\n";

            cout << "  [2]  View All Tasks\n";

            cout << "  [3]  Mark Task Completed\n";

            cout << "  [4]  Delete Task\n";

            cout << "  [5]  Task Statistics\n";

            cout << "  [6]  Exit Application\n";

            cout << "\n";

            cout << "------------------------------------------------------------\n";

            cout << "Enter your choice: ";

            cin >> choice;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
                case 1:
                    addTask();
                    break;

                case 2:
                    viewTasks();
                    break;

                case 3:
                    completeTask();
                    break;

                case 4:
                    deleteTask();
                    break;

                case 5:
                    statistics();
                    break;

                case 6:

                    header();

                    setColor(10);

                    cout << "Thank you for using Todo App!\n";

                    resetColor();

                    break;

                default:

                    setColor(12);

                    cout << "\nInvalid choice!\n";

                    resetColor();

                    system("pause");
            }

        } while (choice != 6);
    }
};

int main()
{
    TodoApp app;

    app.run();

    return 0;
}