#include <initializer_list>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
using namespace std;

template<typename T>
class Matrix {
  
    int n;
    int m;
    vector<vector<T>> v;
    
    public:
        Matrix(); 
        ~Matrix();
        Matrix(istream& fin);
        Matrix(Matrix<T>&& it);
        Matrix(const Matrix<T>& it);
        Matrix(const int& a, const int& b);  
        Matrix(vector<vector<T>>&& it);
        Matrix(initializer_list<pair<vector<int>, T>> it);
        Matrix<T>& operator=(Matrix<T>&& it); 
        Matrix<T>& operator=(const Matrix<T>& it); 
        void matrix_mul(Matrix<T>& res,  const Matrix<T>& m1, const Matrix<T>& m2, int k);
        void scalar_mul(Matrix<T>& res, const Matrix<T>& m, const int& a, int k);
        Matrix<T> operator*(const Matrix<T>& it);
        Matrix<T> operator*(const int& a);
        bool operator!=(const int& a);
        void comp_c(const Matrix<T>& m1, const int a, bool& z, int i);
        bool operator==(const int& a);
        bool operator!=(const Matrix<T>& it);
        void comp(const Matrix<T>& m1, const Matrix<T>& m2, bool& z, int i);
        bool operator==(const Matrix<T>& it);
        
        Matrix<T> operator+(const Matrix<T>& it);
        void sum(Matrix<T>& res, const Matrix<T>& m1, const Matrix<T>& m2, int k);
        Matrix<T> operator-(const Matrix<T>& it);
        void subt(Matrix<T>& res, const Matrix<T>& m1, const Matrix<T>& m2, int k);
        void last(float koef, vector<vector<float>>& vec, int i, const Matrix<T>& m1);
        Matrix<float> operator!();
//Friends>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        friend bool operator==(const int& a, const Matrix<T>& it) {
            if (a == 0 || a == 1)
                return it == a;
            abort();
        }

        friend bool operator!=(const int& a, const Matrix<T>& it) {
            return it != a;
        }

       friend Matrix<T> operator*(const int& a, Matrix<T>& it) {
           return it * a;
       }

      friend istream& operator>>(istream &in, Matrix<T>& it) {
          for (int i = 0; i < it.n; ++i)
              for (int j = 0; j < it.m; ++j)
                  in >> it.v[i][j];
          return in;
      }

      friend ostream& operator<<(ostream &out, const Matrix<T>& it) {
          for (int i = 0; i < it.n; ++i) {
              for (int j = 0; j < it.m; ++j)
                  out << fixed << setprecision(3) << setw(8) << it.v[i][j];
              out << "\n";
         }
         out << "\n";
         return out;
      }
//Static>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    static Matrix<T> unit(const int& a) {
        Matrix<int> e(a, a);
        for (int i = 0; i < e.n; ++i) 
            e.v[i][i] = 1;
        return e;
    }
    static Matrix<T> zero(const int& a, const int& b) {
        Matrix<int> it(a, b);
        return it;
    }
}; 
//Constructors>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
template<typename T>
Matrix<T>::Matrix(istream& in) {
    in >> n >> m;
    v = vector<vector<T>> (n, vector<T> (m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            in >> v[i][j];
}

template<typename T>
Matrix<T>::Matrix() {
    n = 0;
    m = 0;
    v = vector<vector<T>> (n, vector<T> (m));
}

template<typename T>
Matrix<T>::Matrix(const int& a, const int& b) {
    n = a;
    m = b;
    v = vector<vector<T>> (n, vector<T> (m));
}

template<typename T>
Matrix<T>::Matrix(Matrix<T>&& it) {
    (*this) = move(it);
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& it) {
    (*this) = it;
}

//MOVE_SEMANTICS
template<typename T>
Matrix<T>::Matrix(vector<vector<T>>&& it) {
    n = move(it.size());
    m = move(it[0].size());
    v = vector<vector<T>> (n, vector<T> (m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            v[i][j] = move(it[i][j]);
}

//INITIALIZER_LIST
template<typename T>
Matrix<T>::Matrix(initializer_list<pair<vector<int>, T>> it) {
    if (it.size() == 0) {
        n = 0;
        m = 0;
        v = vector<vector<T>> (n, vector<T> (m));
    }
    n = m = 0;
    for (auto e : it) {
        if (e.first[0] > n) n = e.first[0] + 1;
        if (e.first[1] > m) m = e.first[1] + 1;
    }
    v = vector<vector<T>> (n, vector<T> (m));
    for (auto e : it) {
        v[e.first[0]][e.first[1]] = e.second;
    }
}

template<typename T>
Matrix<T>::~Matrix() {}

//Operators>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//MOVE_SEMANTICS
template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& it) {
    if (this != &it) {
        n = move(it.n);
        m = move(it.m);
        v.resize(n);
        for (int i = 0; i < n; ++i)
            v[i].resize(m);
        for (int i = 0; i < it.n; ++i)
            for (int j = 0; j < it.m; ++j)
                v[i][j] = move(it.v[i][j]);
    }
    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& it) {
    if (this != &it) {
        n = it.n;
        m = it.m;
        v.resize(n);
        for (int i = 0; i < n; ++i)
            v[i].resize(m);
        for (int i = 0; i < it.n; ++i)
            for (int j = 0; j < it.m; ++j)
                v[i][j] = it.v[i][j];
    }
    return *this;
}


template<typename T>
void Matrix<T>::comp(const Matrix<T>& m1, const Matrix<T>& m2, bool& z, int i) {
    for (int j = 0; j < m1.m; ++j)
        if (m1.v[i][j] != m2.v[i][j]) { z = false; break; }
}

template<typename T>
bool Matrix<T>::operator==(const Matrix<T>& it) {
    if (n == it.n && m == it.m)
    {
        bool z = true;
        thread* th[n];
        for (int i = 0; i < n; ++i)
            th[i] = new thread([this, &it, &z, i](){comp(*this, it, z, i);});
        for (int i = 0; i < n; ++i)
            th[i]->join();
        if (z) return true;
    }
    return false;
}

template<typename T>
bool Matrix<T>::operator!=(const Matrix<T>& it) {
    return !(it == *this);
}

template<typename T>
void Matrix<T>::comp_c(const Matrix<T>& m1, const int a, bool& z, int i) {
    for (int j = 0; j < m1.m; ++j)
        if (a == 0 && a != m1.v[i][j]) { 
            z = false; 
            return; 
        } else if (a == 1 && (a != m1.v[i][i] || (i != j && m1.v[i][j]))) {
                z = false; 
                return;
            }
}

template<typename T>
bool Matrix<T>::operator==(const int& a) {
    if (a == 0 || a == 1) {
        thread* th[n];
        bool z = true;
        for (int i = 0; i < n; ++i)
            th[i] = new thread([this, a, &z, i](){comp_c(*this, a, z, i);});
        for (int i = 0; i < n; ++i)
            th[i]->join();
        if (z) return true;
        else return false;
    }
    abort();
}

template<typename T>
bool Matrix<T>::operator!=(const int& a) {
    return !((*this) == a);
}

//Threads>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
template<typename T>
void Matrix<T>::matrix_mul(Matrix<T>& res, const Matrix<T>& m1, const Matrix<T>& m2, int k) {
    for (int i = 0; i < res.m; ++i)
        for (int j = 0; j < m1.m; ++j)  
            res.v[k][i] += m1.v[k][j] * m2.v[j][i];
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& it) {
    if (m != it.n) abort();
    Matrix<T> res(n, it.m);
    thread* th[res.n];
    for (int i = 0; i < res.n; ++i) 
        th[i] = new thread([&res, this, &it, i](){matrix_mul(res, *this, it, i);});
    for (int i = 0; i < res.n; ++i)
        th[i]->join();
    return res;
}

template<typename T>
void Matrix<T>::scalar_mul(Matrix<T>& res, const Matrix<T>& m, const int& a, int k) {
    for (int i = 0; i < res.m; ++i)
        res.v[k][i] = m.v[k][i] * a;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const int &a) {
    Matrix<T> res(n, m);
    thread* th[res.n];
    for (int i = 0; i < res.n; ++i)
        th[i] = new thread([&res, this, &a, i](){scalar_mul(res, *this, a, i);});
    for (int i = 0; i < res.n; ++i)
        th[i]->join();
    return res;
}

template<typename T>
void Matrix<T>::subt(Matrix<T>& res, const Matrix<T>& m1, const Matrix<T>& m2, int k) {
    for (int i = 0; i < res.m; ++i)
        res.v[k][i] = m1.v[k][i] - m2.v[k][i];
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& it) {
    if (n != it.n || m != it.m) abort();
    Matrix<T> res(n, m);
    thread* th[res.n];
    for (int i = 0; i < res.n; ++i)
        th[i] = new thread([&res, this, &it, i](){subt(res, *this, it, i);});
    for (int i = 0; i < res.n; ++i)
        th[i]->join();
    return res;   
}

template<typename T>
void Matrix<T>::sum(Matrix<T>& res, const Matrix<T>& m1, const Matrix<T>& m2, int k) {
    for (int i = 0; i < res.m; ++i)
        res.v[k][i] = m1.v[k][i] + m2.v[k][i];
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& it) {
    if (n != it.n || m != it.m) abort();
    Matrix<T> res(n, m);
    thread* th[res.n];
    for (int i = 0; i < res.n; ++i)
        th[i] = new thread([&res, this, &it, i](){sum(res, *this, it, i);});
    for (int i = 0; i < res.n; ++i)
        th[i]->join();
    return res;   
}

template<typename T>
void Matrix<T>::last(float koef, vector<vector<float>>& it, int i, const Matrix<T>& m1) {
    for (int j = 0; j < 2*n; ++j)
        it[i][j] /= koef;
    it[i].erase(it[i].begin(), it[i].begin()+n);
}

template<typename T>
Matrix<float> Matrix<T>::operator!() {
    if (n != m) throw 1;
    vector<vector<float>> vec (n, vector<float> (2*n));
    for (int i = 0; i < n; ++i) 
        for (int j = 0; j < 2*n; ++j) {
            vec[i][j] = v[i][j];
            if (j == (i + n))
                vec[i][j] = 1;
        } 
    float koef;
    for (int i = n - 1; i > 0; --i)
        if (vec[i-1][0] < vec[i][0])
            swap(vec[i], vec[i-1]);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (j != i) {
                koef = float(vec[j][i]) / float(vec[i][i]);
                for (int k = 0; k < 2*n; ++k) 
                    vec[j][k] -= vec[i][k] * koef;
                }
    thread* th[n];
    for (int i = 0; i < n; ++i) {    
        if (vec[i][i] == 0) throw 1;
        koef = vec[i][i];
        th[i] = new thread([koef, &vec, i, this](){last(koef, vec, i, *this);});
    }
    for (int i = 0; i < n; ++i)
        th[i]->join();  
     Matrix<float> inv(move(vec));
     return inv;
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    Matrix<int> m1(cin);
    Matrix<int> m2;
    cout << "<<<< Matrix <<<<" << endl;
    cout << m1;
    m2 = move(m1);
    Matrix<int> m9 = m1;
    Matrix<int> m10 = move(m1);
    cout << "<<<< Assignment <<<<" << endl;
    cout << m2;
    cout << "<<<< Copy <<<<" << endl;
    Matrix<int> m5(move(m1));
    cout << m5;
    Matrix<int> m3({{{0, 0}, 1}, {{0, 1}, 2}, {{1, 1}, 12}});
    cout << "<<<< initializer_list <<<<" << endl;
    cout << m3;
    Matrix<char> m4({{{0, 0}, 'A'}, {{0, 1}, 'B'}, {{1, 0}, 'b'}, {{1, 1}, 'a'}});
    cout << "<<<< initializer_list <<<<" << endl;
    cout << m4;
    return 0;
}
