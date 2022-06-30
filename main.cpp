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
    IScore() {}
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

int main()
{
    IScore<int> a;

    cout << "Hello World!" << endl;
    return 0;
}
