#include <iostream>
#include <vector>

using namespace std;

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

MyTemplate class IScore
{
protected:
    vector<T> m_scores;

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

    // It can be Iran grade system or GPA or ECTS
    virtual void checkScore(T) const {}

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

int main()
{
    cout << "Hello World!" << endl;
    return 0;
}
