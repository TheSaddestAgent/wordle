#include <charconv>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

class OneWordVector {
   private:
    size_t capacity_;
    size_t size_;

   public:
    char* data_;
    void push(char x) {
        if (size_ >= capacity_) {
            if (capacity_ == 0) {
                capacity_ = 1;
            } else {
                capacity_ *= 2;
            }
            char* new_data = new char[capacity_];
            for (size_t i = 0; i < size_; i++) {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
        }
        data_[size_] = x;
        size_++;
    }

    OneWordVector() {
        capacity_ = 0;
        size_ = 0;
        data_ = new char[capacity_];
    }

    OneWordVector(size_t size) {
        capacity_ = size;
        size_ = size;
        data_ = new char[capacity_];
    }

    ~OneWordVector() {
        delete[] data_;
    }

    size_t GetSize() {
        return size_;
    }

    void Clear() {
        size_ = 0;
    }

    char* GetData() {
        char* result = new char[size_ + 1];

        for (size_t i = 0; i < size_; ++i) {
            result[i] = data_[i];
        }

        result[size_] = '\0';
        return result;
    }
};

class WordsVector {
   private:
    size_t capacity_;
    size_t size_;

   public:
    char** data_;
    void push(char* x) {
        if (size_ >= capacity_) {
            if (capacity_ == 0) {
                capacity_ = 1;
            } else {
                capacity_ *= 2;
            }
            char** new_data = new char*[capacity_];
            for (size_t i = 0; i < size_; i++) {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
        }
        data_[size_] = x;
        size_++;
    }

    WordsVector() {
        capacity_ = 0;
        size_ = 0;
        data_ = new char*[capacity_];
    }

    WordsVector(size_t size) {
        capacity_ = size;
        size_ = size;
        data_ = new char*[capacity_];
    }

    ~WordsVector() {
        delete[] data_;
    }

    size_t GetSize() {
        return size_;
    }

    char** GetData() {
        char** result = new char*[size_ + 1];

        for (size_t i = 0; i < size_; ++i) {
            result[i] = data_[i];
        }

        return result;
    }
};

namespace {

struct Options {
    int n = 5;
    char* dictionary_path{};
};

int ParseNumberFromChars(char* input) {
    char* begin = input;
    char* end = input;
    while (end != nullptr && *end != '\0' && *end != '\n' && *end != '\t') {
        end++;
    }
    int result{};
    auto [ptr, ec] = std::from_chars(begin, end, result);

    if (ec == std::errc()) {
        return result;
    } else if (ec == std::errc::invalid_argument) {
        std::cerr << "This is not a number.\n";
        return -1;
    } else if (ec == std::errc::result_out_of_range) {
        std::cerr << "This number is larger than an int.\n";
        return -1;
    }
    std::cout << result << '\n';
    return -1;
}

bool AppendOptions(Options& options, char** argv) {
    char* dictionary_path = argv[1];
    int n = ParseNumberFromChars(argv[2]);
    if (n == -1) {
        return false;
    }
    options.dictionary_path = dictionary_path;
    options.n = n;
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Not enough arguments, should be 3\n";
        return 1;
    }

    Options options{};
    if (!AppendOptions(options, argv)) {
        std::cerr << "Couldn't apply options\n";
        return 1;
    };

    FILE* f = std::fopen(options.dictionary_path, "rb");

    if (f == nullptr) {
        std::cerr << "Couldn't open the file: " << options.dictionary_path << '\n';
        return 1;
    }

    const size_t kBufSize = 8192;
    char buf[kBufSize]{};
    int cnt = 0;
    WordsVector words;
    OneWordVector word{};
    while ((cnt = fread(buf, sizeof(char), kBufSize, f)) > 0) {
        if (cnt < kBufSize) {
            buf[cnt] = '\0';
        }

        char* iter = buf;
        while (iter != nullptr && *iter != '\0') {
            if (*iter != '\0' && *iter != '\n' && *iter != ' ' && *iter != '\r' && *iter != '\t') {
                char x = *iter;
                word.push(x);
            } else {
                if (word.GetSize() == options.n) {
                    words.push(word.GetData());
                }
                word.Clear();
            }
            iter++;
        }
    }
    if (word.GetSize() == options.n) {
        words.push(word.GetData());
    }
    if (words.GetSize() == 0) {
        std::cerr << "No words found with given length\n";
        return 1;
    }
    srand(time(nullptr) + clock());
    int rand_ind = rand() % (words.GetSize());
    char* our_word = words.GetData()[rand_ind];
    // std::cout << our_word << '\n';

    const size_t kMaxAttemptsAllowed = 6;
    int attempt = 1;

    while (attempt <= kMaxAttemptsAllowed) {
        attempt++;
        char* guess = new char[options.n];

        for (int i = 0; i < options.n; i++) {
            std::cin >> guess[i];
        }
        OneWordVector res;
        for (int i = 0; i < options.n; i++) {
            if (our_word[i] == guess[i]) {
                res.push('+');
                continue;
            }
            bool found = false;
            for (int j = 0; j < options.n; j++) {
                if (i == j) {
                    continue;
                }

                if (guess[i] == our_word[j]) {
                    res.push('=');
                    found = true;
                    break;
                }
            }

            if (!found) {
                res.push('-');
            }
        }

        char* res_str = res.GetData();
        bool is_win = true;
        for (int i = 0; i < options.n; i++) {
            if (res_str[i] != '+') {
                is_win = false;
            }
        }
        delete[] guess;
        if (is_win) {
            std::cout << "win!\n";
            return 0;
        }
        std::cout << "No, try again\n";
        std::cout << res_str << '\n';
    }

    std::cout << "You lost\n";

    std::fclose(f);

    return 0;
}