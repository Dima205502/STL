#include"deque.hpp"

template<typename T>
MySTL::deque<T>::deque() = default;

template<typename T>
MySTL::deque<T>::deque(size_t n, const T& value) {
    if (n == 0) return;

    size_t cnt_rows = (n + SZ_COL - 1) / SZ_COL;

    ptr_arr = new T * [3 * cnt_rows];

    size_t i = 0, ii = 0;

    try {
        for (; i < cnt_rows; ++i) {
            ptr_arr[cnt_rows + i] = reinterpret_cast<T*>(new uint8_t[SZ_COL * sizeof(T)]);

            for (ii = 0; ii < SZ_COL && n - i * SZ_COL - ii; ++ii) {
                new (&ptr_arr[cnt_rows + i][ii]) T(value);
            }
        }
    }
    catch (...) {
        for (size_t j = 0; j < i; ++j) {
            delete[] ptr_arr[cnt_rows + j];
        }

        for (size_t jj = 0; jj < ii; ++jj) {
            ptr_arr[cnt_rows + i][jj].~T();
        }
        delete[] reinterpret_cast<uint8_t*>(ptr_arr[cnt_rows + i]);

        delete[] ptr_arr;
        ptr_arr = nullptr;
        throw;
    }

    sz = n;
    count_rows = 3 * cnt_rows;
    row_front = cnt_rows, col_front = 0;
    row_back = 2 * cnt_rows, col_back = 0;

    if (cnt_rows * SZ_COL != n) {
        row_back = 2 * cnt_rows - 1;
        col_back = n - (cnt_rows - 1) * SZ_COL;
    }
}

template<typename T>
MySTL::deque<T>::deque(const MySTL::deque<T>& other) {
    ptr_arr = new T * [other.count_rows];

    size_t cur_row = other.row_front, cur_col = other.col_front;
    try {
        while (cur_row != other.row_back || cur_col != other.col_back) {

            if (cur_col == 0) {
                ptr_arr[cur_row] = reinterpret_cast<T*>(new uint8_t[SZ_COL * sizeof(T)]);
            }

            new (&ptr_arr[cur_row][cur_col]) T(other.ptr_arr[cur_row][cur_col]);
            cur_col++;

            if (cur_col == SZ_COL) {
                cur_row++;
                cur_col = 0;
            }
        }

    }
    catch (...) {
        for (size_t row = other.row_front; row < cur_row; ++row) {
            delete[] ptr_arr[row];
        }

        for (size_t col = 0; col < cur_col; ++col) {
            ptr_arr[cur_row][col].~T();
        }

        delete[] reinterpret_cast<uint8_t*>(ptr_arr[cur_row]);

        ptr_arr = nullptr;
        throw;
    }

    sz = other.sz;

    count_rows = other.count_rows;

    row_front = other.row_front, col_front = other.col_front;
    row_back = other.row_back, col_back = other.col_back;
}

template<typename T>
size_t MySTL::deque<T>::size() const {
    return sz;
}

template<typename T>
bool MySTL::deque<T>::empty() const {
    return sz == 0;
}

template<typename T>
void MySTL::deque<T>::push_back(const T& value) {
    if (row_back == count_rows) {
        size_t new_count_rows = max((size_t)1,count_rows + row_back - row_front);

        T** new_ptr_arr = new T * [new_count_rows];

        size_t i = 0;
        try {

            for (; i < count_rows; ++i) {
                new_ptr_arr[i] = ptr_arr[i];
            }

            for (; i < new_count_rows; ++i) {
                new_ptr_arr[i] = reinterpret_cast<T*>(new uint8_t[SZ_COL * sizeof(T)]);
            }

        }
        catch (...) {
            for (size_t j = count_rows; j < i; ++j) {
                delete[] reinterpret_cast<uint8_t*>(new_ptr_arr[j]);
            }
            delete[] new_ptr_arr;

            throw;
        }

        delete[] ptr_arr;

        count_rows = new_count_rows;
        ptr_arr = new_ptr_arr;
    }

    try {
        new (&ptr_arr[row_back][col_back]) T(value);
    }
    catch (...) {
        ptr_arr[row_back][col_back].~T();
        throw;
    }

    col_back++;
    if (col_back == SZ_COL) {
        row_back++;
        col_back = 0;
    }

    sz++;
}

template<typename T>
void MySTL::deque<T>::push_front(const T& value) {
    if (sz == 0) {
        ptr_arr = new T * [3];
        for (size_t i = 0; i < 3; ++i) {
            ptr_arr[i] = reinterpret_cast<T*>(new uint8_t[SZ_COL * sizeof(T)]);
        }
        row_front = 1, col_front = 0;
        row_back = 1, row_back = 1;
    }
    if (row_front == 0 && col_front == 0 && sz>0) {
        size_t new_count_rows = count_rows + row_back - row_front;
        T** new_ptr_arr = new T * [new_count_rows];

        size_t i = 0;
        size_t difference = row_back - row_front;

        try {

            for (; i < count_rows; ++i) {
                new_ptr_arr[i + difference] = ptr_arr[i];
            }

            for (i = 0; i < max((size_t)1,difference); ++i) {
                new_ptr_arr[i] = reinterpret_cast<T*>(new uint8_t[SZ_COL * sizeof(T)]);
            }

        }
        catch (...) {
            for (size_t j = 0; j < i; ++j) {
                delete[] reinterpret_cast<uint8_t*>(new_ptr_arr[j]);
            }

            delete[] new_ptr_arr;

            throw;
        }

        delete[] ptr_arr;

        count_rows = new_count_rows;
        ptr_arr = new_ptr_arr;

        row_front += difference;
        row_back += difference;
    }

    col_front--;
    if (col_front == -1) {
        row_front--;
        col_front = SZ_COL - 1;
    }

    try {
        new (&ptr_arr[row_front][col_front]) T(value);
    }
    catch (...) {
        ptr_arr[row_front][col_front].~T();

        col_front++;
        if (col_front == SZ_COL) {
            row_front++;
            col_front = 0;
        }

        throw;
    }

    sz++;
}

template<typename T>
T& MySTL::deque<T>::front() {
    return ptr_arr[row_front][col_front];
}

template<typename T>
T& MySTL::deque<T>::back() {
    if (col_back > 0) return ptr_arr[row_back][col_back - 1];
    else return ptr_arr[row_back - 1][SZ_COL - 1];
}

template<typename T>
const T& MySTL::deque<T>::front() const {
    return ptr_arr[row_front][col_front];
}

template<typename T>
const T& MySTL::deque<T>::back() const {
    if (col_back > 0) return ptr_arr[row_back][col_back - 1];
    else return ptr_arr[row_back - 1][SZ_COL - 1];
}

template<typename T>
T& MySTL::deque<T>::operator[](size_t i) {
    size_t row = row_front + (col_front + i) / SZ_COL;
    size_t col = (col_front + i) % SZ_COL;
    return ptr_arr[row][col];
}

template<typename T>
const T& MySTL::deque<T>::operator[](size_t i) const {
    size_t row = row_front + (col_front + i) / SZ_COL;
    size_t col = (col_front + i) % SZ_COL;
    return ptr_arr[row][col];
}

template<typename T>
T& MySTL::deque<T>::at(size_t i) {
    size_t row = row_front + (col_front + i) / SZ_COL;
    size_t col = (col_front + i) % SZ_COL;
    if (row < row_back || (row == row_back && col < col_back)) return ptr_arr[row][col];

    throw std::out_of_range("incorrect call method .at");
}

template<typename T>
const T& MySTL::deque<T>::at(size_t i) const {
    size_t row = row_front + (col_front + i) / SZ_COL;
    size_t col = (col_front + i) % SZ_COL;
    if (row < row_back || (row == row_back && col < col_back)) return ptr_arr[row][col];

    throw std::out_of_range("incorrect call method .at");
}

template<typename T>
void MySTL::deque<T>::pop_back(){
    if(row_front==row_back && col_front==col_back) return;

    col_back--;
    if(col_back==-1){
        row_back--;
        col_back=SZ_COL-1;
    }

    ptr_arr[row_back][col_back].~T();
}

template<typename T>
void MySTL::deque<T>::pop_front(){
    if(row_front==row_back && col_front==col_back) return;

    ptr_arr[row_front][col_front].~T();

    col_front++;
    if(col_front==SZ_COL){
        row_front++;
        col_front=0;
    }
}

template<typename T>
MySTL::deque<T>::~deque() {
    size_t cur_row = row_front, cur_col = col_front;

    while (cur_row != row_back || cur_col != col_back) {
        ptr_arr[cur_row][cur_col].~T();
        cur_col++;
        if (cur_col == SZ_COL) {
            cur_row++;
            cur_col = 0;
        }
    }

    for (size_t i = 0; i < count_rows; ++i) {
        delete[] reinterpret_cast<uint8_t*>(ptr_arr[i]);
    }

    delete[] ptr_arr;
}
