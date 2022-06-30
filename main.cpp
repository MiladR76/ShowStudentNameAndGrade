/*!
 * This is a program that we can create students and work with this students
 * to  show scores and gpa and min and max score of them.
 * Create: Milad rahbari
 * Date: 1400/04/09
 * Library dependency: only stl and some base c++ library like vector and iostream and so on. There is nothing you must
 * to do
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <vector>

using namespace std;

/** \brief write breaf
 *
 *  write detials
 */

/** \brief Define a template accept only numeric general type for IScore class
 *
 *  We can only create numeric general type for IScore class. So if we create a string general type from IScore compiler
 *  will give us error
 */
#define MyTemplate template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>

class IPerson
{
    string m_name;
    string m_lastName;

public:
    IPerson() = default;
    IPerson(const string &name, const string &lastName) : m_name(name), m_lastName(lastName) {}
    virtual ~IPerson() = default;

    string name() const { return m_name; }
    void setName(const string &name) { m_name = name; }
    string lastName() const { return m_lastName; }
    void setLastName(const string &lastName) { m_lastName = lastName; }
};

/** \brief IScore class to store student score in every class
 *
 *  It is a Interface class. It have many virtual funtion but some of this virtual function is implemented.
 *  If you need a specific need for example calculate average of score in another way you can override that method
 */
MyTemplate class IScore
{
protected:
    vector<T> m_scores; /**< Store all class scores for a student */

public:
    IScore() = default;
    virtual ~IScore() = default;

    virtual T max() const
    {
        if (m_scores.size() <= 0) throw std::runtime_error("IScore::max: m_score is empty");
        return *max_element(m_scores.begin(), m_scores.end());
    }

    virtual T min() const
    {
        if (m_scores.size() <= 0) throw std::runtime_error("IScore::min: m_score is empty");
        return *min_element(m_scores.begin(), m_scores.end());
    }

    virtual void addScore(T score)
    {
        checkScore(score);
        m_scores.push_back(score);
    }

    /** Check score when we add new score to list
     *  When we add score to list we can work with different country gpa calculation system. so we maybe need to
     * override this method
     */
    virtual void checkScore(T) const {}

    /** We can work with different country gpa calculation system. so we maybe need to override this method
     */
    virtual T getAverage()
    {
        if (m_scores.size() <= 0) throw std::runtime_error("score is empty");
        return accumulate(m_scores.begin(), m_scores.end(), 0.0) / this->m_scores.size();
    }

    virtual void clear()
    {
        m_scores.clear();
        m_scores.shrink_to_fit();
    }

    virtual vector<T> getScore() const { return m_scores; }
};

/** \brief Grade Used for Iran gpa calulation system
 *
 *  It is a implementation class that implement IScore interface.
 *  We override checkScore() function in this class to check when a score added is it valid or not.
 */
MyTemplate class Grade final : public IScore<T>
{
public:
    Grade() = default;

    /** Check when a score added is it valid in Iran gpa system  or not
     */
    void checkScore(T score) const override
    {
        if (score < 0 || score > 20) throw std::runtime_error("score is bigger than 20 or little than 0");
    }
};

/** \brief Grade Used for USA gpa calulation system
 *
 *  It is a implementation class that implement IScore interface.
 *  We override checkScore() function in this class to check when a score added is it valid or not.
 */
MyTemplate class GPA final : public IScore<T>
{
public:
    /** Check when a score added is it valid in USA gpa system or not
     */
    void checkScore(T score) const override
    {
        if (score < 0 || score > 5) throw std::runtime_error("score is bigger than 5 or little than 0");
    }
};

/** \brief A GPA convert that comvert Iran GPA syatem to USA GPA system
 *
 *  It is a convert class. It calculate GPA in USA according Iran scores. We dont change Iran score we just adapt
 *  Iran data with USA GPA system
 */
MyTemplate class IranGradeToGPAAdapter final : public IScore<T>
{
    shared_ptr<IScore<T>> m_score; /**< A IScore object store Iran System scores in it */

public:
    IranGradeToGPAAdapter(shared_ptr<IScore<T>> score) : m_score(score) {}

    /** Calculate USA GPA according Iran score system. In this function we create a new scores vector and
     *  convert Iran score to USA score and after that we return USA GPA
     */
    T getAverage() override
    {
        if (this->m_score->getScore().size() <= 0) throw std::runtime_error("score is empty");

        vector<T> scoresInGPA;
        for (const auto &score : this->m_score->getScore())
        {
            if (score >= 16)
            {
                scoresInGPA.push_back(4);
            }
            else if (score >= 14)
            {
                scoresInGPA.push_back(3);
            }
            else if (score >= 12)
            {
                scoresInGPA.push_back(2);
            }
            else if (score >= 10)
            {
                scoresInGPA.push_back(1);
            }
            else
            {
                scoresInGPA.push_back(0);
            }
        }
        return accumulate(scoresInGPA.begin(), scoresInGPA.end(), 0.0) / scoresInGPA.size();
    }

    T max() const override
    {
        return m_score->max();
    } /**< Class max fucntion in m_score object. This is only implenet IScore interface*/
    T min() const override { return m_score->min(); }
    void addScore(T score) override { m_score->addScore(score); }
    void checkScore(T score) const override
    {
        if (score < 0 || score > 5) throw std::runtime_error("score is bigger than 5 or little than 0");
    }
    void clear() override { m_score->clear(); }
    vector<T> getScore() const override { return m_score->getScore(); }
};

/** \brief Null Class that implement IScore interface for only Test purpose
 *
 *  We declare NullGrade. When we want to test a Student class but we dont need GPA object we use this class.
 *  In face replace of sending a nullptr to student class we send this null class to student class.
 *  so when student class class getAverage fcuntion or other function we return 0 or any thing we need for test.
 *  This is very important class and design pattern for test purpose.
 */
MyTemplate class NullGrade final : public IScore<T>
{
public:
    T getAverage() override { return 0; }
};

/** \brief Student class that represent a student in program.
 *
 * Student class implement IPerson and composit Iscore class.
 * Student is a person and have some scores in his school.
 * Student class must show us a student information like name and last name and gpa and min and max number in his scores
 */
MyTemplate class Student final : public IPerson
{
    shared_ptr<IScore<T>> m_score = nullptr;

    Student(const Student &) = default;
    Student &operator=(const Student &) = default;

public:
    shared_ptr<IScore<T>> scoreObject() { return m_score; }

    Student(shared_ptr<IScore<T>> score) : m_score(score) {}
    Student(const string &name, const string &lastName, shared_ptr<IScore<T>> score)
        : IPerson(name, lastName), m_score(score)
    {
    }

    T getAverage() const { return m_score->getAverage(); }

    T max() const { return m_score->max(); }

    T min() const { return m_score->min(); }

    vector<T> getScore() const { return m_score->getScore(); }

    /** We need to cout student and show information.
     *  So we overload << operator
     */
    friend ostream &operator<<(ostream &os, shared_ptr<Student> s)
    {
        os << "Name: " << s->name() << ", LastName: " << s->lastName() << ", Max number: " << s->max()
           << ", Min Numer: " << s->min() << ", Average: " << s->getAverage();
        return os;
    }
    friend ostream &operator<<(ostream &os, const Student &s)
    {
        os << "Name: " << s.name() << ", LastName: " << s.lastName() << ", Max number: " << s.max()
           << ", Min Numer: " << s.min() << ", Average: " << s.getAverage();
        return os;
    }

    /** We need to check if a two student is equal or not
     *  For know two student is equal or not we check name and last name.
     */
    friend bool operator==(shared_ptr<Student> lhs, shared_ptr<Student> rhs)
    {
        if (lhs->name() == rhs->name() && lhs->lastName() == rhs->lastName()) return true;
        return false;
    }
    friend bool operator==(const Student &lhs, const Student &rhs)
    {
        if (lhs.name() == rhs.name() && lhs.lastName() == rhs.lastName()) return true;
        return false;
    }
};

/** \brief Students class that store all students in program
 *
 * Students class store all student exists in program and it manage all students in program.
 * It is reponsibile for add or remove and find student between all students.
 * We work with shared_ptr in this class. So actually when we assign('=') a shared_ptr to another sharedptr we copy from
 * student. So student is not actually student it is copy student.
 */
MyTemplate class Students final
{
    vector<shared_ptr<Student<T>>> m_students; /**< Store all students */

    Students(const Students &) = default;
    Students &operator=(const Students &) = default;

public:
    Students() = default;

    void addStudent(shared_ptr<Student<T>> student) { m_students.push_back(student); }

    /** Remove student from student list
     *  Search in student list to find specific student and remove from list and return that student
     */
    shared_ptr<Student<T>> removeStudent(shared_ptr<Student<T>> student)
    {
        for (size_t i = 0; i < m_students.size(); i++)
        {
            if (student == m_students[i])
            {
                auto temp = m_students[i];
                m_students.erase(m_students.begin() + i);
                return temp;
            }
        }

        return nullptr;
    }

    /** find student from student list
     *  Search in student list to find specific student and return that student.
     *  We return share_ptr so actually we get copy from that. Attention to this point
     */
    shared_ptr<Student<T>> findStudent(shared_ptr<Student<T>> student)
    {
        for (const auto &s : m_students)
            if (s == student) return s;
        return nullptr;
    }

    /** We need to cout students list and show information.
     *  So we overload << operator
     */
    friend ostream &operator<<(ostream &os, const Students &students)
    {
        for (const auto &student : students.m_students)
        {
            os << student << endl;
        }
        return os;
    }
    friend ostream &operator<<(ostream &os, shared_ptr<Students> students)
    {
        for (const auto &student : students->m_students)
        {
            os << student << endl;
        }
        return os;
    }
};

int main()
{
    int state = 0;

    try
    {
        Students<double> students;

        cout << "Welcom" << endl;
        while (state >= 0)
        {
            cout << "================" << endl;
            cout << "Create new student with name and last name and list of scores: Enter number '1'" << endl;
            cout << "Show all students with they information: Enter number '2'" << endl;
            cout << "Find student with student name and last name: Enter number '3'" << endl;
            cout << "Remove student with student name and last name: Enter number '4'" << endl;
            cout << "For exit press any number expect 1,2,3,4 and return button" << endl;
            cout << "================" << endl;

            cin >> state;

            if (state == 1)
            {
                string name, lastName;
                double score = 0;

                cout << "We area creating new student." << endl;
                cout << "Enter student name: " << endl;
                cin >> name;
                cout << "Enter student lastName: " << endl;
                cin >> lastName;
                cout << "Enter score or enter -1 to finish creating student" << endl;

                cin >> score;
                shared_ptr<IScore<double>> studentGrade = make_shared<Grade<double>>();
                while (score >= 0)
                {
                    studentGrade->addScore(score);
                    cin >> score;
                }
                shared_ptr<Student<double>> student = make_shared<Student<double>>(studentGrade);
                student->setName(name);
                student->setLastName(lastName);

                cout << "You created new student. Student info: " << endl;
                cout << student << endl;

                students.addStudent(student);

                cout << "List: " << endl;
                cout << students << endl;
            }

            else if (state == 2)
            {
                cout << "All students: " << endl;
                cout << students << endl;
            }

            else if (state == 3)
            {
                string name, lastName;
                cout << "Searcing for specific student." << endl;
                cout << "Enter student name: " << endl;
                cin >> name;
                cout << "Enter student lastName: " << endl;
                cin >> lastName;

                shared_ptr<IScore<double>> nullGrade = make_shared<NullGrade<double>>();
                shared_ptr<Student<double>> student = make_shared<Student<double>>(nullGrade);
                student->setName(name);
                student->setLastName(lastName);

                shared_ptr<Student<double>> s = students.findStudent(student);
                if (s)
                {
                    int showInfoState = 0;
                    cout << "Print student all information. Enter number '1': " << endl;
                    cout << "Print student grade information. Enter number '2': " << endl;
                    cout << "Print student show max and min information. Enter number '3': " << endl;
                    cout << "Print student GPA information. Enter number '4': " << endl;
                    cin >> showInfoState;
                    if (showInfoState == 1)
                    {
                        cout << s << endl;
                    }
                    if (showInfoState == 2)
                    {
                        cout << s->getAverage() << endl;
                    }
                    if (showInfoState == 3)
                    {
                        cout << "max: " << s->max() << ", min:" << s->min() << endl;
                    }

                    if (showInfoState == 4)
                    {
                        shared_ptr<IranGradeToGPAAdapter<double>> gpa =
                            make_shared<IranGradeToGPAAdapter<double>>(s->scoreObject());
                        cout << "gpa is: " << gpa->getAverage() << endl;
                    }
                }
                else
                {
                    cout << "Could not find student. check name and lastName" << endl;
                }
            }

            else if (state == 4)
            {
                string name, lastName;
                cout << "Deleting specific student." << endl;
                cout << "Enter student name: " << endl;
                cin >> name;
                cout << "Enter student lastName: ";
                cin >> lastName;

                shared_ptr<IScore<double>> nullGrade = make_shared<NullGrade<double>>();
                shared_ptr<Student<double>> student = make_shared<Student<double>>(nullGrade);
                student->setName(name);
                student->setLastName(lastName);

                auto s = students.removeStudent(student);
                if (s)
                {
                    cout << "We deleted student: " << s << endl;
                    cout << "List is: " << endl;
                    cout << students << endl;
                }
                else
                {
                    cout << "Could not find student. check name and lastName" << endl;
                }
            }

            else
            {
                cout << "================" << endl;
                cout << "Good by" << endl;
                cout << "================" << endl;
                break;
            }
        }
    }
    catch (const exception &x)
    {
        cout << "some thing gone wrong. error message: " << x.what() << endl;
    }

    return 0;
}
