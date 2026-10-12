#include <iostream>
#include <string>  // Includes and using namespace std
#include <vector>  //Stores multiple objects in a dynamic collection
#include <limits>  //Provides information about data type limits               /* Includes and using namespace std */
#include <fstream> // Read and write files.
#include <sstream> // Read fields from one line of text.
using namespace std;

class Expense // Expense class

{

public:
      int id;
      string title;
      string category;
      int amount;
};

int readInteger(const string &prompt, int minimum, int maximum) // validate input, so letters or negative amounts don’t break the program.
{
      int value;

      while (true)
      {
            cout << prompt;

            if (cin >> value)
            {
                  // Discard the rest of the input line.
                  cin.ignore(numeric_limits<streamsize>::max(), '\n');

                  if (value >= minimum && value <= maximum)
                  {
                        return value;
                  }

                  cout << "Enter a number between "
                       << minimum << " and " << maximum << ".\n";
            }
            else
            {
                  // Recover after input such as "abc".
                  cin.clear();
                  cin.ignore(numeric_limits<streamsize>::max(), '\n');

                  cout << "Invalid input. Enter a whole number.\n";
            }
      }
}

void addExpense(vector<Expense> &expenses, int &nextId) // addExpense function
{

      if (nextId == numeric_limits<int>::max())
      {
            cout << "No more expense IDs are available.\n";
            return;
      }

      Expense expense; // Create a new expense.

      cout << "Enter title: ";
      getline(cin >> ws, expense.title);

      cout << "Enter category: ";
      getline(cin >> ws, expense.category);

      if (expense.title.find('|') != string::npos ||
          expense.category.find('|') != string::npos)
      {
            cout << "Titles and categories cannot contain '|'.\n";
            return;
      }

      expense.amount = readInteger(
          "Enter amount in PKR: ",
          1,
          numeric_limits<int>::max());

      expense.id = nextId;         // Assign a unique ID.
      expenses.push_back(expense); // Store a copy in the vector.
      nextId++;                    // Prepare the ID for the next expense.

      cout << "Expense added successfully!\n";
}

void viewExpenses(const vector<Expense> &expenses) // View Expense Function
{
      // Handle an empty collection.
      if (expenses.empty())
      {
            cout << "No expenses yet.\n";
            return;
      }
      cout << "You have expenses: " << expenses.size() << '\n';
      
      for (const Expense &expense : expenses)
      {
            cout << "ID: " << expense.id << '\n';
            cout << "Title: " << expense.title << '\n';
            cout << "Category: " << expense.category << '\n';
            cout << "Amount: PKR " << expense.amount << '\n';
            cout << "--------------------\n";
      }
}

long long calculateTotal(const vector<Expense>& expenses)  // Total Expense Function
{
    long long total = 0;

    for (const Expense& expense : expenses)
    {
        total += expense.amount;
    }

    return total;
}

bool saveExpenses(const vector<Expense> &expenses) // save function
{
      ofstream file("expenses.txt");

      if (!file)
      {
            cout << "Could not open the file for saving.\n";
            return false;
      }

      for (const Expense &expense : expenses)
      {
            file << expense.id << '|'
                 << expense.title << '|'
                 << expense.category << '|'
                 << expense.amount << '\n';
      }

      file.close();

      if (!file)
      {
            cout << "Saving failed.\n";
            return false;
      }

      return true;
}

void loadExpenses(vector<Expense> &expenses, int &nextId) // Load Function
{
      ifstream file("expenses.txt");

      // On the first run, the file may not exist yet.
      if (!file)
      {
            return;
      }

      string line;

      while (getline(file, line))
      {
            stringstream record(line);
            Expense expense;
            char separator;

            // Read the ID and its following separator.
            if (!(record >> expense.id >> separator) || separator != '|')
            {
                  continue;
            }

            // Read text up to the next separator.
            if (!getline(record, expense.title, '|') ||
                !getline(record, expense.category, '|'))
            {
                  continue;
            }

            if (!(record >> expense.amount))
            {
                  continue;
            }

            // Reject extra characters after the amount.
            record >> ws;

            if (!record.eof() ||
                expense.id <= 0 ||
                expense.id == numeric_limits<int>::max() ||
                expense.amount <= 0 ||
                expense.title.empty() ||
                expense.category.empty())
            {
                  continue;
            }

            // Reject duplicate IDs.
            bool duplicate = false;

            for (const Expense &existing : expenses)
            {
                  if (existing.id == expense.id)
                  {
                        duplicate = true;
                        break;
                  }
            }

            if (duplicate)
            {
                  continue;
            }

            expenses.push_back(expense);

            if (expense.id >= nextId)
            {
                  nextId = expense.id + 1;
            }
      }
}

void showCategoryTotal(const vector<Expense> &expenses)  //showCategoryTotal function
{
      string category;
      long long total = 0;
      int count = 0;

      cout << "Enter category: ";
      getline(cin >> ws, category);

      for (const Expense &expense : expenses)
      {
            if (expense.category == category)
            {
                  total += expense.amount;
                  count++;
            }
      }

      if (count == 0)
      {
            cout << "No expenses found in this category.\n";
            return;
      }

      cout << "Category: " << category << '\n';
      cout << "Number of expenses: " << count << '\n';
      cout << "Total: PKR " << total << '\n';
}
bool deleteExpense(vector<Expense>& expenses)   // Delete function
{
    if (expenses.empty())
    {
        cout << "No expenses to delete.\n";
        return false;
    }

    int id = readInteger(
        "Enter the expense ID to delete: ",
        1,
        numeric_limits<int>::max()
    );

    for (auto it = expenses.begin(); it != expenses.end(); ++it)
    {
        if (it->id == id)
        {
            cout << "Selected: " << it->title
                 << " | PKR " << it->amount << '\n';

            int confirm = readInteger(
                "Delete this expense? 1 = Yes, 0 = Cancel: ",
                0,
                1
            );

            if (confirm == 0)
            {
                cout << "Deletion cancelled.\n";
                return false;
            }

            expenses.erase(it);
            cout << "Expense deleted from the list.\n";
            return true;
        }
    }

    cout << "No expense found with that ID.\n";
    return false;
}
bool editExpense(vector<Expense>& expenses) // Edit expense function
{
    if (expenses.empty())
    {
        cout << "No expenses to edit.\n";
        return false;
    }

    int id = readInteger(
        "Enter the expense ID to edit: ",
        1,
        numeric_limits<int>::max()
    );

    for (Expense& expense : expenses)
    {
        if (expense.id == id)
        {
            cout << "Current title: " << expense.title << '\n';
            cout << "Current category: " << expense.category << '\n';
            cout << "Current amount: PKR " << expense.amount << '\n';

            // Work on a copy before changing the original.
            Expense updated = expense;

            cout << "Enter new title: ";
            getline(cin >> ws, updated.title);

            cout << "Enter new category: ";
            getline(cin >> ws, updated.category);

            if (updated.title.find('|') != string::npos ||
                updated.category.find('|') != string::npos)
            {
                cout << "Titles and categories cannot contain '|'.\n";
                return false;
            }

            updated.amount = readInteger(
                "Enter new amount in PKR: ",
                1,
                numeric_limits<int>::max()
            );

            int confirm = readInteger(
                "Save these changes? 1 = Yes, 0 = Cancel: ",
                0,
                1
            );

            if (confirm == 0)
            {
                cout << "Editing cancelled.\n";
                return false;
            }

            // Replace the original record, keeping its ID.
            expense = updated;

            cout << "Expense updated in the list.\n";
            return true;
        }
    }

             cout << "No expense found with that ID.\n";
    return false;
            }
int main()
{
      cout << "Expense Tracker\n";

      vector<Expense> expenses;
      int nextId = 1;
      int choice = -1;

      // Restore saved expenses and update the next available ID.
      loadExpenses(expenses, nextId);

      do
      {
            cout << "\n--- Expense Tracker ---\n";
            cout << "1. Add expense\n";
            cout << "2. View expenses\n";
            cout << "3. Show total\n";
            cout << "4. Show total by category\n";
            cout << "5. Delete expense\n";
            cout << "6. Edit expense\n";
            cout << "0. Exit\n";

            choice = readInteger("Enter your choice: ", 0, 6);

            switch (choice)
            {
            case 1:
            {
                  // Remember the count so we save only after an addition.
                  size_t previousCount = expenses.size();

                  addExpense(expenses, nextId);

                  if (expenses.size() > previousCount &&
                      saveExpenses(expenses))
                  {
                        cout << "Expenses saved to file.\n";
                  }
                  break;
            }

            case 2:
                  viewExpenses(expenses);
                  break;

            case 3:
                  cout << "Total expenses: PKR "
                       << calculateTotal(expenses) << '\n';
                  break;
            case 4:
                        showCategoryTotal(expenses);
                   break;      
            case 5:
                        if (deleteExpense(expenses))
                    {
                        if (saveExpenses(expenses))
                          {
                         cout << "Deletion saved to file.\n";
                      }
                        }
                   break;
             case 6:
                         if (editExpense(expenses))
                           {
                                if (saveExpenses(expenses))
                    {
                           cout << "Changes saved to file.\n";
              }
    }
    break;
            case 0:
                  cout << "Goodbye!\n";
                  break;
            }

      } while (choice != 0);

      return 0;
}